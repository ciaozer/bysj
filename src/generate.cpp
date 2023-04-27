#include "../include/main.h"
#include "../include/read.h"
#include "../include/find_filename_linux.h"
#include "../include/algorithm.h"
#include "../include/preprocess.h"

using namespace std;

#define RAND 1000   // the precision of random number
#define ADD_TIMES 1 // how many times an item will be added
#define WEIGHT 1
/*
    0 represents no weight
    1 represents random
    2 represents some elements with very high weight
    3 represents with negative weight
*/
#define RAND_WEIGHT 100 // the max weight

extern int itemnum, elementnum;
extern unordered_map<int, Item> data;
extern vector<vector<bool>> conflict_graph;
extern unordered_map<int, int> element_cover_times;
extern vector<int> uncovered;
extern unordered_set<int> solution;

extern unordered_map<int, unordered_set<int>> N_ele;
extern unordered_map<int, unordered_set<int>> M_item;
extern unordered_map<int, unordered_set<int>> G_item;
extern unordered_set<int> pre_solution;
extern int uncover_element;
extern unordered_set<int> total_to_do_item;
extern unordered_map<int, int> weight; // element is key

extern double DENSITY;      // the density of conflict graph
extern double RATE;         // the posibility add into solution
extern int AVG_COVER_TIMES; // the average of element be covered times
extern int ITEMNUM;
extern int ELEMENTNUM;

map<int, Item> ordered_data;
map<int, int> ordered_weight;

void order_data()
{
    for (auto it = data.begin(); it != data.end(); it++)
    {
        ordered_data[it->first] = it->second;
    }
    for (auto it = weight.begin(); it != weight.end(); it++)
    {
        ordered_weight[it->first] = it->second;
    }
}

void generate_data(string filename)
{
    // generate data according to data

    order_data();

    ofstream outfile;
    outfile.open(filename, ios::out);
    if (!outfile.is_open())
    {
        cout << "read file error" << endl;
        cout << "i am generate_data";
        return;
    }

    outfile << itemnum << " " << elementnum << endl;

    // output weight
    int cnt = 0;
    for (auto it = ordered_weight.begin(); it != ordered_weight.end(); it++, cnt++)
    {
        outfile << it->second << " ";
        if (cnt % 20 == 19)
            outfile << endl;
    }
    outfile << endl;

    for (int i = 0; i < itemnum; i++)
    {
        Item itemtmp = ordered_data[i];

        // show the item number and what elements it covers
        outfile << i << " " << itemtmp.covernum << endl;
        for (auto it = itemtmp.elements.begin(); it != itemtmp.elements.end(); it++)
        {
            outfile << *it << " ";
        }
        outfile << endl;

        // show what items it conflicts with
        outfile << itemtmp.conflict_num << endl;
        for (int j = 0; j < itemnum; j++)
            if (conflict_graph[i][j])
                outfile << j << " ";
        outfile << endl;
    }
    outfile.close();
}

void generate_weight()
{
    switch (WEIGHT)
    {
    case 0:
        for (int i = 1; i <= elementnum; i++)
        {
            weight[i] = 1;
        }
        break;

    case 1:
        for (int i = 1; i <= elementnum; i++)
        {
            int rand_weight = (rand() % RAND_WEIGHT) + 1;
            weight[i] = rand_weight;
        }
        break;

    case 2:
        break;

    case 3:
        break;

    default:
        break;
    }
}

void generate_conflict_graph()
{
    conflict_graph = vector<vector<bool>>(itemnum, vector<bool>(itemnum, false));
    // if not initial, the conflict_num will lose 1, but i don't know why
    for (auto it = data.begin(); it != data.end(); it++)
    {
        it->second.conflict_num = 0;
    }
    double tmp;
    for (int i = 0; i < itemnum; i++)
        for (int j = i + 1; j < itemnum; j++)
        {
            tmp = rand() % RAND / (double)RAND;
            if (tmp < DENSITY)
            {
                conflict_graph[i][j] = true;
                conflict_graph[j][i] = true;
                data[i].conflict_num++;
                data[j].conflict_num++;
            }
        }
}

void generate_or_data(string filename)
{
    // generate data to the filename
    generate_conflict_graph();
    generate_weight();
    generate_data(filename);
}

void generate_OR()
{
    vector<string> filenames = find_filename_linux("data/init_data");
    for (int i = 0; i < filenames.size(); i++)
    {
        // cout << filenames[i] << endl;
        string filename = filenames[i];
        string newfilename = "new" + filename;
        filename = "data/init_data/" + filename;
        newfilename = "data/OR_data/" + newfilename;
        data = read_initdata(filename);
        generate_or_data(newfilename);
        cout << "finish generate " << filenames[i] << endl;
    }
}

void generate_random_once(string filename)
{
    itemnum = ITEMNUM;
    elementnum = ELEMENTNUM;
    int avg_cover_num = elementnum / itemnum * AVG_COVER_TIMES;
    int max_offset = avg_cover_num / 2;
    // cout << max_offset << endl;

    generate_weight();

    for (int i = 0; i < itemnum; i++)
    {
        Item itemtmp;

        // generate a rand_covernum
        int offset = rand() % (2 * max_offset);
        // cout << offset << " ";
        offset -= max_offset;
        int rand_covernum = avg_cover_num + offset;
        // cout << rand_covernum << endl;
        itemtmp.covernum = rand_covernum;

        // generate cover elements
        unordered_set<int> rand_elements;
        while (rand_covernum)
        {
            // element conts from 1
            int element = (rand() % elementnum) + 1;
            // element repetitiom
            if (rand_elements.count(element))
            {
                continue;
            }
            rand_covernum--;
            rand_elements.insert(element);
        }
        itemtmp.elements = rand_elements;
        data[i] = itemtmp;
    }

    // generate cover first or conflict message will lose.3
    generate_conflict_graph();
    generate_data(filename);
}

void generate_random()
{
    for (int i = 0; i < 10; i++)
    {
        string path = "newdata/d0.2/ins";
        path = path + to_string(i) + ".txt";
        generate_random_once(path);
    }
}

void get_uncovered_elements()
{
    uncovered.clear();
    for (auto it = element_cover_times.begin(); it != element_cover_times.end(); it++)
    {
        if (it->second == 0)
        {
            uncovered.push_back(it->first);
        }
    }
}

void print_uncovered()
{
    get_uncovered_elements();
    cout << "uncovered size is: " << uncovered.size() << endl;
    for (int i = 0; i < uncovered.size(); i++)
    {
        cout << uncovered[i] << " ";
    }
    cout << endl;
}

int add_into_solution(int element_idx)
{
    // return the index of the chosen item

    int idx = rand() % solution.size();
    auto it = solution.begin();
    while (idx--)
        it++;
    if (!data[*it].elements.count(element_idx))
        return *it;
    return add_into_solution(element_idx);
}

int add_into_nonsolution(int element_idx)
{
    // return the index of the chosen item

    // the item that not in solution
    vector<int> nonsolution;
    for (int i = 0; i < itemnum; i++)
    {
        if (solution.count(i) == 0)
            nonsolution.push_back(i);
    }

    int idx = rand() % nonsolution.size();
    int it = nonsolution[idx];
    if (!data[it].elements.count(element_idx))
        return it;
    return add_into_nonsolution(element_idx);
}

void add_uncovered_elements()
{
    get_uncovered_elements();
    int tmp = get_cover_num(element_cover_times);
    cout << "covered size is: " << tmp << endl;
    print_uncovered();

    // add uncovered elements to items randomly
    for (int i = 0; i < uncovered.size(); i++)
    {
        int cnt = 0; // add times
        int idx;     // the chosen item

        while (cnt < ADD_TIMES)
        {
            // must divide double or it will be 0
            double tmp = rand() / (double)RAND_MAX;
            if (tmp < RATE)
                idx = add_into_solution(uncovered[i]);
            else
                idx = add_into_nonsolution(uncovered[i]);
            cnt++;
            data[idx].covernum++;
            data[idx].elements.insert(uncovered[i]);
        }
    }
    uncovered.clear(); // ensure the uncovered is right
}

void generate_add_data_once(string filename)
{
    add_uncovered_elements();
    generate_data(filename);
    cout << "finish generate once" << endl;
}

void generate_add_data()
{
    for (int i = 0; i < 10; i++)
    {
        // string path = "data/random_data/random";
        // path = path + to_string(i) + ".txt";
        // run(path);
        generate_random_once("final_data/test.txt");
        run("final_data/test.txt");
        // now the element cover times is right

        string path = "final_data/add/add";
        path = path + to_string(i) + ".txt";
        // while( get_cover_num(element_cover_times) != elementnum ){
        generate_add_data_once(path);
        // uncovered.clear();      //ensure the uncovered is right
        // run(path);
        //}
    }
}

void generate_up()
{
    while (1)
    {
        data = read_data("up.txt");
        uncover_element = 0;
        UP();

        // if exist element can't be covered
        if (uncover_element != 0)
        {
            // add uncover_element into an item that not in to_do_item
            int tmp;
            while (1)
            {
                // can be changed, if the data structure it set
                tmp = rand() % itemnum;
                if (total_to_do_item.count(tmp))
                    continue;
                break;
            }

            // update data
            data[tmp].covernum++;
            data[tmp].elements.insert(uncover_element);

            cout << "add " << uncover_element << " into " << tmp << endl
                 << endl;

            generate_data("up.txt");
        }
        else
            break;
    }
}

void generate(int status)
{
    switch (status)
    {
    case 0:
        generate_OR();
        break;
    case 1:
        generate_random();
        break;
    case 2:
        generate_add_data();
        break;
    case 3:
        generate_up();
        break;

    default:
        cout << "to be continue" << endl;
        break;
    }
}