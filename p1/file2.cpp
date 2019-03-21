#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Vertex {
    
public:

    bool _visited;

    list<Vertex*> _adjacent;

    Vertex(bool visited = false): _visited(visited) {}

    void addAdjacent(Vertex* v1) {
        _adjacent.push_back(v1);
    }
};

int DFSVisit(int id, vector<Vertex*> &parents, vector<Vertex*> &vVector) {
    
    int ccId = id;
    Vertex* v = vVector[id];

    for (Vertex* u: v->_adjacent) {
        if (u->_visited == false) {
            u->_visited = true;
            int i = DFSVisit(u, parents, vVector);
            if (ccId < i) { ccId = i; }
        }
    }    
    return ccId;
}

void executeDFS(vector<Vertex*> &vVector) {

    int nSubNetworks = 0;
    list<int> ccIds;

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

    ccIds.sort();
    printf("%d\n", nSubNetworks);
    for (int i: ccIds) {
        printf("%d ", i);
    }
    
    printf("\n");
}

int main() {

    int n, m, p1, p2;

    if (scanf("%d", &n) != 1) {exit(0);};  //reads number of routers
    vector<Vertex*> vVector(n); // O(n);
    for (int i = 0; i < n; i++) {   // O(n);
        vVector[i] = new Vertex(i+1);   
    }

    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections

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