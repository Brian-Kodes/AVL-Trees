#include "equal-paths.h"
#include <algorithm>
using namespace std;



// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    //Add your code below
    
    if (root == NULL)
    {
        return true;
    }
    if (root -> right != NULL && root -> left != NULL)
    {
        int leftSide = p_Length(root -> left);
        int rightSide = p_Length(root -> right);
        if (leftSide == rightSide)
        {
            return true;
        }
    }
    if (root->left != NULL && root -> right == NULL) //left child is not null
    {
        return equalPaths(root -> left);
    }
    if (root->right != NULL && root -> left == NULL)//right child is not null
    {
        return equalPaths(root -> right);
    }
    if (root -> left == NULL && root -> right == NULL)
    {
        return true;
    }
  return false;

}

int p_Length(Node * child)
{
    //Node* length = root;
    if (child == NULL)
    {
        return 0;
    }


    int lLength = p_Length(child -> left);
    int rLength = p_Length (child -> right);
    return max(lLength + 1, rLength + 1);
}

