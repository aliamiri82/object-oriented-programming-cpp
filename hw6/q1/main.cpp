#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Food {
public:
    int base_price;
    int meghdare_ezafe;
    string name;

    Food(int basePrice, int meghdare_ezafe, const string& Name)
    : base_price(basePrice), meghdare_ezafe(meghdare_ezafe), name(Name) {}
};

class Drink : public Food {
public:
    Drink(int base, int extra, const string& name) : Food(base, extra, name) {}

    int calculate_price() {
        return base_price + meghdare_ezafe / 10;
    }
};

class Dessert : public Food {
public:
    Dessert(int base, int extra, const string& name) : Food(base, extra, name) {}

    int calculate_price() {
        return base_price + meghdare_ezafe / 50;
    }
};

class MainCourse : public Food {
public:
    MainCourse(int base, int extra, const string& name) : Food(base, extra, name) {}

    int calculate_price() {
        return base_price + meghdare_ezafe / 20;
    }
};





int main() {
    string line;
    vector<Drink> drinks;
    vector<MainCourse> courses;
    vector<Dessert> desserts;

    while (getline(cin, line)) {
        istringstream iss(line);
        string word;
        vector<string> tokens;
        while (iss >> word) {tokens.push_back(word);}

        if (line == "end") break;
        if (tokens[0] == "add" && tokens[1] == "drink") {
            bool found = false;
            for (int i = 0; i < drinks.size(); i++) {
                if (tokens[2] == drinks[i].name) {
                    found = true;
                }
            }
            if (found) {
                cout << "Item already exists." << endl;
                continue;
            }
            Drink drink(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), tokens[2]);
            drinks.push_back(drink);
            cout << tokens[2] <<" added!" <<  endl;
        }
        if (tokens[0] == "add" && tokens[1] == "dessert") {
            bool found = false;
            for (int i = 0; i < desserts.size(); i++) {
                if (tokens[2] == desserts[i].name) {
                    found = true;
                }
            }
            if (found) {
                cout << "Item already exists." << endl;
                continue;
            }
            Dessert desser(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), tokens[2]);
            desserts.push_back(desser);
            cout << tokens[2] <<" added!" <<  endl;
        }
        if (tokens[0] == "add" && tokens[1] == "main") {
            bool found = false;
            for (int i = 0; i < courses.size(); i++) {
                if (tokens[2] == courses[i].name) {
                    found = true;
                }
            }
            if (found) {
                cout << "Item already exists." << endl;
                continue;
            }
            MainCourse main(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), tokens[2]);
            courses.push_back(main);
            cout << tokens[2] <<" added!" <<  endl;
        }
        if (tokens[0] == "print") {
            bool found = false;
            for (int i = 0; i < courses.size(); i++) {
                if (tokens[1] == courses[i].name) {
                    found = true;
                    cout << tokens[1] << ": " << courses[i].calculate_price() <<endl;
                }
            }
            for (int i = 0; i < desserts.size(); i++) {
                if (tokens[1] == desserts[i].name) {
                    found = true;
                    cout << tokens[1] << ": " << desserts[i].calculate_price() <<endl;
                }
            }
            for (int i = 0; i < drinks.size(); i++) {
                if (tokens[1] == drinks[i].name) {
                    found = true;
                    cout << tokens[1] << ": " << drinks[i].calculate_price() <<endl;
                }
            }
            if (!found) {
                cout << "Item doesn't exist." << endl;
            }
        }
        if (tokens[0] == "sum") {
            int sum = 0;
            for (int i = 0; i < courses.size(); i++) {
                sum += courses[i].calculate_price();
            }
            for (int i = 0; i < drinks.size(); i++) {
               sum += drinks[i].calculate_price();
            }
            for (int i = 0; i < desserts.size(); i++) {
                    sum += desserts[i].calculate_price();
            }
            cout << "Total: " << sum <<endl;
        }

        if (tokens[0] == "delete") {
            bool found = false;
            for (int i = 0; i < courses.size(); i++) {
                if (tokens[1] == courses[i].name) {
                    found = true;
                    cout << tokens[1] << " deleted."  <<endl;
                    courses.erase(courses.begin() + i);
                }
            }
            for (int i = 0; i < desserts.size(); i++) {
                if (tokens[1] == desserts[i].name) {
                    found = true;
                    cout << tokens[1] << " deleted."  <<endl;
                    desserts.erase(desserts.begin() + i);
                }
            }
            for (int i = 0; i < drinks.size(); i++) {
                if (tokens[1] == drinks[i].name) {
                    found = true;
                    cout << tokens[1] << " deleted." <<endl;
                    drinks.erase(drinks.begin() + i);

                }
            }
            if (!found) {
                cout << "Item doesn't exist." << endl;
            }
        }
    }
}