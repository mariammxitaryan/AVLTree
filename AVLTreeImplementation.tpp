#include "AVLTreeHeader.hpp"
 
template <typename T, typename Compare>
AVLTree<T, Compare>::AVLTree() : root(nullptr), tree_size(0), comp(Compare()) {}

template <typename T, typename Compare>
AVLTree<T, Compare>::~AVLTree() {
    clear();
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::height(Node* node) const {
    return node ? node->height : 0;
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::getBalanceFactor(Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    x->parent = y->parent;
    y->parent = x;
    if (T2) {
        T2->parent = y;
    }
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    y->parent = x->parent;
    x->parent = y;
    if (T2) {
        T2->parent = x;
    }

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::insert(Node* node, const T& key, Node* parent) {
    if (!node) {
        ++tree_size;
        return new Node(key, parent);
    }
    if (comp(key, node->key)) {
        node->left = insert(node->left, key, node);
    }
    else if (comp(node->key, key)) {
        node->right = insert(node->right, key, node);
    }
    else {
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalanceFactor(node);
    if (balance > 1 && comp(key, node->left->key)) {
        return rightRotate(node);
    }

    if (balance < -1 && comp(node->right->key, key)) {
        return leftRotate(node);
    }

    if (balance > 1 && comp(node->left->key, key)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && comp(key, node->right->key)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
} 

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::minValueNode(Node* node) const {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::deleteNode(Node* node, const T& key) {
    if (!node) {
        return node;
    }

    if (comp(key, node->key)) {
        node->left = deleteNode(node->left, key);
    }
    else if (comp(node->key, key)) {
        node->right = deleteNode(node->right, key);
    }
    else {
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            }
            else {
                temp->parent = node->parent;
                *node = *temp;
            }
            delete temp;
            --tree_size;
        }
        else {
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }

    }
    
    if (!node) {
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && getBalanceFactor(node->left) >= 0) {
        return rightRotate(node);
    }

    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) <= 0) {
        return leftRotate(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::clear(Node* node) {
    if (!node) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::empty() const {
    return tree_size == 0;
}

template <typename T, typename Compare>
std::size_t AVLTree<T, Compare>::size() const {
    return tree_size;
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::insert(const T& key) {
    root = insert(root, key, nullptr);
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::erase(const T& key) {
    root = deleteNode(root, key);
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::clear() {
    clear(root);
    root = nullptr;
    tree_size = 0;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::find(const T& key) {
    Node* current = root;
    while (current) {
        if (comp(key, current->key)) {
            current = current->left;
        }
        else if (comp(current->key, key)) {
            current = current->right;
        }
        else {
            break;
        }
    }
    return iterator(current, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::find(const T& key) const {
    const Node* current = root;
    while (current) {
        if (comp(key, current->key)) {
            current = current->left;
        }
        else if (comp(current->key, key)) {
            current = current->right;
        }
        else {
            break;
        }
    }
    return const_iterator(current, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::iterator::minimum(Node* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator& AVLTree<T, Compare>::iterator::operator++() {
    if (!current) {
        return *this;
    }
    if (current->right) {
        current = minimum(current->right);
    }
    else {
        Node* p = current->parent;
        while (p && current == p->right) {
            current = p;
            p = p->parent;
        }
        current = p;
    }
    return *this;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator& AVLTree<T, Compare>::iterator::operator--() {
    if (!current) {
        current = tree->root;
        if (!current) {
            return *this;
        }
        while (current->right) {
            current = current->right;
        }
    }
    else if (current->left) {
        current = current->left;
        while (current->right) {
            current = current->right;
        }
    }
    else {
        Node* p = current->parent;
        while (p && current == p->left) {
            current = p;
            p = p->parent;
        }
        current = p;
    }
    return *this;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}

template <typename T, typename Compare>
const typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::const_iterator::minimum(const Node* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator& AVLTree<T, Compare>::const_iterator::operator++() {
    if (!current) {
        return *this;
    }
    if (current->right) {
        const_cast<Node*&>(current) = current->right;
        while (current->left) {
            const_cast<Node*&>(current) = current->left;
        }
    } 
    else {
        const Node* p = current->parent;
        while (p && current == p->right) {
            current = p;
            p = p->parent;
        }
        current = p;
    }
    return *this;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::const_iterator::operator++(int) {
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator& AVLTree<T, Compare>::const_iterator::operator--() {
    if (!current) {
        current = tree->root;
        if (!current) {
            return *this;
        }
        while (current->right) {
            current = current->right;
        }
    } 
    else if (current->left) {
        current = current->left;
        while (current->right) {
            current = current->right;
        }
    } 
    else {
        const Node* p = current->parent;
        while (p && current == p->left) {
            current = p;
            p = p->parent;
        }
        current = p;
    }
    return *this;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::const_iterator::operator--(int) {
    const_iterator temp = *this;
    --(*this);
    return temp;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::begin() {
    Node* current = root;
    if (current) {
        while (current->left) {
            current = current->left;
        }
    }
    return iterator(current, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::end() {
    return iterator(nullptr, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::begin() const {
    Node* current = root;
    if (current) {
        while (current->left) {
            current = current->left;
        }
    }
    return const_iterator(current, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::end() const {
    return const_iterator(nullptr, this);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::cbegin() const {
    return begin();
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::cend() const {
    return end();
}