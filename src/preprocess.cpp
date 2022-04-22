#include"../include/main.h"
#include"../include/algorithm.h"
#include"../include/read.h"

using namespace std;

extern int itemnum, elementnum;
extern unordered_map<int, Item> data;
extern unordered_map<int, unordered_set<int> > N_ele;     //items that can cover element j
extern unordered_map<int, unordered_set<int> > M_item;    //elements that covered by item i
extern unordered_map<int, unordered_set<int> > G_item;    //items that conflict with item i
extern unordered_map<int, int> element_cover_times;
extern unordered_set<int> solution;

unordered_set<int> pre_solution;
int uncover_element;
unordered_set<int> total_to_do_item;
unordered_set<int> total_to_do_element;
unordered_set<int> check_element;       //for rule1

bool subset_to(unordered_set<int> set1, unordered_set<int> set2){
/*
    check whether there subset relation between set1 and set2
    if set1 is the subset of set2 then return true
*/
    //check whether set1 is the subset of set2
    bool is_subset = true;
    for( auto it=set1.begin(); it!=set1.end(); it++ ){
        if( set2.count(*it) == 0 ){
            is_subset = false;
            break;
        }
    }

    return is_subset;
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

void update_ele_cover(vector<int> to_do_element){
    for( int i=0; i<to_do_element.size(); i++ ){
        element_cover_times[to_do_element[i]] = 1;
    }
}
            
void update_data_item(vector<int> to_do_item){
    for( int i=0; i<to_do_item.size(); i++ ){
        data.erase(to_do_item[i]);
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
            update_ele_cover(to_do_element);
            update_data_item(to_do_item);

            cout << "UP_once decrease " << to_do_item.size() << " items";
            cout << " and " << to_do_element.size() << " elements" << endl;

            break;
        }
    }

    return status;
}

bool UP(){
    bool is_useful = false;
    while(1){
        int status = UP_once();
        if( M_item.size() == 0 ){
            cout << "finish by UP!" << endl;
            is_useful = false;
            break;
        }
        if( status == 0 ){
            cout << "no solution" << endl;
            is_useful = false;
            break;
        }
        if( status == 1 ){
            is_useful = true;
            continue;
        }
        else{
            break;
        }
    }

    cout << "finish UP" << endl;

    return is_useful;
}

bool rule1(){
/*
    if N_ele[i] subset N_ele[j] 
    then delete j
    first think it can be covered
    finally have to check
*/
    vector<int> to_do_item;
    vector<int> to_do_element;

    for( auto it1=N_ele.begin(); it1!=N_ele.end(); it1++ ){
        for( auto it2=N_ele.begin(); it2!=N_ele.end(); it2++ ){
            if( it1 == it2 )
                continue;
            if( subset_to(it1->second, it2->second) ){
                to_do_element.push_back(it2->first);
                total_to_do_element.insert(it2->first);
                check_element.insert(it2->first);
                break;  //avoid same element
            }
        }
    }

    update_N_ele(to_do_item, to_do_element);
    update_M_item(to_do_item, to_do_element);
    update_ele_cover(to_do_element);

    cout << "rule1 decrease " << to_do_element.size() << " element" << endl;

    // for( int i=0; i<to_do_element.size(); i++ ){
    //     cout << to_do_element[i] << " ";
    // }
    // cout << endl;

    if( to_do_element.size() )
        return true;
    return false;
}

bool rule2(){
/*
    if M_item(i) subset M_item(j) and G_item(j) subset G_item(i)
    then delete item i
    only have to update item, decrease
*/
    vector<int> to_do_item;
    vector<int> to_do_element;

    for( auto it1=M_item.begin(); it1!=M_item.end(); it1++ ){
        for( auto it2=M_item.begin(); it2!=M_item.end(); it2++ ){
            if( it1 == it2 )    
                continue;
            if( subset_to(it1->second, it2->second) && subset_to(G_item[it2->first], G_item[it1->first]) ){
                to_do_item.push_back(it1->first);
                total_to_do_item.insert(it1->first);
                break;  //avoid same item 
            }
        }
    }

    // unordered_set<int> to_do_element_set;
    // for( int i=0; i<to_do_item.size(); i++ ){
    //     for( auto it=M_item[to_do_item[i]].begin(); it!=M_item[to_do_item[i]].end(); it++ ){
    //         if( to_do_element_set.count(*it) == 0 ){
    //             to_do_element_set.insert(*it);
    //             to_do_element.push_back(*it);
    //         }
    //     }
    // }

    update_N_ele(to_do_item, to_do_element);
    update_M_item(to_do_item, to_do_element);
    update_G_item(to_do_item, to_do_element);
    update_data_item(to_do_item);

    cout << "rule2 decrease " << to_do_item.size() << " items" << endl;

    for( int i=0; i<to_do_item.size(); i++ ){
        cout << to_do_item[i] << " ";
    }
    cout << endl;

    if( to_do_item.size() )
        return true;
    return false;
}

bool rule3(){
/*
    if a G_item[i] == 0, put it into presolution
*/
    bool is_useful = false;
    vector<int> to_do_item;
    vector<int> to_do_element;
    unordered_set<int> to_do_element_set;
    for( auto it=G_item.begin(); it!=G_item.end(); it++ ){
        if( it->second.size() == 0 ){
            is_useful = true;
            to_do_item.push_back(it->first);
            total_to_do_item.insert(it->first);
        }
    }
    for( int i=0; i<to_do_item.size(); i++ ){
        for( auto it=M_item[to_do_item[i]].begin(); it!=M_item[to_do_item[i]].end(); it++ ){
            if( to_do_element_set.count(*it) == 0 ){
                to_do_element_set.insert(*it);
                to_do_element.push_back(*it);
                total_to_do_element.insert(*it);
            }
        }
    }

    update_N_ele(to_do_item, to_do_element);
    update_M_item(to_do_item, to_do_element);
    update_G_item(to_do_item, to_do_element);
    update_ele_cover(to_do_element);
    update_data_item(to_do_item);

    cout << "rule3 decrease " << to_do_item.size() << " items";
    cout << " and " << to_do_element.size() << " elements" << endl;

    for( int i=0; i<to_do_item.size(); i++ ){
        cout << to_do_item[i] << " ";
    }
    cout << endl;

    return is_useful;
}

bool rule4(){
/*
    if N_ele[i] subset G_item[j]
    then delete j
*/
    vector<int> to_do_item;
    vector<int> to_do_element;

    for( auto it1=N_ele.begin(); it1!=N_ele.end(); it1++ ){
        for( auto it2=G_item.begin(); it2!=G_item.end(); it2++ ){
            if( subset_to(it1->second, it2->second) ){
                to_do_item.push_back(it2->first);
                total_to_do_item.insert(it2->first);
                break;  //avoid same item
            }
        }
    }

    update_N_ele(to_do_item, to_do_element);
    update_M_item(to_do_item, to_do_element);
    update_G_item(to_do_item, to_do_element);
    update_data_item(to_do_item);

    cout << "rule4 decrease " << to_do_item.size() << " item" << endl;

    for( int i=0; i<to_do_item.size(); i++ ){
        cout << to_do_item[i] << " ";
    }
    cout << endl;

    if( to_do_item.size() )
        return true;
    return false;
}

void preprocess(){
    bool stop_flag = true;  //if all the process don't have any action then stop
    while(1){
        stop_flag = true;
        if( UP() )          stop_flag = false;
        if( rule1() )       stop_flag = false;
        if( rule2() )       stop_flag = false;
        if( rule3() )       stop_flag = false;
        if( rule4() )       stop_flag = false;
        if( stop_flag )     break;
    }
    cout << "finish preprocess" << endl;
    cout << "now we decrease " << total_to_do_item.size() << " items and ";
    cout << total_to_do_element.size() << " elements" << endl;
    print_solution(pre_solution);
}

void proprocess(){
    for( auto it=pre_solution.begin(); it!=pre_solution.end(); it++ )
        solution.insert(*it);
}