#ifndef Header
#define Header
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include"catch.hpp"


namespace GRLMAT001
{
using namespace std;

class HuffmanNode
    {
    private:
        char key;       //input char
        int freq;      //frequency of key
    public:
        //left & right child of nod
        shared_ptr<HuffmanNode> left = nullptr;
        shared_ptr<HuffmanNode> right = nullptr;

        HuffmanNode();
        HuffmanNode(char c, int f);
        ~HuffmanNode();
        HuffmanNode(const HuffmanNode & rhs);
        HuffmanNode(HuffmanNode && rhs);
        
        bool operator<(const HuffmanNode &rhs)const;
        int getFreq() const;
        char getKey() const;

        //For equals operator constructor
        HuffmanNode & operator=(const HuffmanNode & rhs);
        //For equals operator constructor
        HuffmanNode & operator=(const HuffmanNode && rhs);
    };

//ordering smallest first
struct HuffmanCompare {bool operator() (const shared_ptr<HuffmanNode> & a, const shared_ptr<HuffmanNode> & b) const {return !((*a) < (*b));}};

class HuffmanTree
{


    
private:                                  // private members
    shared_ptr<HuffmanNode> rootNode;
    unordered_map<char, int> map;
    unordered_map< char, string > CodeTable;      //to store codeTable information
    
public:          // public members

    HuffmanTree();  // default constructor - define in .cpp
    HuffmanTree(string in,string out);
    ~HuffmanTree(); // destructor - define in .cpp file
    HuffmanTree(const HuffmanTree & rhs);     //copy constructor
    HuffmanTree(HuffmanTree && rhs);      //move constructor

    //For equals operator constructor
    HuffmanTree & operator=(const HuffmanTree & rhs);
    //For equals operator constructor
    HuffmanTree & operator=(const HuffmanTree && rhs);

    //----------------------------------------------

    string read(string filename);     //reads from text file into program

    //unordered_map<char,int> 
    unordered_map<char, int> uMap(const string & fromFile);    //inserts input into unordered map

    //Function to build tree
    shared_ptr<HuffmanNode> constructTree(priority_queue<shared_ptr<HuffmanNode>, vector<shared_ptr<HuffmanNode>>, HuffmanCompare> & pq);

    //build code table
    
    void codeTable(shared_ptr<HuffmanNode> rootNode, string bitString);

    //int bitStream (string & cStr, const string & out);

    void writeCTToFile(unordered_map< char, int > & kft, const string & name, const unordered_map< char, string > & table) const;

    void writeToFile(const string & cStr,const string & out) const;     //writes bitstring to file

    string compress(string s); //Encodes strings

};

} // namespace GRLMAT001
#endif