#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <Windows.h>

using namespace std;

// Узел AVL-дерева
struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Класс AVL-дерева
class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node) const {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode* node) const {
        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLNode* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    AVLNode* balance(AVLNode* node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = leftRotate(node->left);
            }
            return rightRotate(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rightRotate(node->right);
            }
            return leftRotate(node);
        }
        return node;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (node == nullptr) return new AVLNode(key);
        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else {
            return node;
        }
        return balance(node);
    }

    AVLNode* findMin(AVLNode* node) const {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    AVLNode* remove(AVLNode* node, int key) {
        if (node == nullptr) return nullptr;
        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                AVLNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                AVLNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                AVLNode* minNode = findMin(node->right);
                node->key = minNode->key;
                node->right = remove(node->right, minNode->key);
            }
        }
        return balance(node);
    }

    bool search(AVLNode* node, int key) const {
        if (node == nullptr) return false;
        if (key == node->key) return true;
        if (key < node->key) return search(node->left, key);
        return search(node->right, key);
    }

    void inOrder(AVLNode* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->key << " ";
        inOrder(node->right);
    }

    void destroy(AVLNode* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    int findLongestPrefix(AVLNode* node, int key) const {
        int best = -1;
        AVLNode* current = node;
        while (current != nullptr) {
            if (current->key <= key) {
                best = current->key;
                current = current->right;
            }
            else {
                current = current->left;
            }
        }
        return best;
    }

    void printTree(AVLNode* node, int level) const {
        if (node == nullptr) return;
        printTree(node->right, level + 1);
        for (int i = 0; i < level; ++i) cout << "    ";
        cout << node->key << " (h=" << node->height << ", bf=" << balanceFactor(node) << ")" << endl;
        printTree(node->left, level + 1);
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(int key) { root = insert(root, key); }
    void remove(int key) { root = remove(root, key); }
    bool search(int key) const { return search(root, key); }
    void printInOrder() const { inOrder(root); cout << endl; }
    void print() const { printTree(root, 0); }
    int getLongestPrefix(int key) const { return findLongestPrefix(root, key); }
};

// Простое BST для сравнения (итеративная вставка, поиск и удаление)
struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

public:
    BST() : root(nullptr) {}

    // Итеративный деструктор (без рекурсии)
    ~BST() {
        if (root == nullptr) return;
        // Используем стек для обхода всех узлов
        vector<BSTNode*> stack;
        stack.push_back(root);
        while (!stack.empty()) {
            BSTNode* node = stack.back();
            stack.pop_back();
            if (node->left) stack.push_back(node->left);
            if (node->right) stack.push_back(node->right);
            delete node;
        }
    }

    // Итеративная вставка
    void insert(int key) {
        if (root == nullptr) {
            root = new BSTNode(key);
            return;
        }
        BSTNode* current = root;
        while (true) {
            if (key < current->key) {
                if (current->left == nullptr) {
                    current->left = new BSTNode(key);
                    return;
                }
                current = current->left;
            }
            else if (key > current->key) {
                if (current->right == nullptr) {
                    current->right = new BSTNode(key);
                    return;
                }
                current = current->right;
            }
            else {
                return; // дубликаты не вставляем
            }
        }
    }

    // Итеративный поиск
    bool search(int key) const {
        BSTNode* current = root;
        while (current != nullptr) {
            if (key == current->key) return true;
            if (key < current->key) current = current->left;
            else current = current->right;
        }
        return false;
    }
};

// Префиксное дерево (Trie)
class Trie {
private:
    struct TrieNode {
        TrieNode* children[26];
        bool isEndOfWord;
        TrieNode() : isEndOfWord(false) {
            for (int i = 0; i < 26; ++i) children[i] = nullptr;
        }
    };

    TrieNode* root;

    void destroy(TrieNode* node) {
        if (node == nullptr) return;
        for (int i = 0; i < 26; ++i) destroy(node->children[i]);
        delete node;
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { destroy(root); }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
    }

    bool search(const string& word) const {
        TrieNode* current = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (current->children[index] == nullptr) return false;
            current = current->children[index];
        }
        return current->isEndOfWord;
    }

    bool startsWith(const string& prefix) const {
        TrieNode* current = root;
        for (char ch : prefix) {
            int index = ch - 'a';
            if (current->children[index] == nullptr) return false;
            current = current->children[index];
        }
        return true;
    }

    string longestPrefix(const string& word) const {
        TrieNode* current = root;
        string longest = "";
        string currentPrefix = "";
        for (char ch : word) {
            int index = ch - 'a';
            if (index < 0 || index >= 26) break;
            if (current->children[index] == nullptr) break;
            current = current->children[index];
            currentPrefix += ch;
            if (current->isEndOfWord) {
                longest = currentPrefix;
            }
        }
        return longest;
    }
};

long long measureAVLInsert(const vector<int>& data) {
    AVLTree tree;
    auto start = chrono::high_resolution_clock::now();
    for (int val : data) tree.insert(val);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

long long measureAVLSearch(const vector<int>& data, const vector<int>& queries) {
    AVLTree tree;
    for (int val : data) tree.insert(val);
    auto start = chrono::high_resolution_clock::now();
    for (int q : queries) tree.search(q);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

long long measureBSTInsert(const vector<int>& data) {
    BST tree;
    auto start = chrono::high_resolution_clock::now();
    for (int val : data) tree.insert(val);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

long long measureBSTSearch(const vector<int>& data, const vector<int>& queries) {
    BST tree;
    for (int val : data) tree.insert(val);
    auto start = chrono::high_resolution_clock::now();
    for (int q : queries) tree.search(q);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(static_cast<unsigned int>(time(0)));

    cout << "=== Лабораторная работа №10. AVL-дерево и Trie (Вариант 6) ===" << endl << endl;

    // 1. Демонстрация AVL-дерева
    cout << "1. Демонстрация AVL-дерева (вставка, балансировка, поиск, удаление):" << endl;
    AVLTree avl;
    vector<int> values = { 10, 20, 30, 40, 50, 25 };
    for (int val : values) {
        avl.insert(val);
        cout << "После вставки " << val << ":" << endl;
        avl.print();
        cout << endl;
    }

    cout << "Симметричный обход: ";
    avl.printInOrder();

    cout << "Поиск 25: " << (avl.search(25) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (avl.search(100) ? "найден" : "не найден") << endl;

    int target = 35;
    int prefix = avl.getLongestPrefix(target);
    cout << "Наибольший ключ <= " << target << ": " << (prefix != -1 ? to_string(prefix) : "не найден") << endl;

    avl.remove(20);
    cout << "После удаления 20:" << endl;
    avl.print();
    cout << "Симметричный обход: ";
    avl.printInOrder();

    // 2. Демонстрация Trie
    cout << "\n2. Демонстрация Trie:" << endl;
    Trie trie;
    vector<string> words = { "cat", "car", "dog", "cart", "catfish" };
    for (const string& word : words) {
        trie.insert(word);
    }

    cout << "Поиск слов:" << endl;
    cout << "search(\"cat\"): " << (trie.search("cat") ? "найдено" : "не найдено") << endl;
    cout << "search(\"car\"): " << (trie.search("car") ? "найдено" : "не найдено") << endl;
    cout << "search(\"cattle\"): " << (trie.search("cattle") ? "найдено" : "не найдено") << endl;

    cout << "Поиск по префиксу:" << endl;
    cout << "startsWith(\"ca\"): " << (trie.startsWith("ca") ? "true" : "false") << endl;
    cout << "startsWith(\"do\"): " << (trie.startsWith("do") ? "true" : "false") << endl;
    cout << "startsWith(\"ba\"): " << (trie.startsWith("ba") ? "true" : "false") << endl;

    string testWord = "catfish";
    string longest = trie.longestPrefix(testWord);
    cout << "Самый длинный префикс для \"" << testWord << "\": \"" << longest << "\"" << endl;

    // 3. Сравнительный анализ
    cout << "\n3. Сравнительный анализ AVL и BST:" << endl;
    cout << "N\t\tТип данных\tAVL вставка (мкс)\tBST вставка (мкс)\tAVL поиск (мкс)\tBST поиск (мкс)" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;

    int sizes[] = { 1000, 5000, 10000 };
    int qCount = 1000;

    for (int n : sizes) {
        vector<int> randomData(n);
        for (int i = 0; i < n; ++i) randomData[i] = rand() % (n * 10);

        vector<int> queries(qCount);
        for (int i = 0; i < qCount; ++i) queries[i] = rand() % (n * 10);

        long long avlInsRand = measureAVLInsert(randomData);
        long long bstInsRand = measureBSTInsert(randomData);
        long long avlSrchRand = measureAVLSearch(randomData, queries);
        long long bstSrchRand = measureBSTSearch(randomData, queries);

        cout << n << "\t\tСлучайные\t" << avlInsRand << "\t\t\t" << bstInsRand << "\t\t\t" << avlSrchRand << "\t\t\t" << bstSrchRand << endl;

        vector<int> sortedData(n);
        for (int i = 0; i < n; ++i) sortedData[i] = i;

        long long avlInsSort = measureAVLInsert(sortedData);
        long long bstInsSort = measureBSTInsert(sortedData);
        long long avlSrchSort = measureAVLSearch(sortedData, queries);
        long long bstSrchSort = measureBSTSearch(sortedData, queries);

        cout << n << "\t\tОтсортированные\t" << avlInsSort << "\t\t\t" << bstInsSort << "\t\t\t" << avlSrchSort << "\t\t\t" << bstSrchSort << endl;
    }

    return 0;
}