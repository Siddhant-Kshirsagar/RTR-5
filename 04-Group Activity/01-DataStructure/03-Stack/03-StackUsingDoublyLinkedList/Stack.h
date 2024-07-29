#pragma once
#include<stdio.h>
#include<stdlib.h>

struct NODE
{
	int data;
	struct NODE *prev;
	struct NODE *next;
};

typedef struct NODE Data_Node;

Data_Node *createNode(int);

Data_Node *insertData(Data_Node*,int);

Data_Node *deleteData(Data_Node *);

void displayList(Data_Node *);
