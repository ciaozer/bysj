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

#define GENERATE 2
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
unordered_map<int, int> weight; //element is key
bool is_weight = false;       //whether the example is weighted
int best_score;
bool have_solution = true;

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
        vector<string> new_filenames = find_filename_linux("data/random_data");
        for( int i=0; i<new_filenames.size(); i++ ){
            cout << "executing " << new_filenames[i] << endl;
            new_filenames[i] = "data/random_data/" + new_filenames[i];
            run(new_filenames[i]);
            int total = get_total_score();
            cout << "total score is " << total << endl;
            cout << "loss is " << total-best_score << endl << endl;
        }    
    }
    else if( RUN == 2 ){
        string path = "data/add/test.txt";
        //while( get_cover_num(element_cover_times) != elementnum ){
            generate_add_data_once(path);
            run(path);
        //}
    }
    else{
        //check();
        // generate_random_once("up.txt");
        // run("up.txt");
        // generate_add_data_once("up.txt");
        // run("up.txt");
        //generate_random_once("data/random_data/test.txt");
        //run("data/random_data/random3.txt");
        //preprocess();
        //generate(GENERATE);
        for( int i=0; i<10; i++ ){
            string input_path = "data/random_data/random";
            input_path = input_path + to_string(i) + ".txt";
            string output_path = "data/maxsat100/maxsat";
            output_path = output_path + to_string(i) + ".txt";
            change_to_maxsat(input_path, output_path);
        }
    }

//    system("pause");
    return 0;
}