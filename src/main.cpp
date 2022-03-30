#include"../include/main.h"
#include"../include/find_filename_linux.h"
#include"../include/read.h"
#include"../include/algorithm.h"
#include"../include/check.h"
#include"../include/generate.h"
//the order is important

using namespace std;

#define RUN 2
//0 represents generating data
//1 represents running data
//2 represents generateing add data
//3 represents others

#define GENERATE 2
//0 represents using ORLibrary
//1 represents generating random data

int itemnum, elementnum;
int best_cover_num;

vector<Item> data;
vector< vector<bool> > conflict_graph;
vector<int> conflict_num;       //the number of items it conflicts with
vector<int> candidate_conflict_num; //conflict times between candidates
vector<int> cover_of_candidate; //how many elements a candidate can cover, 0 if not in candidate
unordered_set<int> candidate;   //items that not in solution and not conflicts with the items in solution
unordered_set<int> solution;    //current result, the items that have been selected
unordered_set<int> best_solution;
unordered_map<int, int> element_cover_times;    //solution covers elements times
vector<int> tabulist;           //the list of item tabu
vector<int> stay;               //how many turns an item is in the solution
vector<int> uncovered;          //elements that uncovered by solution

int main(){
    //ensure every time the result is different
    // int randnum;
    // cin >> randnum;
    // srand(randnum);

    //srand(time(NULL));

    if( RUN == 0 ){
        generate(GENERATE);
    }
    else if( RUN == 1 ){
        vector<string> new_filenames = find_filename_linux("data/OR_data");
        for( int i=0; i<new_filenames.size(); i++ ){
            cout << "executing " << new_filenames[i] << endl;
            new_filenames[i] = "data/OR_data/" + new_filenames[i];
            data = read_data(new_filenames[i]);
            run();
            cout << "the solution size is: " << solution.size() << endl;

            //output the sorted solution
            vector<int> sorted_solution;
            for( auto it = solution.begin(); it!=solution.end(); it++ )
                sorted_solution.push_back(*it);
            sort(sorted_solution.begin(), sorted_solution.end());
            for( int i=0; i<sorted_solution.size(); i++ ){
                cout << sorted_solution[i] << " ";
                if( i%10 == 9 )
                    cout << endl;
            }
            cout << endl;
        }    
    }
    else if( RUN == 2 ){
        for( int i=0; i<10; i++ ){
            //run random data
            string path = "data/random_data/random";
            path = path + to_string(i) + ".txt";
            data = read_data(path);
            run();
            generate(GENERATE);
            uncovered.clear();

            //repair it
            path = "data/add/add";
            path = path + to_string(i) + ".txt";
            while( get_cover_num(element_cover_times) == elementnum ){
                data = read_data(path);
                run();
                uncovered.clear();
                cout << "i'm here" << i << endl;
            }
        }
    }
    else{

    }

//    system("pause");
    return 0;
}