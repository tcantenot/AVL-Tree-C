#include <stdlib.h>
#include <stdio.h>

#include "avl_tree.h"

#define max(a, b) (((a) > (b))? (a) : (b))

static int _height_(AvlNode * node);
static AvlNode * single_left_rotate(AvlNode * x);
static AvlNode * single_right_rotate(AvlNode * y);
static AvlNode * double_left_rotate(AvlNode * z);
static AvlNode * double_right_rotate(AvlNode * z);


// Insert the value in the tree if it is not already in it
static AvlNode * _insert_(void * value, AvlNode ** root,
    void * (*Key)(void *),
        int (*Less)(void *, void *),
            void (*Insert)(void *, void **))
{
    if((*root) == NULL)
    {
        (*root) = malloc(sizeof(AvlNode));
        if((*root) == NULL)
        {
            printf("Error : cannot allocate AvlNode !\n");
            return NULL;
        }
        (*root)->l = (*root)->r = NULL;
        Insert(value, (void **) root);
    }
    else
    {
        // If the value is lesser than the value of the current node,
        // we insert the value into the left son tree
        if(Less(Key((void *) value), Key((void *) (*root)->value)))
        {
            _insert_(value, &(*root)->l, Key, Less, Insert);

            // If the tree is unbalanced after the insertion, we rebalance it
            if((_height_((*root)->l) - _height_((*root)->r)) == 2)
            {
                if((Less(Key((void *) value), Key((void *) (*root)->l->value))))
                {
                    (*root) = single_right_rotate((*root));
                }
                else
                {
                    (*root) = double_right_rotate((*root));
                }
            }
        }
        // Else if it is greater, we insert the value into the right son tree
        else if(Less(Key((void *) (*root)->value), Key((void *) value)))
        {
            _insert_(value, &(*root)->r, Key, Less, Insert);

            // If the tree is unbalanced after the insertion, we rebalance it
            if((_height_((*root)->r) - _height_((*root)->l)) == 2)
            {
                if((Less(Key((void *) (*root)->r->value), Key((void *) value))))
                {
                    (*root) = single_left_rotate((*root));
                }
                else
                {
                    (*root) = double_left_rotate((*root));
                }
            }
        }
        // Else the value is equal to the current node value => nothing to do
    }

    // Update the height of the tree
    (*root)->height = max(_height_((*root)->l), _height_((*root)->r)) + 1;
    return (*root);
}


// Insert the value in the tree if it is not already in it
void insert(void * value, AvlTree * tree)
{
    _insert_(value, &tree->root, tree->Key, tree->Less, tree->Insert);
}


// Search the key in the tree and returns the node containing the given key
// if found. Otherwise returns NULL.
static AvlNode * _search_(void * key, AvlNode * root,
        void * (*Key)(void *),
            int (*Less)(void *, void *))
{
    if(root == NULL)
    {
        return root;
    }

    if(Less((void *) key, Key((void *) root->value)))
    {
        return _search_(key, root->l, Key, Less);
    }
    else if(Less(Key((void *) root->value), (void *) key))
    {
        return _search_(key, root->r, Key, Less);
    }
    else
    {
        return root;
    }
}


// Search the key in the tree and return the value associated to this key
// if found. Otherwise returns NULL.
void * search(void * key, AvlTree * tree)
{
    AvlNode * result = _search_(key, tree->root, tree->Key, tree->Less);

    return (result == NULL) ? NULL : result->value;
}


// Delete the tree and his sons and free memory
static void _empty_(AvlNode * root, void (*Free)(void *))
{
    if(root != NULL)
    {
        _empty_(root->l, Free);
        _empty_(root->r, Free);
        Free(root->value);
        free(root);
        root = NULL;
    }
}

void empty(AvlTree * tree)
{
    _empty_(tree->root, tree->Free);
}


// Return the height of the tree.
// Return -1 is the tree is empty
static int _height_(AvlNode * node)
{
    return (node == NULL) ? -1 : node->height;
}

int height(AvlTree * tree)
{
    return _height_(tree->root);
}


// Return if the tree is empty
static int _is_empty_(AvlNode * node)
{
    return node == NULL;
}

int is_empty(AvlTree * tree)
{
    return _is_empty_(tree->root);
}


// pre_order traversal of the tree
static void _pre_order_(AvlNode * root, void (*Print)(void *))
{
    if(root != NULL)
    {
        Print((void *) root->value);
        _pre_order_(root->l, Print);
        _pre_order_(root->r, Print);
    }
}


AvlTree * pre_order(AvlTree * tree)
{
    _pre_order_(tree->root, tree->Print);
    return tree;
}


// in_order traversal of the tree
static void _in_order_(AvlNode * root, void (*Print)(void *))
{
    if(root != NULL)
    {
        _in_order_(root->l, Print);
        Print((void *) root->value);
        _in_order_(root->r, Print);
    }
}


AvlTree * in_order(AvlTree * tree)
{
    _in_order_(tree->root, tree->Print);
    return tree;
}


// post_order traversal of the tree
static void _post_order_(AvlNode * root, void (*Print)(void *))
{
    if(root != NULL)
    {
        _post_order_(root->l, Print);
        _post_order_(root->r, Print);
        Print((void *) root->value);
    }
}


AvlTree * post_order(AvlTree * tree)
{
    _post_order_(tree->root, tree->Print);
    return tree;
}



// ### Rebalancing operations ###

/*    x             y    */
/*   / \           / \   */
/*  A   y   ==>   x   C  */
/*     / \       / \     */
/*    B   C     A   B    */

static AvlNode * single_left_rotate(AvlNode * x)
{
    AvlNode * y = x->r;
    x->r = y->l;
    y->l = x;

    x->height = max(_height_(x->l), _height_(x->r)) + 1;
    y->height = max(x->height, _height_(y->r)) + 1;
    return y;
}


/*     y           x      */
/*    / \         / \     */
/*   x   C  ==>  A   y    */
/*  / \             / \   */
/* A   B           B   C  */

static AvlNode * single_right_rotate(AvlNode * y)
{
    AvlNode * x = y->l;
    y->l = x->r;
    x->r = y;

    y->height = max(_height_(y->l), _height_(y->r)) + 1;
    x->height = max(_height_(x->l), y->height) + 1;
    return x;
}


/*       A                 A                  A       */
/*     /   \             /   \              /   \     */
/*    B     z           B     z            B     x    */
/*   / \     \    ==>  / \     \    ==>   / \   / \   */
/*  C   D     y       C   D     x        C   D z   y  */
/*           /                   \                    */
/*          x                     y                   */

static AvlNode * double_left_rotate(AvlNode * z)
{
    z->r = single_right_rotate(z->r);
    return single_left_rotate(z);
}



/*       A               A               A       */
/*     /   \           /   \           /   \     */
/*    B     z         B     z         B     x    */
/*   / \   /    ==>  / \    /  ==>   / \   / \   */
/*  C   D  y        C   D  x        C   D y   z  */
/*          \             /                      */
/*           x           y                       */

static AvlNode * double_right_rotate(AvlNode * z)
{
    z->l = single_left_rotate(z->l);
    return single_right_rotate(z);
}

