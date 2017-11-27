/**
Author @Aitto_sang
CSE@Buet
*/
#include<stdio.h>
#include<iostream>
//#include<cmath>
//#include<cstdlib>
//#include<algorithm>
//#include<stack>
//#include<cstring>
//#include<bits/stdc++.h>

#define lli long long int
#define pf printf
#define sf scanf
#define pi 3.141592653589793116
#define inf 1e9
#define linf 1e15
#define NULL_VALUE -999999
#define maxNEG -99999999.0
#define debug cout<<"Hello here!"<<endl;
#define readFile freopen("in.txt","r",stdin);
#define syncronize ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

using namespace std;

void bfs(int **graph,int n,int source,int *parent,int *visit);
void dfs(int **graph,int *visited,int n,int source);

class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue()
    {
        queueInitSize = 2 ;
        queueMaxSize = queueInitSize;
        data = new int[queueMaxSize] ; //allocate initial memory
        length = 0 ;
        front = 0;
        rear = 0;
    }

    ~Queue(){
        if(data) delete[] data; //deallocate memory
        data = 0; //set to NULL
    }

    void enqueue(int item){
        if (length == queueMaxSize)
        {
            int * tempData ;
            //allocate new memory space for tempList
            queueMaxSize = 2 * queueMaxSize ;
            tempData = new int[queueMaxSize] ;
            int i, j;
            j = 0;
            for( i = rear; i < length ; i++ )
            {
                tempData[j++] = data[i] ; //copy items from rear
            }
            for( i = 0; i < rear ; i++ )
            {
                tempData[j++] = data[i] ; //copy items before rear
            }
            rear = 0 ;
            front = length ;
            delete[] data ; //free the memory allocated before
            data = tempData ; //make list to point to new memory
        }

        data[front] = item ; //store new item
        front = (front + 1) % queueMaxSize ;
        length++ ;
    } //insert item in the queue


    int dequeue(){
        if(length == 0) return NULL_VALUE ;
        int item = data[rear] ;
        rear = (rear + 1) % queueMaxSize ;
        length-- ;
        return item ;
    } //returns the item according to FIFO

        bool empty(){
        if(length == 0) return true ;
        else return false ;
    } //return true if Queue is empty
};

void initialize(int *p,int n,int val){
    for(int i=0;i<n;i++) p[i]=val;
}

//Finds if there exists a path from source to sink
bool existAPath(int *p,int source,int sink){
    //Finding through parent
    while(1){
        if(source==sink) return true;
        if(sink==-1) return false;
        sink=p[sink];
    }
}

//finding minimum in the path
int minflow(int **graph,int *p,int source,int sink){
    int maxFlow=inf;

    while(1){
        if(source==sink) return maxFlow;
        if( graph[p[sink]][sink] < maxFlow ) maxFlow=graph[p[sink]][sink];
        sink=p[sink];
    }
}

//Making residual graph
void makeResidual(int **graph,int **flow,int **residual,int n){

    for(int i=1;i<=n;i++)
    for(int j=1;j<=n;j++){

        // Residual capacity = graph-flow
        if(graph[i][j]!=inf) {
            residual[i][j]=graph[i][j]-flow[i][j];
            // If there is a reverse path
            if(graph[j][i]==inf and flow[i][j]!=0) {
                // two paths between two vertices
                residual[j][i]=flow[i][j];
            }else residual[j][i]=graph[j][i] + flow[i][j];
        }
        // If residual is zero, means no flow from i to j, which means no path
        if(residual[i][j]==0) residual[i][j]=inf;
    }
}

//Prints a 2D array from s to n
void print(int **arr,int s,int n){
    for(int i=s;i<=n;i++){
        for(int j=s;j<=n;j++){
            if(arr[i][j]==inf) pf("INF ");
            else pf("%3d ",arr[i][j]);
        }
        pf("\n");
    }
}

//Prints a 1D array from s to n
void print(int *arr,int s,int n){
    for(int i=s;i<=n;i++){
        cout<<i<< " " <<arr[i]<<endl;
    }
    pf("\n");
}

// Making flow network
void makeFlow(int **graph,int **flow,int *parent,int source,int sink,int value){

    while(1){
        if(source==sink) return;
        if(graph[parent[sink]][sink]!=inf)
            flow[parent[sink]][sink]+=value;
        else
            flow[parent[sink]][sink]-=value;
        sink=parent[sink];
    }

}

void fordFulkerson(int **graph,int **flow,int **residual,int n,int source,int sink){
    int *parent,*visit;
    parent=new int[n +1];
    visit=new int[n +1];

    int maxflow=0;

    //Initially making residual graph and finding augmented path
    makeResidual(graph,flow,residual,n);
    bfs(residual,n,source,parent,visit);

    while(existAPath(parent,source,sink)){
        int tem=minflow(residual,parent,source,sink);

        maxflow+=tem;

        makeFlow(graph,flow,parent,source,sink,tem);
        makeResidual(graph,flow,residual,n);

        bfs(residual,n,source,parent,visit);break;
    }

    cout<<endl<< "Maxflow: "<<maxflow<<endl;
}

//Finding min cut in the graph
void minCut(int **graph,int **residual,int source,int sink,int n){
        int *visited;

        visited=new int[n +1];

        //Finding unreachable nodes from source in residual graph
        dfs(residual,visited,n,source);

        for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++){
            if(visited[i] and !visited[j] and graph[i][j]!=inf) cout<<i<< " - "<<j<<endl;
        }

}

//DFS
void dfs(int **graph,int *visited,int n,int source){
    if(visited[source]==1) return;
    visited[source]=1;
    for(int i=1;i<=n;i++){
        if(graph[source][i]!=inf) {
            dfs(graph,visited,n,i);
        }
    }
}

//BFS
void bfs(int **graph,int n,int source,int *parent,int *visit){

    initialize(parent,n +1,-1);
    initialize(visit,n +1, 0);
    parent[source]=0;

    int temp;
    Queue que;
    que.enqueue(source);

    while(!que.empty()){
        temp=que.dequeue();

        for(int i=1;i<=n;i++){
            if(visit[i]==1) continue;
            if(  (graph[temp][i]!=inf)and parent[i]==-1){
                que.enqueue(i);
                parent[i]=temp;
            }
        }
        visit[temp]=1;
    }

}

int main(){

    readFile

    int n,m,u,v,c,source,sink;
    cin>>n>>m;

    int **graph,**flow,**residual;

    //Memory allocation and initialization
    graph=new int*[n +1];
    flow=new int*[n +1];
    residual=new int*[n +1];

    for(int i=0;i<=n;i++){
        graph[i]=new int[n +1];
        flow[i]=new int[n +1];
        residual[i]=new int[n +1];

        for(int j=0;j<=n;j++){
            graph[i][j]=inf;
            flow[i][j]=0;
            residual[i][j]=inf;
        }
    }

    //Input graph
    for(int i=0;i<m;i++){
        cin>>u>>v>>c;
        graph[u][v]=c;
    }

    cin>>source>>sink;
    fordFulkerson(graph,flow,residual,n,source,sink);

    cout<<endl<< "Original Graph: ", print(graph,1,n);
    cout<<endl<< "Flow Network: ", print(flow,1,n);
    cout<<endl<< "Residual Graph: ", print(residual,1,n);
    cout<< "min cut: ";
    minCut(graph,residual,source,sink,n);

    return 0;
}
