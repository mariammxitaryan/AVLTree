#ifndef AVLTREEHPP
#define AVLTREEHPP

#include <iostream>
#include <iterator>
#include <algorithm>
#include <utility>

template <typename T, typename Compare = std::less<T> >
class AVLTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        Node(const T& k, Node* par = nullptr) 
            : key(k), left(nullptr), right(nullptr), parent(par), height(1) {}
    };

    Node* root;
    std::size_t tree_size;
    Compare comp;

    int heigth(Node* node) const;
    int getBalanceFactor(Node* node) const;
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    void clear(Node* node);

public:
    AVLTree();
    ~AVLTree();

    class const_iterator;
    class iterator {
        friend class AVLTree;
    private:
        Node* current;
        const AVLTree* tree;
        static Node* minimum(Node* node);

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        iterator() : current(nullptr), tree(nullptr) {}
        iterator(Node* node, const AVLTree* t) : current(node), tree(t) {}

        reference operator*() const {
            return current->key;
        }
        pointer operator->() const {
            return &(current->key);
        }
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    class const_iterator {
        friend class AVLTree;
    private:
        const Node* current;
        const AVLTree* tree;

        static const Node* minimum(const Node* node);

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() : current(nullptr), tree(nullptr) {}
        const_iterator(const Node* node, const AVLTree* t) : current(node), tree(t) {}
        const_iterator(const typename AVLTree<T, Compare>::iterator& it)
            : current(it.current), tree(it. tree) {}

        reference operator*() const {
            return current->key;
        }
        pointer operator->() const {
            return &(current->key);
        }
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator==(const const_iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const const_iterator& other) const {
            return current != other.current;
        }
    };

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iteratoe end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    Node* insert(Node* node, const T& key, Node* parent);
    Node* minValueNode(Node* node) const;
    Node* deleteNode(Node* node, const T& key);
    bool empty() const;
    std::size_t size() const;
    void insert(const T& key);
    void erase(const T& key);
    void clear();
    iterator find(const T& key);
    const_iterator find(const T& key) const;

};

#include "AVLTreeImplementation.tpp"

#endif