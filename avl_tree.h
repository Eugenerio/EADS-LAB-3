#include <iostream>
#include <iomanip>
#include <functional>
#include <fstream>
#include <chrono>
#include <vector>
#include <queue>
#include <map>

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
        new_node->left = copy_helper(src->left);
        new_node->right = copy_helper(src->right);
        new_node->height = src->height;

        return new_node;
    }

    void insert_helper(Node*& node, const Key& key, const Info& info, Node*& found_node)
    {
        if(node == nullptr){
            size++;
            node = new Node(key, info);
            found_node = node;
        }

        if(key < node->key) {
            insert_helper(node->left, key, info, found_node);
        } else if(key > node->key){
            insert_helper(node->right, key, info, found_node);
        } else{
            // if duplicate is found, update the info
            node->info = info;
        }

        update_height(node);
        node = balance(node);
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

    Node* find_min(Node* node) const{
        if (!node->left) return node;

        return find_min(node->left);
    }

    Node* find_max(Node* node) const{
        if(!node->left) return node;

        return find_max(node->right);
    }

    Node* find_node(Node* node, const Key& key) const{
        if (node == nullptr || key == node->key) return node;

        if (key < node->key) return find_node(node->left, key);

        else return find_node(node->right, key);
    }

    bool remove_helper(Node *&node, const Key &key)
    {
        if (!node) return false;
        
        bool deleted = false;
        
        if (node->key > key) { deleted = remove_helper(node->left, key); }

        else if (node->key < key) { deleted = remove_helper(node->right, key); }

        else{
            if (!node->left || !node->right){
                Node *temp = node->left ? node->left : node->right;

                if (!temp){
                    temp = node;
                    node = nullptr;
                }
                else { *node = *temp; }

                delete temp;
            }
            else{
                Node *successor = find_min(node->right);
                node->key = successor->key;
                node->info = successor->info;
                remove_helper(node->right, successor->key);
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

    template <typename Fn> void traverse(const Node* node, Fn fn) const {
        if (node) {
            traverse(node->left, fn);
            fn(node->key, node->info);
            traverse(node->right, fn);
        }
    }

public:
    avl_tree() {}

    avl_tree(const avl_tree& src) { *this = src; }

    ~avl_tree() { clear(); }

    avl_tree& operator=(const avl_tree& src){
        if (this != &src){
            clear();
            root = copy_helper(src.root);
            this->size = src.size;
        }

        return *this;
    }

    template <typename Fn>void for_each(Fn fn) { for_each(root, fn); }

    bool empty() const{
        return size == 0;
    }

    int get_size() const{
        return size;
    }

    /**
     * @brief removes all elements from avl tree
     *
     */
    void clear(){
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
    void insert(const Key& key, const Info& info) {
        Node* found_node;
        insert_helper(root, key, info, found_node);
    }

    /**
     * @brief removes element from avl tree
     *
     * @param key is the key that will be removed
     * @return true if element was removed
     * @return false if element not exists
     */
    bool remove(const Key& key){
        if (remove_helper(root, key)){
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
    bool find(const Key& key) const{
        return find_node(root, key) != nullptr;
    }

    /**
     * @brief returns info by key
     *
     * @param key is the key that will be searched
     * @return Info& info associated with the key
     */
    Info& operator[](const Key& key){
        Node *node = find_node(root, key);
        if (node == nullptr){
            insert_helper(root, key, Info(), node);
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
        tree.printTree(os, tree.root, 0);
        return os;
    }

    // Function designed just for testing
    bool is_balanced() { return is_balanced_helper(root); }

    template<typename Fn> void traverse(Fn fn) const{
        traverse(root, fn);
    }
};

// External methods

template <typename Key, typename Info>
std::vector<std::pair<Key, Info>> maxinfo_selector(const avl_tree<Key, Info>& tree, unsigned cnt) {
    std::vector<std::pair<Key, Info>> result;

    // Initialize priority queue to store elements based on info values
    std::priority_queue<std::pair<Info, Key>> pq;

    // Perform in-order traversal and populate the priority queue
    tree.traverse([&](const Key& key, const Info& info) {
        pq.push({info, key});
    });

    // Pop cnt elements with the highest info values from the priority queue
    while (!pq.empty() && cnt > 0) {
        auto info_key = pq.top();
        result.push_back({info_key.second, info_key.first});
        pq.pop();
        cnt--;
    }

    return result;
}

avl_tree<std::string ,int> count_words(std::istream& is){
    avl_tree<std::string,int> treeRes;

    std::string word;
    while(is >> word){
        treeRes[word]++;
    }

    return treeRes;
}