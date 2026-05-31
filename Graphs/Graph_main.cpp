#include "Header.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    // очистка файлов перед запуском 
    cout « "TestGraph.txt" « endl;
    Graph graph("TestGraph.txt");
    vector<vector<Node*» components = graph.connectedComponents();

    graph.saveComponents(components, "TestGraph_");

    BFS bfs(graph);
    Node* n1 = graph.findNode("0");
    Node* n2 = graph.findNode("20");

    if (n1 && n2) {
        cout « "BFS: вершины 0 и 20 "
            « (bfs.connection(n1, n2) ? "связаны" : "не связаны") « endl;
    }

    cout « "Тест 2: 1000.csv" « endl;
    ifstream check("1000.csv");
        
    Graph graph2("1000.csv");
    vector<vector<Node*» components2 = graph2.connectedComponents();

    graph2.saveComponents(components2, "1000_component_");
    cout « "\nВсе файлы сохранены в текущую директорию: " « endl;
    system("ls -la component_*.txt 2>/dev/null || dir component_*.txt");


    cout « "Файл 1000.csv" « endl;
    for (int i = 1; i <= 2; i++) {
        string fname = "component_" + to_string(i) + ".txt";
        ifstream test(fname);
        if (test.good()) {
            test.close();
            // Получаем размер файла
            test.open(fname, ios::binary | ios::ate);
            streamsize size = test.tellg();
            test.close();
        }

    }

    // Вывод содержимого маленьких файлов для проверки
    for (int i = 1; i <= 3; i++) {
        string fname = "component_" + to_string(i) + ".txt";
        ifstream f(fname);
        if (f.is_open()) {
            cout « "\n=== " « fname « " ===" « endl;
            string line;
            while (getline(f, line)) {
                cout « "[" « line « "]" « endl; // [] покажут пробелы
            }
            f.close();
        }
    }

    return 0;
}
