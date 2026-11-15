#include <algorithm>
#include <clocale>
#include <iostream>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    int data;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(int value) : data(value) {}
};

void collectInOrder(const Node* node, vector<int>& values) {
    if (node == nullptr)
        return;

    collectInOrder(node->left, values);
    values.push_back(node->data);
    collectInOrder(node->right, values);
}

void destroyTree(Node* node) {
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

void printSideways(const Node* node, size_t depth, const char* branch) {
    if (node == nullptr)
        return;

    printSideways(node->right, depth + 1, "/-- ");
    cout << string(depth * 4, ' ') << branch << node->data << '\n';
    printSideways(node->left, depth + 1, "\\-- ");
}

class BinaryTree {
private:
    Node* root = nullptr;

    static void collectPreOrder(const Node* node, vector<int>& values) {
        if (node == nullptr)
            return;

        values.push_back(node->data);
        collectPreOrder(node->left, values);
        collectPreOrder(node->right, values);
    }

    static void collectPostOrder(const Node* node, vector<int>& values) {
        if (node == nullptr)
            return;

        collectPostOrder(node->left, values);
        collectPostOrder(node->right, values);
        values.push_back(node->data);
    }

    static void updateMin(const Node* node, int& result) {
        if (node == nullptr)
            return;

        result = min(result, node->data);
        updateMin(node->left, result);
        updateMin(node->right, result);
    }

public:
    BinaryTree() = default;
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    ~BinaryTree() {
        destroyTree(root);
    }

    void buildSample() {
        destroyTree(root);
        root = nullptr;

        // В обычном дереве значения не обязаны быть упорядочены
        root = new Node(10);
        root->left = new Node(20);
        root->right = new Node(5);
        root->left->left = new Node(7);
        root->left->right = new Node(3);
        root->right->left = new Node(8);
    }

    vector<int> preOrder() const {
        vector<int> values;
        collectPreOrder(root, values);
        return values;
    }

    vector<int> inOrder() const {
        vector<int> values;
        collectInOrder(root, values);
        return values;
    }

    vector<int> postOrder() const {
        vector<int> values;
        collectPostOrder(root, values);
        return values;
    }

    vector<int> levelOrder() const {
        vector<int> values;
        if (root == nullptr)
            return values;

        queue<const Node*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const Node* current = pending.front();
            pending.pop();
            values.push_back(current->data);

            if (current->left != nullptr)
                pending.push(current->left);
            if (current->right != nullptr)
                pending.push(current->right);
        }

        return values;
    }

    optional<int> minValue() const {
        if (root == nullptr)
            return nullopt;

        int result = root->data;
        updateMin(root, result);
        return result;
    }

    void printTree() const {
        if (root == nullptr)
            cout << "(пусто)\n";
        else
            printSideways(root, 0, "");
    }
};

class BST {
private:
    Node* root = nullptr;

    static Node* insert(Node* node, int key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->data)
            node->left = insert(node->left, key);
        else if (key > node->data)
            node->right = insert(node->right, key);

        return node;
    }

    static const Node* leftmost(const Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    static Node* remove(Node* node, int key, bool& removed) {
        if (node == nullptr)
            return nullptr;

        if (key < node->data)
            node->left = remove(node->left, key, removed);
        else if (key > node->data)
            node->right = remove(node->right, key, removed);
        else {
            removed = true;

            // Для листа вместо узла возвращается nullptr
            if (node->left == nullptr) {
                Node* child = node->right;
                delete node;
                return child;
            }
            if (node->right == nullptr) {
                Node* child = node->left;
                delete node;
                return child;
            }

            // Копируем ключ преемника и удаляем его старый узел
            int nextKey = leftmost(node->right)->data;
            node->data = nextKey;
            node->right = remove(node->right, nextKey, removed);
        }

        return node;
    }

    static int height(const Node* node) {
        if (node == nullptr)
            return 0;
        return 1 + max(height(node->left), height(node->right));
    }

public:
    BST() = default;
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    ~BST() {
        destroyTree(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    bool search(int key) const {
        const Node* current = root;
        while (current != nullptr) {
            if (key == current->data)
                return true;

            if (key < current->data)
                current = current->left;
            else
                current = current->right;
        }
        return false;
    }

    optional<int> successor(int key) const {
        const Node* current = root;
        const Node* candidate = nullptr;

        while (current != nullptr && current->data != key) {
            if (key < current->data) {
                candidate = current;
                current = current->left;
            }
            else
                current = current->right;
        }

        if (current == nullptr)
            return nullopt;
        if (current->right != nullptr)
            return leftmost(current->right)->data;
        if (candidate != nullptr)
            return candidate->data;
        return nullopt;
    }

    bool remove(int key) {
        bool removed = false;
        root = remove(root, key, removed);
        return removed;
    }

    vector<int> inOrder() const {
        vector<int> values;
        collectInOrder(root, values);
        return values;
    }

    optional<int> minValue() const {
        if (root == nullptr)
            return nullopt;
        return leftmost(root)->data;
    }

    optional<int> maxValue() const {
        if (root == nullptr)
            return nullopt;

        const Node* current = root;
        while (current->right != nullptr)
            current = current->right;
        return current->data;
    }

    int height() const {
        return height(root);
    }

    void printTree() const {
        if (root == nullptr)
            cout << "(пусто)\n";
        else
            printSideways(root, 0, "");
    }
};

void printValues(const vector<int>& values) {
    cout << '[';
    for (size_t i = 0; i < values.size(); i++) {
        if (i != 0)
            cout << ", ";
        cout << values[i];
    }
    cout << "]\n";
}

void printValue(optional<int> value) {
    if (value.has_value())
        cout << *value << '\n';
    else
        cout << "нет\n";
}

void showBinaryTree() {
    cout << "========== БИНАРНОЕ ДЕРЕВО ==========\n";
    BinaryTree tree;
    tree.buildSample();
    tree.printTree();

    cout << "Прямой обход:      ";
    printValues(tree.preOrder());
    cout << "Симметричный:      ";
    printValues(tree.inOrder());
    cout << "Обратный обход:    ";
    printValues(tree.postOrder());
    cout << "Уровневый обход:   ";
    printValues(tree.levelOrder());
    cout << "Минимум: ";
    printValue(tree.minValue());
}

void showBST() {
    cout << "\n========== БИНАРНОЕ ДЕРЕВО ПОИСКА ==========\n";
    BST tree;
    for (int key : {50, 30, 70, 60, 80, 65, 65})
        tree.insert(key);

    cout << "Симметричный обход (65 вставляли дважды): ";
    printValues(tree.inOrder());
    cout << "Поиск 65: " << tree.search(65) << '\n';
    cout << "Поиск 42: " << tree.search(42) << '\n';
    cout << "Преемник 50 (минимум справа): ";
    printValue(tree.successor(50));
    cout << "Преемник 65 (предок): ";
    printValue(tree.successor(65));
    cout << "Преемник 80 (максимум): ";
    printValue(tree.successor(80));
    cout << "Преемник 42 (ключ отсутствует): ";
    printValue(tree.successor(42));

    tree.remove(50);
    cout << "Удалили 50 (корень с двумя потомками): ";
    printValues(tree.inOrder());
    tree.remove(80);
    cout << "Удалили 80 (лист): ";
    printValues(tree.inOrder());
    tree.remove(70);
    cout << "Удалили 70 (один потомок): ";
    printValues(tree.inOrder());
    cout << "Удаление отсутствующего 42: " << tree.remove(42) << '\n';
}

void showRandomBST() {
    cout << "\n========== ПРИМЕНЕНИЕ BST ==========\n";
    const int count = 20;
    vector<int> numbers(99);
    iota(numbers.begin(), numbers.end(), 1);
    mt19937 generator(42);
    shuffle(numbers.begin(), numbers.end(), generator);
    numbers.resize(count);

    BST tree;
    for (int key : numbers)
        tree.insert(key);

    cout << "Порядок вставки: ";
    printValues(numbers);
    cout << "Отсортированная последовательность: ";
    printValues(tree.inOrder());
    cout << "Минимум: ";
    printValue(tree.minValue());
    cout << "Максимум: ";
    printValue(tree.maxValue());
    cout << "Высота (в узлах): " << tree.height() << '\n';
    cout << "\nДерево до удаления (правые ветви сверху):\n";
    tree.printTree();

    cout << "\nВведите целое число для удаления: ";
    string line;
    if (!getline(cin, line)) {
        cout << "\nВвод завершён.\n";
        return;
    }

    istringstream input(line);
    int key;
    char extra;
    if (!(input >> key) || (input >> extra)) {
        cout << "Ожидалось одно целое число.\n";
        return;
    }

    if (tree.remove(key))
        cout << "Удалили " << key << ".\n";
    else
        cout << "Ключ " << key << " не найден.\n";

    cout << "Дерево после операции:\n";
    tree.printTree();
    cout << "Симметричный обход: ";
    printValues(tree.inOrder());
}

int main() {
    setlocale(LC_ALL, "");
    cout << boolalpha;

    showBinaryTree();
    showBST();
    showRandomBST();

    return 0;
}
