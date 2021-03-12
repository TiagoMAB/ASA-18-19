#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>

using namespace std;

class Vertex;

class Edge {
public:

    Vertex *_in, *_out;
    int _capacity, _flow;
    bool _isStation;

    Edge(Vertex* in, Vertex* out, int capacity = 0, int flow = 0, bool isStation = false): _in(in), _out(out), _capacity(capacity), _flow(flow), _isStation(isStation) {}

};

class Vertex {
public:

    int _index, _height, _excess, _nNeighbors;
    vector<Edge*> _neighbors;
    bool _discovered;

    Vertex(int index, int height = 0, int excess = 0, int nNeighbors = 0): _index(index), _height(height), _excess(excess), _nNeighbors(nNeighbors), _discovered(false) {}

    void addNeighbor(Edge* e) {
        _nNeighbors++;
        _neighbors.push_back(e);
    }

};

struct Comparator {
    bool operator() (Edge* e1,Edge* e2) const { 
        if (e1->_in->_index < e2->_in->_index) {
            return true;
        }
        else if (e1->_in->_index == e2->_in->_index) {
            return e1->_out->_index < e2->_out->_index;
        }
        return false;
    }
} comparator;

class Graph {
public:

    vector<Edge*> _edges; 
    vector<Vertex*> _vertices; 
    queue<int> _queue;
    int _dischargeCount, _stationOutIndex, _e;   

    Graph(int nVertex, int stationOutIndex, int e): _dischargeCount(0), _stationOutIndex(stationOutIndex), _e(e) {
        for (int i = 0; i < nVertex; i++) {
            _vertices.push_back(new Vertex(i));
        }
    }

    void addEdge(int in, int out, int capacity, bool isStation = false) {
        _edges.push_back(new Edge(_vertices[in], _vertices[out], capacity, 0, isStation));
        _vertices[in]->addNeighbor(_edges.back());
    }

    Edge* findOppositeEdge(Vertex* u, Vertex* v) {
        for (Edge* e: v->_neighbors) {
            if (e->_out->_index == u->_index) {
                return e;
            }
        }
        if (u->_index >= _stationOutIndex - _e && v->_index >= _stationOutIndex - _e && abs(u->_index - v->_index) == _e)
            _edges.push_back(new Edge(v, u, 0, 0, true));
        else
            _edges.push_back(new Edge(v, u));
        v->addNeighbor(_edges.back());

        return _edges.back();
    }

    void preFlowInitialize() {
        Vertex* s = _vertices[0];
        s->_height = _vertices.size();

        for (Edge* e: s->_neighbors) {
            Vertex* v = e->_out;
            Edge* o = findOppositeEdge(s, v);
            e->_flow = e->_capacity;
            v->_excess = e->_capacity;
            s->_excess -= e->_capacity;
            o->_flow -= e->_capacity;
            if (v->_index > 1 && v->_excess != 0)  {
                _queue.push(v->_index); 
                _dischargeCount++;
            }
        }
    }

    void push(Vertex* u, Vertex* v, Edge* e) {
        int flow = min(u->_excess, e->_capacity - e->_flow);
        Edge* o = findOppositeEdge(u, v);

        o->_flow -= flow;
        e->_flow += flow; 
        v->_excess += flow;
        u->_excess -= flow;
        if (v->_index > 1 && v->_excess != 0)  {
            _queue.push(v->_index);
            _dischargeCount++;
        }
    }

    void relabel(Vertex* u) {
        int min = __INT_MAX__;

        for (int i = 0; i < u->_nNeighbors; i++) {
            Edge* e = u->_neighbors[i];
            if (e->_capacity - e->_flow > 0 && e->_out->_height < min ) {
                min = e->_out->_height;
            }
        }

        u->_height = 1 + min;
    }

    void discharge(Vertex* u) {
        int index = 0;
        Edge* e;
        Vertex* v;

        while (u->_excess > 0) {
            e = u->_neighbors[index];
            v = e->_out;
            if (index >= u->_nNeighbors) {
                relabel(u);
                index = 0;
            }
            else if (e->_capacity - e->_flow > 0 && u->_height == v->_height + 1) {
                push(u, v, e);
            }
            else {
                index++;
            }
        }
    }

    void pushRelabelWithQueue() {
        for (int i = 0; i < _dischargeCount; i++) { 
            Vertex* v = _vertices[_queue.front()];
            discharge(v);
            _queue.pop();
        }
    }

    void clearNeighbors() {
        for (Vertex* v: _vertices) {
            v->_neighbors.clear();
            v->_nNeighbors = 0;
        }
    }

    void reverseGraph() {
        for (Edge* e: _edges) {
            Vertex* out = e->_in;
            e->_in = e->_out;
            e->_out = out;
            e->_in->addNeighbor(e);
        }
    }

    void dfs(Vertex* v, vector<Edge*> &minimumCutEdges, vector<int> &minimumCutStations) {
        v->_discovered = true;
        for (Edge* e: v->_neighbors) {
            if (e->_capacity != e->_flow && e->_out->_discovered == false) {
                dfs(e->_out, minimumCutEdges, minimumCutStations);
            }
        }
    }

    void findMinimumCut() {
        vector<Edge*> minimumCutEdges(0);
        vector<int> minimumCutStations(0);

        dfs(_vertices[1], minimumCutEdges, minimumCutStations);
        for (Vertex* v: _vertices) {
            if (!v->_discovered) {
                for (Edge* e: v->_neighbors) {
                    if (e->_out->_discovered && e->_isStation) {
                        int index = min(e->_out->_index, e->_in->_index);
                        minimumCutStations.push_back(index);
                    }
                    else if (e->_out->_discovered && e->_flow != 0 && e->_out->_index != 0 && v->_index != 0) {
                        minimumCutEdges.push_back(e);
                    }
                }
            }
        }

        for (int i: minimumCutStations) {
            if (i == minimumCutStations.back()) {
                printf("%d", i);
            }
            else {
                printf("%d ", i);
            }
        }

        sort(minimumCutEdges.begin(), minimumCutEdges.end(), comparator);
        printf("\n");
        for (Edge* e: minimumCutEdges) {
            int in = e->_in->_index, out = e->_out->_index;
            if (in >= _stationOutIndex) {
                in -= _e;
            }
            if (out >= _stationOutIndex) {
                out -= _e;
            }
            printf("%d %d\n", in, out);
        }
    }

    ~Graph() {
        for (Vertex* v : _vertices) {
            delete v;
        }

        for (Edge* e: _edges) {
            delete e;
        }
    }

};

int main() {
    int f, e, t, nVertex, capacity, in, out;

    if (scanf("%d %d %d", &f, &e, &t) != 3) { exit(-1); }
    nVertex = f + 2*e + 2;
    Graph* g = new Graph(nVertex, 2 + f + e, e);
    for (int i = 2; i < f + 2; i++) {
        if (scanf("%d", &capacity) != 1) { exit(-1); }
        g->addEdge(0, i, capacity);
    }

    for (int i = f + 2; i < e + f + 2; i++) {
        if (scanf("%d", &capacity) != 1) { exit(-1); }
        g->addEdge(i, i + e, capacity, true);
    }

    for (int i = 0; i < t; i++) {
        if (scanf("%d %d %d", &in, &out, &capacity) != 3) { exit(-1); }
        if (in >= f + 2) {
            in += e;
        }
        g->addEdge(in, out, capacity);
    }

    g->preFlowInitialize();
    g->pushRelabelWithQueue();
    g->clearNeighbors();
    g->reverseGraph();
    printf("%d\n", g->_vertices[1]->_excess);
    g->findMinimumCut();
    delete g;

    return 0;
}