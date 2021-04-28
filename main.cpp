#include <iostream>
#include "huffman_tree.h"
#include <string>
#include <fstream>
#include <set>
using namespace std;

int main() {
  set<char> temp;
  ifstream file("test.txt");
  string value = "";
  if (file.is_open()){
      string line;
      while (getline(file, line)){
          value += line;
      }
  }
  string value2 = "describing an experience that either";


  huffman_tree myTree(value);
  cout << myTree.encode(value2) << endl;
  cout << myTree.decode(myTree.encode(value2)) << endl;
  cout << bool(myTree.decode(myTree.encode(value2))==value2) << endl;

  
  for (char& c : value2){
      if (!temp.count(c)){
          cout << c << " " << myTree.get_character_code(c) << endl;
          temp.insert(c);
        }
  }

  ofstream output("encode.txt");
  if (output.is_open())
      output << myTree.encode(value2);
  ofstream outputTwo("decode.txt");
  if (outputTwo.is_open())
      outputTwo << myTree.decode(myTree.encode(value2));
  cout << myTree.decode(myTree.encode(value2)).length() << endl;
  return 0;
}
