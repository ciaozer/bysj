#include"../include/main.h"
#include"../include/preprocess.h"
#include"../include/read.h"

using namespace std;

#define TABUSTEP 3      //the depth of tabu search
#define CUTOFF 20       //cutoff time in local search
#define NOTBETTERCUT 2  //tabu search not better cut
#define REINFORCE 0     //1 represents use reinforce learning
#define REMOVENUM 2     //the number of removed items in perturbation

extern int itemnum, elementnum;
extern int best_cover_num;
extern int best_score;
 
extern unordered_map<int, Item> data;
extern vector< vector<bool> > conflict_graph;
extern unordered_set<int> candidate;   //items that not in solution and not conflicts with the items in solution
extern unordered_set<int> solution;    //current result, the items that have been selected
extern unordered_set<int> best_solution;
extern unordered_map<int, int> element_cover_times;    //solution covers elements times
extern unordered_map<int, int> weight; //element is key
extern bool is_weight;
 
void initial(){
    //clear all the things
    candidate.clear();
    solution.clear();
    best_solution.clear();

    for( auto it=data.begin(); it!=data.end(); it++ ){
        //no item is in solution, every item is in candidate
        
        candidate.insert(it->first);
        it->second.candidate_conflict_num = it->second.conflict_num;
        it->second.cover_of_candidate = it->second.covernum;
    }

    //elements count from 1
    for( int i=1; i<=elementnum; i++ )
        element_cover_times[i] = 0;

    cout << "finish initial" << endl;
}

void update_element_cover_times(bool add, int idx, unordered_map<int, int> &element_cover_times){
    /*
        update elements covered times solution covers 
        add: true represents add item into solution, false represents remove
        idx: the index of the item
        the last parameter is for the calculate function, it has to use the inner data
    */
    if( add == true ){
        for( auto it=data[idx].elements.begin(); it!=data[idx].elements.end(); it++ )
            element_cover_times[*it]++;
    }
    else{
        for( auto it=data[idx].elements.begin(); it!=data[idx].elements.end(); it++ )
            element_cover_times[*it]--;        
    }

}

void update_candidate_messege(bool add, int idx){
    /*
        update candidate messege
        add: true represents add item into solution, false represents remove
        idx: the index of the item
    */
            
    //update element cover times
    update_element_cover_times(add, idx, element_cover_times);
    
    //update candidate
    if( add == true ){
        //add item into solution, candidate will decrease
        vector<int> remove_candidate;
        for( auto it=candidate.begin(); it!=candidate.end(); it++ ){
            if( conflict_graph[idx][*it] )
                remove_candidate.push_back(*it);
        }
        for( int i=0; i<remove_candidate.size(); i++ )
            candidate.erase(remove_candidate[i]);
    }
    else{
        //remove item from solution, choose new candidate
        //traverse all items
        for( int i=0; i<itemnum; i++ ){
            //not in solution and candidate
            if( !solution.count(i) && !candidate.count(i) ){
                bool is_conflict = false;

                //not conflict with solution
                for( auto it=solution.begin(); it!=solution.end(); it++ )
                    if( conflict_graph[i][*it] ){
                        is_conflict = true;
                        break;
                    }

                if( !is_conflict )
                    candidate.insert(i);
            }
        }
    }

    //update candidate conflict related
    for( auto it=candidate.begin(); it!=candidate.end(); it++ ){
        Item item = data[*it];
        int cover_cnt = 0;
        int conflict_cnt = 0;

        for( auto iter=item.elements.begin(); iter!=item.elements.end(); iter++ ){

            //how many elements that hasn't been covered before
            if( element_cover_times[*iter] == 0 )
                cover_cnt++;

            //how many candidates it conflicts with
            if( conflict_graph[*it][*iter] )
                conflict_cnt++;
        }

        data[*it].cover_of_candidate = cover_cnt;
        data[*it].candidate_conflict_num = conflict_cnt;
    }
}

int get_cover_num(unordered_map<int, int> element_cover_times){
    /*
        in calulate function, can't really change the global value
        so need to give this function a variable
    */
    int cnt = 0;
    for( int i=1; i<=elementnum; i++ )
        if( element_cover_times[i] )
            cnt++;
    return cnt;
}

int get_score(unordered_map<int, int> element_cover_times){
    int score = 0;
    for( int i=1; i<=elementnum; i++ ){
        if( element_cover_times[i] )
            score += weight[i];
    }
    return score;
}

void greedy(){
    int idx;    //the index of the chosen item
    //choose by covernum/conflictnum
    double best = 0;
    double current = 0;

    //choose an item until no item can be chosen
    while( candidate.size() ){
        best = 0;    
        bool is_conflict = false;    //true represents this item conflict with other candidate

        //traverse candidate to get the best item
        for( auto it=candidate.begin(); it!=candidate.end(); it++ ){
            
            //not conflict with other candidate
            if( data[*it].candidate_conflict_num == 0 ){
                idx = *it;
                break;
            }
            is_conflict = true;
            current = (double)data[*it].cover_of_candidate * weight[*it] / (double)data[*it].candidate_conflict_num;
            
            //include equal to deal with the situation that current_max = 0, it will never go on
            if( current >= best ){
                idx = *it;
                best = current;
            }
        }

        candidate.erase(idx);
        solution.insert(idx);
        data[idx].stay++;

        if( is_conflict )
            update_candidate_messege(true, idx);
        else 
            update_element_cover_times(true, idx, element_cover_times);
    }

    best_solution = solution;
    best_cover_num = get_cover_num(element_cover_times);
    best_score = get_score(element_cover_times);
    cout << "finish greedy" << endl;
    cout << "after greedy, " << best_cover_num << " elements have been covered" << endl;
    cout << "now the score is: " << best_score << endl;

}

int calculate_increase(int before, int idx, unordered_set<int> solution, unordered_map<int, int> element_cover_times){
    /*
        can't really change global value
        pass-by-value instead of pass-by-reference
    */
    
    //after last call we get the wrong element_cover_times value
    //but the solution calue is right because it is pass-by-reference
    update_element_cover_times(false, idx, element_cover_times);
    solution.erase(idx);

    //to ensure the add item is not from the beginning
    int random_index = rand() % itemnum;
    for( int i=random_index; i<random_index+itemnum; i++ ){
        //item not in solution
        if( solution.count(i%itemnum) )
            continue;

        //not conflict, don't use update function because the complexity is higher
        bool is_conflict = false;
        for( int j=0; j<itemnum; j++ ){
            if( conflict_graph[i%itemnum][j] && solution.count(j) ){
                is_conflict = true;
                break;
            }
        }

        //shouldn't be the remove one
        if( !is_conflict && (i%itemnum)!=idx ){
            solution.insert(i%itemnum);
            update_element_cover_times(true, i%itemnum, element_cover_times);
        }
    }

    // int ans = get_cover_num(element_cover_times);
    // ans -= before;
    // return ans;
    //return get_cover_num(element_cover_times) - before;
    return get_score(element_cover_times) - before;
}

int choose_remove_item(){
    //choose the item that can cover more elements, even not better than before
    int best_increase = INT8_MIN;
    int current_increase;
    int idx;    //the index of the return item
    //int before = get_cover_num(element_cover_times);
    int before = get_score(element_cover_times);
    for( auto it=solution.begin(); it!=solution.end(); it++ ){
        //don't choose the item that has been chosen recently
        if( data[*it].tabulist < TABUSTEP )
            continue;
        current_increase = calculate_increase(before, *it, solution, element_cover_times);
        if( current_increase > best_increase ){
            best_increase = current_increase;
            idx = *it;
        }
    }
    return idx;
}

void update_tabu_list(int idx){
    for( auto it=data.begin(); it!=data.end(); it++ ){
        //the chosen item's tabu set to 1
        if( it->first == idx ) 
            it->second.tabulist = 1;
        
        //other item's tabu should increase, but not out of bound
        else if( it->second.tabulist < TABUSTEP )
            it->second.tabulist++;
    }
}

void perturbation(){
    if( REINFORCE == 1 ){
        //choose two items that have the most stay turns
        int stay_most_1_idx = *solution.begin();
        int stay_most_2_idx = *(solution.begin()++);
        if( data[stay_most_1_idx].stay < data[stay_most_2_idx].stay )
            swap(stay_most_1_idx, stay_most_2_idx);
        auto it = solution.begin();
        it++;
        for( it++; it!=solution.end(); it++ ){
            if( data[*it].stay > data[stay_most_1_idx].stay ){
                stay_most_2_idx = stay_most_1_idx;
                stay_most_1_idx = *it;
            }
            else if( data[*it].stay > data[stay_most_2_idx].stay )
                stay_most_2_idx = *it;
        }

        //remove two items that have the most stay turns
        for( int i=0; i<2; i++ ){
            for( auto it=solution.begin(); it!=solution.end(); it++ ){
                if( *it == stay_most_1_idx || *it == stay_most_2_idx ){
                    int remove_item = *it;
                    solution.erase(remove_item);
                    update_candidate_messege(false, remove_item);
                    break;
                }
            }
        }
    }
    else{
        //remove some items from solution randomly
        for( int i=0; i<REMOVENUM; i++ ){
            int remove_index = rand() % solution.size();
            auto it = solution.begin();
            while( remove_index-- )
                it++;
            
            //if not save the *it value, *it will point to nowhere
            int remove_item = *it;
            solution.erase(remove_item);
            update_candidate_messege(false, remove_item);
        }
    }
}

void update_element_cover_times_final(){
    //clear element cover times
    for( int i=1; i<=elementnum; i++ )
        element_cover_times[i] = 0;

    for( auto it=solution.begin(); it!=solution.end(); it++ ){
        Item itemtmp = data[*it];
        for( auto iter=itemtmp.elements.begin(); iter!=itemtmp.elements.end(); iter++ ){
            element_cover_times[*iter]++;
        }
    }
}

void local_search(){
    time_t begintime = time(NULL);      //the time begin local search
    time_t endtime = time(NULL);        //the time now

    while( best_cover_num<elementnum && endtime-begintime < CUTOFF ){
        int not_better_cnt = 0;
        int current_cover_times;
        int current_score;

        //local search for local optimal, if find a feasible solution then quit
        while( not_better_cnt<NOTBETTERCUT && best_cover_num<elementnum ){
            // choose a set to remove from the solution
            int remove = choose_remove_item();
            solution.erase(remove);

            //update tabu list, item, candidate
            update_tabu_list(remove);

            //attention, in the calculate function, it changes the 
            //element_cover_times, but it may not be the right value
            //so we have to update it
            update_candidate_messege(false, remove);

            //insert an item without conflict
            //to ensure every time can get different item, use random
            int random_index = rand() % itemnum;
            for( int i=random_index; i<random_index+itemnum; i++ ){
                //item not in solution
                if( solution.count(i%itemnum) )
                    continue;

                //not conflict, don't use update function because the complexity is higher
                bool is_conflict = false;
                for( int j=0; j<itemnum; j++ ){
                    if( conflict_graph[i%itemnum][j] && solution.count(j) ){
                        is_conflict = true;
                        break;
                    }
                }

                //shouldn't be the remove one
                if( !is_conflict && (i%itemnum)!=remove ){
                    solution.insert(i%itemnum);
                    update_candidate_messege(true, i%itemnum);
                }
            }

            //calculate how many turns an item is in the solution
            for( auto it=solution.begin(); it!=solution.end(); it++ ){
                data[*it].stay++;
            }

            current_cover_times = get_cover_num(element_cover_times);
            current_score = get_score(element_cover_times);
            cout << "best cover: " << best_cover_num;
            cout << "     current cover: " << current_cover_times;
            cout << "     best score: " << best_score;
            cout << "     current score: " << current_score << endl;

            //update best and check whether better
            if( best_cover_num < current_cover_times )
                best_cover_num = current_cover_times;
            if( current_score <= best_score )
                not_better_cnt++;
            else{
                best_cover_num = current_cover_times;
                best_score = current_score;
                best_solution = solution;
                not_better_cnt = 0;
            }
        }

        //check whether find a feasible solution
        if( best_cover_num == elementnum )
            break;
        
        //pertubation to jump out of the local optimal
        perturbation();

        endtime = time(NULL);

    }

    //update solution
    solution = best_solution;

    //print the local search result
    cout << "finish local search" << endl;
    if( best_cover_num == elementnum )
        cout << "find a feasible solution!" << endl;
    else{
        cout << "can't find a feasible solution" << endl;
        cout << "only cover " << best_cover_num << " elements" << endl;
    }
    cout << "the score is: " << best_score << endl;

    //update element cover times with the final solution
    update_element_cover_times_final();
}

void print_solution(unordered_set<int> solution){
    cout << "the solution size is: " << solution.size() << endl;
    //output the sorted solution
    vector<int> sorted_solution;
    for( auto it = solution.begin(); it!=solution.end(); it++ )
        sorted_solution.push_back(*it);
    sort(sorted_solution.begin(), sorted_solution.end());
    for( int i=0; i<sorted_solution.size(); i++ ){
        cout << sorted_solution[i] << " ";
        if( i%10 == 9 )
            cout << endl;
    }
    cout << endl;
}

void run(string filename){
    data = read_data(filename);
    preprocess();
    initial();          //finish some initial works
    greedy();           //construct a initial solution with greedy algorithm 
    local_search(); 
    proprocess();
    print_solution(solution);
}