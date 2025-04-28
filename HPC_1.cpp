#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

const int N = 100;
vector<int> graph[N];
bool visited_bfs[N] = {false};
bool visited_dfs[N] = {false};

void parallel_bfs(int start){
    queue<int> q;
    q.push(start);
    visited_bfs[start] = true;
    
    while(!q.empty()){
        int size = q.size();
        vector<int> current;
        
        while(size--){
            int node = q.front();
            q.pop();
            cout<<node<<" ";
            current.push_back(node);
        }
        
        #pragma omp parallel for
        for(int i = 0;i<current.size();i++){
            int node = current[i];
            for(int neighbor:graph[node]){
                bool push = false;
                
                #pragma omp critical
                {
                    if(!visited_bfs[neighbor]){
                        visited_bfs[neighbor] = true;
                        push = true;
                    }
                }
                
                if(push){
                    #pragma omp critical
                    q.push(neighbor);
                }
            }
        }
    }
}

void parallel_dfs(int node){
    bool skip = false;
    
    #pragma omp critical
    {
        if(visited_dfs[node]){
            skip = true;
        }else{
            visited_dfs[node] = true;
            cout<<node<<" ";
        }
    }
    
    if (skip) return;
    
    #pragma omp parallel for
    for(int i = 0;i<graph[node].size();i++){
        parallel_dfs(graph[node][i]);
    }
}

int main(){
    int nodes, edges;
    cout<<"Enter number of nodes and edges : ";
    cin>>nodes>>edges;
    cout<<"Enter the edges : "<<endl;
    for(int i = 0;i<edges;i++){
        int u,v;
        cin>>u>>v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    
    cout<<"\nParallel BFS travarsal : ";
    parallel_bfs(0);
    
    cout<<"\nParallel DFS travarsal : ";
    parallel_dfs(0);
}