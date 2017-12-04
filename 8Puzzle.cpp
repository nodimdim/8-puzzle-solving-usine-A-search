#include <bits/stdc++.h>


using namespace std;

string huristic_type, algorithm;
int puzzle_dim;

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

map <unsigned long long, state> parent;


void dfs(state initial_state){
    double start = clock();
    visited.clear();
    parent.clear();
    stack <state> s;
    s.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!s.empty()){
        state u = s.top();
        if(u.is_terminal){
            double finish = clock();
            cout << "\n1. Total visited states: " << visited.size() << endl;
            cout << "2. Solution found at depth: " << u.depth << endl;
            cout << "3. Time taken to solve: " << (finish - start) / CLOCKS_PER_SEC << endl;
            stack <state> path;
            while(1){
                path.push(u);
                if(u.hash_val == initial_state.hash_val) break;
                u = parent[u.hash_val];
            }
            cout << "A sequence of moves: " << endl << endl;
            while(!path.empty()){
                u = path.top();
                path.pop();
                u.print_state();
            }
            return;
        }
        s.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                s.push(v);
                parent[v.hash_val] = u;
                visited.insert(v.hash_val);
            }
        }
    }
}


void iterative_deepening(state initial_state){
    double start = clock();
    int depth = 0;
    while(1){
        visited.clear();
        parent.clear();
        stack <state> s;
        s.push(initial_state);
        visited.insert(initial_state.hash_val);
        while(!s.empty()){
            state u = s.top();
            if(u.is_terminal){
                double finish = clock();
                cout << "\n1. Total visited states: " << visited.size() << endl;
                cout << "2. Solution found at depth: " << u.depth << endl;
                cout << "3. Time taken to solve: " << (finish - start) / CLOCKS_PER_SEC << endl;
                stack <state> path;
                while(1){
                    path.push(u);
                    if(u.hash_val == initial_state.hash_val) break;
                    u = parent[u.hash_val];
                }
                cout << "A sequence of moves: " << endl << endl;
                while(!path.empty()){
                    u = path.top();
                    path.pop();
                    u.print_state();
                }
                return;
            }
            s.pop();
            for(auto v : u.successors()){
                if(visited.find(v.hash_val) == visited.end() && v.depth <= depth){
                    s.push(v);
                    parent[v.hash_val] = u;
                    visited.insert(v.hash_val);
                }
            }
        }
        depth++;
    }
}

void bfs(state initial_state){
    double start = clock();
    visited.clear();
    parent.clear();
    queue <state> q;
    q.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!q.empty()){
        state u = q.front();
        if(u.is_terminal){
            double finish = clock();
            cout << "\n1. Total visited states: " << visited.size() << endl;
            cout << "2. Solution found at depth: " << u.depth << endl;
            cout << "3. Time taken to solve: " << (finish - start) / CLOCKS_PER_SEC << endl;
            stack <state> path;
            while(1){
                path.push(u);
                if(u.hash_val == initial_state.hash_val) break;
                u = parent[u.hash_val];
            }
            cout << "A sequence of moves: " << endl << endl;
            while(!path.empty()){
                u = path.top();
                path.pop();
                u.print_state();
            }
            return;
        }
        q.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                q.push(v);
                parent[v.hash_val] = u;
                visited.insert(v.hash_val);
            }
        }
    }
}

void a_star_search(state initial_state){
    double start = clock();
    visited.clear();
    parent.clear();
    priority_queue <state, vector <state>, greater<state> > pq;
    pq.push(initial_state);
    visited.insert(initial_state.hash_val);
    while(!pq.empty()){
        state u = pq.top();
        if(u.is_terminal){
            double finish = clock();
            cout << "\n1. Total visited states: " << visited.size() << endl;
            cout << "2. Solution found at depth: " << u.depth << endl;
            cout << "3. Time taken to solve: " << (finish - start) / CLOCKS_PER_SEC << endl;
            stack <state> path;
            while(1){
                path.push(u);
                if(u.hash_val == initial_state.hash_val) break;
                u = parent[u.hash_val];
            }
            cout << "A sequence of moves: " << endl << endl;
            while(!path.empty()){
                u = path.top();
                path.pop();
                u.print_state();
            }
            return;
        }
        pq.pop();
        for(auto v : u.successors()){
            if(visited.find(v.hash_val) == visited.end()){
                pq.push(v);
                parent[v.hash_val] = u;
                visited.insert(v.hash_val);
            }
        }
    }
}


int main(){
    // freopen("8puzzle.txt", "w", stdout);

    cout << "L-puzzle sovler" << endl << endl;
    int test_case = 1;
    while(1){
        cout << "Please input name of the algorithm that you want to use." << endl;
        cout << "1. bfs" << endl;
        cout << "2. dfs" << endl;
        cout << "3. iterative_deepening" << endl;
        cout << "4. a_star_search" << endl << endl << ">> ";
        cin >> algorithm;
        if(algorithm == "a_star_search"){
            cout << endl << "Please input name of the heuristic you want to use." << endl;
            cout << "1. misplaced" << endl;
            cout << "2. manhattan" << endl << endl << ">> ";
            cin >> huristic_type;
        }
        cout << endl << "Please input the dimension of the puzzle board." << endl << endl << ">> ";
        cin >> puzzle_dim;
        vector < vector <int> > initial_board(puzzle_dim, vector <int> (puzzle_dim));

        cout << endl << "Please input the configuration of the initial puzzle board." << endl << endl << ">> ";
        for(int i = 0; i < puzzle_dim; i++){
            for(int j = 0; j < puzzle_dim; j++){
                cin >> initial_board[i][j];
            }
        }
        state initial_state = state(initial_board, 0);

        cout << "Test Case " << test_case++ << ":" << endl << endl;
        cout << "Initial game board is:" << endl;
        initial_state.print_state();
        if(algorithm == "bfs"){
            cout << "Using bfs, " << endl;
            bfs(initial_state);
        }
        else if(algorithm == "dfs"){
            cout << "Using dfs, " << endl;
            dfs(initial_state);
        }
        else if(algorithm == "iterative_deepening"){
            cout << "Using iterative deepening, " << endl;
            iterative_deepening(initial_state);
        }
        else if(algorithm == "a_star_search"){
            cout << "Using A* search with " << huristic_type << " heuristic, "<< endl;
            a_star_search(initial_state);
        }
    }
}


/*

dfs
3
3 4 6
1 0 8
7 2 5

iterative_deepening
3
3 4 6
1 0 8
7 2 5

a_star_search
misplaced
3
3 4 6
1 0 8
7 2 5

a_star_search
manhattan
3
3 4 6
1 0 8
7 2 5

dfs
3
3 4 6
1 0 8
7 2 5
*/
