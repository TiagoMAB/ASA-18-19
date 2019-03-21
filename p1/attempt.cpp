#include <iostream>
#include <vector>   //to decide which one is better
#include <algorithm> 

using namespace std;

class V_node {

    Vertex* v;

    V_node *next;
};

class Vertex_list {

    V_node *first, *last;

};


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
    
    for (Vertex* v: vVector) {
        delete v;
    }
    return 0;
}