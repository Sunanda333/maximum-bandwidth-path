#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

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

// Function to pick the largest fringe 
int largest_fringe(int bw[], string status[], int n)
{
    int maxbw = INT_MIN, maxid;
    for(int  i = 0; i<n; i++)
    {
        if(status[i] == "fringe" && maxbw < bw[i])
        {
            maxbw = bw[i];
            maxid = i;
        }
    }
    return maxid;
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

//Function to find out the maximum bandwidth path using Dijkstra's algorithm
void maxBw(int src, int target, int n)
{
	string status[n];
	int dad[n]={-1}, bw[n]={INT_MAX};
	int x, count=0;

	for (int v = 0; v < n; v++)
	    status[v] = "unseen";

	status[src] = "in-tree";

	listNode *temp;
	temp=graph->adj[src].head;

	while(temp)
	{
		status[temp->val] = "fringe";
		dad[temp->val] = src;
		bw[temp->val] = temp->weight;
		count++;
		temp=temp->next;
	}

	while(count--)
	{
		int v = largest_fringe(bw,status,n);
		status[v] = "in-tree";
		if (v == target)
		    break;
		temp = graph->adj[v].head;
		while(temp)
		{
			if(status[temp->val] == "unseen")
			{
				status[temp->val] = "fringe";
				bw[temp->val] = min(bw[v], temp->weight); 
				dad[temp->val] = v;
				count++;
			}
			else if(status[temp->val] == "fringe" && bw[temp->val] < min(bw[v], temp->weight))
			{
				bw[temp->val] = min(bw[v], temp->weight);
				dad[temp->val] = v;
			}
			temp = temp->next;
		}
	}
    
    int* path;
	if(status[target] == "in-tree")
	{
	    count = 1;
		x = target;
		while(x!=src)
		{
			count++;
			x = dad[x];
		}
		path = (int*)malloc(count * sizeof(int));
		x = target;
		int i = 0;
		int k=0;
		while(x!=src)
		{
			path[i++] = x;
			x = dad[x];
		}
		path[i] = x;
	}
	printPath(count, path);
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
        auto start = high_resolution_clock::now();
        maxBw(src,target,V);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout<<endl<<"Dijkstra's with array "<<duration.count()<<" microseconds"<<endl;
    }
    return 0;   
}
