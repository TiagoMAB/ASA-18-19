#include <iostream>
#include <vector> 

using namespace std;

class Router { 
public:

    int _id, _dTime, _lTime;
    bool _breakpoint, _headOfNetwork;
    vector<Router*> _adjacent;

    Router(int id, int discovery = -1): _id(id), _dTime(discovery), _breakpoint(false), _headOfNetwork(false) {}

    void addAdjacent(Router* v1) {
        _adjacent.push_back(v1);
    }
};

int DFSVisit(Router* r, int* dTime, vector<Router*> &parents) {
    
    int ccId = r->_id;
    int branches = 0;

    for (Router* u: r->_adjacent) {
        if (u->_dTime == -1) {
            parents[u->_id-1] = r;
            branches++;
            u->_dTime = u->_lTime = (*dTime)++;
            int i = DFSVisit(u, dTime, parents);
            if (ccId < i) 
                ccId = i;

            if (r->_lTime > u->_lTime) 
                r->_lTime = u->_lTime; 
           
            if (((parents[r->_id-1] == nullptr) && (branches > 1)) || ((parents[r->_id-1] != nullptr) && (u->_lTime >= r->_dTime))) 
                r->_breakpoint = true;
    
        }
        else if ((parents[r->_id-1] != u) && (r->_lTime > u->_dTime)) {
            r->_lTime = u->_dTime;
        }
    }    
    return ccId;
}

int DFSVisitAfterBreakpoint(Router* r, int* dTime) {
    
    int nRouter = 1;
    for (Router* u: r->_adjacent) {
        if ((u->_dTime == -1) && (u->_breakpoint == false)) {
            u->_dTime = (*dTime)++;
            nRouter = nRouter + DFSVisitAfterBreakpoint(u, dTime);
        }
    }    
    return nRouter;
}

void executeDFS(vector<Router*> &network) {

    int nSubNetworks = 0, dTime = 0, bigNetwork = 0, nBreakpoints = 0;
    vector<Router*> parents(network.size(), nullptr);
    
    for (Router* r: network) {
        if (r->_dTime == -1) {
            r->_dTime = r->_lTime = dTime++;
            nSubNetworks++;
            network[DFSVisit(r, &dTime, parents) - 1]->_headOfNetwork = true;
        }
    }

    printf("%d\n", nSubNetworks);
    bool first = true;
    for (Router* r: network) {
        r->_dTime = -1;
        if (r->_headOfNetwork) {
            printf(first ? "%d" : " %d", r->_id);
            first = false;
        }
    }

    dTime= 0;
    for (Router* r: network) {
        if ((r->_dTime == -1) && (r->_breakpoint == false)) {
            r->_dTime = r->_lTime = dTime++;
            int nRouter = DFSVisitAfterBreakpoint(r, &dTime);
            if (nRouter > bigNetwork) bigNetwork = nRouter;
        }  
        if (r->_breakpoint == true) {nBreakpoints++;};    
    }
    
    printf("\n%d\n%d\n", nBreakpoints, bigNetwork);
}

int main() {

    int n, m, p1, p2;

    if (scanf("%d", &n) != 1) { exit(0); };  
    if (scanf("%d", &m) != 1) { exit(0); }; 
    
    vector<Router*> network(n); 
    for (int i = 0; i < n; i++) {  
        network[i] = new Router(i+1);   
    }

    while (scanf("%d %d", &p1, &p2) == 2) {
        p1--; p2--;
        network[p1]->addAdjacent(network[p2]);
        network[p2]->addAdjacent(network[p1]);
    }
    
    executeDFS(network);
    for (Router* r: network) {
        delete r;
    }

    return 0;
}