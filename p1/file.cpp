#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Vertex {
    
public:

    bool _visited;

    list<int> _adjacent;

    Vertex(bool visited = false): _visited(visited) {}

    void addAdjacent(int id) {
        _adjacent.push_back(id);
    }
};

int DFSVisit(int id, vector<Vertex*> &parents, vector<Vertex*> &vVector) {
    
    int ccId = id;
    Vertex* v = vVector[id-1];

    for (int i: v->_adjacent) {
        Vertex* u = vVector[i-1]; 
        if (u->_visited == false) {
            u->_visited = true;
            int d = DFSVisit(i, parents, vVector);
            if (ccId < d) { ccId = d; }
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
    for (int id = 1; id <= vVector.size(); id++) {
        Vertex* v = vVector[id-1];
        if (v->_visited == false) {
            nSubNetworks++;
            v->_visited = true;
            ccIds.push_back(DFSVisit(id, parents, vVector));
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
        vVector[i] = new Vertex();   
    }

    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections

    while (scanf("%d %d", &p1, &p2) == 2) {
        vVector[p1-1]->addAdjacent(p2);
        vVector[p2-1]->addAdjacent(p1);
    }
    
    executeDFS(vVector);

    for (Vertex* v: vVector) {
        delete v;
    }
    return 0;
}