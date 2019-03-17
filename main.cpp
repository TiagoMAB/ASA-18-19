#include <iostream>
#include <list>     //to decide which one is better
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

int DFSVisit(int id, int idParent, int idRoot, int* visitedCount, int* rootCount, vector<int> &_low, vector<int> &_d, vector<int> &_breakPoints, vector<bool> &_visited, vector<vector<int>> &_adjacent) {
    
    int ccId = id;
    _visited[id - 1] = true;
    
    (*visitedCount)++;
    _d[id-1] = *visitedCount;
    _low[id-1] = *visitedCount;

    if (idParent == idRoot)
        (*rootCount)++;
    
    /*printf("before ");
    for (int i : _low)
        printf("%d - ", i);
    printf("\n");
    
    printf("%d, %d, %d, %d\n", id, idParent, idRoot, *visitedCount);*/
    
    for (int u: _adjacent[id-1]) {
        if (u == idParent)
            continue;
        else if (!_visited[u-1]) {
            int i = DFSVisit(u, id, idRoot, visitedCount, rootCount, _low, _d, _breakPoints, _visited, _adjacent);
            _low[id-1] = min(_low[id-1], _low[u-1]);
            
            if (_d[id-1] <= _low[u-1]) { _breakPoints.push_back(id); /*printf("cond %d\n", id);*/}

            /*printf("after ");
            for (int i : _low)
                printf("%d - ", i);
            printf("\n");*/
            if (ccId < i) { ccId = i; }
        }
        else
            _low[id-1] = min(_low[id-1], _d[u-1]);
    }    
    return ccId;
}

void executeDFS(int n, vector<bool> &_visited, vector<vector<int>> &_adjacent) {

    int nSubNetworks = 0;
    int visitedCount = 0;
    int rootCount;
    vector<int> ccIds;
    vector<int> _d(_visited.size());
    vector<int> _low(_visited.size());
    vector<int> _breakPoints;

    for (int i = 1; i <= n; i++) {
        if (!_visited[i-1]) {
            rootCount = 0;
            nSubNetworks++;
            ccIds.push_back(DFSVisit(i, -1, i, &visitedCount, &rootCount, _low, _d, _breakPoints, _visited, _adjacent));
            if (rootCount > 1) {
                /*printf("R %d\n", i + 1);*/
                _breakPoints.push_back(i);
            }
        }
    }

    sort(ccIds.begin(), ccIds.end());

    printf("%d\n", nSubNetworks);
    for (int i: ccIds) {
        printf("%d ", i);
    }

    /*
    printf("\nBreakpoints: \n");

    for (int i : _breakPoints) {
        printf("%d\n", i);
    }
    printf("\n");*/
}

int main() {

    int n, m, p1, p2;

    if (scanf("%d", &n) != 1) {exit(0);};  //reads number of routers
    if (scanf("%d", &m) != 1) {exit(0);};  //reads number of connections
    vector<bool> _visited(n, false);
    vector<vector<int>> _adjacent(n);

    while (scanf("%d %d", &p1, &p2) == 2) {
        p1--; p2--;
        _adjacent[p1].push_back(p2 + 1);
        _adjacent[p2].push_back(p1 + 1);
    }
    
    executeDFS(n, _visited, _adjacent);

    return 0;
}