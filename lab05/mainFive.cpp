#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>
#include <Windows.h>

using namespace std;

// 1. Стек (на динамическом массиве)
template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10) : capacity(initialCapacity), topIndex(-1) {
        data = new T[capacity];
    }

    ~Stack() { delete[] data; }

    void push(const T& value) {
        if (topIndex >= capacity - 1) resize();
        data[++topIndex] = value;
    }

    T pop() {
        if (isEmpty()) throw underflow_error("Стек пуст");
        return data[topIndex--];
    }

    T top() const {
        if (isEmpty()) throw underflow_error("Стек пуст");
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};

// Задача для стека: проверка на палиндром
bool isPalindromeWithStack(const string& str) {
    Stack<char> stack;
    for (char ch : str) {
        stack.push(ch);
    }
    for (char ch : str) {
        if (stack.pop() != ch) return false;
    }
    return true;
}

// 2. Очередь (циклический буфер фиксированного размера)
template <typename T>
class Queue {
private:
    T* data;
    int head;
    int tail;
    int count;
    int capacity;

public:
    Queue(int size) : capacity(size), head(0), tail(0), count(0) {
        data = new T[capacity];
    }

    ~Queue() { delete[] data; }

    void enqueue(const T& value) {
        if (count >= capacity) throw overflow_error("Очередь переполнена");
        data[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty()) throw underflow_error("Очередь пуста");
        T value = data[head];
        head = (head + 1) % capacity;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw underflow_error("Очередь пуста");
        return data[head];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// 3. Дек (на двусвязном списке)
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    T pop_front() {
        if (isEmpty()) throw underflow_error("Дек пуст");
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
        return value;
    }

    T pop_back() {
        if (isEmpty()) throw underflow_error("Дек пуст");
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw underflow_error("Дек пуст");
        return head->data;
    }

    T back() const {
        if (isEmpty()) throw underflow_error("Дек пуст");
        return tail->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    // Сдвиг влево на k позиций (Вариант 6)
    void rotateLeft(int k) {
        if (head == nullptr || k <= 0) return;
        k = k % count;
        if (k == 0) return;

        // Находим новый хвост (элемент с индексом k-1)
        Node* newTail = head;
        for (int i = 0; i < k - 1; ++i) {
            newTail = newTail->next;
        }
        Node* newHead = newTail->next;

        // Разрываем связь
        newTail->next = nullptr;
        newHead->prev = nullptr;

        // Соединяем конец с началом
        tail->next = head;
        head->prev = tail;

        // Обновляем head и tail
        head = newHead;
        tail = newTail;
    }

    void print() const {
        Node* current = head;
        cout << "[";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) cout << ", ";
            current = current->next;
        }
        cout << "]" << endl;
    }
};

// 4. Приоритетная очередь (на основе кучи)
// Компаратор по умолчанию - std::less (max-heap). Для min-heap использовать std::greater.
template <typename T, typename Compare = less<T>>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;
    Compare comp;

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; ++i) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comp(heap[parent], heap[index])) {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && comp(heap[largest], heap[left])) {
                largest = left;
            }
            if (right < count && comp(heap[largest], heap[right])) {
                largest = right;
            }
            if (largest == index) break;
            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10) : capacity(initialCapacity), count(0) {
        heap = new T[capacity];
    }

    ~PriorityQueue() { delete[] heap; }

    void push(const T& value) {
        if (count >= capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    T pop() {
        if (isEmpty()) throw underflow_error("Приоритетная очередь пуста");
        T topValue = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return topValue;
    }

    T top() const {
        if (isEmpty()) throw underflow_error("Приоритетная очередь пуста");
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Задача для приоритетной очереди: медиана потока чисел
// Используем две кучи: max-heap для меньшей половины и min-heap для большей
void findMedianStream() {
    cout << "Медианы потока чисел [5, 15, 1, 3, 8, 7, 9, 10, 20]:" << endl;
    int stream[] = { 5, 15, 1, 3, 8, 7, 9, 10, 20 };
    int n = sizeof(stream) / sizeof(stream[0]);

    PriorityQueue<int, less<int>> maxHeap; // Для меньшей половины
    PriorityQueue<int, greater<int>> minHeap; // Для большей половины

    for (int i = 0; i < n; ++i) {
        int num = stream[i];

        // Добавляем в maxHeap
        if (maxHeap.isEmpty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        }
        else {
            minHeap.push(num);
        }

        // Балансируем кучи
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.pop());
        }
        else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.pop());
        }

        // Вычисляем медиану
        double median;
        if (maxHeap.size() == minHeap.size()) {
            median = (maxHeap.top() + minHeap.top()) / 2.0;
        }
        else {
            median = maxHeap.top();
        }
        cout << "Добавлено " << num << ", медиана: " << median << endl;
    }
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №5. Стек, очередь, дек, приоритетная очередь (Вариант 6) ===" << endl << endl;

    // 1. Стек: Проверка на палиндром
    cout << "1. Стек: Проверка строки на палиндром" << endl;
    string testStr1 = "radar";
    string testStr2 = "hello";
    cout << "Строка \"" << testStr1 << "\": " << (isPalindromeWithStack(testStr1) ? "Палиндром" : "Не палиндром") << endl;
    cout << "Строка \"" << testStr2 << "\": " << (isPalindromeWithStack(testStr2) ? "Палиндром" : "Не палиндром") << endl;

    // Проверка обработки исключений для стека
    try {
        Stack<int> emptyStack;
        emptyStack.pop();
    }
    catch (const underflow_error& e) {
        cout << "Исключение (стек): " << e.what() << endl;
    }

    // 2. Очередь: Циклическая очередь фиксированного размера
    cout << "\n2. Очередь: Циклическая очередь фиксированного размера (capacity = 3)" << endl;
    Queue<int> queue(3);
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    cout << "Добавлены 1, 2, 3. Размер: " << queue.size() << endl;
    cout << "Извлечено: " << queue.dequeue() << endl;
    cout << "Извлечено: " << queue.dequeue() << endl;
    queue.enqueue(4);
    queue.enqueue(5);
    cout << "Добавлены 4, 5. Размер: " << queue.size() << endl;
    cout << "Извлечено: " << queue.dequeue() << endl;
    cout << "Извлечено: " << queue.dequeue() << endl;
    cout << "Извлечено: " << queue.dequeue() << endl;

    try {
        queue.dequeue();
    }
    catch (const underflow_error& e) {
        cout << "Исключение (очередь): " << e.what() << endl;
    }
    try {
        Queue<int> qFull(1);
        qFull.enqueue(1);
        qFull.enqueue(2);
    }
    catch (const overflow_error& e) {
        cout << "Исключение (очередь переполнена): " << e.what() << endl;
    }

    // 3. Дек: Сдвиг влево
    cout << "\n3. Дек: Операция rotateLeft(k)" << endl;
    Deque<int> deque;
    for (int i = 1; i <= 5; ++i) deque.push_back(i * 10);
    cout << "Исходный дек: ";
    deque.print();

    deque.rotateLeft(2);
    cout << "После rotateLeft(2): ";
    deque.print();

    deque.rotateLeft(3);
    cout << "После rotateLeft(3): ";
    deque.print();

    try {
        Deque<int> emptyDeque;
        emptyDeque.pop_front();
    }
    catch (const underflow_error& e) {
        cout << "Исключение (дек): " << e.what() << endl;
    }

    // 4. Приоритетная очередь: Медиана потока
    cout << "\n4. Приоритетная очередь: Медиана потока чисел" << endl;
    findMedianStream();

    return 0;
}