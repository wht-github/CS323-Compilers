#include "symtab.h"

/*
 * symbol table type, binary tree impl
 */
struct symtab
{
    entry *entry;
    struct symtab *left, *right, *parent;
};
int found = 0;
// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab *symtab_init()
{
    symtab *root = malloc(sizeof(symtab));
    root->left = root->right = NULL;
    root->entry = NULL;
    return root;
}
symtab *my_symtab_insert(symtab *self, char *key, VAL_T value, int *res)
{
    if (self == NULL || self->entry == NULL)
    {
        if (self == NULL)
        {
            self = malloc(sizeof(symtab));
        }
        symtab *new = malloc(sizeof(symtab));
        memset(new, '\0', sizeof(symtab));
        new->entry = malloc(sizeof(entry));
        entry_init(new->entry, key, value);
        new->left = new->right = NULL;
        memcpy(self, new, sizeof(symtab));
    }
    else
    {
        if (strcmp(key, self->entry->key) > 0)
        {
            self->right = my_symtab_insert(self->right, key, value, res);
        }
        else if (strcmp(key, self->entry->key) < 0)
        {
            self->left = my_symtab_insert(self->left, key, value, res);
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
    my_symtab_insert(self, key, value, &res);
    return res;
}

VAL_T symtab_lookup(symtab *self, char *key)
{
    if (self == NULL)
    {
        return -1;
    }
    else if (strcmp(key, self->entry->key) == 0)
    {
        return self->entry->value;
    }
    else
    {
        return strcmp(key, self->entry->key) > 0 ? symtab_lookup(self->right, key) : symtab_lookup(self->left, key);
    }
}

symtab *myremove(symtab **root, char *key)
{
    if (*root == NULL)
        return NULL;
    if (strcmp(key, (*root)->entry->key) < 0)
    {
        (*root)->left = myremove(&(*root)->left, key);
    }
    else if (strcmp(key, (*root)->entry->key) > 0)
    {
        (*root)->right = myremove(&(*root)->right, key);
    }
    else
    {
        symtab *tmp = root;
        if ((*root)->left != NULL && (*root)->right != NULL)
        {
            symtab *target = (*root)->left;
            while (target->right != NULL)
                target = target->right;
            entry *tmp = target->entry;
            myremove(&(*root), target->entry->key);
            (*root)->entry = tmp;
        }
        else
        {

            (*root) =(*root)->left != NULL ? (*root)->left : (*root)->right;
        }
    }
    return (*root);
}

int symtab_remove(symtab *self, char *key)
{
    found = 0;
    if (symtab_lookup(self, key) == 0)
    {
        return -1;
    }
    memcpy(self, myremove(&self, key), sizeof(symtab));
    // printf("Removed");
    return 1;
}
