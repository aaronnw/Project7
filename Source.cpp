#include <iostream>
#include <list>
#include <queue>

using namespace std;

template <class DT>
class ParentMultiTree {
	template<class T>
	friend ostream& operator<< (ostream& s, ParentMultiTree<T>& pmt); //Overloaded ostream operator
protected:

	DT* ParentArray; //Array indexed by each position with a value for each position's parent
	int* ChildPositionArray; //Array indexed by each position with a value of 0 for left nodes and 1 for right nodes
	int numNodes; //Number of nodes in the tree
	bool firstRoot = true;

public:
	ParentMultiTree(); //Default constructor
	ParentMultiTree(int size); //Initializer
	~ParentMultiTree(); //Destructor
	ParentMultiTree(ParentMultiTree& pmt); //Copy constructor
	void operator=(ParentMultiTree<DT>& pmt); //Overloaded assignment operator
	void insert(DT & root, DT* childArray, int numChildren); //Inserts the values to create the tree
	int getSize(); //Returns the number of nodes in the tree
	int getHeight(); //Returns the height of the tree
	int getNodeHeight(int x); //Returns the height of a single node
	int* getChildren(int parent); //Returns an array of all the children of a node
	int getNumChildren(int parent); //Returns the number of children at a node
	int getTopRoot();
	bool isLeaf(int x);  //Checks if the position given is a leaf node
	void preorderTraversal(); //Traverses the tree by the preorder method, using less-than-root values as 'left' and greater-than-root values as 'right'
	void preorderTraversal(int x); //Traveres by preorder, with the root included as a parameter 
	void levelOrderTraversal(); //Traverses the tree by level
	void createFromParentArray(int* parentArray, int n); //Create a tree from only a parent array and length, putting smaller values as left children
	void printArrays(); //Print the parent array and child position arrays
};

///Overloaded ostream operator
template<class T>
ostream& operator<< (ostream& s, ParentMultiTree<T>& tree) {
	//Print out the preorder traversal
	tree.preorderTraversal();
	s << endl;
	return s;
}

///Default Constructor
template<class DT>
ParentMultiTree<DT>::ParentMultiTree() {
	DT* ParentArray = NULL;
	DT* ChildPositionArray = NULL;
	numNodes = 0;
}
///Initializer
template<class DT>
ParentMultiTree<DT>::ParentMultiTree(int size) {
	ParentArray = new DT[size];
	ChildPositionArray = new int[size];
	numNodes = size;
}
///Destructor
template<class DT>
ParentMultiTree<DT>::~ParentMultiTree() {

}
///Copy constructor
template<class DT>
ParentMultiTree<DT>::ParentMultiTree(ParentMultiTree<DT> & pmt) {
	numNodes = pmt.numNodes;
	ParentArray = new DT[numNodes];
	ChildPositionArray = new int[numNodes];
	//Copy the parent array and the child position array
	for (int i = 0; i < pmt.getSize(); i++) {
		ParentArray[i] = pmt.ParentArray[i];
		ChildPositionArray[i] = pmt.ChildPositionArray[i];
	}

}
///Overloaded assignment operator
template<class DT>
void ParentMultiTree<DT>::operator=(ParentMultiTree<DT>& pmt) {
	numNodes = pmt.numNodes;
	ParentArray = new DT[numNodes];
	ChildPositionArray = new int[numNodes];
	//Copy the parent array and the child position array
	for (int i = 0; i < pmt.getSize(); i++) {
		ParentArray[i] = pmt.ParentArray[i];
		ChildPositionArray[i] = pmt.ChildPositionArray[i];
	}
}
//Inserts values into the tree
template<class DT>
void ParentMultiTree<DT>::insert(DT & root, DT* childArray, int numChildren) {
	//Only for the highest level node
	if (firstRoot) {
		ParentArray[root] = -1;
		ChildPositionArray[root] = -1;
		firstRoot = false;
	}
	//Sets the values for each array
	for (int i = 0; i < numChildren; i++) {
		ParentArray[childArray[i]] = root;
		ChildPositionArray[childArray[i]] = i;
	};
}
//Returns number of nodes in the tree
template<class DT>
int ParentMultiTree<DT>::getSize() {
	return numNodes;
}
//Returns the height of the tree
template<class DT>
int ParentMultiTree<DT>::getHeight() {
	int maxHeight = 1;
	int height = 1;
	//Loop through all the nodes
	for (int i = 0; i < numNodes; i++) {
		//Only look at leaf nodes to find the max height
		if (isLeaf(i)) {
			//Get the height of the node
			height = getNodeHeight(i);
			//Keep only the greatest height found
			if (height > maxHeight) {
				maxHeight = height;
			}
			height = 0;
		}
	}
	return maxHeight;
}
//Returns the height of a single node
template<class DT>
int ParentMultiTree<DT>::getNodeHeight(int x) {
	int height = 1;
	int k = x;
	//Bubble up to the parent, counting levels
	while (ParentArray[k] != -1) {
		k = ParentArray[k];
		height++;
	}
	return height;
}
//Returns an array of all of the children of a node
template<class DT>
int* ParentMultiTree<DT>::getChildren(int parent) {
	//If the node has no children return a special value
	if (!isLeaf(parent)) {
		int numChildren = getNumChildren(parent);
		int* children = new int[numChildren];
		int j = 0;
		//Iterate through all the nodes, adding all the children to an array
		for (int i = 0; i < numNodes; i++) {
			if (ParentArray[i] == parent) {
				children[j] = i;
				j++;
			}
		}
		return children;
	}
	else {
		int* children = new int[1];
		children[0] = -1;
		return children;
	}
}
//Returns the number of children of a node
template<class DT>
int ParentMultiTree<DT>::getNumChildren(int parent) {
	int count = 0;
	//Iterate through all the nodes and count the children of the given parent
	for (int i = 0; i < numNodes; i++) {
		if (ParentArray[i] == parent) {
			count++;
		}
	}
	return count;
}
//Returns the top root of the tree
template<class DT>
int ParentMultiTree<DT>::getTopRoot() {
	//Checks all the nodes to see if they have a parent of -1
	int root = 0;
	while (ParentArray[root] != -1 && root < numNodes) {
		root++;
	}
	return root;
}
//Returns a boolean true if the position is a leaf node
template<class DT>
bool ParentMultiTree<DT>::isLeaf(int x) {
	//Check if the position has children
	if (getNumChildren(x) == 0) {
		return true;
	}
	else {
		return false;
	}
}
//Prints the node values in preorder format
template<class DT>
void ParentMultiTree<DT>::preorderTraversal(int x) {
	//Root, left, right
	cout << x << " ";
	int* children = getChildren(x);
	//Classify left nodes as those with less value than the parent and print the left
	for (int i = 0; i < getNumChildren(x); i++) {
		if (children[i] <= x) {
			preorderTraversal(children[i]);
		}
	}
	//Classify right nodes as those with greater value than the parent and print the right
	for (int i = 0; i < getNumChildren(x); i++) {
		if (children[i] > x) {
			preorderTraversal(children[i]);
		}
	}
}
//Finds the root node then prints the node values in preorder format
template<class DT>
void ParentMultiTree<DT>::preorderTraversal() {
	//Root, left, right
	//Find root node
	int root = getTopRoot();
	//Start the traversal at the root node
	preorderTraversal(root);
}
//Prints the tree, level by level
template<class DT>
void ParentMultiTree<DT>::levelOrderTraversal() {
	queue<int> q;
	int numLevels = getHeight();
	int x = 0;
	//For each level
	for (int level = 1; level <= numLevels; level++) {
		//Check all of the nodes for their height and add them to the queue
		for (int j = 0; j < numNodes; j++) {
			if (getNodeHeight(j) == level) {
				q.push(j);
			}
		}
		cout << level << " ";
		//As long as there are values in the queue, print them out
		while (!q.empty()) {
			x = q.front();
			q.pop();
			cout << x << " ";
		}
		cout << endl;
	}
}
//Create a tree from only a parent array and length, putting smaller values as left children
template<class DT>
void ParentMultiTree<DT>::createFromParentArray(int* inputArray, int n) {
	numNodes = n;
	ParentArray = new DT[n];
	ChildPositionArray = new DT[n];
	//Initialize all the child positions to -1
	for (int i = 0; i < n; i++) {
		ChildPositionArray[i] = -1;
	}
	//Copy the input array to the parent array
	for (int i = 0; i < n; i++) {
		ParentArray[i] = inputArray[i];
		//for each child of a node i
		//add them at the index
		//increment the index
	}
	for (int j = 0; j < n; j++) {
		//If the child position hasn't been set yet
		if (ChildPositionArray[j] == -1) {
			int parent = ParentArray[j];
			int* children = getChildren(parent);
			int numChildren = getNumChildren(parent);
			for (int k = 0; k < numChildren; k++) {
				ChildPositionArray[children[k]] = k;
			}
		}
	}
}
//Print the parent array and child position arrays
template<class DT>
void ParentMultiTree<DT>::printArrays() {
	//Print the parent array
	for (int i = 0; i < numNodes; i++) {
		cout << ParentArray[i] << ' ';
	}
	cout << endl;
	//Print the child position array
	for (int i = 0; i < numNodes; i++) {
		cout << ChildPositionArray[i] << ' ';
	}
	cout << endl;

}

////////////////////////////////////////////////////////////////////////////////////////////////
template<class DT>
class GraphAdjList {

	template<class T>
	friend ostream& operator<< (ostream& s, GraphAdjList<T>& l); //Overloaded ostream operator

protected:
	list<DT>* adjList;
	int	n; //Number of nodes
	int m; //Number of edges
	int* visited; //Array storing which nodes have been visited
	int* parent; //Array storing the parents of each node
public:
	GraphAdjList(); //Default constructor
	GraphAdjList(int n, int m); //Initializer
	~GraphAdjList(); //Destructor
	void addEdge(int x, int y); //Adds an edge the the graph adjacency list
	ParentMultiTree<DT> dfs(int x); //Returns a tree from the depth-first search starting at the given value
	void _dfs(int x); //Recursive method to set the array values
	ParentMultiTree<DT> bfs(int x); //Returns a tree from the breadth-first search starting at the given value

};
///Default constructor
template<class DT>
GraphAdjList<DT>::GraphAdjList() {
	n = 0;
	m = 0;
	visited = nullptr;
	parent = nullptr;
	adjList = new list<DT>[n];
}
///Initializer
template<class DT>
GraphAdjList<DT>::GraphAdjList(int numNodes, int numEdges) {
	n = numNodes;
	m = numEdges;
	visited = new int[numNodes];
	parent = new int[numNodes];
	adjList = new list<DT>[n];
}
///Destructor
template<class DT>
GraphAdjList<DT>::~GraphAdjList() {
}
//Adds an edge the the graph adjacency list
template<class DT>
void GraphAdjList<DT>::addEdge(int x, int y) {
	adjList[x].push_back(y);
	adjList[y].push_back(x);
}
//Returns a tree from the depth-first search starting at the given value
template<class DT>
ParentMultiTree<DT> GraphAdjList<DT>::dfs(int x) {
	//Initialize the array values 
	for(int i = 0; i < n ; i ++){
		visited[i] = 0;
		parent[i] = -1;
	}
	//Call the recursive function
	_dfs(x);
	//Use the parent array to create a parentmultiarray object from x
	ParentMultiTree<DT>* dfsPMT = new ParentMultiTree<DT>();
	dfsPMT->createFromParentArray(parent, n);
	return *dfsPMT;
}
//Recursive method to set the array values
template<class DT>
void GraphAdjList<DT>::_dfs(int x){
	visited[x] = true;
	//For each neighbor y of x
	for (list<int>::const_iterator iterator = adjList[x].begin(), end = adjList[x].end(); iterator != end; ++iterator) {
		//If that neighbor has not been visited yet
		if (visited[*iterator] == 0){
			//Make the parent x and the child y
			parent[*iterator] = x;
			//Call DFS on that neighbor
			_dfs(*iterator);
		}
	}
}
//Returns a tree from the breadth-first search starting at the given value
template<class DT>
ParentMultiTree<DT> GraphAdjList<DT>::bfs(int x) {
	queue<int>* q = new queue<int>();
	//Initialize the arrays
	for(int i = 0; i < n; i ++){
		visited[i] = 0;
		parent[i] = -1;
	}
	//Mark the first array as visited and add it to the queue
	visited[x] = 1;
	q->push(x);
	//Loop while the queue is not empty
	while(!q->empty()){
		x = q->front();
		q->pop();
		//For each neighbor of x
		for (list<int>::const_iterator iterator = adjList[x].begin(), end = adjList[x].end(); iterator != end; ++iterator){
			//Only visit unvisited nodes
			if (visited[*iterator] == 0) {
				//Mark as visited and add to the queue
				visited[*iterator] = 1;
				q->push(*iterator);
				parent[*iterator] = x;
			}
		}
	}
	//Create a parentmultitree object from the parent array
	ParentMultiTree<DT>* bfsPMT = new ParentMultiTree<DT>();
	bfsPMT->createFromParentArray(parent, n);
	return *bfsPMT;
}
///Overloaded ostream operator
template<class T>
ostream& operator<< (ostream& s, GraphAdjList<T>& l) {
	//Print out the adjacency list
	//For each node
	for (int i = 0; i < l.n; i++){
		s << i << " ";
		//Print each edge
		for (int j = 0; j < l.adjList[i].size(); j++) {
			auto l_front = l.adjList[i].begin();
			advance(l_front, j);
			s << *l_front << " ";
		}
		s << endl;
	}
	return s;
}

int main() {
	int n = -1;
	int m = -1;
	int x = -1;
	int y = -1;
	int line = 0;
	cin >> n >> m;
	//Create an array of lists
	GraphAdjList<int>* graphAdjList = new GraphAdjList<int>(n, m);
	//For each edge add the connections
	while (line < m) {
		cin >> x >> y;
		graphAdjList->addEdge(x, y);
		line++;
	}
	cout << "Each node followed by the nodes it connects to: " << endl;
	//Call the overloaded ostream operator to print the adjacency list representation
	cout << *graphAdjList << endl;
	int search = 0;
	//Prints the tree created by the dfs from the search integer to show that dfs works
	cout << "Preorder traversal of DFS tree from " << search << endl;
	ParentMultiTree<int> pmtDFS = graphAdjList->dfs(search);
	cout << pmtDFS << endl;
	//Prints the level order traversal of the DFS tree
	cout << "Level-order traversal of DFS tree from " << search << endl;
	pmtDFS.levelOrderTraversal();
	cout << endl;
	//Prints the tree created by the bfs from the search integer to show that bfs works
	cout << "Preorder traversal of BFS tree from " << search << endl;
	ParentMultiTree<int> pmtBFS = graphAdjList->bfs(search);
	cout << pmtBFS << endl;
	//Prints the level order traversal of the BFS tree
	cout << "Level-order traversal of BFS tree from " << search << endl;
	pmtBFS.levelOrderTraversal();
	cout << endl;
	return 0;
}
