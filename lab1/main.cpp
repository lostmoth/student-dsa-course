#include <iostream>
#include <string>
#include <vector>

class Student {
public:
    Student(std::string name, unsigned age) : name{ name }, age{ age } {
        std::cout << "Студент создан: " << name << std::endl;
        count++;
    }

    ~Student() {
        std::cout << "Студент удалён: " << name << std::endl;
        count--;
    }

    Student(const Student&) = delete;
    Student& operator=(const Student&) = delete;

    void printInfo() const {
        std::cout << "Имя: " << name << "\tВозраст: " << age << std::endl;
    }

    static int getCount() {
        return count;
    }

private:
    std::string name;
    unsigned age;
    static int count; // Общий счётчик студентов
};

int Student::count = 0;

class Course {
public:
    Course(std::string title) : title{ title } {
        std::cout << "Курс создан: " << title << std::endl;
    }

    ~Course() {
        std::cout << "Курс удалён: " << title << std::endl;
    }

    void addStudent(Student* student) {
        if (student == nullptr) {
            std::cout << "Некого добавлять!" << std::endl;
            return;
        }

        for (Student* c_student : students) {
            if (c_student == student) {
                std::cout << "Данный студент уже есть на курсе!" << std::endl;
                return;
            }
        }

        students.push_back(student);
    }

    void printInfo() const {
        std::cout << "Курс: " << title << std::endl;

        if (students.empty()) {
            std::cout << "Студентов нет!" << std::endl;
            return;
        }

        std::cout << "Студенты:" << std::endl;

        for (Student* student : students) {
            student->printInfo();
        }
    }

private:
    std::string title;
    std::vector<Student*> students; // Агрегация: студенты существуют отдельно от курса
};

class TrainingCenter {
public:
    TrainingCenter(const TrainingCenter&) = delete;
    TrainingCenter& operator=(const TrainingCenter&) = delete;

    static TrainingCenter& getInstance() {
        static TrainingCenter instance;
        return instance;
    }

    void addCourse(Course* course) {
        if (course == nullptr) {
            std::cout << "Нечего добавлять!" << std::endl;
            return;
        }

        for (Course* c_course : courses) {
            if (c_course == course) {
                std::cout << "Данный курс уже есть в учебном центре!" << std::endl;
                return;
            }
        }

        courses.push_back(course);
    }

    void removeCourse(Course* course) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            if (*it == course) {
                courses.erase(it); // Удаляем указатель, а не сам курс
                return;
            }
        }
    }

    void printInfo() const {
        std::cout << "Учебный центр:" << std::endl;

        if (courses.empty()) {
            std::cout << "Курсов нет!" << std::endl;
            return;
        }

        for (Course* course : courses) {
            course->printInfo();
            std::cout << std::endl;
        }
    }

private:
    TrainingCenter() {
        std::cout << "Учебный центр создан" << std::endl;
    }

    std::vector<Course*> courses; // Агрегация курсов
};

int main() {
    setlocale(LC_ALL, "Ru");

    std::cout << "\n========== СОЗДАНИЕ СТУДЕНТОВ ==========\n";

    Student s1{ "Виталий", 20 };
    Student s2{ "Александр", 19 };
    Student s3{ "Глеб", 20 };

    std::cout << "\nКоличество студентов: " << Student::getCount() << std::endl;

    std::cout << "\n========== ПРОВЕРКА SINGLETON ==========\n";

    TrainingCenter& center1 = TrainingCenter::getInstance();
    TrainingCenter& center2 = TrainingCenter::getInstance();

    std::cout << "center1 и center2 - один объект: "
        << (&center1 == &center2) << std::endl; // 1 - один экземпляр

    {
        std::cout << "\n========== СОЗДАНИЕ КУРСОВ ==========\n";

        Course c1{ "Программная инженерия" };
        c1.addStudent(&s1);
        c1.addStudent(&s2);

        Course c2{ "Информационная безопасность" };
        c2.addStudent(&s2);
        c2.addStudent(&s3);

        std::cout << "\n========== ДОБАВЛЕНИЕ КУРСОВ В ЦЕНТР ==========\n";

        center1.addCourse(&c1);
        center1.addCourse(&c2);

        center1.printInfo();

        std::cout << "\n========== ПРОВЕРКА ДУБЛИКАТОВ ==========\n";

        c1.addStudent(&s2);
        center1.addCourse(&c2);

        std::cout << "\n========== ПРОВЕРКА nullptr ==========\n";

        c1.addStudent(nullptr);
        center1.addCourse(nullptr);

        std::cout << "\n========== ПРОВЕРКА ПУСТОГО КУРСА ==========\n";

        Course emptyCourse{ "Пустой курс" };
        emptyCourse.printInfo();

        std::cout << "\n========== УДАЛЕНИЕ КУРСОВ ИЗ ЦЕНТРА ==========\n";

        center1.removeCourse(&c1);
        center1.removeCourse(&c2);

        // Повторное удаление несуществующего в центре курса
        center1.removeCourse(&c1);

        center1.printInfo();

        std::cout << "\n========== УНИЧТОЖЕНИЕ ЛОКАЛЬНЫХ КУРСОВ ==========\n";
    }

    std::cout << "\n========== ПРОВЕРКА АГРЕГАЦИИ ==========\n";

    std::cout << "Курсы уничтожены, но студенты существуют:\n";
    s1.printInfo();
    s2.printInfo();
    s3.printInfo();

    std::cout << "\n========== ПРОВЕРКА СТАТИЧЕСКОГО СЧЁТЧИКА ==========\n";

    std::cout << "Студентов до создания нового: "
        << Student::getCount() << std::endl;

    {
        Student s4{ "Максим", 21 };

        std::cout << "Студентов после создания: "
            << Student::getCount() << std::endl;
    }

    std::cout << "Студентов после удаления: "
        << Student::getCount() << std::endl;

    std::cout << "\n========== ЗАВЕРШЕНИЕ ПРОГРАММЫ ==========\n";

    return 0;
}
