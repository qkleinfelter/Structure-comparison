#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
using namespace std;
class BST
{
public:
	BST();
	~BST();
	void insert(const char word[50]);
	void list();
	void displayStatistics();
private:
	struct node
	{
		char word[50];
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
	};
	
	node* root = nullptr;
	unsigned int ptrChanges = 0;
	unsigned int keyComparisons = 0;
	clock_t startTime;

	void traverse(int& index, node* n);

	void deleteNode(node* n);
};