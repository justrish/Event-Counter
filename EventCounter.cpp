
#include <iostream>
#include <cstdio>
#include <cstring>
#ifdef LINUX
#include <sys/time.h>
#endif
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;
#define RED 'R'
#define BLACK 'B'

#ifdef LINUX
double timerval() {
	struct timeval st;
	gettimeofday(&st, NULL);
	return (st.tv_sec + st.tv_usec*1e-6);
}
#endif

//Creating the RBTree Node
struct node
{
    char color;
    int id;
    int count;
    node *left, *right, *parent;
};

//Class EventCounter Declaration which uses RedBlackTree
class EventCounter
{
private:
    node* root;
    //Method to compare two ids
    int compare(int left, int right)
    {
        if (left < right) return -1;
        else if (left > right) return 1;
        else return 0;
    }
    // Method to get level of the nodes to change to red.
    int computeRedLevel(int size)
    {
        int height = 0;
        for (int i = size - 1; i >= 0; i = i /2 - 1)
            height++;
        return height;
    }
    // Method to construct a red black tree using a sorted list of nodes.
    node* buildFromSorted(int level, int lo, int hi, int redLevel, vector<pair<int, int> > &idCountPairs, int &currentIndex)
    {
        //Recursion terminate condition when hi becomes less than low
        if (hi < lo) return NULL;
        //Computing the index of the middle node*
        int mid = (lo + hi) / 2;

        node* left = NULL;
        node* right = NULL;
        //Constructing the left subtree at each level
        if (lo < mid)
        {
            left = buildFromSorted(level + 1, lo, mid - 1, redLevel,
                idCountPairs, currentIndex);
        }

        int id = idCountPairs[currentIndex].first;
        int count = idCountPairs[currentIndex].second;
        currentIndex++;
        char nodeColor;
        // color nodes in non-full bottommost level Red
        if (level == redLevel)
            nodeColor = RED;
        else 
            nodeColor = BLACK;
        //This ensures all the red black tree property satisfied Black node* balanced, No consecutive Red nodes and Root is a black node*.
        //Constructing the right subtree at each level
        if (mid < hi) {
            right = buildFromSorted(level + 1, mid + 1, hi, redLevel,
                idCountPairs, currentIndex);
        }
        //Constructing the parent at each level and then returning it .
        node* middle = newNode(id, count, nodeColor, left, right);
        return middle;
    }
    node* grandparent(node* n);
    node* sibling(node* n);
    node* uncle(node* n);
    char nodeColor(node* n);
    node* newNode(int id, int, char color, node*, node*);
    node* maxNode(node* root);
    void replaceNode(node* old, node* cur);
    node* search(node* cur, int id);
    node* next(node* cur, int id);
    node* previous(node* cur, int id);
    long long int inrange(node* cur, int k1, int k2, long long int sum);
    void rotateLeft(node* cur);
    void rotateRight(node* cur);
    void insertFixup(node* n);
    void deleteFixup(node* n);
    void verifyProperties(node*);
public:
    //Constructor to initialize the Event Counter from sorted IDs 
    EventCounter(vector<pair<int, int> > &idCountPairs) {
        int currentIndex = 0;
        root = buildFromSorted(0, 0, (int)(idCountPairs.size()) - 1,
            computeRedLevel((int)idCountPairs.size()), idCountPairs, currentIndex);
    }

    EventCounter() {}
    int insert(int, int);
    void remove(int);
    node* search(int);
    node* next(int);
    node* previous(int);
    long long int inrange(int, int);

};

//Return Grandparent of Node
node* EventCounter::grandparent(node* n)
{
    return n->parent->parent;
}


// Return Sibling of Node

node* EventCounter::sibling(node* n)
{
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}


// Return Uncle of Node
node* EventCounter::uncle(node* n)
{
    return sibling(n->parent);
}


// Verifying Properties of Red black Tree
void EventCounter::verifyProperties(node* root)
{
    //Property1    Color of all nodes are red or black
    //Property2    Root is black Node
    //Property3    No consecutive Nodes are Red
    //Property4    To check the number of black nodes from root to external nodes.
	//Property5    If all the four are satisfied then the tree is a BBST
}

// Returns color of a node
char EventCounter::nodeColor(node* n)
{
    return (n != NULL) ? n->color :BLACK;
}


//Create a New Node to Insert a RedBlack Tree
node* EventCounter::newNode(int k, int v, char color, node* left, node* right)
{
    //alloc a pointer of node type
    node* cur = new node;
	cur->color = color;
    cur->id = k;
    cur->count = v;
    cur->left = left;
    cur->right = right;
	cur->parent = NULL;
    if (left != NULL)
        left->parent = cur;
    if (right != NULL)
        right->parent = cur;
    
    return cur;
}
//Search the Node with a particular ID
node* EventCounter::search(node* cur,int id)
{
    if (cur == NULL) return NULL;
    int r = compare(id, cur->id);
    if (r == 0)
        return cur;
    else if (r < 0)
        cur = search(cur->left, id);
    else if (r > 0)
        cur = search(cur->right, id);
    return cur;
}
//Return Count value of the Node with a particular ID
node* EventCounter::search(int id)
{
	// Calling Recursice in order search function 
    node* cur = search(root, id);
    return cur;
}
//Left Rotate wrt to current node
void EventCounter::rotateLeft(node* cur)
{
    node* r = cur->right;
	//Replacing Current Node by its Right Node
    replaceNode(cur, r);
	//Updating the right child of the replaced node by the left child of the new node
    cur->right = r->left;
    //Update the parent of new node left child as the replaced node
    if (r->left != NULL)
        r->left->parent = cur;
    // New node left child with the replaced node
    r->left = cur;
	//Making New node the parent of the replaced node
    cur->parent = r;
}
//Right Rotate wrt to current node
void EventCounter::rotateRight(node* cur)
{
    node* l = cur->left;
	//Replacing Current Node by its Left Node
    replaceNode(cur, l);
	//Updating the Left child of the replaced node by the right child of the new node
    cur->left = l->right;
	//Update the parent of new node right child as the replaced node
    if (l->right != NULL)
        l->right->parent = cur;
	// New node right child with the replaced node
    l->right = cur;
	//Making New node the parent of the replaced node
    cur->parent = l;
}
//Replace the old node with a new node 
void EventCounter::replaceNode(node* old, node* cur)
{
	//Updating if the node is root
    if (old->parent == NULL)
        root = cur;
    else
    { // Checking whether the old node was right or left child and updating it.
        if (old == old->parent->left)
            old->parent->left = cur;
        else
            old->parent->right = cur;
    }
	//Updating the parent of the new node.
    if (cur != NULL)
        cur->parent = old->parent;
}

// Insert node into EventCounter

int EventCounter::insert(int id, int count)
{
    node* insertedNode = newNode(id, count, RED, NULL, NULL);

    if (root == NULL)
       root = insertedNode;
    else
    {
        node* n = root;
        while (1)
        {
            int compResult = compare(id, n->id);
            if (compResult == 0)
            {//update the count vlaue of the node
                n->count += count;
                return n->count;
            }
            else if (compResult < 0)
            {// location to insert found then  break the while loop 
                if (n->left == NULL)
                {
                    n->left = insertedNode;
                    break;
                }
                else
                    n = n->left;
            }
            else
            {
                if (n->right == NULL)
                {
                    n->right = insertedNode;
                    break;
                }
                else
                    n = n->right;
            }
        }
        insertedNode->parent = n;
    }
   //called to satisfy the properties of Red black tree to be balanced binary searchtree
    insertFixup(insertedNode);
    verifyProperties(root);
    return count;
}


//Insert Fix Up 
void EventCounter::insertFixup(node* n)
{
    // The node is the root node, i.e., first node of red–black tree
    if (n->parent == NULL) {
        n->color = BLACK;
    }
    else
    {   //The node’s parent is black.
        if (nodeColor(n->parent) == BLACK) {
            // tree is valid 
            return;
        }
        else
        {  // The node parent and uncle are red
            if (nodeColor(uncle(n)) == RED)
            {
                n->parent->color = BLACK;
                uncle(n)->color = BLACK;
                grandparent(n)->color = RED;
                // recursively do insert fixup for the node's grand-parent
                insertFixup(grandparent(n));
            }
            else
            {
                // The node is added to right of left child of grandparent,
                // or the node is added to left of right child of grandparent (parent is red and uncle is black)
                if (n == n->parent->right && n->parent == grandparent(n)->left)
                {
                    rotateLeft(n->parent);
                    n = n->left;
                }
                else if (n == n->parent->left && n->parent == grandparent(n)->right)
                {
                    rotateRight(n->parent);
                    n = n->right;
                }
                // The node is added to left of left child of grandparent,
                // or the node is added to right of right child of grandparent (parent is red and uncle is black)
                n->parent->color = BLACK;
                grandparent(n)->color = RED;
                if (n == n->parent->left && n->parent == grandparent(n)->left)
                {
                    rotateRight(grandparent(n));
                }
                else
                {
                    rotateLeft(grandparent(n));
                }
            }
        }
    }
}

// Delete Node from EventCounter
void EventCounter::remove(int id)
{
    node* child;
    node* n = search(id);
    if (n == NULL)
        return;
	//the node has both the left and right subtree and replaced by the maximum node in its left subtree
    if (n->left != NULL && n->right != NULL)
    {
        node* pred = maxNode(n->left);
        n->id = pred->id;
        n->count = pred->count;
        n = pred;
    }
	// Update the child pointer which replaces the node deleted
    child = n->right == NULL ? n->left : n->right;
	// check if node is black and do the fix up as number of black noes changes
    if (nodeColor(n) == BLACK)
    {
        n->color = nodeColor(child);
        deleteFixup(n);
    }
	//Replace and delete the node.
    replaceNode(n, child);
    delete n;
    verifyProperties(root);
}

//Returns Maximum node
node* EventCounter::maxNode(node* cur)
{
    if (cur == NULL) return NULL;
    if (cur != NULL)
        maxNode(cur->right);
    return cur;
}


//Delete Fix up
void EventCounter::deleteFixup(node* n)
{
    // Case 1 if Node is root Terminating conditon
    if (n->parent == NULL)
        return;
    else
    {   // delete case 2
        if (nodeColor(sibling(n)) == RED)
        {
            n->parent->color = RED;
            sibling(n)->color = BLACK;
            if (n == n->parent->left)
                rotateLeft(n->parent);
            else
                rotateRight(n->parent);
        }
        //Parent is BLACK, sibling is black and both children of sibling are black.
        if (nodeColor(n->parent) == BLACK && nodeColor(sibling(n)) == BLACK &&
            nodeColor(sibling(n)->left) == BLACK && nodeColor(sibling(n)->right) == BLACK)
        {//As number of black node changes call deletefix up for the parent
            sibling(n)->color = RED;
			deleteFixup(n->parent);
        }
        else
        {   //Parent is RED, sibling is black and both children of sibling are black.
            if (nodeColor(n->parent) == RED && nodeColor(sibling(n)) == BLACK &&
                nodeColor(sibling(n)->left) == BLACK && nodeColor(sibling(n)->right) == BLACK)
            {//Just change the sibling color to red and parent to node balances the black node in the tree
                sibling(n)->color = RED;
                n->parent->color = BLACK;
            }
            else
            {   //Node is the left child of parent, sibling is black, left child of sibling is RED, 
				//and right child of sibling is black
                if (n == n->parent->left && nodeColor(sibling(n)) == BLACK &&
                    nodeColor(sibling(n)->left) == RED && nodeColor(sibling(n)->right) == BLACK)
                {
                    sibling(n)->color = RED;
                    sibling(n)->left->color = BLACK;
                    rotateRight(sibling(n));
                }
                else if (n == n->parent->right && nodeColor(sibling(n)) == BLACK &&
                    nodeColor(sibling(n)->right) == RED && nodeColor(sibling(n)->left) == BLACK)
                {
                    sibling(n)->color = RED;
                    sibling(n)->right->color = BLACK;
                    rotateLeft(sibling(n));
                }
                //Node is the right child of parent, sibling is black, right child of sibling is RED, 
				//and left child of sibling is black
                sibling(n)->color = nodeColor(n->parent);
                n->parent->color = BLACK;
                if (n == n->parent->left)
                {
                    sibling(n)->right->color = BLACK;
                    rotateLeft(n->parent);
                }
                else
                {
                    sibling(n)->left->color = BLACK;
                    rotateRight(n->parent);
                }
            }
        }
    }
}

node* EventCounter::next(node* cur, int id) {
    // Recursion terminating condition
	if (cur == NULL)
        return NULL;

    int r = compare(id, cur->id);
    if (r < 0)
    { 
        //If the ID searched is lesser than current node so go to left
        if (cur->left != NULL)
            cur = next(cur->left, id);
        else
            return cur;
    }
    else
    {   //If the ID searched is greater than current node so go to right
        if (cur->right != NULL)
            cur = next(cur->right, id);
        else
        { //if right node is NULL then it traces back and stops at node where current is actually its left child or its the root
            node* parent = cur->parent;
            node* child = cur;
            while (parent != NULL && child == parent->right)
			{     
                child = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }
    return cur;
}
node* EventCounter::next(int id)
{
    node* n = next(root, id);
    return n;
}

node* EventCounter::previous(node* cur, int id) {
    if (cur == NULL)
        return NULL;

    int r = compare(id, cur->id);
    if (r > 0)
    {   //If the ID searched is greater than current node so go to right
        if (cur->right != NULL)
            cur = previous(cur->right, id);
        else
            return cur;
    }
    else
    {   //If the ID searched is lesser than current node so go to left
        if (cur->left != NULL)
            cur = previous(cur->left, id);
        else
        {//if left node is NULL then it traces back and stops at node where current is actually its right child or its the root
            node* parent = cur->parent;
            node* child = cur;
            while (parent != NULL && child == parent->left)
            {
                child = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }
    return cur;
}
node* EventCounter::previous(int id)
{
    node* n = previous(root, id);
    return n;
}

long long int EventCounter::inrange(node* cur, int k1, int k2, long long int sum)
{
    if (NULL == cur)
        return 0;
	// compare the in range IDs with current node 
    int r1 = compare(k1, cur->id);
    int r2 = compare(k2, cur->id);
	//if the a node find greater than ID1 call recursively return the count sum
    if (r1 < 0) 
        sum += inrange(cur->left, k1, k2, 0);
	//if the a node ID is between than ID1 and ID2 then add the count and search greater ID
    if (r1 <= 0 && r2 >= 0) 
        sum += cur->count;
	//if the a node ID is lesser than ID2 call recursively return the count sum
    if (r2 > 0) 
        sum += inrange(cur->right, k1, k2, 0);
    return sum;
}
long long int EventCounter::inrange(int k1, int k2)
{
    //In Range Recursive function call with root node*.
    long long int sum = inrange(root, k1, k2, 0);
    return sum;
}


int main(int argc, char *argv[])
{
    // Initialize
    ifstream input;
    input.open(argv[1]);
	if (!input.good())
		exit(0);

    int n;
    input >> n;
    vector<pair<int, int> > idCountPairs(n);
    /// File read for Input ID and Counters stored in a vector<pair>
#ifdef LINUX
	double startTime = 0;
	double endTime = 0;
	startTime = timerval();
#endif
    for (int i = 0; i < n; i++)
    {
        int id;
        int count;
        input >> id >> count;
        idCountPairs[i] = make_pair(id, count);
    }
    input.close();
    /// File closed for Input ID and Counters 
    // Eventcounter creates redBlack tree from the idcountPairs using the sorted ID list 
    // The constructor initialises the Nodes.
    EventCounter *rbt = new EventCounter(idCountPairs);
    //Vectors created to store the file inputs initially freed
    idCountPairs.erase(idCountPairs.begin(), idCountPairs.end());
    idCountPairs.clear();
    string command;
    //command inputs
    while (1)
    {
        string cmd;
        string arg;
        getline(cin, command);
		//program exits if quit command given
        if (command.find("quit") == 0)
            break;
		//command and arguments of the command soearated.
        cmd = command.substr(0, command.find(' '));
        arg = command.substr(command.find(' '), command.length() - command.find(' '));

        stringstream line(arg);
        if (cmd.compare("increase") == 0)
        {
            int id, m;
            line >> id >> m;
			//insert function will update the node when found else insert
            cout << rbt->insert(id, m) << endl;
        }
        else if (cmd.compare("reduce") == 0)
        {
			//search the tree if node found then decrement the value.
			// Which if happens to make count less than or equal to zero.
			//Thus deleting the node wanted to reduce.
            int id, m;
            line >> id >> m;
            node* n = rbt->search(id);
            if (n != NULL)
            {
                n->count = n->count - m;
                if (n->count <= 0) {
                    rbt->remove(id);
                    cout << "0\n";
                }
                else {
                    cout << n->count << "\n";
                }
            }
            else
            {
                cout << "0\n";
            }

        }
        else if (cmd.compare("count") == 0)
        {
            int id;
            line >> id;
            node* n1 = rbt->search(id);
			// print the count value of the searched ID
            cout << (n1 == NULL ? 0 : n1->count) << "\n";
        }
        else if (cmd.compare("inrange") == 0)
        {
            int id1, id2;
            line >> id1 >> id2;
			//Take the ID1 and ID2 and find the summation of all the nodes between them
            cout << rbt->inrange(id1, id2) << "\n";
        }
        else if (cmd.compare("next") == 0)
        {
            int id, m;
            line >> id;
			// Get the next higher ID node from the tree
            node* next = rbt->next(id);
            if (next != NULL)
                cout << next->id << ' ' << next->count << '\n';
            else
                cout << "0 0\n";
        }
        else if (cmd.compare("previous") == 0)
        {
            int id, m;
            line >> id;
			// Get the just lower ID node from the tree
            node* previous = rbt->previous(id);
            if (previous != NULL)
                cout << previous->id << ' ' << previous->count << '\n';
            else
                cout << "0 0\n";
        }
    }
#ifdef LINUX
	endTime = timerval();
	printf(" \nElapsed time in seconds: %.8f\n",(endTime - startTime));
#endif
    return 0;
}
