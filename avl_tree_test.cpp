#include <cassert>
#include <string>
#include <sstream>

#include "avl_tree.h"

using namespace std;


void print_separator() {
    std::cout << "----------------------" << std::endl;
}

void test_clear_get_size()
{
    avl_tree<int, std::string> tree;
    assert(tree.get_size() == 0);
    assert(tree.empty());
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");
    assert(tree.get_size() == 7);
    tree.clear();
    assert(tree.empty());
    assert(tree.get_size() == 0);
    cout << "Clear and get size tests passed" << endl;
}

void test_insert_get_find()
{
    avl_tree<int, std::string> tree;

    assert(tree.empty());
    assert(tree.get_size() == 0);
    tree.insert(10, "A");
    assert(tree.is_balanced());
    assert(tree.get_size() == 1);
    tree.insert(5, "B");
    assert(tree.is_balanced());
    assert(tree.get_size() == 2);
    tree.insert(15, "C");
    assert(tree.is_balanced());
    assert(tree.get_size() == 3);

    assert(tree.find(10));
    assert(tree.find(5));
    assert(tree.find(15));

    assert(tree.find(11) == false);

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    assert(tree.is_balanced());
    assert(tree.get_size() == 3);

    assert(tree[10] == "A");
    assert(tree[5] == "B");
    assert(tree[15] == "C");

    tree.insert(10, "O");
    assert(tree.get_size() == 3);

    assert(tree[10] == "O");

    tree.insert(10, "Grapes");
    assert(tree.get_size() == 3);

    assert(tree[10] == "Grapes");

    cout << "Insert, find and get tests passed" << endl;
}

void test_remove()
{
    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    assert(tree.remove(2));
    assert(!tree.find(2));
    assert(tree.is_balanced());
    assert(tree.get_size() == 6);

    assert(tree.remove(5));
    assert(!tree.find(5));
    assert(tree.is_balanced());
    assert(tree.get_size() == 5);

    assert(tree.remove(15));
    assert(!tree.find(15));
    assert(tree.is_balanced());
    assert(tree.get_size() == 4);

    assert(!tree.remove(100));
    assert(tree.is_balanced());
    assert(tree.get_size() == 4);

    assert(tree.remove(10));
    assert(!tree.find(10));
    assert(tree.is_balanced());
    assert(tree.get_size() == 3);
    cout << "Remove tests passed!" << endl;
}

void test_assignment_operator()
{

    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    avl_tree<int, std::string> tree2;
    assert(tree2.get_size() == 0);
    assert(tree2.empty());

    tree2 = tree;

    assert(tree.get_size() == tree2.get_size());
    assert(tree2.is_balanced());

    assert(tree[10] == tree2[10]);
    assert(tree[5] == tree2[5]);
    assert(tree[2] == tree2[2]);
    assert(tree[18] == tree2[18]);

    tree2[10] = "HelloWord!";
    assert(tree2[10] == "HelloWord!");
    assert(tree[10] == "A");

    tree2.remove(10);
    assert(tree2.find(10) == false);
    assert(tree.find(10) == true);

    cout << "Assignment_operator tests passed!" << endl;
}

void test_copy_constructor()
{

    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    avl_tree<int, std::string> tree2 = tree;

    assert(tree[10] == tree2[10]);
    assert(tree[5] == tree2[5]);
    assert(tree[2] == tree2[2]);
    assert(tree[18] == tree2[18]);

    tree2[10] = "HelloWord!";
    assert(tree2[10] == "HelloWord!");
    assert(tree[10] == "A");

    tree2.remove(10);
    assert(tree2.find(10) == false);
    assert(tree.find(10) == true);

    cout << "Copy constructor tests passed!" << endl;
}

void test_print()
{
    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    std::ostringstream output;
    output << tree;
    cout << output.str();

    // Check the printed output
    std::string expectedOutput =
            "            18:G\n"
            "      15:C\n"
            "            12:F\n"
            " 10:A\n"
            "            8:E\n"
            "      5:B\n"
            "            2:D\n";

    assert(output.str() == expectedOutput);

    std::cout << "Print test passed!" << std::endl;
}

void test_for_each(){
    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    std::vector<std::pair<int, std::string>> elements;

    auto accumulateFn = [&elements](const int &key, const std::string &info)
    {
        elements.emplace_back(key, info);
    };

    tree.for_each(accumulateFn);

    std::vector<std::pair<int, std::string>> expectedElements = {
            {2, "D"},
            {5, "B"},
            {8, "E"},
            {10, "A"},
            {12, "F"},
            {15, "C"},
            {18, "G"}};

    assert(elements == expectedElements);

    std::cout << "For_each tests passed!" << std::endl;
}

void test_maxinfo_selector(){
    avl_tree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    int n = 3;
    std::vector<std::pair<int, std::string>> selectedElements = maxinfo_selector(tree, n);

    std::vector<std::pair<int, std::string>> expectedElements = {
            {18, "G"},
            {12, "F"},
            {8, "E"}};

    assert(selectedElements == expectedElements);

    selectedElements = maxinfo_selector(tree, 10);
    expectedElements = {
            {18, "G"},
            {12, "F"},
            {8, "E"},
            {2, "D"},
            {15, "C"},
            {5, "B"},
            {10, "A"}};

    assert(selectedElements == expectedElements);

    selectedElements = maxinfo_selector(tree, 0);
    expectedElements = {};
    assert(selectedElements == expectedElements);

    tree[2] = "F";
    selectedElements = maxinfo_selector(tree, n);

    expectedElements = {
            {18, "G"},
            {14, "F"},
            {8, "E"}};

    std::cout << "Maxinfo_selector tests passed!" << std::endl;
}

int test_count_words(){
    for (int rep = 0; rep < 5; ++rep)
    {
        std::ifstream is("beagle_voyage.txt");
        if (!is)
        {
            std::cout << "Error opening input file.\n";
            return 1;
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        std::string word;
        avl_tree<std::string, int> wc; // counting word occurrences in the stream
        while (is >> word)
        {
            wc[word]++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        std::cout << "Ellapsed time: " << time/std::chrono::milliseconds(1)
                  << " ms.\n";
    }
    return 0;
}


int main(){
    print_separator();
    test_clear_get_size();
    print_separator();
    test_insert_get_find();
    print_separator();
    test_remove();
    print_separator();
    test_assignment_operator();
    print_separator();
    test_copy_constructor();
    print_separator();
    test_print();
    print_separator();
    test_for_each();
    print_separator();
    
    test_maxinfo_selector();
    print_separator();
    test_count_words();
    
    return 0;
}