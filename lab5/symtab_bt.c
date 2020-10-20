#include "symtab.h"

/*
 * symbol table type, binary tree impl
 */
typedef struct node
{
    entry entry;
    struct node *left, *right;
} node;
struct symtab
{
    node *init;
};


// ************************************************************
//    Your implementation goes here
// ************************************************************
symtab *symtab_init()
{
    symtab *root = malloc(sizeof(symtab));
    root->init = NULL;
    return root;
}

node ** _symtab_insert(node **self, char *key, VAL_T value, int* res)
{
    if ((*self) == NULL)
    {
        *self = malloc(sizeof(node));
        entry_init(&(*self)->entry, key, value);
        (*self)->left = (*self)->right = NULL;
    }
    else
    {
        if (strcmp(key, (*self)->entry.key) > 0)
        {
            (*self)->right = *_symtab_insert(&(*self)->right, key, value, res);
        }
        else if (strcmp(key, (*self)->entry.key) < 0)
        {
            (*self)->left = *_symtab_insert(&(*self)->left, key, value, res);
        }
        else
        {
            *res = 0;
        }
    }
    return self;
}

int symtab_insert(symtab *self, char *key, VAL_T value)
{
    int res = 1;
    _symtab_insert(&self->init, key, value, &res);
    return res;
}
VAL_T _symtab_lookup(node *self, char *key){
    if (strcmp(key, self->entry.key) == 0)
    {
        return self->entry.value;
    }
    else
    {
        return strcmp(key, self->entry.key) > 0 ? _symtab_lookup(self->right, key) : _symtab_lookup(self->left, key);
    }
}
VAL_T symtab_lookup(symtab *self, char *key)
{
    if (self->init == NULL){
        return -1;
    } else {
        return _symtab_lookup(self->init, key);
    }
}

node **_remove(node **root, char *key, int * res)
{
    if (*root == NULL)
        return NULL;
    if (strcmp(key, (*root)->entry.key) < 0)
    {
        (*root)->left = _remove((*root)->left, key, res);
    }
    else if (strcmp(key, (*root)->entry.key) > 0)
    {
        (*root)->right = _remove((*root)->right, key, res);
    }
    else
    {
        *res = 1;
        node *tmp = (*root);
        if ((*root)->left != NULL && (*root)->right != NULL)
        {
            node *target = (*root)->left;
            while (target->right != NULL)
                target = target->right;
            entry t = target->entry;
            _remove(root, target->entry.key);
            root->entry = t;
        }
        else
        {
            (*root) = (*root)->left != NULL ? (*root)->left : (*root)->right;
        }
        free(tmp);
    }
    return root;
}

int symtab_remove(symtab *self, char *key)
{
    int res = 0;
    _remove(&self->init, key, &res);
    return res;
}
