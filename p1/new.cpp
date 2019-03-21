#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Graph {

public:

    int _m, _n;
    vector<int> _low;
    vector<vector<int>> _adjacent;


    Graph(int n, int m): _m(m), _n(n), _low(n, 0), _adjacent(n) {}

    void addAdjacent(int p1, int p2) {
        _adjacent[p1-1].push_back(p2);
        _adjacent[p2-1].push_back(p1);
    }

    void printt() {
        for(int i = 0; i < _n; i++) {
            printf("%d\n", _low[i]);
        }        
    }

};

int DFSVisit(vector<int> adjacent, Graph* g, int oldId, int* dCount) {

    int ccId = oldId;
    (*dCount)++;
    
    for (int id: adjacent) {
        if (!g->_low[id-1]) {
            g->_low[id-1] = *dCount;
            int i = DFSVisit(g->_adjacent[id-1], g, id, dCount);
            if (ccId < i) { ccId = i; }
        }
    }   
    for (int id: adjacent) {
        if ((g->_low[id-1] < g->_low[oldId-1]) && (id != oldId) && (g->_adjacent[id-1].size() != 1))  {
            g->_low[oldId-1] = g->_low[id-1];
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

    delete(g);


    return 0;
}