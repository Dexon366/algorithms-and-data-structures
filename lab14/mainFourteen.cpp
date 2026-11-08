#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <Windows.h>

using namespace std;

// Задача о рюкзаке: 1D оптимизация (только максимальная стоимость)
int knapsack1D(int W, const vector<int>& weights, const vector<int>& values) {
    int n = weights.size();
    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; ++i) {
        // Обход в обратном порядке, чтобы не использовать предмет дважды
        for (int w = W; w >= weights[i]; --w) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[W];
}

// Задача о рюкзаке: 2D таблица с восстановлением предметов
struct KnapsackResult {
    int maxValue;
    vector<int> selectedItems;
};

KnapsackResult knapsack2DWithRecovery(int W, const vector<int>& weights, const vector<int>& values) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Заполнение таблицы
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= W; ++w) {
            if (weights[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            }
            else {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            }
        }
    }

    // Восстановление предметов
    vector<int> selected;
    int w = W;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(i - 1);
            w -= weights[i - 1];
        }
    }
    // Переворачиваем, чтобы порядок был возрастающим
    reverse(selected.begin(), selected.end());

    return { dp[n][W], selected };
}

// LCS: рекурсия + мемоизация
int lcsMemoRecursive(const string& X, const string& Y, int i, int j, vector<vector<int>>& memo) {
    if (i == 0 || j == 0) return 0;
    if (memo[i][j] != -1) return memo[i][j];

    if (X[i - 1] == Y[j - 1]) {
        memo[i][j] = 1 + lcsMemoRecursive(X, Y, i - 1, j - 1, memo);
    }
    else {
        memo[i][j] = max(lcsMemoRecursive(X, Y, i - 1, j, memo),
            lcsMemoRecursive(X, Y, i, j - 1, memo));
    }
    return memo[i][j];
}

int lcsMemo(const string& X, const string& Y) {
    int m = X.length();
    int n = Y.length();
    vector<vector<int>> memo(m + 1, vector<int>(n + 1, -1));
    return lcsMemoRecursive(X, Y, m, n, memo);
}

// LCS: табличный метод с восстановлением строки
struct LCSResult {
    int length;
    string subsequence;
};

LCSResult lcsTable(const string& X, const string& Y) {
    int m = X.length();
    int n = Y.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Восстановление подпоследовательности
    string lcsStr = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcsStr = X[i - 1] + lcsStr;
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        }
        else {
            j--;
        }
    }

    return { dp[m][n], lcsStr };
}

// Вывод таблицы ДП для рюкзака
void printKnapsackTable(const vector<int>& weights, const vector<int>& values, int W) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= W; ++w) {
            if (weights[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            }
            else {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            }
        }
    }

    cout << "Таблица ДП для рюкзака:" << endl;
    cout << "   ";
    for (int w = 0; w <= W; ++w) cout << w << " ";
    cout << endl;
    for (int i = 0; i <= n; ++i) {
        cout << i << ": ";
        for (int w = 0; w <= W; ++w) {
            cout << dp[i][w] << " ";
        }
        cout << endl;
    }
}

// Вывод таблицы ДП для LCS
void printLCSTable(const string& X, const string& Y) {
    int m = X.length();
    int n = Y.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    cout << "Таблица ДП для LCS:" << endl;
    cout << "   ";
    for (int j = 0; j <= n; ++j) cout << j << " ";
    cout << endl;
    for (int i = 0; i <= m; ++i) {
        cout << i << ": ";
        for (int j = 0; j <= n; ++j) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "=== Лабораторная работа №14. Динамическое программирование (Вариант 6) ===" << endl << endl;

    // 1. Задача о рюкзаке (1D оптимизация и 2D с восстановлением)
    cout << "1. Задача о рюкзаке:" << endl;
    int N, W;
    cout << "Введите количество предметов: ";
    cin >> N;
    cout << "Введите вместимость рюкзака: ";
    cin >> W;

    vector<int> weights(N);
    vector<int> values(N);
    for (int i = 0; i < N; ++i) {
        cout << "Предмет " << i << ": вес = ";
        cin >> weights[i];
        cout << "ценность = ";
        cin >> values[i];
    }

    // 1D оптимизация
    int maxValue1D = knapsack1D(W, weights, values);
    cout << "\nМаксимальная стоимость (1D оптимизация): " << maxValue1D << endl;

    // 2D с восстановлением
    KnapsackResult result2D = knapsack2DWithRecovery(W, weights, values);
    cout << "Максимальная стоимость (2D с восстановлением): " << result2D.maxValue << endl;
    cout << "Выбранные предметы (индексы): ";
    for (int idx : result2D.selectedItems) {
        cout << idx << " ";
    }
    cout << endl;

    // Вывод таблицы ДП для рюкзака
    cout << endl;
    printKnapsackTable(weights, values, W);

    // 2. LCS
    cout << "\n2. Наибольшая общая подпоследовательность (LCS):" << endl;
    string X, Y;
    cout << "Введите первую строку (X): ";
    cin >> X;
    cout << "Введите вторую строку (Y): ";
    cin >> Y;

    LCSResult lcsResult = lcsTable(X, Y);
    cout << "Длина LCS: " << lcsResult.length << endl;
    cout << "LCS: " << lcsResult.subsequence << endl;

    // Вывод таблицы ДП для LCS
    cout << endl;
    printLCSTable(X, Y);

    // 3. Сравнение производительности LCS
    cout << "\n3. Сравнение производительности LCS (рекурсия+мемоизация vs табличный):" << endl;
    cout << "Длина X\tДлина Y\tМемоизация (мкс)\tТабличный (мкс)" << endl;
    cout << "---------------------------------------------------------" << endl;

    vector<pair<string, string>> testCases = {
        {"ABCBDAB", "BDCABA"},
        {"AGGTAB", "GXTXAYB"},
        {"ABCDEFGHIJ", "JIHGFEDCBA"},
        {"ABCDEFGHIJKLMNOP", "PONMLKJIHGFEDCBA"},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ZYXWVUTSRQPONMLKJIHGFEDCBA"}
    };

    for (const auto& test : testCases) {
        auto start = chrono::high_resolution_clock::now();
        int lenMemo = lcsMemo(test.first, test.second);
        auto end = chrono::high_resolution_clock::now();
        long long timeMemo = chrono::duration_cast<chrono::microseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        LCSResult resTable = lcsTable(test.first, test.second);
        end = chrono::high_resolution_clock::now();
        long long timeTable = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << test.first.length() << "\t" << test.second.length() << "\t" << timeMemo << "\t\t\t" << timeTable << endl;
    }

    return 0;
}