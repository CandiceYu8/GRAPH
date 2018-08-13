/* more structured */

#include "template2.cpp"

/* derivative class1: shortest-path */
class ShortestPath: public GraphAlgorithm
{
    public:
        void Init(string filename){
            // memset(process, -1, sizeof(process));
            memset(update, -1, sizeof(update));
            iter = 0;
            // processNum = 0;

            ifstream infile(filename); 
            int i, nodeNum, edgeNum;
            int index_;
            int weight_;
            int toindex_;
            struct Vertex tmp;
            struct EdgeVertex* tmpedge;
            if(!infile.is_open ())
                cout << "Open file failure" << endl;
            infile >> nodeNum >> edgeNum;
            // create nodes
            tmp.groupNum_ = 0;  tmp.next = NULL;  tmp.result = 0;  tmp.parent = 0;
            vertices.push_back(tmp);
            srand((unsigned)time(NULL));
            for(i=1; i<nodeNum; i++){
                // tmp.groupNum_ = rand()%GROUPNUM+1;       // random divide
                // tmp.groupNum_ = i/(nodeNum/GROUPNUM) + 1;   // range divide
                tmp.groupNum_ = i%GROUPNUM + 1;         // hash divide
                tmp.result = MAXRESULT;
                tmp.parent = -1;
                tmp.next = NULL;
                vertices.push_back(tmp);
            }

            while (!infile.eof())            
            {
                infile >> index_ >> weight_ >> toindex_;
                if(vertices[index_].next == NULL){
                    vertices[index_].next = new struct EdgeVertex(weight_, toindex_);
                }
                else{
                    tmpedge = vertices[index_].next;
                    while(tmpedge->next != NULL){
                        tmpedge = tmpedge->next;
                    }
                    tmpedge->next = new struct EdgeVertex(weight_, toindex_);
                }
            }
            infile.close();   
        }

        void Relax(int index){
            int newDist = 0;
            int oldDist = 0;
            int newIndex = -1;
            int dist = vertices[index].result;
            struct EdgeVertex *ver = vertices[index].next;
            while(ver != NULL){
                newIndex = ver->nodeNum_;
                oldDist = vertices[newIndex].result;
                newDist = ver->weight_ + dist;
                if(update[newIndex][0] == -1){
                    if(newDist < oldDist){
                        // record process
                        // process[processNum][0] = newIndex;
                        // process[processNum++][1] = newDist;
                        // cout << "process: " << "node number: " << newIndex << " distance: " << newDist << endl;

                        // record update
                        update[newIndex][0] = newDist;
                        update[newIndex][1] = index;
                    }
                }
                else if(update[newIndex][0] > newDist){
                    // record process
                    // process[processNum][0] = newIndex;
                    // process[processNum++][1] = newDist;
                    // cout << "process: " << "node number: " << newIndex << " distance: " << newDist << endl;

                    // record update
                    update[newIndex][0] = newDist;
                    update[newIndex][1] = index;
                }
                ver = ver->next;
            }
        }

        bool UpdateInfo(vector <struct Vertex> &vertices, int update[][2]){
            int i;
            bool flag = false;
            for(i=0; i<vertices.size(); i++){
                if(update[i][0] != -1){
                    flag = true;
                    // update info
                    vertices[i].result = update[i][0];
                    vertices[i].parent = update[i][1];
                }
            }
            return flag;
        }

        void ShowResult(){
            int i;
            cout << "iteration times: " << iter << endl;
            cout << "each vertex info: " << endl;
            for(i=0; i<vertices.size(); i++){
                cout << "node number: " << i << " group number: " 
                    << vertices[i].groupNum_ << " parent: " 
                    << vertices[i].parent << " distance: " 
                    << vertices[i].result << endl;
            }
        }

        void Clear(){
            int i;
            for(i=1; i<vertices.size(); i++){
                vertices[i].result = MAXRESULT;
                vertices[i].parent = -1;
            }
            iter = 0;
            memset(update, -1, sizeof(update));
        }
};

int main(int argc, char const *argv[])
{
    ShortestPath test1;
    test1.Init("input2.txt");
    // test1.Display();
    // test1.Algorithm();
    int i;      // loss iteration
    int j;      // group number
    for(i=1; i<=17; i++){
        for(j=1; j<=GROUPNUM; j++){
            
            test1.Algorithm_M(j, i);
            cout << "loss iteration: " << i 
            << " group number: " << j 
            << " iter times: " << test1.iter << endl;
            test1.Clear();
            // test1.ShowResult();
        }
    }
    
    // test1.Algorithm_M(2, 17);
    // test1.ShowResult();
    

    return 0;
}