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
	unsigned int bfChanges = 0;
	unsigned int ptrChanges = 0;
	unsigned int keyComparisons = 0;
	unsigned int noRotNeeded = 0;
	unsigned int llRot = 0;
	unsigned int lrRot = 0;
	unsigned int rlRot = 0;
	unsigned int rrRot = 0;
	clock_t startTime;

	void traverse(int& index, node* n);

	void deleteNode(node* n);

	void print2DUtil(node* start, int space);
};