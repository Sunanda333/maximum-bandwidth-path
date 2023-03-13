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

struct maxHeap{
    int size=0;
    int* H;
    int* D;
    int* P;
};

maxHeap* mH;

//Function to create maxHeap
maxHeap* createMaxHeap(int num_nodes)
{
    maxHeap* mH = (maxHeap*) malloc(sizeof(maxHeap));
    mH->H = (int*)malloc(num_nodes * sizeof(int));
    mH->D = (int*)malloc(num_nodes * sizeof(int));
    mH->P = (int*)malloc(num_nodes * sizeof(int));
    for(int i=0;i<num_nodes;i++){
        mH->H[i]=-1;
        mH->D[i]=-1;
        mH->P[i]=-1;
    }
    return mH;
}

//Function to extract max element
int extraction(){
    if(mH->size==0){
        cout<<"Heap is empty, nothing to return";
        return -1;
    }
    return mH->H[0];
}

//Function to get size of the heap
int getSize(){
    return mH->size;
}

//Function to swap values in an array
void swap(int *v, int p1, int p2){
    int temp=v[p1];
    v[p1]=v[p2];
    v[p2]=temp;
}

//Function to heapify a node up in the heap
void heapifyUp(int i){
    int parent;
    if(i%2==0){
        parent = (i-2)/2;
    }
    else{
        parent = (i-1)/2;
    }

    while(i!=0){
        if(mH->D[mH->H[parent]]<mH->D[mH->H[i]]){
            int node1 = mH->H[parent];
            int node2 = mH->H[i];

            mH->H[i] = node1;
            mH->H[parent] = node2;

            int temp=mH->P[node1];
            mH->P[node1]=mH->P[node2];
            mH->P[node2]=temp;

            temp = mH->D[mH->H[i]];
            mH->D[mH->H[i]]=mH->D[mH->H[parent]];
            mH->D[mH->H[parent]]=temp;

            i=parent;
            if(i%2==0){
                parent = (i-2)/2;
            }
            else{
                parent = (i-1)/2;
            }
        }
        else{
            break;
        }
    }
}

//Function to insert a node in the heap
void insertion(int node, int bandwidth){
    // if(heap_debug>0){
    //     cout<<"inserting heap: node: "<<node<<" bandwidth: "<<bandwidth<<endl;
    // }
    if(mH->size==MAX_NODES){
        cout<<"Heap is full, can't insert more nodes";
        return;
    }
    mH->P[node]=mH->size;//P[5000]
    mH->H[mH->size]=node; //H[5000]
    mH->D[node] = bandwidth;//D[5000]
    heapifyUp(mH->size);
    mH->size++;
    // if(heap_debug){
    //     cout<<"after inserting: heap_array, node_to_indx, indx_to node:"<<endl;
    //     print_heap_array();
    // }
}

//Function to print the heap
void printHeap(){
    cout<<"Heap:"<<endl;
    for(int i=0;i<mH->size;i++){
        cout<<mH->H[i]<<" ";
    }
    cout<<endl;
    cout<<"Position: "<<endl;

    for(int i=0;i<mH->size;i++){
        cout<<mH->P[i]<<" ";
    }

    cout<<endl;
    cout<<"Bandwidth: "<<endl;
    for(int i=0;i<mH->size;i++){
        cout<<mH->D[i]<<" ";
    }
    cout<<endl;
}

//Function to heapify a node down the heap
void heapifyDown(int pos){
    int left=2*pos+1, right=2*pos+2;
    while((left<mH->size)||(right<mH->size)){
        int max_val_child=-1,max_child;
        if(left<mH->size && mH->D[mH->H[left]]>max_val_child){
            max_child=left;
            max_val_child=mH->D[mH->H[left]];
        }
        if(right<mH->size && mH->D[mH->H[right]]>max_val_child){
            max_child=right;
            max_val_child=mH->D[mH->H[right]];
        }

        if(max_val_child>mH->D[mH->H[pos]]){
            swap(mH->D, mH->H[pos], mH->H[max_child]);
            int node1 = mH->H[pos];
            int node2 = mH->H[max_child];

            mH->H[pos]=node2;
            mH->H[max_child]=node1;

            mH->P[node1]=max_child;
            mH->P[node2]=pos;

            left=2*max_child+1;
            right=2*max_child+2;
            pos=max_child;
        }
        else{
            break;
        }
    }
}

//Function to delete a node from the heap
void deletion(int node){
    int size= mH->size;
    if(size==0){
        cout<<"Heap is empty, can't delete more node";
        return;
    }
    int pos = mH->P[node];
    int node1 = mH->H[size-1];

    swap(mH->D, mH->H[pos], mH->H[size-1]);
    mH->D[mH->H[size-1]]=-1;

    mH->H[size-1]=-1;
    mH->H[pos]=node1;

    mH->P[node]=-1;
    mH->P[node1]=pos;

    mH->size--;

    heapifyDown(pos);
}

//Function to implement Dijkstra's algorithm using heaps
void maxBwHeap(int src, int target, int n)
{
    mH = createMaxHeap(n);
	string status[n];
	int dad[n]={-1}, bw[n]={INT_MAX};
	int x;
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
		insertion(temp->val, temp->weight);
		temp=temp->next;
	}

	while(getSize()>0)
	{
		int v = extraction();
		deletion(v);
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
				insertion(temp->val, bw[temp->val]);
			}
			else if(status[temp->val] == "fringe" && bw[temp->val] < min(bw[v], temp->weight))
			{
				bw[temp->val] = min(bw[v], temp->weight);
				dad[temp->val] = v;
				deletion(temp->val);
				insertion(temp->val, bw[temp->val]);
			}
			temp = temp->next;
		}
	}
	int count;
	int *path;
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
		while(x!=src)
		{
			path[i++] = x;
			x = dad[x];
		}
		path[i] = x;
	}
	printPath(count,path);
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
    for(int i=0;i<5;i++)
    {
        cout<<"***************new****************"<<endl;
        int src = rand()%4999;
        int target = rand()%4999;
        while(src==target)
            target = rand()%4999;
        auto start1 = high_resolution_clock::now();
        maxBwHeap(src,target,V);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        cout<<endl<<"Dijkstra's with heap "<< duration1.count()<<" microseconds"<<endl;
    }
    return 0;   
}

