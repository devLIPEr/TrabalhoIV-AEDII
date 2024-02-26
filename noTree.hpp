struct noTree{
    char character;
    unsigned int count;
    noTree *left;
    noTree *right;

    noTree(char character, unsigned int count, noTree *left, noTree *right){
        this->character = character;
        this->count = count;
        this->left = left;
        this->right = right;
    }
    noTree(char character, unsigned int count){
        this->character = character;
        this->count = count;
        this->left = NULL;
        this->right = NULL;
    }

    void addCount(){
        this->count++;
    }

    bool operator > (const noTree& other) const{
        return count > other.count;
    }
};

// https://stackoverflow.com/questions/2439283/how-can-i-create-min-stl-priority-queue
struct CompareNoTree {
    bool operator()(const noTree* l, const noTree* r){
        return l->count > r->count;
    }
};