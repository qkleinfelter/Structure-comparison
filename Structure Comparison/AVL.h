#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
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
	unsigned long long getHeight();
	void displayStatistics();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
		node* parent = nullptr;
		int balanceFactor = 0;
	};

	node* root = nullptr;
	unsigned long long bfChanges = 0;
	unsigned long long ptrChanges = 0;
	unsigned long long keyComparisons = 0;
	unsigned long long noRotNeeded = 0;
	unsigned long long llRot = 0;
	unsigned long long lrRot = 0;
	unsigned long long rlRot = 0;
	unsigned long long rrRot = 0;
	unsigned long long treeHeight = 0;

	clock_t startTime;

	void traverse(int& index, node* n);

	void deleteNode(node* n);

	void print2DUtil(node* start, int space);

	void calculateHeight(node* currNode, unsigned long long pathHeight);
	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords);
	void calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords);
};