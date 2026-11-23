#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iomanip>

using namespace std;

class UndirectedGraph {
private:
    int n;
    vector<string> vertices;
    unordered_map<string, int> v_idx;
    vector<vector<int>> adj_matrix;
    vector<vector<int>> adj_list;

public:
    UndirectedGraph(const vector<string>& v_list) {
        n = static_cast<int>(v_list.size());
        vertices = v_list;
        for (int i = 0; i < n; ++i) {
            v_idx[vertices[i]] = i;
        }
        adj_matrix.assign(n, vector<int>(n, 0));
        adj_list.assign(n, vector<int>());
    }

    void addEdge(const string& u_str, const string& v_str) {
        int u = v_idx.at(u_str);
        int v = v_idx.at(v_str);
        adj_matrix[u][v] = 1;
        adj_matrix[v][u] = 1;
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    bool hasEdgeMatrix(const string& u_str, const string& v_str) const {
        int u = v_idx.at(u_str);
        int v = v_idx.at(v_str);
        return adj_matrix[u][v] == 1;
    }

    bool hasEdgeList(const string& u_str, const string& v_str) const {
        int u = v_idx.at(u_str);
        int v = v_idx.at(v_str);
        for (int nb : adj_list[u]) {
            if (nb == v) return true;
        }
        return false;
    }

    void printAdjacencyMatrix() const {
        cout << "\n[1] 인접 행렬 (Adjacency Matrix) - O(V^2) 공간\n";
        cout << setw(4) << " ";
        for (const auto& v : vertices) {
            cout << setw(4) << v;
        }
        cout << "\n";
        for (int i = 0; i < n; ++i) {
            cout << setw(4) << vertices[i];
            for (int j = 0; j < n; ++j) {
                cout << setw(4) << adj_matrix[i][j];
            }
            cout << "\n";
        }
    }

    void printAdjacencyList() const {
        cout << "\n[2] 인접 리스트 (Adjacency List) - O(V + E) 공간\n";
        for (int i = 0; i < n; ++i) {
            cout << "  " << vertices[i] << " -> ";
            for (size_t j = 0; j < adj_list[i].size(); ++j) {
                cout << vertices[adj_list[i][j]];
                if (j + 1 < adj_list[i].size()) cout << " -> ";
            }
            cout << " -> NULL\n";
        }
    }

    void dfsHelper(int curr, unordered_set<int>& visited, vector<string>& order) const {
        visited.insert(curr);
        order.push_back(vertices[curr]);
        for (int nxt : adj_list[curr]) {
            if (visited.find(nxt) == visited.end()) {
                dfsHelper(nxt, visited, order);
            }
        }
    }

    vector<string> dfs(const string& start_str) const {
        int start = v_idx.at(start_str);
        unordered_set<int> visited;
        vector<string> order;
        dfsHelper(start, visited, order);
        return order;
    }

    vector<string> bfs(const string& start_str) const {
        int start = v_idx.at(start_str);
        unordered_set<int> visited;
        queue<int> q;
        vector<string> order;

        visited.insert(start);
        q.push(start);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            order.push_back(vertices[curr]);

            for (int nxt : adj_list[curr]) {
                if (visited.find(nxt) == visited.end()) {
                    visited.insert(nxt);
                    q.push(nxt);
                }
            }
        }
        return order;
    }
};

int main() {
    vector<string> V = {"A", "B", "C", "D", "E", "F"};
    vector<pair<string, string>> E = {
        {"A", "B"}, {"A", "C"}, {"B", "D"},
        {"B", "E"}, {"C", "F"}, {"E", "F"}
    };

    UndirectedGraph g(V);
    for (const auto& edge : E) {
        g.addEdge(edge.first, edge.second);
    }

    g.printAdjacencyMatrix();
    g.printAdjacencyList();

    cout << "\n[3] 두 정점 연결 여부 확인 (O(1) 행렬 vs 순차 탐색 리스트)\n";
    vector<pair<string, string>> queries = {{"A", "B"}, {"A", "F"}, {"E", "F"}};
    for (const auto& q_pair : queries) {
        cout << "  (" << q_pair.first << ", " << q_pair.second << ") "
             << "행렬=" << (g.hasEdgeMatrix(q_pair.first, q_pair.second) ? "연결됨" : "미연결")
             << ", 리스트=" << (g.hasEdgeList(q_pair.first, q_pair.second) ? "연결됨" : "미연결") << "\n";
    }

    vector<string> dfs_res = g.dfs("A");
    vector<string> bfs_res = g.bfs("A");

    cout << "\n[4] DFS 탐색 순서 (시작: A): ";
    for (size_t i = 0; i < dfs_res.size(); ++i) {
        cout << dfs_res[i] << (i + 1 < dfs_res.size() ? " -> " : "\n");
    }

    cout << "[5] BFS 탐색 순서 (시작: A): ";
    for (size_t i = 0; i < bfs_res.size(); ++i) {
        cout << bfs_res[i] << (i + 1 < bfs_res.size() ? " -> " : "\n");
    }

    return 0;
}