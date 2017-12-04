#include <bits/stdc++.h>


using namespace std;

int puzzle_dim;
string huristic_type;

struct state{
    vector < vector <int> > board;
    int huristic_value, depth;
    unsigned long long hash_val;
    bool is_terminal;
    state() {}
    int huristic_misplaced(){
        int huristic = 0;
        for(int i = 0; i < puzzle_dim; i++){
            for(int j = 0; j < puzzle_dim; j++){
                if(i + 1  == puzzle_dim  && j + 1 == puzzle_dim) continue;
                huristic += (board[i][j] != (i * puzzle_dim + j + 1));
            }
        }
        return huristic;
    }

    int huristic_manhattan(){
        int huristic = 0;
        for(int i = 0; i < puzzle_dim; i++){
            for(int j = 0; j < puzzle_dim; j++){
                if(board[i][j] == 0) continue;

                int pi, pj;
                pi = (board[i][j] - 1) / puzzle_dim;
                pj = (board[i][j] - 1) % puzzle_dim;
                huristic += llabs(pi - i) + llabs(pj - j);
            }
        }
        return huristic;
    }

    state(vector < vector <int> > board, int depth): board(board), depth(depth){
        hash_val = 0ull;
        for(auto row : board){
            for(auto item : row){
                hash_val = hash_val * 10ull + (unsigned long long) item;
            }
        }

        is_terminal = huristic_misplaced() == 0;

        huristic_value = 0;
        if(huristic_type == "misplaced"){
            huristic_value = huristic_misplaced();
        }
        else if(huristic_type == "manhattan"){
            huristic_value = huristic_manhattan();
        }
    }

    vector <state> successors(){
        vector <state> return_states;
        for(int i = 0; i < puzzle_dim; i++){
            for(int j = 0; j < puzzle_dim; j++){
                if(board[i][j] == 0){
                    if(i > 0){
                        swap(board[i][j], board[i-1][j]);
                        return_states.push_back(state(board, depth +  1));
                        swap(board[i][j], board[i-1][j]);
                    }
                    if(j > 0){
                        swap(board[i][j], board[i][j-1]);
                        return_states.push_back(state(board, depth + 1));
                        swap(board[i][j], board[i][j-1]);
                    }
                    if(i + 1 < puzzle_dim){
                        swap(board[i][j], board[i+1][j]);
                        return_states.push_back(state(board, depth + 1));
                        swap(board[i][j], board[i+1][j]);
                    }
                    if(j + 1 < puzzle_dim){
                        swap(board[i][j], board[i][j+1]);
                        return_states.push_back(state(board, depth + 1));
                        swap(board[i][j], board[i][j+1]);
                    }
                    return return_states;
                }
            }
        }
    }

    void print_state(){
        for(int i = 0; i < puzzle_dim; i++){
            for(int j = 0; j < puzzle_dim; j++){
                cout << "---";
            }
            cout << "-" << endl;
            for(int j = 0; j < puzzle_dim; j++){
                if(board[i][j] < 10) cout << "| " << board[i][j];
                else cout << "|" << board[i][j];
            }
            cout << "|" << endl;
        }
        for(int j = 0; j < puzzle_dim; j++){
            cout << "---";
        }
        cout << "-" << endl << endl;
    }

    bool operator > (const state &s) const {
        return depth + huristic_value > s.depth + s.huristic_value;
    }
};

unordered_set <unsigned long long> visited;


double dfs(state initial_state){
    double start = clock();
    visited.clear();
    stack <state> s;
    s.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!s.empty()){
        state u = s.top();
        if(u.is_terminal){
            double finish = clock();
            return (finish - start) / CLOCKS_PER_SEC;
        }
        s.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                s.push(v);
                visited.insert(v.hash_val);
            }
        }
    }
}


double iterative_deepening(state initial_state){
    double start = clock();
    int depth = 0;
    while(1){
        visited.clear();
        stack <state> s;
        s.push(initial_state);
        visited.insert(initial_state.hash_val);
        while(!s.empty()){
            state u = s.top();
            if(u.is_terminal){
                double finish = clock();
                return (finish - start) / CLOCKS_PER_SEC;
            }
            s.pop();
            for(auto v : u.successors()){
                if(visited.find(v.hash_val) == visited.end() && v.depth <= depth){
                    s.push(v);
                    visited.insert(v.hash_val);
                }
            }
        }
        depth++;
    }
}

double bfs(state initial_state){
    double start = clock();
    visited.clear();
    queue <state> q;
    q.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!q.empty()){
        state u = q.front();
        if(u.is_terminal){
            double finish = clock();
            return (finish - start) / CLOCKS_PER_SEC;
        }
        q.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                q.push(v);
                visited.insert(v.hash_val);
            }
        }
    }
}

double a_star_search(state initial_state){
    double start = clock();
    visited.clear();
    priority_queue <state, vector <state>, greater<state> > pq;
    pq.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!pq.empty()){
        state u = pq.top();
        if(u.is_terminal){
            double finish = clock();
            return (finish - start) / CLOCKS_PER_SEC;
        }
        pq.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                pq.push(v);
                visited.insert(v.hash_val);
            }
        }
    }
}


vector <state> bfs_gen(int lim){
    vector <state> return_states;
    visited.clear();
    vector < vector <int> > initial_board;
    initial_board.resize(puzzle_dim, vector <int> (puzzle_dim));
    for(int i = 0; i < puzzle_dim; i++){
        for(int j = 0; j < puzzle_dim; j++){
            initial_board[i][j] = i * puzzle_dim + j + 1;
        }
    }
    initial_board[puzzle_dim - 1][puzzle_dim - 1] = 0;
    state initial_state = state(initial_board, 0);
    queue <state> q;
    q.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!q.empty()){
        state u = q.front();
        q.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end() && v.depth <= lim){
                q.push(v);
                visited.insert(v.hash_val);
                if(v.depth == lim) return_states.push_back(v);
            }
        }
    }
    random_shuffle(return_states.begin(), return_states.end());
    while(return_states.size() > 10) return_states.pop_back();
    return return_states;
}


vector <state> generated_states[10];

int main(){
    for(int dim = 3; dim < 8; dim += 2){
        puzzle_dim = dim;
        generated_states[dim] = bfs_gen(10);
    }
    cout << "For bfs," << endl;
    for(int dim = 3; dim < 8; dim += 2){
        puzzle_dim = dim;
        double avg_time = 0;
        for(auto initial_state : generated_states[dim]){
            initial_state.depth = 0;
            avg_time += bfs(initial_state);
        }
        avg_time /= generated_states[dim].size();
        cout << dim << ' ' << avg_time << endl;
    }

    cout << "For iterative_deepening," << endl;
    for(int dim = 3; dim < 8; dim += 2){
        puzzle_dim = dim;
        double avg_time = 0;
        for(auto initial_state : generated_states[dim]){
            initial_state.depth = 0;
            avg_time += iterative_deepening(initial_state);
        }
        avg_time /= generated_states[dim].size();
        cout << dim << ' ' << avg_time << endl;
    }

    cout << "For a_star_search misplaced," << endl;
    for(int dim = 3; dim < 8; dim += 2){
        puzzle_dim = dim;
        double avg_time = 0;
        for(auto initial_state : generated_states[dim]){
            huristic_type = "misplaced";
            initial_state.depth = 0;
            initial_state.huristic_value = initial_state.huristic_misplaced();
            avg_time += iterative_deepening(initial_state);
        }
        avg_time /= generated_states[dim].size();
        cout << dim << ' ' << avg_time << endl;
    }

    cout << "For a_star_search manhattan," << endl;
    for(int dim = 3; dim < 8; dim += 2){
        puzzle_dim = dim;
        double avg_time = 0;
        for(auto initial_state : generated_states[dim]){
            huristic_type = "manhattan";
            initial_state.depth = 0;
            initial_state.huristic_value = initial_state.huristic_manhattan();
            avg_time += iterative_deepening(initial_state);
        }
        avg_time /= generated_states[dim].size();
        cout << dim << ' ' << avg_time << endl;
    }
}


