#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "../Header.h"
#include <unordered_map>
#include <queue>
#include <memory>

using namespace std;
using namespace GRLMAT001;

//Test case 1
TEST_CASE("Huffman tree building and destroying - all methods","[HuffmanTree]")
{

    HuffmanTree tree = HuffmanTree();

    SECTION("Testing letter frequency table")
    {
        //Test string
        string testString = "It's an exciting time in the world right now";
        unordered_map<char,int> map = tree.uMap(testString);

        //Check uMap is correct
        REQUIRE( int(map.at('I')) == 1);
        REQUIRE( int(map.at('t')) == 5);
        REQUIRE( int(map.at('\'')) == 1);
        REQUIRE( int(map.at('s')) == 1);
        REQUIRE( int(map.at('a')) == 1);
        REQUIRE( int(map.at('n')) == 4);
        REQUIRE( int(map.at('e')) == 3);
        REQUIRE( int(map.at('x')) == 1);
        REQUIRE( int(map.at('c')) == 1);
        REQUIRE( int(map.at('g')) == 2);
        REQUIRE( int(map.at('m')) == 1);
        REQUIRE( int(map.at('h')) == 2);
        REQUIRE( int(map.at('w')) == 2);
        REQUIRE( int(map.at('o')) == 2);
        REQUIRE( int(map.at('r')) == 2);
        REQUIRE( int(map.at('l')) == 1);
        REQUIRE( int(map.at('d')) == 1);
    }

    SECTION("Test Huffman node < operator")
    {
        HuffmanNode greater = HuffmanNode('a',18);
        HuffmanNode lesser = HuffmanNode('a',9);
        REQUIRE( (lesser < greater) == true );
        REQUIRE( (greater < lesser) == false );
    }

    SECTION("Test priority queue of Huffman Nodes")
    {
        priority_queue<shared_ptr<HuffmanNode>,vector<shared_ptr<HuffmanNode>>,HuffmanCompare> pqTest;
        //Sample nodes
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('w',12)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('q',19)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('i',2)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('k',15)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('u',4)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('y',8)));

        //Check priority queue is correct
        shared_ptr<HuffmanNode> topOfQueue = pqTest.top();

        REQUIRE( (*topOfQueue).getFreq() == 2);
        pqTest.pop();
        REQUIRE((*pqTest.top()).getFreq() == 4);
        pqTest.pop();
        REQUIRE((*pqTest.top()).getFreq() == 8);
        pqTest.pop();
        REQUIRE((*pqTest.top()).getFreq() == 12);
        pqTest.pop();
        REQUIRE((*pqTest.top()).getFreq() == 15);
        pqTest.pop();
        REQUIRE((*pqTest.top()).getFreq() == 19);
    }

    SECTION("Building the tree")
    {
        priority_queue<shared_ptr<HuffmanNode>,vector<shared_ptr<HuffmanNode>>,HuffmanCompare> pqTest;
                                                                              
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('x',7)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('a',6)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('h',1)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('q',15)));
        pqTest.push(shared_ptr<HuffmanNode>(new HuffmanNode('y',4))); 

        shared_ptr<HuffmanNode> testRootNode = tree.constructTree(pqTest);
        REQUIRE((*testRootNode).getFreq() == 33 ); //root is total freq
        REQUIRE(testRootNode.use_count() == 2 );
        REQUIRE(testRootNode->left.use_count() == 1);
        REQUIRE(testRootNode->right.use_count() == 1);

        SECTION("Test the compress method")
        {
            //Tests whole compress method
            tree.codeTable(testRootNode,"");
            string compressedString = tree.compress("qxhyxxxhy"); //only letters from prev test
            REQUIRE(compressedString == "0101100110110101011001101");
        }

    }
}

