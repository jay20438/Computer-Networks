#include "node.h"

vector<RoutingNode*> distanceVectorNodes;

void routingAlgo(vector<RoutingNode*> distanceVectorNodes);
//void routingAlgo2(vector<RoutingNode*> distanceVectorNodes);

int main() {
    int n; // number of nodes
    cin>>n;
    string name; //Node label
    distanceVectorNodes.clear();
    for (int i = 0 ; i < n; i++) {
      RoutingNode *newnode = new RoutingNode();
      cin>>name;
      newnode->setName(name);
      distanceVectorNodes.push_back(newnode);
    }
    cin>>name;
    /*
      For each node label(@name), it's own ip address, ip address of another node
      defined by @oname will be inserted in the node's own datastructure interfaces 
    */
    while(name != "EOE") { //End of entries
      for (int i =0 ; i < distanceVectorNodes.size(); i++) {
	string myeth,oeth, oname;
	int cost; 
	if(distanceVectorNodes[i]->getName() == name) {
	  //node interface ip
	  cin>>myeth; 	
	  //ip of another node connected to myeth (nd[i])
	  cin>>oeth;
	  //label of the node whose ip is oeth
	  cin>>oname;
    
	  cin>>cost; //remove this for Q1
	  
	  distanceVectorNodes[i]->total_nodes=distanceVectorNodes.size(); 
	  for(int j = 0 ; j < distanceVectorNodes.size(); j++) {
	    if(distanceVectorNodes[j]->getName() == oname) {
	      /*
		@myeth: ip address of my (distanceVectorNodes[i]) end of connection.
		@oeth: ip address of other end of connection.
		@distanceVectorNodes[j]: pointer to the node whose one of the interface is @oeth
	      */
		  
	      distanceVectorNodes[i]->addInterface(myeth, oeth, cost, distanceVectorNodes[j]);  //modify this for q1
	      //Routing table initialization
	      /*
		@myeth: ip address of my (distanceVectorNodes[i]) ethernet interface.
		@0: hop count, 0 as node does not need any other hop to pass packet to itself.
		
	      */
	      distanceVectorNodes[i]->addTblEntry(myeth, 0);
	      break;
	    }
	  }
	}
      }
      cin>>name;
    }

    /* The logic of the routing algorithm should go here */
    routingAlgo(distanceVectorNodes);
    /* Add the logic for periodic update (after every 1 sec) here */
    // routingAlgo(distanceVectorNodes);
    
}

