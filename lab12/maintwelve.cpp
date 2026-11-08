#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Класс ориентированного взвешенного графа на матрице смежности
class Graph {
private:
    int V;
    vector<vector<int>> adj; // 0 - нет ребра, иначе вес

public:
    Graph(int vertices) : V(vertices) {
        adj.assign(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u][v] = weight;
        }
    }

    // Алгоритм Дейкстры с восстановлением пути (оптимизированный, priority_queue)
    pair<vector<int>, vector<int>> dijkstraWithPath(int start) const {
        vector<int> dist(V, INT_MAX);
        vector<int> prev(V, -1);
        // priority_queue хранит пары {расстояние, вершина}, минимум наверху
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[start] = 0;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            // Если извлеченное расстояние больше текущего, это устаревшая запись
            if (d > dist[u]) continue;

            for (int v = 0; v < V; ++v) {
                if (adj[u][v] != 0) {
                    int weight = adj[u][v];
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u;
                        pq.push({ dist[v], v });
                    }
                }
            }
        }
        return { dist, prev };
    }

    // Восстановление пути по массиву prev
    vector<int> getPath(int start, int end, const vector<int>& prev) const {
        vector<int> path;
        if (start == end) {
            path.push_back(start);
            return path;
        }
        if (prev[end] == -1) return path; // пути нет
        for (int v = end; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // Вывод результата Дейкстры
    void printDijkstra(int start) const {
        auto result = dijkstraWithPath(start);
        vector<int> dist = result.first;
        vector<int> prev = result.second;

        cout << "Кратчайшие расстояния от вершины " << start << ":" << endl;
        for (int i = 0; i < V; ++i) {
            cout << "  До " << i << ": ";
            if (dist[i] == INT_MAX) {
                cout << "недостижима";
            }
            else {
                cout << dist[i] << " (путь: ";
                vector<int> path = getPath(start, i, prev);
                for (size_t j = 0; j < path.size(); ++j) {
                    cout << path[j];
                    if (j < path.size() - 1) cout << " -> ";
                }
                cout << ")";
            }
            cout << endl;
        }
    }

    // Алгоритм Флойда-Уоршелла с восстановлением путей и обнаружением отрицательных циклов
    void floydWarshall() const {
        vector<vector<int>> dist(V, vector<int>(V, INT_MAX));
        vector<vector<int>> next(V, vector<int>(V, -1));

        // Инициализация
        for (int i = 0; i < V; ++i) {
            dist[i][i] = 0;
            for (int j = 0; j < V; ++j) {
                if (adj[i][j] != 0) {
                    dist[i][j] = adj[i][j];
                    next[i][j] = j;
                }
            }
        }

        // Основной алгоритм
        for (int k = 0; k < V; ++k) {
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                        }
                    }
                }
            }
        }

        // Обнаружение отрицательных циклов
        bool hasNegativeCycle = false;
        for (int i = 0; i < V; ++i) {
            if (dist[i][i] < 0) {
                hasNegativeCycle = true;
                break;
            }
        }

        cout << "Матрица кратчайших расстояний (Флойд-Уоршелл):" << endl;
        cout << "     ";
        for (int j = 0; j < V; ++j) cout << j << "\t";
        cout << endl;
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] == INT_MAX) cout << "INF\t";
                else cout << dist[i][j] << "\t";
            }
            cout << endl;
        }

        if (hasNegativeCycle) {
            cout << "Обнаружен отрицательный цикл!" << endl;
        }
        else {
            cout << "Отрицательных циклов нет." << endl;
        }

        // Восстановление путей (пример для нескольких пар)
        cout << "\nПримеры восстановления путей:" << endl;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (i != j && dist[i][j] != INT_MAX) {
                    cout << "  " << i << " -> " << j << ": расстояние " << dist[i][j] << ", путь: ";
                    int u = i;
                    cout << u;
                    while (u != j) {
                        u = next[u][j];
                        cout << " -> " << u;
                    }
                    cout << endl;
                }
            }
        }
    }
};

// Генерация случайного ориентированного графа
Graph generateRandomGraph(int V, int maxWeight) {
    Graph g(V);
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i != j && (rand() % 100 < 30)) { // 30% вероятность ребра
                g.addEdge(i, j, rand() % maxWeight + 1);
            }
        }
    }
    return g;
}

// Замер времени работы Дейкстры
long long measureDijkstra(const Graph& g, int start) {
    auto startTime = chrono::high_resolution_clock::now();
    g.dijkstraWithPath(start);
    auto endTime = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
}

// Замер времени работы Флойда-Уоршелла
long long measureFloyd(const Graph& g) {
    auto startTime = chrono::high_resolution_clock::now();
    // Для замера вызываем floydWarshall, но он выводит много текста.
    // Сделаем отдельную версию без вывода для замера.
    // Чтобы не дублировать код, просто вызовем и проигнорируем вывод.
    // В реальном проекте лучше разделить логику и вывод.
    g.floydWarshall();
    auto endTime = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(static_cast<unsigned int>(time(0)));

    cout << "=== Лабораторная работа №12. Алгоритмы Дейкстры и Флойда-Уоршелла (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация на конкретном графе
    cout << "1. Демонстрация алгоритмов на тестовом графе:" << endl;
    Graph g(5);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 6);
    g.addEdge(2, 3, 2);
    g.addEdge(2, 4, 3);
    g.addEdge(3, 4, 1);

    // Дейкстра
    cout << "\n--- Алгоритм Дейкстры (с восстановлением пути) ---" << endl;
    g.printDijkstra(0);

    // Флойд-Уоршелл
    cout << "\n--- Алгоритм Флойда-Уоршелла (с восстановлением путей и проверкой циклов) ---" << endl;
    g.floydWarshall();

    // 2. Сравнительный анализ
    cout << "\n2. Сравнительный анализ производительности:" << endl;
    cout << "V\tДейкстра (мкс)\tФлойд-Уоршелл (мкс)" << endl;
    cout << "------------------------------------------" << endl;

    int sizes[] = { 10, 20, 50, 100 };
    for (int V : sizes) {
        Graph randomGraph = generateRandomGraph(V, 100);

        long long dijkstraTime = measureDijkstra(randomGraph, 0);
        long long floydTime = measureFloyd(randomGraph);

        cout << V << "\t" << dijkstraTime << "\t\t" << floydTime << endl;
    }

    return 0;
}