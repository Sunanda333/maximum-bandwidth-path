#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

#define MAX_NODES 5000

struct listNode
{
    int val;
    int weight;
    listNode* next;
};

struct adjList
{
   listNode *head;
};

struct Graph
{
    int V;
    adjList* adj;
};

Graph* graph;
int** edge;

//Function to create a graph
Graph* createGraph(int V)
{
    int i;
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->V = V;
    
    // Create an array of adjacency lists.
    // Size of array will be V
    graph->adj = (adjList*) malloc(V * sizeof(adjList));

    // Initialize each adjacency list
    // as empty by making head as NULL
    for (i = 0; i < V; i++)
        graph->adj[i].head = NULL;
    return graph;
}

//Function to create a node of the adjacency list
void createNode(int i, int val, int weight)
{
    listNode* new_node = (listNode*)malloc(sizeof(listNode));
    new_node->val = val;
    new_node->weight = weight;
    new_node->next = graph->adj[i].head;
    graph->adj[i].head = new_node;
}

// A function to create the adjacency list from set of edges
void genGraph(int E, int V)
{
    int i;
    for (i = 0; i<E; i++)
    {
        createNode(edge[i][0],edge[i][1],edge[i][2]);
        createNode(edge[i][1],edge[i][0],edge[i][2]);
    }
}

// Function to generate a set of edges of a random graph
void GenerateRandGraphs(int E, int V)
{
	int i, j, w;
	i = 0;
	while(i<V)
	{
	    w = rand() % 50 + 1;
	    edge[i][0] = i;
	    edge[i][1] = (i+1)%V;
	    edge[i][2] = w;
	    i++;
	}

	srand(rand() + (int) time(NULL));
	while(i < E)
	{
	    w = rand() % 50 + 1;
		edge[i][0] = rand()%(V-1);
		edge[i][1] = rand()%(V-1);
		edge[i][2] = w;

		if(edge[i][0] == edge[i][1])
			continue;
		else
		{
			for(j = 0; j < i; j++)
			{
				if((edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] && edge[i][1] == edge[j][0]))
					i--;
			}
		}
		i++;
	}
	genGraph(E, V);
}

//Function to print the graph
void printgraph(int n)
{
    for (int i = 0; i< n; i++)
    {
        cout<<"Node "<<i<<" is connected to"<<endl;
        while(graph->adj[i].head)
        {
            cout<<graph->adj[i].head->val<<" with weight "<<graph->adj[i].head->weight<<endl;
            graph->adj[i].head = graph->adj[i].head->next;
        }
    }
}

//Function to print the path
void printPath(int count, int* path)
{
    if (path == NULL)
    {
        cout<<"No s-t path found";
        return;
    }
    else
    {
        cout<<"Path from source to target is"<<endl;
        for (int i=count-1; i>=0; i--)
    	    cout<<path[i]<<" ";
    }
}

struct union_find{
    int* parent;
    int* rank;
};

union_find* uf;

//Function to create a union find structure 
union_find* createUf(int nodes)
{
    union_find* uf = (union_find*) malloc(sizeof(union_find));
    uf->parent=(int *)malloc(sizeof(int)*nodes);
    uf->rank=(int *)malloc(sizeof(int)*nodes);
    for(int i=0;i<nodes;i++){
        uf->parent[i]=-1;
        uf->rank[i]=0;
    }
    return uf;
}

//Function to swap two edges
void swapping(int i, int j){
    int i0 = edge[i][0];
    int i1 = edge[i][1];
    int i2= edge[i][2];
    edge[i][0]= edge[j][0];
    edge[i][1]= edge[j][1];
    edge[i][2]= edge[j][2];
    edge[j][0]=i0;
    edge[j][1]=i1;
    edge[j][2]=i2;
}

//Function to find the parent of a node
int find(int u){
    if(uf->parent[u]==-1){
        return u;
    }
    uf->parent[u]=find(uf->parent[u]);
    return uf->parent[u];
}

//Function for the union of two nodes
void custom_union(int u, int v){
    int p1=find(u);
    int p2=find(v);

    if(uf->rank[p1]==uf->rank[p2]){
        uf->rank[p1]++;
        uf->parent[p2]=p1;
    }
    else if(uf->rank[p1]<uf->rank[p2]){
        uf->parent[p1]=p2;
    }
    else{
        uf->parent[p2]=p1;
    }
}

//Function to check if cycle exists
bool check_cycle(int u, int v){

    int p1=find(u);
    int p2=find(v);
    if(p1==p2){
        return true;
    }
    return false;
}

//Function to heapify a node up the heap
void fix_heap_up(int size){
    int i=size,parent;
    if(i%2==0){
        parent = (i-2)/2;
    }
    else{
        parent = (i-1)/2;
    }

    while(i>0){
        if(edge[parent][2]<edge[i][2]){
            int x = parent;
            swapping(x, i);
            i=parent;
            parent = (i - 1) / 2;
        }
        else{
            break;
        }
    }

}

//Function to heapify a node down the heap
void fix_heap_down(int size){
    int pos=0;

    int left=2*pos+1, right=2*pos+2;

    while((left<size)||(right<size)){

        int max=-1;
        if(left>=size){
            max=right;
        }
        else if(right >= size){
            max=left;
        }
        else if(edge[left][2]>edge[right][2]){
            max=left;
        }
        else{
            max=right;
        }

        if(edge[pos][2]<edge[max][2]){
            int x = max;
            swapping(pos,x);
            pos=max;
            left=2*pos+1;
            right=2*pos+2;
        }
        else{
            break;
        }

    }

}

//Function to heapsort the edges
void sort(int num_edges){
    auto start = high_resolution_clock::now();
    for(int i=1;i<num_edges;i++){
        fix_heap_up(i);
    }
    for(int i=0;i<num_edges;i++){
        swapping(0,num_edges-1-i);
        fix_heap_down(num_edges-1-i);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout<<"Sorting alone takes "<< duration.count()<<" microseconds"<<endl;
}

//Function to populate the path 
bool backtrack(int i, int &t, int* path, int &pos, int *&visited, listNode**&gh)
{
    visited[i]=1;
    path[pos]=i;
    pos++;

    if(i==t){
        return true;
    }

    for(auto j=gh[i];j!=NULL;j=j->next){
        int v=j->val;
            if(visited[v]==0){

                if(backtrack(v,t,path,pos,visited, gh))
                {
                    return true;
                }
            }
    }

    pos--;
    return false;

}

//Function to generate the maximum bandwidth path using Kruskal's algorithm and Heapsort
void maxBwKruskal(int num_edges, int num_nodes, int s, int t)
{
    sort(num_edges);
    uf = createUf(num_nodes);
    listNode** gh = (listNode**)malloc(num_nodes * sizeof(listNode*));
    int *visited = (int*)malloc(num_nodes * sizeof(int));
    for(int i=0;i<num_nodes;i++){
        visited[i]=0;
        gh[i]=NULL;
    }

    for(int i=num_edges-1;i>=0;i--){
        int u=edge[i][0];
        int v=edge[i][1];
        int bw=edge[i][2];
        if(!check_cycle(u,v)){
            custom_union(u,v);
            if(gh[u]==NULL){
                listNode* t = (listNode*)malloc(sizeof(listNode));
                t->val=v;
                t->weight=bw;
                t->next=NULL;
                gh[u]=t;
            }
            else{
                listNode* temp = (listNode*)malloc(sizeof(listNode));
                temp->val=v;
                temp->weight=bw;
                temp->next=gh[u];
                gh[u]=temp;
            }

            if(gh[v]==NULL){
                listNode* t = (listNode*)malloc(sizeof(listNode));
                t->val=u;
                t->weight=bw;
                t->next=NULL;
                gh[v]=t;
            }
            else{
                listNode* temp = (listNode*)malloc(sizeof(listNode));
                temp->val=u;
                temp->weight=bw;
                temp->next=gh[v];
                gh[v]=temp;
            }

        }
    }

    int* path = (int *)malloc(sizeof(int)*num_nodes);
    int pos=0;
    
    backtrack(s, t, path, pos, visited, gh);

    for(int i=0;i<num_nodes;i++){
        auto j=gh[i];
        for(j=gh[i];j!=NULL;)
        {
            auto temp=j->next;
            delete j;
            j=temp;
        }
    }
    printPath(pos, path);
}

int main()
{
    int V = 5000;
    int E = (5000 * 6)/2;
    graph = createGraph(V);
    edge = (int**)malloc(E * sizeof(int*));
    for(int i = 0; i < E; i++)
		edge[i] = (int*)malloc(3 * sizeof(int));
    GenerateRandGraphs(E, V);
    // printgraph(V);
 
    for(int i=0;i<5;i++)
    {
        cout<<"***************new****************"<<endl;
        int src = rand()%4999;
        int target = rand()%4999;
        while(src==target)
            target = rand()%4999;
        auto start2 = high_resolution_clock::now();
        maxBwKruskal(E,V,src,target);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        cout<<endl<<"Kruskal's with heapsort"<<duration2.count()<<" microseconds"<<endl;
    }
    return 0;   
}

