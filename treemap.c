#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap *map = (TreeMap*) malloc(sizeof(TreeMap));  // Se reserva memoria para el mapa
    if (map == NULL) return NULL;

    // Iniciamos los punteros en Null

    map->root = NULL;
    map->current = NULL;


    map->lower_than = lower_than; // colocamos el comparar 

    return map; // retornamos el mapa :D
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *parent = NULL;
    TreeNode *current = tree->root; 

    while(current != NULL){
        if (is_equal(tree,key, current->pair->key)) return; // La key ya esta en el arbol

        parent = current;

        if (tree->lower_than(key,current->pair->key)) current = current->left;

        else current = current->right;
    }
    // Creamos un nuevo nodo

    TreeNode *newNode = createTreeNode(key, value);
    newNode->parent = parent;

    // Lo enlazamos con su padre, I am your father >:)
    if (parent == NULL) tree->root = newNode;

    else if (tree->lower_than(key, parent->pair->key)) parent->left = newNode;

    else parent->right = newNode;

    tree->current = newNode; // Actualizamos el current 
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    // se mueve a la izquierda del arbol hasta el nodo más chikito ;;
    while(x->left != NULL) x = x->left;

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    // En el caso sin hijos:

    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL) tree->root = NULL;
        else if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    
        free(node->pair);
        free(node);
    }
    // Caso nodo con un solo hijo:

    else if (node->left == NULL || node->right == NULL){
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        child->parent = node->parent;

        if (node->parent == NULL) tree->root = child;

        else if (node->parent->left == node) node->parent->left = child;

        else node->parent->right = child;

        free(node->pair);
        free(node);
    }

    // Caso con dos hijitos:

    else{
        TreeNode *minRight = minimum(node->right);
        node->pair->key = minRight->pair->key;
        node->pair->value = minRight->pair->value;
        removeNode(tree, minRight);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root; // Creamos un auxiliar para recorrer

    while(aux != NULL){ // Se recorre el árbol en busca de la key 0.0
        if (is_equal(tree, key, aux->pair->key)){
            tree->current = aux; // actualizamos el current pe
            return aux->pair; // se retorna el par encontrado
        }

        if (tree->lower_than(key, aux->pair->key)) aux = aux->left; // se crean condiciones para decidir el camino left o right
        else{
            aux = aux->right;
        }
    }


    return NULL; // retornamos Null si no se encuentra la key :c
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;

    TreeNode* node = minimum(tree->root); // Encontramos el nodo más chikito

    tree->current = node; 

    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;

    TreeNode* node = tree->current;

    // Si existe un sub arbol derecho, buscamos su nodo más chikito 0.0
    if (node->right != NULL){
        node = minimum(node->right);
        tree->current = node;
        return node->pair;
    }

    // Si no existe, subimos por el arbol hasta encontrar el siguiente nodo -w-

    TreeNode* parent = node->parent;
    while (parent != NULL && node == parent->right){
        node = parent;
        parent = parent->parent;
    }

    tree->current = parent;

    if (parent == NULL) return NULL;

    return parent->pair;
}
