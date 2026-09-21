#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

// Класс водителя
class Driver {
private:
    string name;
    string licenseNumber;

    // Статическое поле для подсчёта водителей
    static int driverCount;

public:
    // Конструктор
    Driver(const string& driverName, const string& license)
        : name(driverName), licenseNumber(license) {
        driverCount++;

        cout << "[Driver] Водитель нанят: " << name
            << " (Лицензия: " << licenseNumber << ")" << endl;
    }

    // Деструктор
    ~Driver() {
        driverCount--;

        cout << "[Driver] Водитель удалён: " << name << endl;
    }

    // Запрет копирования объектов
    Driver(const Driver&) = delete;
    Driver& operator=(const Driver&) = delete;

    // Геттеры
    string getName() const {
        return name;
    }

    string getLicenseNumber() const {
        return licenseNumber;
    }

    // Статический метод для получения количества водителей
    static int getDriverCount() {
        return driverCount;
    }

    // Вывод информации о водителе
    void printInfo() const {
        cout << " - Водитель: " << name
            << ", лицензия: " << licenseNumber << endl;
    }
};

// Определение статического поля
int Driver::driverCount = 0;


// Класс автомобиля
class Car {
private:
    string model;
    string plateNumber;

    // Агрегация: автомобиль хранит указатель на водителя, но не управляет временем его жизни
    Driver* currentDriver;

    // Статическое поле для подсчёта автомобилей
    static int carCount;

public:
    // Конструктор
    Car(const string& carModel, const string& plate)
        : model(carModel), plateNumber(plate), currentDriver(nullptr) {
        carCount++;

        cout << "[Car] Автомобиль добавлен: " << model
            << " (" << plateNumber << ")" << endl;
    }

    // Деструктор
    ~Car() {
        carCount--;

        cout << "[Car] Автомобиль удалён: " << model
            << " (" << plateNumber << ")" << endl;

        // Водитель не удаляется, так как используется агрегация
    }

    // Запрет копирования объектов
    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;

    // Назначение водителя автомобилю
    void assignDriver(Driver* driver) {
        if (driver == nullptr) {
            currentDriver = nullptr;
            cout << "[Car] Водитель снят с автомобиля "
                << model << endl;
            return;
        }

        currentDriver = driver;

        cout << "[Car] Водитель " << driver->getName()
            << " назначен на автомобиль " << model << endl;
    }

    // Геттеры
    string getModel() const {
        return model;
    }

    string getPlateNumber() const {
        return plateNumber;
    }

    Driver* getDriver() const {
        return currentDriver;
    }

    // Статический метод для получения количества автомобилей
    static int getCarCount() {
        return carCount;
    }

    // Вывод информации об автомобиле
    void printInfo() const {
        cout << " - Автомобиль: " << model
            << ", номер: " << plateNumber;

        if (currentDriver != nullptr) {
            cout << ", водитель: "
                << currentDriver->getName();
        }
        else {
            cout << ", водитель: отсутствует";
        }

        cout << endl;
    }
};

// Определение статического поля
int Car::carCount = 0;


// Класс таксопарка (Реализует паттерн Singleton)
class TaxiCompany {
private:
    string name;

    // Агрегация: таксопарк хранит указатели на автомобили и водителей
    vector<Car*> cars;
    vector<Driver*> drivers;

    // Указатель на единственный экземпляр класса
    static TaxiCompany* instance;

    // Приватный конструктор Singleton
    TaxiCompany(const string& companyName)
        : name(companyName) {
        cout << "[TaxiCompany] Создан таксопарк: \""
            << name << "\"" << endl;
    }

public:
    // Запрет копирования
    TaxiCompany(const TaxiCompany&) = delete;
    TaxiCompany& operator=(const TaxiCompany&) = delete;

    // Деструктор
    ~TaxiCompany() {
        cout << "[TaxiCompany] Таксопарк \""
            << name << "\" закрыт." << endl;

        // Автомобили и водители не удаляются, потому что они существуют независимо от таксопарка
        cars.clear();
        drivers.clear();
    }

    // Получение единственного экземпляра Singleton
    static TaxiCompany* getInstance(const string& companyName) {
        if (instance == nullptr) {
            instance = new TaxiCompany(companyName);
        }

        return instance;
    }

    // Уничтожение экземпляра Singleton
    static void destroyInstance() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }

    // Добавление автомобиля в таксопарк (Реализует агрегацию)
    void addCar(Car* car) {
        if (car == nullptr) {
            cout << "[TaxiCompany] Ошибка: автомобиль не существует."
                << endl;
            return;
        }

        cars.push_back(car);
        cout << "[TaxiCompany] Автомобиль добавлен в таксопарк."
            << endl;
    }

    // Добавление водителя в таксопарк (Реализует агрегацию)
    void addDriver(Driver* driver) {
        if (driver == nullptr) {
            cout << "[TaxiCompany] Ошибка: водитель не существует."
                << endl;
            return;
        }

        drivers.push_back(driver);
        cout << "[TaxiCompany] Водитель добавлен в таксопарк."
            << endl;
    }

    // Вывод информации о таксопарке
    void printInfo() const {
        cout << "\n----------------------------------------" << endl;
        cout << "   ОТЧЕТ ТАКСОПАРКА: \"" << name << "\"" << endl;
        cout << "----------------------------------------" << endl;

        cout << "\nАвтомобили:" << endl;

        if (cars.empty()) {
            cout << "Нет автомобилей." << endl;
        }
        else {
            for (const auto* car : cars) {
                car->printInfo();
            }
        }

        cout << "\nВодители:" << endl;

        if (drivers.empty()) {
            cout << "Нет водителей." << endl;
        }
        else {
            for (const auto* driver : drivers) {
                driver->printInfo();
            }
        }

        cout << "\nКоличество автомобилей: "
            << Car::getCarCount() << endl;

        cout << "Количество водителей: "
            << Driver::getDriverCount() << endl;

        cout << "----------------------------------------" << endl;
    }
};

// Определение статического поля Singleton
TaxiCompany* TaxiCompany::instance = nullptr;


// Главная функция
int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);


    cout << "Лабораторная работа №1" << endl;
    cout << "Вариант 6. Система управления таксопарком" << endl;

    // Проверка статических счётчиков до создания объектов
    cout << "\nКоличество автомобилей до создания: "
        << Car::getCarCount() << endl;

    cout << "Количество водителей до создания: "
        << Driver::getDriverCount() << endl;

    // Создание Singleton таксопарка
    TaxiCompany* company = TaxiCompany::getInstance("City Taxi");

    // Создание водителей
    Driver* driver1 = new Driver("Иван Иванов", "A777AA");
    Driver* driver2 = new Driver("Пётр Петров", "B888BB");

    // Создание автомобилей
    Car* car1 = new Car("Toyota Camry", "X123XX77");
    Car* car2 = new Car("Hyundai Solaris", "Y456YY77");

    // Добавление автомобилей и водителей в таксопарк
    cout << "\nДобавление объектов в таксопарк:" << endl;

    company->addCar(car1);
    company->addCar(car2);

    company->addDriver(driver1);
    company->addDriver(driver2);

    // Назначение водителя автомобилю
    cout << "\nНазначение водителя:" << endl;

    car1->assignDriver(driver1);

    // Вывод информации о таксопарке
    company->printInfo();

    // Проверка статических счётчиков
    cout << "\nПроверка статических счётчиков:" << endl;

    cout << "Всего автомобилей: "
        << Car::getCarCount() << endl;

    cout << "Всего водителей: "
        << Driver::getDriverCount() << endl;

    // Проверка Singleton
    cout << "\nПроверка Singleton:" << endl;

    TaxiCompany* company2 = TaxiCompany::getInstance("Другой таксопарк");

    if (company == company2) {
        cout << "Оба указателя указывают на один объект." << endl;
    }

    // Проверка агрегации
    cout << "\nПроверка агрегации:" << endl;

    TaxiCompany::destroyInstance();

    cout << "Таксопарк уничтожен." << endl;
    cout << "Автомобили и водители продолжают существовать."
        << endl;

    car1->printInfo();
    car2->printInfo();

    driver1->printInfo();
    driver2->printInfo();

    // Освобождение памяти объектов, которые существовали независимо от таксопарка
    cout << "\nОсвобождение памяти:" << endl;

    delete car1;
    delete car2;

    delete driver1;
    delete driver2;

    // Проверка статических счётчиков после удаления
    cout << "\nПосле удаления объектов:" << endl;

    cout << "Осталось автомобилей: "
        << Car::getCarCount() << endl;

    cout << "Осталось водителей: "
        << Driver::getDriverCount() << endl;

    return 0;
}
