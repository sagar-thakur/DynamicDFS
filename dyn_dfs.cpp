#include <bits/stdc++.h>
#include <iostream>
#include <vector>
 
#define MAXNODES 10000
 
using namespace std;
 
struct node {
    vector<int> back_edges;
    int parent;
    int level;
    vector<int> list;
};

vector<int> vertices(MAXNODES);
vector< struct node > adjList(MAXNODES);
int n,m; //n=no. of nodes, m=no. of edges
vector<int> start_time(MAXNODES);
vector<int> finish_time(MAXNODES);
vector<int> in_deg(MAXNODES, 0);
vector<int> out_deg(MAXNODES, 0);
vector<int> parent(MAXNODES);
vector<int> level(MAXNODES);    //level(r) = 0 and level(x) =level(par(x)) + 1.
vector<int> permut(MAXNODES, INT_MAX);
vector<int> LCA_visited(MAXNODES, 0);
vector<int> visited(MAXNODES, 0);
int root;

void update_level(int z, int level, int v) {
    if(z == v)
        return;
    visited[z] = 1;
    adjList[z].level = level;
    vector<int>::iterator itr = adjList[z].list.begin();
    for(;itr != adjList[z].list.end(); itr++) {
        if(!visited[*itr]) {
            update_level(*itr, level+1, v);
        }
    }
    return;
}

void reroot(int v, int x, int y, queue< pair<int, int> > &q) {
    int new_parent = x;
    int z = y;
    int last_node = adjList[v].parent;
    while(z != last_node) {
        if(z != v) {
            // Collect new cross edges
            vector<int>::iterator itr = adjList[z].back_edges.begin();
            for(; itr != adjList[z].back_edges.end() ; itr++) {
                q.push(make_pair(*itr, adjList[z].parent));
            }
        }

        adjList[z].back_edges.clear();
        int next = adjList[z].parent;
        adjList[z].level = adjList[new_parent].level + 1;
        for(int i=0; i<MAXNODES; i++)
                visited[i] = 0;
        visited[new_parent] = 1;
        update_level(z, adjList[z].level, v);
        adjList[z].parent = new_parent;
        new_parent = z;
        z = next;
    }

    adjList[x].list.push_back(y);
    adjList[y].list.push_back(x);
}

int level_ancestor(int y, int level) {
    int cur_level = adjList[y].level;
    while(cur_level != level) {
        y = adjList[y].parent;
        cur_level--;
    }

    return y;
}
 
int LCA(int u, int v)
{
    //to start LCA from node which is nearer to root
    if(adjList[u].level > adjList[v].level) // check if u is below in terms of level
    {
        int tmp = u;
        u = v;
        v = tmp;
    }
 
    //mark parents/ancestors of u
    LCA_visited[u] = 1;
    while(u != root)
    {
        LCA_visited[adjList[u].parent] = 1;
        u = adjList[u].parent;
    }
    //  LCA_visited[u] = 1; //for root
 
    while(v != root)
    {
        if(LCA_visited[v] == 1)
        {
            break;
        }
        v = adjList[v].parent;
    }
 
    // clear LCA_visited because it is global
    for(int i=0; i<MAXNODES; i++)
        LCA_visited[i] = 0;
    return v;
}

void print_dfs(int root) {
    visited[root] = 1;
    cout << root << " ";
    vector<int>::iterator itr = adjList[root].list.begin();
    for(;itr != adjList[root].list.end(); itr++) {
        if(!visited[*itr]) {
            print_dfs(*itr);
        }
    }
    return;
}

int main()
{
    int x,y;

    // Insert the first edge
    cout<<"Graph input format:\n|S| |D|\nu v\n";
    cout<<"Insert edge:\n";
    cin>>x>>y;
    root = x;
    adjList[x].list.push_back(y);
    adjList[y].parent = x;
    adjList[x].level = 0;
    adjList[y].level = 1;
    adjList[x].parent = x;
    adjList[y].list.push_back(x);

    while(1) {
        int x,y;
        cout<<"Insert edge:\n";
        cin>>x>>y;

        // Edge with a new node
        if(adjList[y].parent == 0 || adjList[x].parent == 0) {
            adjList[x].list.push_back(y);
            adjList[y].parent = x;
            adjList[y].level = adjList[x].level + 1;
            adjList[y].list.push_back(x);

            cout << endl << "====== DFS ======" << endl;
            for(int i=0; i<MAXNODES; i++)
                visited[i] = 0;
            print_dfs(root);
            cout << endl << "=================" << endl << endl;
            continue;
        }

        // Make the lowest level as y
        if(adjList[y].level > adjList[x].level) {
            int temp = x;
            x = y;
            y = temp;
        }

        int w = LCA(x,y);
        
        if(w == x) { // Forward edge add to tree
            adjList[x].list.push_back(y);
            adjList[y].parent = x;
            adjList[y].level = adjList[x].level + 1;
            adjList[y].list.push_back(x);
        } else if (w != y) { // Cross edge, reroot
            queue< pair<int, int> > q;

            q.push(make_pair(x,y));

            while(!q.empty()) {
                int x = q.front().first;
                int y = q.front().second;
                q.pop();
                int w = LCA(x,y);
                int v = level_ancestor(y, adjList[w].level + 1);
                int u = level_ancestor(x, adjList[w].level + 1); 
                cout << "Cross edge found, Rerooting T(" << v << ") through edge (" << x << "," << y << ")" << endl;
                reroot(v, x, y, q);


                // Back edges originating in T(v) will now be originating in T(u)
                adjList[u].back_edges.insert(adjList[u].back_edges.end(), adjList[v].back_edges.begin(), adjList[v].back_edges.end());
                adjList[v].back_edges.clear();

                // Since edge v, w is now a back edge remove it from adjacent list
                // and add it to backedges list of u
                adjList[w].list.erase(remove(adjList[w].list.begin(), adjList[w].list.end(), v));
                adjList[u].back_edges.push_back(v);

                adjList[v].list.erase(remove(adjList[v].list.begin(), adjList[v].list.end(), w));
            }

        } else { // Back edge if w = y, update the back edges
            cout << "Back-edge found from " << x << " to " << y << endl;
            int z = x;
            while(adjList[z].parent != y) {
                z = adjList[z].parent;
            }
            // Back edge originted from x and terminated on par(z) == y
            adjList[z].back_edges.push_back(x);
        }

        cout << endl << "====== DFS ======" << endl;
        for(int i=0; i<MAXNODES; i++)
                visited[i] = 0;
        print_dfs(root);
        cout << endl << "=================" << endl << endl;
    }

   
 
    return 0;
}