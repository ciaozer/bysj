#include"../include/main.h"

using namespace std;

int n_item, n_element;
vector< vector<int> > adj_matrix;
unordered_map<int, unordered_set<int> > N_ele;     //items that can cover element j
unordered_map<int, unordered_set<int> > M_item;    //elements that covered by item i
unordered_map<int, unordered_set<int> > G_item;    //items that conflict with item i
unordered_set<int> pre_solution;

void pre_read(string filename){
    ifstream infile;
    infile.open(filename, ios::in);

    //deal with the unexist file
    if( !infile.is_open() ){
        cout << "read file error" << endl;
        //cout << filename << endl;
        cout << "please check your filename" << endl;
        cout << "i'm pre_read" << endl;
        //system("pause");
        exit(0);
    }

    infile >> n_item >> n_element;

    //input what elements an item can cover
    //item counts from 0
    for( int i=0; i<n_item; i++ ){
        //input what elements it covers
        int item_index, cov_num;
        infile >> item_index >> cov_num;
        for( int j=0; j<cov_num; j++ ){
            int element;
            infile >> element;
            N_ele[element].insert(i);   //element j is covered by item i
            M_item[i].insert(element);   //item i covers element j
        }
    
        //input what items it conflicts with
        int conf_num;
        infile >> conf_num;
        for( int j=0; j<conf_num; j++ ){
            int tmp;
            infile >> tmp;
            G_item[i].insert(tmp);
            G_item[tmp].insert(i);
        }

    }
    infile.close();
}

void update_N_ele(vector<int> to_do_item, vector<int> to_do_element){
    for( int i=0; i<to_do_element.size(); i++ ){
        N_ele.erase(to_do_element[i]);
    }
    
    for( auto it=N_ele.begin(); it!=N_ele.end(); it++ ){
        for( int i=0; i<to_do_item.size(); i++ ){
            if( it->second.count(to_do_item[i]) ){
                it->second.erase(to_do_item[i]);
            }
        }
    }
}

void update_M_item(vector<int> to_do_item, vector<int> to_do_element){
    for( int i=0; i<to_do_item.size(); i++ ){
        M_item.erase(to_do_item[i]);
    }

    for( auto it=M_item.begin(); it!=M_item.end(); it++ ){
        for( int i=0; i<to_do_element.size(); i++ ){
            if( it->second.count(to_do_element[i]) ){
                it->second.erase(to_do_element[i]);
            }
        }
    }
}

void update_G_item(vector<int> to_do_item, vector<int> to_do_element){
    for( int i=0; i<to_do_item.size(); i++ ){
        G_item.erase(to_do_item[i]);
    }

    for( auto it=G_item.begin(); it!=G_item.end(); it++ ){
        for( int i=0; i<to_do_item.size(); i++ ){
            if( it->second.count(to_do_item[i]) ){
                it->second.erase(to_do_item[i]);
            }
        }
    }
}

int UP_once(){
    //return the status of UP once
    
    int status = 2;
    //0 represents no result
    //1 represents can up again
    //2 represents can't up again

    for( auto it=N_ele.begin(); it!=N_ele.end(); it++ ){
        int can_cover = it->second.size();
        //no result
        if( can_cover == 0 ){
            status = 0;
            break;
        }
        if( can_cover == 1 ){
            status = 1;
            
            //the chosen item
            int item = it->first;

            //add the chosen item into solution
            pre_solution.insert(item);
            
            vector<int> to_do_element;
            vector<int> to_do_item;

            //find the elements the chosen item covers
            for( auto it=M_item[item].begin(); it!=M_item[item].end(); it++ ){
                to_do_element.push_back(*it);
            }

            //find the chosen item and its conflict item set
            to_do_item.push_back(item);
            for( auto it=G_item[item].begin(); it!=G_item[item].end(); it++ ){
                to_do_item.push_back(*it);
            }

            update_N_ele(to_do_item, to_do_element);
            update_M_item(to_do_item, to_do_element);
            update_G_item(to_do_item, to_do_element);
            break;
        }
    }

    return status;
}

bool UP(){
    bool has_solution = true;
    while(1){
        int status = UP_once();
        if( M_item.size() == 0 ){
            cout << "finish by UP!" << endl;
            break;
        }
        if( status == 0 ){
            has_solution = false;
            break;
        }
        if( status == 1 ){
            status = UP_once();
        }
        else
            break;
    }
    return has_solution;
}

void preprocess(){
    pre_read("up.txt");
    bool has_solution = UP();
    if( !has_solution ){
        cout << "no solution" << endl;
        exit(0);
    }
}