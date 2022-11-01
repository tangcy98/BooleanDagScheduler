/**  
 * @file    importdag.h
 * @brief   Import a DAG in other forms
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-25
 * @note    
 */
#ifndef _IMPORTDAG_
#define _IMPORTDAG_
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "booleandag.h"

#define INVWEIGHT   5
#define ANDWEIGHT   5
#define ORWEIGHT    5
#define XORWEIGHT   5
#define XOR3WEIGHT  5
#define MAJWEIGHT   5
#define EDGEWEIGHT  200

using namespace std;

typedef struct Node {
    string name;
    bool leaf = false;       // false: intermediate nodes; true: leaf/root nodes
    bool inv = false;       // false: origin; true: need an inverter
} node;

void CheckFile(bool iFile, const string& filename)
{
    if (!iFile) {
        cerr << "Error: Cannot open file " << filename << "!" << endl;
        exit(2);
    }
}

int CountOperands(const string& rhs) {
    int count = 0;
    for (char i : rhs) {
        if (i == '&' || i == '|' || i == '^') {
            count++;
        }
    }
    return count;
}

void CheckOperand(node& a) {
    if (a.name[0] == '~') {
        a.inv = true;
        a.name = a.name.substr(1);
    }
    if (a.name[0] != 'n') {
        a.leaf = true;
    }
}

BooleanDag *v2booleandag(const std::string &filename)
{
    BooleanDag *G = new BooleanDag;
    ifstream iVerilog;
    iVerilog.open(filename);
    CheckFile((bool) iVerilog, filename);
    std::string line;

    int x_num = 0;
    int y_num = 0;
    int n_shift = 0;
    int n_num = 0;

    while (getline(iVerilog, line)) {
        if (line.find("output") != std::string::npos) {
            y_num = count(line.begin(), line.end(), 'y');
            continue;
        } else if (line.find("input") != std::string::npos) {
            x_num = count(line.begin(), line.end(), 'x');
            continue;
        } else if (line.find("wire") != std::string::npos) {
            n_num = count(line.begin(), line.end(), 'n');
            G->init(n_num);
            int pos = line.find_first_of('n');
            pos++;
            std::string num_string;
            while (line[pos] != ' ' && line[pos] != ',') {
                num_string += line[pos];
                pos++;
            }
            n_shift = atoi(num_string.c_str());
            continue;
        }
        if (line.find("assign") == std::string::npos) continue; // invalid line
        size_t partition = line.find_first_of('=');
        std::string lhs = line.substr(0, partition - 1);
        std::string rhs = line.substr(partition+1);

        int temp = lhs.find_first_of("assign") + 6;
        while (lhs[temp] == ' ') ++temp;
        if (lhs[temp] != 'n') continue; // xi or yi (not ni, skip)

        node operand[3];    // 3 at most
        node dest;
        while (temp < lhs.length() && lhs[temp] != ' ' && lhs[temp] != '=') {
            dest.name += lhs[temp];
            temp++;
        }
        uint destid = atoi(dest.name.c_str()+1) - n_shift;

        int num_operands = CountOperands(rhs);
        // case 1: assign a = (~)b
        if (num_operands == 0) {
            G->addVertice(destid, INVWEIGHT);
            temp = rhs.find_first_not_of(' ');
            while (rhs[temp] != ' ') {
                operand[0].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[0]);   // set inv and name(remove '~' if exist)

            // rhs is a constant
            if (operand[0].name.find("'b") == std::string::npos && operand[0].name[0] == 'n') {
                G->addEdge(atoi(operand[0].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }
        }

        // case 2: 2-input gate
        else if (num_operands == 1) {
            std::string node_shape = "circle";
            if (line.find('&') != std::string::npos) {
                G->addVertice(destid, ANDWEIGHT);
            } else if (line.find('^') != std::string::npos) {
                G->addVertice(destid, XORWEIGHT);
            } else if (line.find('|') != std::string::npos) {
                G->addVertice(destid, ORWEIGHT);
            }

            temp = rhs.find_first_not_of(' ');
            while (rhs[temp] != ' ') {
                operand[0].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[0]);

            temp += 3;
            while (rhs[temp] != ' ' && rhs[temp] != ';') {
                operand[1].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[1]);

            if (operand[0].name.find("'b") == std::string::npos && operand[0].name[0] == 'n') {
                G->addEdge(atoi(operand[0].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }

            if (operand[1].name.find("'b") == std::string::npos && operand[1].name[0] == 'n') {
                G->addEdge(atoi(operand[1].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }
        }

        // case 3: 3 input XOR gate
        else if (num_operands == 2 && line.find('^') != std::string::npos) {
            G->addVertice(destid, XOR3WEIGHT);
            temp = rhs.find_first_not_of(' ');
            while (rhs[temp] != ' ') {
                operand[0].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[0]);

            temp += 3;
            while (rhs[temp] != ' ' && rhs[temp] != ';') {
                operand[1].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[1]);

            temp += 3;
            while (rhs[temp] != ' ' && rhs[temp] != ';') {
                operand[2].name += rhs[temp];
                temp++;
            }

            // the third one is a constant
            if (operand[0].name.find("'b") == std::string::npos && operand[0].name[0] == 'n') {
                G->addEdge(atoi(operand[0].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }

            if (operand[1].name.find("'b") == std::string::npos && operand[1].name[0] == 'n') {
                G->addEdge(atoi(operand[1].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }
            if (operand[2].name.find("'b") == std::string::npos && operand[2].name[0] == 'n') {
                G->addEdge(atoi(operand[2].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }
        }
        // Otherwise: majority gate, doesn't need conversion
        else {
            G->addVertice(destid, MAJWEIGHT);
            int temp = 0;
            while (rhs[temp] == ' ' || rhs[temp] == '(') {
                temp++;
            }
            while (rhs[temp] != ' ') {
                operand[0].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[0]);

            while (rhs[temp] == ' ' || rhs[temp] == '&') {
                temp++;
            }
            while (rhs[temp] != ' ') {
                operand[1].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[1]);

            while (rhs[temp] != '&') {
                temp++;
            }
            temp+=2;
            while (rhs[temp] != ' ') {
                operand[2].name += rhs[temp];
                temp++;
            }
            CheckOperand(operand[2]);

            if (operand[0].name.find("'b") == std::string::npos && operand[0].name[0] == 'n') {
                G->addEdge(atoi(operand[0].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }

            if (operand[1].name.find("'b") == std::string::npos && operand[1].name[0] == 'n') {
                G->addEdge(atoi(operand[1].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }
            if (operand[2].name.find("'b") == std::string::npos && operand[2].name[0] == 'n') {
                G->addEdge(atoi(operand[2].name.c_str()+1) - n_shift, destid, EDGEWEIGHT);
            }

        }
    }

    iVerilog.close();
    G->linkDAG();
    return G;
}


#endif