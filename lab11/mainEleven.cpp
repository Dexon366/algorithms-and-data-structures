#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Класс графа на основе матрицы смежности (взвешенный, неориентированный)
class Graph {
private:
    int V; // Количество вершин
    vector<vector<int>> adjMatrix; // Матрица смежности (0 - нет ребра, >0 - вес)

    // Вспомогательная рекурсивная функция для DFS
    void dfsUtil(int v, vector<bool>& visited) const {
        visited[v] = true;
        cout << v << " ";
        for (int u = 0; u < V; ++u) {
            if (adjMatrix[v][u] != 0 && !visited[u]) {
                dfsUtil(u, visited);
            }
        }
    }

    // Вспомогательная рекурсивная функция для проверки цикла
    bool hasCycleUtil(int v, vector<bool>& visited, int parent) const {
        visited[v] = true;
        for (int u = 0; u < V; ++u) {
            if (adjMatrix[v][u] != 0) {
                if (!visited[u]) {
                    if (hasCycleUtil(u, visited, v)) return true;
                }
                else if (u != parent) {
                    return true; // Нашли посещенную вершину, не являющуюся родителем
                }
            }
        }
        return false;
    }

public:
    Graph(int vertices) : V(vertices) {
        adjMatrix.assign(V, vector<int>(V, 0));
    }

    // Добавление ребра с весом (для неориентированного графа)
    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adjMatrix[u][v] = weight;
            adjMatrix[v][u] = weight;
        }
    }

    // Удаление ребра
    void removeEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adjMatrix[u][v] = 0;
            adjMatrix[v][u] = 0;
        }
    }

    // Вывод матрицы смежности
    void print() const {
        cout << "Матрица смежности (0 - нет ребра):" << endl;
        cout << "   ";
        for (int i = 0; i < V; ++i) cout << i << " ";
        cout << endl;
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (int j = 0; j < V; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Обход в глубину (рекурсивный)
    void DFS(int start) const {
        if (start < 0 || start >= V) return;
        vector<bool> visited(V, false);
        cout << "DFS (рекурсивный) от " << start << ": ";
        dfsUtil(start, visited);
        cout << endl;
    }

    // Обход в ширину
    void BFS(int start) const {
        if (start < 0 || start >= V) return;
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << "BFS от " << start << ": ";
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

    // Поиск компонент связности
    void printConnectedComponents() const {
        vector<bool> visited(V, false);
        int componentNum = 1;
        cout << "Компоненты связности:" << endl;
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                cout << "Компонента " << componentNum++ << ": ";
                // Используем BFS для сбора компоненты (порядок не важен)
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

    // Проверка наличия цикла в неориентированном графе
    bool hasCycle() const {
        vector<bool> visited(V, false);
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                if (hasCycleUtil(i, visited, -1)) return true;
            }
        }
        return false;
    }

    // Поиск кратчайшего пути (по количеству рёбер) с помощью BFS
    // Примечание: для взвешенного графа это не минимальный вес, а минимальное число рёбер.
    vector<int> shortestPath(int start, int end) const {
        vector<int> path;
        if (start < 0 || start >= V || end < 0 || end >= V) return path;

        vector<int> dist(V, -1);
        vector<int> prev(V, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (v == end) break;
            for (int u = 0; u < V; ++u) {
                if (adjMatrix[v][u] != 0 && dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.push(u);
                }
            }
        }

        if (dist[end] == -1) return path; // Путь не найден

        for (int v = end; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // Вывод кратчайшего пути
    void printShortestPath(int start, int end) const {
        vector<int> path = shortestPath(start, end);
        if (path.empty()) {
            cout << "Путь между " << start << " и " << end << " не существует" << endl;
            return;
        }
        cout << "Кратчайший путь (по числу рёбер) от " << start << " до " << end << ": ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << " (длина: " << path.size() - 1 << ")" << endl;
    }
};

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №11. Графы. DFS и BFS (Вариант 6) ===" << endl << endl;

    // 1. Создание взвешенного неориентированного графа (матрица смежности)
    cout << "1. Создание графа (матрица смежности, взвешенный, неориентированный):" << endl;
    Graph g(7);

    // Добавляем рёбра с весами
    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 5, 1);
    g.addEdge(4, 5, 4);
    g.addEdge(5, 6, 6);
    g.addEdge(3, 6, 8); // Добавим ребро для создания цикла

    g.print();

    // 2. Обходы DFS и BFS
    cout << "\n2. Обходы графа:" << endl;
    g.DFS(0);
    g.BFS(0);

    // 3. Проверка наличия цикла (вариант 6)
    cout << "\n3. Проверка наличия цикла:" << endl;
    cout << "Граф содержит цикл: " << (g.hasCycle() ? "Да" : "Нет") << endl;

    // 4. Компоненты связности
    cout << "\n4. Компоненты связности:" << endl;
    g.printConnectedComponents();

    // 5. Кратчайший путь
    cout << "\n5. Поиск кратчайшего пути (BFS):" << endl;
    g.printShortestPath(0, 6);
    g.printShortestPath(1, 5);
    g.printShortestPath(0, 3);

    // 6. Демонстрация удаления ребра (для полноты)
    cout << "\n6. Удаление ребра (3, 6) и повторная проверка цикла:" << endl;
    g.removeEdge(3, 6);
    cout << "Граф содержит цикл: " << (g.hasCycle() ? "Да" : "Нет") << endl;
    g.print();

    return 0;
}