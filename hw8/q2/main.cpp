#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

struct Flashcard {
    string id;
    string question;
    string answer;
    time_t created_at;
    int added_day;
};

struct AnswerLog {
    int day;
    bool correct;
};

class LeitnerBoxes {
public:
    deque<Flashcard> daily;
    deque<Flashcard> weekly;
    deque<Flashcard> monthly;
    vector<Flashcard> learned;
};

class LeitnerSystem {
private:
    LeitnerBoxes boxes;
    deque<Flashcard> reviewingNow;
    set<string> reviewedTodayIDs;
    map<string, vector<AnswerLog>> answerLogs;
    set<int> participationDays;
    int streak = 0;
    int currentDay = 1;
    int idCounter = 0;
    bool reviewedToday = false;

public:
    void run();

private:
    string generateID() {
        return "fc_" + to_string(idCounter++);
    }

    void showStreak() {
        cout << "Your current streak is: " << streak << endl;
    }

    void addFlashcard(const string& question, const string& answer) {
        Flashcard fc;
        fc.id = generateID();
        fc.question = question;
        fc.answer = answer;
        fc.created_at = time(nullptr);
        fc.added_day = currentDay;
        boxes.daily.push_back(fc);
        cout << "flashcard added to the daily box" << endl;
    }

    void reviewToday(int number) {
        deque<Flashcard*> candidates;

        if (currentDay % 30 == 0) {
            for (int i = 0; i < boxes.monthly.size(); i++) {
                if (reviewedTodayIDs.count(boxes.monthly[i].id) == 0)
                    candidates.push_back(&boxes.monthly[i]);
                if (candidates.size() == number) break;
            }
        }
        if (candidates.size() < number && currentDay % 7 == 0) {
            for (int i = 0; i < boxes.weekly.size(); i++) {
                if (reviewedTodayIDs.count(boxes.weekly[i].id) == 0)
                    candidates.push_back(&boxes.weekly[i]);
                if (candidates.size() == number) break;
            }
        }
        if (candidates.size() < number) {
            for (int i = 0; i < boxes.daily.size(); i++) {
                if (reviewedTodayIDs.count(boxes.daily[i].id) == 0)
                    candidates.push_back(&boxes.daily[i]);
                if (candidates.size() == number) break;
            }
        }

        if (candidates.empty()) {
            cout << "You've completed today's review" << endl;
            return;
        }

        cout << "Flashcards:" << endl;
        reviewingNow.clear();
        for (int i = 0; i < candidates.size(); i++) {
            cout << candidates[i]->question << endl;
            reviewingNow.push_back(*candidates[i]);
            reviewedTodayIDs.insert(candidates[i]->id);
        }
        reviewedToday = true;
    }

    void processAnswers(const vector<string>& userAnswers) {
        cout << "Your Answers:" << endl;
        participationDays.insert(currentDay);
        reviewedToday = true;

        for (size_t i = 0; i < userAnswers.size() && i < reviewingNow.size(); ++i) {
            Flashcard fc = reviewingNow[i];
            bool isCorrect = (userAnswers[i] == fc.answer);

            AnswerLog log;
            log.day = currentDay;
            log.correct = isCorrect;
            answerLogs[fc.id].push_back(log);

            if (isCorrect) {
                cout << "Your answer was correct!" << endl;
                moveToNextBox(fc);
            } else {
                cout << "Your answer was incorrect. The correct answer is: " << fc.answer << endl;
                moveToPreviousBox(fc);
            }
        }
        reviewingNow.clear();
    }

    void moveToNextBox(const Flashcard& fc) {
        bool wasInDaily = isInBox(fc.id, boxes.daily);
        bool wasInWeekly = isInBox(fc.id, boxes.weekly);
        bool wasInMonthly = isInBox(fc.id, boxes.monthly);
        removeFromBoxes(fc.id);
        if (wasInDaily) {
            boxes.weekly.push_back(fc);
        } else if (wasInWeekly) {
            boxes.monthly.push_back(fc);
        } else if (wasInMonthly) {
            boxes.learned.push_back(fc);
        }
    }

    void moveToPreviousBox(const Flashcard& fc) {
        bool wasInDaily = isInBox(fc.id, boxes.daily);
        bool wasInWeekly = isInBox(fc.id, boxes.weekly);
        bool wasInMonthly = isInBox(fc.id, boxes.monthly);

        if (wasInDaily) {
            return;
        }

        removeFromBoxes(fc.id);

        if (wasInWeekly) {
            boxes.daily.push_back(fc);
        } else if (wasInMonthly) {
            boxes.weekly.push_back(fc);
        } else {
            boxes.daily.push_back(fc);
        }
    }

    bool isInBox(const string& id, deque<Flashcard>& box) {
        for (size_t i = 0; i < box.size(); ++i) {
            if (box[i].id == id) return true;
        }
        return false;
    }

    void removeFromBoxes(const string& id) {
        removeFromBox(id, boxes.daily);
        removeFromBox(id, boxes.weekly);
        removeFromBox(id, boxes.monthly);
    }

    void removeFromBox(const string& id, deque<Flashcard>& box) {
        for (deque<Flashcard>::iterator it = box.begin(); it != box.end(); ++it) {
            if (it->id == id) {
                box.erase(it);
                return;
            }
        }
    }

    void getReport(int startDay, int endDay) {
        int correct = 0;
        int incorrect = 0;

        for (map<string, vector<AnswerLog>>::iterator it = answerLogs.begin(); it != answerLogs.end(); ++it) {
            vector<AnswerLog> logs = it->second;
            for (size_t i = 0; i < logs.size(); ++i) {
                if (logs[i].day >= startDay && logs[i].day <= endDay) {
                    if (logs[i].correct) correct++;
                    else incorrect++;
                }
            }
        }

        if (startDay == endDay)
            cout << "Day: " << startDay << endl;
        else
            cout << "Day: " << startDay << " to " << endDay << endl;

        cout << "Correct Answers: " << correct << endl;
        cout << "Incorrect Answers: " << incorrect << endl;
        cout << "Total: " << (correct + incorrect) << endl;
    }

    void getProgressReport() {
        cout << "Challenge Progress Report:" << endl;
        cout << "Day of the Challenge: " << currentDay << endl;
        cout << "Streak: " << streak << endl;
        cout << "Total Days Participated: " << participationDays.size() << endl;
        cout << "Mastered Flashcards: " << boxes.learned.size() << endl;
    }

    void nextDay() {
        if (!reviewedToday) {
            if (currentDay % 30 == 0) {
                while (!boxes.monthly.empty()) {
                    boxes.weekly.push_back(boxes.monthly.front());
                    boxes.monthly.pop_front();
                }
            }
            if (currentDay % 7 == 0) {
                while (!boxes.weekly.empty()) {
                    boxes.daily.push_back(boxes.weekly.front());
                    boxes.weekly.pop_front();
                }
            }
        }

        if (participationDays.count(currentDay))
            streak++;
        else
            streak = 0;
        currentDay++;
        reviewedToday = false;
        reviewedTodayIDs.clear();
        cout << "--------------------------------------------------" << endl;
        cout << "It is day " << currentDay << " of your journey." << endl;
        cout << "Your current streak is: " << streak << endl;
    }
};

void LeitnerSystem::run() {
    string input;
    bool isAnswering = false;
    vector<string> answers;
    int expectedAnswers = 0;

    while (getline(cin, input)) {
        if (isAnswering) {
            answers.push_back(input);
            if (answers.size() == expectedAnswers) {
                processAnswers(answers);
                isAnswering = false;
                answers.clear();
                expectedAnswers = 0;
            }
            continue;
        }

        if (input == "streak") {
            showStreak();
        }
        else if (input == "add flashcard") {
            string question, answer;
            if (!getline(cin, question)) break;
            if (!getline(cin, answer)) break;
            addFlashcard(question, answer);
        }
        else if (input.rfind("review today", 0) == 0) {
            istringstream ss(input);
            string cmd, today;
            int n;
            ss >> cmd >> today >> n;
            reviewToday(n);
            if (!reviewingNow.empty()) {
                isAnswering = true;
                expectedAnswers = reviewingNow.size();
            }
        }
        else if (input.rfind("get report", 0) == 0) {
            istringstream ss(input);
            string cmd, rep;
            int s, e;
            ss >> cmd >> rep >> s >> e;
            getReport(s, e);
        }
        else if (input == "get progress report") {
            getProgressReport();
        }
        else if (input == "next day") {
            nextDay();
        }
        else {
            cout << "Invalid command." << endl;
        }
    }
}

int main() {
    LeitnerSystem system;
    system.run();
    return 0;
}
