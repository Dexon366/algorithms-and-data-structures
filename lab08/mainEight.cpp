#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <Windows.h>

using namespace std;

// Двоичный поиск для double с погрешностью (итеративный)
// Возвращает индекс найденного элемента или -1
int binarySearchDouble(const double arr[], int n, double target, double epsilon) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (fabs(arr[mid] - target) <= epsilon) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}

// Двоичный поиск для double с погрешностью (рекурсивный)
int binarySearchDoubleRecursive(const double arr[], int left, int right, double target, double epsilon) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    if (fabs(arr[mid] - target) <= epsilon) {
        return mid;
    }
    if (arr[mid] < target) {
        return binarySearchDoubleRecursive(arr, mid + 1, right, target, epsilon);
    }
    else {
        return binarySearchDoubleRecursive(arr, left, mid - 1, target, epsilon);
    }
}

// Хеш-таблица с линейным пробированием (Open Addressing)
template <typename V>
class HashTable {
private:
    struct Entry {
        string key;
        V value;
        bool occupied;
        Entry() : occupied(false) {}
    };

    Entry* table;
    int size;
    int count;

    // Хеш-функция для строк
    int hashFunction(const string& key) const {
        unsigned long long hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % size;
        }
        return static_cast<int>(hash);
    }

public:
    HashTable(int s) : size(s), count(0) {
        table = new Entry[size];
    }

    ~HashTable() {
        delete[] table;
    }

    // Вставка элемента
    void insert(const string& key, const V& value) {
        if (count >= size) {
            cout << "Ошибка: хеш-таблица заполнена!" << endl;
            return;
        }
        int index = hashFunction(key);
        int i = 0;
        while (table[(index + i) % size].occupied) {
            // Если ключ уже есть, обновляем значение
            if (table[(index + i) % size].key == key) {
                table[(index + i) % size].value = value;
                return;
            }
            i++;
        }
        int pos = (index + i) % size;
        table[pos].key = key;
        table[pos].value = value;
        table[pos].occupied = true;
        count++;
    }

    // Поиск элемента
    bool find(const string& key, V& result) const {
        int index = hashFunction(key);
        int i = 0;
        while (i < size) {
            int pos = (index + i) % size;
            if (!table[pos].occupied) {
                return false; // Нашли пустую ячейку — элемента нет
            }
            if (table[pos].key == key) {
                result = table[pos].value;
                return true;
            }
            i++;
        }
        return false;
    }

    // Удаление элемента (ленивое: помечаем occupied = false, но это может нарушить поиск)
    // Для простоты в этой работе используем простую реализацию без удаления
    // или с перехешированием. Оставим только поиск и вставку для сравнения.
    // В задании не требуется удаление, но для полноты можно добавить.
    // Мы реализуем remove с перестройкой таблицы (рехешированием) для корректности.
    bool remove(const string& key) {
        int index = hashFunction(key);
        int i = 0;
        while (i < size) {
            int pos = (index + i) % size;
            if (!table[pos].occupied) return false;
            if (table[pos].key == key) {
                // Нашли элемент. Удаляем и перестраиваем таблицу (простой способ)
                // Собираем все оставшиеся элементы
                Entry* newTable = new Entry[size];
                int newCount = 0;
                for (int j = 0; j < size; ++j) {
                    if (j != pos && table[j].occupied) {
                        int newIndex = 0;
                        unsigned long long hash = 0;
                        for (char c : table[j].key) hash = (hash * 31 + c) % size;
                        newIndex = static_cast<int>(hash);
                        int k = 0;
                        while (newTable[(newIndex + k) % size].occupied) k++;
                        int newPos = (newIndex + k) % size;
                        newTable[newPos].key = table[j].key;
                        newTable[newPos].value = table[j].value;
                        newTable[newPos].occupied = true;
                        newCount++;
                    }
                }
                delete[] table;
                table = newTable;
                count = newCount;
                return true;
            }
            i++;
        }
        return false;
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            if (table[i].occupied) {
                cout << i << ": (" << table[i].key << ", " << table[i].value << ")" << endl;
            }
            else {
                cout << i << ": empty" << endl;
            }
        }
    }

    int getSize() const { return count; }
};

// Вспомогательная функция для замера времени бинарного поиска
long long measureBinarySearch(const double arr[], int n, const double queries[], int qCount, double epsilon) {
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < qCount; ++i) {
        binarySearchDouble(arr, n, queries[i], epsilon);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

// Вспомогательная функция для замера времени поиска в хеш-таблице
long long measureHashSearch(HashTable<int>& ht, const string queries[], int qCount) {
    auto start = chrono::high_resolution_clock::now();
    int val;
    for (int i = 0; i < qCount; ++i) {
        ht.find(queries[i], val);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(time(0));

    cout << "=== Лабораторная работа №8. Двоичный поиск и хеш-таблицы (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация двоичного поиска с погрешностью
    cout << "1. Двоичный поиск для double с погрешностью (epsilon = 0.001):" << endl;
    double arrDouble[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 };
    int nDouble = sizeof(arrDouble) / sizeof(arrDouble[0]);
    double target = 4.4001;
    double epsilon = 0.001;

    int idxIter = binarySearchDouble(arrDouble, nDouble, target, epsilon);
    int idxRec = binarySearchDoubleRecursive(arrDouble, 0, nDouble - 1, target, epsilon);

    cout << "Ищем " << target << " в массиве: ";
    for (double d : arrDouble) cout << d << " ";
    cout << endl;
    cout << "Итеративный поиск: индекс = " << idxIter << (idxIter != -1 ? " (найдено)" : " (не найдено)") << endl;
    cout << "Рекурсивный поиск: индекс = " << idxRec << (idxRec != -1 ? " (найдено)" : " (не найдено)") << endl;

    // 2. Демонстрация хеш-таблицы с линейным пробированием
    cout << "\n2. Хеш-таблица с линейным пробированием (ключ: string, значение: int):" << endl;
    HashTable<int> ht(10);
    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("cherry", 300);
    ht.insert("date", 400);
    ht.insert("elderberry", 500);

    cout << "Содержимое хеш-таблицы:" << endl;
    ht.print();

    string key = "cherry";
    int value;
    if (ht.find(key, value)) {
        cout << "Поиск ключа \"" << key << "\": найдено значение " << value << endl;
    }
    else {
        cout << "Поиск ключа \"" << key << "\": не найдено" << endl;
    }

    key = "fig";
    if (ht.find(key, value)) {
        cout << "Поиск ключа \"" << key << "\": найдено значение " << value << endl;
    }
    else {
        cout << "Поиск ключа \"" << key << "\": не найдено" << endl;
    }

    // 3. Сравнительный анализ производительности
    cout << "\n3. Сравнительный анализ производительности:" << endl;
    cout << "N\t\tДвоичный поиск (мкс)\tХеш-таблица (мкс)" << endl;
    cout << "-------------------------------------------------------" << endl;

    int sizes[] = { 1000, 10000, 100000 };
    int qCount = 10000; // Количество запросов

    for (int n : sizes) {
        // Готовим массив для двоичного поиска (отсортированный)
        double* sortedArr = new double[n];
        for (int i = 0; i < n; ++i) {
            sortedArr[i] = i * 1.0; // Просто числа 0.0, 1.0, 2.0...
        }

        // Готовим хеш-таблицу (используем строковые ключи, как в варианте)
        // Для честного сравнения будем искать числа, преобразованные в строки
        HashTable<int> hashTable(n * 2); // Берем с запасом, чтобы избежать переполнения
        for (int i = 0; i < n; ++i) {
            hashTable.insert(to_string(i), i);
        }

        // Генерируем запросы
        double* doubleQueries = new double[qCount];
        string* stringQueries = new string[qCount];
        for (int i = 0; i < qCount; ++i) {
            int q = rand() % n;
            doubleQueries[i] = q * 1.0;
            stringQueries[i] = to_string(q);
        }

        // Замеряем время
        long long timeBinary = measureBinarySearch(sortedArr, n, doubleQueries, qCount, 0.0001);
        long long timeHash = measureHashSearch(hashTable, stringQueries, qCount);

        cout << n << "\t\t" << timeBinary << "\t\t\t" << timeHash << endl;

        delete[] sortedArr;
        delete[] doubleQueries;
        delete[] stringQueries;
    }

    return 0;
}