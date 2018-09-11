/* more structured */

#include "template2.cpp"

/* derivative class1: shortest-path */
class ShortestPath: public GraphAlgorithm
{
public:
    int nodeNum, edgeNum;

    void Init(string filename)
    {
        memset(update, 0, sizeof(update));
        iter = 0;
        processNum = 0;

        ifstream infile(filename);
        int i;
        int index_;
        int weight_;

        int toindex_;
        struct Vertex tmp;
        struct EdgeVertex* tmpedge;
        if(!infile.is_open ())
            cout << "Open file failure" << endl;
        infile >> nodeNum >> edgeNum;

        // create nodes
        tmp.groupNum_ = 0;
        tmp.next = NULL;
        tmp.result = 0;
        tmp.parent = 0; //ohhh, poor lonely zero.
        vertices.push_back(tmp);
        for(i=1; i<nodeNum; i++)
        {
            tmp.groupNum_ = (i+nodeNum/GROUPNUM)/(nodeNum/GROUPNUM); //GroupNum can be changed.
            tmp.result = MAXRESULT;
            tmp.parent = -1;  //clear that this node is not the first one.
            tmp.next = NULL;
            vertices.push_back(tmp);
        }

        //create edges
        while (!infile.eof())
        {
            infile >> index_ >> weight_ >> toindex_;
            if(vertices[index_].next == NULL)
            {
                vertices[index_].next = new struct EdgeVertex(weight_, toindex_);
            }
            else
            {
                tmpedge = vertices[index_].next;
                while(tmpedge->next != NULL)
                {
                    tmpedge = tmpedge->next;
                }
                tmpedge->next = new struct EdgeVertex(weight_, toindex_);
            }
        }
        infile.close();
    }

    void Relax(int index)
    {
        long newStatus = 0;
        long oldStatus = 0;
        long newIndex = -1;
        long status = vertices[index].result;
        struct EdgeVertex *ver = vertices[index].next;
        while(ver != NULL)
        {
            newIndex = ver->nodeNum_;
            oldStatus = vertices[newIndex].result;
            newStatus = ver->weight_ + status;
            if(!update[newIndex][2])
            {
                update[newIndex][2] = 1; // scanned
                if(newStatus < oldStatus)
                {
                    // record update
                    update[newIndex][0] = newStatus;
                    update[newIndex][1] = index;
                    update[newIndex][2] = 2; // changed
                }
                else
                {
                    update[newIndex][0] = oldStatus;
                    if(oldStatus != vertices[newIndex].result)
                    {
                        update[newIndex][2] = 2; // changed
                    }
                }

            }
            else if(newStatus < update[newIndex][0])
            {
                // record update
                update[newIndex][0] = newStatus;
                update[newIndex][1] = index;
                update[newIndex][2] = 2; // changed
            }
            ver = ver->next;
        }
    }

    bool UpdateInfo(vector <struct Vertex> &vertices, long update[][3])
    {
        int i;
        bool flag = false;
        for(i=0; i<vertices.size(); i++)
        {
            if(update[i][2] == 2)
            {
                flag = true;
                // update info
                vertices[i].result = update[i][0];
                vertices[i].parent = update[i][1];
            }
        }
        return flag;
    }

    void ShowResult()
    {
        int i;
        cout << "iteration times: " << iter << endl;
        cout << "each vertex info: " << endl;
        for(i=0; i<vertices.size(); i++)
        {
            cout << "node number: " << i << " group number: "
                 << vertices[i].groupNum_ << " parent: "
                 << vertices[i].parent << " distance: "
                 << vertices[i].result << endl;
        }

 /*       for (int k = 0; k < nodeNum; k++){
            if(vertices[k].groupNum_ == 3){
                cout << k << " ";
            }
        }
        cout<<endl;*/

    }
};

/*int main(int argc, char const *argv[])
{
    ShortestPath test1;
    test1.Init("input2.txt");
    // test1.Display();
    test1.Algorithm();
    test1.Algorithm_M(2, 2, 1);
    test1.ShowResult();

    return 0;
}
*/
