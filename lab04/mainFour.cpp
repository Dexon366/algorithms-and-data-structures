#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <Windows.h>

using namespace std;

// Односвязный список
template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    // Итератор для односвязного списка
    class Iterator {
    private:
        Node* ptr;
    public:
        Iterator(Node* p = nullptr) : ptr(p) {}
        T& operator*() { return ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~SinglyLinkedList() { clear(); }

    // Вставка в начало O(1)
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) tail = newNode;
        count++;
    }

    // Вставка в конец O(1)
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Удаление из начала O(1)
    void pop_front() {
        if (head == nullptr) throw underflow_error("Список пуст");
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) tail = nullptr;
    }

    // Удаление из конца O(n)
    void pop_back() {
        if (head == nullptr) throw underflow_error("Список пуст");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        count--;
    }

    // Поиск O(n)
    Node* find(const T& value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Вывод элементов через один (Вариант 6, базовый уровень)
    void printEverySecond() const {
        Node* current = head;
        bool print = true;
        cout << "[";
        while (current != nullptr) {
            if (print) {
                cout << current->data;
                if (current->next != nullptr && current->next->next != nullptr) cout << ", ";
            }
            print = !print;
            current = current->next;
        }
        cout << "]" << endl;
    }

    // Печать списка
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

    // Очистка памяти
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }

    // Сериализация (Вариант 6, продвинутый уровень)
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл для записи");
        file << count << "\n";
        Node* current = head;
        while (current != nullptr) {
            file << current->data << "\n";
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл для чтения");
        clear();
        int newCount;
        file >> newCount;
        for (int i = 0; i < newCount; ++i) {
            T value;
            file >> value;
            push_back(value);
        }
        file.close();
    }
};

// Двусвязный список
template <typename T>
class DoublyLinkedList {
private:
    struct DNode {
        T data;
        DNode* prev;
        DNode* next;
        DNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    DNode* head;
    DNode* tail;
    int count;

public:
    // Итератор для двусвязного списка
    class Iterator {
    private:
        DNode* ptr;
    public:
        Iterator(DNode* p = nullptr) : ptr(p) {}
        T& operator*() { return ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~DoublyLinkedList() { clear(); }

    // Вставка в начало O(1)
    void push_front(const T& value) {
        DNode* newNode = new DNode(value);
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

    // Вставка в конец O(1)
    void push_back(const T& value) {
        DNode* newNode = new DNode(value);
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

    // Удаление из начала O(1)
    void pop_front() {
        if (head == nullptr) throw underflow_error("Список пуст");
        DNode* temp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
    }

    // Удаление из конца O(1)
    void pop_back() {
        if (tail == nullptr) throw underflow_error("Список пуст");
        DNode* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
    }

    // Поиск O(n)
    DNode* find(const T& value) const {
        DNode* current = head;
        while (current != nullptr) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Циклический сдвиг влево на k позиций (Вариант 6, средний уровень)
    void rotateLeft(int k) {
        if (head == nullptr || k <= 0) return;
        k = k % count;
        if (k == 0) return;

        // Находим новый хвост (элемент с индексом k-1)
        DNode* newTail = head;
        for (int i = 0; i < k - 1; ++i) {
            newTail = newTail->next;
        }
        DNode* newHead = newTail->next;

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

    // Печать списка
    void print() const {
        DNode* current = head;
        cout << "[";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) cout << ", ";
            current = current->next;
        }
        cout << "]" << endl;
    }

    // Очистка памяти
    void clear() {
        while (head != nullptr) {
            DNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }

    // Сериализация (Вариант 6, продвинутый уровень)
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл для записи");
        file << count << "\n";
        DNode* current = head;
        while (current != nullptr) {
            file << current->data << "\n";
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл для чтения");
        clear();
        int newCount;
        file >> newCount;
        for (int i = 0; i < newCount; ++i) {
            T value;
            file >> value;
            push_back(value);
        }
        file.close();
    }
};

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №4. Списки (Вариант 6, продвинутый уровень) ===" << endl << endl;

    // 1. Односвязный список
    cout << "1. Односвязный список:" << endl;
    SinglyLinkedList<int> sList;
    sList.push_back(10);
    sList.push_back(20);
    sList.push_back(30);
    sList.push_front(5);
    cout << "Исходный список: ";
    sList.print();

    cout << "Элементы через один (printEverySecond): ";
    sList.printEverySecond();

    cout << "Размер: " << sList.size() << endl;

    // 2. Двусвязный список и сдвиг
    cout << "\n2. Двусвязный список (rotateLeft):" << endl;
    DoublyLinkedList<int> dList;
    for (int i = 1; i <= 5; ++i) dList.push_back(i * 10);
    cout << "Исходный список: ";
    dList.print();

    dList.rotateLeft(2);
    cout << "После сдвига влево на 2 (rotateLeft(2)): ";
    dList.print();

    dList.rotateLeft(3);
    cout << "После сдвига влево на 3 (rotateLeft(3)): ";
    dList.print();

    // 3. Сериализация
    cout << "\n3. Сериализация (сохранение и загрузка):" << endl;
    string filename = "list_data.txt";
    dList.saveToFile(filename);
    cout << "Список сохранен в файл " << filename << endl;

    DoublyLinkedList<int> loadedList;
    loadedList.loadFromFile(filename);
    cout << "Загруженный список: ";
    loadedList.print();

    // 4. Итераторы и range-based for
    cout << "\n4. Итераторы и range-based for:" << endl;
    cout << "Односвязный список через итератор: ";
    for (auto it = sList.begin(); it != sList.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Двусвязный список через range-based for: ";
    for (int x : dList) {
        cout << x << " ";
    }
    cout << endl;

    // 5. Сравнение производительности
    cout << "\n5. Сравнение производительности (вставка в начало 10000 элементов):" << endl;

    auto start = chrono::high_resolution_clock::now();
    SinglyLinkedList<int> perfSList;
    for (int i = 0; i < 10000; ++i) perfSList.push_front(i);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> sTime = end - start;
    cout << "Односвязный список (push_front): " << sTime.count() << " мс" << endl;

    start = chrono::high_resolution_clock::now();
    DoublyLinkedList<int> perfDList;
    for (int i = 0; i < 10000; ++i) perfDList.push_front(i);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> dTime = end - start;
    cout << "Двусвязный список (push_front): " << dTime.count() << " мс" << endl;

    // Обработка исключений
    cout << "\n6. Проверка обработки исключений:" << endl;
    try {
        SinglyLinkedList<int> emptyList;
        emptyList.pop_front();
    }
    catch (const underflow_error& e) {
        cout << "Перехвачено исключение: " << e.what() << endl;
    }

    return 0;
}