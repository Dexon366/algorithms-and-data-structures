#define NOMINMAX // Отключаем макросы min/max из Windows.h
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <limits>
#include <Windows.h>

using namespace std;

// Базовый абстрактный класс Account
class Account {
protected:
    string accountNumber;
    double balance;
    string owner;

public:
    Account(const string& num, double bal, const string& own)
        : accountNumber(num), balance(bal), owner(own) {}

    virtual ~Account() = default;

    // Чисто виртуальные методы
    virtual string getType() const = 0;
    virtual void printInfo() const = 0;

    // Виртуальный метод с реализацией по умолчанию
    virtual double calculateInterest() const {
        return 0.0;
    }

    // Геттеры
    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    string getOwner() const { return owner; }

    // Сеттеры
    void setBalance(double b) { balance = b; }

    // Виртуальная сериализация
    virtual void saveToFile(ofstream& out) const {
        out << getType() << endl;
        out << accountNumber << endl;
        out << balance << endl;
        out << owner << endl;
    }
};

// Сберегательный счёт
class SavingsAccount : public Account {
private:
    double interestRate; // процентная ставка

public:
    SavingsAccount(const string& num, double bal, const string& own, double rate)
        : Account(num, bal, own), interestRate(rate) {}

    string getType() const override { return "Сберегательный"; }

    void printInfo() const override {
        cout << "=== Сберегательный счёт ===" << endl;
        cout << "Номер: " << accountNumber << endl;
        cout << "Владелец: " << owner << endl;
        cout << "Баланс: " << balance << " руб." << endl;
        cout << "Процентная ставка: " << interestRate << "%" << endl;
    }

    double calculateInterest() const override {
        return balance * interestRate / 100.0;
    }

    double getInterestRate() const { return interestRate; }

    void saveToFile(ofstream& out) const override {
        Account::saveToFile(out);
        out << interestRate << endl;
    }
};

// Расчётный счёт
class CheckingAccount : public Account {
private:
    double overdraftLimit; // лимит овердрафта

public:
    CheckingAccount(const string& num, double bal, const string& own, double limit)
        : Account(num, bal, own), overdraftLimit(limit) {}

    string getType() const override { return "Расчётный"; }

    void printInfo() const override {
        cout << "=== Расчётный счёт ===" << endl;
        cout << "Номер: " << accountNumber << endl;
        cout << "Владелец: " << owner << endl;
        cout << "Баланс: " << balance << " руб." << endl;
        cout << "Лимит овердрафта: " << overdraftLimit << " руб." << endl;
    }

    void saveToFile(ofstream& out) const override {
        Account::saveToFile(out);
        out << overdraftLimit << endl;
    }
};

// Кредитный счёт
class CreditAccount : public Account {
private:
    double creditLimit;
    double interestRate;

public:
    CreditAccount(const string& num, double bal, const string& own, double limit, double rate)
        : Account(num, bal, own), creditLimit(limit), interestRate(rate) {}

    string getType() const override { return "Кредитный"; }

    void printInfo() const override {
        cout << "=== Кредитный счёт ===" << endl;
        cout << "Номер: " << accountNumber << endl;
        cout << "Владелец: " << owner << endl;
        cout << "Баланс: " << balance << " руб." << endl;
        cout << "Кредитный лимит: " << creditLimit << " руб." << endl;
        cout << "Процентная ставка: " << interestRate << "%" << endl;
    }

    double calculateInterest() const override {
        return balance * interestRate / 100.0;
    }

    double getInterestRate() const { return interestRate; }

    void saveToFile(ofstream& out) const override {
        Account::saveToFile(out);
        out << creditLimit << endl;
        out << interestRate << endl;
    }
};

// Класс Bank (контейнер)
class Bank {
private:
    vector<unique_ptr<Account>> accounts;

public:
    // Добавление счёта
    void addAccount(unique_ptr<Account> acc) {
        accounts.push_back(move(acc));
    }

    // Удаление по номеру
    void removeAccount(const string& number) {
        auto it = find_if(accounts.begin(), accounts.end(),
            [&number](const unique_ptr<Account>& acc) {
                return acc->getAccountNumber() == number;
            });
        if (it != accounts.end()) {
            accounts.erase(it);
            cout << "Счёт " << number << " удалён." << endl;
        }
        else {
            cout << "Счёт " << number << " не найден." << endl;
        }
    }

    // Вывод всех счетов
    void showAll() const {
        if (accounts.empty()) {
            cout << "Список счетов пуст." << endl;
            return;
        }
        for (const auto& acc : accounts) {
            acc->printInfo();
            cout << endl;
        }
    }

    // Сортировка по балансу
    void sortByBalance() {
        sort(accounts.begin(), accounts.end(),
            [](const unique_ptr<Account>& a, const unique_ptr<Account>& b) {
                return a->getBalance() < b->getBalance();
            });
        cout << "Отсортировано по балансу." << endl;
    }

    // Сортировка по номеру
    void sortByNumber() {
        sort(accounts.begin(), accounts.end(),
            [](const unique_ptr<Account>& a, const unique_ptr<Account>& b) {
                return a->getAccountNumber() < b->getAccountNumber();
            });
        cout << "Отсортировано по номеру." << endl;
    }

    // Сортировка по проценту (для счетов, где есть процент)
    void sortByInterest() {
        sort(accounts.begin(), accounts.end(),
            [](const unique_ptr<Account>& a, const unique_ptr<Account>& b) {
                double rateA = 0, rateB = 0;
                if (auto sa = dynamic_cast<SavingsAccount*>(a.get())) rateA = sa->getInterestRate();
                if (auto ca = dynamic_cast<CreditAccount*>(a.get())) rateA = ca->getInterestRate();
                if (auto sb = dynamic_cast<SavingsAccount*>(b.get())) rateB = sb->getInterestRate();
                if (auto cb = dynamic_cast<CreditAccount*>(b.get())) rateB = cb->getInterestRate();
                return rateA < rateB;
            });
        cout << "Отсортировано по процентной ставке." << endl;
    }

    // Поиск по номеру
    void findByNumber(const string& number) const {
        auto it = find_if(accounts.begin(), accounts.end(),
            [&number](const unique_ptr<Account>& acc) {
                return acc->getAccountNumber() == number;
            });
        if (it != accounts.end()) {
            (*it)->printInfo();
        }
        else {
            cout << "Счёт с номером " << number << " не найден." << endl;
        }
    }

    // Поиск по владельцу
    void findByOwner(const string& owner) const {
        bool found = false;
        for (const auto& acc : accounts) {
            if (acc->getOwner() == owner) {
                acc->printInfo();
                cout << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Счета владельца " << owner << " не найдены." << endl;
        }
    }

    // Сохранение в файл
    void saveToFile(const string& filename) const {
        ofstream out(filename);
        if (!out.is_open()) {
            throw runtime_error("Не удалось открыть файл для записи: " + filename);
        }
        out << accounts.size() << endl;
        for (const auto& acc : accounts) {
            acc->saveToFile(out);
        }
        out.close();
        cout << "Данные сохранены в файл " << filename << endl;
    }

    // Загрузка из файла
    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            throw runtime_error("Не удалось открыть файл для чтения: " + filename);
        }
        accounts.clear();
        int count;
        in >> count;
        in.ignore();
        for (int i = 0; i < count; ++i) {
            string type;
            getline(in, type);
            string number, owner;
            double balance;
            getline(in, number);
            in >> balance;
            in.ignore();
            getline(in, owner);

            if (type == "Сберегательный") {
                double rate;
                in >> rate;
                in.ignore();
                accounts.push_back(make_unique<SavingsAccount>(number, balance, owner, rate));
            }
            else if (type == "Расчётный") {
                double limit;
                in >> limit;
                in.ignore();
                accounts.push_back(make_unique<CheckingAccount>(number, balance, owner, limit));
            }
            else if (type == "Кредитный") {
                double limit, rate;
                in >> limit;
                in.ignore();
                in >> rate;
                in.ignore();
                accounts.push_back(make_unique<CreditAccount>(number, balance, owner, limit, rate));
            }
            else {
                throw runtime_error("Неизвестный тип счёта: " + type);
            }
        }
        in.close();
        cout << "Данные загружены из файла " << filename << endl;
    }

    bool isEmpty() const { return accounts.empty(); }
    int size() const { return accounts.size(); }
};

// Вспомогательная функция для ввода целого числа с проверкой
int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите целое число." << endl;
        }
    }
}

// Вспомогательная функция для ввода числа с плавающей точкой
double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите число." << endl;
        }
    }
}

void showMenu() {
    cout << "\n=== БАНКОВСКАЯ СИСТЕМА ===" << endl;
    cout << "1. Добавить сберегательный счёт" << endl;
    cout << "2. Добавить расчётный счёт" << endl;
    cout << "3. Добавить кредитный счёт" << endl;
    cout << "4. Показать все счета" << endl;
    cout << "5. Удалить счёт по номеру" << endl;
    cout << "6. Сортировать по балансу" << endl;
    cout << "7. Сортировать по номеру" << endl;
    cout << "8. Сортировать по проценту" << endl;
    cout << "9. Найти счёт по номеру" << endl;
    cout << "10. Найти счета по владельцу" << endl;
    cout << "11. Сохранить в файл" << endl;
    cout << "12. Загрузить из файла" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Bank bank;
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            switch (choice) {
            case 1: {
                string number, owner;
                double balance, rate;
                cout << "Введите номер счёта: ";
                getline(cin, number);
                cout << "Введите владельца: ";
                getline(cin, owner);
                balance = getDoubleInput("Введите баланс: ");
                rate = getDoubleInput("Введите процентную ставку: ");
                bank.addAccount(make_unique<SavingsAccount>(number, balance, owner, rate));
                cout << "Сберегательный счёт добавлен." << endl;
                break;
            }
            case 2: {
                string number, owner;
                double balance, limit;
                cout << "Введите номер счёта: ";
                getline(cin, number);
                cout << "Введите владельца: ";
                getline(cin, owner);
                balance = getDoubleInput("Введите баланс: ");
                limit = getDoubleInput("Введите лимит овердрафта: ");
                bank.addAccount(make_unique<CheckingAccount>(number, balance, owner, limit));
                cout << "Расчётный счёт добавлен." << endl;
                break;
            }
            case 3: {
                string number, owner;
                double balance, limit, rate;
                cout << "Введите номер счёта: ";
                getline(cin, number);
                cout << "Введите владельца: ";
                getline(cin, owner);
                balance = getDoubleInput("Введите баланс: ");
                limit = getDoubleInput("Введите кредитный лимит: ");
                rate = getDoubleInput("Введите процентную ставку: ");
                bank.addAccount(make_unique<CreditAccount>(number, balance, owner, limit, rate));
                cout << "Кредитный счёт добавлен." << endl;
                break;
            }
            case 4:
                bank.showAll();
                break;
            case 5: {
                string number;
                cout << "Введите номер счёта для удаления: ";
                getline(cin, number);
                bank.removeAccount(number);
                break;
            }
            case 6:
                bank.sortByBalance();
                break;
            case 7:
                bank.sortByNumber();
                break;
            case 8:
                bank.sortByInterest();
                break;
            case 9: {
                string number;
                cout << "Введите номер счёта для поиска: ";
                getline(cin, number);
                bank.findByNumber(number);
                break;
            }
            case 10: {
                string owner;
                cout << "Введите владельца для поиска: ";
                getline(cin, owner);
                bank.findByOwner(owner);
                break;
            }
            case 11: {
                string filename;
                cout << "Введите имя файла для сохранения: ";
                getline(cin, filename);
                bank.saveToFile(filename);
                break;
            }
            case 12: {
                string filename;
                cout << "Введите имя файла для загрузки: ";
                getline(cin, filename);
                bank.loadFromFile(filename);
                break;
            }
            case 0:
                cout << "До свидания!" << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}