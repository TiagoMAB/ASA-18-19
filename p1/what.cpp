#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Vertex {
    
public:

    int _id;

    bool _visited;

    list<int> lst;

    Vertex(int id, bool visited = false): _id(id), _visited(visited) {}

};

int main() {

    Vertex* v = new Vertex(3);
    Vertex* v1 = new Vertex(4);

    delete v1;
    delete v;
    
}
