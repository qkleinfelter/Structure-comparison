#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#define COUNT 10
using namespace std;
class AVL
{
public:
	AVL();
	~AVL();
	void insert(const char word[50]);
	void list();
	void print2D();
private:
	struct node
	{
		char word[50];
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
		node* parent = nullptr;
		int balanceFactor = 0;
	};

	node* nil = nullptr;
	node* root = nullptr;

	void traverse(int& index, node* n);

	void deleteNode(node* n);

	void leftRotation(node* x);
	void rightRotation(node* x);
	void insertFixup(node* z);
	void print2DUtil(node* start, int space);
};