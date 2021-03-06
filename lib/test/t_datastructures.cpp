#include <gtest/gtest.h>
#include <inc/datastructures.hpp>
#include "matrix.hpp"
#include <thread>

TEST(Heap, NodeConstructor)
{
    HeapNodeMax<int,4> node(5);

    ASSERT_EQ( node.m_children.size(), 4 );
    ASSERT_EQ( node.m_value, 5 );
    ASSERT_EQ( node.m_nbrOfElements, 1 );

    for( HeapNode<int,4>* n: node.m_children )
        ASSERT_EQ(n, nullptr);
}

TEST(Heap, Find)
{
    std::vector<int> data = {6,5,3,2,1};
    Heap<int,3>* heap = new Heap<int,3>();

    for( int i : data )
        heap->insert(i);

    std::cout << heap;

    for( int i : data )
        ASSERT_TRUE(heap->find(i) != nullptr);

    ASSERT_TRUE(heap->find(-1) == nullptr);

    delete heap;

}

bool maxHeapCheck( HeapNode<int, 2>* node )
{
    int node_val = node->m_value;

    for( auto e : node->m_children )
    {
        if( e != nullptr )
        {
            if( maxHeapCheck(e) )
            {
                // children value must be smaller or equal. if bigger, invalid heap
                if( node_val < e->m_value )
                    return false;
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

TEST(Heap, BigHeap)
{
    size_t nbrOfElementsToInsert = 20000;
    Matrix<int> data = Matrix<int>::random(nbrOfElementsToInsert, 1, 0, 10000);
    Heap<int, 2>* binaryHeap = new Heap<int, 2>();

    ASSERT_EQ(binaryHeap->size(), 0);

    for(size_t i = 0; i < data.getNbrOfElements(); i++)
        binaryHeap->insert(data.data()[i]);

    ASSERT_EQ(binaryHeap->size(), nbrOfElementsToInsert);

    // assure max is in root
    auto max = data.max();
    ASSERT_EQ( std::get<2>(max), binaryHeap->m_root->m_value);

    // assure all values in heap
    for(size_t i = 0; i < data.getNbrOfElements(); i++)
    {
        int val = data.data()[i];
        ASSERT_TRUE( binaryHeap->find(val) != nullptr );
    }

    // value -1 not in heap
    ASSERT_TRUE( binaryHeap->find(-1) == nullptr );

    // check heap is valid
    ASSERT_TRUE(maxHeapCheck( binaryHeap->m_root ));

    delete binaryHeap;
}

TEST(Heap, MinNodeConstructor)
{
    HeapNodeMin<int,4>* node = new HeapNodeMin<int,4>(5);

    ASSERT_EQ( node->m_children.size(), 4 );
    ASSERT_EQ( node->m_value, 5 );

    for( HeapNode<int,4>* n: node->m_children )
        ASSERT_EQ(n, nullptr);

    delete node;
}

TEST(Heap, MinNodeInsert)
{
    std::vector<int> data = {3,2,0,1};
    HeapNodeMin<int,3> root(0);

    for( int i : data )
        ASSERT_TRUE(root.insert(i));

    std::cout << &root << std::endl;

    ASSERT_EQ( root.m_value, 0 );

    ASSERT_EQ( root.m_children.at(0)->m_value, 3);
    ASSERT_EQ( root.m_children.at(1)->m_value, 2 );
    ASSERT_EQ( root.m_children.at(2)->m_value, 0 );

    ASSERT_EQ( root.m_children.at(2)->m_children.at(0)->m_value, 1);
}

TEST(Heap, BigMinHeap)
{
    Matrix<int> data = Matrix<int>::random(10000, 1, 0, 10000);
    Heap<int, 2>* binaryHeap = new Heap<int, 2>(HeapType::Min);

    for(size_t i = 0; i < data.getNbrOfElements(); i++)
        binaryHeap->insert(data.data()[i]);

    // assure min is in root
    auto min = data.min();
    ASSERT_EQ( std::get<2>(min), binaryHeap->m_root->m_value);

    // assure all values in heap
    for(size_t i = 0; i < data.getNbrOfElements(); i++)
    {
        int val = data.data()[i];
        ASSERT_TRUE( binaryHeap->find(val) != nullptr );
    }

    // value -1 not in heap
    ASSERT_TRUE( binaryHeap->find(-1) == nullptr );

    delete binaryHeap;
}

TEST(Heap, NodeDepth)
{
    HeapNode<int,2>* root = new HeapNodeMax<int,2>(5);
    ASSERT_EQ( root->depth(), 1);

    ASSERT_TRUE(root->insert(0));
    ASSERT_EQ( root->depth(), 2);

    ASSERT_TRUE(root->insert(4));
    ASSERT_EQ( root->depth(), 2);
    ASSERT_TRUE(root->insert(3));
    ASSERT_TRUE(root->insert(2));
    ASSERT_TRUE(root->insert(1));

    ASSERT_EQ( root->depth(), 4 );
    ASSERT_EQ( root->m_children.at(0)->depth(), 1);
    ASSERT_EQ( root->m_children.at(1)->depth(), 3);

    delete root;


    HeapNode<int,1>* listNode = new HeapNodeMax<int,1>(10);
    listNode->insert(9);
    listNode->insert(8);
    listNode->insert(7);
    listNode->insert(6);

    ASSERT_EQ( listNode->depth(), 5);

    delete listNode;
}

TEST(Heap, Balance)
{
    Matrix<int> data = Matrix<int>::random(16, 1, 0, 100);
    Heap<int, 2>* binaryHeap = new Heap<int, 2>(HeapType::Max);

    for(size_t i = 0; i < data.getNbrOfElements(); i++)
        binaryHeap->insert(data.data()[i]);

    for( const HeapNode<int,2>* n: binaryHeap->m_root->m_children )
        std::cout << "Child depth = " << n->depth() << ", nbr of elements = " << n->m_nbrOfElements << std::endl;

    delete binaryHeap;
}


TEST(BST, NodeConstruct)
{
    BSTNode<int>* node = new BSTNode<int>(5);

    ASSERT_EQ(node->m_val, 5);
    ASSERT_TRUE(node->m_right == nullptr);
    ASSERT_TRUE(node->m_left == nullptr);

    node->insert(5); // again 5 -> nothing happens
    ASSERT_TRUE(node->m_right == nullptr);
    ASSERT_TRUE(node->m_left == nullptr);

    node->insert(6);
    ASSERT_EQ(node->m_right->m_val,6);

    node->insert(4);
    ASSERT_EQ(node->m_right->m_val, 6);

    delete node;
}

BST<int>* getTestBST()
{
    BST<int>* bst = new BST<int>();
    int data[] = {8,3,10,1,6,14,13,4,9,12,7};
    for( const int& k : data )
    {
        bst->insert(k);
    }

    return bst;
}

TEST(BST, Insert)
{
    auto bst = getTestBST();

    ASSERT_EQ( bst->m_root->m_val, 8 );

    ASSERT_EQ( bst->m_root->m_left->m_val, 3 );
    ASSERT_EQ( bst->m_root->m_right->m_val, 10 );

    ASSERT_EQ( bst->m_root->m_left->m_left->m_val, 1);
    ASSERT_EQ( bst->m_root->m_left->m_left->m_left, nullptr);
    ASSERT_EQ( bst->m_root->m_left->m_left->m_right, nullptr);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_val, 6);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_left->m_val, 4);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_right->m_val, 7);

    ASSERT_EQ( bst->m_root->m_right->m_right->m_val, 14 );
    ASSERT_EQ( bst->m_root->m_right->m_left->m_val, 9 );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_left->m_val, 13 );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_right, nullptr );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_left->m_left->m_val, 12 );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_left->m_right, nullptr );

    // insert second time the same element does not work
    ASSERT_FALSE(bst->insert(8));
    ASSERT_FALSE(bst->insert(13));

    // insert new element works
    ASSERT_TRUE(bst->insert(100));

    delete bst;
}

auto searchFunctor = [] (const BST<int>* bst, const std::vector<int>& v, size_t rStart, size_t rEnd) {
    for( size_t i = rStart; i < rEnd && i < v.size(); i++  )
    {
        ASSERT_TRUE( bst->find(v.at(i)) );
    }
};


TEST(BST, Search)
{
    auto bst = getTestBST();
    std::vector<int> bstVals = {8,3,10,1,6,14,13,4,9,12,7};

    std::vector<std::thread> ths;
    int nThreads = 4;
    size_t pSize = bstVals.size() / nThreads;
    for( int t = 0; t < nThreads; t++ )
    {
        if( t == (nThreads - 1 )) // last
            ths.push_back( std::thread( searchFunctor, bst, std::ref(bstVals), t*pSize, bstVals.size() ) );
        else
            ths.push_back( std::thread( searchFunctor, bst, std::ref(bstVals), t*pSize, (t+1)*pSize ) );
    }

    for( std::thread& thr : ths )
        thr.join();


    ASSERT_FALSE( bst->find(20) );
    ASSERT_FALSE( bst->find(0) );

    delete bst;
}

TEST(BST, NodeType)
{
    auto bst = getTestBST();

    ASSERT_EQ( BSTNode<int>::getNodeType(nullptr), BSTNode<int>::NodeType::NoNode);
    ASSERT_EQ( BSTNode<int>::getNodeType( bst->m_root ), BSTNode<int>::NodeType::TwoChildren);
    ASSERT_EQ( BSTNode<int>::getNodeType( bst->m_root->m_left->m_left ), BSTNode<int>::NodeType::NoChild);
    ASSERT_EQ( BSTNode<int>::getNodeType( bst->m_root->m_right->m_right ), BSTNode<int>::NodeType::OnlyLeftChild);

    bst->insert(2);
    ASSERT_EQ( BSTNode<int>::getNodeType( bst->m_root->m_left->m_left ), BSTNode<int>::NodeType::OnlyRightChild);
}

TEST(BST, LeftMostChild)
{
    auto bst = getTestBST();
    ASSERT_EQ(bst->m_root->getLeftMostChild()->m_val, 1);
    ASSERT_EQ(bst->m_root->m_right->getLeftMostChild()->m_val, 9);
    ASSERT_EQ(bst->m_root->m_right->m_right->getLeftMostChild()->m_val, 12);
    ASSERT_EQ(bst->m_root->m_right->m_right->getLeftMostChild()->getLeftMostChild()->m_val, 12);
}

TEST(BST, Remove)
{
    auto bst = getTestBST();

    // remove when no child -> delete node
    ASSERT_EQ( bst->m_root->m_left->m_right->m_left->m_val, 4);
    bst->remove(4);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_left, nullptr);

    // remove when 1 child -> 6 replaced by 7
    ASSERT_EQ( bst->m_root->m_left->m_right->m_val, 6);
    bst->remove(6);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_val, 7);

    // remove when 1 child -> replace node with child node
    ASSERT_EQ( bst->m_root->m_right->m_right->m_val, 14 );
    bst->remove(14);
    ASSERT_EQ( bst->m_root->m_right->m_right->m_val, 13 ); // 13 was only child of 14, therefore it was replaced

    // remove when 2 children -> replace node left most element of right subtree
    // 10 is replaced with 12
    ASSERT_EQ( bst->m_root->m_right->m_val, 10 );
    bst->remove(10);
    ASSERT_EQ( bst->m_root->m_right->m_val, 12 );
    ASSERT_EQ( bst->m_root->m_right->m_left->m_val, 9 );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_val, 13 );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_left, nullptr );
    ASSERT_EQ( bst->m_root->m_right->m_right->m_right, nullptr );

    bst->remove(12);
    ASSERT_EQ( bst->m_root->m_right->m_left->m_val, 9 );
    ASSERT_EQ( bst->m_root->m_right->m_val, 13 );
    ASSERT_EQ( bst->m_root->m_right->m_right, nullptr );

    delete bst;
}

TEST(BST, RemoveRoot)
{
    auto bst = getTestBST();

    ASSERT_EQ( bst->m_root->m_right->m_left->m_val, 9 );
    ASSERT_EQ( bst->m_root->m_val, 8 );

    // root 8 replaced by 9
    bst->remove(8);

    ASSERT_EQ( bst->m_root->m_val, 9 );
    ASSERT_EQ( bst->m_root->m_right->m_left, nullptr);

    delete bst;
}

TEST(BST, RemoveAllElements)
{
    BST<int>* bst = new BST<int>();
    bst->insert(1);
    bst->insert(2);

    bst->remove(1);
    bst->remove(2);

    ASSERT_EQ(bst->m_root, nullptr);

    delete bst;
}

TEST(BST, Size)
{
    auto bst = getTestBST();
    ASSERT_EQ(bst->size(), 11);

    int testBSTContent[] = {3,10,1,6,14,13,4,9,8,12,7};

    for( int k = 0; k < 11; k++ )
    {
        bst->remove( testBSTContent[k] );
        ASSERT_EQ(bst->size(), 11-1-k);
    }

    delete bst;
}

auto searchFunctor2 = [] (const BST<int>* bst, const Matrix<int>& v, size_t rStart, size_t rEnd) {
    for( size_t i = rStart; i < rEnd && i < v.rows(); i++  )
    {
        ASSERT_TRUE( bst->find(v(i,0)) );
    }
};


TEST(BST, SpeedCheck)
{
    // Compare lookup time between binary heap and tree
    size_t nbrOfElementsToInsert = 1000000;
    Matrix<int> data = Matrix<int>::random(nbrOfElementsToInsert, 1, 0, 1000000);

/*
    std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();
    Heap<int, 2>* binaryHeap = new Heap<int, 2>();
    for (size_t i = 0; i < data.getNbrOfElements(); i++)
        binaryHeap->insert(data.data()[i]);
    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    std::cout << "Insert binary heap: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count() << " ms" << std::endl;

    t_begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < data.getNbrOfElements(); i++)
        ASSERT_NE(binaryHeap->find(data.data()[i]), nullptr);
    t_end = std::chrono::steady_clock::now();
    std::cout << "Search binary heap: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count() << " ms" << std::endl;
*/

    std::chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();
    BST<int>* bst = new BST<int>();
    for (size_t i = 0; i < data.getNbrOfElements(); i++)
        bst->insert(data.data()[i]);
    std::chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    std::cout << "Insert binary search tree: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count() << " ms" << std::endl;

    t_begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < data.getNbrOfElements(); i++)
        ASSERT_TRUE(bst->find(data.data()[i]));
    t_end = std::chrono::steady_clock::now();
    std::cout << "Search binary search tree: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count() << " ms" << std::endl;



    t_begin = std::chrono::steady_clock::now();
    std::vector<std::thread> ths;
    int nThreads = 6;
    size_t pSize = nbrOfElementsToInsert / nThreads;
    for( int t = 0; t < nThreads; t++ )
    {
        if( t == (nThreads - 1 )) // last
            ths.push_back( std::thread( searchFunctor2, bst, std::ref(data), t*pSize, nbrOfElementsToInsert ) );
        else
            ths.push_back( std::thread( searchFunctor2, bst, std::ref(data), t*pSize, (t+1)*pSize ) );
    }

    for( std::thread& thr : ths )
        thr.join();

    t_end = std::chrono::steady_clock::now();
    std::cout << "Threaded search binary search tree: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count() << " ms" << std::endl;



    delete bst;
}

TEST(BST, Print)
{
    auto bst = getTestBST();
    bst->print();

    std::cout << std::endl << std::endl;

    bst->remove(10);
    bst->print();
}

TEST(BST, Height)
{
    BST<int>* bst = new BST<int>();

    ASSERT_EQ(bst->height(), 0);

    bst->insert(10);
    ASSERT_EQ(bst->height(), 1);

    bst->insert(11);
    bst->insert(5);
    ASSERT_EQ(bst->height(), 2);

    bst->insert(4);
    bst->insert(6);
    ASSERT_EQ(bst->height(), 3);

    bst->insert(3);
    ASSERT_EQ(bst->height(), 4);

    bst->print();

    delete bst;
}

TEST(BST, BalanceFactor)
{
    BST<int>* bst = new BST<int>();
    bst->insert(10);
    bst->insert(11);
    bst->insert(5);
    bst->insert(4);
    bst->insert(6);
    bst->insert(3);

    bst->computeBalanceFactors();

    ASSERT_EQ(bst->m_root->m_balanceFactor, -2);
    ASSERT_EQ(bst->m_root->m_right->m_balanceFactor, 0);
    ASSERT_EQ(bst->m_root->m_left->m_balanceFactor, -1);
    ASSERT_EQ(bst->m_root->m_left->m_left->m_balanceFactor, -1);

    delete bst;
}

TEST(BST, TreeRotation)
{
    // example https://en.wikipedia.org/wiki/Tree_rotation#/media/File:Tree_Rotations.gif
    BST<int>* bst = new BST<int>();
    bst->insert(5); bst->insert(3); bst->insert(7); bst->insert(2); bst->insert(4);

    ASSERT_FALSE( bst->m_root->m_left->m_left->rotateRight() );
    ASSERT_TRUE( bst->m_root->rotateRight() );

    ASSERT_EQ( bst->m_root->m_val, 3);
    ASSERT_EQ( bst->m_root->m_left->m_val, 2);
    ASSERT_EQ( bst->m_root->m_right->m_val, 5);
    ASSERT_EQ( bst->m_root->m_right->m_left->m_val, 4);

    // rotate back -> bst is again original
    ASSERT_TRUE( bst->m_root->rotateLeft() );
    ASSERT_EQ( bst->m_root->m_val, 5);
    ASSERT_EQ( bst->m_root->m_right->m_val, 7);
    ASSERT_EQ( bst->m_root->m_left->m_val, 3);
    ASSERT_EQ( bst->m_root->m_left->m_left->m_val, 2);
    ASSERT_EQ( bst->m_root->m_left->m_right->m_val, 4);

    delete bst;
}

TEST(BST, Flatten)
{
    BST<int>* bst = new BST<int>();
    bst->insert(5); bst->insert(3); bst->insert(7); bst->insert(2); bst->insert(4);

    bst->m_root->flatten();
    bst->print();

    ASSERT_EQ(bst->m_root->m_val, 2);
    ASSERT_EQ(bst->m_root->m_right->m_val, 3);
    ASSERT_EQ(bst->m_root->m_right->m_right->m_val, 4);
    ASSERT_EQ(bst->m_root->m_right->m_right->m_right->m_val, 5);
    ASSERT_EQ(bst->m_root->m_right->m_right->m_right->m_right->m_val, 7);

    delete bst;
}

TEST(BST, MaxNumberOfElements)
{
    ASSERT_EQ( BSTNode<int>::getMaxNumberOfNodes(0), 0);
    ASSERT_EQ( BSTNode<int>::getMaxNumberOfNodes(1), 1);
    ASSERT_EQ( BSTNode<int>::getMaxNumberOfNodes(2), 3);
    ASSERT_EQ( BSTNode<int>::getMaxNumberOfNodes(3), 7);
}

TEST(BST, MinHeight)
{
    ASSERT_EQ( BSTNode<int>::getCompactHeight(0), 0);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(1), 1);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(2), 2);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(3), 2);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(4), 3);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(6), 3);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(7), 3);
    ASSERT_EQ( BSTNode<int>::getCompactHeight(8), 4);
}

TEST(BST, NodesLowestLayer)
{
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(0), 0);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(1), 0);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(2), 1);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(3), 0);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(4), 1);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(5), 2);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(6), 3);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(7), 0);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(8), 1);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(10), 3);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(12), 5);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(14), 7);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(15), 0);
    ASSERT_EQ( BSTNode<int>::getCompactNbrNodesLowestLayer(16), 1);
}

TEST(BST, Compare)
{
    BST<int>* bst0 = new BST<int>();
    int d0[] = {1,0,2,3,4};
    for( int k : d0 )
        bst0->insert(k);

    BST<int>* bst1 = new BST<int>();
    int d1[] = {1,0,2,4,3};
    for( int k : d1 )
        bst1->insert(k);

    BST<int>* bst2 = new BST<int>();
    int d2[] = {1,2,3,4,0};
    for( int k : d2 )
        bst2->insert(k);

    ASSERT_FALSE(bst0->compare( bst1 ));
    ASSERT_FALSE(bst1->compare( bst0 ));

    ASSERT_TRUE(bst0->compare( bst2 ));
    ASSERT_TRUE(bst2->compare( bst0 ));

    delete bst0;
    delete bst1;
    delete bst2;
}

// http://www.smunlisted.com/day-stout-warren-dsw-algorithm.html
TEST(BST, Balance)
{
    BST<int>* bst = new BST<int>();
    int data[] = {10,5,7,20,15,30,25,40,23};
    for( int k : data )
        bst->insert(k);

    BST<int>* cmp = new BST<int>();
    int cmpData[] = {23,15,30,7,20,25,40,5,10};
    for( int k : cmpData )
        cmp->insert(k);

    ASSERT_FALSE(bst->isBalanced());
    bst->balance();
    ASSERT_TRUE(bst->isBalanced());

    ASSERT_TRUE( bst->compare(cmp));

    delete bst;
    delete cmp;
}

TEST(BST, IsBalanced)
{
    BST<int>* bst = new BST<int>();

    ASSERT_TRUE(bst->isBalanced());

    bst->insert(5);
    ASSERT_TRUE(bst->isBalanced());

    bst->insert(6);
    ASSERT_TRUE(bst->isBalanced());

    bst->insert(4);
    ASSERT_TRUE(bst->isBalanced());

    bst->insert(3);
    ASSERT_TRUE(bst->isBalanced());

    bst->insert(2);
    ASSERT_FALSE(bst->isBalanced());

    delete bst;
}

TEST(BST, BatchBalancing)
{
    size_t runs = 100;
    size_t bstSize = 1024;

    for(int k = 0; k < runs; k++)
    {
        // random data to random bst
        Matrix<int> rData = Matrix<int>::random(bstSize,1, 0, 100000);
        BST<int>* bst = new BST<int>();
        for(size_t i = 0; i < bstSize; i++)
            bst->insert(rData(i,0));

        bst->balance();
        ASSERT_TRUE(bst->isBalanced());

        delete bst;
    }
}

TEST(BST, MinMax)
{
    BST<int>* bst = getTestBST();

    ASSERT_EQ(bst->getMax(), 14);
    ASSERT_EQ(bst->getMin(), 1);

    BST<int>* emptyBST = new BST<int>();
    EXPECT_ANY_THROW(emptyBST->getMax());

    delete bst;
    delete emptyBST;
}
