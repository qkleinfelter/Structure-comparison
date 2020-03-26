// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BST.h"
#include "Skiplist.h"
#include "RBT.h"
#include "AVL.h"

int main()
{
   
    AVL* tree = new AVL();
    
    tree->insert("j");
    tree->insert("d");
    tree->insert("n");
    tree->insert("l");
    tree->insert("o");
    tree->insert("k");

    //tree->list();
    /*
    tree->insert("b");
    tree->insert("a");
    tree->insert("c");
    tree->insert("d");
    tree->insert("e");
    tree->insert("f");
    tree->insert("f");
    tree->insert("q");
    tree->insert("z");
    tree->insert("k");
    tree->insert("k");
    tree->insert("k");
    tree->insert("v");
    tree->insert("z");
    tree->insert("u");
    tree->insert("t");
    tree->insert("h");
    tree->insert("s");
    tree->insert("y");
    tree->insert("x");
    tree->insert("w");
    tree->insert("m");
    tree->insert("p");
    tree->insert("n");
    tree->insert("l");
    tree->insert("o");
    tree->insert("z");
    tree->insert("j");
    tree->insert("s");
    tree->insert("i");
    tree->insert("r");
    tree->insert("o");
    tree->insert("g");
    */

    //tree->list();
    tree->print2D();
    //tree->stackedList();
    

    delete tree;
}
