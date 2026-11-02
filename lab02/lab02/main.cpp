 
// Лабораторная работа №2 — ПРОДВИНУТЫЙ УРОВЕНЬ (100 баллов)
// Вариант 6: шаблонный контейнер PriorityQueue<T> на основе
//            бинарной кучи (max-heap) + сравнение со std::priority_queue
//
// Реализовано:
//   1. Анализ сложности фрагментов кода A..E (базовый уровень)
//   2. Шаблонная функция average<T>                     
//   3. Шаблонный класс Nullable<T>                      
//   4. Шаблонный контейнер PriorityQueue<T>             
//      Операции: push, pop, top, isEmpty, size
//      Сравнение с std::priority_queue (таблица + замеры)
 

#include <iostream>
#include <string>
#include <vector>
#include <queue>       // std::priority_queue — для сравнения
#include <chrono>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <Windows.h>

using namespace std;
using namespace chrono;

 
// АНАЛИЗ СЛОЖНОСТИ ФРАГМЕНТОВ КОДА (A..E)

// A: O(n) — один цикл по массиву
int exampleA(const int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum;
}

// B: O(n^2) — два вложенных цикла по n
int exampleB(const int arr[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i] == arr[j]) count++;
    return count;
}

// C: O(log n) — i удваивается
int exampleC(int n) {
    int count = 0;
    for (int i = 1; i < n; i *= 2) count++;
    return count;
}

// D: O(n log n) — внешний n, внутренний log n
int exampleD(int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = n; j > 0; j /= 2) count++;
    return count;
}

// E: O(sqrt(n)) — условие i*i < n
int exampleE(int n) {
    int count = 0;
    for (int i = 0; i * i < n; i++) count++;
    return count;
}

 
// ШАБЛОННАЯ ФУНКЦИЯ average<T>
// Сложность O(n).
 
template <typename T>
T average(T arr[], int n) {
    if (n <= 0) throw invalid_argument("average: размер массива должен быть > 0");
    T sum = T();
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum / static_cast<T>(n);
}

 
// ШАБЛОННЫЙ КЛАСС Nullable<T>
// Все операции O(1) (для T = string стоимость копирования O(len)).
 
template <typename T>
class Nullable {
private:
    T value;
    bool hasValueFlag;
public:
    Nullable() : value(), hasValueFlag(false) {}
    Nullable(const T& v) : value(v), hasValueFlag(true) {}
    Nullable(const Nullable<T>& o) : value(o.value), hasValueFlag(o.hasValueFlag) {}
    Nullable<T>& operator=(const Nullable<T>& o) {
        if (this != &o) { value = o.value; hasValueFlag = o.hasValueFlag; }
        return *this;
    }
    bool hasValue() const { return hasValueFlag; }
    T getValue() const {
        if (!hasValueFlag) throw runtime_error("Nullable: значение отсутствует");
        return value;
    }
    void reset() { hasValueFlag = false; }
    void print() const {
        if (hasValueFlag) cout << "Nullable{ value = " << value << " }";
        else              cout << "Nullable{ empty }";
    }
};

 
// ШАБЛОННЫЙ КОНТЕЙНЕР PriorityQueue<T>   
//
// Бинарная куча (max-heap) на std::vector.
// Для T требуется operator< (для сравнения элементов).
//
// Операции и их сложность:
//   push(x)      — O(log n)   (sift-up после добавления)
//   pop()        — O(log n)   (sift-down после удаления корня)
//   top()        — O(1)       (корень кучи)
//   isEmpty()    — O(1)
//   size()       — O(1)
//   clear()      — O(1)       (сброс размера)
//   Конструктор  — O(1)
//   makeHeap     — O(n)       (heapify для массива)
 
template <typename T>
class PriorityQueue {
private:
    vector<T> heap; // внутреннее хранилище — бинарная куча

    // Вспомогательные индексы (0-based индексация):
    //   родитель(i) = (i - 1) / 2
    //   левый(i)    = 2*i + 1
    //   правый(i)   = 2*i + 2

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    // Поднимаем элемент вверх, пока он больше родителя.
    // В худшем случае — O(log n) шагов (высота кучи).
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    // Опускаем элемент вниз, выбирая большего из детей.
    // В худшем случае — O(log n) шагов.
    void siftDown(int i) {
        int n = static_cast<int>(heap.size());
        while (true) {
            int l = leftChild(i);
            int r = rightChild(i);
            int largest = i;

            if (l < n && heap[largest] < heap[l]) largest = l;
            if (r < n && heap[largest] < heap[r]) largest = r;

            if (largest == i) break; // свойство кучи восстановлено

            swap(heap[i], heap[largest]);
            i = largest;
        }
    }

public:
    // Конструктор — O(1)
    PriorityQueue() = default;

    // Конструктор из массива за O(n) (алгоритм heapify).
    // Идём от последнего нелистового узла к корню и делаем siftDown.
    PriorityQueue(const vector<T>& data) : heap(data) {
        int n = static_cast<int>(heap.size());
        // Последний нелистовой узел — parent(n-1).
        for (int i = n / 2 - 1; i >= 0; i--) {
            siftDown(i);
        }
    }

    // push(x): добавляем в конец и поднимаем вверх. O(log n)
    void push(const T& value) {
        heap.push_back(value);
        siftUp(static_cast<int>(heap.size()) - 1);
    }

    // pop(): удаляем корень (максимум).
    // Ставим последний элемент в корень и опускаем его вниз. O(log n)
    void pop() {
        if (heap.empty()) throw out_of_range("PriorityQueue::pop — пустая очередь");
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) siftDown(0);
    }

    // top(): возвращает максимум (корень кучи). O(1)
    const T& top() const {
        if (heap.empty()) throw out_of_range("PriorityQueue::top — пустая очередь");
        return heap[0];
    }

    // isEmpty(). O(1)
    bool isEmpty() const { return heap.empty(); }

    // size(). O(1)
    int size() const { return static_cast<int>(heap.size()); }

    // clear(): освобождает память. O(1) в терминах количества операций
    // (std::vector::clear — деструкторы элементов, но размер становится 0).
    void clear() { heap.clear(); }

    // Печать кучи (для отладки). O(n)
    void print() const {
        cout << "[";
        for (size_t i = 0; i < heap.size(); i++) {
            cout << heap[i];
            if (i + 1 < heap.size()) cout << ", ";
        }
        cout << "]";
    }
};

 
// ВСПОМОГАТЕЛЬНОЕ: замер времени
 
template <typename Func>
long long measureTimeUs(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

 
// MAIN

int main() {
    // Русская локаль в консоли
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << " Лабораторная работа №2 — ПРОДВИНУТЫЙ УРОВЕНЬ" << endl;
    cout << " Вариант 6. PriorityQueue<T> на бинарной куче" << endl << endl;


    // 1. Анализ сложности A..E 
     
    cout << "1. Анализ сложности фрагментов кода " << endl;
    int sample[10] = { 5, 3, 8, 1, 9, 2, 7, 4, 6, 0 };
    cout << "A (O(n)):        " << exampleA(sample, 10) << endl;
    cout << "B (O(n^2)):      " << exampleB(sample, 10) << endl;
    cout << "C (O(log n)):    " << exampleC(1024) << " (для n=1024 ожидается 10)" << endl;
    cout << "D (O(n log n)):  " << exampleD(16) << " (для n=16 ожидается 64)" << endl;
    cout << "E (O(sqrt n)):   " << exampleE(100) << " (для n=100 ожидается 10)" << endl;
    cout << endl;

     
    // 2. average<T> 
     
    cout << "2. Шаблонная функция average<T> " << endl;
    int    iArr[5] = { 1, 2, 3, 4, 5 };
    double dArr[4] = { 1.5, 2.5, 3.5, 4.5 };
    cout << "average(int)    = " << average(iArr, 5) << endl;
    cout << "average(double) = " << average(dArr, 4) << endl;
    cout << endl;

     
    // 3. Nullable<T> 
     
    cout << "3. Шаблонный класс Nullable<T> " << endl;
    Nullable<int> ni;
    Nullable<int> niv(42);
    ni.print();  cout << " | hasValue = " << (ni.hasValue() ? "true" : "false") << endl;
    niv.print(); cout << " | getValue = " << niv.getValue() << endl;
    niv.reset();
    niv.print(); cout << " | hasValue = " << (niv.hasValue() ? "true" : "false") << endl;
    try { (void)niv.getValue(); }
    catch (const runtime_error& e) { cout << "Ожидаемое исключение: " << e.what() << endl; }
    Nullable<string> ns("Привет");
    ns.print(); cout << " | getValue = " << ns.getValue() << endl;
    cout << endl;

     
    // PriorityQueue<T>
     
    cout << "--- 4. Демонстрация PriorityQueue<T> ---" << endl;

    PriorityQueue<int> pq;
    cout << "Изначально: isEmpty = " << (pq.isEmpty() ? "true" : "false")
        << ", size = " << pq.size() << endl;

    vector<int> toPush = { 5, 1, 9, 3, 7, 2, 8, 4, 6, 0 };
    for (int x : toPush) {
        pq.push(x);
        cout << "push(" << x << ") -> size = " << pq.size()
            << ", top = " << pq.top() << endl;
    }

    cout << "Куча: "; pq.print(); cout << endl;

    cout << "Извлечение по одному (должно идти по убыванию):" << endl;
    while (!pq.isEmpty()) {
        int mx = pq.top();
        pq.pop();
        cout << "  top=" << mx << ", после pop size = " << pq.size() << endl;
    }
    cout << "После всех pop: isEmpty = " << (pq.isEmpty() ? "true" : "false") << endl;

    // Работа со строковым типом — демонстрация шаблонности
    PriorityQueue<string> pqs;
    pqs.push("apple");
    pqs.push("zebra");
    pqs.push("mango");
    pqs.push("banana");
    cout << "\nPriorityQueue<string>.top() = " << pqs.top()
        << " (лексикографический максимум)" << endl;

    // Конструктор из вектора — O(n) heapify
    vector<int> data = { 5, 1, 9, 3, 7, 2, 8, 4, 6, 0 };
    PriorityQueue<int> pqFromVec(data);
    cout << "\nКонструктор из вектора (heapify O(n)): ";
    pqFromVec.print();
    cout << " | top = " << pqFromVec.top() << endl;
    cout << endl;

     
    // Сравнение с std::priority_queue (таблица + график-таблица)
     
    cout << "5. Сравнение PriorityQueue<T> с std::priority_queue " << endl;
    cout << "Замер: push N элементов, затем N pop (худший случай для кучи)." << endl << endl;

    vector<int> sizes = { 10000, 50000, 100000, 500000, 1000000 };

    // Заголовок таблицы
    cout << left
        << setw(12) << "Размер N"
        << setw(22) << "Наша PQ (мкс)"
        << setw(22) << "std::PQ (мкс)"
        << setw(15) << "Отношение" << endl;
    cout << string(71, '-') << endl;

    // Для воспроизводимости — фиксированный seed
    mt19937 rng(12345);

    for (int n : sizes) {
        // Заранее готовим массив случайных значений, чтобы оба контейнера обрабатывали одинаковые данные.
        vector<int> values(n);
        for (int i = 0; i < n; i++) values[i] = static_cast<int>(rng() % 1000000);

        // замер нашей реализации
        long long ourTime = measureTimeUs([&]() {
            PriorityQueue<int> pqLocal;
            for (int x : values) pqLocal.push(x);
            volatile int sink = 0;
            while (!pqLocal.isEmpty()) {
                sink += pqLocal.top();
                pqLocal.pop();
            }
            (void)sink;
            });

        // замер std::priority_queue
        long long stdTime = measureTimeUs([&]() {
            priority_queue<int> pqLocal;
            for (int x : values) pqLocal.push(x);
            volatile int sink = 0;
            while (!pqLocal.empty()) {
                sink += pqLocal.top();
                pqLocal.pop();
            }
            (void)sink;
            });

        // Отношение (наша / std). < 1 — наша быстрее, > 1 — std быстрее.
        double ratio = (stdTime == 0) ? 0.0 : static_cast<double>(ourTime) / stdTime;

        cout << left
            << setw(12) << n
            << setw(22) << ourTime
            << setw(22) << stdTime
            << setw(15) << fixed << setprecision(2) << ratio << endl;
    }

    cout << endl;
    cout << "Пояснение к таблице:" << endl;
    cout << "  Наша PQ — PriorityQueue<T> на std::vector + sift-up/sift-down." << endl;
    cout << "  std::PQ — стандартная реализация (обычно d-куча/бинарная куча)." << endl;
    cout << "  Обе имеют одинаковую асимптотику O(log n) на push/pop." << endl;
    cout << "  Разница во времени — это константа (кэш, аллокации, оптимизации STL)." << endl;
    cout << endl;

    cout << "==============================================" << endl;
    cout << " Программа успешно завершена." << endl;
    cout << "==============================================" << endl;
    return 0;
}