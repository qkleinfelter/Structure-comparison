#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
using namespace std;
class RBT
{
public:
	RBT();
	~RBT();
	void insert(const char word[50]);
	void list();
private:
	struct node
	{
		char word[50];
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
		node* parent = nullptr;
		bool color; // true is black, false is red
	};

	const static bool BLACK = true;
	const static bool RED = false;

	node* nil = nullptr;
	node* root = nullptr;

	void traverse(int& index, node* n);

	void deleteNode(node* n);

	void leftRotation(node* x);
	void rightRotation(node* x);
};