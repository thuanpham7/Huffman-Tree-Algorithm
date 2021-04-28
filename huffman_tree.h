#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class huffman_tree {
private:
    //Create an object that store character and its frequencies
    struct Obj {
        char c;
        int freq;
    };

    //Create a treenode that represents object value - each node contains value's data (char c, frequecies)
    struct TreeNode {
        Obj value;
        //A treenode has 2 children left and right
        TreeNode* left;
        TreeNode* right;
        //Constructor that initializes treenode
        TreeNode() { right = nullptr; left = nullptr; }
        //Constructor that initializes treenode passing its children data and its new value's data
        TreeNode(TreeNode* leftNode, TreeNode* rightNode, char c, int freq){
          right = leftNode;
          left = rightNode;
          value.c = c;
          value.freq = freq;
        }
    };
    //priority queue comparator passing 2 objects (TreeNode in this case)
    //In this case would represent min heap
    struct Comparator {
	      bool operator () (TreeNode* left, TreeNode* right) {
		        return left->value.freq > right->value.freq;      //compare passing object's frequencies (value)
	      }
    };
    unordered_map<char, int> m;
    //Initalizes priority queue passing Comparator function and TreeNode as object
    priority_queue<TreeNode*, vector<TreeNode*>, Comparator> pq;
    queue<TreeNode*> myQueue;

public:

    //function that checks frequencies of the specific character in the string
    void checkFreq(string input){
        for (char& c : input){
            //if map already has that character, its frequencies will increase
            //else set it to 1
            if (m.count(c))
                m[c]++;     //O(1) time using map implementation
            else m[c] = 1;  //O(1) time
        }
    }
    //-----> Time complexity: O(n). Traverse the whole string to get each character
    //O(n) for best, average, worst case. n is the length of string input

    //creating huffman tree function
    huffman_tree(const string& input) {
        checkFreq(input);   //Recall checkFreq function to get character frequencies

        //Loop through the map
        for (auto iter = m.begin(); iter != m.end(); iter++)
            //push each character and its frequency to priority queue ADT as TreeNode
            //Pass map's key as object c and map's value as object int
            //Set each TreeNode as root itself without children
            pq.push(new TreeNode(nullptr, nullptr, iter->first, iter->second)); //O(1)
        //------>Time complexity: O(n) - best, average, worst case - Traverse the whole map
        //n is size of map m

        //Temporary priority queue to support queue that accepts last node as last index
        priority_queue<TreeNode*, vector<TreeNode*>, Comparator> tempo;

        //Deep copy from pq to normal queue
        while (!pq.empty()){
            //O(1)
            tempo.push(pq.top());
            myQueue.push(pq.top());
            pq.pop();
        }
        pq = tempo;
        //Time complexity: O(n), best, average, worst

        while (pq.size() > 1){    //Set greater than 1 because last node will be left most child which is a leaf node
            //Remove 2 treenodes then set their root as their sum treenode using queue
            //Priority queue and queue implementation cost O(1) time
            TreeNode* r = pq.top();
            pq.pop();
            myQueue.pop();
            TreeNode* l = pq.top();
            pq.pop();
            myQueue.pop();
            //After removing, makes a new treenode with root is the sum of the 2 treenodes
            //And the 2 treenodes will be root's children
            //Sum node does not have any character representation
            pq.push(new TreeNode(l, r, '\0', r->value.freq + l->value.freq));
            //Using queue for properly pushing treenode in order
            myQueue.push(new TreeNode(l, r, '\0', r->value.freq + l->value.freq));
        }
    }
    //--->Time complexity:

    //encode method function
    void encodeMethod(unordered_map<char, string>& encodeMap, TreeNode* root, string input) const {
        //Return if queue the queue has been traversed all values
        if (root == nullptr) return;    //Base case

        //Traverse treenode till it reach leaf node - Obj value (char and its frequencies)
        //Otherwise skip when it has children (sum of the 2)
        if (root->left == nullptr && root->right == nullptr){
            //Case when the input string has only 1 unique character, return '0'
            if (input.length() == 0)
                encodeMap[root->value.c] = "0";     //O(1)
            else encodeMap[root->value.c] = input;  //input string follows the path when doing preorder traversal
        }

        //preorder traversal which input follows the path '0' when traverse through left and '1' when traverse through right
        encodeMethod(encodeMap, root->left, input + "0");
        encodeMethod(encodeMap, root->right, input + "1");
    }
    //Time complexity: O(n) for all best, average, worst case since traverse through all nodes of the treenode
    //where n is number of nodes

    //get character code function
    string get_character_code(char character) const {
        unordered_map<char, string> temp;
        //encode using huffman tree algorithm again to store character code in the temporary map
        //temporary map with key of character and value as string of character code
        encodeMethod(temp, myQueue.front(), "");
        //Case when the character not in the temporary map - should return empty string
        if (!temp.count(character))
            return "";
        return temp[character];
    }
    //Time complexity O(n) for best, average, and worst since traversing
    //the tree using encodeMethod

    //Function adding up all encode character
    void getStringValue(unordered_map<char, string> encodeMap, string input, string& output) const {
        for (char& c : input)
           output += encodeMap[c];
    }
    //----->Time complexity: O(n) - traversing the whole input string

    //Encode function
    string encode(const string& input) const {
        string result = "";
        //Case when input an empty string would return empty string
        if (m.size() == 0)
            return result;

        //Case when string input contain characters that not in the huffman tree
        //Checking using map
        for (int i = 0; i < input.length(); i++){
            if (!m.count(input.at(i)))
                return result;
        }
        //cost O(n) when n = length of string input

        //Initialize root as root node of the tree then traverse the whole tree Using
        //encodeMethod function
         TreeNode* root = myQueue.front();
         string emptyString = "";
         unordered_map<char, string> encodeMap;
         encodeMethod(encodeMap, root, emptyString);
         getStringValue(encodeMap, input, result);
         return result;
    }
    //---->Time complexity: O(n+m) for best, average and worst case since we have to
    //traverse the whole tree(n nodes) and the input string (m length)

    //Decode method function
    void decodeMethod(vector<char>& v, int& value, TreeNode* root, string input, int index) const{
        if (root == nullptr) return;  //base case

        //When it reach leaf node which represent the object node (character and its frequecies)
        if (root->left == nullptr && root->right == nullptr){
            //Adding the TreeNode character to vector
            v.push_back(root->value.c);
            //Case the TreeNode has only 1 node (stop traversing and return in decode function)
            if (m.size() == 1)
                value = index+1;
            return;
        }
        value++; //Using recursive that traversing both input string and huffman tree
        try {
          if (input.at(value) == '1')   //When the string index = 1, traverse to the right, stop when it reach leaf node
              decodeMethod(v, value, root->right, input, index);
          else if (input.at(value) == '0')  //When the string index = 0, traverse to the left, stop when it reach leaf node
              decodeMethod(v, value, root->left, input, index);
          //Case when input have different letters beside 0 and 1. In this case return empty string
          //by clearing out the vector
          else {
              v.clear();
              value = index+1;
              return;
          }
        }
        //Exception is thrown when input string have invalid encode string that is not in the map
        catch (exception& e){
              v.clear();
        }
    }
    //-----Time complexity: O(n) for best, average, and worst since traversing the whole tree

    //decode function
    string decode(const string& string_to_decode) const {
        string result = "";
        //Case when input is an empty string, decode would be empty string
        if (m.size() == 0)
            return result;
        //Initialize 'to' value when looping through the string_to_decode string
        //-2 for not index out of range when using decodeMethod recursively since value++ is before recursive is called
        int stringLength = string_to_decode.length()-2;
        //-1 for checking the Treenode's root
        int value = -1;
        TreeNode* root = myQueue.front();
        vector<char> v;
        //Stop when value = stringLength - traversing the whole tree
        while (value <= stringLength)
            decodeMethod(v, value, root, string_to_decode, stringLength);

        for(int i = 0; i < v.size(); i++)
            result += v[i];

        return result;
    }
    //----->Time complexity: O(n+m) for best, average and worst
    //n for number of nodes in the TreeNode and m is the length of string input

};
