#include "Header.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    // Тест 1
    cout << "Тест 1: TestGraph.txt" << endl;
    Graph graph("TestGraph.txt");

    // очистка старых файлов перед запуском 
    graph.cleanupOldFiles("TestGraph_component_*.txt");
    graph.cleanupOldFiles("1000_component_*.txt");

    vector<vector<Node*>> components1 = graph.connectedComponents();
    graph.saveComponents(components1, "TestGraph_");

    BFS bfs(graph);
    Node* n1 = graph.findNode("0");
    Node* n2 = graph.findNode("20");
    if (n1 && n2) {
        cout << "BFS: вершины 0 и 20 "
            << (bfs.connection(n1, n2) ? "связаны" : "не связаны") << endl;
    }

    //  ТЕСТ 2 
    cout << "\nТест 2: 1000.csv " << endl;

    Graph graph2("1000.csv");
    vector<vector<Node*>> components2 = graph2.connectedComponents();
    graph2.saveComponents(components2, "1000_");

    return 0;
}
