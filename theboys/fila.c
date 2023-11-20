#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


/*
    cria e retorna um ponteiro para um TAD fila
*/
struct fila *fila_cria ()
{
    struct fila *nodoC;
    if(!(nodoC = malloc(sizeof(struct fila))))
    {
        return NULL;
    }
    nodoC->tamanho = 0;
    nodoC->ini = NULL;

    return nodoC;
}

/* Libera toda memoria da fila usando o laço e no final faz a fila receber NULL. */
void fila_destroi (struct fila **fila)
{
    struct nodo *p, *aux;
    p = (*fila)->ini;
    while (p != NULL)
    {
        aux = p;
        p = p->prox;
        free(aux);
    }
    free(*fila);
    *fila = NULL;
}

/* 
 * Insere dado na fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (struct fila *fila, int dado)
{
    struct nodo *novo;
    if (!(novo = malloc(sizeof(struct nodo))))
    {
        return 0;
    }
    if (fila->tamanho == 0)
    {
        fila->ini = novo;
        fila->fim = novo;
        fila->tamanho++;
        return 1;
    }
    fila->fim->prox = novo;
    fila->fim = novo;
    novo->chave = dado;
    fila->tamanho++;
    return 1;
}

/* 
 * Remove dado da fila (politica FIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (struct fila *fila, int *dado)
{
    struct nodo *auxiliar;
    if (fila->tamanho == 0)
    {
        return 0;
    }
    if (fila -> tamanho == 1)
    {
        *dado = fila->ini->chave;
        free(fila->ini);
        fila->ini = NULL;
        fila->fim = NULL;
        fila->tamanho--;
        return 1;
    }
    auxiliar = fila->ini;
    fila->ini = auxiliar->prox;
    *dado = auxiliar->chave;
    free(auxiliar);
    fila->tamanho--;
    return 1;
}

/* Retorna o numero de elementos da pilha, que pode ser 0.*/
int fila_tamanho (struct fila *fila)
{
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario*/ 
int fila_vazia (struct fila *fila)
{
    return (fila->tamanho == 0);
}

void fila_imprime(struct fila *fila) {
    struct nodo *atual = fila->ini;

    printf("[ ");
    
    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->prox;
    }

    printf("]\n");
}