#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Vertex {
    
public:

    int _id;

    bool _visited;

    vector<Vertex*> _adjacent;

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

int DFSVisit(Vertex* v, vector<Vertex*> &parents) {
    
    int ccId = v->_id;

    for (Vertex* u: v->_adjacent) {
        if (u->_visited == false) {
            u->_visited = true;
            int i = DFSVisit(u, parents);
            if (ccId < i) { ccId = i; }
        }
    }    
    return ccId;
}

void executeDFS(vector<Vertex*> &vVector) {

    int nSubNetworks = 0;
    vector<int> ccIds;

    for (Vertex* v: vVector) {
        v->_visited = false;
    }
    vector<Vertex*> parents(vVector.size());
    for (Vertex* v: vVector) {
        if (v->_visited == false) {
            nSubNetworks++;
            v->_visited = true;
            ccIds.push_back(DFSVisit(v, parents));
        }
    }

    printf("%d\n", nSubNetworks);
    for (int i: ccIds) {
        printf("%d ", i);
    }
    
    printf("\n");
}

int main() {

    int n, m, p1, p2;

    if (scanf("%d", &n) != 1) {exit(0);};  //reads number of routers
    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections
    vector<Vertex*> vVector(n); // O(n);
    for (int i = 0; i < n; i++) {   // O(n);
        vVector[i] = new Vertex(i+1);   
    }

    while (scanf("%d %d", &p1, &p2) == 2) {
        p1--; p2--;
        vVector[p1]->addAdjacent(vVector[p2]);
        vVector[p2]->addAdjacent(vVector[p1]);
    }
    
    executeDFS(vVector);

    for (Vertex* v: vVector) {
        delete v;
    }
    return 0;
}