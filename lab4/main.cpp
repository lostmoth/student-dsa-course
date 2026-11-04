#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>

using namespace std;

// ОДНОСВЯЗНЫЙ СПИСОК
template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

    // Слияние двух отсортированных цепочек
    static Node* merge(Node* a, Node* b) {
        Node* result = nullptr;
        Node* last = nullptr;

        while (a != nullptr && b != nullptr) {
            Node* chosen;

            if (a->data <= b->data) {
                chosen = a;
                a = a->next;
            }
            else {
                chosen = b;
                b = b->next;
            }

            if (result == nullptr)
                result = chosen;
            else
                last->next = chosen;

            last = chosen;
        }

        if (last == nullptr)
            return a != nullptr ? a : b;

        last->next = a != nullptr ? a : b;
        return result;
    }

    // Рекурсивная сортировка цепочки узлов
    static Node* sortNodes(Node* start) {
        if (start == nullptr || start->next == nullptr)
            return start;

        Node* slow = start;
        Node* fast = start->next;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Node* second = slow->next;
        slow->next = nullptr; // Разделяем список на две части

        Node* left = sortNodes(start);
        Node* right = sortNodes(second);

        return merge(left, right);
    }

public:
    // Итератор для обхода списка
    class Iterator {
    private:
        Node* ptr;

    public:
        Iterator(Node* p) : ptr(p) {}

        T& operator*() const {
            return ptr->data;
        }

        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
    };

    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    // Запрещаем копирование, чтобы не удалить узлы дважды
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    void push_front(const T& value) {
        Node* node = new Node(value);

        node->next = head;
        head = node;

        if (tail == nullptr)
            tail = node;

        count++;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);

        if (tail == nullptr) {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }

        count++;
    }

    void pop_front() {
        if (empty())
            throw underflow_error("List is empty");

        Node* old = head;
        head = head->next;

        delete old;

        if (head == nullptr)
            tail = nullptr;

        count--;
    }

    void pop_back() {
        if (empty())
            throw underflow_error("List is empty");

        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* current = head;

            while (current->next != tail)
                current = current->next;

            delete tail;
            tail = current;
            tail->next = nullptr;
        }

        count--;
    }

    Node* find(const T& value) const {
        Node* current = head;

        while (current != nullptr) {
            if (current->data == value)
                return current;

            current = current->next;
        }

        return nullptr;
    }

    bool remove(const T& value) {
        if (empty())
            return false;

        if (head->data == value) {
            pop_front();
            return true;
        }

        Node* current = head;

        while (current->next != nullptr &&
            current->next->data != value) {
            current = current->next;
        }

        if (current->next == nullptr)
            return false;

        Node* old = current->next;
        current->next = old->next;

        if (old == tail)
            tail = current;

        delete old;
        count--;

        return true;
    }

    // Сортировка слиянием
    void mergeSort() {
        head = sortNodes(head);

        // После сортировки обновляем tail
        tail = head;

        while (tail != nullptr && tail->next != nullptr)
            tail = tail->next;
    }

    void clear() {
        while (head != nullptr) {
            Node* old = head;
            head = head->next;
            delete old;
        }

        tail = nullptr;
        count = 0;
    }

    void print() const {
        cout << "[";

        Node* current = head;

        while (current != nullptr) {
            cout << current->data;

            if (current->next != nullptr)
                cout << ", ";

            current = current->next;
        }

        cout << "]\n";
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

// ДВУСВЯЗНЫЙ СПИСОК
template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value)
            : data(value), prev(nullptr), next(nullptr) {
        }
    };

    Node* head;
    Node* tail;
    int count;

public:
    // Итератор для обхода списка
    class Iterator {
    private:
        Node* ptr;

    public:
        Iterator(Node* p) : ptr(p) {}

        T& operator*() const {
            return ptr->data;
        }

        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
    };

    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_front(const T& value) {
        Node* node = new Node(value);

        if (head == nullptr) {
            head = tail = node;
        }
        else {
            node->next = head;
            head->prev = node;
            head = node;
        }

        count++;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);

        if (tail == nullptr) {
            head = tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        count++;
    }

    void pop_front() {
        if (empty())
            throw underflow_error("List is empty");

        Node* old = head;
        head = head->next;

        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete old;
        count--;
    }

    void pop_back() {
        if (empty())
            throw underflow_error("List is empty");

        Node* old = tail;
        tail = tail->prev;

        if (tail != nullptr)
            tail->next = nullptr;
        else
            head = nullptr;

        delete old;
        count--;
    }

    Node* find(const T& value) const {
        Node* current = head;

        while (current != nullptr) {
            if (current->data == value)
                return current;

            current = current->next;
        }

        return nullptr;
    }

    // Вставка после найденного узла
    void insert_after(Node* place, const T& value) {
        if (place == nullptr) {
            push_front(value);
            return;
        }

        if (place == tail) {
            push_back(value);
            return;
        }

        Node* node = new Node(value);

        node->next = place->next;
        node->prev = place;

        place->next->prev = node;
        place->next = node;

        count++;
    }

    // Удаление указанного узла
    void erase(Node* place) {
        if (place == nullptr)
            return;

        if (place == head) {
            pop_front();
            return;
        }

        if (place == tail) {
            pop_back();
            return;
        }

        place->prev->next = place->next;
        place->next->prev = place->prev;

        delete place;
        count--;
    }

    void print() const {
        cout << "[";

        Node* current = head;

        while (current != nullptr) {
            cout << current->data;

            if (current->next != nullptr)
                cout << ", ";

            current = current->next;
        }

        cout << "]\n";
    }

    void printReverse() const {
        cout << "[";

        Node* current = tail;

        while (current != nullptr) {
            cout << current->data;

            if (current->prev != nullptr)
                cout << ", ";

            current = current->prev;
        }

        cout << "]\n";
    }

    void clear() {
        while (head != nullptr) {
            Node* old = head;
            head = head->next;
            delete old;
        }

        tail = nullptr;
        count = 0;
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

int main() {
    setlocale(LC_ALL, "");

    cout << "=== ОДНОСВЯЗНЫЙ СПИСОК ===\n";

    SinglyLinkedList<int> singly;

    singly.push_back(4);
    singly.push_back(1);
    singly.push_back(3);
    singly.push_front(5);
    singly.push_back(2);
    singly.push_back(3);

    cout << "Исходный: ";
    singly.print();

    cout << "Найдено число 3: "
        << (singly.find(3) != nullptr) << "\n";

    singly.mergeSort();

    cout << "После mergeSort: ";
    singly.print();

    cout << "Через итератор: ";
    for (int x : singly)
        cout << x << " ";
    cout << "\n";

    singly.remove(3);
    singly.pop_front();
    singly.pop_back();

    cout << "После удаления: ";
    singly.print();

    cout << "Размер: " << singly.size() << "\n";

    singly.clear();

    cout << "После clear: размер = " << singly.size()
        << ", пуст = " << singly.empty() << "\n";

    cout << "\n=== ДВУСВЯЗНЫЙ СПИСОК ===\n";

    DoublyLinkedList<int> doubly;

    doubly.push_back(10);
    doubly.push_back(20);
    doubly.push_front(5);
    doubly.push_back(30);

    cout << "Исходный: ";
    doubly.print();

    doubly.insert_after(doubly.find(20), 25);

    cout << "После вставки: ";
    doubly.print();

    cout << "Обратный порядок: ";
    doubly.printReverse();

    doubly.erase(doubly.find(25));
    doubly.pop_front();
    doubly.pop_back();

    cout << "После удаления: ";
    doubly.print();

    cout << "Через итератор: ";
    for (int x : doubly)
        cout << x << " ";

    cout << "\nРазмер: " << doubly.size() << "\n";

    doubly.clear();

    cout << "После clear: размер = " << doubly.size()
        << ", пуст = " << doubly.empty() << "\n";

    cout << "\n=== ДРУГИЕ СЛУЧАИ ===\n";

    SinglyLinkedList<string> words;
    words.push_back("Привет");
    words.push_back("мир");
    words.print();

    SinglyLinkedList<int> emptyList;
    emptyList.mergeSort();

    cout << "Сортировка пустого: ";
    emptyList.print();

    try {
        emptyList.pop_front();
    }
    catch (const underflow_error& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }

    try {
        DoublyLinkedList<int> emptyDoubly;
        emptyDoubly.pop_back();
    }
    catch (const underflow_error& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }

    cout << "\n=== СРАВНЕНИЕ pop_back (мкс) ===\n";
    cout << "N\tОдносвязный\tДвусвязный\n";

    for (int n : {2000, 5000, 10000}) {
        SinglyLinkedList<int> a;
        DoublyLinkedList<int> b;

        for (int i = 0; i < n; i++) {
            a.push_back(i);
            b.push_back(i);
        }

        auto start = chrono::steady_clock::now();

        for (int i = 0; i < n; i++)
            a.pop_back();

        auto end = chrono::steady_clock::now();

        auto t1 = chrono::duration_cast<chrono::microseconds>(
            end - start).count();

        start = chrono::steady_clock::now();

        for (int i = 0; i < n; i++)
            b.pop_back();

        end = chrono::steady_clock::now();

        auto t2 = chrono::duration_cast<chrono::microseconds>(
            end - start).count();

        cout << n << "\t" << t1 << "\t\t" << t2 << "\n";
    }

    return 0;
}
