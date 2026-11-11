#include <algorithm>
#include <chrono>
#include <clocale>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;

struct Item {
    int key;
    char label;

    bool operator<(const Item& other) const {
        return key < other.key;
    }
};

// Медиана первого, среднего и последнего элементов
template <typename T>
T choosePivot(const vector<T>& arr, size_t first, size_t last) {
    T a = arr[first];
    T b = arr[first + (last - first) / 2];
    T c = arr[last - 1];

    if (b < a)
        swap(a, b);
    if (c < b)
        swap(b, c);
    if (b < a)
        swap(a, b);

    return b;
}

// Устойчивая быстрая сортировка диапазона [first, last)
template <typename T>
void quickSortRange(vector<T>& arr, vector<T>& buffer, size_t first, size_t last) {
    while (last - first > 1) {
        const T pivot = choosePivot(arr, first, last);
        size_t lessCount = 0;
        size_t equalCount = 0;

        for (size_t i = first; i < last; i++) {
            if (arr[i] < pivot)
                lessCount++;
            else if (!(pivot < arr[i]))
                equalCount++;
        }

        const size_t equalBegin = first + lessCount;
        const size_t greaterBegin = equalBegin + equalCount;
        size_t lessPos = first;
        size_t equalPos = equalBegin;
        size_t greaterPos = greaterBegin;

        // В каждой группе сохраняем порядок обхода
        for (size_t i = first; i < last; i++) {
            if (arr[i] < pivot)
                buffer[lessPos++] = arr[i];
            else if (pivot < arr[i])
                buffer[greaterPos++] = arr[i];
            else
                buffer[equalPos++] = arr[i];
        }

        for (size_t i = first; i < last; i++)
            arr[i] = buffer[i];

        // Меньшая часть обрабатывается рекурсией, большая – циклом
        if (equalBegin - first < last - greaterBegin) {
            quickSortRange(arr, buffer, first, equalBegin);
            first = greaterBegin;
        }
        else {
            quickSortRange(arr, buffer, greaterBegin, last);
            last = equalBegin;
        }
    }
}

template <typename T>
void quickSort(vector<T>& arr) {
    if (arr.size() < 2)
        return;

    vector<T> buffer(arr.size());
    quickSortRange(arr, buffer, 0, arr.size());
}

// Слияние соседних отсортированных частей
template <typename T>
void mergeRanges(vector<T>& arr, vector<T>& buffer, size_t first, size_t middle, size_t last) {
    size_t left = first;
    size_t right = middle;
    size_t pos = first;

    while (left < middle && right < last) {
        if (arr[right] < arr[left])
            buffer[pos++] = arr[right++];
        else
            buffer[pos++] = arr[left++]; // При равенстве берём слева
    }

    while (left < middle)
        buffer[pos++] = arr[left++];

    while (right < last)
        buffer[pos++] = arr[right++];

    for (size_t i = first; i < last; i++)
        arr[i] = buffer[i];
}

template <typename T>
void mergeSortRange(vector<T>& arr, vector<T>& buffer, size_t first, size_t last) {
    if (last - first < 2)
        return;

    const size_t middle = first + (last - first) / 2;
    mergeSortRange(arr, buffer, first, middle);
    mergeSortRange(arr, buffer, middle, last);
    mergeRanges(arr, buffer, first, middle, last);
}

template <typename T>
void mergeSort(vector<T>& arr) {
    if (arr.size() < 2)
        return;

    vector<T> buffer(arr.size());
    mergeSortRange(arr, buffer, 0, arr.size());
}

// Просеивание вниз в пределах активной кучи
template <typename T>
void heapify(vector<T>& arr, size_t root, size_t heapSize) {
    while (root < heapSize / 2) {
        size_t child = 2 * root + 1;

        if (child + 1 < heapSize && arr[child] < arr[child + 1])
            child++;

        if (!(arr[root] < arr[child]))
            return;

        swap(arr[root], arr[child]);
        root = child;
    }
}

template <typename T>
void heapSort(vector<T>& arr) {
    // Начинаем с последнего узла, у которого есть дети
    for (size_t i = arr.size() / 2; i > 0; i--)
        heapify(arr, i - 1, arr.size());

    for (size_t end = arr.size(); end > 1; end--) {
        swap(arr[0], arr[end - 1]);
        heapify(arr, 0, end - 1);
    }
}

void printItems(const vector<Item>& arr) {
    cout << "[";

    for (size_t i = 0; i < arr.size(); i++) {
        if (i > 0)
            cout << ", ";

        cout << arr[i].key << arr[i].label;
    }

    cout << "]\n";
}

using SortFunction = void (*)(vector<int>&);

long long measureTime(const vector<int>& source, SortFunction sortFunction) {
    constexpr int repeats = 5;
    long long times[repeats];

    for (int i = 0; i < repeats; i++) {
        vector<int> copy = source;

        const auto start = chrono::steady_clock::now();
        sortFunction(copy);
        const auto finish = chrono::steady_clock::now();

        times[i] = chrono::duration_cast<chrono::microseconds>(finish - start).count();

        if (!is_sorted(copy.begin(), copy.end()))
            throw logic_error("Array is not sorted");
    }

    // Упорядочиваем только пять измерений, чтобы взять медиану
    sort(times, times + repeats);
    return times[repeats / 2];
}

void printBenchmark(const char* name, const vector<int>& source) {
    const long long quickTime = measureTime(source, quickSort<int>);
    const long long mergeTime = measureTime(source, mergeSort<int>);
    const long long heapTime = measureTime(source, heapSort<int>);

    cout << left << setw(9) << source.size() << setw(16) << name
        << right << setw(10) << quickTime << setw(10) << mergeTime
        << setw(10) << heapTime << "\n";
}

int main() {
    setlocale(LC_ALL, "");

    cout << "========== ПРОВЕРКА УСТОЙЧИВОСТИ ==========\n";
    cout << "Число – ключ, буква – метка записи.\n";

    const vector<Item> items = {
        {4, 'A'}, {2, 'X'}, {1, 'Y'}, {4, 'B'}, {5, 'Z'}, {4, 'C'}, {3, 'W'}
    };
    vector<Item> quickItems = items;
    vector<Item> mergeItems = items;
    vector<Item> heapItems = items;

    quickSort(quickItems);
    mergeSort(mergeItems);
    heapSort(heapItems);

    cout << "Исходный:   ";
    printItems(items);
    cout << "Quick Sort: ";
    printItems(quickItems);
    cout << "Merge Sort: ";
    printItems(mergeItems);
    cout << "Heap Sort:  ";
    printItems(heapItems);

    cout << "\n========== СРАВНЕНИЕ ВРЕМЕНИ ==========\n";
    cout << "Медиана пяти замеров, мкс.\n";
    cout << "Random – случайные, Nearly sorted – почти отсортированные,\n";
    cout << "Reverse – обратный порядок, Repeats – повторы ключей 0..9.\n\n";
    cout << left << setw(9) << "N" << setw(16) << "Data"
        << right << setw(10) << "Quick" << setw(10) << "Merge"
        << setw(10) << "Heap" << "\n";

    mt19937 generator(42);
    uniform_int_distribution<int> randomValue(-1000000, 1000000);
    uniform_int_distribution<int> repeatedValue(0, 9);

    for (int n : {1000, 10000, 50000}) {
        vector<int> random(n);
        vector<int> nearly(n);
        vector<int> reverse(n);
        vector<int> repeats(n);

        for (int i = 0; i < n; i++) {
            random[i] = randomValue(generator);
            nearly[i] = i;
            reverse[i] = n - i;
            repeats[i] = repeatedValue(generator);
        }

        for (int i = 0; i + 1 < n; i += 100)
            swap(nearly[i], nearly[i + 1]);

        printBenchmark("Random", random);
        printBenchmark("Nearly sorted", nearly);
        printBenchmark("Reverse", reverse);
        printBenchmark("Repeats", repeats);
    }

    cout << "\nПроверка порядка после всех замеров пройдена.\n";
    return 0;
}
