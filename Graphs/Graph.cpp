#include "Header.h"
#include <queue>
#include <fstream>

Node::Node(const string& aname) : name(aname){}

const string& Node::getName() const{
	return (name);
}

//Возвращает итератор на первого соседа узла
node_iterator Node::nb_begin() const {
	return neighbours.begin();
}

// Возвращает итератор на конец списка соседей узла
node_iterator Node::nb_end() const {
	return neighbours.end();
}

// Добавляет соседний узел в множество соседей
void Node::addNeighbour(Node* neighbour) {
	neighbours.insert(neighbour);
}

// Удаляет соседний узел из множества соседей
void Node::removeNeighbour(Node* neighbour) {
	neighbours.erase(neighbour);
}

// Ищет узел по имени в графе
Node* Graph::findNode(const string& name) {
	for (node_iterator it = begin(); it != end(); ++it) {
		if ((*it) -> getName() == name) return *it;
	}
	return nullptr;
}

// Добавление узла в множество вершин графа
void Graph::addNode(Node* node) {
	nodes.insert(node); // вставляет узел в множество 
}


// Возвращает итератор на первый узел в множестве графа
node_iterator Graph::begin() const {
	return nodes.begin(); // уазатель на первыый узел
}

// Возвращает итератор на конец множества графа
node_iterator Graph::end() const {
	return nodes.end(); // указатель на последний узел 
}


// Добавляет неориентированное ребро между двумя вершинами
void Graph::addEdge(Node* begin, Node* end) { 

	if (nodes.find(begin) == nodes.end()) // если вершина begin не найдена во всех вершинах выходим 
		return;
	if (nodes.find(end) == nodes.end())
		return;

	// ребро добавляется в обе стороны 
	begin->addNeighbour(end); // у вершнины bеgin добавляем соседа 
	end->addNeighbour(begin);
}

// констуктор для графа с парами смежных вершин 
Graph::Graph(const char* file_name) {
	ifstream file(file_name);

	// Пропускаем первую строку (заголовок "Source Target")
	string source, target;
	while (file >> source >> target) {
		if (source == "Source" || source == "Target") continue; 


		Node* s = findNode(source);

		// если не найдена верши source 
		if (s == nullptr) {
			s = new Node(source); // создает новый объект вершины в динамической памяти 
			addNode(s);
		}	

		Node* t = findNode(target);

		// если не найдена вершина target 
		if (t == nullptr) {
			t = new Node(target);
			addNode(t);
		}

		addEdge(s, t);
	}
	file.close();
}


Graph::~Graph() {
	for (node_iterator it = begin(); it != end(); ++it) {
		delete* it;
	}
}


// Удаление узла из графа
void Graph::removeNode(Node* node) {
	nodes.erase(node); // удаляем узел из множества 
	for (set<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++) { // проходим по каждому оставшимуся узлу 
		(*it)->removeNeighbour(node); // удаляем связь между удаленным узлом и его соседом 
	}
}


// Удаляет ребро между двумя вершинами
void Graph::removeEdge(Node* begin, Node* end) {

	if (nodes.find(begin) == nodes.end()) // если вершина не найдена
		return;
	if (nodes.find(end) == nodes.end())
		return;

	// удаляет указатель на соседей 
	begin->removeNeighbour(end);
	end->removeNeighbour(begin);
}


// Проверка связи между двумя вершинами алгоритмом поиска в ширину
bool BFS::connection(Node* begin, Node* end) {

	if (begin == end) return true;  // сравниваем адреса в памяти 
	queue<Node*> nodes; // очередь с узлами 
	nodes.push(begin); // очередь обработки 
	set<Node*> visited;
	visited.insert(begin);

	while (!nodes.empty()) { // пока есть вершины 
		Node* next = nodes.front(); // указатель на первый элемент 
		nodes.pop(); // удаление 1 элемента из очереди 

		visited.insert(next);

		// обход соседей вершины 
		for (node_iterator it = next->nb_begin(); it != next->nb_end(); it++)
			
			// проверка соседей на посещенных 
			if (visited.find(*it) == visited.end()) {
				visited.insert(*it);
				nodes.push(*it);
			}
	}
	return false;
}


// публичный метод, поиск в глубину(обертка)
bool DFS::connected(Node* begin, Node* end) {
	visited.clear(); 
	return connected(begin, end, 0); // очищает множество перед новым поиском 
}


// Приватный рекурсивный метод поиска в глубину
bool DFS::connected(Node* begin, Node* end, int depth) {

	if (begin == end) return true;
	visited.insert(begin);
	for (node_iterator it = begin->nb_begin(); it != begin->nb_end(); it++) {
		if (visited.find(*it) == visited.end()) {
			if (connected(*it, end, depth + 1)) return true;
		}
	}
	return false;
}

// Извлекание элемента с наивысшим приоритетом
MarkedNode PriorityQueue::pop() {
	MarkedNode mn = nodes.front();
	nodes.erase(nodes.begin());
	return mn;
}

// Добавляет элемент в приоритетную очередь
void PriorityQueue::push(Node* node, int mark, Node* prev) {
	std::vector<MarkedNode>::iterator it = nodes.begin();
	MarkedNode mn(node, mark, prev);
	while (it != nodes.end() && mark < it->mark) ++it;
	nodes.insert(it, mn);
}

// Находит все непересекающиеся компоненты связности графа
vector < vector < Node* >> Graph::connectedComponents() {
	vector < vector < Node* >> components;
	set<Node*> visited;

	for (node_iterator it = begin();it != end();++it) {

		Node* start = *it;

		if (visited.find(start) != visited.end()) continue;

		vector<Node*> component;
		queue<Node*> q;
		q.push(start);
		visited.insert(start);

		while (!q.empty()) {

			Node* current = q.front();
			q.pop();
			component.push_back(current);

			for (node_iterator nb =current->nb_begin();nb != current->nb_end();++nb) {

				if (visited.find(*nb) == visited.end()) {

					visited.insert(*nb);
					q.push(*nb);
				}
			}
		}

		components.push_back(component);
	}

	return components;
}

// Сохраняет каждую компоненту связности с префиксом  в отдельный текстовый файл 
void Graph::saveComponents(vector<vector<Node*>>& components, const string& prefix) {
	cout << "saveComponents: сохраняю " << components.size() << " компонент" << endl;

	for (size_t i = 0; i < components.size(); i++) {
		cout << "  Компонента " << i + 1 << " содержит " << components[i].size() << " вершин" << endl;

		string filename = prefix + "component_" + to_string(i + 1) + ".txt";
		remove(filename.c_str());

		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "  Ошибка: не могу открыть файл " << filename << endl;
			continue;
		}

		set<pair<string, string>> edges;

		// Собираем все рёбра компоненты
		for (Node* node : components[i]) {
			for (node_iterator nb = node->nb_begin(); nb != node->nb_end(); ++nb) {
				string a = node->getName();
				string b = (*nb)->getName();

				// Нормализуем порядок (меньшее имя первым)
				if (a < b)
					edges.insert({ a, b });
				else
					edges.insert({ b, a });
			}
		}

		cout << "  Найдено уникальных рёбер: " << edges.size() << endl;

		// Записываем рёбра в файл
		for (const auto& edge : edges) {
			file << edge.first << " " << edge.second << endl;
		}

		// Принудительная запись и закрытие
		file.flush();
		file.close();

		// Проверка размера файла
		ifstream check(filename, ios::binary | ios::ate);
		streamsize size = check.tellg();
		check.close();

		cout << "  Файл " << filename << " сохранён, размер: " << size << " байт" << endl;

		// Если файл пустой, выводим предупреждение
		if (size == 0) {
			cout << "  ВНИМАНИЕ: Файл пустой! Компонента содержит "
				<< components[i].size() << " вершин, но 0 рёбер" << endl;
		}
	}
}

void Graph::cleanupOldFiles(const string& pattern) {
	string cmd = "del " + pattern + " /Q 2>nul";
	system(cmd.c_str());
}
