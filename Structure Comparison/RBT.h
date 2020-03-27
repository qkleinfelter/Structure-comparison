#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
#define COUNT 10
using namespace std;
class RBT
{
public:
	RBT();
	~RBT();
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
		bool color = RED; // true is black, false is red
	};

	const static bool BLACK = true;
	const static bool RED = false;

	node* nil = nullptr;
	node* root = nullptr;
	unsigned long long recolorings = 0;
	unsigned long long ptrChanges = 0;
	unsigned long long keyComparisons = 0;
	unsigned long long noFixesNeeded = 0;
	unsigned long long case1Fix = 0;
	unsigned long long case2Fix = 0;
	unsigned long long case3Fix = 0;
	clock_t startTime;

	void traverse(int& index, node* n);

	void deleteNode(node* n);

	void leftRotation(node* x);
	void rightRotation(node* x);
	void insertFixup(node* z);
	void print2DUtil(node* start, int space);
};