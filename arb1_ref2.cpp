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
    Vertex* prev = nullptr;

    Vertex()
    {
      this->name = "";
      this->distance = DBL_MAX;
    }

    Vertex(std::string name)
    {
      this->name = name;
      this->distance = DBL_MAX;
    }
};

class Edge
{
  public:
    Vertex* from;
    Vertex* to;
    double cost;

    Edge(Vertex* from, Vertex* to, double cost)
    {
      this->from = from;
      this->to = to;
      // this->cost = cost;
      this->cost = cost;
    }
};

class Graph
{
  private:
    std::unordered_map<std::string, Vertex*> _vtx;
    std::list<Edge*> _edges;
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
      Edge* e1 = new Edge(_vtx[t1], _vtx[t2], cost);
      Edge* e2 = new Edge(_vtx[t2], _vtx[t1], 1/cost);

      _edges.push_back(e1);
      _edges.push_back(e2);
    }

    void update_edge(std::string t1, std::string t2, double cost) {}

    bool find_path(Vertex* &source, std::list<Edge*> &edges, int v)
    {
      for (const auto& [first, second] : _vtx)
      {
        _vtx[first]->distance = 0;
      }

      source->distance = 1;

      // int pp = 0;
      for (int i = 0; i < (v-1); i++)
      {
        
        for (Edge* edge : edges)
        {
          // pp+=1;
          if ((edge->from->distance * edge->cost > edge->to->distance) && (edge->from->distance!=DBL_MAX))
          {
            edge->to->distance = edge->from->distance * edge->cost;
            edge->to->prev = edge->from;
          }
        }
      }
      // printf("pp   %d\n", pp);

      // for (int i = 0; i < (v-1); i++)
      // {
      //   for (Edge* edge : edges)
      //   {
      //     if ((edge->from->distance * edge->cost > edge->to->distance) && (edge->from->distance!=DBL_MAX))
      //     {
      //       std::cout << "Graph contains negative weight cycle" << std::endl;
      //       return false;
      //     }
      //   }
      // }

      return true;
    }

    std::list<Vertex*> trace_path(Vertex* &source, Vertex* &destination, bool is_source=true)
    {
      Vertex* vertex = destination;
      std::list<Vertex*> path;

      while (vertex != source)
      {
        path.push_front(vertex);
        vertex = vertex->prev;
      }

      if (is_source) path.push_front(source);

      return path;
    }

    std::list<Vertex*> arbitrage_path(std::string source, std::string destination)
    {
      std::list<Vertex*> path_in, path_out;

      if (find_path(_vtx[source], _edges, _vtx.size()))
      {
        path_in = trace_path(_vtx[source], _vtx[destination]);
        return path_in;
      }
      else
      {
        std::cout << "No path found." << std::endl;
        return {};
      }

      return {};
    }
};

int main(int argc, char** argv)
{
  // printf("kkkkk");
  Graph g;

  g.add_vertex("A");
  g.add_vertex("B");
  g.add_vertex("C");
  g.add_vertex("D");
  g.add_vertex("E");
  
  g.add_edge("A", "B", 0.741);
  g.add_edge("C", "B", 0.732);
  g.add_edge("C", "A", 0.995);
  g.add_edge("D", "A", 1.521);
  g.add_edge("D", "C", 1.538);
  g.add_edge("A", "E", 1.061);
  // g.add_edge("C", "E", 1.049);

  // std::list<Vertex*> path = g.arbitrage_path('argv[1]', argv[2]);
  std::list<Vertex*> path = g.arbitrage_path("E", "A");

  for (Vertex* vertex : path)
    std::cout << vertex->name << " ";

  return 0;
}
