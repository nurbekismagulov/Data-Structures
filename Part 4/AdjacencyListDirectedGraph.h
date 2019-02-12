
#ifndef ASSIGNMENT5_ADJACENCYLISTDIRECTEDGRAPH_H
#define ASSIGNMENT5_ADJACENCYLISTDIRECTEDGRAPH_H

#include <iostream>
#include <list>
#include <stdexcept>

using namespace std;

template<typename V, typename E>
class AdjacencyListDirectedGraph {
public:

  class Vertex;
  class Edge;

  typedef list<Vertex> VertexList;
  typedef list<Edge> EdgeList;
  typedef typename VertexList::iterator VertexItor;
  typedef typename EdgeList::iterator EdgeItor;
  typedef typename VertexList::const_iterator VertexConstItor;
  typedef typename EdgeList::const_iterator EdgeConstItor;

private:

  struct VertexObject;
  struct EdgeObject;

  typedef list<VertexObject> VertexObjectList;
  typedef list<EdgeObject> EdgeObjectList;
  typedef list<EdgeList> IncidenceEdgesList;

  typedef typename VertexObjectList::iterator VertexObjectItor;
  typedef typename EdgeObjectList::iterator EdgeObjectItor;
  typedef typename IncidenceEdgesList::iterator IncidenceEdgesItor;

  struct VertexObject {
    V elt;                             // the element stored at this vertex
    VertexObjectItor pos;              // position in vertex_collection
    IncidenceEdgesItor inc_edges_pos;  // position in inc_edges_collection

    VertexObject(V _elt) : elt(_elt) {}  // pos and inc_edges_pos are initially "NULL".
  };

  struct EdgeObject {
    E elt;                          // the element stored at this edge
    Vertex origin_vertex;           // the vertex at the origin
    Vertex dest_vertex;             // the vertex at the destination
    EdgeObjectItor pos;             // position in edge_collection
    EdgeItor origin_inc_edges_pos;  // position in an edge list in inc_edges_collection
    EdgeItor dest_inc_edges_pos;    // position in an edge list in inc_edges_collection

    EdgeObject(const Vertex& v, const Vertex& w, E _elt) : origin_vertex(v), dest_vertex(w), elt(_elt) {} // pos origin_inc_edges_pos, and dest_inc_edges_pos are initially "NULL".
  };

  VertexObjectList vertex_collection;
  EdgeObjectList edge_collection;
  IncidenceEdgesList inc_edges_collection;

public:

  class Vertex {

    VertexObject *v_obj;

  public:
    Vertex(VertexObject* v = NULL) : v_obj(v) {}

    V& operator*() const {
      return v_obj->elt;
    }

    EdgeList incidentEdges() const {
      return *(v_obj->inc_edges_pos);
    }

    bool isAdjacentTo(const Vertex& v) const {
      EdgeList looplist = v.incidentEdges();
      EdgeItor it;
      for(it = looplist.begin(); it != looplist.end(); ++it){
        if((*it).opposite(v).v_obj == v_obj)
          return true;
      }
      return false;
    }

    bool isOutgoingTo(const Vertex& v) const {
      EdgeList looplist = incidentEdges();
      EdgeItor it;
      for(it = looplist.begin(); it != looplist.end(); ++it){
        if((*it).e_obj->dest_vertex == v){
          return true;
        }
      }
      return false;
    }

    Edge outgoingEdge(const Vertex& v) const {
      EdgeList looplist = incidentEdges();
      EdgeItor it;
      for(it = looplist.begin(); it != looplist.end(); ++it){
        if((*it).e_obj->dest_vertex == v)
          return *it;
      }
      throw runtime_error("there is no edge");
    }

    EdgeList outgoingEdges() const {
      EdgeList looplist = incidentEdges();
      EdgeItor it;
      EdgeList anslist;
      for(it = looplist.begin(); it != looplist.end(); ++it){
        if((*it).e_obj->origin_vertex == *this){
          anslist.push_back(*it);
        }
      }
      return anslist;
    }

    bool operator==(const Vertex& v) const {
      return v_obj == v.v_obj;
    }

    friend class AdjacencyListDirectedGraph<V,E>;
  };

  class Edge {

    EdgeObject *e_obj;

  public:

    Edge(EdgeObject* e = NULL) : e_obj(e) {}

    E& operator*() const {
      return e_obj->elt;
    }

    VertexList endVertices() const {
      VertexList anslist;
      anslist.push_back(e_obj->origin_vertex);
      anslist.push_back(e_obj->dest_vertex);
      return anslist;
    }

    Vertex opposite(const Vertex& v) const {
      if(e_obj->origin_vertex == v)
        return e_obj->dest_vertex;
      if(e_obj->dest_vertex == v)
        return e_obj->origin_vertex;
      throw runtime_error("gg no opposite");
    }

    bool isAdjacentTo(const Edge& edge) const {
      if(e_obj->origin_vertex == edge.e_obj->origin_vertex)
        return true;
      if(e_obj->origin_vertex == edge.e_obj->dest_vertex)
        return true;
      if(e_obj->dest_vertex == edge.e_obj->origin_vertex)
        return true;
      if(e_obj->dest_vertex == edge.e_obj->dest_vertex)
        return true;
      return false;
    }

    bool isIncidentOn(const Vertex& v) const {
      if(e_obj->origin_vertex == v)
        return true;
      if(e_obj->dest_vertex == v)
        return true;
      return false;
    }

    Vertex origin() const {
      return e_obj->origin_vertex;
    }

    Vertex dest() const {
      return e_obj->dest_vertex;
    }

    bool isDirected() const {
      return true;
    }

    bool operator==(const Edge& edge) const {
      return e_obj == edge.e_obj;
    }

    friend class AdjacencyListDirectedGraph<V,E>;
  };


public:

  VertexList vertices() {
    VertexList anslist;
    VertexObjectItor it;
    for(it = vertex_collection.begin(); it != vertex_collection.end(); ++it){
      anslist.push_back(Vertex(&(*it)));
    }
    return anslist;
  }

  EdgeList edges() {
    EdgeList anslist;
    EdgeObjectItor it;
    for(it = edge_collection.begin(); it != edge_collection.end(); ++it){
      anslist.push_back(Edge(&(*it)));
    }
    return anslist;
  }

  Vertex insertVertex(const V& x) {
    vertex_collection.push_back(VertexObject(x));
    VertexObjectItor it = --vertex_collection.end();
    (*it).pos = it;
    inc_edges_collection.push_back(EdgeList());
    (*it).inc_edges_pos = --inc_edges_collection.end();
    return Vertex(&(*it));
  }

  Edge insertDirectedEdge(const Vertex& v, const Vertex& w, E x) {
    if(v.isOutgoingTo(w))
      throw runtime_error("this edge already exists");
    edge_collection.push_back(EdgeObject(v, w, x));
    EdgeObjectItor it = --edge_collection.end();
    (*it).pos = it;
    Edge tempedge(&(*it));
    (*(v.v_obj->inc_edges_pos)).push_back(tempedge);
    (*(w.v_obj->inc_edges_pos)).push_back(tempedge);
    (*it).origin_inc_edges_pos = --(*(v.v_obj->inc_edges_pos)).end();
    (*it).dest_inc_edges_pos = --(*(w.v_obj->inc_edges_pos)).end();
    return tempedge;
  }

  void eraseVertex(const Vertex& v) {
    EdgeList edgelist = v.incidentEdges();
    EdgeItor it;
    for(it = edgelist.begin(); it != edgelist.end(); ++it){
      eraseEdge(*it);
    }
    inc_edges_collection.erase(v.v_obj->inc_edges_pos);
    vertex_collection.erase(v.v_obj->pos);
  }

  void eraseEdge(const Edge& e) {
    Vertex ori = e.origin();
    Vertex des = e.dest();
    (*(ori.v_obj->inc_edges_pos)).erase(e.e_obj->origin_inc_edges_pos);
    (*(des.v_obj->inc_edges_pos)).erase(e.e_obj->dest_inc_edges_pos);
    edge_collection.erase(e.e_obj->pos);
  }

};


#endif //ASSIGNMENT5_ADJACENCYLISTDIRECTEDGRAPH_H
