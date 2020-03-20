#include <iostream>
#include <list>
#include <iterator>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include<bits/stdc++.h>
#include <string>

using namespace std;

class board	{
	public:
		int N;
		int numSnakes;
		int numLadders;
		vector<vector<int>> adj;

		board(int N, int numLadders, int numSnakes)	{
			this->N = N;
			this->numLadders = numLadders;
			this->numSnakes = numSnakes;
			adj.resize(N*N + 1);
			for(int i=0; i<adj.size(); i++)	{
				for(int j = 0; j<adj[i].size(); j++)	{
					adj[i][j] = 0;
				}
			}
		}

		~board()	{
			N = 0;
			numLadders = 0;
			numSnakes = 0;
			for(int i = 0; i<adj.size(); i++)	{
				adj[i].clear();
			}
			adj.clear();
		}

		void add_edge(int src, int dest)	{ 
    		adj[src].push_back(dest); //Directional
		} 
};

bool BFS(vector<vector<int>> adj, int src, int dest, int v, int pred[], int dist[])	{ 
    list<int> queue = {}; 
    bool visited[v] = {false}; 
 
    for (int i = 0; i < v; i++) { 
        visited[i] = false; 
        dist[i] = INT_MAX; 
        pred[i] = -1; 
    } 

    visited[src] = true; 
    dist[src] = 0; 
    queue.push_back(src); 

    while (!queue.empty())	{ 
        int u = queue.front(); 
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++)	{
            if (visited[adj[u][i]] == false) { 
                visited[adj[u][i]] = true;
                dist[adj[u][i]] = dist[u] + 1; 
                pred[adj[u][i]] = u; 
                cout << adj[u][i] << endl;
                queue.push_back(adj[u][i]);
                if (adj[u][i] == dest)	{
                	return true; 
                }
            }
        }
    }

    return false; 
}

void printShortestPath(vector<vector<int>> adj, int s, int dest, int v)	{ 
    int pred[v], dist[v] = {0};
 
    if (BFS(adj, s, dest, v, pred, dist) == false) 
    { 
        cout << "Given source and destination"
             << " are not connected"; 
        return; 
    } 

    vector<int> path;
    int crawl = dest;
    path.push_back(crawl);
    while (pred[crawl] != -1) { 
        path.push_back(pred[crawl]); 
        crawl = pred[crawl]; 
    }

    for (int i = path.size() - 1; i >= 0; i--) {
    	if((i > 0) && (abs(path[i]-path[i-1]) > 6))	{ //This means this jump was a snake/ladder. Adjust accordingly
    		if(i == path.size()-1)	{ //Because you make a move on a ladder at the start position, you must offset distance by +1 bc the algorithm will still decrease by 1 at the end
    			dist[dest]++;
    		}
    		while((i > 0) && (abs(path[i]-path[i-1]) > 6))	{
    			i--;
    		}
    		dist[dest]--;
    	}
    }

    cout << dist[dest] << endl; 

    for (int i = path.size() - 1; i >= 0; i--) {
    	if((i > 0) && (abs(path[i]-path[i-1]) > 6))	{ //This means this jump was a snake/ladder. Adjust accordingly
    		while((i > 0) && (abs(path[i]-path[i-1]) > 6))	{
    			if(i == path.size()-1)	{ //If snake/ladder occurs on first position
	    			cout << "1 ";
	    		}
	    		if(path[i-1]-path[i] < 0)	{ //Snake
	    			cout << path[i] << "-" << path[i-1] << " ";
	    		}
	    		else if(path[i-1]-path[i] > 0)	{ //Ladder
	    			cout << path[i] << "+" << path[i-1] << " ";
	    		}
	    		i--;
    		}
    	}
    	else	{
    		cout << path[i] << " "; 
    	}
    }
}

void shortestPath(board b)	{
	int num_vertices = b.N*b.N;
	printShortestPath(b.adj, 1, num_vertices, num_vertices); //Destination = position at num_vertices (N*N)
}

//Assume only one board for now, adapt to multiple later
vector<board> parseInput(string input)	{
	stringstream stream(input);
	stringstream s;
	string line;
	string n_filler;
	int N, numLadders, numSnakes, ladderStart, ladderEnd, snakeStart, snakeEnd, array_len, num_boards = 0;
	bool snake = 0;

	getline(stream, line, '\\');
	getline(stream, n_filler, 'n');
	s = stringstream(line);
	s >> num_boards;

	vector<board> board_list;

	for(int k = 0; k<num_boards; k++)	{ //Repeat process num_boards times
		getline(stream, line, '\\');
		getline(stream, n_filler, 'n');
		s = stringstream(line);
		s >> N;
		s >> numLadders;
		s >> numSnakes;
		board b(N, numLadders, numSnakes);
		getline(stream, line, '\\');
		getline(stream, n_filler, 'n');
		s = stringstream(line);
		for(int i = 0; i<numLadders; i++)	{ //Create ladders for the board
			s >> ladderStart;
			s >> ladderEnd;
			b.add_edge(ladderStart, ladderEnd);
		}
		getline(stream, line, '\\');
		getline(stream, n_filler, 'n');
		s = stringstream(line);
		for(int i = 0; i<numSnakes; i++)	{ //Create snakes for the board
			s >> snakeStart;
			s >> snakeEnd;
			b.add_edge(snakeStart, snakeEnd);
		}
		for(int i = 1; i<(N*N + 1); i++)	{ //Create normal connections for the board from pos 1 to N*N  (i.e. die rolls)
			for(int j = 0; j<b.adj[i].size(); j++)	{
				if(b.adj[i][j] < i )	{ //This means a snake exists. Don't add any new connections to this node bc you must take the snake down
					snake = true;
				}
			}
			if(snake)	{ //Current node is a snake, skip adding edges bc snake path must be taken
				snake = false;
				continue;
			}
			for(int j = 6; j>0; j--)	{
				if((i+j) <= N*N)	{
					b.add_edge(i, i+j);
				}
			}
		}

		board_list.push_back(b);
	}
	return board_list;
	
}


int main(int argc, char const *argv[])
{
	if(argc != 2)	{
		cout << "Please only enter a string of numbers and new lines as a parameter";
		exit(0);
	}
	string input(argv[1]);
	vector<board> b_list = parseInput(input);
	for(int i = 0; i<b_list.size(); i++)	{
		cout << "Board Game #" << i+1 << ":\n";
	 	shortestPath(b_list[i]);
	 	cout << "\n";
	}

	//Memory Management
	return 0;
}