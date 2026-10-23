#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class Hugenumbers {
    public:
    string name;
    string number;
    string type;
};

    string sum_of_numbers(string number1, string number2, string number3,string type ) {
        reverse(number1.begin(), number1.end());
        reverse(number2.begin(), number2.end());
        if (type == "Decimal") {
            int min;
            if(number1.length() < number2.length()) {
                min = number1.length();
            }
            else {
                min = number2.length();
            }
            int carry = 0;
            for (int i = 0; i < min; i++) {
                int k = carry + (number1[i] - '0') + (number2[i] - '0');
                if (k > 9) {
                    carry = 1;
                    k = k - 10;
                }
                else {
                    carry = 0;
                }
                number1[i] = k + '0';
                number2[i] = k + '0';
            }
            if (number1.size() > number2.size()) {
                for (int i = min; i < number1.size(); i++) {
                    int k = carry + (number1[i] - '0');
                    if (k > 9) {
                        carry = 1;
                        k = k - 10;
                    }
                    else {
                        carry = 0;
                    }
                    number1[i] = k + '0';
                }
                if (carry == 1) {
                    number1.push_back('1');
                }
                reverse(number1.begin(), number1.end());
                number3 = number1;
            }
            else{
                for (int i = min; i < number2.size(); i++) {
                    int k = carry + (number2[i] - '0');
                    if (k > 9) {
                        carry = 1;
                        k = k - 10;
                    }
                    else {
                        carry = 0;
                    }
                    number2[i] = k + '0';
                }
                if (carry == 1) {
                    number2.push_back('1');
                }
                reverse(number2.begin(), number2.end());
                number3 = number2;
            }
        }
        if (type == "Binary") {
            int min;
            if(number1.length() < number2.length()) {
                min = number1.length();
            }
            else {
                min = number2.length();
            }
            int carry = 0;
            for (int i = 0; i < min; i++) {
                int k = carry + (number1[i] - '0') + (number2[i] - '0');
                if (k > 1) {
                    carry = 1;
                    k = k - 2;
                }
                else {
                    carry = 0;
                }
                number1[i] = k + '0';
                number2[i] = k + '0';
            }
            if (number1.size() > number2.size()) {
                for (int i = min; i < number1.size(); i++) {
                    int k = carry + (number1[i] - '0');
                    if (k > 1) {
                        carry = 1;
                        k = k - 2;
                    }
                    else {
                        carry = 0;
                    }
                    number1[i] = k + '0';
                }
                if (carry == 1) {
                    number1.push_back('1');
                }
                reverse(number1.begin(), number1.end());
                number3 = number1;
            }
            else{
                for (int i = min; i < number2.size(); i++) {
                    int k = carry + (number2[i] - '0');
                    if (k > 1) {
                        carry = 1;
                        k = k - 2;
                    }
                    else {
                        carry = 0;
                    }
                    number2[i] = k + '0';
                }
                if (carry == 1) {
                    number2.push_back('1');
                }
                reverse(number2.begin(), number2.end());
                number3 = number2;
            }
        }
        if (type == "Hexadecimal") {
            int min;
            if(number1.length() < number2.length()) {
                min = number1.length();
            }
            else {
                min = number2.length();
            }
            int carry = 0;
            for (int i = 0; i < min; i++) {
                int a,b;
                char c;
                if (number1[i] == '0') a=0;else if (number1[i] == '1') a=1;else if (number1[i] == '2') a=2;else if (number1[i] == '3') a=3;else if (number1[i] == '4') a=4;else if (number1[i] == '5') a=5;else if (number1[i] =='6')a=6;else if (number1[i] =='7')a=7;else if (number1[i] =='8')a=8;else if (number1[i] =='9')a=9;else if (number1[i] =='A')a=10;else if (number1[i] =='B')a=11;else if (number1[i] =='C')a=12;else if (number1[i] =='D')a=13;else if (number1[i] =='E')a=14;else if (number1[i] =='F')a=15;
                if (number2[i] == '0') b=0;else  if (number2[i] == '1') b=1;else if (number2[i] == '2') b=2;else if (number2[i] == '3') b=3;else if (number2[i] == '4') b=4;else if (number2[i] == '5') b=5;else if (number2[i] =='6')b=6;else if (number2[i] =='7')b=7;else if (number2[i] =='8')b=8;else if (number2[i] =='9')b=9;else if (number2[i] =='A')b=10;else if (number2[i] =='B')b=11;else if (number2[i] =='C')b=12;else if (number2[i] =='D')b=13;else if (number2[i] =='E')b=14;else if (number2[i] =='F')b=15;
                int k = carry + a + b;
                if (k > 15) {
                    carry = 1;
                    k = k - 16;
                }
                else {
                    carry = 0;
                }
                if (k == 0)c='0'; else if (k == 1)c='1';else if (k == 2)c='2';else if (k == 3)c='3';else if (k == 4)c='4';else if (k == 5)c='5';else if (k == 6)c='6';else if (k == 7)c='7';else if (k == 8)c='8';else if (k == 9)c='9';else if (k == 10)c='A';else if (k == 11)c='B';else if (k == 12)c='C';else if (k == 13)c='D';else if (k == 14)c='E';else if (k == 15)c='F';
                number1[i] = c;
                number2[i] = c;
            }
            if (number1.size() > number2.size()) {
                for (int i = min; i < number1.size(); i++) {
                    int a;
                    char c;
                    if (number1[i] == '0') a=0;else if (number1[i] == '1') a=1;else if (number1[i] == '2') a=2;else if (number1[i] == '3') a=3;else if (number1[i] == '4') a=4;else if (number1[i] == '5') a=5;else if (number1[i] =='6')a=6;else if (number1[i] =='7')a=7;else if (number1[i] =='8')a=8;else if (number1[i] =='9')a=9;else if (number1[i] =='A')a=10;else if (number1[i] =='B')a=11;else if (number1[i] =='C')a=12;else if (number1[i] =='D')a=13;else if (number1[i] =='E')a=14;else if (number1[i] =='F')a=15;
                    int k = carry + a;
                    if (k > 15) {
                        carry = 1;
                        k = k - 16;
                    }
                    else {
                        carry = 0;
                    }
                    if (k == 0)c='0'; else if (k == 1)c='1';else if (k == 2)c='2';else if (k == 3)c='3';else if (k == 4)c='4';else if (k == 5)c='5';else if (k == 6)c='6';else if (k == 7)c='7';else if (k == 8)c='8';else if (k == 9)c='9';else if (k == 10)c='A';else if (k == 11)c='B';else if (k == 12)c='C';else if (k == 13)c='D';else if (k == 14)c='E';else if (k == 15)c='F';
                    number1[i] = c;
                }
                if (carry == 1) {
                    number1.push_back('1');
                }
                reverse(number1.begin(), number1.end());
                number3 = number1;
            }
            else{
                for (int i = min; i < number2.size(); i++) {
                    int a;
                    char c;
                if (number2[i] == '0') a=0;else  if (number2[i] == '1') a=1;else if (number2[i] == '2') a=2;else if (number2[i] == '3') a=3;else if (number2[i] == '4') a=4;else if (number2[i] == '5') a=5;else if (number2[i] =='6')a=6;else if (number2[i] =='7')a=7;else if (number2[i] =='8')a=8;else if (number2[i] =='9')a=9;else if (number2[i] =='A')a=10;else if (number2[i] =='B')a=11;else if (number2[i] =='C')a=12;else if (number2[i] =='D')a=13;else if (number2[i] =='E')a=14;else if (number2[i] =='F')a=15;
                    int k = carry + a;
                    if (k > 15) {
                        carry = 1;
                        k = k - 16;
                    }
                    else {
                        carry = 0;
                    }
                    if (k == 0)c='0'; else if (k == 1)c='1';else if (k == 2)c='2';else if (k == 3)c='3';else if (k == 4)c='4';else if (k == 5)c='5';else if (k == 6)c='6';else if (k == 7)c='7';else if (k == 8)c='8';else if (k == 9)c='9';else if (k == 10)c='A';else if (k == 11)c='B';else if (k == 12)c='C';else if (k == 13)c='D';else if (k == 14)c='E';else if (k == 15)c='F';
                    number2[i] = c;
                }
                if (carry == 1) {
                    number2.push_back('1');
                }
                reverse(number2.begin(), number2.end());
                number3 = number2;
            }
        }
        return number3;
    }


int char_to_val(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0;
    }

char val_to_char(int v) {
        if (v < 10) return v + '0';
        return v - 10 + 'A';
    }


////////////////////برای این قسمت از الگوریتم جی پی تی کمک گرفته شد
string multiply_big_numbers(string num1, string num2, string type) {
        int base;
        if (type == "Decimal") base = 10;
        if (type == "Binary") base = 2;
        if (type == "Hexadecimal") base = 16;

        int n = num1.size(), m = num2.size();
        vector<int> result(n + m, 0);

        reverse(num1.begin(), num1.end());
        reverse(num2.begin(), num2.end());

        for (int i = 0; i < n; ++i) {
            int d1 = char_to_val(num1[i]);
            for (int j = 0; j < m; ++j) {
                int d2 = char_to_val(num2[j]);
                result[i + j] += d1 * d2;
                if (result[i + j] >= base) {
                    result[i + j + 1] += result[i + j] / base;
                    result[i + j] %= base;
                }
            }
        }

        while (result.size() > 1 && result.back() == 0)
            result.pop_back();

        string res_str = "";
        for (int i = result.size() - 1; i >= 0; --i)
            res_str += val_to_char(result[i]);

        return res_str;
    }


int main() {
    vector<string> tokens;

    string line ;
    string word;
    vector<Hugenumbers> hugenumbers;
    Hugenumbers hugenumber;
while (true) {
    tokens.clear();
    getline(cin, line);
    stringstream ss(line);
    while (ss >> word) {
        tokens.push_back(word);
    }
    if (tokens.size() == 1 && tokens[0] == "end") {
        break;
    }

    if (tokens.size() == 5 && tokens[0] == "create" && tokens[2] == "as") {
        bool found = false;
        for (int i = 0; i < hugenumbers.size(); i++) {
            if (hugenumbers[i].name == tokens[1]) {
                hugenumbers[i].number = tokens[4];
                hugenumbers[i].type = tokens[3];
                found = true;
            }
        }
        if (!found) {
            hugenumber.name = tokens[1];
            hugenumber.number = tokens[4];
            hugenumber.type = tokens[3];
            hugenumbers.push_back(hugenumber);
        }
        cout << hugenumber.name << " as " << hugenumber.type << " is " << hugenumber.number << endl;
        continue;
    }

    if (tokens.size() == 5 && tokens[1] == "+" && tokens[3] == "in") {
        string number_out;
        for (int i = 0; i < hugenumbers.size(); i++) {
            if (hugenumbers[i].name == tokens[0]) {
                for (int j = 0; j < hugenumbers.size(); j++) {
                    if (hugenumbers[j].name == tokens[2]) {
                        if (hugenumbers[i].type == hugenumbers[j].type) {
                            number_out = sum_of_numbers(hugenumbers[i].number, hugenumbers[j].number, number_out,hugenumbers[j].type);
                            cout << tokens[4] << " as " << hugenumbers[i].type << " is " << number_out << endl;
                            bool found = false;
                            for (int k = 0; k < hugenumbers.size(); k++) {
                                if (hugenumbers[k].name == tokens[4]) {
                                    hugenumbers[k].number = number_out;
                                    hugenumbers[k].type = hugenumbers[j].type;
                                    hugenumbers[k].name = tokens[4];
                                    found = true;
                                }
                            }
                            if (!found) {
                                hugenumber.number = number_out;
                                hugenumber.type = hugenumbers[j].type;
                                hugenumber.name = tokens[4];
                                hugenumbers.push_back(hugenumber);
                            }
                        }
                    }
                }
            }
        }
    }

    if (tokens.size() == 5 && tokens[1] == "*" && tokens[3] == "in") {
        string number_out;
        for (int i = 0; i < hugenumbers.size(); i++) {
            if (hugenumbers[i].name == tokens[0]) {
                for (int j = 0; j < hugenumbers.size(); j++) {
                    if (hugenumbers[j].name == tokens[2]) {
                        if (hugenumbers[i].type == hugenumbers[j].type) {
                            number_out = multiply_big_numbers(hugenumbers[i].number,hugenumbers[j].number,hugenumbers[j].type);
                            cout << tokens[4] << " as " << hugenumbers[i].type << " is " << number_out << endl;
                            bool found = false;
                            for (int k = 0; k < hugenumbers.size(); k++) {
                                if (hugenumbers[k].name == tokens[4]) {
                                    hugenumbers[k].number = number_out;
                                    hugenumbers[k].type = hugenumbers[j].type;
                                    hugenumbers[k].name = tokens[4];
                                    found = true;
                                }
                            }
                            if (!found) {
                                hugenumber.number = number_out;
                                hugenumber.type = hugenumbers[j].type;
                                hugenumber.name = tokens[4];
                                hugenumbers.push_back(hugenumber);
                            }
                        }
                    }
                }
            }
        }
    }
}



return 0;
}