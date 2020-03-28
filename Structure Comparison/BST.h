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
	unsigned long long getHeight();
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
	unsigned long long ptrChanges = 0;
	unsigned long long keyComparisons = 0;
	unsigned long long treeHeight = 0;

	clock_t startTime;

	void traverse(int& index, node* n);
	void calculateHeight(node* currNode, unsigned long long pathHeight);
	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords);
	void calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords);

	void deleteNode(node* n);
};