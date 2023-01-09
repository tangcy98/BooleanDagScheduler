#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <map>
#include <algorithm>
#include <vector>


using namespace std;

void newinv(string &s, int n1, int n2, char n1type)
{
    s = "  assign n";
    s += to_string(n2);
    s += " = ~";
    s += n1type;
    s += to_string(n1);
    s += " ;";
}

void newwire(string &s, int start, int end)
{
    int pos = s.find(';');
    string insert = "";
    for (int i = start; i < end; ++i) {
        insert += ", n" + to_string(i) + ' ';
    }
    s.insert(pos, insert);
}

int main(int argc, char* argv[])
{
    const char *inputfile = argv[1];
    const char *outputfile = argv[2];

    ifstream iVerilog;
    ofstream oVerilog;

    map<int, int> invmap;

    iVerilog.open(inputfile);
    oVerilog.open(outputfile);

    string readline;
    string writeline;

    uint x_num = 0;
    uint y_num = 0;
    uint n_shift = 0;
    uint n_num = 0;

    int curmax = 0;

    string wireline;
    vector<string> assignlines;
    vector<string> yassignlines;

    while (getline(iVerilog, readline)) {
        if (readline.find("endmodule") != std::string::npos) {
            newwire(wireline, n_shift+n_num, curmax);
            oVerilog << wireline << endl;
            vector<string>::iterator it;
            for (it = assignlines.begin(); it != assignlines.end(); ++it) {
                oVerilog << *it << endl;
            }
            for (it = yassignlines.begin(); it != yassignlines.end(); ++it) {
                oVerilog << *it << endl;
            }
            oVerilog << readline << endl;
        } else if (readline.find("module") != std::string::npos) {
            oVerilog << readline << endl;
        } else if (readline.find("output") != std::string::npos) {
            oVerilog << readline << endl;
            y_num = count(readline.begin(), readline.end(), 'y');
            continue;
        } else if (readline.find("input") != std::string::npos) {
            oVerilog << readline << endl;
            x_num = count(readline.begin(), readline.end(), 'x');
            continue;
        } else if (readline.find("wire") != std::string::npos) {
            n_num = count(readline.begin(), readline.end(), 'n');

            string::size_type pos = readline.find_first_of('n');
            pos++;
            std::string num_string;
            while (readline[pos] != ' ' && readline[pos] != ',') {
                num_string += readline[pos];
                pos++;
            }
            n_shift = atoi(num_string.c_str());
            curmax = n_num + n_shift;
            wireline = readline;
            continue;
        }

        if (readline.find("assign") == std::string::npos) continue; // invalid line

        string::size_type pos, movepos;
        while ((pos=readline.find("~")) != std::string::npos) {
            int n1, n2;
            char n1type = readline[pos+1];
            movepos = pos + 2;
            n1 = 0;
            while (readline[movepos] <= '9' && readline[movepos] >= '0') {
                n1 = n1*10 + readline[movepos++] - '0';
            }
            n1 = n1type=='n' ? n1 : -n1;
            map<int, int>::iterator it = invmap.find(n1);
            if (it != invmap.end()) {
                n2 = it->second;
            }
            else {
                n2 = curmax++;
                invmap.insert(make_pair(n1, n2));
                string newline;
                n1 = n1type=='n' ? n1 : -n1;
                newinv(newline, n1, n2, n1type);
                assignlines.push_back(newline);
            }

            readline.replace(pos, movepos-pos, 'n'+to_string(n2));
        }
        if (readline.find("assign y") != std::string::npos) {
            yassignlines.push_back(readline);
        }
        else {
            assignlines.push_back(readline);
        }
    }

    iVerilog.close();
    oVerilog.close();

    return 0;
}