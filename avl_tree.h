#ifndef EADS_LAB_3_AVL_TREE_H
#define EADS_LAB_3_AVL_TREE_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <fstream>



template<typename Key, typename Info>
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

        Node(const Key& key, const Info& info, Node* left = nullptr, Node* right = nullptr, int height = 1): key(key), info(info), left(left), right(right), height(height){}

        friend class avl_tree;
    };

    Node* root = nullptr;

    int size = 0;

    template<typename Fn> void for_each(Node* node, Fn fn){
        if(node == nullptr) return;
        for_each(node->left, fn);
        fn(node->key, node->info);
        for_each(node->right, fn);
    }

    Node *rotate_right(Node *y){
        Node *x = y->left;
        y->left = x->right;
        x->right = y;

        update_height(y);
        update_height(x);

        return x;
    }

    Node *rotate_left(Node *x){
        Node *y = x->right;
        x->right = y->left;
        y->left = x;

        update_height(x);
        update_height(y);

        return y;
    }

    bool is_balanced_helper(typename avl_tree<Key, Info>:: Node* node){
        if(node == nullptr) return true;

        int b_factor = avl_tree<Key, Info>::balance_factor(node);

        if(b_factor < -1 || b_factor > 1) return false;

        return is_balanced_helper(node->left) && is_balanced_helper(node->right);
    }

    int balance_factor(Node* node){
        if(node == nullptr) return 0;

        int left_height = (node->left != nullptr) ? node->left->height : 0;
        int right_height = (node->right != nullptr) ? node->right->height : 0;

        return right_height - left_height;
    }

    void update_height(Node* node){
        if(node != nullptr){
            int left_height = (node->left != nullptr) ? node->left->height : 0;
            int right_height = (node->right != nullptr) ? node->right->height : 0;

            node->height = 1 + std::max(left_height, right_height);
        }
    }

    void clear_helper(Node* node){
        if(node != nullptr){
            clear_helper(node->left);
            clear_helper(node->right);
            delete node;
            size--;
        }
    }

    Node* copy_helper(Node* src_node){
        if(src_node == nullptr) return nullptr;

        Node* new_node = new Node(src_node->key, src_node->info);
        new_node->left = copy_helper(src_node->left);
        new_node->right = copy_helper(src_node->right);
        new_node->height = src_node->height;

        return new_node;
    }

    Node *insert_helper(Node *node, const Key &key, const Info &info, std::function<Info(const Info &old_info, const Info &new_info)> on_key_exists){
        if (node == nullptr){
            size++;
            return new Node(key, info);
        }

        if (key > node->key){
            node->right = insert_helper(node->right, key, info, on_key_exists);
        }
        else if (key < node->key){
            node->left = insert_helper(node->left, key, info, on_key_exists);
        }
        else{
            node->info = on_key_exists(node->info, info);
        }

        return balance(node);
    }

    Node *balance(Node *node){
        update_height(node);

        int b_factor = balance_factor(node);

        if (b_factor > 1){
            // Left-Right case (LR)
            if (balance_factor(node->left) < 0){
                node->left = rotate_left(node->left);
            }
            // Left-Left case (LL)
            return rotate_right(node);
        }

        if (b_factor < -1){
            // Right-Left case (RL)
            if (balance_factor(node->right) > 0){
                node->right = rotate_right(node->right);
            }
            // Right-Right case (RR)
            return rotate_left(node);
        }

        return node;
    }

    bool remove_helper(Node *&node, const Key &key)
    {
        if (!node) return false;

        bool deleted = false;
        if (node->key > key){
            deleted = remove_helper(node->left, key);
        }
        else if (node->key < key){
            deleted = remove_helper(node->right, key);
        }
        else{
            if (!node->left || !node->right){
                Node *temp = node->left ? node->left : node->right;

                if (!temp){
                    temp = node;
                    node = nullptr;
                }
                else{
                    *node = *temp; // Copy the content of the non-empty child
                }

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

    Node *find_node(Node *node, const Key &key) const{

        if (node == nullptr || key == node->key)
        {
            return node;
        }

        if (key < node->key)
        {
            return find_node(node->left, key);
        }
        else
        {
            return find_node(node->right, key);
        }
    }

    Node *find_min(Node *node) const{
        if (!node->left){
            return node;
        }
        return find_min(node->left);
    }

    void get_largest_helper(Node *node, int &n, std::vector<std::pair<Key, Info>> &result){
        if (node == nullptr || n == 0) return;

        get_largest_helper(node->right, n, result);

        if (n > 0){
            result.push_back(std::pair<Key, Info>(node->key, node->info));
            n--;
        }

        if (n > 0){
            get_largest_helper(node->left, n, result);
        }
    }

    void get_smallest_helper(Node *node, int &n, std::vector<std::pair<Key, Info>> &result){
        if (node == nullptr || n == 0) return;

        get_smallest_helper(node->left, n, result);

        if (n > 0){
            result.push_back(std::pair<Key, Info>(node->key, node->info));
            n--;
        }

        if (n > 0){
            get_smallest_helper(node->right, n, result);
        }
    }

    void print_tree(std::ostream &os, Node *node, int indent) const
    {
        if (node != nullptr)
        {
            // Print right subtree
            print_tree(os, node->right, indent + 6);

            // Print current node
            os << std::setw(indent) << ' ';
            os << node->key << ":" << node->info << "\n";

            // Print left subtree
            print_tree(os, node->left, indent + 6);
        }
    }

public:
    avl_tree(){}

    avl_tree(const avl_tree& src){
        *this = src;
    }

    ~avl_tree(){
        clear();
    }

    avl_tree& operator=(const avl_tree& src){
        if(this != &src){
            clear();
            root = copy_helper(src.root);
            this->size = src.size;
        }

        return *this;
    }

    template <typename Fn> void for_each(Fn fn) { for_each(root, fn); }

    std::vector<std::pair<Key, Info>> get_largest(int n)
    {
        std::vector<std::pair<Key, Info>> result;
        get_largest_helper(root, n, result);
        return result;
    }

    std::vector<std::pair<Key, Info>> get_smallest(int n)
    {
        std::vector<std::pair<Key, Info>> result;
        get_smallest_helper(root, n, result);
        return result;
    }

    bool empty() const{
        return size == 0;
    }

    int get_size() const{
        return size;
    }

    void clear(){
        clear_helper(root);
        root = nullptr;
    }

    void insert(const Key &key, const Info &info, std::function<Info(const Info &old_info, const Info &new_info)> on_key_exists = [](const Info &old_info, const Info &new_info) { return new_info; }){
        root = insert_helper(root, key, info, on_key_exists);
    }

    bool remove(const Key &key){
        if (remove_helper(root, key)){
            size--;
            return true;
        }
        return false;
    }

    bool find(const Key &key) const{
        return find_node(root, key) != nullptr;
    }

    /**
 * @brief returns info by key
 *
 * @param key is the key that will be searched
 * @return Info& info associated with the key
 */
    Info& operator[](const Key& key) {
        Node* node = find_node(root, key);

        // If the key is not found, insert it with default-initialized Info
        if (node == nullptr) {
            insert(key, Info());
            node = find_node(root, key);
        }

        return node->info;
    }


    /**
    * @brief returns info by key
    *
    * @param key is the key that will be searched
    * @return const Info& info associated with the key
    */
    const Info &operator[](const Key &key) const{
        Node *node = find_node(root, key);
        if (node == nullptr){
            throw std::runtime_error("Key not found");
        }
        return node->info;
    }

    friend std::ostream &operator<<(std::ostream &os, const avl_tree &tree){
        if (tree.get_size() > 40){
            os << "Tree is too big to print";
            return os;
        }
        tree.print_tree(os, tree.root, 0);
        return os;
    }

    bool is_balanced()
    {
        return is_balanced_helper(root);
    }
};

template <typename Key, typename Info>
std::vector<std::pair<Key, Info>> maxinfo_selector(const avl_tree<Key, Info> &tree, unsigned cnt)
{
    avl_tree<std::pair<Info, Key>, int> inverted;

    auto copy = tree;

    auto accumulateFn = [&inverted](const Key &key, const Info &info)
    {
        inverted.insert(make_pair(info, key), 1);
    };

    copy.for_each(accumulateFn);

    std::vector<std::pair<std::pair<Info, Key>, int>> largest = inverted.get_largest(cnt);

    std::vector<std::pair<Key, Info>> vec;
    for (auto &pair : largest)
    {
        vec.push_back(make_pair(pair.first.second, pair.first.first));
    }

    return vec;
}

avl_tree<std::string, int> count_words(std::istream &is)
{
    std::string word;
    avl_tree<std::string, int> wc;
    while (is >> word)
    {
        wc.insert(word, 1, [](const int &oldValue, const int &newValue)
        { return oldValue + newValue; });
    }
    return wc;
}

#endif //EADS_LAB_3_AVL_TREE_H
