#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <fstream>
#include <windows.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Структура для ребра графа
struct Edge {
    int u, v;
    double weight;
    double capacity; // Для алгоритмів потоку
    double flow;     // Для алгоритмів потоку
    
    // Конструктор для зручності
    Edge(int start, int end, double w, double cap = 0) 
        : u(start), v(end), weight(w), capacity(cap), flow(0) {}
    
    // Для сортування в алгоритмі Крускала
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Структура для вершини графа на площині
struct Vertex {
    double x, y;
    int id;
};

class GraphViewer {
private:
    vector<Vertex> vertices;
    vector<Edge> edges;
    int selectedVertex = -1;
    
    // Допоміжні функції для OpenGL
    void drawCircle(double x0, double y0, double R, float r, float g, float b) {
        glColor3f(r, g, b);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            double x = x0 + R * cos((double)i / 180.0 * M_PI);
            double y = y0 + R * sin((double)i / 180.0 * M_PI);
            glVertex2d(x, y);
        }
        glEnd();
    }

    void drawText(float x, float y, const string& s) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(x, y);
        for (char c : s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

public:
    int startNode = 0;
    int endNode = 1;
    // Замінюємо стару addVertex (прибираємо автоматичне створення ребер)
    void addVertex(double x, double y) {
        vertices.push_back({x, y, (int)vertices.size()});
        cout << "Додано вершину " << vertices.back().id << endl;
    }

    // Додаємо ручне створення ребра
    void addEdge(int u, int v, double weight) {
        edges.push_back(Edge(u, v, weight));
    }
    // Додаємо ребро, де вага = фізичній відстані між вершинами
    void addEdgeByDistance(int u, int v) {
        if (u >= 0 && u < vertices.size() && v >= 0 && v < vertices.size()) {
            double dx = vertices[u].x - vertices[v].x;
            double dy = vertices[u].y - vertices[v].y;

            // Обчислюємо довжину і ділимо на 10 для зручності. round() округлює до цілого.
            double distance = round(sqrt(dx * dx + dy * dy) / 10.0);

            if (distance < 1) distance = 1; // Вага не може бути нульовою

            edges.push_back(Edge(u, v, distance));
            cout << "Створено ребро: " << u << " -> " << v << " (довжина/вага: " << distance << ")" << endl;
        }
    }

    // Функція генерації графа датчиком випадкових чисел
    void generateRandomGraph(int numVertices, int numEdges, double maxWidth, double maxHeight) {
        vertices.clear();
        edges.clear();

        // Генеруємо випадкові вершини
        for (int i = 0; i < numVertices; i++) {
            // Робимо відступи від країв екрана (по 50 пікселів), щоб було красиво
            double x = 50 + rand() % (int)(maxWidth - 100);
            double y = 50 + rand() % (int)(maxHeight - 100);
            vertices.push_back({x, y, i});
        }

        // Генеруємо випадкові ребра
        int addedEdges = 0;
        while (addedEdges < numEdges) {
            int u = rand() % numVertices;
            int v = rand() % numVertices;

            // Уникаємо петель (ребро в саму себе)
            if (u != v) {
                // Замість рандому викликаємо нашу нову логіку:
                double dx = vertices[u].x - vertices[v].x;
                double dy = vertices[u].y - vertices[v].y;
                double distance = round(sqrt(dx * dx + dy * dy) / 10.0);
                if (distance < 1) distance = 1;

                edges.push_back(Edge(u, v, distance));
                addedEdges++;
            }
        }
        cout << "Згенеровано випадковий граф: " << numVertices << " вершин, " << numEdges << " ребер." << endl;
    }

    void updateVertexPosition(int id, double x, double y) {
        if (id >= 0 && id < vertices.size()) {
            vertices[id].x = x;
            vertices[id].y = y;

            // НОВЕ: Оновлюємо ваги всіх суміжних ребер при русі
            for (auto& edge : edges) {
                if (edge.u == id || edge.v == id) {
                    double dx = vertices[edge.u].x - vertices[edge.v].x;
                    double dy = vertices[edge.u].y - vertices[edge.v].y;
                    double distance = round(sqrt(dx * dx + dy * dy) / 10.0);
                    if (distance < 1) distance = 1;
                    edge.weight = distance;
                }
            }
        }
    }

    void loadFromFile(const string& filename) {
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "Помилка: не вдалося відкрити файл " << filename << endl;
            return;
        }

        vertices.clear();
        edges.clear();

        int nv, ne; // к-ть вершин и ребер
        f >> nv >> ne;

        string s;
        getline(f, s); // считываем остаток строки

        // Считываем координаты вершин
        for (int i = 0; i < nv; i++) {
            double x, y;
            f >> x >> y;
            getline(f, s);
            vertices.push_back({x, y, i});
        }

        // Считываем ребра и их вес
        // Зчитуємо ребра та їх вагу
        for (int i = 0; i < ne; i++) {
            int u, v;
            double w;
            f >> u >> v >> w;
            getline(f, s);

            // ПЕРЕВІРКА: чи існують такі вершини в нашому графі?
            if (u >= 0 && u < nv && v >= 0 && v < nv) {
                edges.push_back(Edge(u, v, w));
            } else {
                cout << "Попередження: у файлі знайдено некоректне ребро (" << u << " - " << v << "), його пропущено." << endl;
            }
        }

        f.close();
        cout << "Граф завантажено з " << filename << ": " << nv << " вершин, " << ne << " ребер." << endl;
    }

    int getVertexAt(double x, double y, double radius = 15.0) {
        for (int i = 0; i < vertices.size(); i++) {
            double dx = vertices[i].x - x;
            double dy = vertices[i].y - y;
            if (sqrt(dx * dx + dy * dy) <= radius) {
                return i;
            }
        }
        return -1;
    }

    void display() {
        // Малюємо ребра
        for (const auto& edge : edges) {
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
            glVertex2d(vertices[edge.u].x, vertices[edge.u].y);
            glVertex2d(vertices[edge.v].x, vertices[edge.v].y);
            glEnd();

            // Малюємо вагу ребра посередині
            double midX = (vertices[edge.u].x + vertices[edge.v].x) / 2.0;
            double midY = (vertices[edge.u].y + vertices[edge.v].y) / 2.0;
            drawText(midX, midY + 5, to_string((int)edge.weight));
        }


        // Малюємо вершини
        for (const auto& v : vertices) {
            if (v.id == startNode) {
                drawCircle(v.x, v.y, 15.0, 0.0f, 0.8f, 0.0f); // Зелений - Старт
            } else if (v.id == endNode) {
                drawCircle(v.x, v.y, 15.0, 0.8f, 0.0f, 0.0f); // Червоний - Фініш (Стік)
            } else {
                drawCircle(v.x, v.y, 15.0, 0.2f, 0.6f, 1.0f); // Синій - Звичайні
            }
            drawText(v.x - 5, v.y - 5, to_string(v.id));
        }
    }

    // ==========================================
    // 1. ЕЛЕМЕНТАРНІ АЛГОРИТМИ
    // ==========================================
    void runBFS(int startVertex) {
        if (vertices.empty()) return;
        cout << "\n--- BFS (Пошук в ширину від вершини " << startVertex << ") ---" << endl;
        vector<bool> visited(vertices.size(), false);
        queue<int> q;
        
        visited[startVertex] = true;
        q.push(startVertex);
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";
            
            for (const auto& edge : edges) {
                int neighbor = -1;
                if (edge.u == u) neighbor = edge.v;
                else if (edge.v == u) neighbor = edge.u; // Якщо неорієнтований
                
                if (neighbor != -1 && !visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    void runDFS(int startVertex) {
        cout << "\n--- DFS (Пошук в глибину від вершини " << startVertex << ") ---" << endl;
        vector<bool> visited(vertices.size(), false);
        dfsHelper(startVertex, visited);
        cout << endl;
    }

private:
    void dfsHelper(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";
        for (const auto& edge : edges) {
            int neighbor = -1;
            if (edge.u == u) neighbor = edge.v;
            else if (edge.v == u) neighbor = edge.u;
            
            if (neighbor != -1 && !visited[neighbor]) {
                dfsHelper(neighbor, visited);
            }
        }
    }

public:
    // ==========================================
    // 2. МІНІМАЛЬНІ ОСТОВНІ ДЕРЕВА
    // ==========================================
    void runKruskal() {
        cout << "Алгоритм Крускала (MST):" << endl;
        // Реалізація через систему неперетинних множин (Disjoint Set Union)
        vector<Edge> mst;
        vector<int> parent(vertices.size());
        for(int i=0; i<vertices.size(); i++) parent[i] = i;
        
        auto find = [&](int i, auto& find_ref) -> int {
            if (parent[i] == i) return i;
            return parent[i] = find_ref(parent[i], find_ref);
        };
        
        auto unite = [&](int i, int j, auto& find_ref) {
            int root_i = find_ref(i, find_ref);
            int root_j = find_ref(j, find_ref);
            if (root_i != root_j) parent[root_i] = root_j;
        };

        vector<Edge> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end());

        for (const auto& edge : sortedEdges) {
            if (find(edge.u, find) != find(edge.v, find)) {
                unite(edge.u, edge.v, find);
                mst.push_back(edge);
                cout << "Ребро: " << edge.u << "-" << edge.v << " Вага: " << edge.weight << endl;
            }
        }
    }

    void runPrim() {
        if(vertices.empty()) return;
        cout << "\n--- Алгоритм Пріма (Побудова дерева від вершини " << startNode << ") ---" << endl;

        int V = vertices.size();
        vector<bool> inMST(V, false);
        // Пріоритетна черга: зберігає пари {вага_ребра, {з_відки, куди}}
        priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<>> pq;

        int startVertex = startNode;
        inMST[startVertex] = true;

        // Додаємо всі ребра від стартової вершини
        for(const auto& edge : edges) {
            if(edge.u == startVertex) pq.push({edge.weight, {edge.u, edge.v}});
            else if(edge.v == startVertex) pq.push({edge.weight, {edge.v, edge.u}});
        }

        double totalWeight = 0;
        while(!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            double w = top.first;
            int u = top.second.first;
            int v = top.second.second;

            if(inMST[v]) continue; // Якщо вершина вже в дереві - пропускаємо

            inMST[v] = true;
            totalWeight += w;
            cout << "Додано ребро: " << u << " - " << v << " (Вага: " << w << ")" << endl;

            // Додаємо нові доступні ребра
            for(const auto& edge : edges) {
                if(edge.u == v && !inMST[edge.v]) pq.push({edge.weight, {edge.u, edge.v}});
                else if(edge.v == v && !inMST[edge.u]) pq.push({edge.weight, {edge.v, edge.u}});
            }
        }
        cout << "Загальна вага дерева: " << totalWeight << endl;
    }

    // ==========================================
    // 3. НАЙКОРОТШІ ШЛЯХИ
    // ==========================================
    void runDijkstra(int startVertex) {
        if(vertices.empty()) return;
        cout << "Алгоритм Дейкстри від вершини " << startVertex << ":" << endl;
        vector<double> dist(vertices.size(), INFINITY);
        dist[startVertex] = 0;
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0, startVertex});

        while(!pq.empty()) {
            int u = pq.top().second;
            double d = pq.top().first;
            pq.pop();

            if(d > dist[u]) continue;

            for(const auto& edge : edges) {
                int v = -1;
                if(edge.u == u) v = edge.v;
                else if(edge.v == u) v = edge.u; // Якщо неорієнтований

                if(v != -1 && dist[u] + edge.weight < dist[v]) {
                    dist[v] = dist[u] + edge.weight;
                    pq.push({dist[v], v});
                }
            }
        }
        for(int i=0; i<dist.size(); i++) cout << "Вершина " << i << ", Відстань: " << dist[i] << endl;
    }

    void runBellmanFord(int startVertex) {
        if(vertices.empty()) return;
        cout << "\n--- Алгоритм Беллмана-Форда від вершини " << startVertex << " ---" << endl;

        int V = vertices.size();
        vector<double> dist(V, INFINITY);
        dist[startVertex] = 0;

        // Повторюємо релаксацію V-1 разів
        for(int i = 1; i <= V - 1; i++) {
            for(const auto& edge : edges) {
                if(dist[edge.u] != INFINITY && dist[edge.u] + edge.weight < dist[edge.v]) {
                    dist[edge.v] = dist[edge.u] + edge.weight;
                }
                // Оскільки графи з файлу зазвичай неорієнтовані:
                if(dist[edge.v] != INFINITY && dist[edge.v] + edge.weight < dist[edge.u]) {
                    dist[edge.u] = dist[edge.v] + edge.weight;
                }
            }
        }

        for(int i = 0; i < V; i++) {
            cout << "Вершина " << i << ", Мінімальна відстань: " << dist[i] << endl;
        }
    }
    void runFloydWarshall() {
        if(vertices.empty()) return;
        cout << "\n--- Алгоритм Флойда-Варшалла (Матриця відстаней) ---" << endl;

        int V = vertices.size();
        vector<vector<double>> dist(V, vector<double>(V, INFINITY));

        for(int i = 0; i < V; i++) dist[i][i] = 0;

        for(const auto& edge : edges) {
            dist[edge.u][edge.v] = edge.weight;
            dist[edge.v][edge.u] = edge.weight; // Неорієнтований граф
        }

        // Основний цикл алгоритму
        for(int k = 0; k < V; k++) {
            for(int i = 0; i < V; i++) {
                for(int j = 0; j < V; j++) {
                    if(dist[i][k] < INFINITY && dist[k][j] < INFINITY) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        // Вивід матриці
        cout << "   ";
        for(int i=0; i<V; i++) cout << i << "\t";
        cout << "\n-------------------------------------------------\n";
        for(int i = 0; i < V; i++) {
            cout << i << "| ";
            for(int j = 0; j < V; j++) {
                if(dist[i][j] == INFINITY) cout << "INF\t";
                else cout << dist[i][j] << "\t";
            }
            cout << endl;
        }
    }
    void runJohnson() {
        if(vertices.empty()) return;
        cout << "\n--- Алгоритм Джонсона (Усі найкоротші шляхи) ---" << endl;
        int V = vertices.size();

        // Крок 1: Додаємо фіктивну вершину V (з'єднану з усіма іншими вагою 0)
        vector<Edge> modifiedEdges = edges;
        for (int i = 0; i < V; i++) {
            modifiedEdges.push_back(Edge(V, i, 0));
        }

        // Крок 2: Беллман-Форд від фіктивної вершини
        vector<double> h(V + 1, INFINITY);
        h[V] = 0;
        for (int i = 1; i <= V; i++) {
            for (const auto& edge : modifiedEdges) {
                if (h[edge.u] != INFINITY && h[edge.u] + edge.weight < h[edge.v]) {
                    h[edge.v] = h[edge.u] + edge.weight;
                }
            }
        }

        // Перевірка на негативні цикли
        for (const auto& edge : modifiedEdges) {
            if (h[edge.u] != INFINITY && h[edge.u] + edge.weight < h[edge.v]) {
                cout << "Помилка: Граф містить цикл з від'ємною вагою!" << endl;
                return;
            }
        }

        // Крок 3: Перезважування ребер (щоб позбутися мінусів)
        vector<vector<pair<int, double>>> adj(V);
        for (const auto& edge : edges) {
            adj[edge.u].push_back({edge.v, edge.weight + h[edge.u] - h[edge.v]});
            // Для неорієнтованого графа додаємо зворотне ребро:
            adj[edge.v].push_back({edge.u, edge.weight + h[edge.v] - h[edge.u]});
        }

        // Крок 4: Запускаємо алгоритм Дейкстри для кожної вершини
        vector<vector<double>> dist(V, vector<double>(V, INFINITY));
        for (int u = 0; u < V; u++) {
            priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
            dist[u][u] = 0;
            pq.push({0, u});

            while (!pq.empty()) {
                auto top = pq.top();
                pq.pop();
                double d = top.first;
                int curr = top.second;

                if (d > dist[u][curr]) continue;

                for (const auto& neighbor : adj[curr]) {
                    int v = neighbor.first;
                    double weight = neighbor.second;

                    if (dist[u][curr] + weight < dist[u][v]) {
                        dist[u][v] = dist[u][curr] + weight;
                        pq.push({dist[u][v], v});
                    }
                }
            }

            // Крок 5: Відновлюємо реальні відстані
            for (int v = 0; v < V; v++) {
                if (dist[u][v] != INFINITY) {
                    dist[u][v] = dist[u][v] - h[u] + h[v];
                }
            }
        }

        // Виводимо результати у вигляді таблиці
        cout << "   ";
        for(int i=0; i<V; i++) cout << i << "\t";
        cout << "\n-------------------------------------------------\n";
        for(int i = 0; i < V; i++) {
            cout << i << "| ";
            for(int j = 0; j < V; j++) {
                if(dist[i][j] >= INFINITY / 2) cout << "INF\t";
                else cout << dist[i][j] << "\t";
            }
            cout << endl;
        }
    }

    // ==========================================
    // 4. МАКСИМАЛЬНИЙ ПОТІК
    // ==========================================
    // Допоміжна функція DFS для Форда-Фалкерсона
    bool dfsFF(vector<vector<double>>& rGraph, int u, int t, vector<bool>& visited, vector<int>& parent) {
        if (u == t) return true;
        visited[u] = true;

        for (int v = 0; v < rGraph.size(); v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                parent[v] = u;
                if (dfsFF(rGraph, v, t, visited, parent)) {
                    return true;
                }
            }
        }
        return false;
    }

    void runFordFulkerson() {
        if(vertices.empty()) return;
        int V = vertices.size();
        int s = startNode;
        int t = endNode;

        cout << "\n--- Алгоритм Форда-Фалкерсона (Макс. потік з " << s << " у " << t << ") ---" << endl;

        // Залишкова мережа
        vector<vector<double>> rGraph(V, vector<double>(V, 0));
        for(const auto& edge : edges) {
            rGraph[edge.u][edge.v] += edge.weight; // Вважаємо weight за capacity
            rGraph[edge.v][edge.u] += edge.weight;
        }

        vector<int> parent(V);
        double max_flow = 0;

        while (true) {
            vector<bool> visited(V, false);
            // Шукаємо шлях за допомогою DFS
            if (!dfsFF(rGraph, s, t, visited, parent)) {
                break; // Якщо шляху немає - завершуємо
            }

            double path_flow = INFINITY;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                rGraph[u][v] -= path_flow;
                rGraph[v][u] += path_flow; // Зворотне ребро
            }
            max_flow += path_flow;
        }
        cout << "Максимальний потік: " << max_flow << endl;
    }

    bool bfsEK(vector<vector<double>>& rGraph, int s, int t, vector<int>& parent) {
        int V = vertices.size();
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && rGraph[u][v] > 0) {
                    if (v == t) {
                        parent[v] = u;
                        return true;
                    }
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }
        return false;
    }

    void runEdmondsKarp() {
        if(vertices.empty()) return;
        int V = vertices.size();
        int s = startNode;
        int t = endNode;

        cout << "\n--- Алгоритм Едмондса-Карпа (Макс. потік з " << s << " у " << t << ") ---" << endl;

        // Будуємо залишкову мережу (residual graph)
        vector<vector<double>> rGraph(V, vector<double>(V, 0));
        for(const auto& edge : edges) {
            rGraph[edge.u][edge.v] += edge.weight; // Вважаємо weight за пропускну здатність
            rGraph[edge.v][edge.u] += edge.weight;
        }

        vector<int> parent(V);
        double max_flow = 0;

        while (bfsEK(rGraph, s, t, parent)) {
            double path_flow = INFINITY;

            // Знаходимо мінімальну пропускну здатність на знайденому шляху
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }

            // Оновлюємо залишкову мережу
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                rGraph[u][v] -= path_flow;
                rGraph[v][u] += path_flow;
            }
            max_flow += path_flow;
        }
        cout << "Максимальний потік: " << max_flow << endl;
    }
};

// Глобальний об'єкт графа
GraphViewer graph;
int windowWidth = 800;
int windowHeight = 600;


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    graph.display();
    glutSwapBuffers(); // Використовуємо подвійну буферизацію для уникнення мерехтіння
}


// Глобальні змінні
int draggedVertex = -1;
int edgeStartVertex = -1; // Зберігає індекс першої виділеної вершини для ребра

void mouse(int button, int state, int x, int y) {
    double glX = x;
    double glY = windowHeight - y;

    // ЛІВА КНОПКА: Додавання, перетягування та вибір Старт/Фініш
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            int clickedVertex = graph.getVertexAt(glX, glY);
            int modifiers = glutGetModifiers(); // Перевіряємо, чи затиснуті Shift/Ctrl/Alt

            if (clickedVertex != -1) {
                // Якщо клікнули по вершині
                if (modifiers == GLUT_ACTIVE_CTRL) {
                    // Ctrl + Клік = Старт
                    graph.startNode = clickedVertex;
                    cout << "Вершину " << clickedVertex << " встановлено як СТАРТОВУ (Джерело)." << endl;
                }
                else if (modifiers == GLUT_ACTIVE_SHIFT) {
                    // Shift + Клік = Фініш
                    graph.endNode = clickedVertex;
                    cout << "Вершину " << clickedVertex << " встановлено як КІНЦЕВУ (Стік)." << endl;
                }
                else {
                    // Звичайний клік = Перетягування
                    draggedVertex = clickedVertex;
                }
            }
            else if (modifiers == 0) {
                // Якщо клікнули по пустому місцю (без Shift/Ctrl), додаємо нову вершину
                graph.addVertex(glX, glY);
            }
        } else if (state == GLUT_UP) {
            draggedVertex = -1;
        }
    }

    // ПРАВА КНОПКА: З'єднання вершин ребрами
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            int clickedVertex = graph.getVertexAt(glX, glY);

            if (clickedVertex != -1) {
                if (edgeStartVertex == -1) {
                    // Виділяємо першу вершину
                    edgeStartVertex = clickedVertex;
                    cout << "Виділено вершину " << edgeStartVertex << " для створення ребра." << endl;
                } else {
                    // Клікнули на другу вершину - створюємо ребро
                    if (edgeStartVertex != clickedVertex) {
                        // Викликаємо нову функцію, яка сама порахує відстань
                        graph.addEdgeByDistance(edgeStartVertex, clickedVertex);
                    }
                    edgeStartVertex = -1; // Скидаємо виділення
                }
            } else {
                // Якщо клікнули правою кнопкою в пустоту - скидаємо виділення
                edgeStartVertex = -1;
                cout << "Створення ребра скасовано." << endl;
            }
        }
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (draggedVertex != -1) {
        double glX = x;
        double glY = windowHeight - y;
        graph.updateVertexPosition(draggedVertex, glX, glY);
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': graph.runBFS(graph.startNode); break;
        case '2': graph.runDFS(graph.startNode); break;
        case '3': graph.runKruskal(); break;
        case '4': graph.runPrim(); break;
        case '5': graph.runBellmanFord(graph.startNode); break;
        case '6': graph.runDijkstra(graph.startNode); break;
        case '7': graph.runFloydWarshall(); break;
        case '8': graph.runJohnson(); break;
        case '9': graph.runFordFulkerson(); break;
        case '0': graph.runEdmondsKarp(); break;
            // ... ваші попередні case '1' - '0' ...
        case 'g':
        case 'G':
            // Генеруємо: 7 вершин, 10 ребер у межах поточного вікна
            graph.generateRandomGraph(7, 10, windowWidth, windowHeight);
            glutPostRedisplay();
            break;
        case 27: exit(0); // ESC для виходу
    }
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Graph Algorithms Visualizer");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_POINT_SMOOTH);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    cout << "Управління:" << endl;
    cout << "- ЛКМ по пустому місцю: додати вершину" << endl;
    cout << "- ЛКМ по вершині: перетягнути її" << endl;
    cout << "- ПКМ по двом вершинам по черзі: з'єднати їх ребром" << endl;
    cout << "- Клавіша 'G': згенерувати випадковий граф" << endl;
    cout << "- Клавіші 1-0: запуск алгоритмів" << endl;
    graph.loadFromFile("graph4.txt");
    glutMainLoop();
    return 0;
}