#include <iostream>
#include <fstream>
using namespace std;

struct Disease {
    string name;
    int count;
    Disease() {}
    Disease(string n, int c) { name = n; count = c; }
};

struct Node {
    Disease data;
    Node *left;
    Node *right;
    int height;
    Node(Disease dis) { data = dis; left = NULL; right = NULL; height = 1; }
};

Node *root = NULL;
string filename = "dataset/Diseasecategory.txt";

int getHeight(Node *node) {
    if (node == NULL) return 0;
    return node->height;
}

int getBalance(Node *node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

Node *insert(Node *node, Disease d) {
    if (node == NULL) return new Node(d);
    if (d.name < node->data.name) node->left = insert(node->left, d);
    else if (d.name > node->data.name) node->right = insert(node->right, d);
    else {
        node->data.count += d.count;
        return node;
    }
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);
    if (balance > 1 && d.name < node->left->data.name) return rightRotate(node);
    if (balance < -1 && d.name > node->right->data.name) return leftRotate(node);
    if (balance > 1 && d.name > node->left->data.name) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && d.name < node->right->data.name) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node *min_val(Node *node) {
    Node *current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

Node *deleteNode(Node *node, string name) {
    if (node == NULL) return node;
    if (name < node->data.name) node->left = deleteNode(node->left, name);
    else if (name > node->data.name) node->right = deleteNode(node->right, name);
    else {
        if ((node->left == NULL) || (node->right == NULL)) {
            Node *temp = node->left ? node->left : node->right;
            if (temp == NULL) {
                temp = node;
                node = NULL;
            } else *node = *temp;
            delete temp;
        } else {
            Node *temp = min_val(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.name);
        }
    }
    if (node == NULL) return node;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0) return rightRotate(node);
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) return leftRotate(node);
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void inorder(Node *node) {
    if (!node) return;
    inorder(node->left);
    cout << node->data.name << " (" << node->data.count << ")\n";
    inorder(node->right);
}

void findMostCommon(Node *node, string &name, int &maxCount) {
    if (!node) return;
    if (node->data.count > maxCount) {
        maxCount = node->data.count;
        name = node->data.name;
    }
    findMostCommon(node->left, name, maxCount);
    findMostCommon(node->right, name, maxCount);
}

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

void add_Disease() {
    string name;
    int count;
    cout << "Enter disease name: ";
    getline(cin >> ws, name);
    cout << "Enter count: ";
    cin >> count;
    Disease d(trim(name), count);
    root = insert(root, d);
    ofstream file(filename, ios::app);
    if (file) file << d.name << " | " << d.count << endl;
    cout << "Disease added successfully.\n";
}

void updat_Disease() {
    string name;
    int newCount;
    cout << "Enter disease name to update: ";
    getline(cin >> ws, name);
    cout << "Enter new count: ";
    cin >> newCount;
    Disease d(trim(name), newCount);
    root = insert(root, d);
    cout << "Disease updated successfully.\n";
}

void delete_Disease() {
    string name;
    cout << "Enter disease name to delete: ";
    getline(cin >> ws, name);
    root = deleteNode(root, trim(name));
    cout << "Disease deleted successfully.\n";
}

void view_Diseases() {
    cout << "Showing all diseases...\n";
    if (!root) {
        cout << "No diseases found.\n";
        return;
    }
    cout << "Disease List (Inorder Traversal):\n";
    inorder(root);
}

void Count_Diseases() {
    string name = "";
    int maxCount = 0;
    findMostCommon(root, name, maxCount);
    if (name == "") cout << "No data available.\n";
    else cout << "Most common disease: " << name << " (" << maxCount << ")\n";
}

void loadDiseasesFromFile(const string &fname) {
    ifstream file(fname);
    if (!file) {
        cout << "Could not open file " << fname << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        string s = trim(line);
        if (s.empty()) continue;
        size_t pos = s.find('|');
        string name;
        if (pos != string::npos) {
            name = trim(s.substr(0, pos));
        } else {
            name = s;
        }
        if (name.empty()) continue;
        Disease d(name, 1);
        root = insert(root, d);
    }
    file.close();
    cout << "Diseases loaded successfully from file.\n";
}

int main() {
    loadDiseasesFromFile(filename);
    while (true) {
        int choice;
        cout << "\nADMIN DISEASE MANAGEMENT\n";
        cout << "1. Add Disease\n";
        cout << "2. Update Disease\n";
        cout << "3. Delete Disease\n";
        cout << "4. View Disease List\n";
        cout << "5. Count Most Common Disease\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: add_Disease(); break;
            case 2: updat_Disease(); break;
            case 3: delete_Disease(); break;
            case 4: view_Diseases(); break;
            case 5: Count_Diseases(); break;
            case 6: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
 
        }
    }
    return 0;
}