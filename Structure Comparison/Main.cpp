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
    
    tree->insert("mar");
    tree->insert("may");
    tree->insert("nov");
    tree->insert("aug");
    tree->insert("apr");
    tree->insert("jan");
    tree->insert("dec");
    tree->insert("jul");
    tree->insert("feb");
    tree->insert("jun");
    tree->insert("oct");
    tree->insert("sep");

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
