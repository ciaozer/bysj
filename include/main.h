#include<iostream>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<fstream>
#include<string>
#include<ctime>
#include<algorithm>
#include<map>
using namespace std;

struct Item{
    int covernum;                   //the size of the elements it can cover
    int conflict_num;               //the number of items it conflicts with
    int candidate_conflict_num;     //conflict times between candidates
    int cover_of_candidate;         //how many elements a candidate can cover, 0 if not in candidate
    int tabulist;                   //the list of item tabu
    int stay;                       //how many turns an item is in the solution
    unordered_set<int> elements;    //which elements it can cover
};
