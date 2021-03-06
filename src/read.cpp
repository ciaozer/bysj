#include"../include/main.h"
#include"../include/algorithm.h"

using namespace std;

#define TABUSTEP 3

extern int itemnum, elementnum;
extern vector< vector<bool> > conflict_graph;
extern unordered_map<int, unordered_set<int> > N_ele;     //items that can cover element j
extern unordered_map<int, unordered_set<int> > M_item;    //elements that covered by item i
extern unordered_map<int, unordered_set<int> > G_item;    //items that conflict with item i
extern unordered_map<int, int> weight;
extern bool is_weight;      
extern unordered_map<int, Item> data;                                //whether the example is weighted

unordered_map<int, Item> read_initdata(string filename){
    unordered_map<int, Item> items;
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
    int index;      //the index of item
    for( int i=0; i<itemnum; i++ ){
        Item item;
        index = cnt++;
        infile >> item.covernum;
        unordered_set<int> elements;
        for( int j=0; j<item.covernum; j++ ){
            int element;
            infile >> element;
            elements.insert(element);
        }
        item.elements = elements;

        items[i] = item;
    }
    infile.close();
    return items;
}

unordered_map<int, Item> read_data(string filename){
    unordered_map<int, Item> items;

    N_ele.clear();
    M_item.clear();
    G_item.clear();

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

    //initial the three variable, in case that 0
    for( int i=0; i<itemnum; i++ ){
        M_item[i] = unordered_set<int>();
        G_item[i] = unordered_set<int>();
    }
    //element counts from 1
    for( int i=1; i<=elementnum; i++ ){
        N_ele[i] = unordered_set<int>();
    }

    //input element weight
    for( int i=1; i<=elementnum; i++ ){
        infile >> weight[i];
        if( weight[i] != 1 )
            is_weight = true;
    }
    
    conflict_graph = vector< vector<bool> >(itemnum, vector<bool>(itemnum, false));

    //input what elements an item can cover
    //item counts from 0
    for( int i=0; i<itemnum; i++ ){
        Item item;

        //initial item
        item.candidate_conflict_num = 0;
        item.cover_of_candidate = 0;
        item.stay = 0;
        item.tabulist = TABUSTEP;

        //input what elements it covers
        int index;      //the index of item
        infile >> index >> item.covernum;
        unordered_set<int> elements;
        for( int j=0; j<item.covernum; j++ ){
            int element;
            infile >> element;
            elements.insert(element);
            N_ele[element].insert(index);   //element j is covered by item i
            M_item[index].insert(element);  //item i covers element j
        }
        item.elements = elements;
    
        //input what items it conflicts with
        infile >> item.conflict_num;
        for( int j=0; j<item.conflict_num; j++ ){
            int tmp;
            infile >> tmp;
            conflict_graph[index][tmp] = true;
            G_item[index].insert(tmp);
            G_item[tmp].insert(index);
        }

        items[index] = item;
    }
    infile.close();
    cout << "finish read_data" << endl;

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

void change_to_maxsat(string input_filename, string output_filename){
    data = read_data(input_filename);
    const int INF = get_total_score() + 1;

    int conflict_cnt = 0;

    for( int i=0; i<itemnum; i++ ){
        for( int j=i+1; j<itemnum; j++ ){
            if( conflict_graph[i][j] )
                conflict_cnt++;
        }
    }

    ofstream outfile;
    outfile.open(output_filename, ios::out);
    if( !outfile.is_open() ){
        cout << "read file error" << endl;
        cout << "i am change_to_maxsat" << endl;
        return ;
    }

    outfile << "p wcnf " << itemnum << " " << conflict_cnt+elementnum << " " << INF << endl;

    for( int i=0; i<itemnum; i++ ){
        for( int j=i+1; j<itemnum; j++ ){
            if( conflict_graph[i][j] ){
                if( i == 0 )
                    outfile << INF << " " << "-" << itemnum << " " << "-" << j << " " << 0 << endl;
                else
                    outfile << INF << " " << "-" << i << " " << "-" << j << " " << 0 << endl;
            }
        }
    }

    for( int i=1; i<=elementnum; i++ ){
        outfile << weight[i] << " ";
        for( auto it=N_ele[i].begin(); it!=N_ele[i].end(); it++ ){
            if( *it == 0 )
                outfile << itemnum << " ";
            else
                outfile << *it << " ";
        }
        outfile << 0 << endl;
    }

    outfile.close();
}