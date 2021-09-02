#include <iostream> //for cout,
#include <string>
#include "Header.h"
#include "catch.hpp"
using namespace std;

int main(int argc, char *argv[])
{ //main method
    if (argc != 3)
    {
        cout<<"incorrect arguments."<<"\n";
    }
    else
    {
    GRLMAT001::HuffmanTree d;      //creating class object
    string in = d.read(string(argv[1]));
    string out = string(argv[2]);
    GRLMAT001::HuffmanTree tree = GRLMAT001::HuffmanTree(in,out);
    }

    return 1;
}