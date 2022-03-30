#include"../include/main.h"
#include"../include/read.h"
#include"../include/find_filename_linux.h"
#include"../include/algorithm.h"

using namespace std;

#define DENSITY 0.1     //the density of conflict graph
#define RAND 1000       //the precision of random number

extern int itemnum, elementnum;
extern vector<Item> data;
extern vector< vector<bool> > conflict_graph;
extern vector<int> conflict_num;     
extern unordered_map<int, int> element_cover_times;
extern vector<int> uncovered;

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

void generate_add_data(string filename){
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
    int add_times = itemnum / 10;  //how many times an element will be added
    for( int i=0; i<uncovered.size(); i++ ){
        int cnt = 0;                //add times
        unordered_set<int> added;  //which item has added
        while( cnt < add_times ){
            int idx = rand() % itemnum;

            //added before or item has it before
            if( added.count(idx) || data[idx].elements.count(uncovered[i]) ){
                continue;
            }

            cnt++;
            added.insert(idx);
            data[idx].covernum++;
            data[idx].elements.insert(uncovered[i]);
        }
    }

    generate_add_data("data/add/test.txt");
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
        add_uncovered_elements();
        break;
    
    default:
        cout << "to be continue" << endl;
        break;
    }
}