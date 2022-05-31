#include <iostream>
#include <limits>
#include <list>
#include <cfloat>
#include <unordered_map>
#include <cmath>

class Vertex
{
public:
  std::string name;
  double distance;
  bool gone;
  Vertex *prev = nullptr;

  Vertex()
  {
    this->name = "";
    this->distance = DBL_MAX;
    this->gone = false;
  }

  Vertex(std::string name)
  {
    this->name = name;
    this->distance = DBL_MAX;
    this->gone = false;
  }
};

class Edge
{
public:
  Vertex *from;
  Vertex *to;
  double cost;

  Edge(Vertex *from, Vertex *to, double cost)
  {
    this->from = from;
    this->to = to;
    this->cost = -log(cost);
  }
};

class Graph
{
private:
  std::unordered_map<std::string, Vertex *> _vtx;
  std::list<Edge *> _edges;

public:
  Graph()
  {
    _vtx = {};
    _edges = {};
  }

  void add_vertex(std::string name)
  {
    _vtx[name] = new Vertex(name);
  }

  void add_edge(std::string t1, std::string t2, double cost)
  {
    Edge *e1 = new Edge(_vtx[t1], _vtx[t2], cost);
    Edge *e2 = new Edge(_vtx[t2], _vtx[t1], 1 / cost);

    _edges.push_back(e1);
    _edges.push_back(e2);
  }

  void update_edge(std::string t1, std::string t2, double cost) {}

  void find_path(Vertex *&source, std::list<Edge *> &edges)
  {
    for (Edge *edge : edges)
    {
      if (edge->to->gone || edge->from != source)
        continue;

      if (edge->from->distance + edge->cost < edge->to->distance && edge->from->distance != DBL_MAX)
      {
        edge->to->distance = edge->from->distance + edge->cost;
        edge->to->prev = edge->from;
        edge->to->gone = true;
        find_path(edge->to, edges);
        edge->to->gone = false;
      }
    }
  }

  std::list<Vertex *> trace_path(Vertex *&source, Vertex *&destination, bool is_source = true)
  {
    Vertex *vertex = destination;
    std::list<Vertex *> path;

    while (vertex != source)
    {
      path.push_front(vertex);
      vertex = vertex->prev;
      if(vertex == nullptr)
        return {};
    }

    if (is_source)
      path.push_front(source);

    return path;
  }

  void arbitrage_path(std::string source, std::string destination)
  {
    std::list<Vertex *> path_in, path_out;

    for (const auto &[first, second] : _vtx)
    {
      _vtx[first]->distance = DBL_MAX;
      _vtx[first]->gone = false;
    }

    _vtx[source]->distance = 0;
    _vtx[source]->gone = true;

    find_path(_vtx[source], _edges);

    path_in = trace_path(_vtx[source], _vtx[destination]);
    
    if(path_in.size()){
        for (Vertex *vertex : path_in)
          std::cout << vertex->name << " ";
        std::cout << std::endl<< std::endl;;
    }
    else
    {
      std::cout << "No path found." << std::endl<< std::endl;;
    }
  }
};

int main(int argc, char **argv)
{
  Graph g;

  g.add_vertex("A");
  g.add_vertex("B");
  g.add_vertex("C");
  g.add_vertex("D");
  g.add_vertex("E");
  g.add_vertex("F");

  g.add_edge("A", "B", 0.741);
  g.add_edge("C", "B", 0.732);
  g.add_edge("C", "A", 0.995);
  g.add_edge("D", "A", 1.521);
  g.add_edge("D", "C", 1.538);
  g.add_edge("A", "E", 1.061);
  g.add_edge("C", "E", 1.049);

  // std::list<Vertex *> path = g.arbitrage_path(argv[1], argv[2]);
  std::cout << "**   A->F" << std::endl;
  g.arbitrage_path("A", "F");

  std::cout << "**   A->C" << std::endl;
  g.arbitrage_path("A", "C");

  std::cout << "**   E->A" << std::endl;
  g.arbitrage_path("E", "A");



  return 1;
}
