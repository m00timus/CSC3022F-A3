#include <string>
#include<iostream>
#include<fstream>
#include "Header.h"

using namespace std;

namespace GRLMAT001         //declaring namespace to work in
{        
//default constructor
HuffmanTree::HuffmanTree() {}
//destructor
HuffmanTree::~HuffmanTree() {}
//move constructor
HuffmanTree::HuffmanTree(HuffmanTree && rhs) : rootNode(new HuffmanNode(*rhs.rootNode))
{
    this->map = std::move(rhs.map);
    this->CodeTable = std::move(rhs.CodeTable);
    rhs.rootNode.reset();
    rhs.map.clear();
    rhs.CodeTable.clear();

}
// Copy constructor 
HuffmanTree::HuffmanTree(const HuffmanTree & rhs)  
{
    this->map = rhs.map;
    this->CodeTable = rhs.CodeTable;
} 
//Copy assignment operator
HuffmanTree & HuffmanTree::operator=(const HuffmanTree & rhs) 
{
    this->map = rhs.map;
    this->CodeTable = rhs.CodeTable;
    rootNode.operator=(rhs.rootNode);
    return *this;
}
//"=" override operator
HuffmanTree & HuffmanTree::operator=(const HuffmanTree && rhs) {
        this->rootNode = std::move(rhs.rootNode);
        this->map = std::move(rhs.map);
        this->CodeTable = move(rhs.CodeTable);
        return *this;
    }
//------------------------------------------------------
//default constructor and destructor
HuffmanNode::HuffmanNode() {}
HuffmanNode::~HuffmanNode()  {}
//copy constructor
HuffmanNode::HuffmanNode(const HuffmanNode & rhs) : left(new HuffmanNode(*rhs.left)), right(new HuffmanNode(*rhs.right)) 
{
    this->freq = rhs.freq;
    this->key = rhs.key;
}
//move constructor
HuffmanNode::HuffmanNode(HuffmanNode && rhs) 
{
    left = std::move(rhs.left);
    right = std::move(rhs.right);
    this->freq = std::move(rhs.freq);
    this->key = std::move(rhs.key);
    
}
//constructor to build a node
HuffmanNode::HuffmanNode(char c, int f) {
    this->key = c;
    this->freq = f;
}
//Copy assignment
HuffmanNode & HuffmanNode::operator=(const HuffmanNode &rhs) 
{
    this->key = rhs.key;
    this->freq = rhs.freq;
    this->left.operator=(rhs.left);
    this->right.operator=(rhs.right);
    return *this;
}
//Move assignment
HuffmanNode & HuffmanNode::operator=(const HuffmanNode &&rhs) 
{
    this->left = std::move(rhs.left);
    this->right =std::move(rhs.right);
    this->key =  std::move(rhs.key);
    this->freq = std::move(rhs.freq);
    return *this;
}
// "<" operation
bool HuffmanNode::operator<(const HuffmanNode & rhs) const {
    return (this->freq < rhs.getFreq());
}   
int HuffmanNode::getFreq() const {
    return freq;
}
char HuffmanNode::getKey() const {
    return key;
}
//------------------------------------------------------
//reads text from file - returns string
std::string HuffmanTree::read(std::string filename)
{
    ifstream in(filename);
        string input((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();
        return input;
}
unordered_map<char, int> HuffmanTree::uMap(const string &fromFile)     //generates map of letter occurences
{
    char * letters  = (char *)  fromFile.c_str();       //put string into array
    //std::unordered_map<char, int> map;
    for (unsigned int i = 0; i < fromFile.length(); ++i)  //nb ++i
    {
        const unordered_map<char,int>::iterator found = map.find(letters[i]); //Check if already in map

        if (found != map.end())     //if letter found
        {
            found -> second = found -> second + 1;      //add frequency if occurence found
        }
        else
        {
            map.insert({letters[i],1});     //else add new letter to freq map
        } 
    }

    //for (unordered_map<char,int>::iterator i = map.begin(); i != map.end();++i)
    //{
    //    int j = 1;
    //    cout << letters[j] << " : "<< map.at(letters[j]) << "\n"; //testing
    //    j++;
    //}

    return map;
}
HuffmanTree::HuffmanTree(string infile,string outfile)
{
    HuffmanTree d;
    this->map = d.uMap(infile);
    cout << "map generated successfully.\n";
    //priority queue - smallest first
    priority_queue<shared_ptr<HuffmanNode>, vector<shared_ptr<HuffmanNode>>,HuffmanCompare> pqNodes;

    for (unordered_map<char,int>::iterator i = map.begin(); i != map.end(); ++i)
    {
        pqNodes.push(shared_ptr<HuffmanNode>(new HuffmanNode(i->first,i->second)));
    }
    
    this->rootNode = constructTree(pqNodes); 
    cout << "tree generated successfully\n"; //test

    codeTable(rootNode,"");

    cout << "bitstring representation generated.\n";
    //cout<< "test: bitstring for " << CodeTable.at(0);
    string compressedData = compress(infile);   //gets encoding

    d.writeToFile(compressedData,outfile);
    cout<<"successfully wrote data to file: " << outfile << "\n";
    
    string name = "codeTable.txt";
    d.writeCTToFile(map,name,CodeTable);
    cout << "successfully wrote code table to file: " << name << "\n";

    int sizeOfOriginal = infile.length() * sizeof(char);
    cout<<"File size of original file in bytes: "<<sizeOfOriginal<<"\n";
    int sizeOfOutput = outfile.length() * sizeof(char);
    cout<<"File size of output file in bytes: "<<sizeOfOutput<<"\n";
    int sizeOfCT = name.length() * sizeof(char);
    cout<<"File size of codeTable file in bytes: "<<sizeOfCT<<"\n";
    //int sizeOfBitStream = d.bitStream(compressedData,outfile) * sizeof(char);
    //cout<<"File size of bitstream file in bytes: "<<sizeOfBitStream<<"\n";

}
shared_ptr<HuffmanNode> HuffmanTree::constructTree(priority_queue<shared_ptr<HuffmanNode>,vector<shared_ptr<HuffmanNode>>,HuffmanCompare> & pq)
{
    //enable shared_ptr
    shared_ptr<HuffmanNode> newNode = nullptr;  
        while (pq.size() > 1){
            //get smallest frequency - put left
            shared_ptr <HuffmanNode> newLeft = pq.top();
            pq.pop();
            //Get 2nd sammlest freq - put right
            shared_ptr <HuffmanNode> newRight = pq.top();
            pq.pop();
            //sum of frequency of children
            int sum = (*newLeft).getFreq() + (*newRight).getFreq();
            newNode.reset(new HuffmanNode('\0',sum)); //Empty char and summed freq
            newNode->left=newLeft; //add pointers
            newNode->right=newRight;
            pq.push(newNode); //merge into prio queue
        }
        return newNode; //return the root node
}
void HuffmanTree::codeTable(std::shared_ptr<HuffmanNode> rootNode, string str) 
{
        if (rootNode -> left != nullptr)  //if left not null 
        {
            codeTable(rootNode -> left, str + "0"); //add 0  if left
        }

        //Set the code table with key and bit string
        this -> CodeTable.insert(
            {
                (*rootNode).getKey(), str
            }); 

        if (rootNode -> right != nullptr)     //if right not null
        {
            codeTable(rootNode -> right, str + "1"); //add 1 if right
        }
}
string HuffmanTree::compress(std::string s) 
{
    string compressedString = "";
    char * lArray = (char *) s.c_str(); //Split into letter array
    for (size_t i = 0; i < s.length(); ++i) 
    {
        compressedString += CodeTable.at( lArray[ i ] ); //Add code value of that letter
    }
    return compressedString; //return full coded string
}
void HuffmanTree::writeCTToFile(unordered_map<char, int> & map, const string & name, const unordered_map<char,string> & CodeTable) const 
{
        ofstream file;
        file.open(name);
        file << map.size() << "\n"; //Header field

        for (unordered_map<char,int>::iterator i = map.begin(); i != map.end(); ++i) 
        {
            file << i -> first << " " << CodeTable.at( i->first ) << "\n";
        }
        file.close();
}
void HuffmanTree::writeToFile(const string & cStr,const string & out) const 
{
        char * size = (char *) cStr.c_str();
        ofstream outFile(out, ios::binary);
        outFile.write(size, cStr.length());
        outFile.close();
}
/*
void HuffmanTree::writeUMap(unordered_map<char, int> & map, const string & out) const
{
    ofstream outFile("uMap.txt"+out, ios::binary);
    for (unordered_map<char,int>::iterator i = map.begin(); i != map.end(); ++i)
    {
        outFile.write(map.at(i),map.size());
    }
    outFile.close();
}

int HuffmanTree::bitStream(string & cStr,const string & out)
{
    ofstream output("Bitstream.txt", ios::binary);
    int bits = cStr.size();
    int bitStreamSize = (bits / 8) + (bits % 8 ? 1 : 0);
    output.write((const char *) & bits,sizeof(int));
    output.write((const char *) & "\n",sizeof("\n"));
    output.close();
    return bitStreamSize;
}
*/
}



