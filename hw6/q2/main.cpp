#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <algorithm>
using namespace std;


class Node {
    int    id_;
    string name_;
    double v_ = 0.0;
public:
    Node(int id, const string& n) : id_(id), name_(n) {}
    int    id()  const { return id_; }
    double v()   const { return v_; }
    void   setV(double v) { v_ = v; }
    const string& name() const { return name_; }
};

class Component {
protected:
    string name_;
    Node*  n1_;
    Node*  n2_;
public:
    Component(const string& nm, Node* a, Node* b)
        : name_(nm), n1_(a), n2_(b) {}
    virtual double getVoltage() const = 0;
    virtual double getCurrent() const = 0;
    virtual ~Component() = default;
    const string& name() const { return name_; }
};

class Resistor : public Component {
    double R_;
public:
    Resistor(const string& nm, Node* a, Node* b, double r)
        : Component(nm, a, b), R_(r) {}
    double getVoltage() const override { return fabs(n1_->v() - n2_->v()); }
    double getCurrent() const override { return getVoltage() / R_; }
    double conductance() const { return 1.0 / R_; }
    Node*  n1() const { return n1_; }
    Node*  n2() const { return n2_; }
};

class VoltageSource : public Component {
    double V_;
    double I_ = 0.0;
public:
    VoltageSource(Node* a, Node* b, double v)
        : Component("VIN", a, b), V_(v) {}
    double getVoltage() const override { return fabs(V_); }
    double getCurrent() const override { return I_; }
    void   setCurrent(double i) { I_ = i; }
    Node*  n1() const { return n1_; }
    Node*  n2() const { return n2_; }
    double value() const { return V_; }
};
vector<double> gaussianElimination(vector<vector<double>> A, vector<double> b) {
    int N = A.size();
    for (int i = 0; i < N; ++i) {
        // Pivoting
        int maxRow = i;
        for (int k = i+1; k < N; ++k) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i]))
                maxRow = k;
        }
        swap(A[i], A[maxRow]);
        swap(b[i], b[maxRow]);

        // Eliminate below
        for (int k = i+1; k < N; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < N; ++j)
                A[k][j] -= factor * A[i][j];
            b[k] -= factor * b[i];
        }
    }

    vector<double> x(N);
    for (int i = N-1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i+1; j < N; ++j)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
    return x;
}

class Circuit {
    unordered_map<string, unique_ptr<Node>>      nodes_;
    unordered_map<string, unique_ptr<Component>> comps_;
    vector<Resistor*>  resistors_;
    VoltageSource*     src_ = nullptr;
    vector<Node*>      grounds_;
    bool               solved_ = false;

    Node* fetch(const string& key) const {
        auto it = nodes_.find(key);
        return it == nodes_.end() ? nullptr : it->second.get();
    }

public:
    void addNode(const string& n) {
        nodes_[n] = make_unique<Node>(static_cast<int>(nodes_.size()), n);
    }

    void addRes(double R, const string& nm, const string& a, const string& b) {
        Node *n1 = fetch(a), *n2 = fetch(b);
        auto r = make_unique<Resistor>(nm, n1, n2, R);
        resistors_.push_back(r.get());
        comps_[nm] = move(r);
    }

    void addVsrc(double V, const string& a, const string& b) {
        Node *n1 = fetch(a), *n2 = fetch(b);
        auto v = make_unique<VoltageSource>(n1, n2, V);
        src_ = v.get();
        comps_["VIN"] = move(v);
    }

    void setGround(const string& n) {
        Node* g = fetch(n);
        if (g) grounds_.push_back(g);
    }

    double readElemI(const string& nm) { if (!solved_) solve(); return fabs(comps_[nm]->getCurrent()); }
    double readElemV(const string& nm) { if (!solved_) solve(); return comps_[nm]->getVoltage(); }
    double readNodeV(const string& n ) {
        if (!solved_) solve();
        Node* target = fetch(n);
        if (grounds_.empty()) return target->v();
        return target->v() - grounds_[0]->v();
    }

    void solve() {
        solved_ = true;

        unordered_map<int, int> nodeToIndex;
        int idx = 0;
        for (auto& pair : nodes_) {
            Node* node = pair.second.get();
            bool isGround = any_of(grounds_.begin(), grounds_.end(), [node](Node* g) { return g == node; });
            if (isGround) {
                node->setV(0.0);
            } else {
                nodeToIndex[node->id()] = idx++;
            }
        }

        int N = nodeToIndex.size();
        int M = N + 1;

        vector<vector<double>> A(M, vector<double>(M, 0.0));
        vector<double> b(M, 0.0);

        for (auto* r : resistors_) {
            int id1 = r->n1()->id();
            int id2 = r->n2()->id();
            double g = r->conductance();

            bool has1 = nodeToIndex.count(id1);
            bool has2 = nodeToIndex.count(id2);

            if (has1) A[nodeToIndex[id1]][nodeToIndex[id1]] += g;
            if (has2) A[nodeToIndex[id2]][nodeToIndex[id2]] += g;
            if (has1 && has2) {
                A[nodeToIndex[id1]][nodeToIndex[id2]] -= g;
                A[nodeToIndex[id2]][nodeToIndex[id1]] -= g;
            }
        }

        int idPos = src_->n1()->id();
        int idNeg = src_->n2()->id();

        int vsrcRow = N;

        if (nodeToIndex.count(idPos)) {
            int i = nodeToIndex[idPos];
            A[vsrcRow][i] = 1.0;
            A[i][vsrcRow] = 1.0;
        }

        if (nodeToIndex.count(idNeg)) {
            int j = nodeToIndex[idNeg];
            A[vsrcRow][j] = -1.0;
            A[j][vsrcRow] = -1.0;
        }

        b[vsrcRow] = src_->value();

        vector<double> x = gaussianElimination(A, b);

        for (auto& pair : nodes_) {
            Node* node = pair.second.get();
            if (any_of(grounds_.begin(), grounds_.end(), [&](Node* g) { return g == node; })) {
                node->setV(0.0);
            } else {
                node->setV(x[nodeToIndex[node->id()]]);
            }
        }

        src_->setCurrent(x[N]);
    }
};


int main() {
    Circuit circuit;
    while (true) {
        vector<string> tokens;
        string line ;
        string word;
        tokens.clear();
        getline(cin, line);
        stringstream ss(line);
        while (ss >> word) {
            tokens.push_back(word);
        }
        if (tokens[0] == "end") break;
        if (tokens[0] == "add" && tokens[1] == "node") {
            circuit.addNode(tokens[2]);
        }
        if (tokens[0] == "add" && tokens[1] == "resistor") {
            double resistance = stod(tokens[2]);
            string resName = tokens[3];
            string node1 = tokens[4];
            string node2 = tokens[5];
            circuit.addRes(resistance, resName, node1, node2);
        }
        if (tokens[0] == "add" && tokens[1] == "voltage" && tokens[2] == "source") {
            double voltage = stod(tokens[3]);
            string node1 = tokens[4];
            string node2 = tokens[5];
            circuit.addVsrc(voltage, node1, node2);
        }
        if (tokens[0] == "add" && tokens[1] == "ground") {
            circuit.setGround(tokens[2]);
        }
        if (tokens[0] == "read" && tokens[1] == "current") {
            string compName = tokens[2];
            double current = circuit.readElemI(compName);
            cout << fixed << setprecision(2);
            cout << compName << " current = " << fabs(current) << " amps" << endl;
        }
        if (tokens[0] == "read" && tokens[1] == "voltage") {
            string compName = tokens[2];
            double voltage = circuit.readElemV(compName);
            cout << fixed << setprecision(2);
            cout << compName << " voltage = " << voltage << " volts" << endl;
        }
        if (tokens[0] == "read" && tokens[1] == "node" && tokens[2] == "voltage") {
            string nodeName = tokens[3];
            double voltage = circuit.readNodeV(nodeName);
            cout << fixed << setprecision(2);
            cout << nodeName << " voltage = " << voltage << " volts" << endl;
        }







    }
    return 0;
}
