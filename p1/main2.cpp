#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Vertex {
    
public:

    int _id, _dTime, _lTime;

    bool _visited, _breakpoint;

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

int DFSVisit(Vertex* v, int* dTime, vector<Vertex*> &parents) {
    
    int ccId = v->_id;
    int c = 0;
    for (Vertex* u: v->_adjacent) {
        if (u->_visited == false) {
            parents[u->_id-1] = v;
            c++;
            u->_dTime = u->_lTime = (*dTime)++;
            u->_visited = true;
            int i = DFSVisit(u, dTime, parents);
            if (ccId < i) { ccId = i; }
            if (v->_lTime > u->_lTime) { v->_lTime = u->_lTime; }
            if (((parents[v->_id-1] == nullptr) && (c > 1)) || \
            ((parents[v->_id-1] != nullptr) && (u->_lTime >= v->_dTime))) {v->_breakpoint = true;}
        }
        else if (parents[v->_id-1] != u) {
            if (v->_lTime > u->_dTime) { v->_lTime = u->_dTime; }
        }
    }    
    return ccId;
}

void executeDFS(vector<Vertex*> &vVector) {

    int nSubNetworks = 0, dTime = 0;
    vector<int> ccIds;

    for (Vertex* v: vVector) {
        v->_breakpoint = false;
    }

    vector<Vertex*> parents(vVector.size(), nullptr);
    for (Vertex* v: vVector) {
        if (v->_visited == false) {
            v->_dTime = v->_lTime = dTime++;
            nSubNetworks++;
            v->_visited = true;
            ccIds.push_back(DFSVisit(v, &dTime, parents));
        }
    }
    sort(ccIds.begin(), ccIds.end());
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
    int i = 0;
    for (Vertex* v: vVector) {
        if (v->_breakpoint == true) {i++;};
        printf("Id: %d | Low: %d | Disc: %d | Breakpoint? %d\n", v->_id, v->_lTime, v->_dTime, v->_breakpoint == true? 1 : 0);
        delete v;
    }
    printf("%d\n", i);
    return 0;
}