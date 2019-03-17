#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class Graph {

public:

    int _m, _n;
    vector<bool> _visited;
    list<int> *_adjacent;


    Graph(int n, int m): _m(m), _n(n), _visited(n, false) { 
        _adjacent = new list<int>[n]; 
    }

    void addAdjacent(int p1, int p2) {
        _adjacent[p1-1].push_back(p2);
        _adjacent[p2-1].push_back(p1);
    }

    ~Graph() {
        for(int i = 0; i < _n; i++) {
            _adjacent[i].clear();
        }
        delete[] _adjacent;
    }

};

int DFSVisit(list<int> &adjacent, Graph* g, int oldId) {

    int ccId = oldId;
    for (int id: adjacent) {
        if (g->_visited[id-1] == false) {
            g->_visited[id-1]= true;
            int i = DFSVisit(g->_adjacent[id-1], g, id);
            if (ccId < i) { ccId = i; }
        }
    }    
    return ccId;
}

void executeDFS(Graph* g) {

    int nSubNetworks = 0;
    list<int> ccIds;

    for (int id = 0; id < g->_n; id++) {
        if (!g->_visited[id]) {
            nSubNetworks++;
            g->_visited[id] = true;
            ccIds.push_back(DFSVisit(g->_adjacent[id], g, id+1));
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
    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections
    Graph* g = new Graph(n, m);

    while (scanf("%d %d", &p1, &p2) == 2) {
        g->addAdjacent(p1, p2);
    }

    executeDFS(g);

    delete(g);


    return 0;
}