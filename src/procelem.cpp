/**  
 * @file    procelem.cpp
 * @brief   Processing Element Implementation
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */


#include "procelem.h"



StageProcessors::StageProcessors()
{
    pnum = 0;
    PE = NULL;
    makespan = 0;
    next = NULL;
    prior = NULL;
}


int StageProcessors::init(uint pn)
{
    if (PE) {
        delete[] PE;
    }
    pnum = pn;
    schedule.clear();
    PE = new _PE[pnum];
    for (uint i = 0; i < pn; ++i) {
        PE[i].id = i;
    }
    makespan = 0;
    next = NULL;
    prior = NULL;
    return 1;
}

int StageProcessors::clean()
{
    if (PE) {
        delete[] PE;
    }
    pnum = 0;
    schedule.clear();
    PE = NULL;
    makespan = 0;
    next = NULL;
    prior = NULL;
    inst.clear();
    return 1;
}

inline InstructionNameSpace::InstructionType Vtype2Instop(VerticeType vt)
{
    switch (vt) {
        case VINV: {
            return InstructionNameSpace::INV;
            break;
        }
        case VAND: {
            return InstructionNameSpace::AND;
            break;
        }
        case VOR: {
            return InstructionNameSpace::OR;
            break;
        }
        case VXOR: {
            return InstructionNameSpace::XOR;
            break;
        }
        case VXOR3: {
            return InstructionNameSpace::XOR3;
            break;
        }
        case VMAJ: {
            return InstructionNameSpace::MAJ;
            break;
        }
        default: {
            return InstructionNameSpace::NOP;
            break;
        }
    }
}

int StageProcessors::checkPlaceable(BooleanDag *G, uint peid, uint taskid)
{
    Vertice *v = G->getvertice(taskid);
    uint prednum = v->prednum;
    _PE* pe = PE + peid;

    int load = 0;
    int copy = 0;

    for (uint i = 0; i < prednum; ++i) {
        if (pe->cache.find(v->predecessors[i]->src->id) != pe->cache.end()) {
            ;
        }
        else if (getAssignmentByTask(taskid)) {
            copy++;
        }
        else {
            load++;
        }
    }
    if (pe->overwriteflag + load > BLOCKROW || pe->cache.size() + copy + load + 1 > BLOCKROW) {
        return 0;
    }
    return 1;
}

// TODO: Check conflict with existing assignment
int StageProcessors::assignTask(BooleanDag* G, uint taskid, uint PEid, int starttime, int finishtime)
{
    Assignment a;
    a.pid = PEid;
    a.tid = taskid;
    a.starttime = starttime;
    a.finishtime = finishtime;
    schedule.insert(std::make_pair(taskid, a));
    makespan = makespan > finishtime ? makespan : finishtime;

    _PE *pe = PE+PEid;
    pe->eft = pe->eft > finishtime ? pe->eft : finishtime;
    // TODO: Possible insertion in the future
    // for (std::vector<Assignment*>::iterator iter = pe->tasks.begin(); iter != pe->tasks.end(); ++iter) {
    //     if (starttime < (*iter)->starttime) {
    //         pe->tasks.insert(iter, &schedule[taskid]);
    //         return 1;
    //     }
    // }
    pe->tasks.push_back(&schedule[taskid]);


    Vertice *v = G->getvertice(taskid);
    uint prednum = v->prednum;
    InstructionNameSpace::Instruction opinst;
    opinst.taskid = taskid;
    opinst.op = Vtype2Instop(v->type);
    for (int i = 0; i < 3; ++i) {
        if (v->invflags&(1<<i)) {
            opinst.invflag[i] = true;
        }
    }
    for (uint i = 0u; i < prednum; ++i) {
        Edge *prede = v->predecessors[i];
        Vertice *predv = prede->src;
        uint predid = predv->id;
        Assignment* predassignment;
        if (pe->cache.find(predid) == pe->cache.end()) {
            // Did not find local data
            // need copy / load and assign new row
            predassignment = getAssignmentByTask(predid);
            if (predassignment) {
                // in mesh
                InstructionNameSpace::Instruction copyinst;
                uint srcpeid = predassignment->pid;
                uint srcidx = (PE+srcpeid)->cache[predassignment->tid];
                opinst.src[i] = MESHADDR(PEid, pe->smallestfreeidx);
                pe->cache.insert(std::make_pair(v->predecessors[i]->src->id, pe->smallestfreeidx));
                pe->line[pe->smallestfreeidx] = v->predecessors[i]->src->id;
                copyinst.taskid = taskid;
                copyinst.op = InstructionNameSpace::COPY;
                copyinst.dest = MESHADDR(PEid, pe->smallestfreeidx);
                copyinst.src[0] = MESHADDR(srcpeid, srcidx);

                inst.push_back(copyinst);
                while (++(pe->smallestfreeidx) < BLOCKROW && pe->line[pe->smallestfreeidx] < UINT_MAX);  // next smallestfreeidx

                // std::vector<InstructionNameSpace::Instruction>::iterator it = inst.begin();
                // while (it != inst.end()) {
                //     if (it->taskid == predv->id && it->op >= InstructionNameSpace::INV) {
                //         break;  // find pred op position
                //     }
                //     ++it;
                // }
                // if (it == inst.end()) {
                //     exit(-1);
                // }
                // inst.insert(++it, copyinst);
            }
            else {
                if (prior) {
                    predassignment = prior->getAssignmentByTask(predid);
                }
                if (predassignment && prior->getLine(predid)>=getOverwritepos(predassignment->pid)) {
                    uint srcpeid = predassignment->pid;
                    uint srcidx = (prior->PE+srcpeid)->cache[predid];   // no need to use getLine
                    // if (srcpeid == pe->id) {
                    //     // in this pe, no need to copy
                    //     opinst.src[i] = MESHADDR(PEid, srcidx);
                    //     pe->cache.insert(std::make_pair(predassignment->tid, srcidx));
                    //     pe->line[srcidx] = predassignment->tid;
                    //     if (pe->smallestfreeidx == srcidx) {
                    //         while (++(pe->smallestfreeidx) < BLOCKROW && pe->line[pe->smallestfreeidx] < UINT_MAX);  // next smallestfreeidx
                    //     }
                    // }
                    // else {
                        // need copy
                    InstructionNameSpace::Instruction copyinst;
                    opinst.src[i] = MESHADDR(PEid, pe->smallestfreeidx);
                    pe->cache.insert(std::make_pair(v->predecessors[i]->src->id, pe->smallestfreeidx));
                    pe->line[pe->smallestfreeidx] = v->predecessors[i]->src->id;
                    copyinst.taskid = taskid;
                    copyinst.op = InstructionNameSpace::COPY;
                    copyinst.dest = MESHADDR(PEid, pe->smallestfreeidx);
                    copyinst.src[0] = MESHADDR(srcpeid, srcidx);
                    inst.push_back(copyinst);
                    while (++(pe->smallestfreeidx) < BLOCKROW && pe->line[pe->smallestfreeidx] < UINT_MAX);  // next smallestfreeidx
                    // }

                    // remove STORE
                    bool rmv = true;
                    for (uint k = 0; k < predv->succnum; ++k) {
                        if (predv->successors[k]->dest->id != taskid && G->getPriority(predv->successors[k]->dest->id) <= G->getPriority(taskid)) {
                            rmv = false;
                        }
                    }
                    if (rmv) {
                        prior->removeStoreInst(predv->id);
                    }
                }
                else {
                    InstructionNameSpace::Instruction loadinst;
                    opinst.src[i] = MESHADDR(PEid, pe->overwriteflag);
                    pe->cache.insert(std::make_pair(v->predecessors[i]->src->id, pe->overwriteflag));
                    pe->line[pe->overwriteflag] = v->predecessors[i]->src->id;
                    loadinst.taskid = taskid;
                    loadinst.op = InstructionNameSpace::LOAD;
                    loadinst.dest = MESHADDR(PEid,pe->overwriteflag);
                    loadinst.src[0] = MESHADDR(pnum, predid);   //TODO: ADD MEM ADDR

                    inst.insert(inst.begin(), loadinst);
                    if (pe->smallestfreeidx == pe->overwriteflag) {
                        ++(pe->smallestfreeidx);
                        pe->overwriteflag = pe->smallestfreeidx;
                    }
                    else {
                        ++(pe->overwriteflag);
                    }
                }
                // in memory

                // std::vector<InstructionNameSpace::Instruction>::iterator it = inst.begin();
                // while (it != inst.end() && it->op != InstructionNameSpace::LOAD) ++it;
                // inst.insert(it, loadinst);
            }
            pe->overwriteflag = pe->overwriteflag > pe->smallestfreeidx ? pe->overwriteflag : pe->smallestfreeidx;
        }
        else {
            opinst.src[i] = MESHADDR(PEid, pe->cache[predid]);
        }
    }

    opinst.dest = MESHADDR(PEid, pe->smallestfreeidx);
    inst.push_back(opinst);
    pe->cache.insert(std::make_pair(taskid, pe->smallestfreeidx));
    pe->line[pe->smallestfreeidx] = taskid;
    while (++(pe->smallestfreeidx) < BLOCKROW && pe->line[pe->smallestfreeidx] < UINT_MAX);  // next smallestfreeidx
    pe->overwriteflag = pe->overwriteflag > pe->smallestfreeidx ? pe->overwriteflag : pe->smallestfreeidx;

    return 1;
}

// int checkAllFinish;

int StageProcessors::releaseMem(BooleanDag* g, uint taskid, int *priority)
{
    Vertice *v = g->getvertice(taskid);
    uint prednum = v->prednum;
    int prio = priority[taskid];
    for (uint i = 0u; i < prednum; ++i) {
        Vertice *pred = v->predecessors[i]->src;
        uint succnum = pred->succnum;
        bool releaseable = true;
        for (uint j = 0u; j < succnum; ++j) {
            // still have vertice not assigned
            if (pred->successors[j]->dest->id != taskid && priority[pred->successors[j]->dest->id] <= prio) {
                releaseable = false;
            }
        }
        if (releaseable) {
            for (uint j = 0u; j < pnum; ++j) {
                std::map<uint, uint>::iterator it = PE[j].cache.find(pred->id);
                if (it != PE[j].cache.end()) {
                    PE[j].line[it->second] = UINT_MAX;
                    if (it->second < PE[j].smallestfreeidx) {
                        PE[j].smallestfreeidx = it->second;
                    }
                    PE[j].cache.erase(it);
                }
            }
        }
    }
    return 1;
}


int StageProcessors::releaseMem(BooleanDag* g, uint taskid, bool *assigned)
{
    Vertice *v = g->getvertice(taskid);
    uint prednum = v->prednum;
    for (uint i = 0u; i < prednum; ++i) {
        Vertice *pred = v->predecessors[i]->src;
        uint succnum = pred->succnum;
        bool releaseable = true;
        for (uint j = 0u; j < succnum; ++j) {
            // still have vertice not assigned
            if (assigned[pred->successors[j]->dest->id] == false) {
                releaseable = false;
            }
        }
        if (releaseable) {
            for (uint j = 0u; j < pnum; ++j) {
                std::map<uint, uint>::iterator it = PE[j].cache.find(pred->id);
                if (it != PE[j].cache.end()) {
                    PE[j].line[it->second] = UINT_MAX;
                    if (it->second < PE[j].smallestfreeidx) {
                        PE[j].smallestfreeidx = it->second;
                    }
                    PE[j].cache.erase(it);
                }
            }
        }
    }
    return 1;
}


int StageProcessors::assignFinish()
{
    _PE* p;
    for (uint i = 0u; i < pnum; ++i) {
        p = PE+i;
        std::vector<Assignment*>::iterator it;
        for (it = p->tasks.begin(); it != p->tasks.end(); ++it) {
            std::map<uint, uint>::iterator mp;
            mp = p->cache.find((*it)->tid);
            if (mp != p->cache.end()) {
                InstructionNameSpace::Instruction storeinst;
                storeinst.taskid = mp->first;
                storeinst.op = InstructionNameSpace::STORE;
                storeinst.src[0] = MESHADDR(i,mp->second);
                storeinst.dest = MESHADDR(pnum, mp->first);
                inst.push_back(storeinst);
            }
        }
    }
    return 1;
}


/* Setters */
int StageProcessors::removeStoreInst(uint taskid)
{
    std::vector<InstructionNameSpace::Instruction>::iterator it;
    for (it = inst.begin(); it != inst.end(); ++it) {
        if (it->op == InstructionNameSpace::STORE && it->taskid == taskid) {
            inst.erase(it);
            // it->op = InstructionNameSpace::NOP;
            return 1;
        }
    }
    return 0;
}


/* Visitors */
int StageProcessors::getmakespan()
{
    return makespan;
}

uint StageProcessors::getpnum()
{
    return pnum;
}


_PE* StageProcessors::getPE(uint id)
{
    return PE+id;
}

StageProcessors* StageProcessors::getNext()
{
    return next;
}

StageProcessors* StageProcessors::getPrior()
{
    return prior;
}


uint StageProcessors::getPEByTask(uint taskid)
{
    std::map<uint, Assignment>::iterator it = schedule.find(taskid);
    if (it == schedule.end()) {
        return UINT_MAX;
    }
    return schedule.at(taskid).pid;
}

uint StageProcessors::getTaskByPE(uint PEid, int n)
{
    if (!PE) {
        exit(-1);
    }
    _PE * pe;
    pe = PE+PEid;
    if (n >= 0 && n < pe->tasks.size()) {
        return pe->tasks[n]->tid;
    }
    return pe->tasks.at(pe->tasks.size()-1)->tid;
}

uint StageProcessors::getLine(uint taskid)
{
    uint peid = getPEByTask(taskid);
    if (peid < UINT_MAX) {
        return (PE+peid)->cache[taskid];
    }
    return BLOCKROW;
}

uint StageProcessors::getOverwritepos(uint peid)
{
    return (PE+peid)->overwriteflag;
}

Assignment* StageProcessors::getAssignmentByTask(uint taskid)
{
    std::map<uint, Assignment>::iterator it = schedule.find(taskid);

    if (it != schedule.end()) {
        return &it->second;
    }
    return NULL;
}

void StageProcessors::printScheduleByTasks()
{
    printf("--------Schedule of each Task BEGIN--------\n");

    uint size = schedule.size();
    Assignment *a;
    for (uint i = 0u; i < size; ++i) {
        a = getAssignmentByTask(i);
        printf("    Task %u is assigned to PE No.%u. Start at %d. End at %d.\n", a->tid, a->pid, a->starttime, a->finishtime);
    }
    printf("--------Schedule of each Task END--------\n");
}

void StageProcessors::printScheduleByPE()
{
    _PE *pe;
    std::vector<Assignment*>::iterator ait;
    int tasknum;
    printf("--------Schedule of each PE BEGIN--------\n");
    for (uint i = 0u; i < pnum; ++i) {
        pe = PE+i;
        printf("No.%u PE's schedule:\n", i);
        for (ait = pe->tasks.begin(); ait != pe->tasks.end(); ++ait) {
            printf("\t[Task No.%u] Start at %d. End at %d.\n", (*ait)->tid, (*ait)->starttime, (*ait)->finishtime);
        }
    }
    printf("--------Schedule of each PE END--------\n");
}

void StageProcessors::printInstructions(int stage)
{
    std::vector<InstructionNameSpace::Instruction>::iterator it;
    printf("Stage %d:\n", stage);
    for (it = inst.begin(); it < inst.end(); ++it) {
        int pe[4] = {it->src[0]/BLOCKROW, it->src[1]/BLOCKROW,it->src[2]/BLOCKROW,it->dest/BLOCKROW};
        printf("\t%-8s\t%s%d[%d] %s%d[%d] %s%d[%d] %d[%d]", \
            InstructionNameSpace::instname[(int)(it->op)], \
            it->invflag[0]?"~":"", it->src[0], pe[0], \
            it->invflag[1]?"~":"", it->src[1], pe[1], \
            it->invflag[2]?"~":"", it->src[2], pe[2], \
            it->dest, pe[3]);
        if (it->op >= InstructionNameSpace::INV) {
            printf("\t// Task.%d finished", it->taskid);
        }
        printf("\n");
    }
    printf("Stage finish.\n\n");
}