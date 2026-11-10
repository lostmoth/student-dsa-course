#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <clocale>

using namespace std;

// ФАКТОРИАЛ
long long factorialRecursive(int n) {
    if (n < 0 || n > 20)
        throw out_of_range("Factorial requires 0 <= n <= 20");

    if (n == 0)
        return 1;

    return n * factorialRecursive(n - 1);
}

long long factorialIterative(int n) {
    if (n < 0 || n > 20)
        throw out_of_range("Factorial requires 0 <= n <= 20");

    long long result = 1;

    for (int i = 2; i <= n; i++)
        result *= i;

    return result;
}

// ЧИСЛА ФИБОНАЧЧИ
long long fibonacciRecursive(int n) {
    // Ограничиваем наивную рекурсию из-за большого числа вызовов
    if (n < 0 || n > 35)
        throw out_of_range("Recursive Fibonacci requires 0 <= n <= 35");

    if (n <= 1)
        return n;

    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

long long fibonacciIterative(int n) {
    if (n < 0 || n > 92)
        throw out_of_range("Iterative Fibonacci requires 0 <= n <= 92");

    if (n <= 1)
        return n;

    long long a = 0;
    long long b = 1;

    for (int i = 2; i <= n; i++) {
        long long next = a + b;
        a = b;
        b = next;
    }

    return b;
}

void checkArray(const int arr[], int n) {
    if (n < 0)
        throw invalid_argument("Array size must be non-negative");

    if (n > 0 && arr == nullptr)
        throw invalid_argument("Array is null");
}

// СУММА ЭЛЕМЕНТОВ МАССИВА
long long sumRecursive(const int arr[], int n) {
    checkArray(arr, n);

    if (n == 0)
        return 0;

    return static_cast<long long>(arr[n - 1]) + sumRecursive(arr, n - 1);
}

long long sumIterative(const int arr[], int n) {
    checkArray(arr, n);
    long long result = 0;

    for (int i = 0; i < n; i++)
        result += arr[i];

    return result;
}

// ХАНОЙСКИЕ БАШНИ
long long hanoi(int n, char from, char to, char aux) {
    if (n < 0 || n > 15)
        throw out_of_range("Hanoi demonstration requires 0 <= n <= 15");

    if (from == to || from == aux || to == aux)
        throw invalid_argument("Rod names must be different");

    if (n == 0)
        return 0;

    long long moves = hanoi(n - 1, from, aux, to);

    cout << "Диск " << n << ": " << from << " -> " << to << "\n";
    moves++;

    moves += hanoi(n - 1, aux, to, from);
    return moves;
}

// СЧЁТЧИКИ ОПЕРАЦИЙ СОРТИРОВОК
struct SortStats {
    long long comparisons = 0;
    long long swaps = 0;
    long long shifts = 0;
};

SortStats bubbleSort(int arr[], int n) {
    checkArray(arr, n);
    SortStats stats;

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            stats.comparisons++;

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return stats;
}

SortStats selectionSort(int arr[], int n) {
    checkArray(arr, n);
    SortStats stats;

    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            stats.comparisons++;

            if (arr[j] < arr[minIndex])
                minIndex = j;
        }

        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
            stats.swaps++;
        }
    }

    return stats;
}

// Дополнительная сортировка для общей шкалы оценивания
SortStats insertionSort(int arr[], int n) {
    checkArray(arr, n);
    SortStats stats;

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            stats.comparisons++;

            if (arr[j] <= key)
                break;

            arr[j + 1] = arr[j];
            stats.shifts++;
            j--;
        }

        arr[j + 1] = key;
    }

    return stats;
}

void printArray(const int arr[], int n) {
    checkArray(arr, n);
    cout << "[";

    for (int i = 0; i < n; i++) {
        if (i > 0)
            cout << ", ";

        cout << arr[i];
    }

    cout << "]\n";
}

void printStats(const char* name, const SortStats& stats) {
    cout << left << setw(16) << name
        << setw(14) << stats.comparisons
        << setw(10) << stats.swaps
        << stats.shifts << "\n";
}

void compareSorts(const char* title, const int arr[], int n) {
    checkArray(arr, n);

    // Каждая сортировка получает отдельную копию исходных данных
    vector<int> bubble(n);
    vector<int> selection(n);
    vector<int> insertion(n);

    for (int i = 0; i < n; i++) {
        bubble[i] = arr[i];
        selection[i] = arr[i];
        insertion[i] = arr[i];
    }

    SortStats bubbleStats = bubbleSort(bubble.data(), n);
    SortStats selectionStats = selectionSort(selection.data(), n);
    SortStats insertionStats = insertionSort(insertion.data(), n);

    cout << "\n" << title << "\n";
    cout << "Исходный: ";
    printArray(arr, n);
    cout << "Пузырьковая: ";
    printArray(bubble.data(), n);
    cout << "Выбором:     ";
    printArray(selection.data(), n);
    cout << "Вставками:   ";
    printArray(insertion.data(), n);

    cout << "Алгоритм        Сравнения     Обмены    Сдвиги\n";
    printStats("Bubble", bubbleStats);
    printStats("Selection", selectionStats);
    printStats("Insertion", insertionStats);
}

int main() {
    setlocale(LC_ALL, "");

    cout << "========== ФАКТОРИАЛ ==========\n";
    cout << "n\tРекурсивно\tИтеративно\n";

    for (int n : {0, 1, 5, 10})
        cout << n << "\t" << factorialRecursive(n)
            << "\t\t" << factorialIterative(n) << "\n";

    cout << "20! = " << factorialRecursive(20) << "\n";

    cout << "\n========== ЧИСЛА ФИБОНАЧЧИ ==========\n";
    cout << "n\tРекурсивно\tИтеративно\n";

    for (int n : {0, 1, 2, 5, 10, 20})
        cout << n << "\t" << fibonacciRecursive(n)
            << "\t\t" << fibonacciIterative(n) << "\n";

    cout << "\n========== СУММА МАССИВА ==========\n";

    int values[] = {4, -2, 7, 0, 5};
    int count = sizeof(values) / sizeof(values[0]);

    cout << "Массив: ";
    printArray(values, count);
    cout << "Рекурсивная сумма: " << sumRecursive(values, count) << "\n";
    cout << "Итеративная сумма: " << sumIterative(values, count) << "\n";
    cout << "Сумма пустого массива: " << sumRecursive(nullptr, 0) << "\n";

    cout << "\n========== ХАНОЙСКИЕ БАШНИ: 4 ДИСКА ==========\n";

    long long moves = hanoi(4, 'A', 'C', 'B');
    cout << "Количество ходов: " << moves << "\n";
    cout << "Ожидается по формуле 2^4 - 1: 15\n";
    cout << "Для нуля дисков: " << hanoi(0, 'A', 'C', 'B') << "\n";

    cout << "\n========== СРАВНЕНИЕ СОРТИРОВОК ==========\n";
    cout << "Считаются сравнения значений и реальные обмены.\n";
    cout << "У вставок сдвиги считаются отдельно, обменов нет.\n";

    int mixed[] = {4, 2, 3, 1};
    int sorted[] = {1, 2, 3, 4};
    int reversed[] = {4, 3, 2, 1};
    int repeated[] = {3, 1, 3, -2, 1, 0};
    int almostSorted[] = {1, 2, 3, 5, 4, 6};
    int single[] = {42};

    compareSorts("Смешанный массив", mixed, 4);
    compareSorts("Отсортированный массив", sorted, 4);
    compareSorts("Обратный порядок", reversed, 4);
    compareSorts("Повторы и отрицательные значения", repeated, 6);
    compareSorts("Почти отсортированный массив", almostSorted, 6);
    compareSorts("Один элемент", single, 1);
    compareSorts("Пустой массив", nullptr, 0);

    cout << "\n========== ПРОВЕРКА ОШИБОК ==========\n";

    try {
        factorialRecursive(-1);
    }
    catch (const out_of_range& e) {
        cout << "Отрицательный факториал: " << e.what() << "\n";
    }

    try {
        factorialIterative(21);
    }
    catch (const out_of_range& e) {
        cout << "Переполнение факториала предотвращено: " << e.what() << "\n";
    }

    try {
        fibonacciRecursive(36);
    }
    catch (const out_of_range& e) {
        cout << "Ограничение наивной рекурсии: " << e.what() << "\n";
    }

    try {
        fibonacciIterative(93);
    }
    catch (const out_of_range& e) {
        cout << "Переполнение Фибоначчи предотвращено: " << e.what() << "\n";
    }

    try {
        sumRecursive(nullptr, 3);
    }
    catch (const invalid_argument& e) {
        cout << "Отсутствующий массив: " << e.what() << "\n";
    }

    try {
        bubbleSort(values, -1);
    }
    catch (const invalid_argument& e) {
        cout << "Отрицательный размер: " << e.what() << "\n";
    }

    try {
        hanoi(-1, 'A', 'C', 'B');
    }
    catch (const out_of_range& e) {
        cout << "Некорректное число дисков: " << e.what() << "\n";
    }

    try {
        hanoi(1, 'A', 'A', 'B');
    }
    catch (const invalid_argument& e) {
        cout << "Совпадающие стержни: " << e.what() << "\n";
    }

    cout << "\n========== КОНЕЦ ПРОГРАММЫ ==========\n";
    return 0;
}
