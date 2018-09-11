#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string.h>
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

/* parent sturcture */
class GraphAlgorithm
{
    public:
        // data
        vector<struct Vertex> vertices;
        int process[MAXN][2];       // record algorithm process
        int update[MAXN][2];        // distance + parentNum
        int iter;
        int processNum;         

        // static funcfions
        void Display(){
            int i;
            struct EdgeVertex* tmpedge;
            for(i=0; i<vertices.size(); i++){
                cout << "index: " << i << endl;
                tmpedge = vertices[i].next;
                while(tmpedge != NULL){
                    cout << "weight: " << tmpedge->weight_ << " toindex: " << tmpedge->nodeNum_ << endl;
                    tmpedge = tmpedge->next;
                }
            }
        }

        /* main part*/
        void Algorithm(){
            int i;
            bool flag = false;      // judge finish condition
            // if the status doesn't change anymore, then it is finished.       
            do{
                iter ++;
                cout << "iter: " << iter << endl;
                flag = false;
                memset(update, -1, sizeof(update));
                for(i=0; i<vertices.size(); i++){
                    Relax(i);
                }
                flag = UpdateInfo(vertices, update);
            }while(flag);
            iter--;
        }

        /* algorithm with missing condition
        loss is the iterate time when info is to lose */
        void Algorithm_M(int missGroup, int loss){
            int i;
            bool flag = false;      // judge finish condition
            // if the status doesn't change anymore, then it is finished.
            do{
                iter ++;
                cout << "iter: " << iter << endl;
                // missing info
                if(iter == loss){
                    for(i=0; i<vertices.size(); i++){
                        if(vertices[i].groupNum_ == missGroup){
                            vertices[i].distance_ = 10000;
                            vertices[i].parent = -1;
                        }
                    }
                }

                flag = false;
                memset(update, -1, sizeof(update));
                for(i=0; i<vertices.size(); i++){
                    Relax(i);
                }
                flag = UpdateInfo(vertices, update);
            }while(flag);
            iter--;
        }

        /* auxiliary functions */
        virtual void Init(string filename){}
        virtual void Relax(int index){}
        virtual bool UpdateInfo(vector <struct Vertex> &vertices, int update[][2]){
            bool flag = false;
            return flag;
        }
        virtual void ShowResult(){}
};
