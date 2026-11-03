#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <algorithm>

using namespace std;

template <typename T>
class Vector {
private:
    T* data;
    int sz;
    int cap;

    // Обмен содержимым двух векторов
    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

public:
    // Конструктор по умолчанию
    Vector() : data(nullptr), sz(0), cap(0) {}

    // Конструктор с заданным размером
    explicit Vector(int n) : data(nullptr), sz(0), cap(0) {
        if (n < 0)
            throw invalid_argument("Negative size");

        if (n > 0) {
            data = new T[n]{};
            sz = n;
            cap = n;
        }
    }

    // Конструктор с размером и значением
    Vector(int n, const T& value) : Vector(n) {
        for (int i = 0; i < sz; i++)
            data[i] = value;
    }

    // Конструктор копирования
    Vector(const Vector& other) : Vector(other.sz) {
        for (int i = 0; i < sz; i++)
            data[i] = other.data[i];
    }

    // Конструктор перемещения
    Vector(Vector&& other) noexcept
        : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    // Деструктор
    ~Vector() {
        delete[] data;
    }

    // Присваивание копированием
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // Присваивание перемещением
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

    int size() const {
        return sz;
    }

    int capacity() const {
        return cap;
    }

    bool empty() const {
        return sz == 0;
    }

    // Выделение памяти большей вместимости
    void reserve(int newCap) {
        if (newCap <= cap)
            return;

        T* newData = new T[newCap];

        try {
            for (int i = 0; i < sz; i++)
                newData[i] = data[i];
        }
        catch (...) {
            delete[] newData;
            throw;
        }

        delete[] data;
        data = newData;
        cap = newCap;
    }

    // Добавление в конец
    void push_back(const T& value) {
        if (sz == cap) {
            T saved = value;
            reserve(cap == 0 ? 1 : cap * 2);
            data[sz] = saved;
        }
        else {
            data[sz] = value;
        }

        sz++;
    }

    // Удаление последнего элемента
    void pop_back() {
        if (empty())
            throw out_of_range("Vector is empty");

        sz--;
    }

    // Изменение размера
    void resize(int newSize, const T& value = T()) {
        if (newSize < 0)
            throw invalid_argument("Negative size");

        if (newSize > sz) {
            T saved = value;

            if (newSize > cap)
                reserve(newSize);

            for (int i = sz; i < newSize; i++)
                data[i] = saved;
        }

        sz = newSize;
    }

    // Очистка вектора
    void clear() {
        sz = 0;
    }

    // Доступ по индексу
    T& operator[](int index) {
        if (index < 0 || index >= sz)
            throw out_of_range("Invalid index");

        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= sz)
            throw out_of_range("Invalid index");

        return data[index];
    }

    // Итераторы
    T* begin() {
        return data;
    }

    T* end() {
        return data == nullptr ? nullptr : data + sz;
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data == nullptr ? nullptr : data + sz;
    }

    // Конкатенация двух векторов
    Vector operator+(const Vector& other) const {
        Vector result(sz + other.sz);

        for (int i = 0; i < sz; i++)
            result.data[i] = data[i];

        for (int i = 0; i < other.sz; i++)
            result.data[sz + i] = other.data[i];

        return result;
    }

    // Умножение элементов на число
    Vector operator*(const T& number) const {
        Vector result(sz);

        for (int i = 0; i < sz; i++)
            result.data[i] = data[i] * number;

        return result;
    }
};

int main() {
    setlocale(LC_ALL, "");

    cout << "========== СОЗДАНИЕ ВЕКТОРОВ ==========\n";

    Vector<int> a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    Vector<int> b(2, 5);

    cout << "Вектор a: ";
    for (int x : a)
        cout << x << " ";

    cout << "\nВектор b: ";
    for (int x : b)
        cout << x << " ";

    cout << "\n\n========== ОПЕРАТОР + ==========\n";

    Vector<int> c = a + b;

    cout << "a + b: ";
    for (int x : c)
        cout << x << " ";

    cout << "\n\n========== ОПЕРАТОР * ==========\n";

    Vector<int> d = a * 3;

    cout << "a * 3: ";
    for (int x : d)
        cout << x << " ";

    cout << "\n\n========== КОПИРОВАНИЕ ==========\n";

    Vector<int> copy(a);
    copy[0] = 999;

    cout << "Первый элемент a: " << a[0] << "\n";
    cout << "Первый элемент копии: " << copy[0] << "\n";

    Vector<int> assigned;
    assigned = b;
    assigned = assigned; // Проверка самоприсваивания

    cout << "После присваивания: ";
    for (int x : assigned)
        cout << x << " ";

    cout << "\n\n========== ПЕРЕМЕЩЕНИЕ ==========\n";

    Vector<int> moved(std::move(copy));

    Vector<int> movedAgain;
    movedAgain = std::move(moved);

    cout << "После перемещения: ";
    for (int x : movedAgain)
        cout << x << " ";

    cout << "\n\n========== РАСШИРЕНИЕ МАССИВА ==========\n";

    Vector<int> v;

    for (int i = 1; i <= 6; i++) {
        v.push_back(i);

        cout << "Добавили " << i
            << " | size = " << v.size()
            << " | capacity = " << v.capacity() << "\n";
    }

    cout << "\n========== RESERVE И RESIZE ==========\n";

    v.reserve(12);

    cout << "После reserve(12): size = " << v.size()
        << ", capacity = " << v.capacity() << "\n";

    v.resize(8, 42);

    cout << "После resize(8, 42): ";
    for (int x : v)
        cout << x << " ";

    v.pop_back();

    cout << "\nПосле pop_back(): ";
    for (int x : v)
        cout << x << " ";

    cout << "\n\n========== ИТЕРАТОРЫ ==========\n";

    cout << "Перебор через begin() и end(): ";

    for (auto it = v.begin(); it != v.end(); ++it)
        cout << *it << " ";

    cout << "\nСортировка через std::sort(): ";

    std::sort(c.begin(), c.end());

    for (int x : c)
        cout << x << " ";

    cout << "\n\n========== ШАБЛОН СО STRING ==========\n";

    Vector<string> words;
    words.push_back("Привет");
    words.push_back("мир");

    for (const string& word : words)
        cout << word << " ";

    cout << "\n\n========== CLEAR И EMPTY ==========\n";

    v.clear();

    cout << "Размер после clear(): " << v.size() << "\n";
    cout << "Вектор пуст: " << boolalpha << v.empty() << "\n";
    cout << "Вместимость осталась: " << v.capacity() << "\n";

    cout << "\n========== ПРОВЕРКА ОШИБОК ==========\n";

    try {
        v.pop_back();
    }
    catch (const out_of_range& e) {
        cout << "Ошибка удаления: " << e.what() << "\n";
    }

    try {
        cout << a[100];
    }
    catch (const out_of_range& e) {
        cout << "Ошибка индекса: " << e.what() << "\n";
    }

    cout << "\n========== КОНЕЦ ПРОГРАММЫ ==========\n";

    return 0;
}
