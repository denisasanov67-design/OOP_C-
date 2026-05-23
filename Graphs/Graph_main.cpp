#include "Header.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");


    Graph graph("TestGraph.txt");
    vector<vector<Node*>> components = graph.connectedComponents();
    cout << "В [TestGraph.txt] найдено компонент связности: " << components.size() << endl;
    graph.saveComponents(components);


    BFS bfs(graph);
    Node* n1 = graph.findNode("0");
    Node* n2 = graph.findNode("20");
    if (n1 && n2) {
        cout << "[BFS Test] Вершины 0 и 20: "
            << (bfs.connection(n1, n2) ? "связаны" : "не связаны") << endl;
    }

    ifstream check("1000.csv");
    if (check.good()) {
        Graph graph2("1000.csv"); 
        vector<vector<Node*>> components2 = graph2.connectedComponents();
        cout << "В [1000.csv] найдено компонент связности: " << components2.size() << endl;
        graph2.saveComponents(components2);
    }
    else {
        cerr << "[Ошибка] Файл 1000.csv не найден в рабочей директории." << endl;
    }

    return 0;
}