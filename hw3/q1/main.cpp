#include <iostream>
#include <vector>
#include <string>
using namespace std;

class airplane {
    public:
    string ID;
    int status;
};

class bands {
    public:
    string ID;
};

int main() {
    int k;
    cin >> k;
    cin.ignore();
    string ID;
    int number_band;

    vector<airplane> havapiema;
    airplane airplane;

    vector<bands> band(k);

    for (int i = 0; i < k; i++) {
        band[i].ID = "A";
    }

    string dastur;
    int size = 0;
    while (true) {
        cin >> dastur;
        if (dastur == "ADD-PLANE") {
            cin >> ID;
            airplane.ID = ID;
            airplane.status = 1;
            havapiema.push_back(airplane);
            size++;
        }
        else if (dastur == "TAKE-OFF") {
            cin >> ID;
            bool found = false;
            for (int i = 0; i < size; i++) {
                if (havapiema[i].ID == ID) {
                    found = true;
                    if (havapiema[i].status == 2) {cout << "YOU ARE TAKING OFF" << endl;   break;}
                    else if (havapiema[i].status == 3) {cout << "YOU ARE LANDING NOW"<< endl;   break;}
                    else if (havapiema[i].status == 1) {
                        for (int j = 0; j < k; j++) {
                            if (band[j].ID == "A") {
                                havapiema[i].status = 2;
                                band[j].ID = ID;
                                break;
                            }
                            if (j == k - 1) cout << "NO FREE BAND"<< endl;
                        }
                    }
                }
            }
            if (!found){cout<< "YOU ARE NOT HERE"<< endl; }
        }
        else if (dastur == "PLANE-STATUS") {
            cin >> ID;
            bool found = false;
            for (int i = 0; i < size; i++) {
                if (havapiema[i].ID == ID) {
                    found = true;
                    cout << havapiema[i].status << endl;
                    break;
                }
            }
            if (!found) {cout << 4<< endl; }
        }
        else if (dastur == "BAND-STATUS") {
            cin >> number_band;
            if (band[number_band - 1].ID == "A") {
                cout << "FREE" << endl;
            }
            if (band[number_band - 1].ID != "A") {
                cout << band[number_band - 1].ID << endl;
            }
        }
        else if (dastur == "LANDING") {
            cin >> ID;
            bool found = true;
            for (int i = 0; i < size; i++) {
                if (havapiema[i].ID == ID) {
                    found = false;
                    if (havapiema[i].status == 1) {
                        cout << "YOU ARE HERE" << endl;
                        break;
                    }
                    if (havapiema[i].status == 2) {
                        cout << "YOU ARE TAKING OFF" << endl;
                        break;
                    }
                    if (havapiema[i].status == 3) {
                        cout << "YOU ARE LANDING NOW" << endl;
                        break;
                    }
                }
            }
            if (found) {
                for (int j = k - 1; j >= 0; j--) {
                    if (band[j].ID == "A") {
                        airplane.ID = ID;
                        airplane.status = 3;
                        havapiema.push_back(airplane);
                        size++;
                        band[j].ID = ID;
                        break;
                    }
                    if (j == 0) {
                        cout << "NO FREE BAND" << endl;
                    }
                }
            }
        }
        else if (dastur == "EXIT") break;
    }
    return 0;
}
