
#pragma once
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

class Node;

typedef std::set<Node*>::const_iterator node_iterator; //node - базовая единица данных, используется для построения графа

// Класс неориентированного граффа 
class Graph {
 set<Node*> nodes;

public:
 Graph(){}
 Graph(const char* file_name);
 ~Graph();

 void addNode(Node* node);
 void removeNode(Node* node);

 void addEdge(Node* begin, Node* end); // добавить ребро
 void removeEdge(Node* begin, Node* end); // удалить ребро
 
 Node* findNode(const string& name);

 node_iterator begin() const; // указатель на первый элемент 
 node_iterator end() const; // укзатель на последний элемент

 vector<vector<Node*» connectedComponents();
 void saveComponents(vector<vector<Node*»& components, const string& prefix = "");
 void cleanupOldFiles(const string& pattern); 
};

// Класс узлов графа 
class Node {
 string name;
 set<Node*> neighbours; // множество указателей соседних узлов 
 void addNeighbour(Node* neighbour);
 void removeNeighbour(Node* neighbour);

public:
 
 Node(const string& aname);
 const string& getName() const; // позвращает имя узла 
 node_iterator nb_begin() const; // указатель на первого соседа узла 
 node_iterator nb_end() const; // указатель на последнего соседа узла 
 friend class Graph; // доступ к полям и методам класса Graph 
};

// Обход графа в ширину 
class BFS {
 const Graph& graph;
public:
 BFS(const Graph& agraph) : graph(agraph) {}
 bool connection(Node* begin, Node* end);
};

// Обход графа в глубину
class DFS{
 const Graph & graph;
 std::set<Node*> visited;
 bool connected(Node* begin, Node* end, int depth);
public:
 DFS(const Graph& agraph) : graph(agraph) {}
 bool connected(Node* begin, Node* end);
};

// Класс для отмеченных узлов
struct MarkedNode { 
 Node* node;
 int mark;
 Node* prev; // пердыдущий узел
 MarkedNode(Node* anode = 0, int amark = 0, Node* aprev = 0) :
  node(anode), mark(amark), prev(aprev) {} // передаем значения переменным через указатель 
};

 // приоритетная очередь, класс для хранения вершин с приоритетом  
class PriorityQueue {  
 vector<MarkedNode> nodes;

public:
 MarkedNode pop();
 void push(Node* node, int mark, Node* prev);// Добавляет вершину в очередь с приоритетом, запоминая её метку и предыдущую вершину
 bool empty() const { return nodes.empty(); } // проверка на пустоту очереди
};
