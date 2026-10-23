#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
using namespace std;
class view;
class User;
class Plane;
class Flights;
class Programcontroller;

bool admin = false;
bool sabtenam = true;
bool canlogin = true;
bool menus = false;
bool adminmenu = false;
bool customermenu = false;
bool buyticketmenu = false;
bool cancelticketmenu = false;
bool ayaadmin = false;
/////////////////////////model
class Plain {
    public:
    string name;
    int capacity;
    int flights;

    Plain(string name, int capacity, int flights)
        : name(name), capacity(capacity), flights(flights) {}

};

class Flights {
public:
    Plain plain;
    int passengers;
    int capacity;
    string origin;
    string destination;
    string date;
    int ticketprce;
    public:
    Flights(string origin, string destination, int ticketprce , string date , Plain plain , int capacity)
        : origin(origin), destination(destination),ticketprce(ticketprce), date(date), plain(plain) , capacity(capacity) {}
};

class User {
public:
    string username;
    string password;
    bool isadmin;
    int balance;
    vector<Flights>tickets;
public:

    User(string username, string password, bool isadmin, int balance , vector<Flights>tickets)
        :  username(username),password(password), isadmin(isadmin), balance(balance), tickets(tickets) {}

    User() :  username(""),password(""), isadmin(false), balance(0), tickets( {}) {}


    string getUsername() { return username; }
    string getPassword() { return password; }
    bool getIsAdmin() { return isadmin; }
    int getBalance() { return balance; }
    vector<Flights> getTickets() { return tickets; }
};

//////////////////controller
class ProgramController {
private:
    vector<User> users;
    vector<Plain> plains;
    vector<Flights> flights;
public:
    string addclientUser(string username, string password) {
        if (!sabtenam){return "invalid command!";}
        if (!regex_match(username, regex("^[A-Za-z0-9_]*[A-Za-z]+[A-Za-z0-9_]*$")))
            return "username format is invalid";

        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) return "a user exists with this username";
        }

        if (!regex_match(password, regex("^[A-Za-z0-9_]+$")))
            return "password format is invalid";

        if (password.length() < 5) return "password is weak";
        if (!regex_match(password,regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).+$"))) return "password is weak";
        vector<Flights> tickets;
        users.push_back(User(username, password, false, 0, tickets));
        return "register successful";
    }

    string addadminUser(string username, string password) {
        if (!sabtenam){return "invalid command!";}
        if (admin) return "admin user already created";
        if (!regex_match(username, regex("^[A-Za-z0-9_]*[A-Za-z]+[A-Za-z0-9_]*$")))
            return "username format is invalid";

        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) return "a user exists with this username";
        }

        if (!regex_match(password, regex("^[A-Za-z0-9_]+$")))
            return "password format is invalid";

        if (password.length() < 5) return "password is weak";
        if (!regex_match(password,regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).+$"))) return "password is weak";
        vector<Flights> tickets;
        users.push_back(User(username, password, true, 0, tickets));
        admin = true;
        return "admin user created successfully";
    }

    string login(string username, string password) {
        if (!canlogin) return "invalid command!";
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                if (users[i].getPassword() == password){
                    sabtenam=false;  canlogin=false;  menus = true;
                    if (users[i].getIsAdmin()){ayaadmin=true;}
                    if (!users[i].getIsAdmin()){ayaadmin=false;}
                    return"login successful";
                }
                if (users[i].getPassword() != password){ return "incorrect password";}
            }
        }
        return "no user exists with this username";
    }
    string logout() {
        if (menus) {
            sabtenam=true;  canlogin=true; menus=false;
            return "logout successful";
        }
        return "invalid command!";
    }


    void getUser(string username ,int balance, vector<Flights> tickets , bool adminis) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                balance = users[i].getBalance();
                tickets = users[i].getTickets();
                adminis = users[i].getIsAdmin();
            }
        }
    }

    string addplane(string name, string capacity) {
        for (int i = 0; i < plains.size(); i++) {
            if (plains[i].name == name) {
                return "an airplane exists with this name";
            }
        }
        if (stoi(capacity) < 10) {
            return "invalid capacity";
        }
        int x = 0;
        plains.push_back(Plain(name , stoi(capacity) , x));
        return "plane created successfully";
    }


    string addflight(string name, string origin, string date, string destination, string ticketprc) {
        int a;
        if (!regex_match(date , regex(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$)"))) {
            return "invalid command!";
        }
        bool found = false;
        for (int i = 0; i < plains.size(); i++) {
            if (plains[i].name == name) {
                found = true;
                a = i;
            }
        }
        if (!found){ return "no airplane exists with this name";}
        if (stoi(ticketprc) <= 0) {
            return "invalid ticket price";
        }
        for (int i = 0; i < flights.size(); i++) {

            if (flights[i].plain.name == name) {
                if (flights[i].date == date) {
                    return "This aircraft already has a flight on this date";
                }
            }
        }
        int capacity = 0;
        plains[a].flights++;
        flights.push_back(Flights(origin, destination, stoi(ticketprc), date, plains[a], capacity ));
        return "flight created successfully";
    }

    static bool Sortflight(const Flights& a, const Flights& b) {
        if (a.date != b.date)
            return a.date < b.date;
        if (a.ticketprce != b.ticketprce)
            return a.ticketprce < b.ticketprce;
        return a.plain.name < b.plain.name;
    }

    void printallflights() {
        sort(flights.begin(), flights.end(), Sortflight);
        if (flights.size() > 0) {
            for (int i = 0; i < flights.size(); i++) {
                cout << i+1 <<"- " << flights[i].origin<< "->" << flights[i].destination<< " " << flights[i].date<< " " << flights[i].plain.name <<" "<< flights[i].ticketprce<< endl;
            }
        }

        if (flights.size() == 0) {
            cout<<"nothing"<<endl;
        }
    }
    void printallflightsondate(string date) {
        vector<Flights> flightsdate;
        for (int i = 0; i < flights.size(); i++) {
            if (flights[i].date == date) {
                flightsdate.push_back(flights[i]);
            }
        }
        sort(flightsdate.begin(), flightsdate.end(), Sortflight);
        if (flightsdate.size() > 0) {
            for (int i = 0; i < flightsdate.size(); i++) {
                cout << i+1 <<"- " << flightsdate[i].origin<< "->" << flightsdate[i].destination<< " " << flightsdate[i].date<< " " << flightsdate[i].plain.name <<" "<< flightsdate[i].ticketprce<< endl;
            }
        }

        if (flightsdate.size() == 0) {
            cout<<"nothing"<<endl;
        }
    }

    static bool Sortairplain(const Plain& a, const Plain& b) {
        return a.name < b.name;
    }

    void printallplain() {
        sort(plains.begin(), plains.end(), Sortairplain);
        if (plains.size() > 0) {
            for (int i = 0; i < plains.size(); i++) {
                cout << i+1 <<"- " << plains[i].name<< " : " << plains[i].flights << endl;
            }
        }

        if (plains.size() == 0) {
            cout<<"nothing"<<endl;
        }
    }

    void showbuyflights(string origin, string destination ,string username) {
        vector<Flights> flightsorigin;
        flightsorigin.clear();
        int a;
        for (int i = 0; i < flights.size(); i++) {
            if (flights[i].origin == origin && flights[i].destination == destination) {
                customermenu = false;
                buyticketmenu = true;
                flightsorigin.push_back(flights[i]);

            }
        }
        sort(flightsorigin.begin(), flightsorigin.end(), Sortflight);
        for (int i = 0; i < flightsorigin.size(); i++) {
             cout << i+1 << "- " << flightsorigin[i].date << " " << flightsorigin[i].plain.name <<" "<< flightsorigin[i].ticketprce<< endl;
        }
        if (flightsorigin.size() == 0) {
            cout << "There is no direct flight from " << origin << " to " << destination << endl;
            vector<Flights> flightsmabda;
            vector<Flights> flightsmaghsad;
            vector<Flights> flights1;
            vector<Flights> flights2;
            for (int i = 0; i < flights.size(); i++) {
                if (flights[i].origin == origin) {
                    flightsmabda.push_back(flights[i]);
                }
            }
            for (int i = 0; i < flights.size(); i++) {
                if (flights[i].destination == destination) {
                    flightsmaghsad.push_back(flights[i]);
                }
            }
            int k=1;
            for (int i = 0; i < flightsmabda.size(); i++) {
                for (int j = 0; j < flightsmaghsad.size(); j++) {
                    if (flightsmabda[i].destination == flightsmaghsad[j].origin && flightsmabda[i].date < flightsmaghsad[j].date) {
                        flights2.push_back(flightsmaghsad[j]);
                        flights1.push_back(flightsmabda[i]);
                        cout<< k<< "- "<<flightsmabda[i].origin<<"->"<<flightsmabda[i].destination<<" "<<flightsmabda[i].date<<" "<<flightsmabda[i].ticketprce<<" | "<<flightsmaghsad[j].origin<<"->"<<flightsmaghsad[j].destination<<" "<<flightsmaghsad[j].date<<" "<<flightsmaghsad[j].ticketprce << endl;
                        k++;
                    }
                }
            }
            if (k == 1) return;
            while (true) {
                string input;
                getline(cin, input);
                if (input == "end") {
                    customermenu = true;
                    buyticketmenu = false;
                    return;
                }


                if (!regex_match(input, regex("^[0-9]+$"))) {
                    cout << "invalid command!" << endl;
                    continue;
                }
                if (stoi(input) <= 0 || stoi(input) > flights1.size() ) {
                    cout << "invalid number" << endl;
                    continue;
                }
                int index = stoi(input) - 1;
                if (flights1[index].capacity == flights1[index].plain.capacity || flights2[index].capacity == flights2[index].plain.capacity) {
                    cout <<"no empty seat" << endl;
                    continue;
                }
                for (int i = 0; i < users.size(); i++) {
                    if (users[i].getUsername() == username) {
                        a = i;
                        if (users[i].getBalance() < (flights1[index].ticketprce + flights2[index].ticketprce)) {
                            cout << "you don't have enough money" << endl;
                            goto outer_continue1;
                        }
                    }
                }
                cout << "purchase successful" << endl;
                flights1[index].capacity++;
                flights2[index].capacity++;
                for (int i = 0; i < flights.size(); i++) {
                    if (flights[i].plain.name == flights1[index].plain.name && flights[i].date == flights1[index].date ) {
                        flights[i] = flights1[index];
                    }
                }
                for (int i = 0; i < flights.size(); i++) {
                    if (flights[i].plain.name == flights2[index].plain.name && flights[i].date == flights2[index].date ) {
                        flights[i] = flights2[index];
                    }
                }
                users[a].balance = users[a].balance - flights1[index].ticketprce - flights2[index].ticketprce;
                users[a].tickets.push_back(flights1[index]);
                users[a].tickets.push_back(flights2[index]);
                for (int j = 0; j < users.size(); j++) {
                    if (users[j].getIsAdmin() == true) {
                        users[j].balance = users[j].balance + flights1[index].ticketprce + flights2[index].ticketprce;
                        break;
                    }
                }
                outer_continue1:;
            }

            return;
        }
        if (flightsorigin.size() > 0) {
            while (true) {
                string input;
                getline(cin, input);
                if (input == "end") {
                    customermenu = true;
                    buyticketmenu = false;
                    return;
                }


                if (!regex_match(input, regex("^[0-9]+$"))) {
                    cout << "invalid command!" << endl;
                    continue;
                }
                if (stoi(input) <= 0 || stoi(input) > flightsorigin.size() ) {
                    cout << "invalid number" << endl;
                    continue;
                }
                if (flightsorigin[stoi(input) - 1].capacity == flightsorigin[stoi(input) - 1].plain.capacity) {
                    cout <<"no empty seat" << endl;
                    continue;
                }
                int index = stoi(input) - 1;
                for (int i = 0; i < users.size(); i++) {
                    if (users[i].getUsername() == username) {
                        a = i;
                        if (users[i].getBalance() < flightsorigin[stoi(input) - 1].ticketprce) {
                            cout << "you don't have enough money" << endl;
                            goto outer_continue;
                        }
                    }
                }
                cout << "purchase successful" << endl;
                flightsorigin[index].capacity++;
                for (int i = 0; i < flights.size(); i++) {
                    if (flights[i].plain.name == flightsorigin[index].plain.name && flights[i].date == flightsorigin[index].date ) {
                        flights[i] = flightsorigin[index];
                    }
                }
                users[a].balance = users[a].balance - flightsorigin[index].ticketprce;
                users[a].tickets.push_back(flightsorigin[index]);
                for (int j = 0; j < users.size(); j++) {
                    if (users[j].getIsAdmin() == true) {
                        users[j].balance = users[j].balance - flightsorigin[index].ticketprce;
                        break;
                    }
                }
                outer_continue:;
            }
        }
    }

    string increasebalance(string username , string amount) {
        if (stoi(amount) > 0) {
            for (int i = 0; i < users.size(); i++) {
                if (users[i].getUsername() == username) {
                    users[i].balance += stoi(amount);
                    return "account charged successfully";
                }
            }
        }
        return "invalid amount";
    }

    void cancelticket(string username) {
        int a;
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                a = i;
                if(users[i].tickets.size() == 0) {cout << "you don't have any tickets" << endl; return;}
                if (users[i].tickets.size() > 0) {
                    for (int j = 0; j < users[i].tickets.size(); j++) {
                        cout << j+1 << "- " << users[i].tickets[j].origin << "->" << users[i].tickets[j].destination <<" " << users[i].tickets[j].date <<" "<<  users[i].tickets[j].ticketprce<< endl;
                    }
                }
            }
        }

        while (true) {
            string input;
            getline(cin, input);
            if (input == "end") {
                customermenu = true;
                cancelticketmenu = false;
                return;
            }
            if (!regex_match(input, regex("^[0-9]+$"))) {
                cout << "invalid command!" << endl;
                continue;
            }
            if (stoi(input) <= 0 || stoi(input) > users[a].tickets.size() ) {
                cout << "invalid number" << endl;
                continue;
            }
            int index = stoi(input);
            cout << "cancel successful" << endl;
            int money = users[a].tickets[index - 1].ticketprce / 5;
            users[a].balance = users[a].balance + users[a].tickets[index - 1].ticketprce - money;
            for (int j = 0; j < users.size(); j++) {
                if (users[j].getIsAdmin() == true) {
                    users[j].balance = users[j].balance - users[a].tickets[index - 1].ticketprce + money;
                    break;
                }
            }
            users[a].tickets.erase(users[a].tickets.begin() + index);
            for (int j = 0; j < flights.size(); j++) {
                if (flights[j].plain.name == users[a].tickets[index - 1].plain.name && flights[j].date == users[a].tickets[index - 1].date ) {
                    flights[j].capacity--;
                    break;
                }
            }
            break;
        }
    }

    int capital(string username) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username) {
                return users[i].getBalance();
            }
        }
    }

};


/////////////////////view
class View{
public:
    User currentuser;
    ProgramController controller;

    void run() {
        vector<string> tokens;
        string line ;
        string word;

        while (true) {
            tokens.clear();
            getline(cin, line);
            stringstream ss(line);
            while (ss >> word) {
                tokens.push_back(word);
            }
            if (tokens[0] == "register") {
                if (tokens.size() == 3) {
                    cout << controller.addclientUser(tokens[1], tokens[2]) << endl;
                    continue;
                } else if (tokens.size() == 5 && tokens[1] == "as" && tokens[2] == "admin") {
                    cout << controller.addadminUser(tokens[3], tokens[4]) << endl;
                    continue;
                }
            }

            if (tokens[0] == "login") {
                if (tokens.size() == 3) {
                   string out = controller.login(tokens[1], tokens[2]);
                    cout << out << endl;
                    if (out == "login successful") {
                        vector<Flights> tickets;
                        int balance = 0;
                        controller.getUser(tokens[1], balance, tickets, ayaadmin);
                        currentuser = User(tokens[1], tokens[2], ayaadmin, balance , tickets);
                    }
                    continue;
                }
            }
            if (tokens[0] == "Exit") {
                if (tokens.size() == 1) {
                    break;
                }
            }

            if (tokens[0] == "logout") {
                if (tokens.size() == 1) {
                    cout << controller.logout() << endl;
                    continue;
                }
            }

            if (tokens[0] == "admin" && tokens[1] == "menu" && tokens.size() == 2 && menus) {
                if (currentuser.getIsAdmin()) {
                    adminmenu = true;
                    customermenu = false;
                    menus = false;
                    continue;
                }
                else {
                    cout << "you don't have access to this menu" << endl;
                    continue;
                }
            }

            if (tokens[0] == "customer" && tokens[1] == "menu" && tokens.size() == 2 && menus) {
                if (!currentuser.getIsAdmin()) {
                    customermenu = true;
                    adminmenu = false;
                    menus = false;
                    continue;
                }
                else {
                    cout << "you don't have access to this menu" << endl;
                    continue;
                }
            }

            if (tokens[0] == "add" && tokens[1] == "airplane" && tokens.size() == 4 && adminmenu) {
                cout << controller.addplane(tokens[2], tokens[3]) << endl;
                continue;
            }

            if (tokens[0] == "back" && tokens.size() == 1) {
                adminmenu = false;
                customermenu = false;
                menus = true;
                continue;
            }

            if (tokens[0] == "add" && tokens[1] == "flight" && tokens.size() == 7 && adminmenu) {
                cout << controller.addflight(tokens[5] , tokens[2], tokens[4], tokens[3], tokens[6]) << endl;
                continue;
            }

            if (tokens[0] == "show" && tokens[1] == "all" && tokens[2] == "flights" && tokens.size() == 3 && adminmenu) {
                controller.printallflights();
                continue;
            }

            if (tokens[0] == "show" && tokens[1] == "flights" && tokens[2] == "on" && tokens.size() == 4 && adminmenu) {
                controller.printallflightsondate(tokens[3]);
                continue;
            }

            if (tokens[0] == "show" && tokens[1] == "airplanes" && tokens.size() == 2 && adminmenu) {
                controller.printallplain();
                continue;
            }

            if (tokens[0] == "show" && tokens[1] == "capital" && tokens.size() == 2 && (adminmenu|| customermenu) ) {
                cout << controller.capital(currentuser.username) << endl;
                continue;
            }

            if (tokens[0] == "purchase" && tokens[1] == "ticket" && tokens.size() == 4 && customermenu) {
                 controller.showbuyflights(tokens[2], tokens[3] , currentuser.username);
                continue;
            }

            if (tokens[0] == "charge" && tokens[1] == "account" && tokens.size() == 3 && customermenu) {
                cout << controller.increasebalance(currentuser.username, tokens[2]) << endl;
                continue;
            }

            if (tokens[0] == "cancel" && tokens[1] == "ticket" && tokens.size() == 2 && customermenu) {
                controller.cancelticket(currentuser.username);
                continue;
            }
            cout << "invalid command!" << endl;

        }
    }
};


int main() {
    View view;
    view.run();
    return 0;
}



