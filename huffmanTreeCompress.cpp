#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "noTree.hpp"

using namespace std;

void deleteTree(noTree *T){
    if(T != NULL){
        deleteTree(T->left);
        deleteTree(T->right);
        delete T;
        T = NULL;
    } 
}

int itemOnTree(noTree *T, char character){
    if(T != NULL){
        if(T->character != character){
            return itemOnTree(T->left, character) || itemOnTree(T->right, character);
        }
        return 1;
    }
    return 0;
}

int findItem(noTree *T, char character, vector<bool> *compressedStr){
    if(T != NULL){
        if(T->character != character){
            int l = itemOnTree(T->left, character);
            if(l){
                compressedStr->push_back(0);
                findItem(T->left, character, compressedStr);
                return 1;
            }
            int r = itemOnTree(T->right, character);
            if(r){
                compressedStr->push_back(1);
                findItem(T->right, character, compressedStr);
                return 1;
            }
        }else{
            return 1;
        }
    }
    return 0;
}

void writeTree(noTree* T, vector<bool>& bitStream){
    if(T != NULL){
        if(T->left == NULL && T->right == NULL){
            bitStream.push_back(1);
            for (int i = 0; i < 8; i++){
                bitStream.push_back(T->character & (1 << (7-i)));
            }
        }else{
            bitStream.push_back(0);
            writeTree(T->left, bitStream);
            writeTree(T->right, bitStream);
        }
    }
}

void writeBitsToFile(string fileName, vector<bool>& bits){
    ofstream file(fileName, ios::binary);

    bits.push_back(1);

    while(bits.size() % 8){
        bits.push_back(0);
    }

    for(size_t i = 0; i < bits.size(); i += 8){
        char byte = 0;
        for(int j = 0; j < 8 && i + j < bits.size(); ++j){
            byte = (byte << 1) | bits[i + j];
        }
        file.write(&byte, sizeof(byte));
    }
}


int main(){
    priority_queue<noTree*, vector<noTree*>, CompareNoTree> pQ = priority_queue<noTree*, vector<noTree*>, CompareNoTree>();
    map<char, noTree*> charMap = map<char, noTree*>();

    ifstream fileToCompress("fileToCompress.txt");

    // Get all lines from file
    string strToCompress, line;
    while(getline(fileToCompress, line)){
        strToCompress += line + '\n';
    }
    strToCompress.erase(strToCompress.length()-1);
    int strLen = strToCompress.length();

    // Count all ocurrences of letters
    for(int i = 0; i < strLen; i++){
        if(charMap.find(strToCompress[i]) != charMap.end()){
            charMap.at(strToCompress[i])->addCount();
        }else{
            charMap.insert(make_pair(strToCompress[i], new noTree(strToCompress[i], 1)));
        }
    }

    // Loop through map putting noTrees on priority_queue
    for(const auto& par : charMap){
        pQ.push(par.second);
    }

    // Create Huffman tree 
    while(pQ.size() > 1){
        noTree *f1 = pQ.top();
        pQ.pop();

        noTree *f2 = pQ.top();
        pQ.pop();

        noTree *newNo = new noTree('\0', f1->count+f2->count, f1, f2);
        pQ.push(newNo);
    }

    noTree *resultingTree = pQ.top();
    pQ.pop();

    // Compress characters
    vector<bool> *compressedVector = new vector<bool>();
    for(int i = 0; i < strLen; i++){
        findItem(resultingTree, strToCompress[i], compressedVector);
    }

    vector<bool> treeBits;
    writeTree(resultingTree, treeBits);

    writeBitsToFile("huffmanTree.bin", treeBits);
    writeBitsToFile("compressedFile.bin", *compressedVector);

    deleteTree(resultingTree);
    delete compressedVector;

    return 0;
}