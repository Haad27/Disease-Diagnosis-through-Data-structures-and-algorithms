#include<iostream>
#include<vector>
using namespace std;

// used to loads all data from files into linkedlists each node keeping a statmemt
class Node{
    public:
    string term;
    Node* next;
    Node(string temp){
        term = temp;
        next = nullptr;
    }
};

// disease has diff categories and those categories has many diseases whose vectors are empty.
// might implement a dfs for finding category of speciffic disease.  or just simply traverse each array of category to find the disease(but i can find category through disease anyway , its for printing whole category)
class CategoryTree{
    public:
    string name;
    vector<CategoryTree*> children;
    
};


// holds all data about a disease
class DiseaseNode{
    Node* symptomHead;
    Node* preventionHead;
    Node* treatmentHead;
    string category;
    DiseaseNode(){
        symptomHead=nullptr;
        preventionHead=nullptr;
        treatmentHead=nullptr;
    }
};
// heads to be used again and again for each disease, once assigns the head to DiseaseNode than can be used for other disease
Node* symptomHead=nullptr;
Node* preventionHead=nullptr;
Node* treatmentHead=nullptr;

void loadData(){
    
}



int main(){



    return 0;
}