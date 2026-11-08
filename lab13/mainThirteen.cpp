#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <Windows.h>

using namespace std;

// Система непересекающихся множеств (DSU)
class DSU {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // сжатие пути
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return;

        // объединение по рангу
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        }
        else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// Структура для ребра (для Краскала)
struct Edge {
    int u, v, weight;
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Класс графа (взвешенный, неориентированный)
class Graph {
private:
    int V;
    vector<vector<int>> adjMatrix; // 0 - нет ребра

public:
    Graph(int vertices) : V(vertices) {
        adjMatrix.assign(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adjMatrix[u][v] = weight;
            adjMatrix[v][u] = weight;
        }
    }

    // Алгоритм Прима с priority_queue
    // Возвращает {общий вес, список рёбер}
    pair<int, vector<pair<int, int>>> primMST(int start = 0) const {
        vector<int> minEdge(V, INT_MAX);
        vector<bool> inMST(V, false);
        vector<int> parent(V, -1);
        int totalWeight = 0;
        vector<pair<int, int>> mstEdges;

        // priority_queue хранит пары {вес, вершина}, минимум наверху
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        minEdge[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int w = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            // Если вершина уже включена в MST, пропускаем
            if (inMST[u]) continue;

            inMST[u] = true;
            totalWeight += w;
            if (parent[u] != -1) {
                mstEdges.push_back({ parent[u], u });
            }

            for (int v = 0; v < V; ++v) {
                if (adjMatrix[u][v] != 0 && !inMST[v] && adjMatrix[u][v] < minEdge[v]) {
                    minEdge[v] = adjMatrix[u][v];
                    parent[v] = u;
                    pq.push({ minEdge[v], v });
                }
            }
        }

        // Проверка на связность
        for (int i = 0; i < V; ++i) {
            if (!inMST[i]) {
                return { -1, {} }; // граф несвязный
            }
        }

        return { totalWeight, mstEdges };
    }

    // Вспомогательная функция для сбора всех рёбер (для Краскала)
    vector<Edge> getAllEdges() const {
        vector<Edge> edges;
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                if (adjMatrix[i][j] != 0) {
                    edges.push_back(Edge(i, j, adjMatrix[i][j]));
                }
            }
        }
        return edges;
    }

    // Алгоритм Краскала с DSU
    // Возвращает {общий вес, список рёбер}
    pair<int, vector<pair<int, int>>> kruskalMST() const {
        vector<Edge> edges = getAllEdges();
        sort(edges.begin(), edges.end());

        DSU dsu(V);
        int totalWeight = 0;
        vector<pair<int, int>> mstEdges;

        for (const Edge& e : edges) {
            if (!dsu.connected(e.u, e.v)) {
                dsu.unite(e.u, e.v);
                totalWeight += e.weight;
                mstEdges.push_back({ e.u, e.v });
                if (mstEdges.size() == V - 1) break;
            }
        }

        if (mstEdges.size() != V - 1) {
            return { -1, {} }; // граф несвязный
        }

        return { totalWeight, mstEdges };
    }

    // Вывод компонент связности (для несвязного графа)
    void printConnectedComponents() const {
        vector<bool> visited(V, false);
        int componentNum = 1;
        cout << "Компоненты связности:" << endl;
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                cout << "Компонента " << componentNum++ << ": ";
                queue<int> q;
                visited[i] = true;
                q.push(i);
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    cout << v << " ";
                    for (int u = 0; u < V; ++u) {
                        if (adjMatrix[v][u] != 0 && !visited[u]) {
                            visited[u] = true;
                            q.push(u);
                        }
                    }
                }
                cout << endl;
            }
        }
    }

    // Вывод MST
    void printMST(const pair<int, vector<pair<int, int>>>& result, const string& algorithmName) const {
        if (result.first == -1) {
            cout << algorithmName << ": граф несвязный!" << endl;
            printConnectedComponents();
            return;
        }

        cout << algorithmName << ":" << endl;
        cout << "  Рёбра MST:" << endl;
        for (const auto& edge : result.second) {
            cout << "    " << edge.first << " - " << edge.second
                << " (вес: " << adjMatrix[edge.first][edge.second] << ")" << endl;
        }
        cout << "  Общий вес: " << result.first << endl;
    }
};

// Генерация случайного графа с заданной плотностью
Graph generateRandomGraph(int V, int densityPercent) {
    Graph g(V);
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            if (rand() % 100 < densityPercent) {
                int weight = rand() % 100 + 1;
                g.addEdge(i, j, weight);
            }
        }
    }
    return g;
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(static_cast<unsigned int>(time(0)));

    cout << "=== Лабораторная работа №13. Алгоритмы Прима и Краскала (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация на тестовом графе (из методички)
    cout << "1. Демонстрация на тестовом графе:" << endl;
    Graph testGraph(5);
    testGraph.addEdge(0, 1, 2);
    testGraph.addEdge(0, 3, 6);
    testGraph.addEdge(1, 2, 3);
    testGraph.addEdge(1, 3, 8);
    testGraph.addEdge(1, 4, 5);
    testGraph.addEdge(2, 4, 7);
    testGraph.addEdge(3, 4, 9);

    auto primResult = testGraph.primMST(0);
    auto kruskalResult = testGraph.kruskalMST();

    testGraph.printMST(primResult, "Алгоритм Прима");
    cout << endl;
    testGraph.printMST(kruskalResult, "Алгоритм Краскала");

    // 2. Обработка несвязного графа
    cout << "\n2. Обработка несвязного графа:" << endl;
    Graph disconnectedGraph(5);
    disconnectedGraph.addEdge(0, 1, 3);
    disconnectedGraph.addEdge(1, 2, 2);
    // Вершины 3 и 4 изолированы
    auto discPrim = disconnectedGraph.primMST(0);
    disconnectedGraph.printMST(discPrim, "Алгоритм Прима (несвязный граф)");

    // 3. Сравнение производительности на плотных графах
    cout << "\n3. Сравнение производительности (плотный граф):" << endl;
    cout << "V\tПрим (мкс)\tКраскал (мкс)\tВес MST" << endl;
    cout << "----------------------------------------------" << endl;

    int sizes[] = { 10, 20, 50, 100 };
    for (int V : sizes) {
        Graph denseGraph = generateRandomGraph(V, 80); // 80% плотность

        auto start = chrono::high_resolution_clock::now();
        auto primRes = denseGraph.primMST(0);
        auto end = chrono::high_resolution_clock::now();
        long long primTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        auto kruskalRes = denseGraph.kruskalMST();
        end = chrono::high_resolution_clock::now();
        long long kruskalTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << V << "\t" << primTime << "\t\t" << kruskalTime << "\t\t" << primRes.first << endl;
    }

    // 4. Сравнение производительности на разреженных графах
    cout << "\n4. Сравнение производительности (разреженный граф, 20%):" << endl;
    cout << "V\tПрим (мкс)\tКраскал (мкс)\tВес MST" << endl;
    cout << "----------------------------------------------" << endl;

    for (int V : sizes) {
        Graph sparseGraph = generateRandomGraph(V, 20);

        auto start = chrono::high_resolution_clock::now();
        auto primRes = sparseGraph.primMST(0);
        auto end = chrono::high_resolution_clock::now();
        long long primTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        auto kruskalRes = sparseGraph.kruskalMST();
        end = chrono::high_resolution_clock::now();
        long long kruskalTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << V << "\t" << primTime << "\t\t" << kruskalTime << "\t\t" << primRes.first << endl;
    }

    return 0;
}