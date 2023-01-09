#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <map>
#include <algorithm>
#include <vector>


using namespace std;

int main(int argc, char* argv[])
{
    const char *inputfile = argv[1];
    const char *outputfile = argv[2];

    ifstream iVerilog;
    ofstream oVerilog;

    map<int, int> exchangemap;
    map<int, int>::iterator it;

    iVerilog.open(inputfile);
    oVerilog.open(outputfile);

    string readline;
    string writeline;

    uint x_num = 0;
    uint y_num = 0;
    uint n_shift = 0;
    uint n_num = 0;
    int bottom, top;
    string::size_type pos;

    while (getline(iVerilog, readline)) {
        if (readline.find("output") != std::string::npos) {
            y_num = count(readline.begin(), readline.end(), 'y');
            continue;
        } else if (readline.find("input") != std::string::npos) {
            x_num = count(readline.begin(), readline.end(), 'x');
            continue;
        } else if (readline.find("wire") != std::string::npos) {
            n_num = count(readline.begin(), readline.end(), 'n');
            pos = readline.find_first_of('n');
            pos++;
            n_shift = 0;
            while (readline[pos] <= '9' && readline[pos] >= '0') {
                n_shift = n_shift * 10 + readline[pos++] - '0';
            }
            bottom = n_shift;
            top = n_shift + y_num;
            continue;
        } else if (readline.find("assign y") == std::string::npos) continue; // invalid line

        pos = readline.find("y") + 1;

        int p1, p2;
        p1 = 0;
        p2 = 0;
        while (readline[pos] <= '9' && readline[pos] >= '0') {
            p2 = p2 * 10 + readline[pos++] - '0';
        }

        while (readline[pos++] != 'n');
        // now readline[pos-1] = 'n'

        while (readline[pos] <= '9' && readline[pos] >= '0') {
            p1 = p1 * 10 + readline[pos++] - '0';
        }
        p2 += bottom;

        exchangemap[p1] = p2;
    }

    it = exchangemap.lower_bound(top);

    for (int i = bottom; i < top; ++i) {
        if (exchangemap.find(i) == exchangemap.end()) {
            exchangemap[i] = it->first;
            ++it;
        }
    }
    iVerilog.clear();
    iVerilog.seekg(0,ios::beg);

    int num;
    while (getline(iVerilog, readline)) {
        if (readline.find("assign") == std::string::npos) {
            oVerilog << readline << endl;
            continue;
        }
        writeline = "";
        pos = 0;
        while (readline[pos]) {
            writeline += readline[pos];
            if (readline[pos++] == 'n') {
                num = 0;
                while (readline[pos] <= '9' && readline[pos] >= '0') {
                    num = num * 10 + readline[pos++] - '0';
                }
                if (num) {
                    it = exchangemap.find(num);
                    if (it != exchangemap.end()) {
                        writeline += to_string(it->second);
                    }
                    else {
                        writeline += to_string(num);
                    }
                }
            }
        }
        oVerilog << writeline << endl;
    }

    iVerilog.close();
    oVerilog.close();

    return 0;
}