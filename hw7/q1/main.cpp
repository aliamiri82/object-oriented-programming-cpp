#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
using namespace std;

// ------------- Exception example ------------------
class invalid_Exception : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: invalid command"; //خروجی خطای مورد نظر
    }
};
class PatientNotFoundException : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: no patient with this name exist"; //خروجی خطای مورد نظر
    }
};
class DoctorsWithThisSpecialtyBusyException : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: doctors with this specialty are busy"; //خروجی خطای مورد نظر
    }
};
class DoctorForProblemNotExistException : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: no doctor for this problem exist"; //خروجی خطای مورد نظر
    }
};
class DoctorNotFoundException : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: no doctor with this name exist"; //خروجی خطای مورد نظر
    }
};
class NameException : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: error output"; //خروجی خطای مورد نظر
    }
};
class Doctor_name_exception : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: doctor added twice"; //خروجی خطای مورد نظر
    }
};
class week_name_exception : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: invalid weekday"; //خروجی خطای مورد نظر
    }
};
class negative_patients_exception : public exception { //ساختن اکسپشن جدا برای هر خطا
public:
    const char* what() const noexcept override {
        return "Error: number of patients can not be negative"; //خروجی خطای مورد نظر
    }
};

// ------------- your code goes here ----------------
class Appointment {
    public:
    string patient_name;
    string doctor_name;
    string day;
};
vector<Appointment> appointments;

class Patients {
    public:
    string name;
    string specialty;
};
class Doctors {
    public:
    string name;
    string specialty;
    int number_of_patients;
    int number_of_days;
    string working_days[7];
    vector<Patients> patients;
    vector<Patients> daily_patients[7];

};
vector<Doctors> doctors;
bool inputHandler(string line) {
    vector<string> tokens;
    Doctors doctor;
    string word;
        tokens.clear();
        stringstream ss(line);
        while (ss >> word) {
            tokens.push_back(word);
        }
        /////////////////////////exit
        if (tokens.size() == 1 && tokens[0] == "exit") {
            return false;
        }
        //add doctor <name> <specialty> <number of patients> <working days>
        if (tokens.size() >= 6 && tokens[0] == "add" && tokens[1] == "doctor") {
            for (int i = 0; i < doctors.size(); i++) {
                if (doctors[i].name == tokens[2]) {
                    throw Doctor_name_exception();
                }
            }
            for (int i = 0; i < tokens.size() -  5; i++) {
                if (tokens[i + 5] == "saturday" || tokens[i + 5] == "sunday" ||tokens[i + 5] == "monday" ||tokens[i + 5] == "tuesday" ||tokens[i + 5] == "wednesday") {
                    continue;
                }
                else {
                    throw week_name_exception();
                }
            }
            if (stoi(tokens[4]) < 0) {
                throw negative_patients_exception();
            }
            doctor.name = tokens[2];
            doctor.specialty = tokens[3];
            doctor.number_of_patients = stoi(tokens[4]);
            doctor.number_of_days = tokens.size() - 5;
            for (int i = 0; i < tokens.size() - 5 ; i++) {
                doctor.working_days[i] = tokens[5 + i];
            }
            //doctor <name> added with specialty <specialty> with <number of patients> patients
            cout << "doctor " << doctor.name << " added with specialty " << doctor.specialty << " with " << doctor.number_of_patients << " patients" << endl; ;
            doctors.push_back(doctor);
            return true;
        }
    //change the number of patients of doctor <name> to <number of patients>
    if (tokens.size() == 10 &&tokens[0] == "change" &&tokens[1] == "the" &&tokens[2] == "number" &&tokens[3] == "of" &&tokens[4] == "patients" &&tokens[5] == "of" &&tokens[6] == "doctor" &&tokens[8] == "to") {

        string doctor_name = tokens[7];
        int new_patient_count = stoi(tokens[9]);
        bool doctor_found = false;

        if (new_patient_count < 0) {
            throw negative_patients_exception();
        }

        for (int i = 0; i < doctors.size(); ++i) {
            if (doctors[i].name == doctor_name) {
                doctor_found = true;
                for (int d = 0; d < doctors[i].number_of_days; ++d) {
                    if (doctors[i].daily_patients[d].size() > new_patient_count) {
                        doctors[i].daily_patients[d].resize(new_patient_count);
                    }
                }
                doctors[i].number_of_patients = new_patient_count;
                cout << "number of patients changed" << endl;
                return true;
            }
        }

        if (!doctor_found) {
            throw DoctorNotFoundException();
        }
    }

    const set<string> valid_days = {"saturday", "sunday", "monday", "tuesday", "wednesday"};
    //change the working days of doctor <name> to <working days>
    if (tokens.size() >= 8 &&
       tokens[0] == "change" &&
       tokens[1] == "the" &&
       tokens[2] == "working" &&
       tokens[3] == "days" &&
       tokens[4] == "of" &&
       tokens[5] == "doctor" &&
       tokens[7] == "to") {

        string doctor_name = tokens[6];
        vector<string> new_working_days;
        set<string> new_days_set;

        for (int i = 8; i < tokens.size(); ++i) {
            if (valid_days.find(tokens[i]) == valid_days.end()) {
                throw week_name_exception();
            }
            new_working_days.push_back(tokens[i]);
            new_days_set.insert(tokens[i]);
        }

        bool doctor_found = false;
        for (int i = 0; i < doctors.size(); ++i) {
            if (doctors[i].name == doctor_name) {
                doctor_found = true;

                for (int old_day = 0; old_day < doctors[i].number_of_days; ++old_day) {
                    string old_working_day = doctors[i].working_days[old_day];
                    if (new_days_set.find(old_working_day) == new_days_set.end()) {
                        doctors[i].daily_patients[old_day].clear();
                    }
                }

                doctors[i].number_of_days = new_working_days.size();
                for (int j = 0; j < new_working_days.size(); ++j) {
                    doctors[i].working_days[j] = new_working_days[j];
                }
                for (int j = new_working_days.size(); j < 7; ++j) {
                    doctors[i].working_days[j] = "";
                    doctors[i].daily_patients[j].clear();
                }

                cout << "working days changed" << endl;
                return true;
            }
        }
        if (!doctor_found) {
            throw DoctorNotFoundException();
        }
       }


    if (tokens.size() == 3 && tokens[0] == "delete" && tokens[1] == "patient") {
        string patient_name = tokens[2];
        bool found = false;
        for (int i = 0; i < doctors.size(); ++i) {
            Doctors& doc = doctors[i];
            for (int d = 0; d < doc.number_of_days; ++d) {
                for (int p = 0; p < doc.daily_patients[d].size(); ++p) {
                    if (doc.daily_patients[d][p].name == patient_name) {
                        doc.daily_patients[d].erase(doc.daily_patients[d].begin() + p);
                        cout << "appointment deleted on day " << doc.working_days[d] << " doctor " << doc.name << endl;
                        found = true;
                        return true;
                    }
                }
            }
        }
        if (!found) {
            throw PatientNotFoundException();
        }
    }


vector<string> week_days = {"saturday", "sunday", "monday", "tuesday", "wednesday"};

    if (tokens.size() == 4 && tokens[0] == "add" && tokens[1] == "patient") {
        string patient_name = tokens[2];
        string needed_specialty = tokens[3];

        vector<int> specialist_doctor_indices;
        for (int i = 0; i < doctors.size(); ++i) {
            if (doctors[i].specialty == needed_specialty)
                specialist_doctor_indices.push_back(i);
        }

        if (specialist_doctor_indices.empty()) {
            throw DoctorForProblemNotExistException();
        }

        for (int day = 0; day < week_days.size(); ++day) {
            for (int idx : specialist_doctor_indices) {
                Doctors& doc = doctors[idx];
                int day_index = -1;
                for (int j = 0; j < doc.number_of_days; ++j) {
                    if (doc.working_days[j] == week_days[day]) {
                        day_index = j;
                        break;
                    }
                }
                if (day_index == -1) continue;

                if (doc.daily_patients[day_index].size() < doc.number_of_patients) {
                    Patients new_patient;
                    new_patient.name = patient_name;
                    new_patient.specialty = needed_specialty;
                    doc.daily_patients[day_index].push_back(new_patient);

                    cout << "appointment set on day " << week_days[day] << " doctor " << doc.name << endl;
                    return true;
                }
            }
        }

        throw DoctorsWithThisSpecialtyBusyException();
    }




    if (tokens.size() == 2 && tokens[0] == "patients" && tokens[1] == "list") {
        for (int wd = 0; wd < week_days.size(); ++wd) {
            cout << week_days[wd] << ":" << endl;
            int patient_no = 1;
            for (int i = 0; i < doctors.size(); ++i) {
                Doctors& doc = doctors[i];
                for (int d = 0; d < doc.number_of_days; ++d) {
                    if (doc.working_days[d] == week_days[wd]) {
                        for (int p = 0; p < doc.daily_patients[d].size(); ++p) {
                            cout << "patient" << patient_no << " " << doc.daily_patients[d][p].name << " : doctor " << doc.name << endl;
                            patient_no++;
                        }
                    }
                }
            }
        }
        return true;
    }



    throw invalid_Exception();
    }


// ------ do not change main() function ------
int main() {
    string line;
    bool cond = true;
    while (cond) {
        getline(cin, line);
        try {
            cond = inputHandler(line);
        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}
