#include "../include/main.h"
#include "../include/find_filename_linux.h"
#include "../include/read.h"
#include "../include/algorithm.h"
#include "../include/check.h"
#include "../include/generate.h"
#include "../include/preprocess.h"
// the order is important

using namespace std;

#define RUN 4
// 0 represents generating data
// 1 represents running data
// 2 represents others
// 3 represents changing to sat

#define GENERATE 1
// 0 represents using ORLibrary
// 1 represents generating random data
// 2 represents generating added data

int itemnum, elementnum;
int best_cover_num;

unordered_map<int, Item> data;
vector<vector<bool>> conflict_graph;
unordered_set<int> candidate; // items that not in solution and not conflicts with the items in solution
unordered_set<int> solution;  // current result, the items that have been selected
unordered_set<int> best_solution;
unordered_map<int, int> element_cover_times;   // solution covers elements times
vector<int> uncovered;                         // elements that uncovered by solution
unordered_map<int, unordered_set<int>> N_ele;  // items that can cover element j
unordered_map<int, unordered_set<int>> M_item; // elements that covered by item i
unordered_map<int, unordered_set<int>> G_item; // items that conflict with item i
unordered_map<int, int> weight;                // element is key
bool is_weight = false;                        // whether the example is weighted
int best_score;
bool have_solution = true;

double DENSITY = 0.1;     // the density of conflict graph
double RATE = 1;          // the posibility add into solution
int AVG_COVER_TIMES = 50; // the average of element be covered times
int ITEMNUM = 2000;
int ELEMENTNUM = 10000;
string GENERATE_POSITION;

void parse_args(int argc, char *argv[])
{
    if ( argv[1][1] == 'g' )
    {
        ITEMNUM = atoi(argv[2]);
        ELEMENTNUM = atoi(argv[3]);
        DENSITY = atof(argv[4]);
        AVG_COVER_TIMES = atoi(argv[5]);
        printf("ITEMNUM = %d\nELEMENTNUM = %d\nDENSITY = %lf\nAVG_COVER_TIMES = %d\n", ITEMNUM, ELEMENTNUM, DENSITY, AVG_COVER_TIMES);
        generate_random_once(argv[6]);
    }
    else if ( argv[1][1] == 'r' )
    {
        printf("i'm -r\n");
    }
    else if ( argv[1][1] == 's' )
    {
        printf("0\n");
        change_to_sat(argv[2], argv[3], atof(argv[4]));
    }
    else
    {
        printf("unknown commend\n");
    }

    return;
}

int main(int argc, char *argv[])
{
    // ensure every time the result is different
    //  int randnum;
    //  cin >> randnum;
    //  srand(randnum);

    srand(time(NULL));

    // if (RUN == 0)
    // {
    //     generate(GENERATE);
    // }
    // else if (RUN == 1)
    // {
    //     // vector<string> new_filenames = find_filename_linux("final_data/random_data0.5");
    //     // for( int i=0; i<new_filenames.size(); i++ ){
    //     //     cout << "executing " << new_filenames[i] << endl;
    //     //     new_filenames[i] = "final_data/random_data0.5/" + new_filenames[i];
    //     //     run(new_filenames[i]);
    //     //     int total = get_total_score();
    //     //     cout << "total score is " << total << endl;
    //     //     cout << "loss is " << total-best_score << endl << endl;
    //     // }
    //     // check();
    //     for (int i = 0; i < 10; i++)
    //     {
    //         string path = "newdata/d0.3/ins";
    //         path = path + to_string(i) + ".txt";
    //         run(path);
    //         int total = get_total_score();
    //         cout << "total score is " << total << endl;
    //         cout << "loss is " << total - best_score << endl
    //              << endl;
    //     }
    // }
    // else if (RUN == 2)
    // {
    //     string path = "data/add/test.txt";
    //     // while( get_cover_num(element_cover_times) != elementnum ){
    //     for (int i = 0; i < 10; i++)
    //     {
    //         path = path + to_string(i) + ".txt";
    //         run(path);
    //     }
    //     //}
    // }
    // else if (RUN == 3)
    // {
    //     // check();
    //     //  generate_random_once("up.txt");
    //     //  run("up.txt");
    //     //  generate_add_data_once("up.txt");
    //     //  run("up.txt");
    //     //  for( int i=0; i<20; i++ ){
    //     //      string path = "data/zjz/zjz";
    //     //      path = path + to_string(i) + ".txt";
    //     //      generate_random_once("zjz.txt");
    //     //      run("zjz.txt");
    //     //      generate_add_data_once("zjz.txt");
    //     // preprocess();
    //     // generate(GENERATE);
    //     for (int i = 0; i < 10; i++)
    //     {
    //         string input_path = "newdata/d0.2/ins";
    //         input_path = input_path + to_string(i) + ".txt";
    //         string output_path = "sat2023/data/d0.2/sat";
    //         output_path = output_path + to_string(i) + ".txt";
    //         change_to_sat(input_path, output_path, 0.8);
    //     }
    //     // change_to_sat("zjz.txt", path);
    // }
    // else
    // {
    //     string input_path = "sat2023/data/test/test.txt";
    //     generate_random_once(input_path);
    //     string output_path = "sat2023/data/test/sat.txt";
    //     change_to_sat(input_path, output_path, 1);
    //     // clock_t start, finish;
    //     // start = clock();
    //     // check();
    //     // run("final_data/pre/pre1.txt");

    //     // finish = clock();
    //     // cout << "use time " << double(finish - start) / CLOCKS_PER_SEC << endl;
    // }

    // //    system("pause");
    parse_args(argc, argv);
    return 0;
}