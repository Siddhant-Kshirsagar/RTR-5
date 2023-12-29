#include<stdio.h>
#include<stdlib.h>

struct TreeNode
{
	struct TreeNode *leftChild;
	int data;
	struct TreeNode *rightChild;
};

struct TreeNode *insertNode(struct TreeNode *root, const int value);

void display_PreOrder(struct TreeNode *root);
void display_PostOrder(struct TreeNode *root);
void display_InOrder(struct TreeNode *root);

int main(void)
{
	//variable declaration
	struct TreeNode* root = NULL;
	int values[] = { 50,100,10,5,15,115,90 };
	int length = (sizeof(values) / sizeof(values[0]));

	for (int i = 0; i < length; i++)
	{
		root = insertNode(root, values[i]);
	}

	printf("\n\tDisplay Pre-Order\n");
	display_PreOrder(root);

	printf("\n\tDisplay Post-Order\n");
	display_PostOrder(root);

	printf("\n\tDisplay In-Order\n");
	display_InOrder(root);

	return(0);
}

struct TreeNode *insertNode(struct TreeNode *root, const int value)
{
	// if root is null means tree is not present create root node and assigned data
	if (root == NULL)
	{
		root = (struct TreeNode *)malloc(sizeof(struct TreeNode));

		root->data = value;
		root->leftChild = NULL;
		root->rightChild = NULL;

		return(root);
	}


	// variable declaration for new data insert
	struct TreeNode *temp = root;	

	// ***** create new node to insert new data in tree
	struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	//fill the data
	newNode->data = value;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	// insert data according to value of new inserted data and root  

	while (1)
	{
		if (temp->data < value)
		{
			// for insert data right side of root(because value of new insert data is greater than root)
			if (temp->rightChild == NULL)
			{
				temp->rightChild = newNode;
				break;
			}
			else
			{
				temp = temp->rightChild;
			}
		}
		else
		{
			// for insert data left side of root(because value of new insert data is less than root)
			if (temp->leftChild == NULL)
			{
				temp->leftChild = newNode;
				break;
			}
			else
			{
				temp = temp->leftChild;
			}
		}
	}

	return(root);
}


void display_InOrder(struct TreeNode *root)
{
	// null check
	if (root == NULL)
	{
		return;
	}
	// first go left than print data then go right( In - Order traversal - LDR method )
	display_InOrder(root->leftChild);
	printf("\t%d", root->data);
	display_InOrder(root->rightChild);
}

void display_PreOrder(struct TreeNode *root)
{
	// null check
	if (root == NULL)
	{
		return;
	}

	// first print data then go left than go right( Pre - Order traversal - DLR method)
	printf("\t%d", root->data);
	display_PreOrder(root->leftChild);
	display_PreOrder(root->rightChild);
}

void display_PostOrder(struct TreeNode *root)
{
	// null check
	if (root == NULL)
	{
		return;
	}

	// first go left than go right than print data( Post - Order traversal - LRD method)
	display_PostOrder(root->leftChild);
	display_PostOrder(root->rightChild);
	printf("\t%d", root->data);
}

