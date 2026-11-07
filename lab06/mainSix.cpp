#include <iostream>
#include <chrono>
#include <cstdlib>
#include <Windows.h>

using namespace std;

// Рекурсивная функция для вычисления факториала
long long factorial(int n) {
    if (n <= 1) return 1; // Базовый случай
    return n * factorial(n - 1); // Шаг рекурсии
}

// Итеративная версия факториала для сравнения
long long factorialIter(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Рекурсивная функция для чисел Фибоначчи (экспоненциальная сложность)
int fibRecursive(int n) {
    if (n <= 1) return n; // Базовый случай
    return fibRecursive(n - 1) + fibRecursive(n - 2); // Шаг рекурсии
}

// Итеративная версия Фибоначчи (линейная сложность)
int fibIterative(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// Рекурсивный поиск максимума в массиве
int findMaxRecursive(int arr[], int n) {
    if (n == 1) return arr[0]; // Базовый случай: один элемент
    int maxRest = findMaxRecursive(arr, n - 1);
    return (arr[n - 1] > maxRest) ? arr[n - 1] : maxRest;
}

// Ханойские башни
void hanoi(int n, char from, char to, char aux, int& moveCount) {
    if (n == 1) {
        cout << "Переместить диск 1 с " << from << " на " << to << endl;
        moveCount++;
        return;
    }
    hanoi(n - 1, from, aux, to, moveCount);
    cout << "Переместить диск " << n << " с " << from << " на " << to << endl;
    moveCount++;
    hanoi(n - 1, aux, to, from, moveCount);
}

// Сортировка пузырьком с подсчетом сравнений и обменов
void bubbleSort(int arr[], int n, int& comparisons, int& swaps) {
    comparisons = 0;
    swaps = 0;
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swaps++;
                swapped = true;
            }
        }
        if (!swapped) break; // Оптимизация: если обменов не было, массив отсортирован
    }
}

// Сортировка вставками с подсчетом сравнений и обменов
void insertionSort(int arr[], int n, int& comparisons, int& swaps) {
    comparisons = 0;
    swaps = 0;
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                swaps++;
                j--;
            }
            else {
                break;
            }
        }
        arr[j + 1] = key;
        swaps++; // Учитываем вставку ключа
    }
}

// Сортировка выбором с подсчетом сравнений и обменов
void selectionSort(int arr[], int n, int& comparisons, int& swaps) {
    comparisons = 0;
    swaps = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            comparisons++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
            swaps++;
        }
    }
}

// Вспомогательная функция для вывода массива
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №6. Рекурсия и простые сортировки (Вариант 6) ===" << endl << endl;

    // 1. Факториал
    cout << "1. Факториал числа:" << endl;
    int nFact = 5;
    cout << "Рекурсивно: " << nFact << "! = " << factorial(nFact) << endl;
    cout << "Итеративно: " << nFact << "! = " << factorialIter(nFact) << endl;

    // 2. Числа Фибоначчи
    cout << "\n2. Числа Фибоначчи:" << endl;
    int nFib = 10;
    cout << "Рекурсивно: fib(" << nFib << ") = " << fibRecursive(nFib) << endl;
    cout << "Итеративно: fib(" << nFib << ") = " << fibIterative(nFib) << endl;

    // 3. Максимум в массиве
    cout << "\n3. Поиск максимума в массиве (рекурсивно):" << endl;
    int arrMax[] = { 3, 7, 2, 9, 5, 1, 8 };
    int sizeMax = sizeof(arrMax) / sizeof(arrMax[0]);
    cout << "Массив: ";
    printArray(arrMax, sizeMax);
    cout << "Максимум: " << findMaxRecursive(arrMax, sizeMax) << endl;

    // 4. Ханойские башни (5 дисков)
    cout << "\n4. Ханойские башни (5 дисков):" << endl;
    int moveCount = 0;
    hanoi(5, 'A', 'C', 'B', moveCount);
    cout << "Всего ходов: " << moveCount << endl;

    // 5. Сортировки
    cout << "\n5. Простые сортировки (подсчет сравнений и обменов):" << endl;
    const int N = 10;
    int original[N];
    srand(time(0));
    for (int i = 0; i < N; ++i) {
        original[i] = rand() % 100;
    }
    cout << "Исходный массив: ";
    printArray(original, N);

    // Копии для каждой сортировки
    int arr1[N], arr2[N], arr3[N];
    for (int i = 0; i < N; ++i) {
        arr1[i] = original[i];
        arr2[i] = original[i];
        arr3[i] = original[i];
    }

    int comp, swaps;

    // Пузырьковая
    bubbleSort(arr1, N, comp, swaps);
    cout << "\nПузырьковая сортировка:" << endl;
    cout << "Отсортированный массив: ";
    printArray(arr1, N);
    cout << "Сравнений: " << comp << ", Обменов: " << swaps << endl;

    // Вставками
    insertionSort(arr2, N, comp, swaps);
    cout << "\nСортировка вставками:" << endl;
    cout << "Отсортированный массив: ";
    printArray(arr2, N);
    cout << "Сравнений: " << comp << ", Обменов: " << swaps << endl;

    // Выбором
    selectionSort(arr3, N, comp, swaps);
    cout << "\nСортировка выбором:" << endl;
    cout << "Отсортированный массив: ";
    printArray(arr3, N);
    cout << "Сравнений: " << comp << ", Обменов: " << swaps << endl;

    return 0;
}