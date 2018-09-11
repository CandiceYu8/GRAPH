#include <iostream>
#include <stdlib.h>
#include <algorithm>
#define MAXN 100

using namespace std;

/* vertex structure */
struct Vertex{
    int groupNum_;    // there is no need to add nodeNum for it is just the index of array
    int distance_;
    int parent;
    struct EdgeVertex *next;

    Vertex(){}
    Vertex(int groupNum, int distance):
        groupNum_(groupNum), distance_(distance){
        parent = -1;
        next = NULL;
    }
};

struct EdgeVertex{
    // it is the start point of the edge
    int weight_;
    int nodeNum_;    // the index of the vertices
    struct EdgeVertex *next;

    EdgeVertex(){}
    EdgeVertex(int weight, int nodeNum):
        weight_(weight), nodeNum_(nodeNum){
        next = NULL;
    }
};

/* graph structure */
struct Graph{
    struct Vertex vertices[5];
    Graph(){
        // init nodeVertex
        int i;
        for(i=0; i<5; i++){
            vertices[i].groupNum_ = (i+1)/2;
            vertices[i].distance_ = 10000;
            vertices[i].parent = -1;
            vertices[i].next = NULL;
        }
        vertices[0].distance_ = 0;
        vertices[0].parent = 0;

        // init edges
        vertices[0].next = new struct EdgeVertex(10, 1);
        vertices[0].next->next = new struct EdgeVertex(5, 3);

        vertices[1].next = new struct EdgeVertex(1, 2);
        vertices[1].next->next = new struct EdgeVertex(2, 3);

        vertices[2].next = new struct EdgeVertex(4, 4);

        vertices[3].next = new struct EdgeVertex(3, 1);
        vertices[3].next->next = new struct EdgeVertex(9, 2);
        vertices[3].next->next->next = new struct EdgeVertex(2, 4);

        vertices[4].next = new struct EdgeVertex(7, 0);
        vertices[4].next->next = new struct EdgeVertex(6, 2);
    }
};

/* relax the distance of each vertex */
void Relax(struct Graph *graph, int index,
            int process[][2], int &processNum, int update[][2])
{
    int newDist = 0;
    int oldDist = 0;
    int newIndex = -1;
    int dist = graph->vertices[index].distance_;
    struct EdgeVertex *ver = graph->vertices[index].next;
    while(ver != NULL){
        newIndex = ver->nodeNum_;
        oldDist = graph->vertices[newIndex].distance_;
        newDist = ver->weight_ + dist;
        if(update[newIndex][0] == -1){
            if(newDist < oldDist){
                // record process
                process[processNum][0] = newIndex;
                process[processNum++][1] = newDist;
                cout << "process: " << "node number: " << newIndex << " distance: " << newDist << endl;

                // record update
                update[newIndex][0] = newDist;
                update[newIndex][1] = index;
            }
        }
        else if(update[newIndex][0] > newDist){
            // record process
            process[processNum][0] = newIndex;
            process[processNum++][1] = newDist;
            cout << "process: " << "node number: " << newIndex << " distance: " << newDist << endl;

            // record update
            update[newIndex][0] = newDist;
            update[newIndex][1] = index;
        }
        ver = ver->next;
    }
}

/* realize Dijkstra algorithm
   graph is the structure
   nodeNum is the count of vertices
   process is used to record updated vertex number and distance 
   Return the iteration times */
int Dijkstra(struct Graph *graph, int nodeNum, int process[MAXN][2])
{
    int i;
    int processNum = 0;
    int iter = 0;
    bool flag = false;      // judge finish condition
    // if the status doesn't change anymore, then it is finished.
    int update[nodeNum][2];         // distance + parentNum
    do{
        iter ++;
        cout << "iter: " << iter << endl;
        flag = false;
        memset(update, -1, sizeof(update));
        for(i=0; i<nodeNum; i++){
            Relax(graph, i, process, processNum, update);
        }

        for(i=0; i<nodeNum; i++){
            if(update[i][0] != -1){
                flag = true;
                // update info
                graph->vertices[i].distance_ = update[i][0];
                graph->vertices[i].parent = update[i][1];
            }
        }
    }while(flag);
    return --iter;
}

/* realize Dijkstra algorithm for randomly missing information
   graph is the structure
   nodeNum is the count of vertices
   process is used to record updated vertex number and distance 
   missGroup is the group number which miss information 
   loss is when the information is lost
   Return the iteration times */
int Dijkstra_M(struct Graph *graph, int nodeNum, int process[MAXN][2], 
                int missGroup, int loss)
{
    int i;
    int processNum = 0;
    int iter = 0;
    bool flag = false;      // judge finish condition
    // if the status doesn't change anymore, then it is finished.
    int update[nodeNum][2];         // distance + parentNum
    do{
        iter ++;
        cout << "iter: " << iter << endl;
        // missing info
        if(iter == loss){
            for(i=0; i<nodeNum; i++){
                if(graph->vertices[i].groupNum_ == missGroup){
                    graph->vertices[i].distance_ = 10000;
                    graph->vertices[i].parent = -1;
                }
            }
        }

        flag = false;
        memset(update, -1, sizeof(update));
        for(i=0; i<nodeNum; i++){
            Relax(graph, i, process, processNum, update);
        }

        for(i=0; i<nodeNum; i++){
            if(update[i][0] != -1){
                flag = true;
                // update info
                graph->vertices[i].distance_ = update[i][0];
                graph->vertices[i].parent = update[i][1];
            }
        }
    }while(flag);
    return --iter;
}

int main(int argc, char const *argv[])
{
    int i;
    struct Graph *graph = new struct Graph;
    int process[MAXN][2];
    memset(process, -1, sizeof(process));

    int iter = Dijkstra(graph, 5, process);
    // int iter = Dijkstra_M(graph, 5, process, 2, 2);

    cout << "iteration times: " << iter << endl;

    cout << "each vertex info: " << endl;
    for(i=0; i<5; i++){
        cout << "node number: " << i << " group number: " 
            << graph->vertices[i].groupNum_ << " parent: " << 
            graph->vertices[i].parent << " distance: " 
            << graph->vertices[i].distance_ << endl;
    }
    return 0;
}
