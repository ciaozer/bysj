#include<iostream>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<fstream>
#include<string>
#include<ctime>
#include<algorithm>
using namespace std;

struct Item{
    int no;                         //item number
    int covernum;                   //the size of the elements it can cover
    //int conflict_num;               //the size of the items it conflicts with
    unordered_set<int> elements;    //which elements it can cover
};
