#include "AVLTreeHeader.hpp"

int main() {
    // Create an AVL tree container for integers.
    AVLTree<int> tree;

    // Insert elements into the AVL tree.
    tree.insert(15);
    tree.insert(10);
    tree.insert(20);
    tree.insert(8);
    tree.insert(12);
    tree.insert(16);
    tree.insert(25);
    tree.insert(5);
    tree.insert(11);
    tree.insert(13);

    // Traverse the tree using range-based for loop (in-order)
    std::cout << "Tree elements (in-order traversal): ";
    for (const auto& value : tree) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Use the find function to check for a specific value.
    int valueToFind = 10;
    auto it = tree.find(valueToFind);
    if (it != tree.end()) {
        std::cout << "Found " << valueToFind << " in the AVL tree." << std::endl;
    } else {
        std::cout << "Value " << valueToFind << " not found in the AVL tree." << std::endl;
    }

    // Erase an element from the tree.
    int valueToErase = 10;
    tree.erase(valueToErase);
    std::cout << "After erasing " << valueToErase << ", the AVL tree elements are: ";
    for (const auto& value : tree) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Check the size of the tree.
    std::cout << "Tree size: " << tree.size() << std::endl;

    // Clear the tree.
    tree.clear();
    std::cout << "After clearing, is the tree empty? " 
              << (tree.empty() ? "Yes" : "No") << std::endl;

    return 0;
}