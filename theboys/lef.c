#include "lef.h"
#include <stdio.h>
#include <stdlib.h>


/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2)
{
    struct evento_t *evento;
    if (!(evento = malloc(sizeof(struct evento_t))))
    {
        return NULL;
    }

    evento->tempo = tempo;
    evento->tipo  = tipo;
    evento->dado1 = dado1;
    evento->dado2 = dado2;

    return evento;
}

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e)
{
    free(e);
    e = NULL;
    return NULL;
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef ()
{
    struct lef_t *lef;
    if (!(lef = malloc(sizeof(struct lef_t))))
    {
        return NULL;
    }
    lef->primeiro = NULL;
    return lef;
}

/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l)
{
    struct nodo_lef_t *p = l->primeiro;
    struct nodo_lef_t *temp;
    while (p != NULL)
    {
        temp = p;
        destroi_evento(p->evento);
        p = p->prox;
        free(temp);
    }
    free(l);
    return NULL;
}

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef(struct lef_t *l, struct evento_t *e)
{
    if (l == NULL)
    {
        return 0;
    }

    struct nodo_lef_t *novo;
    if (!(novo = malloc(sizeof(struct nodo_lef_t))))
    {
        return 0;
    }

    novo->evento = e;
    novo->prox = NULL;

    if (l->primeiro == NULL || l->primeiro->evento->tempo > e->tempo)
    {
        // Inserir no início da lista ou em uma lista vazia
        novo->prox = l->primeiro;
        l->primeiro = novo;
        return 1;
    }

    struct nodo_lef_t *temp = l->primeiro;
    while (temp->prox != NULL && temp->prox->evento->tempo <= e->tempo)
    {
        temp = temp->prox;
    }

    // Inserir no meio ou no final da lista
    novo->prox = temp->prox;
    temp->prox = novo;

    return 1;
}

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l)
{
    if(l == NULL || l->primeiro == NULL)
    {
        return NULL;
    }
    struct nodo_lef_t *temp;
    struct evento_t *evento;
    temp = l->primeiro;
    l->primeiro = temp->prox;
    evento = temp->evento;
    free(temp);
    return evento;
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se vazia e 0 senao ou se falhar.
 */
int vazia_lef (struct lef_t *l)
{
    if(l == NULL)
    {
        return 0;
    }
    return (l->primeiro != NULL);
}

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l)
{
    int cont = 0;
    struct nodo_lef_t *p = l->primeiro;
    while (p != NULL)
    {
        printf("%d %d %d %d\n", p->evento->tempo, p->evento->tipo, p->evento->dado1, p->evento->dado2);
        cont += 1;
        p = p->prox;
    }
    printf("total %d eventos\n", cont);
}