#include "node.h"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void printRT(vector<RoutingNode *> nd)
{
  /*Print routing table entries*/
  for (int i = 0; i < nd.size(); i++)
  {
    nd[i]->printTable();
  }
}

void routingAlgo(vector<RoutingNode *> nd)
{

  bool saturation = false;


  int index = 0;

  //assign an index to each node 
  for (RoutingNode *node : nd)
  {
    node->assign_index(index);
    index++;
  }

  //we dont invoke sendMsg() as there is no need to send data separately. 
  //we already have a global view of graph by Node datastructure
   
  //run Dijkstra's algorithm for all nodes  
  for (RoutingNode *node : nd)
  {
    node->recvMsg(node, nd);
    
  }
  
  

  /*Print routing table entries after routing algo converges */
  printf("Printing the routing tables after the convergence \n");
  printRT(nd);
}

void RoutingNode::recvMsg(RoutingNode* node, vector<RoutingNode*> nd)
{
  // your code here
  dijikstra(node, nd);
  
 
}

//find cost from a node to its given neighbour
int RoutingNode::find_cost(RoutingNode *n1, RoutingNode *n2)
{

  for (int i = 0; i < n1->interfaces.size(); i++)
  {
    if (n1->interfaces[i].second->getName() == n2->getName())
    {
      return n1->interfaces[i].first.getcost();
    }
  }
  return 1e9;
}

//find the ips connecting nodes with index1 and index2 (i.e. interface)
pair<string, string> RoutingNode::findIps(int index1, int index2, vector<RoutingNode *> nd)
{

  //store the nodes in these variables 
  RoutingNode *from;
  RoutingNode *to;
  pair<string, string> nexthop_interface;
  for (RoutingNode *nx : nd)
  {
    if (nx->index == index1)
    {
      from = nx;
    }
    if (nx->index == index2)
    {
      to = nx;
    }
  }
  
  //find and return the pair of ips 
  for (int i = 0; i < from->interfaces.size(); i++)
  {
    if (from->interfaces[i].second->getName() == to->getName())
    {
      nexthop_interface.first = from->interfaces[i].first.getConnectedIp();
      nexthop_interface.second = from->interfaces[i].first.getip();
    }
  }
  return nexthop_interface;
}

//backtrack path from dest node to src node (for nexthop and myinterface)
pair<string, string> RoutingNode::path(vector<RoutingNode *> nd, vector<int> parent_nodes,  RoutingNode *n2 )
{

  vector<int> path_storage;
  int temp = n2->index;
  while (temp != -1)
  {
    path_storage.push_back(temp);
    int temp1 = parent_nodes[temp];
    temp = temp1;
  }
  pair<string, string> p1;
  
  //last and second last nodes will give the nexthop and myinterface 
  pair<string, string> nexthop_myinterface = findIps(path_storage[path_storage.size() - 1], path_storage[path_storage.size() - 2], nd);
  p1.first = nexthop_myinterface.first;
  p1.second = nexthop_myinterface.second;

  return p1;
}

//calculates src to dest shortest path for each node 
void RoutingNode::dijikstra(RoutingNode *node, vector<RoutingNode *> nd)
{

  vector<int> visited(total_nodes, false);
  vector<int> parent_node(nd.size());
  vector<int> distances(total_nodes);

  //initialising the arrays
  for (int i = 0; i < total_nodes; i++)
  {
    distances[i] = 1e9;
    parent_node[i] = -1;
  }
  int vis = 0; // keeps track of no. of nodes visited

  distances[index] = 0; //src dest is 0 
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //min heap to get min value based on cost
  pq.push({0, index});

  while (!pq.empty())
  {
    int node_index = pq.top().second;
    int node_dis = pq.top().first;
    pq.pop();

    // Check for all adjacent nodes of the popped out
    // element whether the prev dist is larger than current or not.
    for (RoutingNode *nodde : nd)
    {
      if (nodde->index == node_index)
      {

        for (int i = 0; i < nodde->interfaces.size(); i++)
        {
          {
            for(RoutingNode *temp: nd){
             if(temp->getName()==nodde->interfaces[i].second->getName()){
            

            int nhr_index = temp->index;    //store the neighbours index
             
            int to_nhr_cost = find_cost(nodde, temp);   //find cost to neighbour

            //Dijkstras equation for updating distance 
            if (node_dis + to_nhr_cost < distances[nhr_index]) 
            { 
              distances[nhr_index] = node_dis + to_nhr_cost;
              
              vector<string> dest_ips; 

              //this calculated distance can be used to go to every interface of a node
              for(int j=0; j<temp->interfaces.size(); j++){
                 dest_ips.push_back(temp->interfaces[j].first.getip());
              }

              parent_node[nhr_index] = nodde->index;
              
              //backtrack path from temp till src to find nexthop and myinterface 
              pair<string, string> nexthop_myinterface = path(nd, parent_node, temp);
              
              //avoid entrying present data in table 
              if(nexthop_myinterface.first!=nexthop_myinterface.second){ 
              //add table entry for all the tables    
              for(int k=0; k<dest_ips.size(); k++)
              
              node->addNewEntry(dest_ips[k], nexthop_myinterface.first, nexthop_myinterface.second, distances[nhr_index]);

              }
              // If current distance is smaller,
              // push it into the queue.
              pq.push({node_dis + to_nhr_cost, nhr_index});
            }
          }
            }
          }
        }
      }
    }
  }
  Distances = distances;
  Visited = visited;

  // Return the list containing shortest distances
  // from source to all the nodes.
  // return distances;
}
