#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

// Вычисление префикс-функции для образца
vector<int> computePrefixFunction(const string& pattern) {
    int m = pattern.length();
    vector<int> pi(m, 0);

    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

// Алгоритм КМП для поиска всех вхождений (с перекрытием)
vector<int> kmpSearch(const string& text, const string& pattern) {
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();

    if (m == 0 || m > n) return positions;

    vector<int> pi = computePrefixFunction(pattern);
    int j = 0; // количество совпавших символов образца

    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            j = pi[j - 1];
        }
        if (text[i] == pattern[j]) {
            j++;
        }
        if (j == m) {
            // Нашли вхождение, начинающееся с позиции i - m + 1
            positions.push_back(i - m + 1);
            // Продолжаем поиск с перекрытием
            j = pi[j - 1];
        }
    }
    return positions;
}

// Структура для студента (для задачи STL)
struct Student {
    string name;
    int score;
};

// Компаратор для сортировки студентов по убыванию баллов, затем по имени
bool compareStudents(const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score > b.score; // по убыванию баллов
    }
    return a.name < b.name; // по возрастанию имени
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №15. Алгоритм КМП и STL (Вариант 6) ===" << endl << endl;

    // 1. Поиск с помощью КМП (с перекрытием)
    cout << "1. Поиск подстроки с помощью КМП (с перекрытием):" << endl;
    string text, pattern;
    cout << "Введите текст: ";
    getline(cin, text);
    cout << "Введите образец для поиска: ";
    getline(cin, pattern);

    if (pattern.empty()) {
        cout << "Образец не может быть пустым!" << endl;
    }
    else {
        vector<int> positions = kmpSearch(text, pattern);
        cout << "Найдено вхождений: " << positions.size() << endl;
        if (!positions.empty()) {
            cout << "Позиции вхождений: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
            cout << endl;

            // Визуализация
            cout << "\nВизуализация:" << endl;
            cout << text << endl;
            for (int pos : positions) {
                for (int i = 0; i < pos; ++i) cout << " ";
                cout << pattern << endl;
            }
        }
    }

    // 2. STL: Сортировка студентов по баллам
    cout << "\n2. STL: Сортировка студентов по баллам:" << endl;
    vector<Student> students = {
        {"Иванов", 85},
        {"Петров", 92},
        {"Сидоров", 78},
        {"Козлов", 92},
        {"Смирнов", 85},
        {"Волков", 95}
    };

    cout << "Исходный список студентов:" << endl;
    for (const auto& s : students) {
        cout << "  " << s.name << ": " << s.score << endl;
    }

    // Сортировка
    sort(students.begin(), students.end(), compareStudents);

    cout << "\nОтсортированный список (по убыванию баллов, затем по имени):" << endl;
    for (const auto& s : students) {
        cout << "  " << s.name << ": " << s.score << endl;
    }

    // 3. Дополнительно: демонстрация других возможностей STL (для полноты)
    cout << "\n3. Дополнительная демонстрация STL (поиск максимального балла):" << endl;
    auto maxIt = max_element(students.begin(), students.end(),
        [](const Student& a, const Student& b) { return a.score < b.score; });
    cout << "Студент с максимальным баллом: " << maxIt->name << " (" << maxIt->score << ")" << endl;

    return 0;
}