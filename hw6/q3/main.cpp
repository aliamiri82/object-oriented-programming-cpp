#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <iomanip>
#include <ctime>
#include <random>
using namespace std;
class Course;
bool isValidStudentID(const string& id) {
    return id.size() == 9 && all_of(id.begin(), id.end(), ::isdigit);
}
bool isValid_ID(const string& id) {
    return id.size() == 10 && all_of(id.begin(), id.end(), ::isdigit);
}
string formatGrade(float grade) {
    stringstream ss;
    ss << fixed << setprecision(2) << grade;
    string result = ss.str();

    if (result.substr(result.find('.')) == ".00") {
        result = result.substr(0, result.find('.'));
    }
    else if (result.back() == '0' && result[result.size() - 2] == '.') {
        result = result.substr(0, result.size() - 2);
    }

    return result;
}
bool isNumeric(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

class Person {
public:
    string  name;
    string  national_id;
    string  phone_number;
    Person(const string& name,
           const string& nid,
           const string& phone)
        : name(name), national_id(nid), phone_number(phone) {}

    virtual void displayInfo() const
    {
        cout << "Name: "        << name         << '\n'
             << "National ID: " << national_id  << '\n'
             << "Phone: "       << phone_number << '\n';
    }

    virtual ~Person() = default;
};

class Student : public Person {
public:
    string  majo;
    string  student_id;
    string  entrance_year;
    vector<Course*> enrolledCourses;
    Student(const string& name,
            const string& nid,
            const string& phone,
            const string& majo,
            const string& st_id,
            const string& year)
        : Person(name, nid, phone),
          majo(majo), student_id(st_id), entrance_year(year) {}

    void displayInfo() const override
    {
        Person::displayInfo();
        cout << "Major: "         << majo         << '\n'
             << "Student ID: "    << student_id    << '\n'
             << "Entrance Year: " << entrance_year << "\n\n";
    }
};

class Assistant : public Student {
    string role;
public:
    Assistant(const string& name,
              const string& nid,
              const string& phone,
              const string& majo,
              const string& st_id,
              const string& year,
              const string& role)
        : Student(name, nid, phone, majo, st_id, year),
          role(role) {}

    void displayInfo() const override
    {
        Student::displayInfo();
        cout << "Role: " << role << "\n\n";
    }
    string getRole() const {
        return role;
    }
};

class Professor : public Person {
    string department;
    int    salary;
public:
    Professor(const string& name,
              const string& nid,
              const string& phone,
              const string& dept,
              int salary)
        : Person(name, nid, phone),
          department(dept), salary(salary) {}

    void displayInfo() const override
    {
        Person::displayInfo();
        cout << "Department: " << department << '\n'
             << "Salary: "     << salary     << "\n\n";
    }
    string getDepartment() const {
        return department;
    }

    int getSalary() const {
        return salary;
    }
};

class Course {
public:
    string               course_name;
    Professor*           instructor;
    vector<Assistant*>   assistants;
    vector<Student*>     students;
    map<Student*, float> grades;

    Course(const string& name)
    : course_name(name), instructor(nullptr) {}

    void addAssistant(Assistant* a) { assistants.push_back(a); }
    void addStudent  (Student*  s) { students.push_back(s);   }
    void addProf  (Professor* q) { instructor = q; }
    void displayCourseInfo() const
    {
        cout << "=== Course: " << course_name << " ===\n";
        cout << "\n-- Instructor --\n";
        instructor->displayInfo();

        cout << "-- Assistants (" << assistants.size() << ") --\n";
        for (const auto* a : assistants) a->displayInfo();

        cout << "-- Students ("   << students.size() << ") --\n";
        for (const auto* s : students)  s->displayInfo();
    }
};

int main() {
    vector<string> tokens;
    string line ;
    string word;
    vector<Person*> persons;
    vector<Course*> courses;
    vector<Professor*> professors;
    vector<Assistant*> assistants;
    vector<Student*> students;
    while (true) {
        tokens.clear();
        getline(cin, line);
        stringstream ss(line);
        while (ss >> word) {
            tokens.push_back(word);
        }
        //Add student <first name> <last name> <national id> <phone> <major> <student id> <year>
        if (tokens[0] == "exit") break;
        if (tokens[0] == "Add" && tokens[1] == "student" && tokens.size() >= 9) {
            if (tokens[2].empty() || tokens[3].empty()) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }

            if (!isNumeric(tokens[4])) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }
            string student_id2 = tokens[tokens.size() - 2];

            if (!isValidStudentID(student_id2)) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }
            for (char& c : tokens[2]) if (c == '_') c = ' ';
            for (char& c : tokens[3]) if (c == '_') c = ' ';
            string fullName = tokens[2] + " " + tokens[3];

            string national_id = tokens[4];
            string phone = tokens[5];

            string majo = "";
            int majoStart = 6;
            int majoEnd = tokens.size() - 3;

            for (int i = majoStart; i <= majoEnd; ++i) {
                if (!majo.empty()) majo += " ";
                majo += tokens[i];
            }

            string student_id = tokens[tokens.size() - 2];
            string entrance_year = tokens[tokens.size() - 1];

            bool exists = false;
            for (Person* p : persons) {
                if (p->name == fullName && p->national_id == national_id) {
                    exists = true;
                    break;
                }
            }

            if (exists) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                Student* s = new Student(fullName, national_id, phone, majo, student_id, entrance_year);
                students.push_back(s);
                persons.push_back(s);
                cout << "Student added successfully!" << endl;
            }
            continue;
        }


        if (tokens[0] == "Add" && tokens[1] == "professor" && tokens.size() >= 8) {
            for (char& c : tokens[2]) if (c == '_') c = ' ';
            for (char& c : tokens[3]) if (c == '_') c = ' ';
            string fullName = tokens[2] + " " + tokens[3];

            string national_id = tokens[4];
            string phone = tokens[5];
            int salary = stoi(tokens[tokens.size() - 1]);
            if (!isValid_ID(national_id)) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }
            string department = "";
            for (int i = 6; i < tokens.size() - 1; ++i) {
                if (!department.empty()) department += " ";
                department += tokens[i];
            }

            bool exists = false;
            for (Person* p : persons) {
                if (p->name == fullName && p->national_id == national_id) {
                    exists = true;
                    break;
                }
            }

            if (exists) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                Professor* prof = new Professor(fullName, national_id, phone, department, salary);
                professors.push_back(prof);
                persons.push_back(prof);
                cout << "Professor added successfully!" << endl;
            }
            continue;
        }


        if (tokens[0] == "Add" && tokens[1] == "TA" && tokens.size() >= 10) {
            for (char& c : tokens[2]) if (c == '_') c = ' ';
            for (char& c : tokens[3]) if (c == '_') c = ' ';
            string fullName = tokens[2] + " " + tokens[3];

            string national_id = tokens[4];
            string phone = tokens[5];

            string student_id = tokens[tokens.size() - 3];
            string entrance_year = tokens[tokens.size() - 2];

            if (!isValidStudentID(student_id)) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }
            string role = tokens[tokens.size() - 1];
            for (char& c : role) if (c == '_') c = ' ';

            string majo = "";
            for (int i = 6; i < tokens.size() - 3; ++i) {
                if (!majo.empty()) majo += " ";
                majo += tokens[i];
            }

            bool exists = false;
            for (Person* p : persons) {
                if (p->name == fullName && p->national_id == national_id) {
                    exists = true;
                    break;
                }

            }

            if (exists) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                Assistant* TA = new Assistant(fullName, national_id, phone, majo, student_id, entrance_year, role);
                assistants.push_back(TA);
                students.push_back(TA);
                persons.push_back(TA);
                cout << "Assistant added successfully!" << endl;
            }
            continue;
        }


        if (tokens[0] == "Create" && tokens[1] == "course" && tokens.size() >= 3) {
            string courseName = "";
            for (int i = 2; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }

            bool exists = false;
            for (Course* c : courses) {
                if (c->course_name == courseName) {
                    exists = true;
                    break;
                }
            }

            if (exists) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                Course* newCourse = new Course(courseName);
                courses.push_back(newCourse);
                cout << "Course created successfully!" << endl;
            }
            continue;
        }

        if (tokens[0] == "Enroll" && tokens[1] == "student" &&
    tokens[5] == "in" && tokens[6] == "course" &&
    tokens.size() >= 8)
        {
            for (char& c : tokens[2]) if (c == '_') c = ' ';
            for (char& c : tokens[3]) if (c == '_') c = ' ';
            string fullName = tokens[2] + " " + tokens[3];

            string studentID = tokens[4];

            string courseName = "";
            for (int i = 7; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }

            Student* foundStudent = nullptr;
            for (Student* s : students) {
                if (s->name == fullName && s->student_id == studentID) {
                    foundStudent = s;
                    break;
                }
            }

            Course* foundCourse = nullptr;
            for (Course* c : courses) {
                if (c->course_name == courseName) {
                    foundCourse = c;
                    break;
                }
            }

            if (!foundStudent || !foundCourse) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                // بررسی تکراری نبودن
                bool alreadyEnrolled = false;
                for (Student* s : foundCourse->students) {
                    if (s == foundStudent) {
                        alreadyEnrolled = true;
                        break;
                    }
                }

                if (alreadyEnrolled) {
                    cout << "OOPs, something went wrong!" << endl;
                } else {
                    foundCourse->addStudent(foundStudent);
                    foundStudent->enrolledCourses.push_back(foundCourse); // ← اضافه کردن
                    cout << "Student enrolled in the course successfully!" << endl;
                }
            }
            continue;
        }

        if (tokens[0] == "Assign" && tokens[1] == "professor" &&
    tokens[5] == "to" && tokens[6] == "course" && tokens.size() >= 7)
        {
            string first = tokens[2], last = tokens[3];
            for (int i = 0; i < first.size(); i++) if (first[i] == '_') first[i] = ' ';
            for (int i = 0; i < last.size(); i++)  if (last[i]  == '_') last[i]  = ' ';

            string fullName   = first + " " + last;
            string nationalID = tokens[4];
            string courseName = "";
            for (int i = 7; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }

            // جستجوی استاد
            Professor* foundProfessor = nullptr;
            for (int i = 0; i < professors.size(); i++) {
                if (professors[i]->name == fullName &&
                    professors[i]->national_id == nationalID) {
                    foundProfessor = professors[i];
                    break;
                    }
            }

            Course* foundCourse = nullptr;
            for (int i = 0; i < courses.size(); i++) {
                if (courses[i]->course_name == courseName) {
                    foundCourse = courses[i];
                    break;
                }
            }

            // بررسی
            if (!foundProfessor || !foundCourse) {
                cout << "OOPs, something went wrong!" << endl;
            } else {
                if (foundCourse->instructor == foundProfessor) {
                    cout << "OOPs, something went wrong!" << endl;
                } else {
                    foundCourse->addProf(foundProfessor);
                    cout << "Professor assigned to the course successfully!" << endl;
                }
            }
            continue;
        }
        if (tokens[0] == "Assign" && tokens[1] == "grade" &&
    tokens.size() >= 9)
{
    float grade = stof(tokens[2]);

    if (grade < 0.0f || grade > 20.0f) {
        cout << "OOPs, something went wrong!" << endl;
        continue;
    }

    int studentIndex = -1;
    for (int i = 5; i < tokens.size() - 3; ++i) {
        if (tokens[i] == "for" && tokens[i + 1] == "student") {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1 || tokens.size() < studentIndex + 4) {
        cout << "OOPs, something went wrong!" << endl;
        continue;
    }

    string courseName = "";
    for (int i = 5; i < studentIndex; ++i) {
        if (!courseName.empty()) courseName += " ";
        courseName += tokens[i];
    }

    string first = tokens[studentIndex + 2];
    string last  = tokens[studentIndex + 3];
    for (char& c : first) if (c == '_') c = ' ';
    for (char& c : last)  if (c == '_') c = ' ';
    string fullName = first + " " + last;
    string studentID = tokens[studentIndex + 4];

    Student* foundStudent = nullptr;
    for (Student* s : students) {
        if (s->name == fullName && s->student_id == studentID) {
            foundStudent = s;
            break;
        }
    }

    Course* foundCourse = nullptr;
    for (Course* c : courses) {
        if (c->course_name == courseName) {
            foundCourse = c;
            break;
        }
    }

    if (!foundStudent || !foundCourse) {
        cout << "OOPs, something went wrong!" << endl;
        continue;
    }

    bool enrolled = false;
    for (Student* s : foundCourse->students) {
        if (s == foundStudent) {
            enrolled = true;
            break;
        }
    }

    if (!enrolled || foundCourse->grades.count(foundStudent)) {
        cout << "OOPs, something went wrong!" << endl;
        continue;
    }

    foundCourse->grades[foundStudent] = grade;
    cout << "Course graded successfully!" << endl;
            continue;
}


if (tokens[0] == "Find" && tokens[1] == "student" && tokens.size() == 5) {
    string first = tokens[2], last = tokens[3];
    for (char& c : first) if (c == '_') c = ' ';
    for (char& c : last)  if (c == '_') c = ' ';

    string fullName = first + " " + last;
    string studentID = tokens[4];

    Student* targetStudent = nullptr;
    Assistant* asTA = nullptr;

    for (Student* s : students) {
        if (s->name == fullName && s->student_id == studentID) {
            targetStudent = s;
            break;
        }
    }

    if (!targetStudent) {
        cout << "OOPs, something went wrong!" << endl;
        continue;
    }

    for (Assistant* a : assistants) {
        if (a == targetStudent) {
            asTA = dynamic_cast<Assistant*>(targetStudent);
            break;
        }
    }

    cout << "Name: " << targetStudent->name << endl;
    cout << "National ID: " << targetStudent->national_id << endl;
    cout << "Phone number: " << targetStudent->phone_number << endl;
    cout << "Student ID: " << targetStudent->student_id << endl;
    cout << "Major: " << targetStudent->majo << endl;
    cout << "Entrance Year: " << targetStudent->entrance_year << endl;

    float sum = 0;
    int graded = 0;
    for (Course* c : targetStudent->enrolledCourses) {
        if (c->grades.count(targetStudent)) {
            float g = c->grades[targetStudent];
            sum += g;
            graded++;
        }
    }
    cout << "GPA: ";
    if (graded > 0)
        cout << formatGrade(sum / graded) << endl;
    else
        cout << "0" << endl;

    cout << "Courses:" << endl;
    if (targetStudent->enrolledCourses.empty()) {
        cout << "This student is not enrolled in any course." << endl;
    } else {
        for (Course* c : targetStudent->enrolledCourses) {
            cout << c->course_name << ": ";
            if (c->grades.count(targetStudent)) {
                float g = c->grades[targetStudent];
                cout << formatGrade(g) << endl;
            } else {
                cout << "[Not Graded]" << endl;
            }
        }
    }

    if (asTA) {
        cout << "Role: " << asTA->getRole() << endl;
        cout << "Assigned Courses:" << endl;
        vector<string> assigned;
        for (Course* c : courses) {
            for (Assistant* a : c->assistants) {
                if (a == asTA) {
                    assigned.push_back(c->course_name);
                    break;
                }
            }
        }
        if (assigned.empty()) {
            cout << "This student is not assistant in any course." << endl;
        } else {
            for (const string& name : assigned)
                cout << name << endl;
        }
    }
    continue;
}

        if (tokens[0] == "Find" && tokens[1] == "professor" && tokens.size() == 5) {
            string first = tokens[2], last = tokens[3];
            for (int i = 0; i < first.size(); i++) if (first[i] == '_') first[i] = ' ';
            for (int i = 0; i < last.size(); i++)  if (last[i]  == '_') last[i]  = ' ';
            string fullName = first + " " + last;
            string nationalID = tokens[4];

            Professor* foundProfessor = nullptr;

            for (int i = 0; i < professors.size(); ++i) {
                if (professors[i]->name == fullName &&
                    professors[i]->national_id == nationalID) {
                    foundProfessor = professors[i];
                    break;
                    }
            }

            if (!foundProfessor) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }

            cout << "Name: " << foundProfessor->name << endl;
            cout << "National ID: " << foundProfessor->national_id << endl;
            cout << "Phone number: " << foundProfessor->phone_number << endl;
            cout << "Department: " << foundProfessor->getDepartment() << endl;
            cout << "Salary: " << foundProfessor->getSalary() << endl;

            vector<string> assignedCourses;
            for (int i = 0; i < courses.size(); ++i) {
                if (courses[i]->instructor == foundProfessor) {
                    assignedCourses.push_back(courses[i]->course_name);
                }
            }

            cout << "Assigned Courses:" << endl;
            if (assignedCourses.empty()) {
                cout << "This professor does not teach any course." << endl;
            } else {
                for (const string& c : assignedCourses)
                    cout << c << endl;
            }
            continue;
        }

        if (tokens[0] == "Find" && tokens[1] == "course" && tokens.size() >= 3) {
            string courseName = "";
            for (int i = 2; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }

            Course* targetCourse = nullptr;
            for (Course* c : courses) {
                if (c->course_name == courseName) {
                    targetCourse = c;
                    break;
                }
            }

            if (!targetCourse) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }

            cout << "Course: " << targetCourse->course_name << endl;

            cout << "Instructor: ";
            if (targetCourse->instructor == nullptr) {
                cout << "[None]" << endl;
            } else {
                cout << targetCourse->instructor->name << endl;
            }

            cout << "Assistants:" << endl;
            if (targetCourse->assistants.empty()) {
                cout << "There are no assistants for this course." << endl;
            } else {
                for (Assistant* a : targetCourse->assistants) {
                    cout << a->name << " " << a->student_id << endl;
                }
            }

            cout << "Number of participants: " << targetCourse->students.size() << endl;
            continue;
        }


        if (tokens[0] == "List" && tokens[1] == "students" &&
            tokens[2] == "in" && tokens[3] == "course" && tokens.size() >= 5)
        {
            string courseName = "";
            for (int i = 4; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }

            Course* targetCourse = nullptr;
            for (Course* c : courses) {
                if (c->course_name == courseName) {
                    targetCourse = c;
                    break;
                }
            }

            if (!targetCourse) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }

            cout << "Students in " << courseName << ":" << endl;

            if (targetCourse->students.empty()) {
                cout << "There are no enrolled students in this course." << endl;
            } else {
                for (Student* s : targetCourse->students) {
                    cout << s->name << " " << s->student_id << endl;
                }
            }
            continue;
        }


        if (tokens[0] == "Assign" && tokens[1] == "assistant" &&
    tokens[5] == "to" && tokens[6] == "course" &&
    tokens.size() >= 8)
        {
            string first = tokens[2], last = tokens[3];
            for (int i = 0; i < first.size(); ++i)
                if (first[i] == '_') first[i] = ' ';
            for (int i = 0; i < last.size(); ++i)
                if (last[i] == '_') last[i] = ' ';

            string fullName = first + " " + last;
            string studentID = tokens[4];
            string courseName = "";
            for (int i = 7; i < tokens.size(); ++i) {
                if (!courseName.empty()) courseName += " ";
                courseName += tokens[i];
            }
            Assistant* foundAssistant = nullptr;
            for (Assistant* a : assistants) {
                if (a->name == fullName && a->student_id == studentID) {
                    foundAssistant = a;
                    break;
                }
            }

            Course* foundCourse = nullptr;
            for (Course* c : courses) {
                if (c->course_name == courseName) {
                    foundCourse = c;
                    break;
                }
            }

            if (!foundAssistant || !foundCourse) {
                cout << "OOPs, something went wrong!" << endl;
                continue;
            }

            bool found = false;
            for (Assistant* a : foundCourse->assistants) {
                if (a == foundAssistant) {
                    cout << "OOPs, something went wrong!" << endl;
                    found = true;
                }
            }
            if (found) {continue;}

            foundCourse->addAssistant(foundAssistant);
            cout << "Assistant assigned to the course successfully!" << endl;
            continue;
        }
        cout << "OOPs, something went wrong!" << endl;
    }
}



