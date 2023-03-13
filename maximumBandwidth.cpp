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
        auto start = high_resolution_clock::now();
        maxBw(src,target,V);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout<<endl<<"Dijkstra's with array "<<duration.count()<<" microseconds"<<endl;
        auto start1 = high_resolution_clock::now();
        maxBwHeap(src,target,V);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        cout<<endl<<"Dijkstra's with heap "<< duration1.count()<<" microseconds"<<endl;
        auto start2 = high_resolution_clock::now();
        maxBwKruskal(E,V,src,target);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        cout<<endl<<"Kruskal's with heapsort"<<duration2.count()<<" microseconds"<<endl;
    }
    return 0;   
}
