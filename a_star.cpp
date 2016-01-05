/**
 *
 *  @name A* Search
 *  @author A B M Ruman
 *
 **/
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <string>
using namespace std;

#define UNVISITED 'w'
#define VISITING 'g'
#define VISITED 'b'

struct Node;
struct Edge;
struct Graph;

struct Node{
	int number;
	string name;
	char state;
	vector<Edge> edges;
	int pathCost;
	Node *parent;
	int heuristic;
};

struct Edge{
	Node *endNode;
	int cost;
};

struct Graph{
	vector<Node> nodes;
}graph;

struct compareNodes{
	bool operator() (Node* a, Node* b){
		if(a->pathCost > b->pathCost)
            return true;
		return false;
	}
};

void refreshFrontier();
void makeSolution(Node*);
bool aStar(Node *, Node *);

vector <Node*> explored;
priority_queue <Node*, vector<Node*>, compareNodes> frontier;
stack<Node*> solution;

int main(){
	string nodeName;
	int nodeNumber, startNode, goalNode;;

	cout << "Number of nodes: ";
	cin >> nodeNumber;
	cout << "Enter node names sequentially: \n";
	getline(cin, nodeName);

    /** Takes inputs for node names and assigns name and serial number **/
	for (int i = 0; i < nodeNumber; i++){
		Node n;

		getline(cin, nodeName);

		n.number = i + 1;
		n.name = nodeName;
		n.state = UNVISITED;
        n.pathCost = 0;
        n.parent = nullptr;
		graph.nodes.push_back(n);
	}

	nodeNumber = graph.nodes.size();
	cout << "ID of the nodes:\n";


    /** Prints the cities with serial number **/
	for (int i = 0; i < nodeNumber; i++){
		cout << i + 1 << ". " << graph.nodes[i].name << "\n";
	}

	cout << "\n\n";

	/** Takes input for neighbor cities **/
	for (int i = 0; i < nodeNumber; i++){
        Node &node = graph.nodes[i];
		int connected;

		cout << i + 1 << ". " << node.name << "\n";
        cout << "\nEnter heuristic values: ";
        cin >> node.heuristic;
        node.pathCost = node.heuristic;

		cout << "Number of connected nodes: ";
		cin >> connected;
		cout << "Enter ID(s) and cost of the node(s):\n";

		/** Takes input for all connected neighbor of current city **/
		for (int j = 0; j < connected; j++){
			int id, cost;

			cin >> id >> cost;

			Edge e = {&graph.nodes[id - 1], cost};
			graph.nodes[i].edges.push_back(e);
		}
        cout << "\n";
	}


	cout << "\n\nGraph: (with Heuristic)\n";

	for (int i = 0; i < nodeNumber; i++){
		Node &node = graph.nodes[i];
		int edges = node.edges.size();

		cout << i + 1 << ". " << node.name << ": [ h(x) = "<< node.heuristic<<" ]\n";

		for (int j = 0; j < edges; j++){
			Edge edge = node.edges[j];

			cout << "\t"<<"(" << edge.endNode->name << ", " << edge.cost << ")\n";
		}

		cout << endl;
	}

	cout << "\nEnter start node no.: ";
    cin >> startNode;
	cout << "\nEnter goal node no.: ";
    cin >> goalNode;

    if(graph.nodes[goalNode-1].heuristic>0){
        cout << "\n\n** heuristic values are not targeted to ("<<graph.nodes[goalNode-1].name<< ") be a goal node.\n\n";
    }
    cout << "\n\nA* search:";

	if(aStar(&graph.nodes[startNode-1], &graph.nodes[goalNode-1])){
        Node *node;
        cout << "\n\nExplored list: " ;

        for (int i = 0; i < explored.size()-1; i++){
            node = explored[i];
            cout << "("<< node->number << ". "<< node->name << ") -> ";
        }
        node = explored[explored.size()-1];
        cout << "("<< node->number << ". "<< node->name << ")";

        cout << "\n\nA* Path: " ;
        if(!solution.empty()){
            while(true){
                node = solution.top();
                solution.pop();
                if(solution.empty())
                    break;
                cout << "(" << node->number << "." << node->name << ") -> ";
            }
            cout <<  "(" << node->number << "." << node->name << ")";
            cout << "\n\nTotal Cost: " << node->pathCost;
        }
	}
	else {
        cout << "Goal not found.";
	}

	return 0;
}

bool aStar(Node *startNode, Node *goalNode) {
    //startNode->pathCost = 0;
    startNode->state = VISITING;
    frontier.push(startNode);

    while(!frontier.empty()){
        Node *node = frontier.top();
        frontier.pop();

        //cout << node->name << " " << node->pathCost <<endl;
        if(node == goalNode){
            makeSolution(node);
            return true;
        }
        node->state = VISITED;
        explored.push_back(node);

        size_t numOfChild = node->edges.size();
        for(int i=0; i<numOfChild; i++){
            Edge edge = node->edges[i];
            Node *child = edge.endNode;
            int pathCost = node->pathCost - node->heuristic + edge.cost + child->heuristic;

            if(child->state == UNVISITED){
                child->pathCost = pathCost;
                child->parent = node;
                child->state = VISITING;
                frontier.push(child);
                //cout << "\t" << child->name << " " << child->pathCost << endl;
            }
            else if(child->state==VISITING && child->pathCost > pathCost){
                //cout << "\t" << child->name << " " << child->pathCost << " " << pathCost <<endl;
                child->pathCost = pathCost;
                child->parent = node;
                refreshFrontier();
            }
        }
    }
    return false;
}

void refreshFrontier(){
    Node *node = frontier.top();
    frontier.pop();
    frontier.push(node);
}
void makeSolution(Node *node){
    while(node != nullptr){
        solution.push(node);
        //cout << node->name << endl;
        //if (node->parent != nullptr)
        if(node->parent == nullptr)
            break;

        node = node->parent;
        //cout << "\t" <<node->name << endl;
    }
}

