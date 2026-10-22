#include <iostream>
#include <deque>
#include <chrono>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class Deque {
private:
    T* data;
    int capacity;
    int count;
    int first;

    // Увеличение вместимости массива
    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < count; i++) {
            newData[i] = data[(first + i) % capacity];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
        first = 0;
    }

public:
    Deque() : capacity(4), count(0), first(0) {
        data = new T[capacity];
    }

    ~Deque() {
        delete[] data;
    }

    // Запрет копирования для защиты от двойного удаления памяти
    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    // Добавление элемента в конец
    void push_back(const T& value) {
        if (count == capacity)
            resize();

        data[(first + count) % capacity] = value;
        count++;
    }

    // Добавление элемента в начало
    void push_front(const T& value) {
        if (count == capacity)
            resize();

        first = (first - 1 + capacity) % capacity;
        data[first] = value;
        count++;
    }

    // Удаление последнего элемента
    void pop_back() {
        if (count == 0)
            throw out_of_range("Deque is empty");

        count--;

        if (count == 0)
            first = 0;
    }

    // Удаление первого элемента
    void pop_front() {
        if (count == 0)
            throw out_of_range("Deque is empty");

        first = (first + 1) % capacity;
        count--;

        if (count == 0)
            first = 0;
    }

    // Получение элемента по индексу
    T& operator[](int index) {
        if (index < 0 || index >= count)
            throw out_of_range("Invalid index");

        return data[(first + index) % capacity];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= count)
            throw out_of_range("Invalid index");

        return data[(first + index) % capacity];
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }
};

// Измерение времени выполнения
template <typename Func>
long long measureTime(Func func) {
    auto start = chrono::steady_clock::now();

    func();

    auto end = chrono::steady_clock::now();

    return chrono::duration_cast<chrono::microseconds>(
        end - start).count();
}

int main() {
    setlocale(LC_ALL, "");

    cout << "=== Проверка Deque<int> ===\n";

    Deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_front(5);
    d.push_front(1);
    d.push_back(30); // Вызывает расширение массива

    cout << "Элементы: ";
    for (int i = 0; i < d.size(); i++)
        cout << d[i] << " ";
    cout << "\n";

    d.pop_front();
    d.pop_back();

    cout << "После удаления с двух сторон: ";
    for (int i = 0; i < d.size(); i++)
        cout << d[i] << " ";
    cout << "\n";

    cout << "\n=== Проверка Deque<string> ===\n";

    Deque<string> words;
    words.push_back("мир");
    words.push_front("Привет");

    cout << words[0] << " " << words[1] << "\n";

    cout << "\n=== Проверка ошибок ===\n";

    try {
        Deque<int> emptyDeque;
        emptyDeque.pop_front();
    }
    catch (const out_of_range& e) {
        cout << "Удаление из пустой очереди: "
            << e.what() << "\n";
    }

    try {
        cout << d[100] << "\n";
    }
    catch (const out_of_range& e) {
        cout << "Неверный индекс: "
            << e.what() << "\n";
    }

    cout << "\n=== Сравнение со std::deque (мкс) ===\n";
    cout << "N\tОперации\t\tМой Deque\tstd::deque\n";

    for (int n : {10000, 100000, 500000}) {

        long long myBack = measureTime([&]() {
            Deque<int> q;

            for (int i = 0; i < n; i++)
                q.push_back(i);

            for (int i = 0; i < n; i++)
                q.pop_back();
            });

        long long stlBack = measureTime([&]() {
            std::deque<int> q;

            for (int i = 0; i < n; i++)
                q.push_back(i);

            for (int i = 0; i < n; i++)
                q.pop_back();
            });

        long long myFront = measureTime([&]() {
            Deque<int> q;

            for (int i = 0; i < n; i++)
                q.push_front(i);

            for (int i = 0; i < n; i++)
                q.pop_front();
            });

        long long stlFront = measureTime([&]() {
            std::deque<int> q;

            for (int i = 0; i < n; i++)
                q.push_front(i);

            for (int i = 0; i < n; i++)
                q.pop_front();
            });

        cout << n << "\tback + pop\t\t"
            << myBack << "\t\t" << stlBack << "\n";

        cout << n << "\tfront + pop\t\t"
            << myFront << "\t\t" << stlFront << "\n";
    }

    return 0;
}
