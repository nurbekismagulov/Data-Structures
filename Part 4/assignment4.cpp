

#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>
#include <list>
#include <vector>
#include <map>
#include <limits>
#include <set>
#include "AdjacencyListDirectedGraph.h"
#include "FlightMap.h"


double FlightMap::calcRouteDistance(const list<string> route) {
    list<string>::const_iterator it;
    list<string>::const_iterator prev_it;
    double totaldistance = 0;
    for(it = route.begin(); it != route.end(); ++it){
      if(!isAirportExist(*it))
        throw runtime_error("airpot does not exits in current route");
      if(it != route.begin()){
        if(!isConnectionExist(*prev_it, *it))
          throw runtime_error("no such connetion in route");
        totaldistance += *(airport_db[*prev_it].outgoingEdge(airport_db[*it]));
      }
      prev_it = it;
    }
    return totaldistance;
}


list<string> FlightMap::findRoute(const string &airport1, const string &airport2) {
  list<string> ans;
  if(!isAirportExist(airport1))
    throw runtime_error("no such airport1");
  if(!isAirportExist(airport2))
    throw runtime_error("no such airport2");
  map<string, bool> visited;
  map<string, string> parent;
  dfs(airport_db[airport1], visited, parent);
  string current_airport = airport2;
  if(parent.count(airport2)){
    while(current_airport != airport1){
      ans.push_front(current_airport);
      current_airport = parent[current_airport];
    }
    ans.push_front(current_airport);
  }
  return ans;
}


list<string> FlightMap::findReachableAirports(const string &airport) {
  list<string> ans;
  if(!isAirportExist(airport))
    throw runtime_error("no such airport");
  map<string, bool> visited;
  map<string, string> parent;
  dfs(airport_db[airport], visited, parent);
  map<string, bool>::iterator it;
  for(it = visited.begin(); it != visited.end(); ++it){
    string name = (*it).first;
    if(name != airport){
      ans.push_back(name);
    }
  }
  return ans;
}


list<string> FlightMap::findShortestRoute(const string &airport1, const string &airport2) {
  if(!isAirportExist(airport1))
    throw runtime_error("no such airport1");
  if(!isAirportExist(airport2))
    throw runtime_error("no such airport2");
  set<pair<double, string> > st;
  map<string, double> len;
  map<string, string> parent;
  list<string> ans;
  len[airport1] = 0;
  st.insert(make_pair(0, airport1));
  while(!st.empty()){
    double current_len = (*st.begin()).first;
    string name = (*st.begin()).second;
    st.erase(st.begin());
    FlightGraph::EdgeList successors;
    FlightGraph::Vertex node = airport_db[name];
    successors = node.outgoingEdges();
    for(FlightGraph::EdgeItor it = successors.begin(); it != successors.end(); ++it){
      double dist = **it;
      string nextnode = *((*it).dest());
      if(len.count(nextnode)){
        if(len[nextnode] > current_len + dist){
          st.erase(make_pair(len[nextnode], nextnode));
          len[nextnode] = current_len + dist;
          st.insert(make_pair(current_len + dist, nextnode));
          parent[nextnode] = name;
        }
      }
      else{
        len[nextnode] = current_len + dist;
        st.insert(make_pair(current_len + dist, nextnode));
        parent[nextnode] = name;
      }
    }
  }
  string current_airport = airport2;
  if(parent.count(airport2)){
    while(current_airport != airport1){
      ans.push_front(current_airport);
      current_airport = parent[current_airport];
    }
    ans.push_front(current_airport);
  }
  return ans;
}



void FlightMap::printAllShortestRoutes(const string &airport) {
  if(!isAirportExist(airport))
    throw runtime_error("no such airport1");
  set<pair<double, string> > st;
  map<string, double> len;
  map<string, string> parent;
  len[airport] = 0;
  st.insert(make_pair(0, airport));
  while(!st.empty()){
    double current_len = (*st.begin()).first;
    string name = (*st.begin()).second;
    st.erase(st.begin());
    FlightGraph::EdgeList successors;
    FlightGraph::Vertex node = airport_db[name];
    successors = node.outgoingEdges();
    for(FlightGraph::EdgeItor it = successors.begin(); it != successors.end(); ++it){
      double dist = **it;
      string nextnode = *((*it).dest());
      if(len.count(nextnode)){
        if(len[nextnode] > current_len + dist){
          st.erase(make_pair(len[nextnode], nextnode));
          len[nextnode] = current_len + dist;
          st.insert(make_pair(current_len + dist, nextnode));
          parent[nextnode] = name;
        }
      }
      else{
        len[nextnode] = current_len + dist;
        st.insert(make_pair(current_len + dist, nextnode));
        parent[nextnode] = name;
      }
    }
  }
  map<string,string>::iterator it;
  for(it = parent.begin(); it != parent.end(); ++it){
    list<string> ans;
    string name = (*it).first;
    while(name != airport){
      ans.push_front(name);
      name = parent[name];
    }
    ans.push_front(airport);
    printRoute(ans);
    cout << " (Distance = " << calcRouteDistance(ans) << ")" << endl;
  }
}
