#include <iostream>
using namespace std;

template <typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;

        Node(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // Obtener la altura de un nodo
    int height(Node* node) {
        return node ? node->height : 0;
    }

    // Máximo entre dos números
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    // Factor de balance de un nodo
    int getBalanceFactor(Node* node) {
        if (!node) return 0;
        return height(node->left) - height(node->right);
    }

    // Rotación a la derecha
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Rotación
        x->right = y;
        y->left = T2;

        // Actualizar alturas
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Rotación a la izquierda
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Rotación
        y->left = x;
        x->right = T2;

        // Actualizar alturas
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Balancear un nodo
    Node* balance(Node* node) {
        int balanceFactor = getBalanceFactor(node);

        // Izquierda-Izquierda
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Derecha-Derecha
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Izquierda-Derecha
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Derecha-Izquierda
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Inserción no recursiva
    Node* insert(Node* root, T value) {
        Node* newNode = new Node(value);
        Node* current = root;
        Node* parent = nullptr;

        // Buscar el lugar correcto para insertar el nodo
        while (current) {
            parent = current;
            if (value < current->data)
                current = current->left;
            else if (value > current->data)
                current = current->right;
            else
                return root; // No se permiten duplicados
        }

        // Conectar el nuevo nodo al árbol
        if (!parent)
            return newNode; // El árbol estaba vacío
        else if (value < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;

        // Actualizar alturas y balancear el árbol
        return balanceTree(root, newNode);
    }

    // Balancear el árbol después de insertar
    Node* balanceTree(Node* root, Node* newNode) {
        Node* current = root;
        while (current) {
            current->height = max(height(current->left), height(current->right)) + 1;
            current = balance(current);
            if (newNode->data < current->data)
                current = current->left;
            else if (newNode->data > current->data)
                current = current->right;
            else
                break; // El nodo fue balanceado
        }
        return root;
    }

    // Mostrar el árbol en orden por niveles (manual, sin `queue`)
    void printLevelOrder(Node* node) {
        if (!node) return;

        // Simulación de cola con un array dinámico
        Node* nodes[100];
        int front = 0, back = 0;
        nodes[back++] = node;

        while (front < back) {
            Node* current = nodes[front++];
            cout << current->data << " ";

            if (current->left) nodes[back++] = current->left;
            if (current->right) nodes[back++] = current->right;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T value) {
        root = insert(root, value);
    }

    void printLevelOrder() {
        if (!root) {
            cout << "El árbol está vacío.\n";
            return;
        }
        printLevelOrder(root);
        cout << endl;
    }
};

int main() {
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    cout << "Árbol en orden por niveles:\n";
    tree.printLevelOrder();

    return 0;
}