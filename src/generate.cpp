#include"../include/main.h"
#include"../include/read.h"
#include"../include/find_filename_linux.h"
#include"../include/algorithm.h"

using namespace std;

#define DENSITY 0.1     //the density of conflict graph
#define RAND 1000       //the precision of random number
#define ADD_TIMES 2     //how many times an item will be added
#define RATE 0.7        //the posibility add into solution 

extern int itemnum, elementnum;
extern vector<Item> data;
extern vector< vector<bool> > conflict_graph;
extern vector<int> conflict_num;     
extern unordered_map<int, int> element_cover_times;
extern vector<int> uncovered;
extern unordered_set<int> solution;

void generate_conflict_graph(){
    conflict_graph = vector< vector<bool> >(itemnum, vector<bool>(itemnum, false));
    conflict_num = vector<int>(itemnum, 0);
    //srand(time(NULL));
    //this will make sand same every time
    float tmp;
    for( int i=0; i<itemnum; i++ )
        for( int j=i+1; j<itemnum; j++ ){
            tmp = rand() % RAND / (float)RAND;
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
    ofstream outfile;
    outfile.open(filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
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
    itemnum = 200;
    elementnum = 1000;
    int avg_cover_num = elementnum*5/itemnum;
    int max_offset = avg_cover_num/5; 
    //cout << max_offset << endl; 

    ofstream outfile;
    outfile.open(filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
        return ;
    }
    outfile << itemnum << " " << elementnum << endl;

    generate_conflict_graph();

    //srand(time(NULL));
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
            int element = rand() % elementnum;
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
    cout << "uncovered size is: " << uncovered.size() << endl;
    for( int i=0; i<uncovered.size(); i++ ){
        cout << uncovered[i] << " ";
    }
    cout << endl;

    //add uncovered elements to items randomly
    for( int i=0; i<uncovered.size(); i++ ){
        int cnt = 0;                //add times
        int idx;                    //the chosen item

        while( cnt < ADD_TIMES ){
            double tmp = rand() / RAND_MAX;
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
    ofstream outfile;
    outfile.open(filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
        cout << "i'm generate_add_data" << endl;
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
    
    default:
        cout << "to be continue" << endl;
        break;
    }
}