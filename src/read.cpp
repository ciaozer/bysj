#include"../include/main.h"

using namespace std;

extern int itemnum, elementnum;
extern string input_filename;
extern vector< vector<bool> > conflict_graph;
extern vector<int> conflict_num;

vector<Item> read_initdata(string filename){
    vector<Item> items;
    ifstream infile;
    infile.open(filename, ios::in);

    //deal with the unexist file
    if( !infile.is_open() ){
        cout << "read file error" << endl;
        //cout << input_filename << endl;
        cout << "please check your filename" << endl;
        cout << "i'm read_initdata" << endl;
        //system("pause");
        exit(0);
    }

    infile >> itemnum >> elementnum;

    //input useless weight
    int tmp;    
    for( int i=0; i<elementnum; i++ )
        infile >> tmp;
    
    //input what elements an item can cover
    int cnt = 0;    //item counts from 0
    for( int i=0; i<itemnum; i++ ){
        Item item;
        item.no = cnt++;
        infile >> item.covernum;
        unordered_set<int> elements;
        for( int j=0; j<item.covernum; j++ ){
            int element;
            infile >> element;
            elements.insert(element);
        }
        item.elements = elements;

        items.push_back(item);
    }
    infile.close();
    return items;
}

vector<Item> read_data(string filename){
    vector<Item> items;
    ifstream infile;
    infile.open(filename, ios::in);

    //deal with the unexist file
    if( !infile.is_open() ){
        cout << "read file error" << endl;
        //cout << filename << endl;
        cout << "please check your filename" << endl;
        cout << "i'm read data" << endl;
        //system("pause");
        exit(0);
    }

    infile >> itemnum >> elementnum;
    
    conflict_graph = vector< vector<bool> >(itemnum, vector<bool>(itemnum, false));
    conflict_num = vector<int>(itemnum, 0);

    //input what elements an item can cover
    //item counts from 0
    for( int i=0; i<itemnum; i++ ){
        Item item;

        //input what elements it covers
        infile >> item.no >> item.covernum;
        unordered_set<int> elements;
        for( int j=0; j<item.covernum; j++ ){
            int element;
            infile >> element;
            elements.insert(element);
        }
        item.elements = elements;
    
        //input what items it conflicts with
        infile >> conflict_num[i];
        for( int j=0; j<conflict_num[i]; j++ ){
            int tmp;
            infile >> tmp;
            conflict_graph[i][tmp] = true;
        }

        items.push_back(item);
    }
    infile.close();

    //output conflict graph
    // for( int i=0; i<itemnum; i++ ){
    //     for( int j=0; j<itemnum; j++ ){
    //         if( conflict_graph[i][j] )
    //             cout << 1 << " ";
    //         else 
    //             cout << 0 << " ";
    //     }
    //     cout << endl;
    // }

    return items;
}