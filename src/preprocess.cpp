#include"../include/main.h"
#include"../include/algorithm.h"
#include"../include/read.h"

using namespace std;

extern unordered_map<int, Item> data;
extern unordered_map<int, unordered_set<int> > N_ele;     //items that can cover element j
extern unordered_map<int, unordered_set<int> > M_item;    //elements that covered by item i
extern unordered_map<int, unordered_set<int> > G_item;    //items that conflict with item i
unordered_set<int> pre_solution;
int uncover_element;
unordered_set<int> total_to_do_item;
unordered_set<int> total_to_do_element;

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
            uncover_element = it->first;
            cout << "the element " << uncover_element << " can't be covered!" << endl;
            break;
        }
        if( can_cover == 1 ){
            status = 1;
            
            //the chosen item
            int item = *(it->second.begin());


            //add the chosen item into solution
            pre_solution.insert(item);
            
            vector<int> to_do_element;
            vector<int> to_do_item;

            //find the elements the chosen item covers
            for( auto it=M_item[item].begin(); it!=M_item[item].end(); it++ ){
                to_do_element.push_back(*it);
                total_to_do_element.insert(*it);
            }

            //find the chosen item and its conflict item set
            to_do_item.push_back(item);
            total_to_do_item.insert(item);
            for( auto it=G_item[item].begin(); it!=G_item[item].end(); it++ ){
                to_do_item.push_back(*it);
                total_to_do_item.insert(*it);
            }

            update_N_ele(to_do_item, to_do_element);
            update_M_item(to_do_item, to_do_element);
            update_G_item(to_do_item, to_do_element);
            break;
        }
    }

    return status;
}

void UP(){
    while(1){
        int status = UP_once();
        if( M_item.size() == 0 ){
            cout << "finish by UP!" << endl;
            break;
        }
        if( status == 0 ){
            cout << "no solution" << endl;
            break;
        }
        if( status == 1 ){
            continue;
        }
        else
            break;
    }

    cout << "finish UP" << endl;
    cout << "now we decrease " << total_to_do_item.size() << " items and ";
    cout << total_to_do_element.size() << " elements" << endl;
    cout << "the pre_solution size is: " << pre_solution.size() << endl;
    print_solution(pre_solution);
}

void preprocess(){
    data = read_data("up.txt");
    UP();
}