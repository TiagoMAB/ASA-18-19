#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Graph {

public:

    int _m, _n;
    vector<int> _low;
    list<int> *_adjacent;
    vector<bool> _end;


    Graph(int n, int m): _m(m), _n(n), _low(n, 0), _end(n, false) { 
        _adjacent = new list<int>[n]; 
    }

    void addAdjacent(int p1, int p2) {
        _adjacent[p1-1].push_back(p2);
        _adjacent[p2-1].push_back(p1);
    }

    void printt() {
        for(int i = 0; i < _n; i++) {
            printf("%d\n", _low[i]);
        }        
    }

    ~Graph() {
        for(int i = 0; i < _n; i++) {
            _adjacent[i].clear();
        }
        delete[] _adjacent;
    }

};

int DFSVisit(list<int> &adjacent, Graph* g, int oldId, int* dCount) {

    int ccId = oldId;
    (*dCount)++;
    printf("%d -\n", *dCount);
    
    for (int id: adjacent) {
        if (!g->_low[id-1]) {
            g->_low[id-1] = *dCount;
            printf("Id: %d | Low: %d\n", id, *dCount);
            int i = DFSVisit(g->_adjacent[id-1], g, id, dCount);
            if (ccId < i) { ccId = i; }
        }
    }
    g->_end[id-1] = true;   
    for (int id: adjacent) {
        if ((g->_low[id-1] < g->_low[oldId-1]) && (id != oldId) && (g->_adjacent[id-1].size() != 1))  {
            printf("Antes: Id: %d | Low: %d | OldId: %d\n", id, g->_low[id-1], g->_low[oldId-1]);
            g->_low[oldId-1] = g->_low[id-1];
            printf("Dps: Id: %d | Low: %d | OldId: %d\n", id, g->_low[id-1], g->_low[oldId-1]);
        }
    }
    return ccId;
}

void executeDFS(Graph* g) {

    int nSubNetworks = 0;
    list<int> ccIds;
    int dCount = 1;

    for (int id = 0; id < g->_n; id++) {
        if (!g->_low[id]) {
            g->_low[id] = dCount;
            nSubNetworks++;
            ccIds.push_back(DFSVisit(g->_adjacent[id], g, id+1, &dCount));
        } 
    }

    ccIds.sort();
    printf("%d\n", nSubNetworks);
    for (int i: ccIds) {
        printf("%d ", i);
    }
    
    printf("---\n");
}

int main() {

    int n, m, p1, p2;
    
    if (scanf("%d", &n) != 1) {exit(0);};  //reads number of routers
    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections
    Graph* g = new Graph(n, m);

    while (scanf("%d %d", &p1, &p2) == 2) {
        g->addAdjacent(p1, p2);
    }

    executeDFS(g);

    g->printt();
    delete(g);


    return 0;
}