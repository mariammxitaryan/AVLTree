#include "AVLTreeHeader.hpp"
#include <algorithm>  // for std::max

/***************************************
 * AVLTree Constructor
 *
 * Initializes an empty AVL tree.
 *
 * Parameters:
 *  - None
 *
 * Behavior:
 *  - Sets the root to nullptr.
 *  - Initializes tree size to 0.
 *  - Constructs a Compare object using the default.
 ***************************************/
template <typename T, typename Compare>
AVLTree<T, Compare>::AVLTree() : root(nullptr), tree_size(0), comp(Compare()) {}


/***************************************
 * AVLTree Destructor
 *
 * Frees the allocated memory by calling clear().
 *
 * Parameters:
 *  - None
 *
 * Behavior:
 *  - Clears all nodes in the AVL tree.
 ***************************************/
template <typename T, typename Compare>
AVLTree<T, Compare>::~AVLTree() {
    clear();
}


/***************************************
 * height
 *
 * Returns the height of a given node.
 *
 * Parameters:
 *  - node: A pointer to a node.
 *
 * Returns:
 *  - The node's height, or 0 if the node is nullptr.
 ***************************************/
template <typename T, typename Compare>
int AVLTree<T, Compare>::height(Node* node) const {
    return node ? node->height : 0;
}


/***************************************
 * getBalanceFactor
 *
 * Computes the balance factor of a node.
 *
 * Parameters:
 *  - node: A pointer to a node.
 *
 * Returns:
 *  - Balance factor: difference between heights of left and right child nodes.
 ***************************************/
template <typename T, typename Compare>
int AVLTree<T, Compare>::getBalanceFactor(Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}


/***************************************
 * rightRotate
 *
 * Performs a right rotation on the subtree rooted at y.
 *
 * Parameters:
 *  - y: The root of the subtree to rotate.
 *
 * Returns:
 *  - The new root after rotation.
 *
 * Behavior:
 *  - Adjusts child pointers and updates heights.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation.
    x->right = y;
    y->left = T2;

    // Update parent pointers.
    x->parent = y->parent;
    y->parent = x;
    if (T2) {
        T2->parent = y;
    }

    // Update heights.
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}


/***************************************
 * leftRotate
 *
 * Performs a left rotation on the subtree rooted at x.
 *
 * Parameters:
 *  - x: The root of the subtree to rotate.
 *
 * Returns:
 *  - The new root after rotation.
 *
 * Behavior:
 *  - Adjusts pointers and updates heights of involved nodes.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation.
    y->left = x;
    x->right = T2;

    // Update parent pointers.
    y->parent = x->parent;
    x->parent = y;
    if (T2) {
        T2->parent = x;
    }

    // Update heights.
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}


/***************************************
 * insert (helper recursive function)
 *
 * Recursively inserts a key into the subtree rooted at node.
 *
 * Parameters:
 *  - node: Current root of the subtree.
 *  - key: The value to insert.
 *  - parent: Pointer to the parent node.
 *
 * Returns:
 *  - The new root of the subtree.
 *
 * Behavior:
 *  - Performs a standard BST insert.
 *  - Updates node height.
 *  - Rebalances the subtree if needed.
 ***************************************/
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
        // Duplicate keys are not inserted.
        return node;
    }

    // Update the node's height.
    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalanceFactor(node);

    // Left Left Case.
    if (balance > 1 && comp(key, node->left->key)) {
        return rightRotate(node);
    }
    // Right Right Case.
    if (balance < -1 && comp(node->right->key, key)) {
        return leftRotate(node);
    }
    // Left Right Case.
    if (balance > 1 && comp(node->left->key, key)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left Case.
    if (balance < -1 && comp(key, node->right->key)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}


/***************************************
 * minValueNode
 *
 * Finds the node with the minimum key in the subtree.
 *
 * Parameters:
 *  - node: The root of the subtree.
 *
 * Returns:
 *  - Pointer to the node with the minimum key.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::minValueNode(Node* node) const {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}


/***************************************
 * deleteNode
 *
 * Recursively deletes a key from the subtree.
 *
 * Parameters:
 *  - node: The root of the subtree.
 *  - key: The key to delete.
 *
 * Returns:
 *  - The new root of the subtree.
 *
 * Behavior:
 *  - Follows BST deletion rules.
 *  - Updates the height and rebalances the subtree.
 ***************************************/
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
        // This node is the one to be deleted.
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                // No child case.
                temp = node;
                node = nullptr;
            }
            else {
                // One child case: copy the child into node.
                temp->parent = node->parent;
                *node = *temp;
            }
            delete temp;
            --tree_size;
        }
        else {
            // Node with two children: get the inorder successor.
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
    }

    if (!node) {
        return node;
    }
    // Update height.
    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalanceFactor(node);

    // Rebalance the node if needed.
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


/***************************************
 * clear (private recursive helper)
 *
 * Recursively deletes all nodes in the subtree.
 *
 * Parameters:
 *  - node: The current root of the subtree.
 *
 * Behavior:
 *  - Frees memory allocated to each node.
 ***************************************/
template <typename T, typename Compare>
void AVLTree<T, Compare>::clear(Node* node) {
    if (!node) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}


/***************************************
 * empty
 *
 * Checks if the AVL tree is empty.
 *
 * Returns:
 *  - true if the tree has no nodes; otherwise false.
 ***************************************/
template <typename T, typename Compare>
bool AVLTree<T, Compare>::empty() const {
    return tree_size == 0;
}


/***************************************
 * size
 *
 * Returns the number of elements in the AVL tree.
 *
 * Returns:
 *  - The tree's size.
 ***************************************/
template <typename T, typename Compare>
std::size_t AVLTree<T, Compare>::size() const {
    return tree_size;
}


/***************************************
 * insert (public interface)
 *
 * Inserts a key into the AVL tree.
 *
 * Parameters:
 *  - key: The key to insert.
 *
 * Behavior:
 *  - Calls the recursive helper to insert the key.
 ***************************************/
template <typename T, typename Compare>
void AVLTree<T, Compare>::insert(const T& key) {
    root = insert(root, key, nullptr);
}


/***************************************
 * erase
 *
 * Erases a key from the AVL tree.
 *
 * Parameters:
 *  - key: The key to erase.
 *
 * Behavior:
 *  - Calls the recursive deleteNode function.
 ***************************************/
template <typename T, typename Compare>
void AVLTree<T, Compare>::erase(const T& key) {
    root = deleteNode(root, key);
}


/***************************************
 * clear (public interface)
 *
 * Clears the AVL tree of all nodes.
 *
 * Behavior:
 *  - Calls the clear helper and resets the root and size.
 ***************************************/
template <typename T, typename Compare>
void AVLTree<T, Compare>::clear() {
    clear(root);
    root = nullptr;
    tree_size = 0;
}


/***************************************
 * find (non-const)
 *
 * Searches for a key in the AVL tree.
 *
 * Parameters:
 *  - key: The key to search for.
 *
 * Returns:
 *  - An iterator pointing to the found key or end() if not found.
 ***************************************/
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


/***************************************
 * find (const version)
 *
 * Searches for a key in the AVL tree.
 *
 * Parameters:
 *  - key: The key to search for.
 *
 * Returns:
 *  - A const_iterator pointing to the found key or end() if not found.
 ***************************************/
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


/***************************************
 * iterator::minimum (Helper)
 *
 * Finds the minimum node in the given subtree.
 *
 * Parameters:
 *  - node: The root of the subtree.
 *
 * Returns:
 *  - Pointer to the leftmost (minimum) node.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::iterator::minimum(Node* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}


/***************************************
 * iterator::operator++ (Pre-increment)
 *
 * Advances the iterator to the next element (in-order).
 *
 * Returns:
 *  - A reference to the updated iterator.
 ***************************************/
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


/***************************************
 * iterator::operator++ (Post-increment)
 *
 * Advances the iterator to the next element (in-order) but returns the original.
 *
 * Returns:
 *  - The original iterator before increment.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}


/***************************************
 * iterator::operator-- (Pre-decrement)
 *
 * Moves the iterator to the previous element (in-order).
 *
 * Returns:
 *  - A reference to the updated iterator.
 ***************************************/
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


/***************************************
 * iterator::operator-- (Post-decrement)
 *
 * Moves the iterator to the previous element (in-order) but returns the original.
 *
 * Returns:
 *  - The original iterator before decrement.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}


/***************************************
 * const_iterator::minimum (Helper)
 *
 * Finds the minimum node in the given subtree.
 *
 * Parameters:
 *  - node: The root of the subtree.
 *
 * Returns:
 *  - Pointer to the leftmost (minimum) node (const).
 ***************************************/
template <typename T, typename Compare>
const typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::const_iterator::minimum(const Node* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}


/***************************************
 * const_iterator::operator++ (Pre-increment)
 *
 * Advances the const_iterator to the next element (in-order).
 *
 * Returns:
 *  - A reference to the updated const_iterator.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator& AVLTree<T, Compare>::const_iterator::operator++() {
    if (!current)
        return *this;
    if (current->right) {
        const_cast<Node*&>(current) = current->right;
        while (current->left)
            const_cast<Node*&>(current) = current->left;
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


/***************************************
 * const_iterator::operator++ (Post-increment)
 *
 * Advances the const_iterator to the next element (in-order) but returns the original.
 *
 * Returns:
 *  - The original const_iterator before increment.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::const_iterator::operator++(int) {
    const_iterator temp = *this;
    ++(*this);
    return temp;
}


/***************************************
 * const_iterator::operator-- (Pre-decrement)
 *
 * Moves the const_iterator to the previous element (in-order).
 *
 * Returns:
 *  - A reference to the updated const_iterator.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator& AVLTree<T, Compare>::const_iterator::operator--() {
    if (!current) {
        current = tree->root;
        if (!current)
            return *this;
        while (current->right)
            current = current->right;
    } 
    else if (current->left) {
        current = current->left;
        while (current->right)
            current = current->right;
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


/***************************************
 * const_iterator::operator-- (Post-decrement)
 *
 * Moves the const_iterator to the previous element (in-order) but returns the original.
 *
 * Returns:
 *  - The original const_iterator before decrement.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::const_iterator::operator--(int) {
    const_iterator temp = *this;
    --(*this);
    return temp;
}


/***************************************
 * begin (non-const)
 *
 * Returns an iterator to the first (smallest) element in the AVL tree.
 *
 * Returns:
 *  - An iterator pointing to the leftmost node.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::begin() {
    Node* current = root;
    if (current) {
        while (current->left)
            current = current->left;
    }
    return iterator(current, this);
}


/***************************************
 * end (non-const)
 *
 * Returns an iterator representing the end of the AVL tree.
 *
 * Returns:
 *  - An iterator pointing to nullptr.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::iterator AVLTree<T, Compare>::end() {
    return iterator(nullptr, this);
}


/***************************************
 * begin (const)
 *
 * Returns a const_iterator to the first (smallest) element in the AVL tree.
 *
 * Returns:
 *  - A const_iterator pointing to the leftmost node.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::begin() const {
    Node* current = root;
    if (current) {
        while (current->left)
            current = current->left;
    }
    return const_iterator(current, this);
}


/***************************************
 * end (const)
 *
 * Returns a const_iterator representing the end of the AVL tree.
 *
 * Returns:
 *  - A const_iterator pointing to nullptr.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::end() const {
    return const_iterator(nullptr, this);
}


/***************************************
 * cbegin
 *
 * Returns a constant iterator to the beginning of the AVL tree.
 *
 * Returns:
 *  - A const_iterator to the first element.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::cbegin() const {
    return begin();
}


/***************************************
 * cend
 *
 * Returns a constant iterator to the end of the AVL tree.
 *
 * Returns:
 *  - A const_iterator to one past the last element.
 ***************************************/
template <typename T, typename Compare>
typename AVLTree<T, Compare>::const_iterator AVLTree<T, Compare>::cend() const {
    return end();
}
