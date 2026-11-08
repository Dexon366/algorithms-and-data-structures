#include <iostream>
#include <string>  
#include <queue>
#include <iomanip>
#include <cstdlib>
#include <Windows.h>

using namespace std;

// Узел бинарного дерева
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Класс бинарного дерева (для обходов)
class BinaryTree {
private:
    Node* root;

    void preOrder(Node* node) const {
        if (node == nullptr) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void postOrder(Node* node) const {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }

    int sumRecursive(Node* node) const {
        if (node == nullptr) return 0;
        return node->data + sumRecursive(node->left) + sumRecursive(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() { destroy(root); }

    void setRoot(Node* r) { root = r; }

    void preOrder() const {
        cout << "Прямой обход (pre-order): ";
        preOrder(root);
        cout << endl;
    }

    void inOrder() const {
        cout << "Симметричный обход (in-order): ";
        inOrder(root);
        cout << endl;
    }

    void postOrder() const {
        cout << "Обратный обход (post-order): ";
        postOrder(root);
        cout << endl;
    }

    void levelOrder() const {
        cout << "Уровневый обход (level-order): ";
        if (root == nullptr) return;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            cout << current->data << " ";
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << endl;
    }

    int getSum() const {
        return sumRecursive(root);
    }
};

// Класс бинарного дерева поиска (BST)
class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);
        if (key < node->data) {
            node->left = insert(node->left, key);
        }
        else if (key > node->data) {
            node->right = insert(node->right, key);
        }
        return node;
    }

    Node* search(Node* node, int key) const {
        if (node == nullptr || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }

    Node* findMin(Node* node) const {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr) return nullptr;
        if (key < node->data) {
            node->left = remove(node->left, key);
        }
        else if (key > node->data) {
            node->right = remove(node->right, key);
        }
        else {
            // Нашли узел для удаления
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                // Узел с двумя потомками
                Node* minNode = findMin(node->right);
                node->data = minNode->data;
                node->right = remove(node->right, minNode->data);
            }
        }
        return node;
    }

    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    int height(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    // Поиск предшественника (предыдущего по порядку)
    int findPredecessor(int key) const {
        Node* current = root;
        int predecessor = -1;
        while (current != nullptr) {
            if (key <= current->data) {
                current = current->left;
            }
            else {
                predecessor = current->data;
                current = current->right;
            }
        }
        return predecessor;
    }

    // Визуализация дерева (повернутая на 90 градусов)
    void printTree(Node* node, int space) const {
        const int SPACE_STEP = 5;
        if (node == nullptr) return;
        space += SPACE_STEP;
        printTree(node->right, space);
        cout << endl;
        for (int i = SPACE_STEP; i < space; ++i) cout << " ";
        cout << node->data << endl;
        printTree(node->left, space);
    }

public:
    BST() : root(nullptr) {}

    ~BST() { destroy(root); }

    void insert(int key) {
        root = insert(root, key);
    }

    bool search(int key) const {
        return search(root, key) != nullptr;
    }

    void remove(int key) {
        root = remove(root, key);
    }

    void inOrder() const {
        inOrder(root);
        cout << endl;
    }

    int getHeight() const {
        return height(root);
    }

    int getMin() const {
        if (root == nullptr) return -1;
        return findMin(root)->data;
    }

    int getMax() const {
        if (root == nullptr) return -1;
        Node* current = root;
        while (current->right != nullptr) current = current->right;
        return current->data;
    }

    int getPredecessor(int key) const {
        return findPredecessor(key);
    }

    void print() const {
        printTree(root, 0);
    }

    Node* getRoot() const { return root; }
};

int main() {
    // Настройка кодировки для корректного отображения русского текста в консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(static_cast<unsigned int>(time(0)));

    cout << "=== Лабораторная работа №9. Бинарное дерево и BST (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация бинарного дерева и обходов
    cout << "1. Бинарное дерево и обходы:" << endl;

    // Создаем дерево вручную:
    //        10
    //       /  \
    //      5    15
    //     / \     \
    //    3   7     20
    //       /
    //      6
    Node* root = new Node(10);
    root->left = new Node(5);
    root->right = new Node(15);
    root->left->left = new Node(3);
    root->left->right = new Node(7);
    root->left->right->left = new Node(6);
    root->right->right = new Node(20);

    BinaryTree tree;
    tree.setRoot(root);

    tree.preOrder();
    tree.inOrder();
    tree.postOrder();
    tree.levelOrder();
    cout << "Сумма всех элементов: " << tree.getSum() << endl;

    // 2. Бинарное дерево поиска (BST)
    cout << "\n2. Бинарное дерево поиска (BST):" << endl;
    BST bst;
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (int key : keys) {
        bst.insert(key);
    }

    cout << "Симметричный обход BST: ";
    bst.inOrder();

    cout << "Высота дерева: " << bst.getHeight() << endl;
    cout << "Минимальный элемент: " << bst.getMin() << endl;
    cout << "Максимальный элемент: " << bst.getMax() << endl;

    cout << "Поиск 40: " << (bst.search(40) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (bst.search(100) ? "найден" : "не найден") << endl;

    // Поиск предшественника (вариант 6)
    cout << "\nПоиск предшественника для ключей:" << endl;
    int testKeys[] = { 50, 40, 20, 80, 100 };
    for (int key : testKeys) {
        int pred = bst.getPredecessor(key);
        cout << "Предшественник для " << key << ": " << (pred != -1 ? to_string(pred) : "не найден") << endl;
    }

    // 3. Применение BST: случайные числа, удаление, визуализация
    cout << "\n3. Применение BST (N = 20 случайных чисел):" << endl;
    BST randomBst;
    for (int i = 0; i < 20; ++i) {
        randomBst.insert(rand() % 100);
    }

    cout << "Отсортированная последовательность (in-order): ";
    randomBst.inOrder();

    cout << "Минимальный элемент: " << randomBst.getMin() << endl;
    cout << "Максимальный элемент: " << randomBst.getMax() << endl;
    cout << "Высота дерева: " << randomBst.getHeight() << endl;

    cout << "\nВизуализация дерева (повернутая на 90 градусов):" << endl;
    randomBst.print();

    // Удаление элемента, введенного пользователем
    int keyToDelete;
    cout << "\nВведите элемент для удаления: ";
    cin >> keyToDelete;

    if (randomBst.search(keyToDelete)) {
        randomBst.remove(keyToDelete);
        cout << "Элемент " << keyToDelete << " удален." << endl;
        cout << "Дерево после удаления:" << endl;
        randomBst.print();
    }
    else {
        cout << "Элемент " << keyToDelete << " не найден в дереве." << endl;
    }

    return 0;
}