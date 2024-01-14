#include "avl_tree.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include "avl_tree_test.h"

using namespace std;

void test_clear_get_size(){
    avl_tree<int, string> tree;

    assert(tree.get_size() == 0);
    assert(tree.empty());

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");

    assert(tree.get_size() == 4);

    tree.clear();

    assert(tree.empty());
    assert(tree.get_size() == 0);

    cout << "Clear and get size tests passed" << endl;
}

void test_insert_find(){
    avl_tree<int, string> tree;

    assert(tree.empty());
    assert(tree.get_size() == 0);

    tree.insert(1, "Apple");

    assert(tree.is_balanced());
    assert(tree.get_size() == 1);

    tree.insert(4, "Google");

    assert(tree.is_balanced());
    assert(tree.get_size() == 2);

    tree.insert(81, "Microsoft");

    assert(tree.is_balanced());
    assert(tree.get_size() == 3);

    assert(tree.find(4));
    assert(tree.find(1));
    assert(tree.find(81));

    assert(tree.find(11) == false);

    cout << "Insert and find tests passed" << endl;
}

void test_insert_get(){
    avl_tree<int, string> tree;

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

    tree.insert(10, "G", [](const string &oldInfo, const string &newInfo){ return oldInfo + newInfo; });

    assert(tree.get_size() == 3);

    assert(tree[10] == "OG");

    cout << "Insert and get tests passed" << endl;
}

void test_remove(){
    avl_tree<int, string> tree;

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

    cout << "All tests passed!" << endl;
}

void test_assignment_operator(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    avl_tree<int, string> tree2;

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

    cout << "All tests passed!" << endl;
}

void test_copy_constructor(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    avl_tree<int, string> tree2 = tree;

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

    cout << "All tests passed!" << endl;
}

void test_print(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    ostringstream output;
    output << tree;
    
    string expectedOutput =
            "            18:G\n"
            "      15:C\n"
            "            12:F\n"
            " 10:A\n"
            "            8:E\n"
            "      5:B\n"
            "            2:D\n";

    assert(output.str() == expectedOutput);

    cout << "Print test passed!" << endl;
}

void test_get_largest(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    int n = 3;
    vector<pair<int, string>> largestElements = tree.get_largest(n);

    vector<pair<int, string>> expectedElements = {pair<int, string>(18, "G"), pair<int, string>(15, "C"), pair<int, string>(12, "F")};

    assert(largestElements == expectedElements);

    largestElements = tree.get_largest(0);
    expectedElements = {};

    assert(largestElements == expectedElements);

    largestElements = tree.get_largest(10);
    expectedElements = {pair<int, string>(18, "G"), pair<int, string>(15, "C"), pair<int, string>(12, "F"),
                        pair<int, string>(10, "A"), pair<int, string>(8, "E"), pair<int, string>(5, "B"), pair<int, string>(2, "D")};

    assert(largestElements == expectedElements);

    cout << "Largest tests passed!" << endl;
}

void test_get_smallest(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    int n = 3;
    vector<pair<int, string>> smallestElements = tree.get_smallest(n);

    vector<pair<int, string>> expectedElements = {
            make_pair(2, "D"),
            make_pair(5, "B"),
            make_pair(8, "E")};

    assert(smallestElements == expectedElements);

    smallestElements = tree.get_smallest(0);
    expectedElements = {};

    assert(smallestElements == expectedElements);

    smallestElements = tree.get_smallest(10); // more than in the array
    expectedElements = {
            make_pair(2, "D"),
            make_pair(5, "B"),
            make_pair(8, "E"),
            make_pair(10, "A"),
            make_pair(12, "F"),
            make_pair(15, "C"),
            make_pair(18, "G")};

    assert(smallestElements == expectedElements);

    cout << "Smallest tests passed!" << endl;
}

void test_for_each(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    vector<pair<int, string>> elements;

    auto accumulateFn = [&elements](const int &key, const string &info)
    {
        elements.emplace_back(key, info);
    };

    tree.for_each(accumulateFn);

    vector<pair<int, string>> expectedElements = {
            {2, "D"},
            {5, "B"},
            {8, "E"},
            {10, "A"},
            {12, "F"},
            {15, "C"},
            {18, "G"}};

    assert(elements == expectedElements);

    cout << "All for_each tests passed!" << endl;
}

void test_maxinfo_selector(){
    avl_tree<int, string> tree;

    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");
    tree.insert(2, "D");
    tree.insert(8, "E");
    tree.insert(12, "F");
    tree.insert(18, "G");

    int n = 3;
    vector<pair<int, string>> selectedElements = maxinfo_selector(tree, n);

    vector<pair<int, string>> expectedElements = {
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

    cout << "All maxinfo_selector tests passed!" << endl;
}

void test_word_count(){
    ifstream voyage("beagle_voyage.txt");

    auto wc = count_words(voyage);
    // cout << wc.get_size() << endl;
    // auto most_used_words = maxinfo_selector(wc, 20);
    // for (const auto &pair : most_used_words)
    // {
    //     cout << pair.first << ": " << pair.second << endl;
    // }
    cout << "Count words tests passed" << endl;
}

void time_measurement(){
    for (int rep = 0; rep < 5; rep++){
        ifstream is("beagle_voyage.txt");
        auto start_time = chrono::high_resolution_clock::now();
        auto wc = count_words(is);
        auto end_time = chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        cout << "Ellapsed time: " << time / chrono::milliseconds(1) << "ms" << endl;
    }
}

int main()
{
    test_clear_get_size();
    test_insert_find();
    test_insert_get();
    test_remove();
    test_assignment_operator();
    test_copy_constructor();
    test_print();
    test_get_largest();
    test_get_smallest();
    test_for_each();
    cout
            << "All tests passed!" << endl;

    // External functions tests
    test_maxinfo_selector();
    test_word_count();

    time_measurement();
}