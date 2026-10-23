#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <optional>
using namespace std;
///////Global
bool setup = true;
int counter = 0;
int tv=0,yakh=0,bokh=0,koler=0;



class accesspoint {
public:
    optional <vector<string>> connected_IPs;
    optional <vector<string>> ban_IPs;
    optional <int> max_client;
    string username;
    string password;
    bool heater=false,television=false,air_conditioner=false,refrigerator=false;
    bool heater_onoff=false,television_onoff=false,air_conditioner_onoff=false,refrigerator_onoff=false;

    void create_accesspoint(string input) {
        istringstream iss(input);
        string word;
        vector<string> tokens;
        while (iss >> word) { tokens.push_back(word);}
        if (tokens.size() >= 5 && tokens[0] == "create" && tokens[1] == "Access" && tokens[2] == "point") {
            username = tokens[3];
            password = tokens[4];
            if (password == "heater" || password == "air_conditioner" || password == "refrigerator" || password == "television") {
                password = "empty";
            }
            //if (password.length() < 8 && password != "empty") {cout << "invalid password" << endl; return;}
            for (int i = 4; i < tokens.size(); i++) {
                if (tokens[i] == "heater") {heater = true; counter++;}
                if (tokens[i] == "air_conditioner") {air_conditioner = true; counter++;}
                if (tokens[i] == "refrigerator") {refrigerator = true; counter++;}
                if (tokens[i] == "television") {television = true; counter++;}
            }
            cout << "access point created successfully"  << endl;
        } else  cerr << "invalid command" << endl; return;
    };

    void limitaion_client(vector<string> tokens) {

            if (stoi(tokens[5]) >= 0) {max_client = stoi(tokens[5]); cout << "limitation set" << endl; return;}
            else {cout << "number of clients can not be negative" << endl; return;}

    };

    void limitaion_IP(vector<string> tokens) {
        if (!ban_IPs.has_value()) {
            ban_IPs = vector<string>{};
        }
        ban_IPs->push_back(tokens[2]);
        cout << "limitation set" << endl; return;
    };

    void connect_client(string input) {
        if (!connected_IPs.has_value()) {
            connected_IPs = vector<string>{};
        }
        if (!ban_IPs.has_value()) {
            ban_IPs = vector<string>{};
        }
        if (max_client.has_value()) {
            if (connected_IPs->size() == max_client) {cout <<"can not connect because of limitations" << endl; return;}
        }
        for (int i = 0; i < ban_IPs->size(); i++) {
            if (input == ban_IPs->at(i)) {cout <<"can not connect because of limitations" << endl; return;}
        }

        connected_IPs->push_back(input);
        cout << "client connected successfully" << endl; return;
    };

    void show_clients_IP() {
        if (!connected_IPs.has_value()) {
            cout << "no client connected yet" << endl; return;
        }
        if (connected_IPs->size() == 0) {
            cout << "no client connected yet" << endl; return;
        }
        for (int i = 0; i < connected_IPs->size(); i++) {
            cout << "client" << i + 1 << " " << connected_IPs->at(i) << endl;
        }
    };


    void delete_IP(string input) {
        if (!connected_IPs.has_value()) {
            cout << "this IP does not exist in the access point " << username << endl; return;
        }
        if (connected_IPs->size() == 0) {
            cout << "this IP does not exist in the access point " << username << endl; return;
        }
        for (int i = 0; i < connected_IPs->size(); i++) {
            if (input == connected_IPs->at(i)) {
                connected_IPs->erase(connected_IPs->begin() + i);
                cout << "client deleted" << endl; return;
            }
        }
        cout << "this IP does not exist in the access point " << username << endl;
    };


    void dissconnect_IP(string input) {
        if (!connected_IPs.has_value()) {
            cout << "no such client exist in this access point" <<endl; return;
        }
        if (connected_IPs->size() == 0) {
            cout << "no such client exist in this access point" <<endl; return;
        }
        for (int i = 0; i < connected_IPs->size(); i++) {
            if (input == connected_IPs->at(i)) {
                connected_IPs->erase(connected_IPs->begin() + i);
                cout << "client disconnected" << endl; return;
            }
        }
        cout << "no such client exist in this access point" << endl;
    };


    void control(string IP, string on_off){
        if (!connected_IPs.has_value()) {
            cout << "no such device found" << endl; return;
        }
        bool found = false;
        for (int i = 0; i < connected_IPs->size(); i++) {
            if (IP == connected_IPs->at(i)) {
                found = true;


                if (on_off == "000") {if (heater_onoff == false) {
                    bokh++; tv=0;yakh=0;koler=0;
                    if (bokh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"heater was off"<<endl;
                } else {
                    bokh++; tv=0;yakh=0;koler=0;
                    if (bokh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    heater_onoff = false; cout <<"heater turns off"<<endl;


                }}

                if (on_off == "001") {if (heater_onoff == false) {
                    bokh++; tv=0;yakh=0;koler=0;
                    if (bokh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    heater_onoff = true; cout <<"heater turns on"<<endl;
                } else {
                    bokh++; tv=0;yakh=0;koler=0;
                    if (bokh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"heater was on"<<endl;
                }}




                if (on_off == "010") {if (television_onoff == false) {
                    tv++; yakh=0;koler=0;bokh=0;
                    if (tv == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);

                        return;
                    }
                    cout <<"television was off"<<endl;
                } else {
                    tv++; yakh=0;koler=0;bokh=0;
                    if (tv == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    television_onoff = false; cout <<"television turns off"<<endl;
                }}
                if (on_off == "011") {if (television_onoff == false) {
                    tv++; yakh=0;koler=0;bokh=0;
                    if (tv == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    television_onoff = true; cout <<"television turns on"<<endl;
                } else {
                    tv++; yakh=0;koler=0;bokh=0;
                    if (tv == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"television was on"<<endl;
                }}



                if (on_off == "100") {if (air_conditioner_onoff == false) {
                    koler++; tv=0;yakh=0;bokh=0;
                    if (koler == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"air_conditioner was off"<<endl;
                } else {
                    koler++; tv=0;yakh=0;bokh=0;
                    if (koler == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    air_conditioner_onoff = false; cout <<"air_conditioner turns off"<<endl;
                }}
                if (on_off == "101") {if (air_conditioner_onoff == false) {
                    koler++; tv=0;yakh=0;bokh=0;
                    if (koler == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    air_conditioner_onoff = true; cout <<"air_conditioner turns on"<<endl;
                } else {
                    koler++; tv=0;yakh=0;bokh=0;
                    if (koler == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"air_conditioner was on"<<endl;
                }}





                if (on_off == "110") {if (refrigerator_onoff == false) {
                    yakh++; tv=0;koler=0;bokh=0;
                    if (yakh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"refrigerator was off"<<endl;
                } else {
                    yakh++; tv=0;koler=0;bokh=0;
                    if (yakh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    refrigerator_onoff = false; cout <<"refrigerator turns off"<<endl;
                }}
                if (on_off == "111") {if (refrigerator_onoff == false) {
                    yakh++; tv=0;koler=0;bokh=0;
                    if (yakh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    refrigerator_onoff = true; cout <<"refrigerator turns on"<<endl;
                } else {
                    yakh++; tv=0;koler=0;bokh=0;
                    if (yakh == 5) {
                        cout << "client " << IP << " banned due to spam" << endl;
                        connected_IPs->erase(connected_IPs->begin() + i);
                        if (!ban_IPs.has_value()) {
                            ban_IPs = vector<string>{};
                        }
                        ban_IPs->push_back(IP);
                        return;
                    }
                    cout <<"refrigerator was on"<<endl;
                }}
            }
        }
        if (!found) {cout << "no such device found" << endl; return;}
    };

    };



    int main() {
        string line;
        accesspoint acount;
        vector<accesspoint> acounts;
        vector<string> IPs;
        string control;
        string last_IP = "az";

        while (getline(cin, line)) {

            istringstream iss(line);
            string word;
            vector<string> tokens;
            while (iss >> word) {tokens.push_back(word);}

            if (line == "exit") break;
            if (tokens[0] == "create" && tokens[1] == "Access" && tokens[2] == "point" && setup) {
                bool found = false;
                if (tokens[4] == "heater" || tokens[4] == "air_conditioner" || tokens[4] == "refrigerator" || tokens[4] == "television") {
                    tokens[4] = "empty";
                }
                if (tokens[4].length() < 8 && tokens[4] != "empty") {cout << "invalid password" << endl; continue;}
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[3]) {cout << "this name already used" << endl; found = true; break;}
                }
                if (found) continue;
                acount.create_accesspoint(line);   acounts.push_back(acount);
                continue;
            }


            if (tokens[0] == "access" && tokens[1] == "point" && tokens[3] == "max" && tokens[4] == "client" && setup) {
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[2]) {found = true; acounts[i].limitaion_client(tokens); break;}
                }
                if (found) continue;
                cout << "invalid access point name" << endl;
                continue;
            }


            if (tokens[0] == "limit" && tokens[1] == "client" && tokens[3] == "from" && tokens[4] == "access" && tokens[5] == "point" && setup) {
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[6]) {found = true; acounts[i].limitaion_IP(tokens); break;}
                }
                if (found) continue;
                cout << "invalid access point name" << endl;
                continue;
            }


            if (tokens[0] == "connect" && tokens[1] == "client" && counter == 4) {
                setup = false;
                if (tokens[2].substr(0 , 10) != "192.168.1.") {cout << "invalid IP" << endl; continue; }
                string ip = tokens[2];
                IPs.push_back(tokens[2]);
                tokens[2] = tokens[2].substr(10);
                if (stoi(tokens[2]) < 0 || stoi(tokens[2]) > 255) {cout << "invalid IP" << endl; continue; }
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[3]) {
                        found = true;
                        if ( tokens.size() == 5) {
                            if (acounts[i].password != tokens[4]) {cout << "wrong password" << endl; break;}
                        }
                        acounts[i].connect_client(ip);
                        break;
                    }
                }
                if (!found) {cout << "invalid access point name" << endl; continue;}
                continue;
            }



            if (tokens[0] == "access" && tokens[1] == "point" && tokens[3] == "clients" && tokens[4] == "list" && counter == 4) {
                setup = false;
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[2]) {
                        found = true;
                        acounts[i].show_clients_IP();
                        break;
                    }
                }
                if (!found) {cout << "invalid access point name" << endl; continue;}
                continue;
            }


            if (tokens[0] == "delete" && tokens[1] == "client" && tokens[3] == "from" && tokens[4] == "access" && tokens[5] == "point"&& counter == 4) {
                setup = false;
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[6]) {
                        found = true;
                        acounts[i].delete_IP(tokens[2]);
                        break;
                    }
                }
                if (!found) {cout << "invalid access point name" << endl; continue;}
                continue;
            }


            if (tokens[0] == "disconnect" && tokens[1] == "client" && tokens[3] == "from" && tokens[4] == "access" && tokens[5] == "point"&& counter == 4) {
                setup = false;
                bool found = false;
                for (int i = 0; i < acounts.size(); i++) {
                    if (acounts[i].username == tokens[6]) {
                        found = true;
                        acounts[i].dissconnect_IP(tokens[2]);
                        break;
                    }
                }
                if (!found) {cout << "this access point does not exist" << endl; continue;}
                continue;
            }

            if (tokens[0] == "read" && tokens[1] == "from" && tokens[2] == "client" && counter == 4) {
                setup = false;
                getline(cin, control);
                if (control == "exit") break;
                bool found = false;
                IPs.clear();
                 for (int i = 0; i < acounts.size(); i++) {
                     if (acounts[i].connected_IPs.has_value()) {
                         for (int j = 0; j < acounts[i].connected_IPs->size() ; j++) {
                             IPs.push_back(acounts[i].connected_IPs->at(j));
                         }
                     }
                 }
                for (int i = 0; i < IPs.size(); i++) {
                    if (IPs[i] == tokens[3]) {
                        if (last_IP != tokens[3]) {
                            bokh=0;
                            koler=0;
                            yakh=0;
                            tv=0;
                        }
                        last_IP = IPs[i];
                        found = true;
                        break;
                    }
                }
                if (!found){cout << "invalid IP" << endl; continue;}
                for (int i = 0; i < acounts.size(); i++) {
                    if (control.substr(0 , 2) == "00") {
                        if (acounts[i].heater) {
                            acounts[i].control(tokens[3] , control);
                            break;
                        }
                    }
                    if (control.substr(0 , 2) == "01") {
                        if (acounts[i].television) {
                            acounts[i].control(tokens[3] , control);
                            break;
                        }
                    }
                    if (control.substr(0 , 2) == "10") {
                        if (acounts[i].air_conditioner) {
                            acounts[i].control(tokens[3] , control);
                            break;
                        }
                    }
                    if (control.substr(0 , 2) == "11") {
                        if (acounts[i].refrigerator == true) {
                            acounts[i].control(tokens[3] , control);
                            break;
                        }
                    }
                }
                continue;
            }

            cout << "invalid command" << endl;
        }
        return 0;

    }







