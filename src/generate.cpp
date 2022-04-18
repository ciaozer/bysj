#include"../include/main.h"
#include"../include/read.h"
#include"../include/find_filename_linux.h"
#include"../include/algorithm.h"
#include"../include/preprocess.h"

using namespace std;

#define DENSITY 0.1     //the density of conflict graph
#define RAND 1000       //the precision of random number
#define ADD_TIMES 20     //how many times an item will be added
#define RATE 0.4        //the posibility add into solution 
#define AVG_COVER_TIMES 35 //the average of element be covered times
#define ITEMNUM 1000
#define ELEMENTNUM 10000

extern int itemnum, elementnum;
extern vector<Item> data;
extern vector< vector<bool> > conflict_graph;
extern vector<int> conflict_num;     
extern unordered_map<int, int> element_cover_times;
extern vector<int> uncovered;
extern unordered_set<int> solution;

extern int n_item, n_element;
extern vector< vector<int> > adj_matrix;
extern unordered_map<int, unordered_set<int> > N_ele;     
extern unordered_map<int, unordered_set<int> > M_item;    
extern unordered_map<int, unordered_set<int> > G_item;    
extern unordered_set<int> pre_solution;
extern int uncover_element;
extern unordered_set<int> total_to_do_item;

void generate_data(string filename){
    //generate data according to data
    ofstream outfile;
    outfile.open(filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
        cout << "i am generate_data";
        return ;
    }
    outfile << itemnum << " " << elementnum << endl;
    for( int i=0; i<itemnum; i++ ){
        Item itemtmp = data[i];

        //show the item number and what elements it covers
        outfile << i << " " << itemtmp.covernum << endl;
        for( auto it=itemtmp.elements.begin(); it!=itemtmp.elements.end(); it++ ){
            outfile << *it << " ";
        }
        outfile << endl;

        //show what items it conflicts with
        outfile << conflict_num[i] << endl;
        for( int j=0; j<itemnum; j++ )
            if( conflict_graph[i][j] )
                outfile << j << " ";
        outfile << endl;
    }
    outfile.close();
}

void generate_conflict_graph(){
    conflict_graph = vector< vector<bool> >(itemnum, vector<bool>(itemnum, false));
    conflict_num = vector<int>(itemnum, 0);
    //this will make sand same every time
    double tmp;
    for( int i=0; i<itemnum; i++ )
        for( int j=i+1; j<itemnum; j++ ){
            tmp = rand() % RAND / (double)RAND;
            if( tmp < DENSITY ){
                conflict_graph[i][j] = true;
                conflict_graph[j][i] = true;
                conflict_num[i]++;
                conflict_num[j]++;
            }
        }
}

void generate_or_data(string filename){
    //generate data to the filename
    generate_conflict_graph();
    generate_data(filename);
}

void generate_OR(){
    vector<string> filenames = find_filename_linux("data/init_data");
    for( int i=0; i<filenames.size(); i++ ){
        //cout << filenames[i] << endl;
        string filename = filenames[i];
        string newfilename = "new" + filename;
        filename = "data/init_data/" + filename;
        newfilename = "data/OR_data/" + newfilename;
        data = read_initdata(filename);
        generate_or_data(newfilename);
        cout << "finish generate " << filenames[i] << endl;
    }
}

void generate_random_once(string filename){
    itemnum = ITEMNUM;
    elementnum = ELEMENTNUM;
    int avg_cover_num = elementnum/itemnum*AVG_COVER_TIMES;
    int max_offset = avg_cover_num/2; 
    //cout << max_offset << endl; 

    ofstream outfile;
    outfile.open(filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
        return ;
    }
    outfile << itemnum << " " << elementnum << endl;

    generate_conflict_graph();

    for( int i=0; i<itemnum; i++ ){
        //generate a rand_covernum
        int offset = rand() % (2*max_offset);
        //cout << offset << " ";
        offset -= max_offset;
        int rand_covernum = avg_cover_num + offset;
        //cout << rand_covernum << endl;
        outfile << i << " " << rand_covernum << endl;

        //generate cover elements
        unordered_set<int> rand_elements;
        while( rand_covernum ){
            //element conts from 1
            int element = (rand() % elementnum) + 1;
            //element repetitiom
            if( rand_elements.count(element) ){
                continue;
            }
            rand_covernum--;
            rand_elements.insert(element);
            outfile << element << " ";
        }   
        outfile << endl;

        //show what items it conflicts with
        outfile << conflict_num[i] << endl;
        for( int j=0; j<itemnum; j++ )
            if( conflict_graph[i][j] )
                outfile << j << " ";
        outfile << endl;
    }
    outfile.close();
}

void generate_random(){
    for( int i=0; i<10; i++ ){
        string path = "data/random_data/random";
        path = path + to_string(i) + ".txt";
        generate_random_once(path);
    }
}

void get_uncovered_elements(){
    for( auto it=element_cover_times.begin(); it!=element_cover_times.end(); it++ ){
        if( it->second == 0 ){
            uncovered.push_back(it->first);
        }
    }
}

void print_uncovered(){
    cout << "uncovered size is: " << uncovered.size() << endl;
    for( int i=0; i<uncovered.size(); i++ ){
        cout << uncovered[i] << " ";
    }
    cout << endl;
}

int add_into_solution(int element_idx){
    //return the index of the chosen item 

    int idx = rand() % solution.size();
    auto it = solution.begin();
    while( idx-- )
        it++;
    if( !data[*it].elements.count(element_idx) )
        return *it;
    return add_into_solution(element_idx);
}

int add_into_nonsolution(int element_idx){
    //return the index of the chosen item 
    
    //the item that not in solution
    vector<int> nonsolution;
    for( int i=0; i<itemnum; i++ ){
        if( solution.count(i) == 0 )
            nonsolution.push_back(i);
    }

    int idx = rand() % nonsolution.size();
    int it = nonsolution[idx];
    if( !data[it].elements.count(element_idx) )
        return it;
    return add_into_nonsolution(element_idx);
}

void add_uncovered_elements(){
    get_uncovered_elements();
    int tmp = get_cover_num(element_cover_times);
    cout << "covered size is: " << tmp << endl;
    print_uncovered();

    //add uncovered elements to items randomly
    for( int i=0; i<uncovered.size(); i++ ){
        int cnt = 0;                //add times
        int idx;                    //the chosen item

        while( cnt < ADD_TIMES ){
            //must divide double or it will be 0
            double tmp = rand() / (double)RAND_MAX;
            if( tmp < RATE )
                idx = add_into_solution(uncovered[i]);
            else 
                idx = add_into_nonsolution(uncovered[i]);
            cnt++;
            data[idx].covernum++;
            data[idx].elements.insert(uncovered[i]);
        }
    }
    uncovered.clear();  //ensure the uncovered is right
}

void generate_add_data_once(string filename){
    add_uncovered_elements();
    generate_data(filename);
}

void generate_add_data(){
    for( int i=0; i<10; i++ ){
        string path = "data/random_data/random";
        path = path + to_string(i) + ".txt";
        data = read_data(path);
        run();
        //now the element cover times is right

        path = "data/add/add";
        path = path + to_string(i) + ".txt";
        while( get_cover_num(element_cover_times) != elementnum ){
            generate_add_data_once(path);
            uncovered.clear();      //ensure the uncovered is right
            data = read_data(path);
            run();
        }
    }
}

void generate_up(){
    while(1){
        pre_read("up.txt");
        data = read_data("up.txt");
        uncover_element = 0;
        UP();

        //if exist element can't be covered
        if( uncover_element != 0 ){
            //add uncover_element into an item that not in to_do_item
            int tmp;
            while(1){
                //can be changed, if the data structure it set
                tmp = rand() % n_item;
                if( total_to_do_item.count(tmp) )
                    continue;
                break;
            }

            //update data
            data[tmp].covernum++;
            data[tmp].elements.insert(uncover_element);

            cout << "add " << uncover_element << " into " << tmp << endl << endl;

            generate_data("up.txt");
        }
        else 
            break;
    }
}

void generate(int status){
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