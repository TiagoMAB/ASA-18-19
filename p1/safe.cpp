#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better

using namespace std;

class Vertex {
    
public:

    int _id;

    bool _visited;

    list<Vertex*> _adjacent;

    Vertex(int id, bool visited = false): _id(id), _visited(visited) {}

    void addAdjacent(Vertex* v1) {
        _adjacent.push_back(v1);
    }

    void printAdjacent() {
        for (Vertex* v: _adjacent) {
            printf("Printing: %d\n", v->_id);
        }
    }
};

void DFSVisit(Vertex* v) {
    
    for (Vertex* u: v->_adjacent) {
        if (u->_visited == false) {
            u->_visited = true;
            DFSVisit(u);
        }
    }    
}

int executeDFS(vector<Vertex*> vVector) {

    int n = 0;

    for (Vertex* v: vVector) {
        v->_visited = false;
    }

    for (Vertex* v: vVector) {
        if (v->_visited == false) {
            n++;
            v->_visited = true;
            DFSVisit(v);
        }
    }

    return n;
}

int main() {

    int n, m, p1, p2;
    Vertex *v1, *v2;

    scanf("%d", &n);  //reads number of routers
    vector<Vertex*> vVector(n); // O(n);
    for (int i = 0; i < n; i++) {   // O(n);
        vVector[i] = new Vertex(i+1);   
    }

    scanf("%d", &m);  //reads number of connections
    while (scanf("%d %d", &p1, &p2) == 2) {
        p1--; p2--;
        vVector[p1]->addAdjacent(vVector[p2]);
        vVector[p2]->addAdjacent(vVector[p1]);
    }
    
    int nSubNetworks = executeDFS(vVector);

    printf("Component: %d\n", nSubNetworks);

    for (Vertex* v: vVector) {
        delete v;
    }
    return 0;
}