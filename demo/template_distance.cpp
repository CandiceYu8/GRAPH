/* more structured */

#include "template.cpp"

/* derivative class1: shortest-path */
class ShortestPath: public GraphAlgorithm
{
    public:
        void Init(string filename){
            memset(process, -1, sizeof(process));
            memset(update, -1, sizeof(update));
            iter = 0;
            processNum = 0;

            ifstream infile(filename); 
            int index_;
            int weight_;
            int toindex_;
            struct Vertex tmp;
            struct EdgeVertex* tmpedge;
            if(!infile.is_open ())
                cout << "Open file failure" << endl;
            while (!infile.eof())            
            {
                infile >> index_ >> weight_ >> toindex_;
                // judge whether this node has already been created
                if(vertices.size() <= index_){
                    tmp.groupNum_ = (index_+1)/2;
                    tmp.next = NULL;
                    if(index_ == 0){
                        tmp.distance_ = 0;
                        tmp.parent = 0;
                    }
                    else{
                        tmp.distance_ = 10000;
                        tmp.parent = -1;
                    }
                    vertices.push_back(tmp);
                }

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
            int dist = vertices[index].distance_;
            struct EdgeVertex *ver = vertices[index].next;
            while(ver != NULL){
                newIndex = ver->nodeNum_;
                oldDist = vertices[newIndex].distance_;
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

        bool UpdateInfo(vector <struct Vertex> &vertices, int update[][2]){
            int i;
            bool flag = false;
            for(i=0; i<vertices.size(); i++){
                if(update[i][0] != -1){
                    flag = true;
                    // update info
                    vertices[i].distance_ = update[i][0];
                    vertices[i].parent = update[i][1];
                }
            }
            return flag;
        }

        void ShowResult(){
            int i;
            cout << "iteration times: " << iter << endl;
            cout << "each vertex info: " << endl;
            for(i=0; i<5; i++){
                cout << "node number: " << i << " group number: " 
                    << vertices[i].groupNum_ << " parent: " 
                    << vertices[i].parent << " distance: " 
                    << vertices[i].distance_ << endl;
            }
        }
};

int main(int argc, char const *argv[])
{
    ShortestPath test1;
    test1.Init("input.txt");
    // test1.Display();
    // test1.Algorithm();
    test1.Algorithm_M(2, 2);
    test1.ShowResult();

    return 0;
}