#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Структура для хранения статистики
struct SortStats {
    long long comparisons = 0;
    long long swaps = 0;
};

// Вспомогательная функция для обмена
template <typename T>
void mySwap(T& a, T& b, SortStats& stats) {
    swap(a, b);
    stats.swaps++;
}

// Функция для выбора медианы из трёх и перемещения её в конец
template <typename T>
void medianOfThree(T arr[], int low, int high, SortStats& stats) {
    int mid = low + (high - low) / 2;

    // Сортируем три элемента: arr[low], arr[mid], arr[high]
    if (arr[low] > arr[mid]) {
        stats.comparisons++;
        mySwap(arr[low], arr[mid], stats);
    }
    if (arr[low] > arr[high]) {
        stats.comparisons++;
        mySwap(arr[low], arr[high], stats);
    }
    if (arr[mid] > arr[high]) {
        stats.comparisons++;
        mySwap(arr[mid], arr[high], stats);
    }

    // Теперь медиана в arr[mid], меняем её с arr[high] (опорным)
    mySwap(arr[mid], arr[high], stats);
}

// Быстрая сортировка
template <typename T>
int partition(T arr[], int low, int high, SortStats& stats) {
    // Используем медиану из трёх для выбора опорного элемента
    medianOfThree(arr, low, high, stats);

    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        stats.comparisons++;
        if (arr[j] <= pivot) {
            ++i;
            mySwap(arr[i], arr[j], stats);
        }
    }
    mySwap(arr[i + 1], arr[high], stats);
    return i + 1;
}

template <typename T>
void quickSortRecursive(T arr[], int low, int high, SortStats& stats) {
    if (low < high) {
        int pi = partition(arr, low, high, stats);
        quickSortRecursive(arr, low, pi - 1, stats);
        quickSortRecursive(arr, pi + 1, high, stats);
    }
}

template <typename T>
void quickSort(T arr[], int n, SortStats& stats) {
    stats = { 0, 0 };
    quickSortRecursive(arr, 0, n - 1, stats);
}

// Сортировка слиянием
template <typename T>
void merge(T arr[], int left, int mid, int right, SortStats& stats) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        stats.comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
        stats.swaps++;
    }
    while (i < n1) {
        arr[k++] = L[i++];
        stats.swaps++;
    }
    while (j < n2) {
        arr[k++] = R[j++];
        stats.swaps++;
    }

    delete[] L;
    delete[] R;
}

template <typename T>
void mergeSortRecursive(T arr[], int left, int right, SortStats& stats) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRecursive(arr, left, mid, stats);
        mergeSortRecursive(arr, mid + 1, right, stats);
        merge(arr, left, mid, right, stats);
    }
}

template <typename T>
void mergeSort(T arr[], int n, SortStats& stats) {
    stats = { 0, 0 };
    mergeSortRecursive(arr, 0, n - 1, stats);
}

// Пирамидальная сортировка
template <typename T>
void heapify(T arr[], int n, int i, SortStats& stats) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        stats.comparisons++;
        if (arr[left] > arr[largest]) largest = left;
    }
    if (right < n) {
        stats.comparisons++;
        if (arr[right] > arr[largest]) largest = right;
    }

    if (largest != i) {
        mySwap(arr[i], arr[largest], stats);
        heapify(arr, n, largest, stats);
    }
}

template <typename T>
void heapSort(T arr[], int n, SortStats& stats) {
    stats = { 0, 0 };
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i, stats);
    }
    for (int i = n - 1; i > 0; --i) {
        mySwap(arr[0], arr[i], stats);
        heapify(arr, i, 0, stats);
    }
}

// Интроспективная сортировка (гибрид QuickSort + HeapSort)
template <typename T>
void introsortRecursive(T arr[], int low, int high, int depthLimit, SortStats& stats) {
    if (low < high) {
        if (depthLimit == 0) {
            // Если глубина рекурсии исчерпана, переключаемся на HeapSort
            // Создаем временный массив для статистики, чтобы не сбить счетчики
            SortStats heapStats;
            heapSort(arr + low, high - low + 1, heapStats);
            stats.comparisons += heapStats.comparisons;
            stats.swaps += heapStats.swaps;
            return;
        }
        int pi = partition(arr, low, high, stats);
        introsortRecursive(arr, low, pi - 1, depthLimit - 1, stats);
        introsortRecursive(arr, pi + 1, high, depthLimit - 1, stats);
    }
}

template <typename T>
void introSort(T arr[], int n, SortStats& stats) {
    stats = { 0, 0 };
    if (n <= 1) return;
    int depthLimit = 2 * (int)log2(n);
    introsortRecursive(arr, 0, n - 1, depthLimit, stats);
}

// Вспомогательная функция для вывода массива
template <typename T>
void printArray(T arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Вспомогательная функция для копирования массива
template <typename T>
T* copyArray(T arr[], int n) {
    T* copy = new T[n];
    for (int i = 0; i < n; ++i) copy[i] = arr[i];
    return copy;
}

// Шаблонная функция для тестирования сортировки
template <typename T>
void testSort(const string& name, void (*sortFunc)(T[], int, SortStats&), T original[], int n) {
    T* arr = copyArray(original, n);
    SortStats stats;

    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr, n, stats);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end - start;

    cout << name << ":" << endl;
    cout << "  Время: " << duration.count() << " мс" << endl;
    cout << "  Сравнений: " << stats.comparisons << endl;
    cout << "  Обменов (или записей): " << stats.swaps << endl;

    delete[] arr;
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(time(0));

    cout << "=== Лабораторная работа №7. Эффективные сортировки (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация на небольшом массиве
    cout << "1. Демонстрация на небольшом массиве:" << endl;
    int smallArr[] = { 64, 34, 25, 12, 22, 11, 90, 5 };
    int smallN = sizeof(smallArr) / sizeof(smallArr[0]);

    cout << "Исходный массив: ";
    printArray(smallArr, smallN);

    SortStats stats;
    int* tempArr = copyArray(smallArr, smallN);
    quickSort(tempArr, smallN, stats);
    cout << "После QuickSort: ";
    printArray(tempArr, smallN);
    delete[] tempArr;

    tempArr = copyArray(smallArr, smallN);
    mergeSort(tempArr, smallN, stats);
    cout << "После MergeSort: ";
    printArray(tempArr, smallN);
    delete[] tempArr;

    tempArr = copyArray(smallArr, smallN);
    heapSort(tempArr, smallN, stats);
    cout << "После HeapSort: ";
    printArray(tempArr, smallN);
    delete[] tempArr;

    tempArr = copyArray(smallArr, smallN);
    introSort(tempArr, smallN, stats);
    cout << "После IntroSort: ";
    printArray(tempArr, smallN);
    delete[] tempArr;

    // 2. Сравнение на случайных массивах разного размера
    cout << "\n2. Сравнение производительности на случайных массивах:" << endl;
    int sizes[] = { 1000, 10000, 50000 };

    for (int n : sizes) {
        cout << "\nРазмер массива: " << n << endl;

        int* randomArr = new int[n];
        for (int i = 0; i < n; ++i) {
            randomArr[i] = rand() % 100000;
        }

        testSort("QuickSort", quickSort<int>, randomArr, n);
        testSort("MergeSort", mergeSort<int>, randomArr, n);
        testSort("HeapSort", heapSort<int>, randomArr, n);
        testSort("IntroSort", introSort<int>, randomArr, n);

        delete[] randomArr;
    }

    // 3. Проверка на граничных случаях
    cout << "\n3. Проверка на граничных случаях:" << endl;

    // Уже отсортированный массив
    cout << "\nУже отсортированный массив (размер 10000):" << endl;
    int* sortedArr = new int[10000];
    for (int i = 0; i < 10000; ++i) sortedArr[i] = i;
    testSort("QuickSort", quickSort<int>, sortedArr, 10000);
    testSort("MergeSort", mergeSort<int>, sortedArr, 10000);
    testSort("HeapSort", heapSort<int>, sortedArr, 10000);
    testSort("IntroSort", introSort<int>, sortedArr, 10000);
    delete[] sortedArr;

    // Обратно отсортированный массив
    cout << "\nОбратно отсортированный массив (размер 10000):" << endl;
    int* reverseArr = new int[10000];
    for (int i = 0; i < 10000; ++i) reverseArr[i] = 10000 - i;
    testSort("QuickSort", quickSort<int>, reverseArr, 10000);
    testSort("MergeSort", mergeSort<int>, reverseArr, 10000);
    testSort("HeapSort", heapSort<int>, reverseArr, 10000);
    testSort("IntroSort", introSort<int>, reverseArr, 10000);
    delete[] reverseArr;

    return 0;
}