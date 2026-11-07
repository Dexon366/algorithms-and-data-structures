#include <iostream>
#include <stdexcept>
#include <functional>
#include <iomanip>
#include <chrono>
#include <Windows.h>

using namespace std;

// Шаблонный класс Vector - аналог std::vector
template <typename T>
class Vector {
private:
    T* data;      // Указатель на динамический массив
    int sz;       // Текущий размер (количество элементов)
    int cap;      // Ёмкость (выделенная память)

public:
    // Конструктор по умолчанию
    Vector() : data(nullptr), sz(0), cap(0) {}

    // Конструктор с параметром размера и значением по умолчанию
    explicit Vector(int n, const T& val = T()) : sz(n), cap(n) {
        if (n < 0) throw invalid_argument("Size cannot be negative");
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = val;
        }
    }

    // Конструктор копирования (глубокое копирование)
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    // Конструктор перемещения
    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Деструктор
    ~Vector() {
        delete[] data;
    }

    // Оператор присваивания копированием (copy-and-swap)
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // Оператор присваивания перемещением
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // Оператор доступа по индексу (неконстантный)
    T& operator[](int index) {
        if (index < 0 || index >= sz) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    // Оператор доступа по индексу (константный)
    const T& operator[](int index) const {
        if (index < 0 || index >= sz) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    // Методы доступа
    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    // Резервирование памяти
    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < sz; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    // Добавление в конец
    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 1 : cap * 2;
            reserve(newCap);
        }
        data[sz++] = value;
    }

    // Удаление последнего элемента
    void pop_back() {
        if (empty()) {
            throw underflow_error("Vector is empty");
        }
        sz--;
    }

    // Изменение размера
    void resize(int newSize, const T& defaultValue = T()) {
        if (newSize < 0) throw invalid_argument("Size cannot be negative");
        if (newSize < sz) {
            sz = newSize;
        }
        else if (newSize > sz) {
            if (newSize > cap) {
                reserve(newSize);
            }
            for (int i = sz; i < newSize; ++i) {
                data[i] = defaultValue;
            }
            sz = newSize;
        }
    }

    // Очистка
    void clear() {
        sz = 0;
    }

    // Обмен содержимого
    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // Поиск элемента по значению (возвращает индекс или -1)
    int find(const T& value) const {
        for (int i = 0; i < sz; ++i) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    // Применение функции к каждому элементу (вариант 6)
    void apply(function<void(T&)> func) {
        for (int i = 0; i < sz; ++i) {
            func(data[i]);
        }
    }

    // Печать вектора
    void print() const {
        cout << "[";
        for (int i = 0; i < sz; ++i) {
            cout << data[i];
            if (i < sz - 1) cout << ", ";
        }
        cout << "]" << endl;
    }

    // Итераторы
    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }
};

// Вспомогательный класс для амортизированного анализа
template <typename T>
class VectorTest : public Vector<T> {
private:
    int copyCount = 0;
public:
    void push_back(const T& value) {
        if (this->size() >= this->capacity()) {
            int newCap = (this->capacity() == 0) ? 1 : this->capacity() * 2;
            // Считаем копирования при расширении
            copyCount += this->size();
            this->reserve(newCap);
        }
        // Используем базовый push_back, но без повторного расширения
        Vector<T>::push_back(value);
    }
    int getCopyCount() const { return copyCount; }
    void resetCopyCount() { copyCount = 0; }
};

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Демонстрация работы класса Vector (Вариант 6, продвинутый уровень) ===" << endl << endl;

    // 1. Базовые операции и конструкторы
    cout << "1. Создание вектора с параметрами (размер 5, значение 10):" << endl;
    Vector<int> v1(5, 10);
    v1.print(); // Ожидается: [10, 10, 10, 10, 10]

    cout << "\n2. Добавление элементов (push_back):" << endl;
    for (int i = 1; i <= 5; ++i) {
        v1.push_back(i);
        cout << "Добавлено " << i << ". Размер: " << v1.size() << ", Ёмкость: " << v1.capacity() << endl;
    }
    cout << "Текущий вектор: ";
    v1.print();

    cout << "\n3. Удаление последнего элемента (pop_back):" << endl;
    v1.pop_back();
    cout << "После удаления: ";
    v1.print();

    cout << "\n4. Поиск элемента (find):" << endl;
    int searchVal = 3;
    int idx = v1.find(searchVal);
    if (idx != -1) {
        cout << "Элемент " << searchVal << " найден на индексе " << idx << endl;
    }
    else {
        cout << "Элемент " << searchVal << " не найден" << endl;
    }

    cout << "\n5. Применение функции (apply) - умножение всех элементов на 2:" << endl;
    v1.apply([](int& x) { x *= 2; });
    v1.print();

    cout << "\n6. Доступ по индексу (operator[]):" << endl;
    cout << "v1[0] = " << v1[0] << endl;

    cout << "\n7. Изменение размера (resize):" << endl;
    v1.resize(12, 0);
    cout << "После resize(12, 0): ";
    v1.print();

    cout << "\n8. Очистка (clear):" << endl;
    v1.clear();
    cout << "Размер после clear: " << v1.size() << endl;

    // 2. Проверка обработки исключений (безопасность)
    cout << "\n9. Проверка обработки исключений (выход за границы):" << endl;
    try {
        Vector<int> vErr(2);
        vErr[5] = 10; // Должно вызвать исключение
    }
    catch (const out_of_range& e) {
        cout << "Перехвачено исключение: " << e.what() << endl;
    }

    try {
        Vector<int> vEmpty;
        vEmpty.pop_back(); // Должно вызвать исключение
    }
    catch (const underflow_error& e) {
        cout << "Перехвачено исключение: " << e.what() << endl;
    }

    // 3. Демонстрация range-based for (итераторы)
    cout << "\n10. Работа range-based for (итераторы):" << endl;
    Vector<int> vIter;
    for (int i = 1; i <= 5; ++i) vIter.push_back(i * 10);
    cout << "Элементы: ";
    for (int x : vIter) {
        cout << x << " ";
    }
    cout << endl;

    // 4. Амортизированный анализ
    cout << "\n=== Амортизированный анализ push_back ===" << endl;
    cout << setw(10) << "n"
        << setw(15) << "Копирований"
        << setw(15) << "Всего операций"
        << setw(15) << "Среднее" << endl;
    cout << string(55, '-') << endl;

    for (int n = 1000; n <= 100000; n *= 10) {
        VectorTest<int> v;
        for (int i = 0; i < n; ++i) {
            v.push_back(i);
        }
        int totalOps = n + v.getCopyCount();
        double average = static_cast<double>(totalOps) / n;
        cout << setw(10) << n
            << setw(15) << v.getCopyCount()
            << setw(15) << totalOps
            << setw(15) << fixed << setprecision(2) << average << endl;
    }

    return 0;
}