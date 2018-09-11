#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string.h>
#define MAXN 10000
#define MAXRESULT 100000
#define GROUPNUM 6

using namespace std;

/* vertex structure */
struct Vertex{
    int groupNum_;
    long result;
    long parent;
    struct EdgeVertex *next;

    Vertex(){}
    Vertex(int groupNum, long result_):
        groupNum_(groupNum), result(result_){
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
        long update[MAXN][3];        // distance + parentNum + flag of whether be changed.
        int iter;
        int processNum;
        int current_max_result;

        // static funcfions
        void Display(){
            int i;
            struct EdgeVertex* tmpedge;
            for(i=0; i<vertices.size(); i++){
                cout << "index: " << i << " groupNum:" << vertices[i].groupNum_ << endl;
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
                memset(update, 0, sizeof(update));
                for(i=0; i<vertices.size(); i++){
                    Relax(i);
                }
                //Checkthestatus(vertices, update);
                flag = UpdateInfo(vertices, update);
            }while(flag);
            iter--;
        }

        /* algorithm with missing condition
        missGroup is the group number, and there are 6 groups.
        loss is the iterate time when info is to lose */
        void Algorithm_M(int missGroup, int loss, int option){
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
                            if(option == 1)
                            {
                                vertices[i].result = MAXRESULT; // distance
                            }
                            else
                            {
                                vertices[i].result = i; //connected
                            }
                            vertices[i].parent = -1;
                        }
                    }
                }

                flag = false;
                memset(update, 0, sizeof(update));
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
        virtual bool UpdateInfo(vector <struct Vertex> &vertices, long update[][3]){
            bool flag = false;
            return flag;
        }
        virtual void ShowResult(){}
};
