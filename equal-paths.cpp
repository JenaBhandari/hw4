#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int nodeHeight(Node* root)
{
	if (root == nullptr)
	{
		return 0;
	}
	
	int left = nodeHeight(root->left);
	int right = nodeHeight(root->right);
	if (left > right)
	{
		return 1 + left;
	}
	else
	{
		return 1 + right;
	}
	

}


bool equalPaths(Node * root)
{

	if (root == nullptr) 
	{
    return true;
  }

  if (root->left == nullptr && root->right == nullptr) 
	{
     return true;
  }
  
	int leftHeight;
  int rightHeight;
  if (root->left != nullptr)
  {
    leftHeight = nodeHeight(root->left);
  }
  else
  {
    leftHeight = 0;
  }
    
  if (root->right != nullptr)
  {
    rightHeight = nodeHeight(root->right);
  }
  else
  {
    rightHeight = 0;
  }

	if (root->left != nullptr && root->right != nullptr)
	{
  	if (abs(leftHeight - rightHeight) != 0) 
		{
      return false;
  	}
	}

  return (equalPaths(root->left) && equalPaths(root->right));
}



