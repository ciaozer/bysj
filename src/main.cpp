#include"../include/main.h"
#include"../include/find_filename_linux.h"
#include"../include/read.h"
#include"../include/algorithm.h"
#include"../include/check.h"
#include"../include/generate.h"
#include"../include/preprocess.h"
//the order is important

using namespace std;

#define RUN 3
//0 represents generating data
//1 represents running data
//2 represents others

#define GENERATE 3
//0 represents using ORLibrary
//1 represents generating random data
//2 represents generating added data

int itemnum, elementnum;
int best_cover_num;

unordered_map<int, Item> data;
vector< vector<bool> > conflict_graph;
unordered_set<int> candidate;   //items that not in solution and not conflicts with the items in solution
unordered_set<int> solution;    //current result, the items that have been selected
unordered_set<int> best_solution;
unordered_map<int, int> element_cover_times;    //solution covers elements times
vector<int> uncovered;          //elements that uncovered by solution
unordered_map<int, unordered_set<int> > N_ele;     //items that can cover element j
unordered_map<int, unordered_set<int> > M_item;    //elements that covered by item i
unordered_map<int, unordered_set<int> > G_item;    //items that conflict with item i

int main(){
    //ensure every time the result is different
    // int randnum;
    // cin >> randnum;
    // srand(randnum);

    srand(time(NULL));

    if( RUN == 0 ){
        generate(GENERATE);
    }
    else if( RUN == 1 ){
        vector<string> new_filenames = find_filename_linux("data/OR_data");
        for( int i=0; i<new_filenames.size(); i++ ){
            cout << "executing " << new_filenames[i] << endl;
            new_filenames[i] = "data/OR_data/" + new_filenames[i];
            run(new_filenames[i]);
            cout << "the solution size is: " << solution.size() << endl;
            print_solution(solution);
        }    
    }
    else if( RUN == 2 ){
        string path = "data/random_data/test.txt";
        run(path);
        print_solution(solution);
        //now the element cover times is right

        path = "data/add/test.txt";
        //while( get_cover_num(element_cover_times) != elementnum ){
            generate_add_data_once(path);
            run(path);
            print_solution(solution);
        //}
    }
    else{
        //check();
        // run("data/add/test.txt");
        // generate_random_once("up.txt");
        run("up.txt");
        //preprocess();
        //generate(GENERATE);
    }

//    system("pause");
    return 0;
}