#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <fstream>
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

noTree* readTree(vector<bool>::iterator& bit, vector<bool>::iterator& end){
    if(bit == end){
        return NULL;
    }
    if(*bit++){
        char character = 0;
        for(int i = 0; i < 8; i++){
            if(bit == end){
                return NULL;
            }
            character = ((character << 1) | *bit++);
        }
        return new noTree(character, 0);
    }else{
        noTree* left = readTree(bit, end);
        noTree* right = readTree(bit, end);
        return new noTree('\0', 0, left, right);
    }
    return NULL;
}

vector<bool> readBitsFromFile(string fileName) {
    ifstream file(fileName, ios::binary);
    vector<bool> bits = vector<bool>();

    char byte;
    while(file.read(&byte, sizeof(byte))){
        for(int i = 7; i >= 0; i--){
            bits.push_back((byte >> i) & 1);
        }
    }

    int last1 = 0;
    for(int i = 0; i < bits.size(); i++){
        if(bits[i]){
            last1 = i;
        }
    }
    bits.erase(bits.begin() + last1 + 1, bits.end());

    return bits;
}


int main(){
    ofstream uncompressedFile("uncompressedFile.txt");

    string strToCompress;

    vector<bool> treeBits = readBitsFromFile("huffmanTree.bin");
    auto start = treeBits.begin();
    auto end = treeBits.end();
    noTree *huffmanTree = readTree(start, end);

    vector<bool> compressedBits = readBitsFromFile("compressedFile.bin");

    // Decompress the file
    noTree *aux;
    int idx = 0;
    while(idx < compressedBits.size()){
        aux = huffmanTree;
        while(aux != NULL && idx < compressedBits.size()){
            if(compressedBits[idx]){
                if(aux->right == NULL){
                    uncompressedFile << aux->character;
                }else{
                    idx++;
                }
                aux = aux->right;
            }else{
                if(aux->left == NULL){
                    uncompressedFile << aux->character;
                }else{
                    idx++;
                }
                aux = aux->left;
            }
        }
    }

    deleteTree(huffmanTree);

    return 0;
}