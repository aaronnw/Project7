#include <iostream>
#include <list>
using namespace std;

template <class DT>
class ParentBinaryTree {
	template<class T>
	friend ostream& operator<< (ostream& s, ParentBinaryTree<T>& pbt); //Overloaded ostream operator
protected:

	DT* ParentArray; //Array indexed by each position with a value for each position's parent
	int* ChildPositionArray; //Array indexed by each position with a value of 0 for left nodes and 1 for right nodes
	int numNodes; //Number of nodes in the tree
	bool firstRoot = true;

public:
	ParentBinaryTree(); //Default constructor
	ParentBinaryTree(int size); //Initializer
	~ParentBinaryTree(); //Destructor
	ParentBinaryTree(ParentBinaryTree& pbt); //Copy constructor
	void operator=(ParentBinaryTree<DT>& pbt); //Overloaded assignment operator
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
};

///Overloaded ostream operator
template<class T>
ostream& operator<< (ostream& s, ParentBinaryTree<T>& tree) {
	//Print out the preorder traversal
	tree.preorderTraversal();
	s << endl;
	return s;
}

///Default Constructor
template<class DT>
ParentBinaryTree<DT>::ParentBinaryTree() {
	DT* ParentArray = NULL;
	DT* ChildPositionArray = NULL;
	numNodes = 0;
}
///Initializer
template<class DT>
ParentBinaryTree<DT>::ParentBinaryTree(int size) {
	ParentArray = new DT[size];
	ChildPositionArray = new int[size];
	numNodes = size;
}
///Destructor
template<class DT>
ParentBinaryTree<DT>::~ParentBinaryTree() {

}
///Copy constructor
template<class DT>
ParentBinaryTree<DT>::ParentBinaryTree(ParentBinaryTree<DT> & pbt) {
	numNodes = pbt.numNodes;
	ParentArray = new DT[numNodes];
	ChildPositionArray = new int[numNodes];
	//Copy the parent array and the child position array
	for (int i = 0; i < pbt.getSize(); i++) {
		ParentArray[i] = pbt.ParentArray[i];
		ChildPositionArray[i] = pbt.ChildPositionArray[i];
	}

}
///Overloaded assignment operator
template<class DT>
void ParentBinaryTree<DT>::operator=(ParentBinaryTree<DT>& pbt) {
	numNodes = pbt.numNodes;
	ParentArray = new DT[numNodes];
	ChildPositionArray = new int[numNodes];
	//Copy the parent array and the child position array
	for (int i = 0; i < pbt.getSize(); i++) {
		ParentArray[i] = pbt.ParentArray[i];
		ChildPositionArray[i] = pbt.ChildPositionArray[i];
	}
}
//Inserts values into the tree
template<class DT>
void ParentBinaryTree<DT>::insert(DT & root, DT* childArray, int numChildren) {
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
int ParentBinaryTree<DT>::getSize() {
	return numNodes;
}
//Returns the height of the tree
template<class DT>
int ParentBinaryTree<DT>::getHeight() {
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
int ParentBinaryTree<DT>::getNodeHeight(int x) {
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
int* ParentBinaryTree<DT>::getChildren(int parent) {
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
int ParentBinaryTree<DT>::getNumChildren(int parent) {
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
int ParentBinaryTree<DT>::getTopRoot() {
	//Checks all the nodes to see if they have a parent of -1
	int root = 0;
	while (ParentArray[root] != -1 && root < numNodes) {
		root++;
	}
	return root;
}
//Returns a boolean true if the position is a leaf node
template<class DT>
bool ParentBinaryTree<DT>::isLeaf(int x) {
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
void ParentBinaryTree<DT>::preorderTraversal(int x) {
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
void ParentBinaryTree<DT>::preorderTraversal() {
	//Root, left, right
	//Find root node
	int root = getTopRoot();
	//Start the traversal at the root node
	preorderTraversal(root);
}
//Prints the tree, level by level
template<class DT>
void ParentBinaryTree<DT>::levelOrderTraversal() {
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
template	<class	DT>
class	GraphAdjList {

	template<class T>
	friend ostream& operator<< (ostream& s, GraphAdjList<T>& l); //Overloaded ostream operator
protected:
	list<DT>* adjList;
	int	n;	//	Number	of	nodes
	int m; //	Number	of	edges
public:
	GraphAdjList();
	GraphAdjList(int n, int m);
	~GraphAdjList();
	void addEdge(int x, int y);
	ParentBinaryTree<DT> dfs(int x);
	ParentBinaryTree<DT> bfs(int x);

};
template<class DT>
GraphAdjList<DT>::GraphAdjList() {
	n = 0;
	m = 0;
	adjList = new list<DT>[n];
}

template<class DT>
GraphAdjList<DT>::GraphAdjList(int numNodes, int numEdges) {
	n = numNodes;
	m = numEdges;
	adjList = new list<DT>[n];
}

template<class DT>
GraphAdjList<DT>::~GraphAdjList() {
}

template<class DT>
void GraphAdjList<DT>::addEdge(int x, int y) {
	adjList[x].push_back(y);
	adjList[y].push_back(x);
}

template<class DT>
ParentBinaryTree<DT> GraphAdjList<DT>::dfs(int x) {
	/*
	for(int i = 0; i < n ; i ++}{
     visited[i] = false;     
     parent[i] = -1;
}
     _DFS(x);
     return parentArray;
}

void _DFS(int x){
visited[x] = True;
for each neighbor y of x{
if !visited[y]{
     parent[y] = x;
     _DFS(y);
}
}

	*/
	return ParentBinaryTree<DT>();
}

template<class DT>
ParentBinaryTree<DT> GraphAdjList<DT>::bfs(int x) {
	/*
	for(int i = 0; i < n; i ++){
visited[i] = false;
parent[i] = -1;
}
visited[x] = true;
q.push(x);
while(!q.empty()){
x = q.front();
q.pop();
for each y neighbor of x{
visited[y] = true;
q.push(y);
parent[y] = x;
}
}

	*/

	return ParentBinaryTree<DT>();
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
	cout << *graphAdjList;
	return 0;
}

