#include <iostream>
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Vertex {
    
public:

    int _id, _dTime, _lTime;

    bool _breakpoint;

    vector<Vertex*> _adjacent;

    Vertex(int id, int discovery = -1): _id(id), _dTime(discovery), _breakpoint(false) {}

    void addAdjacent(Vertex* v1) {
        _adjacent.push_back(v1);
    }
};

int DFSVisit(Vertex* v, int* dTime, vector<Vertex*> &parents) {
    
    int ccId = v->_id;
    int c = 0;
    for (Vertex* u: v->_adjacent) {
        if (u->_dTime == -1) {
            parents[u->_id-1] = v;
            c++;
            u->_dTime = u->_lTime = (*dTime)++;
            int i = DFSVisit(u, dTime, parents);
            if (ccId < i) { ccId = i; }
            if (v->_lTime > u->_lTime) { v->_lTime = u->_lTime; }
            if (((parents[v->_id-1] == nullptr) && (c > 1)) || \
            ((parents[v->_id-1] != nullptr) && (u->_lTime >= v->_dTime))) {
                v->_breakpoint = true;
                }
        }
        else if (parents[v->_id-1] != u) {
            if (v->_lTime > u->_dTime) { v->_lTime = u->_dTime; }
        }
    }    
    return ccId;
}

int DFSVisit1(Vertex* v, int* dTime) {
    
    int big = 1;
    for (Vertex* u: v->_adjacent) {
        if ((u->_dTime == -1) && (u->_breakpoint == false)) {
            u->_dTime = u->_lTime = (*dTime)++;
            big = big + DFSVisit1(u, dTime);
        }
    }    
    return big;
}

void executeDFS(vector<Vertex*> &vVector) {

    int nSubNetworks = 0, dTime = 0;
    vector<int> ccIds;

    vector<Vertex*> parents(vVector.size(), nullptr);
    for (Vertex* v: vVector) {
        if (v->_dTime == -1) {
            v->_dTime = v->_lTime = dTime++;
            nSubNetworks++;
            ccIds.push_back(DFSVisit(v, &dTime, parents));
        }
    }
    for (Vertex* v: vVector) {
        v->_dTime = -1;
    }
    dTime= 0;
    int nBig = 0;
    int NNN = 0;
    for (Vertex* v: vVector) {
        if ((v->_dTime == -1) && (v->_breakpoint == false)) {
            v->_dTime = v->_lTime = dTime++;
            int i = DFSVisit1(v, &dTime);
            if (i > nBig) nBig = i;
        }  
        if (v->_breakpoint == true) {NNN++;};    
    }
    sort(ccIds.begin(), ccIds.end());
    printf("%d\n", nSubNetworks);
    for (unsigned int i = 0; i < (ccIds.size() - 1); i++) {
        printf("%d ", ccIds[i]);
    }
    printf("%d\n", ccIds.back());
    printf("%d\n", NNN);
    printf("%d\n", nBig);  
    
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