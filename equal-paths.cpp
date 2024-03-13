#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int treeDepth(Node* root) {
    if(root == nullptr) {
        return 0;
    }
    else {
        int left_d = treeDepth(root->left);
        int right_d = treeDepth(root->right);
        int maxDepth = (left_d > right_d) ? left_d : right_d;

        return 1 + maxDepth;
    }
}

bool equalPaths(Node * root)
{

    if(root == nullptr) {
        return true;
    }
    
    int left_d = treeDepth(root->left);
    int right_d = treeDepth(root->right);

    if(root->left != nullptr && root->right != nullptr) {
        if (left_d != right_d) {
            return false;
        }
        else {
            return equalPaths(root->left) && equalPaths(root->right);
        }
    }
    
    if (root->left != nullptr) {
        return equalPaths(root->left);
    }
    if (root->right != nullptr) {
        return equalPaths(root->right);
    }

    return true;

}

