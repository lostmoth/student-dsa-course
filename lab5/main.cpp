#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <clocale>

using namespace std;

// СТЕК НА ДИНАМИЧЕСКОМ МАССИВЕ
template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        try {
            for (int i = 0; i <= topIndex; i++)
                newData[i] = data[i];
        }
        catch (...) {
            delete[] newData;
            throw;
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    explicit Stack(int initialCapacity = 4)
        : data(nullptr), topIndex(-1), capacity(initialCapacity) {
        if (capacity <= 0)
            throw invalid_argument("Capacity must be positive");

        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        if (size() == capacity)
            resize();

        data[topIndex + 1] = value;
        topIndex++;
    }

    T pop() {
        if (isEmpty())
            throw underflow_error("Stack is empty");

        T value = data[topIndex];
        topIndex--;
        return value;
    }

    T top() const {
        if (isEmpty())
            throw underflow_error("Stack is empty");

        return data[topIndex];
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }
};

bool isPalindrome(const string& str) {
    Stack<char> stack;

    for (char ch : str)
        stack.push(ch);

    for (char ch : str) {
        if (ch != stack.pop())
            return false;
    }

    return true;
}

// ЦИКЛИЧЕСКАЯ ОЧЕРЕДЬ ФИКСИРОВАННОГО РАЗМЕРА
template <typename T>
class Queue {
private:
    T* data;
    int frontIndex;
    int rearIndex;
    int count;
    int capacity;

public:
    explicit Queue(int fixedCapacity)
        : data(nullptr), frontIndex(0), rearIndex(0),
        count(0), capacity(fixedCapacity) {
        if (capacity <= 0)
            throw invalid_argument("Capacity must be positive");

        data = new T[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& value) {
        if (isFull())
            throw overflow_error("Queue is full");

        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty())
            throw underflow_error("Queue is empty");

        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty())
            throw underflow_error("Queue is empty");

        return data[frontIndex];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    int size() const {
        return count;
    }

    void print() const {
        cout << "[";

        for (int i = 0; i < count; i++) {
            if (i > 0)
                cout << ", ";

            cout << data[(frontIndex + i) % capacity];
        }

        cout << "]\n";
    }
};

// ДЕК НА ДВУСВЯЗНОМ СПИСКЕ
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value)
            : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (head != nullptr) {
            Node* old = head;
            head = head->next;
            delete old;
        }
    }

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head;

        if (head != nullptr)
            head->prev = node;
        else
            tail = node;

        head = node;
        count++;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail;

        if (tail != nullptr)
            tail->next = node;
        else
            head = node;

        tail = node;
        count++;
    }

    T pop_front() {
        if (isEmpty())
            throw underflow_error("Deque is empty");

        Node* old = head;
        T value = old->data;
        head = head->next;

        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete old;
        count--;
        return value;
    }

    T pop_back() {
        if (isEmpty())
            throw underflow_error("Deque is empty");

        Node* old = tail;
        T value = old->data;
        tail = tail->prev;

        if (tail != nullptr)
            tail->next = nullptr;
        else
            head = nullptr;

        delete old;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty())
            throw underflow_error("Deque is empty");

        return head->data;
    }

    T back() const {
        if (isEmpty())
            throw underflow_error("Deque is empty");

        return tail->data;
    }

    // Переносим первые k узлов в конец без выделения памяти
    void rotateLeft(int k) {
        if (k < 0)
            throw invalid_argument("Rotation must be non-negative");

        if (count < 2)
            return;

        k %= count;

        for (int i = 0; i < k; i++) {
            Node* first = head;
            head = head->next;
            head->prev = nullptr;

            tail->next = first;
            first->prev = tail;
            first->next = nullptr;
            tail = first;
        }
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    void print() const {
        cout << "[";

        for (Node* current = head; current != nullptr;
            current = current->next) {
            if (current != head)
                cout << ", ";

            cout << current->data;
        }

        cout << "]\n";
    }
};

// ПРИОРИТЕТНАЯ ОЧЕРЕДЬ НА ДВОИЧНОЙ КУЧЕ
template <typename T>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;
    bool minHeap;

    bool higherPriority(const T& a, const T& b) const {
        if (minHeap)
            return a < b;

        return a > b;
    }

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];

        try {
            for (int i = 0; i < count; i++)
                newHeap[i] = heap[i];
        }
        catch (...) {
            delete[] newHeap;
            throw;
        }

        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    // Поднимаем элемент, пока он важнее родителя
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (!higherPriority(heap[index], heap[parent]))
                break;

            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    // Опускаем элемент к ребёнку с большим приоритетом
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int best = index;

            if (left < count && higherPriority(heap[left], heap[best]))
                best = left;

            if (right < count && higherPriority(heap[right], heap[best]))
                best = right;

            if (best == index)
                break;

            swap(heap[index], heap[best]);
            index = best;
        }
    }

public:
    explicit PriorityQueue(bool useMinHeap = false, int initialCapacity = 4)
        : heap(nullptr), count(0), capacity(initialCapacity),
        minHeap(useMinHeap) {
        if (capacity <= 0)
            throw invalid_argument("Capacity must be positive");

        heap = new T[capacity];
    }

    ~PriorityQueue() {
        delete[] heap;
    }

    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    void push(const T& value) {
        if (count == capacity)
            resize();

        heap[count] = value;
        count++;
        heapifyUp(count - 1);
    }

    T pop() {
        if (isEmpty())
            throw underflow_error("PriorityQueue is empty");

        T value = heap[0];

        if (count > 1)
            heap[0] = heap[count - 1];

        count--;

        if (!isEmpty())
            heapifyDown(0);

        return value;
    }

    T top() const {
        if (isEmpty())
            throw underflow_error("PriorityQueue is empty");

        return heap[0];
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};

// МЕДИАНА ПОТОКА: НИЖНЯЯ И ВЕРХНЯЯ ПОЛОВИНЫ ЧИСЕЛ
class MedianFinder {
private:
    PriorityQueue<int> lower;
    PriorityQueue<int> upper;

public:
    MedianFinder() : lower(false), upper(true) {}

    void add(int value) {
        if (lower.isEmpty() || value <= lower.top())
            lower.push(value);
        else
            upper.push(value);

        // В нижней половине столько же элементов или на один больше
        if (lower.size() > upper.size() + 1)
            upper.push(lower.pop());
        else if (upper.size() > lower.size())
            lower.push(upper.pop());
    }

    double getMedian() const {
        if (lower.isEmpty())
            throw underflow_error("No numbers in stream");

        if (lower.size() == upper.size())
            return (static_cast<double>(lower.top()) + upper.top()) / 2.0;

        return lower.top();
    }
};

int main() {
    setlocale(LC_ALL, "");
    cout << boolalpha;

    cout << "========== СТЕК ==========\n";

    Stack<int> stack(2);
    stack.push(10);
    stack.push(20);
    stack.push(30); // Проверка расширения массива

    cout << "Размер: " << stack.size() << "\n";
    cout << "Вершина без удаления: " << stack.top() << "\n";
    cout << "Извлечение: ";

    while (!stack.isEmpty())
        cout << stack.pop() << " ";

    cout << "\nСтек пуст: " << stack.isEmpty() << "\n";

    cout << "\n========== ПАЛИНДРОМ ЧЕРЕЗ СТЕК ==========\n";

    for (const string& word : {string("radar"), string("abba"),
        string("hello"), string(""), string("a")}) {
        cout << "\"" << word << "\": " << isPalindrome(word) << "\n";
    }

    cout << "\n========== ФИКСИРОВАННАЯ ОЧЕРЕДЬ ==========\n";

    Queue<int> queue(5);

    for (int value : {10, 20, 30, 40, 50})
        queue.enqueue(value);

    cout << "Заполненная очередь: ";
    queue.print();
    cout << "Размер: " << queue.size()
        << ", заполнена: " << queue.isFull() << "\n";
    cout << "Первый без удаления: " << queue.front() << "\n";

    try {
        queue.enqueue(60);
    }
    catch (const overflow_error& e) {
        cout << "Переполнение: " << e.what() << "\n";
    }

    cout << "Извлекли: " << queue.dequeue();
    cout << " " << queue.dequeue() << "\n";

    queue.enqueue(60);
    queue.enqueue(70);

    cout << "После перехода через конец массива: ";
    queue.print();
    cout << "Извлечение: ";

    while (!queue.isEmpty())
        cout << queue.dequeue() << " ";

    queue.enqueue(99);
    cout << "\nПовторное использование: " << queue.dequeue() << "\n";
    cout << "Очередь пуста: " << queue.isEmpty() << "\n";

    cout << "\n========== ДЕК И rotateLeft ==========\n";

    Deque<int> deque;
    deque.push_front(10);

    for (int value : {20, 30, 40, 50})
        deque.push_back(value);

    cout << "Исходный дек: ";
    deque.print();
    cout << "Начало: " << deque.front() << ", конец: " << deque.back()
        << ", размер: " << deque.size() << "\n";

    deque.rotateLeft(2);
    cout << "После rotateLeft(2): ";
    deque.print();

    deque.rotateLeft(7);
    cout << "Затем rotateLeft(7), то есть ещё на 2: ";
    deque.print();

    deque.rotateLeft(0);
    deque.rotateLeft(deque.size());
    cout << "Сдвиги на 0 и на размер ничего не изменили: ";
    deque.print();

    cout << "Удалили с начала: " << deque.pop_front() << "\n";
    cout << "Удалили с конца: " << deque.pop_back() << "\n";
    cout << "Осталось: ";
    deque.print();

    Deque<int> single;
    single.rotateLeft(3);
    single.push_back(42);
    single.rotateLeft(10);
    cout << "Дек с одним элементом после сдвига: ";
    single.print();
    single.pop_back();
    cout << "После удаления единственного: " << single.isEmpty() << "\n";

    Deque<string> words;
    words.push_back("one");
    words.push_back("two");
    words.push_front("zero");
    words.rotateLeft(1);
    cout << "Дек строк после сдвига: ";
    words.print();

    cout << "\n========== ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ==========\n";

    PriorityQueue<int> maxQueue;
    PriorityQueue<int> minQueue(true);

    for (int value : {30, 10, 50, 20, 40}) {
        maxQueue.push(value);
        minQueue.push(value);
    }

    cout << "Размер max-heap: " << maxQueue.size()
        << ", вершина: " << maxQueue.top() << "\n";
    cout << "Размер min-heap: " << minQueue.size()
        << ", вершина: " << minQueue.top() << "\n";
    cout << "Извлечение из max-heap: ";

    while (!maxQueue.isEmpty())
        cout << maxQueue.pop() << " ";

    cout << "\nИзвлечение из min-heap: ";

    while (!minQueue.isEmpty())
        cout << minQueue.pop() << " ";

    cout << "\n\n========== МЕДИАНА ПОТОКА ==========\n";

    MedianFinder median;
    cout << "Число\tМедиана\n";

    for (int value : {5, 15, 1, 3, 8, -2, 8}) {
        median.add(value);
        cout << value << "\t" << median.getMedian() << "\n";
    }

    cout << "\n========== ПРОВЕРКА ОШИБОК ==========\n";

    try {
        stack.pop();
    }
    catch (const underflow_error& e) {
        cout << "pop пустого стека: " << e.what() << "\n";
    }

    try {
        stack.top();
    }
    catch (const underflow_error& e) {
        cout << "top пустого стека: " << e.what() << "\n";
    }

    try {
        queue.dequeue();
    }
    catch (const underflow_error& e) {
        cout << "dequeue пустой очереди: " << e.what() << "\n";
    }

    try {
        queue.front();
    }
    catch (const underflow_error& e) {
        cout << "front пустой очереди: " << e.what() << "\n";
    }

    try {
        single.pop_front();
    }
    catch (const underflow_error& e) {
        cout << "pop_front пустого дека: " << e.what() << "\n";
    }

    try {
        single.pop_back();
    }
    catch (const underflow_error& e) {
        cout << "pop_back пустого дека: " << e.what() << "\n";
    }

    try {
        single.front();
    }
    catch (const underflow_error& e) {
        cout << "front пустого дека: " << e.what() << "\n";
    }

    try {
        single.back();
    }
    catch (const underflow_error& e) {
        cout << "back пустого дека: " << e.what() << "\n";
    }

    try {
        deque.rotateLeft(-1);
    }
    catch (const invalid_argument& e) {
        cout << "Отрицательный сдвиг: " << e.what() << "\n";
    }

    try {
        maxQueue.pop();
    }
    catch (const underflow_error& e) {
        cout << "pop пустой кучи: " << e.what() << "\n";
    }

    try {
        maxQueue.top();
    }
    catch (const underflow_error& e) {
        cout << "top пустой кучи: " << e.what() << "\n";
    }

    try {
        MedianFinder emptyStream;
        emptyStream.getMedian();
    }
    catch (const underflow_error& e) {
        cout << "Медиана пустого потока: " << e.what() << "\n";
    }

    try {
        Queue<int> invalidQueue(0);
    }
    catch (const invalid_argument& e) {
        cout << "Нулевая вместимость: " << e.what() << "\n";
    }

    cout << "\n========== КОНЕЦ ПРОГРАММЫ ==========\n";

    return 0;
}
