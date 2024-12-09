#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct SiblingNode {
    string name;
    SiblingNode* next;

    SiblingNode(string name) : name(name), next(nullptr) {}
};

struct PersonNode {
    string name;
    PersonNode* father;
    PersonNode* mother;
    SiblingNode* siblings;

    PersonNode(string name) : name(name), father(nullptr), mother(nullptr), siblings(nullptr) {}
};

class FamilyTree {
private:
    struct NodeList {
        PersonNode* person;
        NodeList* next;

        NodeList(PersonNode* person) : person(person), next(nullptr) {}
    };

    NodeList* nodes; // Lista de todos los nodos
    PersonNode* root;

    // Buscar o crear un nodo
    PersonNode* getNode(string name) {
        if (name.empty()) return nullptr;

        NodeList* current = nodes;
        while (current) {
            if (current->person->name == name)
                return current->person;
            current = current->next;
        }

        // Si no existe, crear uno nuevo
        PersonNode* newPerson = new PersonNode(name);
        NodeList* newNode = new NodeList(newPerson);
        newNode->next = nodes;
        nodes = newNode;
        return newPerson;
    }

    // Agregar un hermano a la lista de hermanos
    void addSibling(PersonNode* person, string siblingName) {
        if (!person || siblingName.empty()) return;

        SiblingNode* newSibling = new SiblingNode(siblingName);
        newSibling->next = person->siblings;
        person->siblings = newSibling;
    }

public:
    FamilyTree() : nodes(nullptr), root(nullptr) {}

    // Cargar datos desde un archivo CSV
    void loadFromCSV(const string& filename, const string& rootName) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, father, mother, sibling;

            getline(ss, name, ',');
            getline(ss, father, ',');
            getline(ss, mother, ',');

            PersonNode* person = getNode(name);
            person->father = getNode(father);
            person->mother = getNode(mother);

            // Leer y agregar hermanos
            while (getline(ss, sibling, ',')) {
                addSibling(person, sibling);
            }

            // Establecer la raíz del árbol
            if (name == rootName)
                root = person;
        }

        file.close();
    }

    // Imprimir el árbol en orden jerárquico
    void printTree(PersonNode* person, int level = 0) {
        if (!person) return;

        // Imprimir con indentación
        for (int i = 0; i < level; ++i)
            cout << "  ";
        cout << person->name << endl;

        // Imprimir padre y madre
        if (person->father) {
            for (int i = 0; i <= level; ++i) cout << "  ";
            cout << "Padre: " << person->father->name << endl;
        }
        if (person->mother) {
            for (int i = 0; i <= level; ++i) cout << "  ";
            cout << "Madre: " << person->mother->name << endl;
        }

        // Imprimir hermanos
        SiblingNode* sibling = person->siblings;
        if (sibling) {
            for (int i = 0; i <= level; ++i) cout << "  ";
            cout << "Hermanos: ";
            while (sibling) {
                cout << sibling->name << " ";
                sibling = sibling->next;
            }
            cout << endl;
        }

        // Continuar con el padre y la madre
        printTree(person->father, level + 1);
        printTree(person->mother, level + 1);
    }

    void print() {
        if (!root) {
            cout << "El árbol está vacío." << endl;
            return;
        }
        printTree(root);
    }
};

int main() {
    FamilyTree tree;

    // Archivo CSV de ejemplo
    string filename = "familia.csv";
    string rootName = "PersonaPrincipal";

    // Cargar datos y mostrar el árbol
    tree.loadFromCSV(filename, rootName);
    tree.print();

    return 0;
}