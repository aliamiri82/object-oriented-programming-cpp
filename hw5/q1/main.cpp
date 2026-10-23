#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


class FIFO {
    public:
    string name;
    int n;
    int fill;
    int chandom;
    int saf;
    int arr[10000];
};

int main() {
    string line;
    int n;
    vector<FIFO> fifos;
    while (getline(cin, line)) {
        FIFO f;
        istringstream iss(line);
        string word;
        vector<string> tokens;
        while (iss >> word) {tokens.push_back(word);}
        //create FIFO <name> with size <n>
        if (tokens[0] == "create" && tokens[1] == "FIFO" && tokens[3] == "with" && tokens[4] == "size") {
            bool found = false;
            for (int i = 0; i < fifos.size(); i++) {
                if (fifos[i].name == tokens[2]) {
                    found = true;
                    cout << "Error! this name is already exist." << endl;
                }
            }
            if (found) continue;
            f.name = tokens[2];
            f.n = stoi(tokens[5]);
            f.fill = 0;
            f.chandom = 0;
            f.saf = 0;
            fifos.push_back(f);
            cout << "FIFO " << f.name << " created." << endl;
        }
        //push <number> to FIFO <name>
        if (tokens[0] == "push" && tokens[2] == "to" && tokens[3] == "FIFO") {
            bool found = false;
            int index;
            for (int i = 0; i < fifos.size(); i++) {
                if (fifos[i].name == tokens[4]) {
                    found = true;
                    index = i;
                }
            }
            if (!found) {
                cout << "FIFO doesn't exist." << endl;
                continue;
            }
            if (fifos[index].fill == fifos[index].n) {
                cout << "FIFO " << fifos[index].name << " is full." << endl;
                continue;
            }
            fifos[index].arr[fifos[index].chandom] = stoi(tokens[1]);
            fifos[index].fill++;
            fifos[index].chandom = (fifos[index].chandom + 1) % fifos[index].n;
            cout << tokens[1] << " added successfully!" << endl;
        }
        ///pop from FIFO <name>
        if (tokens[0] == "pop" && tokens[1] == "from" && tokens[2] == "FIFO") {
            bool found = false;
            int index;
            for (int i = 0; i < fifos.size(); i++) {
                if (fifos[i].name == tokens[3]) {
                    found = true;
                    index = i;
                }
            }
            if (!found) {
                cout << "FIFO doesn't exist." << endl;
                continue;
            }
            if (fifos[index].fill == 0) {
                cout << "FIFO " << fifos[index].name << " is empty." << endl;
                continue;
            }
            fifos[index].fill--;
            cout << fifos[index].arr[fifos[index].saf] << endl;
            fifos[index].saf = (fifos[index].saf + 1) % fifos[index].n;

        }
        //delete FIFO <name>
        if (tokens[0] == "delete" && tokens[1] == "FIFO") {
            bool found = false;
            int index;
            for (int i = 0; i < fifos.size(); i++) {
                if (fifos[i].name == tokens[2]) {
                    found = true;
                    cout << "FIFO " << fifos[i].name << " deleted." << endl;
                    fifos.erase(fifos.begin() + i);
                    index = i;
                }
            }
            if (!found) {
                cout << "FIFO doesn't exist." << endl;
                continue;
            }
        }
        //print <i> from FIFO <name>
        if (tokens[0] == "print" && tokens[2] == "from") {
            bool found = false;
            int index;
            for (int i = 0; i < fifos.size(); i++) {
                if (fifos[i].name == tokens[4]) {
                    found = true;
                    //fifos.erase(fifos.begin() + i);
                    index = i;
                }
            }
            if (!found) {
                cout << "FIFO doesn't exist." << endl;
                continue;
            }
            cout << fifos[index].arr[   stoi(tokens[1])  %  fifos[index].n ] << endl;
            continue;
        }
        if (line == "end")
            break;


    }
    return 0;
}
