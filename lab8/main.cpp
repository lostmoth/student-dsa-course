#include <algorithm>
#include <array>
#include <chrono>
#include <clocale>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

ptrdiff_t binarySearch(const vector<int>& arr, int target) {
    size_t left = 0;
    size_t right = arr.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] == target)
            return static_cast<ptrdiff_t>(mid);
        if (arr[mid] < target)
            left = mid + 1;
        else
            right = mid;
    }

    return -1;
}

ptrdiff_t binarySearchRecursive(const vector<int>& arr, int target, size_t left, size_t right) {
    if (left == right)
        return -1;

    size_t mid = left + (right - left) / 2;
    if (arr[mid] == target)
        return static_cast<ptrdiff_t>(mid);
    if (arr[mid] < target)
        return binarySearchRecursive(arr, target, mid + 1, right);
    return binarySearchRecursive(arr, target, left, mid);
}

// Первая позиция с элементом >= target или > target
size_t findBound(const vector<int>& arr, int target, bool upper) {
    size_t left = 0;
    size_t right = arr.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < target || (upper && arr[mid] == target))
            left = mid + 1;
        else
            right = mid;
    }

    return left;
}

pair<ptrdiff_t, ptrdiff_t> findBounds(const vector<int>& arr, int target) {
    size_t first = findBound(arr, target, false);
    if (first == arr.size() || arr[first] != target)
        return {-1, -1};

    size_t afterLast = findBound(arr, target, true);
    return {static_cast<ptrdiff_t>(first), static_cast<ptrdiff_t>(afterLast - 1)};
}

class HashTable {
private:
    struct Node {
        string key;
        int value;
        Node* next;
    };

    vector<Node*> buckets;
    size_t count = 0;

    size_t hash(const string& key) const {
        size_t code = 0;
        for (char ch : key)
            code = code * 31 + static_cast<unsigned char>(ch);
        return code % buckets.size();
    }

public:
    explicit HashTable(size_t bucketCount) : buckets(bucketCount, nullptr) {
        if (bucketCount == 0)
            throw invalid_argument("Bucket count must be positive");
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    ~HashTable() {
        for (Node* current : buckets) {
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    void insert(const string& key, int value) {
        size_t index = hash(key);
        Node* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        // Новый узел становится головой цепочки
        buckets[index] = new Node{key, value, buckets[index]};
        count++;
    }

    bool find(const string& key, int& result) const {
        const Node* current = buckets[hash(key)];
        while (current != nullptr) {
            if (current->key == key) {
                result = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool remove(const string& key) {
        size_t index = hash(key);
        Node* current = buckets[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                // Перенастраиваем связь до освобождения узла
                if (prev == nullptr)
                    buckets[index] = current->next;
                else
                    prev->next = current->next;

                delete current;
                count--;
                return true;
            }

            prev = current;
            current = current->next;
        }
        return false;
    }

    size_t size() const {
        return count;
    }

    void print() const {
        for (size_t i = 0; i < buckets.size(); i++) {
            cout << i << ": ";
            const Node* current = buckets[i];
            while (current != nullptr) {
                cout << '(' << current->key << ", " << current->value << ") -> ";
                current = current->next;
            }
            cout << "nullptr\n";
        }
    }
};

void printBounds(const vector<int>& arr, int target) {
    auto [first, last] = findBounds(arr, target);
    cout << "Ищем " << target << ": [" << first << ", " << last << ']';
    if (first == -1)
        cout << " (элемент не найден)";
    cout << '\n';
}

void showBinarySearch() {
    cout << "========== ДВОИЧНЫЙ ПОИСК ==========\n";
    const vector<int> arr{1, 3, 3, 3, 7, 9};
    cout << "Массив: [1, 3, 3, 3, 7, 9]\n";
    cout << "Итеративный поиск 7: " << binarySearch(arr, 7) << '\n';
    cout << "Рекурсивный поиск 7: " << binarySearchRecursive(arr, 7, 0, arr.size()) << '\n';

    cout << "Границы вхождений включительно (индексы с 0):\n";
    for (int target : {3, 1, 9, 4, 0, 10})
        printBounds(arr, target);
    cout << "Пустой массив: ";
    printBounds({}, 3);
    cout << "Один элемент [5]: ";
    printBounds({5}, 5);
    cout << "Все элементы равны [2, 2, 2]: ";
    printBounds({2, 2, 2}, 2);
}

void showHashTable() {
    cout << "\n========== ХЕШ-ТАБЛИЦА ==========\n";
    HashTable table(5);
    int value = 0;

    cout << "Поиск в пустой таблице: " << table.find("a", value) << '\n';
    table.insert("a", 10);
    table.insert("f", 20);
    table.insert("k", 30);
    cout << "Коллизия: ключи a, f, k попали в бакет 2.\n";
    table.print();

    if (table.find("f", value))
        cout << "Ключ f: " << value << '\n';
    table.insert("a", 99);
    if (table.find("a", value))
        cout << "Переопределили a: " << value << ", размер: " << table.size() << '\n';

    cout << "Удалили f из середины цепочки: " << table.remove("f") << '\n';
    table.print();
    cout << "Поиск удалённого f: " << table.find("f", value) << '\n';
    cout << "Повторное удаление f: " << table.remove("f") << '\n';
    cout << "Удалили голову k: " << table.remove("k") << '\n';
    cout << "Удалили последний узел a: " << table.remove("a") << '\n';
    cout << "Размер после удаления: " << table.size() << '\n';
}

struct SearchResult {
    long long microseconds;
    int found;
};

template <typename Key, typename Search>
SearchResult measureSearch(const vector<Key>& queries, Search search) {
    int found = 0;
    const auto start = chrono::steady_clock::now();
    for (const Key& key : queries) {
        if (search(key))
            found++;
    }
    const auto finish = chrono::steady_clock::now();
    return {chrono::duration_cast<chrono::microseconds>(finish - start).count(), found};
}

void compareSearch(int n) {
    const int queryCount = 50000;
    const int repeats = 5;
    vector<int> arr;
    arr.reserve(n);
    HashTable table(n);

    // Чётные ID есть в обеих структурах, нечётных нет
    for (int i = 0; i < n; i++) {
        int id = 2 * i;
        arr.push_back(id);
        table.insert("key_" + to_string(id), id);
    }

    mt19937 generator(42);
    uniform_int_distribution<int> pick(0, n - 1);
    vector<int> numbers;
    numbers.reserve(queryCount);
    for (int i = 0; i < queryCount; i++)
        numbers.push_back(2 * pick(generator) + i % 2);
    shuffle(numbers.begin(), numbers.end(), generator);

    vector<string> keys;
    keys.reserve(queryCount);
    for (int id : numbers)
        keys.push_back("key_" + to_string(id));

    auto searchArray = [&arr](int id) {
        return binarySearch(arr, id) != -1;
    };
    auto searchTable = [&table](const string& key) {
        int value = 0;
        return table.find(key, value);
    };

    array<long long, repeats> binaryTimes{};
    array<long long, repeats> hashTimes{};
    int found = 0;
    for (int i = 0; i < repeats; i++) {
        SearchResult binary{};
        SearchResult hashed{};
        // Чередуем порядок замеров двух способов
        if (i % 2 == 0) {
            binary = measureSearch(numbers, searchArray);
            hashed = measureSearch(keys, searchTable);
        }
        else {
            hashed = measureSearch(keys, searchTable);
            binary = measureSearch(numbers, searchArray);
        }

        if (binary.found != queryCount / 2 || hashed.found != binary.found)
            throw logic_error("Search results do not match");

        binaryTimes[i] = binary.microseconds;
        hashTimes[i] = hashed.microseconds;
        found = binary.found;
    }

    sort(binaryTimes.begin(), binaryTimes.end());
    sort(hashTimes.begin(), hashTimes.end());
    cout << left << setw(12) << n << setw(17) << binaryTimes[repeats / 2]
        << setw(17) << hashTimes[repeats / 2] << found << '\n';
}

int main() {
    setlocale(LC_ALL, "");
    cout << boolalpha;

    showBinarySearch();
    showHashTable();

    cout << "\n========== СРАВНЕНИЕ ВРЕМЕНИ ПОИСКА ==========\n";
    cout << "50 000 запросов, половина успешных. Медиана пяти замеров, мкс.\n";
    cout << "Binary: массив int. Hash: строковые ключи, число бакетов равно N.\n\n";
    cout << left << setw(12) << "N" << setw(17) << "Binary" << setw(17) << "Hash" << "Found\n";

    for (int n : {1000, 10000, 100000})
        compareSearch(n);

    return 0;
}
