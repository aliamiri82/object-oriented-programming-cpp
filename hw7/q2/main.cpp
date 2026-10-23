#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <tuple>
#include <exception>
using namespace std;

class InvalidCommandException : public exception {
    string message;
public:
    InvalidCommandException(const string& msg = "INVALID COMMAND")
        : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};
class Vertex {
public:
    int id;
    double weight;
    Vertex(int id, double weight) : id(id), weight(weight) {}
};

enum GraphType {
    DIRECTED,
    UNDIRECTED,
    WEIGHTED,
    KNOWLEDGE
};

class Graph {
public:
    int graph_id;
    GraphType type;
    vector<Vertex> vertices;
    unordered_map<int, int> vertex_id_to_index;
    vector<vector<double>> adjacency_matrix;

    Graph(int id) : graph_id(id), type(DIRECTED) {}

    bool add_vertex(int v_id, double weight) {
        if (vertex_id_to_index.count(v_id)) return false;
        vertices.push_back(Vertex(v_id, weight));
        vertex_id_to_index[v_id] = vertices.size() - 1;
        int n = vertices.size();
        adjacency_matrix.resize(n);
        for (size_t k = 0; k < adjacency_matrix.size(); ++k)
            adjacency_matrix[k].resize(n, 0.0);

        return true;
    }

    bool add_edge(int start_id, int end_id, double weight) {
        if (!vertex_id_to_index.count(start_id) || !vertex_id_to_index.count(end_id))
            return false;
        int i = vertex_id_to_index[start_id];
        int j = vertex_id_to_index[end_id];
        adjacency_matrix[i][j] = weight;
        if (type == UNDIRECTED)
            adjacency_matrix[j][i] = weight;
        return true;
    }

    bool delete_vertex(int v_id) {
        if (!vertex_id_to_index.count(v_id)) return false;
        int idx = vertex_id_to_index[v_id];

        vertices.erase(vertices.begin() + idx);

        adjacency_matrix.erase(adjacency_matrix.begin() + idx);
        for (size_t k = 0; k < adjacency_matrix.size(); ++k)
            adjacency_matrix[k].erase(adjacency_matrix[k].begin() + idx);


        vertex_id_to_index.clear();
        for (int i = 0; i < vertices.size(); ++i)
            vertex_id_to_index[vertices[i].id] = i;
        return true;
    }

    bool delete_edge(int start_id, int end_id) {
        if (!vertex_id_to_index.count(start_id) || !vertex_id_to_index.count(end_id))
            return false;
        int i = vertex_id_to_index[start_id];
        int j = vertex_id_to_index[end_id];
        adjacency_matrix[i][j] = 0.0;
        if (type == UNDIRECTED)
            adjacency_matrix[j][i] = 0.0;
        return true;
    }


    bool edit_vertex_weight(int v_id, double new_weight) {
        if (!vertex_id_to_index.count(v_id)) return false;
        int idx = vertex_id_to_index[v_id];
        vertices[idx].weight = new_weight;
        return true;
    }

    bool edit_edge_weight(int start_id, int end_id, double new_weight) {
        if (!vertex_id_to_index.count(start_id) || !vertex_id_to_index.count(end_id))
            return false;
        int i = vertex_id_to_index[start_id];
        int j = vertex_id_to_index[end_id];
        adjacency_matrix[i][j] = new_weight;
        if (type == UNDIRECTED)
            adjacency_matrix[j][i] = new_weight;
        return true;
    }
    int edge_count() const {
        int count = 0;
        int n = vertices.size();
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (adjacency_matrix[i][j] != 0.0)
                    count++;
        return count;
    }

    void show() const {
        int n = vertices.size();
        int m = edge_count();

        cout << graph_id << " " << n << " " << m << "\n";

        vector<pair<int, double>> verts;
        for (size_t i = 0; i < vertices.size(); ++i)
            verts.emplace_back(vertices[i].id, vertices[i].weight);

        sort(verts.begin(), verts.end());
        for (size_t i = 0; i < verts.size(); ++i)
            cout << graph_id << " " << verts[i].first << " " << fixed << setprecision(2) << verts[i].second << "\n";


        vector<tuple<int, int, double>> edges;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (adjacency_matrix[i][j] != 0.0)
                    edges.emplace_back(vertices[i].id, vertices[j].id, adjacency_matrix[i][j]);
        sort(edges.begin(), edges.end());

        for (size_t i = 0; i < edges.size(); ++i)
            cout << graph_id << " "<< get<0>(edges[i]) << " "<< get<1>(edges[i]) << " "<< fixed << setprecision(2) << get<2>(edges[i]) << "\n";
    }
};



class GraphManager {
private:
    unordered_map<int, shared_ptr<Graph>> graphs;
public:
    bool add_graph(int id) {
        if (graphs.count(id)) return false;
        graphs[id] = make_shared<Graph>(id);
        return true;
    }
    shared_ptr<Graph> get_graph(int id) {
        if (graphs.count(id)) return graphs[id];
        return nullptr;
    }
    bool add_vertex_to_graph(int g_id, int v_id, double weight) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->add_vertex(v_id, weight);
    }
    bool add_edge_to_graph(int g_id, int v_start, int v_end, double weight) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->add_edge(v_start, v_end, weight);
    }
    bool delete_vertex_from_graph(int g_id, int v_id) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->delete_vertex(v_id);
    }
    bool delete_edge_from_graph(int g_id, int v_start, int v_end) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->delete_edge(v_start, v_end);
    }
    bool edit_vertex_weight_in_graph(int g_id, int v_id, double new_weight) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->edit_vertex_weight(v_id, new_weight);
    }
    bool edit_edge_weight_in_graph(int g_id, int v_start, int v_end, double new_weight) {
        auto g = get_graph(g_id);
        if (!g) return false;
        return g->edit_edge_weight(v_start, v_end, new_weight);
    }
    bool show_graph(int g_id) const {
        auto it = graphs.find(g_id);
        if (it != graphs.end()) {
            it->second->show();
            return true;
        }
        return false;
    }

};


int main() {
    int n;
    std::cin >> n;
    std::cin.ignore();

    GraphManager manager;
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);

        try {
            std::vector<std::string> tokens;
            std::istringstream iss(line);
            std::string token;
            while (iss >> token)
                tokens.push_back(token);

            if (tokens.empty())
                throw InvalidCommandException();

            std::string cmd = tokens[0];

            if (cmd == "NEW_GRAPH" && tokens.size() == 2) {
                int id = std::stoi(tokens[1]);
                if (!manager.add_graph(id))
                    throw InvalidCommandException();
            }
            else if (cmd == "ADD_VERTEX" && tokens.size() == 4) {
                int g_id = std::stoi(tokens[1]);
                int v_id = std::stoi(tokens[2]);
                double weight = std::stod(tokens[3]);
                if (!manager.add_vertex_to_graph(g_id, v_id, weight))
                    throw InvalidCommandException();
            }
            else if (cmd == "ADD_EDGE" && tokens.size() == 5) {
                int g_id = std::stoi(tokens[1]);
                int start_v = std::stoi(tokens[2]);
                int end_v = std::stoi(tokens[3]);
                double weight = std::stod(tokens[4]);
                if (!manager.add_edge_to_graph(g_id, start_v, end_v, weight))
                    throw InvalidCommandException();
            }
            else if (cmd == "DEL_VERTEX" && tokens.size() == 3) {
                int g_id = std::stoi(tokens[1]);
                int v_id = std::stoi(tokens[2]);
                if (!manager.delete_vertex_from_graph(g_id, v_id))
                    throw InvalidCommandException();
            }
            else if (cmd == "DEL_EDGE" && tokens.size() == 4) {
                int g_id = std::stoi(tokens[1]);
                int start_v = std::stoi(tokens[2]);
                int end_v = std::stoi(tokens[3]);
                if (!manager.delete_edge_from_graph(g_id, start_v, end_v))
                    throw InvalidCommandException();
            }
            else if (cmd == "EDIT_VERTEX" && tokens.size() == 4) {
                int g_id = std::stoi(tokens[1]);
                int v_id = std::stoi(tokens[2]);
                double weight = std::stod(tokens[3]);
                if (!manager.edit_vertex_weight_in_graph(g_id, v_id, weight))
                    throw InvalidCommandException();
            }
            else if (cmd == "EDIT_EDGE" && tokens.size() == 5) {
                int g_id = std::stoi(tokens[1]);
                int start_v = std::stoi(tokens[2]);
                int end_v = std::stoi(tokens[3]);
                double weight = std::stod(tokens[4]);
                if (!manager.edit_edge_weight_in_graph(g_id, start_v, end_v, weight))
                    throw InvalidCommandException();
            }
            else if (cmd == "SHOW_GRAPH" && tokens.size() == 2) {
                int g_id = std::stoi(tokens[1]);
                if (!manager.show_graph(g_id)) // باید خروجی داشته باشد
                    throw InvalidCommandException();
            }
            else {
                throw InvalidCommandException();
            }
        }
        catch (const std::exception&) {
            std::cout << "INVALID COMMAND" << std::endl;
        }
    }
    return 0;
}

