/*
	Skiplist.h
	Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 3/28/2020
	Header for Skiplist.cpp
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
#include <random>
using namespace std;
class Skiplist
{
public:
	Skiplist();
	~Skiplist();
	void insert(const char word[50]);
	void list();
	void stackedList();
	void displayStatistics();
private:
	struct node
	{
		char word[50];
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
		node* up = nullptr;
		node* down = nullptr;
		bool isSentinel = false;
	};

	mt19937 coin;
	node* head = nullptr;
	node* tail = nullptr;
	int height;
	int numItems;
	unsigned long long ptrChanges = 0;
	unsigned long long keyComparisons = 0;
	unsigned long long coinTosses = 0;
	clock_t startTime;

	node* find(const char word[50], bool& found);

	node* createSentinelNode();

	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords);
	unsigned long long countTotalNodes();
};