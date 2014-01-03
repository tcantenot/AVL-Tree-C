#ifndef AVL_TREE
#define AVL_TREE

typedef struct AvlNode AvlNode;
typedef struct AvlTree AvlTree;

// ### AvlNode ###
// Node of the AVL Tree
//  - l      : left son
//  - r      : right son
//  - height : height of the node
//  - value  : value stored in the node
typedef struct AvlNode
{
    AvlNode * l;
    AvlNode * r;
    int height;
    void * value;
} AvlNode;


// ### AVL Tree ###
// - root   : root of the tree
// - Key    : function returning the key (hash) of the value stored in a node
// - Less   : function returning 1 if the key of the first value is lesser
//            than the key of the second. 0 otherwise.
// - Insert : function of insertion of a value in a node
// - Print  : function displaying the value of a node
// - Free   : function freeing the memory allocated to the value
typedef struct AvlTree
{
    AvlNode * root;
    void * (*Key)(void * value);
    int (*Less)(void * lhs, void * rhs);
    void (*Insert)(void * value, void ** AvlNode);
    void (*Print)(void * value);
    void (*Free)(void * value);
} AvlTree;


// Insert the value in the tree if it is not already in it
void insert(void * value, AvlTree * tree);

// Search the key in the tree and return the value associated to this key
// if found. Otherwise returns NULL.
void * search(void * key, AvlTree * tree);

// Delete the tree and his sons and free memory
void empty(AvlTree * tree);

// Return the height of the tree.
// Return -1 is the tree is empty
int height(AvlTree * tree);

// Return if the tree is empty
int is_empty(AvlTree * tree);

// pre_order traversal of the tree
AvlTree * pre_order(AvlTree * tree);

// in_order traversal of the tree
AvlTree * in_order(AvlTree * tree);

// post_order traversal of the tree
AvlTree * post_order(AvlTree * tree);

#endif
