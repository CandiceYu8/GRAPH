#include <iostream>
#include <stdlib.h>
#include <algorithm>
#define MAXN 100

using namespace std;

/* vertex structure */
struct Vertex{
    int groupNum_;    // there is no need to add nodeNum for it is just the index of array
    int distance_;
    int found;        // 1 already found shortest path, otherwise 0
    int parent;
    struct EdgeVertex *next;

    Vertex(){}
    Vertex(int groupNum, int distance):
        groupNum_(groupNum), distance_(distance){
        found = 0;
        parent = -1;
        next = NULL;
    }
};

/* sort according to distance */
bool cmp(const Vertex &x, const Vertex &y)
{
    return x.distance_ < y.distance_;
}

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

struct InverseEdge{
    // it is the end point of the edge, used to update missing info
    int weight_;
    int nodeNum_;    // the index of the vertices, if there is no inverse edge for this point, then nodeNum_ is -1
    struct InverseEdge *next;

    InverseEdge(){}
    InverseEdge(int weight, int nodeNum):
        weight_(weight), nodeNum_(nodeNum){
        next = NULL;
    }
};

/* graph structure */
struct Graph{
    struct Vertex vertices[5];
    struct InverseEdge verticesInverse[5];
    Graph(){
        // init nodeVertex
        int i;
        for(i=0; i<5; i++){
            vertices[i].groupNum_ = (i+1)/2;
            vertices[i].distance_ = 10000;
            vertices[i].found = 0;
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

        // init inverse-edges
        verticesInverse[0].nodeNum_ = 4;
        verticesInverse[0].weight_ = 7;
        verticesInverse[0].next = NULL;

        verticesInverse[1].nodeNum_ = 0;
        verticesInverse[1].weight_ = 10;
        verticesInverse[1].next = new struct InverseEdge(3, 3);

        verticesInverse[2].nodeNum_ = 1;
        verticesInverse[2].weight_ = 1;
        verticesInverse[2].next = new struct InverseEdge(3, 9);
        verticesInverse[2].next->next = new struct InverseEdge(4, 6);

        verticesInverse[3].nodeNum_ = 0;
        verticesInverse[3].weight_ = 5;
        verticesInverse[3].next = new struct InverseEdge(1, 2);
        
        verticesInverse[4].nodeNum_ = 2;
        verticesInverse[4].weight_ = 4;
        verticesInverse[4].next = new struct InverseEdge(3, 2);
    }
};


/* return the index of the shortest path vertex
   startNum is the start index mumber of the vertex */
int Shortest(struct Graph *graph, int nodeNum, int startNum){
    int i, result = -1;
    // find the first node which hasn't found the shortest path
    for(i=startNum; i<nodeNum; i++){
        if(graph->vertices[i].found == 0){
            result = i;
            break;
        }
    }
    for(; i<nodeNum; i++){
        if(graph->vertices[result].distance_ > graph->vertices[i].distance_ 
            && graph->vertices[i].found == 0){
            result = i;
        }
    }
    return result;
}


/* relax the distance of each vertex
   cur_index is the index of the vertex to relax other vertices
   relax_index is the node to be relaxed
   weight is the edge weight */
void Relax(struct Graph *graph, int cur_index, int relax_index, 
            int weight, int process[][2], int &processNum)
{
    int u = graph->vertices[cur_index].distance_;
    int v = graph->vertices[relax_index].distance_;
    if(v > u+weight){
        // need to relax
        graph->vertices[relax_index].distance_ = u+weight;
        graph->vertices[relax_index].parent = cur_index;
        // record process
        process[processNum][0] = relax_index;
        process[processNum++][1] = u+weight;
    }
}


/* realize Dijkstra algorithm
   graph is the structure
   nodeNum is the count of vertices
   process is used to record updated vertex number and distance */
int Dijkstra(struct Graph *graph, int nodeNum, int process[MAXN][2])
{
    int i;
    int cur_;
    int next_index;
    int weight;
    int processNum = 0;
    int undo = nodeNum;
    struct EdgeVertex *next_ = NULL;
    while(undo--){
        cur_ = Shortest(graph, nodeNum, 0);
        if(cur_ >= 0){
            graph->vertices[cur_].found = 1;
            // record process
            process[processNum][0] = cur_;
            process[processNum++][1] = graph->vertices[cur_].distance_;
            next_ = graph->vertices[cur_].next;

            // relax distance
            while(next_ != NULL){
                next_index = next_->nodeNum_;
                weight = next_->weight_;
                Relax(graph, cur_, next_index, weight, process, processNum);
                next_ = next_->next;
            }
        }
    }
    return processNum;
}


/* Used for missing information update.
   when missing vertex couldn't update by rest vertices, 
   we need to use already found vertices to update them. */
void UpdateMissing(struct Graph *graph, int cur_index, int process[][2], int &processNum)
{
    cout << "update info" << endl;
    struct InverseEdge *cur = &graph->verticesInverse[cur_index];
    if(cur->nodeNum_ == -1)
        return;
    Relax(graph, cur->nodeNum_, cur_index, cur->weight_, process, processNum);
    cur = cur->next;
    while(cur != NULL){
        Relax(graph, cur->nodeNum_, cur_index, cur->weight_, process, processNum);
        cur = cur->next;
    }
}


/* realize Dijkstra algorithm for randomly missing information
   graph is the structure
   nodeNum is the count of vertices
   process is used to record updated vertex number and distance 
   missGroup is the group number which miss information 
   iter is when the information is lost */
int Dijkstra_M(struct Graph *graph, int nodeNum, int process[MAXN][2], int missGroup, int iter)
{
    int i;
    int cur_;
    int next_index;
    int weight;
    int processNum = 0;
    int undo = nodeNum;
    int first_ = 0;
    bool flag = true;
    struct EdgeVertex *next_ = NULL;
    while(undo--){
        // judge whether should miss group information
        if(nodeNum-undo == iter && flag){
            for(i=0; i<nodeNum; i++){
                if(graph->vertices[i].groupNum_ == missGroup){
                    // if it has already been found the shortest path, then undo++
                    if(graph->vertices[i].found == 1)
                        undo ++;
                    // miss information
                    graph->vertices[i].distance_ = 10000;
                    graph->vertices[i].parent = -1;
                    graph->vertices[i].found = 0;
                }
            }
            flag = false;
        }

        cur_ = Shortest(graph, nodeNum, 0);
        // if it is in the missing group and still with distance 10000, then we need to use
        // found vertices to update this vertex
        while(cur_ >= 0 && graph->vertices[cur_].distance_ == 10000 && 
            graph->vertices[cur_].groupNum_ == missGroup){
                if(first_){
                    cur_ = Shortest(graph, nodeNum, cur_+1);
                }
                UpdateMissing(graph, cur_, process, processNum);
                first_ ++; 
        }

        if(cur_ >= 0){
            graph->vertices[cur_].found = 1;
            // record process
            process[processNum][0] = cur_;
            process[processNum++][1] = graph->vertices[cur_].distance_;
            next_ = graph->vertices[cur_].next;

            // relax distance
            while(next_ != NULL){
                next_index = next_->nodeNum_;
                weight = next_->weight_;
                Relax(graph, cur_, next_index, weight, process, processNum);
                next_ = next_->next;
            }
        }
    }
    return processNum;
}


int main(int argc, char const *argv[])
{
    int i;
    struct Graph *graph = new struct Graph;
    int process[MAXN][2];
    memset(process, 0, sizeof(process));

    int processNum = Dijkstra(graph, 5, process);
    // int processNum = Dijkstra_M(graph, 5, process, 2, 2);
    cout << "Dijkstra process: " << endl;
    for(i=0; i<processNum; i++){
        cout << "node number: " << process[i][0] << " distance: " << process[i][1] << endl;
    }
    cout << "each vertex info: " << endl;
    for(i=0; i<5; i++){
        cout << "node number: " << i << " found: " << graph->vertices[i].found <<
            " group number: " << graph->vertices[i].groupNum_ << " parent: " << 
            graph->vertices[i].parent << " distance: " << graph->vertices[i].distance_ << endl;
    }
    return 0;
}
