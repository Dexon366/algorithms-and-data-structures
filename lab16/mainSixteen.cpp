#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <memory>
#include <Windows.h>

using namespace std;

// Базовый абстрактный класс Student
class Student {
protected:
    string name;
    int course;
    string group;

public:
    Student(const string& n, int c, const string& g) : name(n), course(c), group(g) {}
    virtual ~Student() = default;

    // Чисто виртуальные методы
    virtual string getType() const = 0;
    virtual void attendClass() const = 0;
    virtual string getInfo() const = 0;

    // Виртуальный метод для расчёта стипендии (средний уровень)
    virtual double calculateScholarship() const {
        return 0.0;
    }

    string getName() const { return name; }
    int getCourse() const { return course; }
    string getGroup() const { return group; }
};

// Производный класс: Студент бакалавриата
class UndergradStudent : public Student {
public:
    UndergradStudent(const string& n, int c, const string& g) : Student(n, c, g) {}

    string getType() const override { return "Бакалавр"; }
    void attendClass() const override {
        cout << name << " посещает лекции и семинары." << endl;
    }
    string getInfo() const override {
        return "Студент: " + name + ", курс: " + to_string(course) + ", группа: " + group + ", тип: Бакалавр";
    }
    double calculateScholarship() const override {
        return 1500.0; // базовая стипендия
    }
};

// Производный класс: Магистрант
class GradStudent : public Student {
public:
    GradStudent(const string& n, int c, const string& g) : Student(n, c, g) {}

    string getType() const override { return "Магистр"; }
    void attendClass() const override {
        cout << name << " посещает специализированные курсы." << endl;
    }
    void doResearch() const {
        cout << name << " проводит научное исследование." << endl;
    }
    string getInfo() const override {
        return "Студент: " + name + ", курс: " + to_string(course) + ", группа: " + group + ", тип: Магистр";
    }
    double calculateScholarship() const override {
        return 2500.0;
    }
};

// Производный класс: Аспирант
class PhdStudent : public Student {
public:
    PhdStudent(const string& n, int c, const string& g) : Student(n, c, g) {}

    string getType() const override { return "Аспирант"; }
    void attendClass() const override {
        cout << name << " посещает лекции и ведёт семинары." << endl;
    }
    void defendThesis() const {
        cout << name << " готовится к защите диссертации." << endl;
    }
    string getInfo() const override {
        return "Студент: " + name + ", курс: " + to_string(course) + ", группа: " + group + ", тип: Аспирант";
    }
    double calculateScholarship() const override {
        return 4000.0;
    }
};

// Класс Course (Курс)
class Course {
private:
    string title;
    string teacher;
    vector<Student*> students;
    map<Student*, int> grades;

public:
    Course(const string& t, const string& teach) : title(t), teacher(teach) {}

    void enrollStudent(Student* s) {
        students.push_back(s);
        grades[s] = 0;
    }

    void setGrade(Student* s, int grade) {
        grades[s] = grade;
    }

    void printCourseInfo() const {
        cout << "Курс: " << title << ", Преподаватель: " << teacher << endl;
        cout << "Студенты:" << endl;
        for (Student* s : students) {
            cout << "  " << s->getInfo() << ", оценка: " << grades.at(s) << endl;
        }
    }

    string getTitle() const { return title; }
    string getTeacher() const { return teacher; }
    vector<Student*> getStudents() const { return students; }
    map<Student*, int> getGrades() const { return grades; }
};

// Класс Schedule (Расписание)
class Schedule {
private:
    struct Entry {
        string day;
        string time;
        Course* course;
    };
    vector<Entry> entries;

public:
    void addEntry(const string& day, const string& time, Course* course) {
        entries.push_back({ day, time, course });
    }

    void printSchedule() const {
        cout << "Расписание:" << endl;
        for (const auto& e : entries) {
            cout << "  " << e.day << " " << e.time << " - " << e.course->getTitle() << endl;
        }
    }
};

// Функция для сохранения данных в файл
void saveToFile(const vector<Student*>& students, const vector<Course*>& courses, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    // Сохраняем студентов
    file << students.size() << endl;
    for (Student* s : students) {
        file << s->getType() << endl;
        file << s->getName() << endl;
        file << s->getCourse() << endl;
        file << s->getGroup() << endl;
    }

    // Сохраняем курсы
    file << courses.size() << endl;
    for (Course* c : courses) {
        file << c->getTitle() << endl;
        file << c->getTeacher() << endl;
        auto studentsInCourse = c->getStudents();
        auto gradesInCourse = c->getGrades();
        file << studentsInCourse.size() << endl;
        for (Student* s : studentsInCourse) {
            file << s->getName() << endl;
            file << gradesInCourse.at(s) << endl;
        }
    }

    file.close();
    cout << "Данные сохранены в файл " << filename << endl;
}

// Функция для загрузки данных из файла
void loadFromFile(vector<Student*>& students, vector<Course*>& courses, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения!" << endl;
        return;
    }

    // Очищаем текущие данные
    for (Student* s : students) delete s;
    for (Course* c : courses) delete c;
    students.clear();
    courses.clear();

    // Загружаем студентов
    int studentCount;
    file >> studentCount;
    file.ignore(); // игнорируем символ новой строки
    for (int i = 0; i < studentCount; ++i) {
        string type, name, group;
        int course;
        getline(file, type);
        getline(file, name);
        file >> course;
        file.ignore();
        getline(file, group);

        if (type == "Бакалавр") {
            students.push_back(new UndergradStudent(name, course, group));
        }
        else if (type == "Магистр") {
            students.push_back(new GradStudent(name, course, group));
        }
        else if (type == "Аспирант") {
            students.push_back(new PhdStudent(name, course, group));
        }
    }

    // Загружаем курсы
    int courseCount;
    file >> courseCount;
    file.ignore();
    for (int i = 0; i < courseCount; ++i) {
        string title, teacher;
        getline(file, title);
        getline(file, teacher);
        Course* c = new Course(title, teacher);
        int studentCountInCourse;
        file >> studentCountInCourse;
        file.ignore();
        for (int j = 0; j < studentCountInCourse; ++j) {
            string studentName;
            int grade;
            getline(file, studentName);
            file >> grade;
            file.ignore();
            // Находим студента по имени
            for (Student* s : students) {
                if (s->getName() == studentName) {
                    c->enrollStudent(s);
                    c->setGrade(s, grade);
                    break;
                }
            }
        }
        courses.push_back(c);
    }

    file.close();
    cout << "Данные загружены из файла " << filename << endl;
}

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    vector<Student*> students;
    vector<Course*> courses;
    Schedule schedule;

    // Демонстрационные данные
    students.push_back(new UndergradStudent("Иванов Иван", 1, "Группа 101"));
    students.push_back(new GradStudent("Петров Петр", 2, "Группа 201"));
    students.push_back(new PhdStudent("Сидоров Сидор", 3, "Группа 301"));

    Course* math = new Course("Математика", "Проф. Смирнов");
    Course* physics = new Course("Физика", "Доц. Кузнецов");
    courses.push_back(math);
    courses.push_back(physics);

    // Записываем студентов на курсы
    for (Student* s : students) {
        math->enrollStudent(s);
        physics->enrollStudent(s);
    }

    // Ставим оценки
    math->setGrade(students[0], 5);
    math->setGrade(students[1], 4);
    math->setGrade(students[2], 5);
    physics->setGrade(students[0], 4);
    physics->setGrade(students[1], 5);
    physics->setGrade(students[2], 3);

    // Добавляем расписание
    schedule.addEntry("Понедельник", "09:00", math);
    schedule.addEntry("Среда", "11:00", physics);

    // Демонстрация полиморфизма
    cout << "=== Демонстрация полиморфизма ===" << endl;
    for (Student* s : students) {
        s->attendClass();
        cout << "  Стипендия: " << s->calculateScholarship() << " руб." << endl;
        cout << "  " << s->getInfo() << endl;
    }

    // Вызов специфичных методов
    cout << "\n=== Специфичные методы ===" << endl;
    for (Student* s : students) {
        if (auto* gs = dynamic_cast<GradStudent*>(s)) {
            gs->doResearch();
        }
        else if (auto* phd = dynamic_cast<PhdStudent*>(s)) {
            phd->defendThesis();
        }
    }

    // Информация о курсах
    cout << "\n=== Информация о курсах ===" << endl;
    for (Course* c : courses) {
        c->printCourseInfo();
        cout << endl;
    }

    // Расписание
    cout << "=== Расписание ===" << endl;
    schedule.printSchedule();

    // Сохранение в файл
    cout << "\n=== Сохранение данных ===" << endl;
    saveToFile(students, courses, "university_data.txt");

    // Очистка памяти (для демонстрации загрузки)
    for (Student* s : students) delete s;
    for (Course* c : courses) delete c;
    students.clear();
    courses.clear();

    // Загрузка из файла
    cout << "\n=== Загрузка данных ===" << endl;
    loadFromFile(students, courses, "university_data.txt");

    // Проверка загруженных данных
    cout << "\n=== Проверка загруженных данных ===" << endl;
    for (Course* c : courses) {
        c->printCourseInfo();
        cout << endl;
    }

    // Очистка памяти
    for (Student* s : students) delete s;
    for (Course* c : courses) delete c;

    return 0;
}