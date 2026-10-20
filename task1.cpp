// Lab 8 Task 1
// string: a(b(d,e),c(,f(g,)))
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;

    Node(char c) {
        data = c;
        left = NULL;
        right = NULL;
    }
};

// builds the tree from the string, i is the current position in string
// time complexity: O(n)
Node* buildTree(const string& s, int& i) {
    // no letter means no child here
    if (i >= (int)s.size() || !isalpha(s[i])) {
        return NULL;
    }

    Node* node = new Node(s[i]);
    i++;

    // if there is a bracket then it has children
    if (i < (int)s.size() && s[i] == '(') {
        i++;
        node->left = buildTree(s, i);
        if (i < (int)s.size() && s[i] == ',') {
            i++;
        }
        node->right = buildTree(s, i);
        if (i < (int)s.size() && s[i] == ')') {
            i++;
        }
    }
    return node;
}

// preorder: root, left, right
// time complexity: O(n)
void preorder(Node* root) {
    if (root == NULL) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

// same preorder but printed like the given string
// time complexity: O(n)
void preorderString(Node* root) {
    if (root == NULL) return;
    cout << root->data;
    if (root->left != NULL || root->right != NULL) {
        cout << "(";
        preorderString(root->left);
        cout << ",";
        preorderString(root->right);
        cout << ")";
    }
}

// inorder with stack: left, root, right
// time complexity: O(n)
void inorderIterative(Node* root) {
    stack<Node*> st;
    Node* cur = root;

    while (cur != NULL || !st.empty()) {
        // go left as much as we can
        while (cur != NULL) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop();
        cout << cur->data << " ";
        cur = cur->right;
    }
}

// postorder with two stacks: left, right, root
// time complexity: O(n)
void postorderIterative(Node* root) {
    if (root == NULL) return;

    stack<Node*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        Node* n = s1.top();
        s1.pop();
        s2.push(n);
        if (n->left != NULL) s1.push(n->left);
        if (n->right != NULL) s1.push(n->right);
    }

    // second stack gives the postorder when we pop it
    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
}

// checks the string without making the tree
// time complexity: O(m), m is the length of the string
bool validateHelper(const string& s, int& i) {
    // empty child is okay
    if (i >= (int)s.size() || !isalpha(s[i])) {
        return true;
    }
    i++;

    if (i < (int)s.size() && s[i] == '(') {
        i++;
        if (!validateHelper(s, i)) return false;

        // need a comma between the two children
        if (i >= (int)s.size() || s[i] != ',') return false;
        i++;

        if (!validateHelper(s, i)) return false;

        // need a closing bracket
        if (i >= (int)s.size() || s[i] != ')') return false;
        i++;
    }
    return true;
}

bool isValidTreeString(const string& s) {
    if (s.empty() || !isalpha(s[0])) return false;
    int i = 0;
    bool ok = validateHelper(s, i);
    // whole string should be used
    return ok && i == (int)s.size();
}

// height of the tree (counting edges)
// time complexity: O(n)
int height(Node* root) {
    if (root == NULL) return -1;
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

// prints depth of every node
// time complexity: O(n)
void printDepths(Node* root, int depth) {
    if (root == NULL) return;
    cout << "Depth of " << root->data << ": " << depth << endl;
    printDepths(root->left, depth + 1);
    printDepths(root->right, depth + 1);
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// BST part
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int v) {
        data = v;
        left = NULL;
        right = NULL;
    }
};

// smaller goes left, bigger goes right
// time complexity: O(log n) average, O(n) worst
BSTNode* insertBST(BSTNode* root, int value) {
    if (root == NULL) {
        return new BSTNode(value);
    }
    if (value < root->data) {
        root->left = insertBST(root->left, value);
    } else if (value > root->data) {
        root->right = insertBST(root->right, value);
    }
    return root;
}

// first number becomes root, then insert the rest
// time complexity: O(k log k) average, O(k^2) worst
BSTNode* buildBST(const vector<int>& values) {
    BSTNode* root = NULL;
    for (int i = 0; i < (int)values.size(); i++) {
        root = insertBST(root, values[i]);
    }
    return root;
}

// time complexity: O(k)
void inorderBST(BSTNode* root) {
    if (root == NULL) return;
    inorderBST(root->left);
    cout << root->data << " ";
    inorderBST(root->right);
}

void freeBST(BSTNode* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    delete root;
}

int main() {
    string s = "a(b(d,e),c(,f(g,)))";
    cout << "Given string: " << s << endl;

    int pos = 0;
    Node* root = buildTree(s, pos);

    cout << "Preorder: ";
    preorder(root);
    cout << endl;

    cout << "Preorder (string form): ";
    preorderString(root);
    cout << endl;

    cout << "Inorder: ";
    inorderIterative(root);
    cout << endl;

    cout << "Postorder: ";
    postorderIterative(root);
    cout << endl;

    // testing validation with some valid and invalid strings
    string tests[] = {
        "a(b(d,e),c(,f(g,)))",
        "a(b(d,e),c(,f(g,))",
        "a(b,c))",
        "a(b c)",
        "a(b,c,d)"
    };
    for (int i = 0; i < 5; i++) {
        cout << tests[i] << " is ";
        if (isValidTreeString(tests[i])) {
            cout << "valid" << endl;
        } else {
            cout << "invalid" << endl;
        }
    }

    cout << "Height: " << height(root) << endl;
    printDepths(root, 0);

    vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    BSTNode* bstRoot = buildBST(values);
    cout << "BST inorder: ";
    inorderBST(bstRoot);
    cout << endl;

    freeTree(root);
    freeBST(bstRoot);
    return 0;
}