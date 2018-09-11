/* more structured */

#include "template_connected2.cpp"


int main(int argc, char const *argv[])
{
    int option;
    while(true)
    {
        cout << "input which mode you wanna run:(1: shortest_distance; 0: connected; -1: exit)" << endl;
        cin >> option ;
        if(option == 1)
        {
            ShortestPath test1;
            test1.Init("input2.txt");
            // test1.Display();
            test1.Algorithm();
            test1.Algorithm_M(2, 2,0);
            //test1.Display();
            test1.ShowResult();

        }
        else if(option == 0)
        {
            Connected test1;
            test1.Init("input2.txt");
            // test1.Display();
            test1.Algorithm();
            test1.Algorithm_M(2, 2,0);
            //test1.Display();
            test1.ShowResult();
        }
        else if(option == -1)
        {
            break;
        }
        else{
            cout << "wrong" << endl;
        }
    }
        return 0;
    }
