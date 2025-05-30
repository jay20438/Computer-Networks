#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
  Each row in the table will have these fields
  dstip:	Destination IP address
  nexthop: 	Next hop on the path to reach dstip
  ip_interface: nexthop is reachable via this interface (a node can have multiple interfaces)
  cost: 	cost of reaching dstip (number of hops)
*/
class RoutingEntry{
 public:
  string dstip, nexthop;
  string ip_interface;
  int cost;
};

/*
 * Class for specifying the sort order of Routing Table Entries
 * while printing the routing tables
 * 
*/
class Comparator{
 public:
  bool operator()(const RoutingEntry &R1,const RoutingEntry &R2){
    if (R1.cost == R2.cost) {
      return R1.dstip.compare(R2.dstip)<0;
    }
    else if(R1.cost > R2.cost) {
      return false;
    }
    else {
      return true;
    }
  }
} ;

/*
  This is the routing table
*/
struct routingtbl {
  vector<RoutingEntry> tbl;
};

/*
  Message format to be sent by a sender
  from: 		Sender's ip
  mytbl: 		Senders routing table
  recvip:		Receiver's ip
*/
class RouteMsg {
 public:
  string from;			// I am sending this message, so it must be me i.e. if A is sending mesg to B then it is A's ip address
  struct routingtbl *mytbl;	// This is routing table of A
  string recvip;		// B ip address that will receive this message
};

/*
  Emulation of network interface. Since we do not have a wire class, 
  we are showing the connection by the pair of IP's
  
  ip: 		Own ip
  connectedTo: 	An address to which above mentioned ip is connected via ethernet.
*/
class NetInterface {
 private:
  string ip;
  string connectedTo; 	//this node is connected to this ip
  int cost; 
  
 public:
  string getip() {
    return this->ip;
  }
  string getConnectedIp() {
    return this->connectedTo;
  }
  void setip(string ip) {
    this->ip = ip;
  }
  void setConnectedip(string ip) {
    this->connectedTo = ip;
  }
  int getcost(){
    return cost; 
  }
  void setcost(int cost1){
    cost=cost1; 
  }
  
};

/*
  Struct of each node
  name: 	It is just a label for a node
  interfaces: 	List of network interfaces a node have
  Node* is part of each interface, it easily allows to send message to another node
  mytbl: 		Node's routing table
*/
class Node {
 private:
  string name;
  
 protected:
  
  vector<pair<NetInterface, Node*> > interfaces;
  
  struct routingtbl mytbl;
  virtual void recvMsg(RouteMsg* msg) {
    cout<<"Base"<<endl;
  }

  bool isMyInterface(string eth) {
    for (int i = 0; i < interfaces.size(); ++i) {
      if(interfaces[i].first.getip() == eth)
	return true;
    }
    return false;
  }
 public:
  int total_nodes; 
  vector<int> Distances;
  vector<int> Visited; 
  int index; 

  string getName(){
    return name; 
  }
  void assign_index(int ind){
       index=ind;
  }
  void setName(string name){
    this->name = name;
  }
  
  void addInterface(string ip, string connip, int cost, Node *nextHop) {
    NetInterface eth;
    eth.setip(ip);
    eth.setConnectedip(connip);
    eth.setcost(cost);
    interfaces.push_back({eth, nextHop});
  }
  
  void addTblEntry(string myip, int cost) {
    RoutingEntry entry;
    entry.dstip = myip;
    entry.nexthop = myip;
    entry.ip_interface = myip;
    entry.cost = cost;
    mytbl.tbl.push_back(entry);
  }
  
  //adds new entry to table
  void addNewEntry(string dest, string nexthop, string myinterface, int cost){
    RoutingEntry entry; 
    entry.dstip=dest; 
    entry.nexthop=nexthop; 
    entry.ip_interface=myinterface; 
    entry.cost=cost; 
    mytbl.tbl.push_back(entry);
  }

  void updateTblEntry(string dstip, int cost) {
    // to update the dstip hop count in the routing table (if dstip already exists)
    // new hop count will be equal to the cost 
    for (int i=0; i<mytbl.tbl.size(); i++){
      RoutingEntry entry = mytbl.tbl[i];

      if (entry.dstip == dstip) 
        mytbl.tbl[i].cost = cost;

    }

    // remove interfaces 
    for(int i=0; i<interfaces.size(); ++i){
      // if the interface ip is matching with dstip then remove
      // the interface from the list
      if (interfaces[i].first.getConnectedIp() == dstip) {
        interfaces.erase(interfaces.begin() + i);
      }
    }
  }
  

  
  struct routingtbl getTable() {
    return mytbl;
  }
  
  //remove any duplicate values in the routing table 
  void printTable() {
    Comparator myobject;
    vector<string> track; 
    
    sort(mytbl.tbl.begin(),mytbl.tbl.end(),myobject);
    cout<<this->getName()<<":"<<endl;
    for (int i = 0; i < mytbl.tbl.size(); ++i) {
    int marker=0;   
    //track.push_back(mytbl.tbl[0].dstip);  
      if(track.size()>0){
        for(int j=0 ; j<track.size()-1; j++){
          if(mytbl.tbl[i].dstip==track[j]){
            marker=1; 
          }
        }
      }
      if(marker==0)
      cout<<mytbl.tbl[i].dstip<<" | "<<mytbl.tbl[i].nexthop<<" | "<<mytbl.tbl[i].ip_interface<<" | "<<mytbl.tbl[i].cost <<endl;
      track.push_back(mytbl.tbl[i].dstip);
      
    }
  }

  
  void sendMsg(){
    struct routingtbl ntbl;
    for (int i = 0; i < mytbl.tbl.size(); ++i) {
      ntbl.tbl.push_back(mytbl.tbl[i]);
    }
    
    for (int i = 0; i < interfaces.size(); ++i) {
      RouteMsg msg;
      msg.from = interfaces[i].first.getip();
      //printf("i=%d, msg-from-interface=%s\n",i, msg.from.c_str());
      msg.mytbl = &ntbl;
      msg.recvip = interfaces[i].first.getConnectedIp();		
      interfaces[i].second->recvMsg(&msg);
    }
  }


  
};

class RoutingNode: public Node {
 public:

  void recvMsg(RoutingNode* node, vector<RoutingNode*> nd);
  void dijikstra(RoutingNode* node, vector<RoutingNode*> nd);
  int find_cost(RoutingNode* s1, RoutingNode* s2);
  pair<string, string> path(vector<RoutingNode*> nd, vector<int> parent_nodes,  RoutingNode* n2);
  pair<string, string> findIps(int index1, int index2, vector<RoutingNode*> nd);
  
};
