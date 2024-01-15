#include <iostream>
#include <iomanip>
#include <functional>
#include <fstream>
#include <chrono>
#include <vector>

#pragma once

template <typename Key, typename Info>
class avl_tree{
private:
    class Node{
    private:
        Node* left;
        Node* right;
        int height;

    public:
        Key key;
        Info info;

        Node(const Key& key, const Info& info, Node* left = nullptr, Node* right = nullptr, int height = 1): key(key), info(info), left(left), right(right), height(height) {}

        friend class avl_tree;
    };

    Node *root = nullptr;
    int size = 0;

    template <typename Fn> void for_each(Node* node, Fn fn){
        if (node == nullptr) return;
        for_each(node->left, fn);
        fn(node->key, node->info);
        for_each(node->right, fn);
    }

    bool is_balanced_helper(typename avl_tree<Key, Info>::Node* node){
        if (node == nullptr) return true;

        int b_factor = avl_tree<Key, Info>::balance_factor(node);

        if (b_factor < -1 || b_factor > 1) return false;  // Check if the balance factor is within the range [-1, 0, 1] || returns false if the tree is not balanced in this node

        return is_balanced_helper(node->left) && is_balanced_helper(node->right);
    }

    void clear_helper(Node* node){
        if (node != nullptr)
        {
            clear_helper(node->left);
            clear_helper(node->right);
            delete node;
            size--;
        }
    }

    int balance_factor(Node* node){
        if (node == nullptr) return 0;

        int left_height = (node->left != nullptr) ? node->left->height : 0;
        int right_height = (node->right != nullptr) ? node->right->height : 0;

        return left_height - right_height;
    }

    void update_height(Node* node){
        if (node != nullptr){
            int left_height = (node->left != nullptr) ? node->left->height : 0;
            int right_height = (node->right != nullptr) ? node->right->height : 0;
            
            node->height = 1 + std::max(left_height, right_height);
        }
    }

    Node* copy_helper(const Node* src){
        if (src == nullptr) return nullptr;

        Node *new_node = new Node(src->key, src->info);
        new_node->left = copyHelper(src->left);
        new_node->right = copyHelper(src->right);
        new_node->height = src->height;

        return new_node;
    }

    Node *insert_helper(Node*& node, const Key& key, const Info& info)
    {
        if(node == nullptr){
            size++;
            return new Node(key, info);
        }

        if(key < node->key){
            node->left = insert_helper(node->left, key, info);
        } else{
            throw std::invalid_argument("Duplicate key found");
            // if duplicate is found, update the info
            node->info = info;
            return node;
        }

        update_height();
        return balance(node);
    }

    Node* balance(Node* node){
        update_height(node);

        int b_factor = balance_factor(node);

        if (b_factor > 1){
            // Left-Right case
            if (balance_factor(node->left) < 0){
                node->left = rotate_left(node->left);
            }
            // Left-Left case
            return rotate_right(node);
        }

        if (b_factor < -1){
            // Right-Left case
            if (balance_factor(node->right) > 0){
                node->right = rotate_right(node->right);
            }
            // Right-Right case
            return rotate_left(node);
        }

        return node;
    }

    Node* rotate_right(Node* rotate)
    {
        Node *new_root = rotate->left;
        rotate->left = new_root->right;
        new_root->right = rotate;

        update_height(rotate);
        update_height(new_root);

        return new_root;
    }

    Node* rotate_left(Node* rotate){
        Node *new_root = rotate->right;
        rotate->right = new_root->left;
        new_root->left = rotate;

        update_height(rotate);
        update_height(new_root);

        return new_root;
    }

    Node *findMin(Node *node) const
    {
        if (!node->left)
        {
            return node;
        }
        return findMin(node->left);
    }

    Node *findNode(Node *node, const Key &key) const
    {

        if (node == nullptr || key == node->key)
        {
            return node;
        }

        if (key < node->key)
        {
            return findNode(node->left, key);
        }
        else
        {
            return findNode(node->right, key);
        }
    }

    bool removeHelper(Node *&node, const Key &key)
    {
        if (!node)
        {
            return false; // node not found
        }
        bool deleted = false;
        if (node->key > key)
        {
            deleted = removeHelper(node->left, key);
        }
        else if (node->key < key)
        {
            deleted = removeHelper(node->right, key);
        }
        else
        {
            if (!node->left || !node->right)
            {
                Node *temp = node->left ? node->left : node->right;

                if (!temp) // no child
                {
                    temp = node;
                    node = nullptr;
                }
                else // one child
                {
                    *node = *temp; // Copy the content of the non-empty child
                }

                delete temp;
            }
            else
            {
                Node *successor = findMin(node->right);
                node->key = successor->key;
                node->info = successor->info;
                removeHelper(node->right, successor->key);
            }

            deleted = true;
        }

        node = balance(node);
        return deleted;
    }

    void printTree(std::ostream &os, Node *node, int indent) const
    {
        if (node != nullptr)
        {
            // Print right subtree
            printTree(os, node->right, indent + 6);

            // Print current node
            os << std::setw(indent) << ' ';
            os << node->key << ":" << node->info << "\n";

            // Print left subtree
            printTree(os, node->left, indent + 6);
        }
    }

public:
    // Constructor
    avl_tree(){};

    // Copy constructor
    avl_tree(const avl_tree &src)
    {
        *this = src;
    }

    // Destructor
    ~avl_tree()
    {
        clear();
    }

    // Overloaded assignment operator
    avl_tree &operator=(const avl_tree &src)
    {
        if (this != &src)
        {
            clear();
            root = copyHelper(src.root);
            this->size = src.size;
        }

        return *this;
    }

    template <typename Fn>
    void for_each(Fn fn) { for_each(root, fn); }
    vector<pair<Key, Info>> getLargest(int n)
    {
        std::vector<pair<Key, Info>> result;
        getLargestHelper(root, n, result);
        return result;
    }

    vector<pair<Key, Info>> getSmallest(int n)
    {
        std::vector<pair<Key, Info>> result;
        getSmallestHelper(root, n, result);
        return result;
    }

    bool empty() const
    {
        return size == 0;
    }

    int get_size() const
    {
        return size;
    }

    /**
     * @brief removes all elements from avl tree
     *
     */
    void clear()
    {
        clear_helper(root);
        root = nullptr;
    }

    /**
     * @brief Inserts element to avl tree. If element not exists in the tree it will be inserted. If it exists the action will be defined by onKeyExists.
     *
     * @param key is the key that will be inserted
     * @param info is info that will be inserted
     * @param onKeyExists is function that will be called if key already exists, by default function returns new info
     */
    void insert(
            const Key &key, const Info &info,
            function<Info(const Info &oldInfo, const Info &newInfo)> onKeyExists =
            [](const Info &oldInfo, const Info &newInfo)
            { return newInfo; })
    {
        root = insertHelper(root, key, info, onKeyExists);
    }

    /**
     * @brief removes element from avl tree
     *
     * @param key is the key that will be removed
     * @return true if element was removed
     * @return false if element not exists
     */
    bool remove(const Key &key)
    {
        if (removeHelper(root, key))
        {
            size--;
            return true;
        }
        return false;
    }

    /**
     * @brief searches for element in avl tree
     *
     * @param key is the key that will be searched
     * @return true if element found
     * @return false if element not found
     */
    bool find(const Key &key) const
    {
        return findNode(root, key) != nullptr;
    }

    /**
     * @brief returns info by key
     *
     * @param key is the key that will be searched
     * @return Info& info associated with the key
     */
    Info &operator[](const Key &key)
    {
        Node *node = findNode(root, key);
        if (node == nullptr)
        {
            throw std::runtime_error("Key not found");
        }
        return node->info;
    }

    /**
     * @brief returns info by key
     *
     * @param key is the key that will be searched
     * @return const Info& info associated with the key
     */
    const Info &operator[](const Key &key) const
    {
        Node *node = findNode(root, key);
        if (node == nullptr)
        {
            throw std::runtime_error("Key not found");
        }
        return node->info;
    }

    friend std::ostream &operator<<(std::ostream &os, const avl_tree &tree)
    {
        if (tree.get_size() > 40)
        {
            os << "Tree is too big to print";
            return os;
        }
        tree.printTree(os, tree.root, 0);
        return os;
    }

    // Function designed just for testing
    bool isBalanced()
    {
        return isBalancedHelper(root);
    }
    //
};

// External methods

template <typename Key, typename Info>
std::vector<std::pair<Key, Info>> maxinfo_selector(const avl_tree<Key, Info> &tree, unsigned cnt)
{
    avl_tree<pair<Info, Key>, int> inverted;

    auto copy = tree;

    auto accumulateFn = [&inverted](const Key &key, const Info &info)
    {
        inverted.insert(make_pair(info, key), 1);
    };

    copy.for_each(accumulateFn);

    vector<pair<pair<Info, Key>, int>> largest = inverted.getLargest(cnt);

    vector<pair<Key, Info>> aboba;
    for (auto &pair : largest)
    {
        aboba.push_back(make_pair(pair.first.second, pair.first.first));
    }

    return aboba;
}

avl_tree<string, int> count_words(istream &is)
{
    std::string word;
    avl_tree<string, int> wc;
    while (is >> word)
    {
        wc.insert(word, 1, [](const int &oldValue, const int &newValue)
        { return oldValue + newValue; });
    }
    return wc;
}