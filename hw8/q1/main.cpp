#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <cmath>
#include <numeric>


using namespace std;

// ======== Response Class ==========
class Response {
public:
    string text;
    bool hasError = false;
    string errorText;
    void errorHappen(bool e) { hasError = e; }
    void setErrorText(const string &et) { errorText = et; }
    void setText(const string &t) { text = t; }
    void print() const {
        if (hasError)
            cout << errorText << endl;
        else
            cout << text << endl;
    }
};

// ======== Data Classes =============
class Data {
public:
    string word;
    virtual void setWord(string w) = 0;
    string getWord() const { return word; }
    virtual ~Data() = default;
};

class StringData : public Data {
public:
    void setWord(string w) override {
        word = w;
    }
};

class VectorData : public Data {
public:
    vector<int> vect;
    void setWord(string w) override {
        word = w;
        vect.clear();
        for (char c : w)
            vect.push_back((int)c);
    }
    vector<int> getVector() const { return vect; }
    void setVector(vector<int> v) { vect = v; }
};

// ======== DataSet Class =============
class DataSet {
    vector<shared_ptr<Data>> data;
public:
    string name;
    // کلیدی: حتماً رفرنس برگردان!
    vector<shared_ptr<Data>>& getDataAll() { return data; }
    shared_ptr<Data> getDataAt(int i) {
        if (i < 0 || i >= (int)data.size()) return nullptr;
        return data[i];
    }
    void clear() { data.clear(); }
    // type: 0->StringData, 1->VectorData
    void cinData(int wordcount, int type, istream &in=cin) {
        data.clear();
        cout << "lets push " << wordcount << " words to " << name << " !" << endl;
        string tmp;
        getline(in, tmp); // read remain newline
        for (int i = 0; i < wordcount; ++i) {
            string word;
            getline(in, word);
            // remove trailing \r if present (windows)
            if(!word.empty() && word.back() == '\r') word.pop_back();
            shared_ptr<Data> d;
            if (type == 0) d = make_shared<StringData>();
            else d = make_shared<VectorData>();
            d->setWord(word);
            data.push_back(d);
            cout << "pushed word: \"" << word << "\"" << endl;
        }
    }
};

// ======== Abstract PI Model Class ============
class PIModel {
public:
    string name;
    int version;
    shared_ptr<DataSet> trainData;
    PIModel(string n, int v) : name(n), version(v) {}
    virtual void train(shared_ptr<DataSet> DS) = 0;
    virtual void sayYourNameAndVersion() const {
        cout << name << "_v" << version << " -> Hi! I'm " << name << ". You are using version " << version << "!" << endl;
    }
    virtual Response response(string input) = 0;
    virtual ~PIModel() = default;
};

// ========= Parrots ==========
class Parrots : public PIModel {
public:
    Parrots(int v) : PIModel("Parrots", v) {}
    void train(shared_ptr<DataSet> DS) override {
        trainData = DS;
        cout << "Trained Parrots_v" << version << " with dataset " << DS->name << endl;
    }
    Response response(string input) override {
        Response res;
        while (!input.empty() && isspace(input.front())) input.erase(input.begin());
        if (input == "$INTRO") {
            stringstream ss;
            ss << name << "_v" << version << " -> Hi! I'm " << name << ". You are using version " << version << "!";
            res.setText(ss.str());
        } else {
            stringstream ss;
            ss << name << "_v" << version << " -> " << input;
            res.setText(ss.str());
        }
        return res;
    }
};

// ========= Grammarly ==========
int compare(const string &s1, const string &s2) {
    int m = s1.size(), n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
    return dp[m][n];
}
class Grammarly : public PIModel {
public:
    bool autocorrect = false;
    Grammarly(int v) : PIModel("Grammarly", v) {}
    void setAutocorrect(bool ac) { autocorrect = ac; }
    void train(shared_ptr<DataSet> DS) override {
        trainData = DS;
        cout << "Trained Grammarly_v" << version << " with dataset " << DS->name << endl;
    }
    Response response(string input) override {
        Response res;
        while (!input.empty() && isspace(input.front())) input.erase(input.begin());
        if (input == "$INTRO") {
            stringstream ss;
            ss << name << "_v" << version << " -> Hi! I'm " << name << ". You are using version " << version << "!";
            res.setText(ss.str());
        } else if (input == "$ATON") {
            autocorrect = true;
            stringstream ss;
            ss << name << "_v" << version << " -> autocorrect <= true";
            res.setText(ss.str());
        } else if (input == "$ATOFF") {
            autocorrect = false;
            stringstream ss;
            ss << name << "_v" << version << " -> autocorrect <= false";
            res.setText(ss.str());
        } else if (!autocorrect) {
            stringstream ss;
            ss << name << "_v" << version << " -> " << input;
            res.setText(ss.str());
        } else {
            // autocorrect == true
            int minDist = 1e9, minIdx = 0;
            string resStr;
            auto dset = trainData;
            for (int i = 0; i < (int)dset->getDataAll().size(); ++i) {
                auto d = dset->getDataAt(i);
                int dist = compare(d->getWord(), input);
                if (dist < minDist ||
                    (dist == minDist && abs((int)d->getWord().size()-(int)input.size()) < abs((int)resStr.size()-(int)input.size())) ||
                    (dist == minDist && abs((int)d->getWord().size()-(int)input.size()) == abs((int)resStr.size()-(int)input.size()) && i < minIdx)
                ) {
                    minDist = dist;
                    minIdx = i;
                    resStr = d->getWord();
                }
            }
            stringstream ss;
            ss << name << "_v" << version << " -> " << resStr;
            res.setText(ss.str());
        }
        return res;
    }
};

// ========= MathGeek ==========
int evaluateExpr(string expr) {
    // accepts very simple expr: a+b, a-b, a*b, a/b (without spaces)
    int a=0, b=0;
    char op = 0;
    size_t i = 6;
    while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.')) i++;
    if (i == expr.size()) return -1;
    string a_str = expr.substr(6, i-6);
    op = expr[i];
    string b_str = expr.substr(i+1);
    try {
        a = stoi(a_str);
        b = stoi(b_str);
    } catch (...) { return -1; }
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return b == 0 ? 0 : a / b;
    return -1;
}

class MathGeek : public PIModel {
public:
    int DataVectorSize = 5;
    MathGeek(int v) : PIModel("MathGeek", v) {}

    void train(shared_ptr<DataSet> DS) override {
        // هر بار train، یک دیتاست مستقل VectorData برای این مدل بساز:
        auto newDS = make_shared<DataSet>();
        newDS->name = DS->name;
        for (auto& d : DS->getDataAll()) {
            auto vd = make_shared<VectorData>();
            vd->setWord(d->getWord());
            auto v = vd->getVector();
            if ((int)v.size() > DataVectorSize)
                v = vector<int>(v.end() - DataVectorSize, v.end());
            else
                while ((int)v.size() < DataVectorSize) v.push_back(32);
            vd->setVector(v);
            newDS->getDataAll().push_back(vd);
        }
        trainData = newDS; // فقط این مدل باید به دیتاست جدید اشاره کند!
        cout << "Trained MathGeek_v" << version << " with dataset " << newDS->name << endl;
    }

    void updateVectorDataSet() {
        if (!trainData) return;
        for (auto& d : trainData->getDataAll()) {
            auto vd = dynamic_pointer_cast<VectorData>(d);
            if (!vd) continue;
            auto v = vd->getVector();
            if ((int)v.size() > DataVectorSize)
                v = vector<int>(v.end() - DataVectorSize, v.end());
            else
                while ((int)v.size() < DataVectorSize) v.push_back(32);
            vd->setVector(v);
        }
    }

    Response response(string input) override {
        Response res;
        // حذف فاصله‌های ابتدایی
        while (!input.empty() && isspace(input.front())) input.erase(input.begin());

        // $INTRO
        if (input == "$INTRO") {
            stringstream ss;
            ss << name << "_v" << version << " -> Hi! I'm " << name << ". You are using version " << version << "!";
            res.setText(ss.str());
            return res;
        }
        // تغییر سایز وکتور (DataVectorSize)
        if (input.substr(0, 5) == "$DVS_") {
            string param = input.substr(5);
            int val = -1;
            try {
                size_t op = param.find_first_of("+-*/");
                if (op != string::npos) {
                    int a = stoi(param.substr(0, op));
                    int b = stoi(param.substr(op + 1));
                    char oper = param[op];
                    if (oper == '+') val = a + b;
                    else if (oper == '-') val = a - b;
                    else if (oper == '*') val = a * b;
                    else if (oper == '/') { if (b != 0) val = a / b; }
                } else if (all_of(param.begin(), param.end(), ::isdigit)) {
                    val = stoi(param);
                }
            } catch (...) { val = -1; }
            if (val < 1) {
                stringstream ss;
                ss << name << "_v" << version << " -> !!! Invalid DataVectorSize !!! My DataVectorSize = " << DataVectorSize;
                res.setText(ss.str());
            } else {
                DataVectorSize = val;
                updateVectorDataSet();
                stringstream ss;
                ss << name << "_v" << version << " -> DataVectorSize <= " << DataVectorSize;
                res.setText(ss.str());
            }
            return res;
        }

        // اگر دیتاست خالی است
        if (!trainData || trainData->getDataAll().empty()) {
            stringstream ss;
            ss << name << "_v" << version << " ->";
            res.setText(ss.str());
            return res;
        }

        // ساخت وکتور ورودی و اصلاح اندازه
        VectorData vd;
        vd.setWord(input);
        auto v = vd.getVector();
        if ((int)v.size() > DataVectorSize)
            v = vector<int>(v.end() - DataVectorSize, v.end());
        else
            while ((int)v.size() < DataVectorSize) v.push_back(32);
        int inSum = accumulate(v.begin(), v.end(), 0);

        // پیدا کردن نزدیک‌ترین
        int minDiff = 1e9, minIdx = 0;
        string resStr = dynamic_pointer_cast<VectorData>(trainData->getDataAt(0))->getWord();
        for (int i = 0; i < (int)trainData->getDataAll().size(); ++i) {
            auto d = trainData->getDataAt(i);
            auto vd2 = dynamic_pointer_cast<VectorData>(d);
            if (!vd2) continue;
            auto v2 = vd2->getVector();
            int sum2 = accumulate(v2.begin(), v2.end(), 0);
            int diff = abs(sum2 - inSum);
            if (i == 0 || diff < minDiff || (diff == minDiff && i < minIdx)) {
                minDiff = diff;
                minIdx = i;
                resStr = vd2->getWord();
            }
        }
        stringstream ss;
        ss << name << "_v" << version << " -> " << resStr;
        res.setText(ss.str());
        return res;
    }
};



// ============ Manager & Main ===============
map<string, shared_ptr<PIModel>> PIs;
map<string, shared_ptr<DataSet>> dataSets;
map<string, int> pi_name_to_version;

shared_ptr<PIModel> createPI(const string &name) {
    int v = 1;
    if (pi_name_to_version.count(name))
        v = ++pi_name_to_version[name];
    else
        pi_name_to_version[name] = 1;
    string key = name + "_v" + to_string(v);
    if (name == "Parrots") {
        auto pi = make_shared<Parrots>(v);
        PIs[key] = pi;
        cout << "Parrots_v" << v << " created" << endl;
        return pi;
    }
    if (name == "Grammarly") {
        auto pi = make_shared<Grammarly>(v);
        PIs[key] = pi;
        cout << "Grammarly_v" << v << " created" << endl;
        return pi;
    }
    if (name == "MathGeek") {
        auto pi = make_shared<MathGeek>(v);
        PIs[key] = pi;
        cout << "MathGeek_v" << v << " created" << endl;
        return pi;
    }
    cout << "!!! Error : Invalid name for PI !!!" << endl;
    return nullptr;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


void run() {
    string line;
    int readingDataset = 0;
    int datasetWordCount = 0;
    string currentDatasetName;
    shared_ptr<DataSet> currentDataset;
    vector<string> datasetBuffer;

    while (getline(cin, line)) {
        if (readingDataset > 0) {
            string trimmed = trim(line);
            datasetBuffer.push_back(trimmed);
            cout << "pushed word: \"" << trimmed << "\"" << endl;
            readingDataset--;
            if (readingDataset == 0) {
                currentDataset->name = currentDatasetName;
                for (const string& s : datasetBuffer) {
                    shared_ptr<Data> d = make_shared<StringData>();
                    d->setWord(s);
                    currentDataset->getDataAll().push_back(d);
                }
                dataSets[currentDatasetName] = currentDataset; // بازنویسی دیتاست
                datasetBuffer.clear();
            }
            continue;
        }

        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty())
            continue;

        if (line == "!end")
            break;

        if (line[0] == '!') {
            // ایجاد PI
            if (line.find("!create PI :") == 0) {
                string piName = line.substr(13);
                piName.erase(0, piName.find_first_not_of(" \t"));
                if (piName == "Parrots" || piName == "Grammarly" || piName == "MathGeek") {
                    createPI(piName);
                } else {
                    cout << "!!! Error : Invalid name for PI !!!" << endl;
                }
                continue;
            }
            // ایجاد Dataset
            if (line.find("!create") == 0 && line.find(": word count :") != string::npos) {
                size_t p1 = line.find("!create") + 7;
                size_t p2 = line.find(": word count :");
                currentDatasetName = line.substr(p1, p2 - p1);
                currentDatasetName.erase(remove_if(currentDatasetName.begin(), currentDatasetName.end(), ::isspace), currentDatasetName.end());
                datasetWordCount = stoi(line.substr(p2 + 15));
                cout << "lets push " << datasetWordCount << " words to " << currentDatasetName << " !" << endl;
                currentDataset = make_shared<DataSet>();
                datasetBuffer.clear();
                readingDataset = datasetWordCount;
                continue;
            }
            // آموزش PI با دیتاست
            if (line.find("!train") == 0) {
                size_t v1 = line.find("!train") + 6;
                size_t v2 = line.find("with");
                if (v2 == string::npos) { cout << "Invalid Command" << endl; continue; }
                string piName = line.substr(v1, v2 - v1);
                piName.erase(0, piName.find_first_not_of(" \t_"));
                piName.erase(piName.find_last_not_of(" \t") + 1);
                string dsName = line.substr(v2 + 4);
                dsName.erase(0, dsName.find_first_not_of(" \t"));
                auto piIt = PIs.find(piName);
                auto dsIt = dataSets.find(dsName);
                if (piIt == PIs.end() || dsIt == dataSets.end()) {
                    cout << "Invalid Command" << endl;
                } else {
                    // --- MathGeek: دیتاست جدید مخصوص همین PI ساخته می‌شود! ---
                    if (dynamic_pointer_cast<MathGeek>(piIt->second)) {
                        auto dsOld = dsIt->second;
                        auto dsNew = make_shared<DataSet>();
                        dsNew->name = dsOld->name;
                        for (auto &d : dsOld->getDataAll()) {
                            auto vd = make_shared<VectorData>();
                            vd->setWord(d->getWord());
                            dsNew->getDataAll().push_back(vd);
                        }
                        piIt->second->train(dsNew); // فقط مدل همین MathGeek به این دیتا وصل می‌شود
                    }
                    // Parrots/Grammarly: دیتاست اصلی را بدون تغییر می‌گیرند
                    else if (dynamic_pointer_cast<Parrots>(piIt->second) || dynamic_pointer_cast<Grammarly>(piIt->second)) {
                        piIt->second->train(dsIt->second);
                    }
                    else {
                        cout << "Invalid Command" << endl;
                    }
                }
                continue;
            }
            cout << "Invalid Command" << endl;
            continue;
        }
        // دستور PI مانند: Parrots_v1 <- hello
        size_t arrow = line.find("<-");
        if (arrow != string::npos) {
            string pi = line.substr(0, arrow);
            string input = line.substr(arrow + 2);
            pi.erase(pi.find_last_not_of(" \t") + 1);
            input.erase(0, input.find_first_not_of(" \t"));
            auto piIt = PIs.find(pi);
            if (piIt == PIs.end()) {
                cout << "Invalid Command" << endl;
                continue;
            }
            auto res = piIt->second->response(input);
            res.print();
            continue;
        }
        cout << "Invalid Command" << endl;
    }
}




int main() {
    run();
    return 0;
}
