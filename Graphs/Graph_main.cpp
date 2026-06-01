#include "Header.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

     
    cout « "TestGraph.txt" « endl;
    Graph graph("Тест 1: TestGraph.txt");
    
    // очистка файлов перед запуском
    graph.cleanupFiles("TestGraph_component_*.txt");
    graph.cleanupFiles("1000_component_*.txt");
    vector<vector<Node*>> components = graph.connectedComponents();

    graph.saveComponents(components, "TestGraph_");

    BFS bfs(graph);
    Node* n1 = graph.findNode("0");
    Node* n2 = graph.findNode("20");

    if (n1 && n2) {
        cout « "BFS: вершины 0 и 20 "
            « (bfs.connection(n1, n2) ? "связаны" : "не связаны") « endl;
    }

    cout <<"Тест 2: 1000.csv" << endl;
    ifstream check("1000.csv");
        
    Graph graph2("1000.csv");
    vector<vector<Node*>> components2 = graph2.connectedComponents();

    graph2.saveComponents(components2, "1000_");
    cout « "\nВсе файлы сохранены в текущую директорию: " « endl;
    system("ls -la component_*.txt 2>/dev/null || dir component_*.txt");
    
            
    return 0;
}
