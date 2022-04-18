#include"../include/main.h"
#include"../include/algorithm.h"
#include"../include/read.h"
#include<sstream>

using namespace std;

/*
    input solution in check.txt
    input data name in input_filename_for_check
*/

string filename = "check.txt";
string input_filename_for_check = "up.txt";

//extern string input_filename;
extern int itemnum, elementnum;
extern vector<Item> data;
extern vector< vector<bool> > conflict_graph;
extern vector<int> conflict_num;       //the number of items it conflicts with
unordered_set<int> test_solution;
extern unordered_map<int, int> element_cover_times;    //solution covers elements times

void str_to_int(string s){
    stringstream ssin(s);
    int item;
    while( ssin >> item )
        test_solution.insert(item);
}

bool check_conflict(){
    bool is_conflict = false;
    for( auto iti=test_solution.begin(); iti!=test_solution.end(); iti++ ){
        for( auto itj=iti; itj!=test_solution.end(); itj++ ){
            if( conflict_graph[*iti][*itj] ){
                is_conflict = true;
                cout << *iti << " " << *itj << endl;
                return is_conflict;
            }
        }
    }
    return is_conflict;
}

int check_cover(){
    for( int i=1; i<=elementnum; i++ )
        element_cover_times[i] = 0;
    for( auto it=test_solution.begin(); it!=test_solution.end(); it++ ){
        update_element_cover_times(true, *it, element_cover_times);
    }
    return get_cover_num(element_cover_times);
}

void check(){
    //check the solution is right or not

    data = read_data(input_filename_for_check);        //input conflict graph

    ifstream infile;
    infile.open(filename, ios::in);

    if( !infile.is_open() ){
        cout << "read file error" << endl;
        cout << "please check your filename" << endl;
        cout << "i'm check" << endl;
        return ;
    }

    string buf;
    while( getline(infile, buf) )
        str_to_int(buf);

    if( check_conflict() ){
        cout << "conflict" << endl;
    }
    cout << check_cover() << endl;
}