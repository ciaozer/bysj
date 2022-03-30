#include<unordered_map>
#include<unordered_set>

using namespace std;

void initial();

void update_element_cover_times(bool add, int idx, unordered_map<int, int> &element_cover_times);

void update_candidate_messege(bool add, int idx);

int get_cover_num(unordered_map<int, int> element_cover_times);

void greedy();

int calculate_increase(int before, int idx, unordered_set<int> solution, unordered_map<int, int> element_cover_times);

int choose_remove_item();

void update_tabu_list(int idx);

void perturbation();

void update_element_cover_times_final();

void local_search();

void run();