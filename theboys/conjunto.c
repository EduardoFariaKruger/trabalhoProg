#include <stdio.h>
#include "conjunto.h"
#include <stdlib.h>

/*
 * Cria um conjunto vazio e o retorna, se falhar retorna NULL.
 * max eh o tamanho maximo do conjunto, isto eh, o tamanho maximo do vetor
 */
struct conjunto *cria_cjt (int max)
{
    struct conjunto *conjunto;
    if(!(conjunto = malloc(sizeof(struct conjunto))))
    {
        return NULL;
    }
    int *v;
    if(!(v = (int*) malloc(max * sizeof(int))))
    {
        return NULL;
    }
    conjunto->max = max;
    conjunto->card = 0;
    conjunto->ptr = 0;
    conjunto->v = v;
    return conjunto;
}

/*
 * Remove todos os elementos do conjunto, libera espaco e devolve NULL.
 */
struct conjunto *destroi_cjt (struct conjunto *c)
{
    free(c->v);
    free(c);
    return NULL;
}

/*
 * Retorna 1 se o conjunto esta vazio e 0 caso contrario.
 */
int vazio_cjt (struct conjunto *c)
{
    return(c->card == 0);
}

/*
 * Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.
 */
int cardinalidade_cjt (struct conjunto *c)
{
    return c->card;
}

/*
 * Insere o elemento no conjunto, garante que nao existam repeticoes.
 * Retorna 1 se a operacao foi bem sucedida. Se tentar inserir elemento existente,
 * nao faz nada e retorna 1 tambem. Retorna 0 em caso de falha por falta de espaco.
 */
int insere_cjt (struct conjunto *c, int elemento)
{
    if(!pertence_cjt(c, elemento))
    {
        if(c->card > c->max)
        {
            return 0;
        }
        (c->v)[c->card] = elemento;
        c->card = c->card + 1;
        return 1;
    }
    return 0;
}

/*
 * Remove o elemento 'elemento' do conjunto caso ele exista.
 * Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.
 */
int retira_cjt (struct conjunto *c, int elemento)
{
    if(pertence_cjt(c, elemento))
    {
        for(int i=0; i < c->card; i++)
        {
            if((c->v)[i] == elemento)
            {
                c->card = c->card - 1;
                (c->v)[i] = (c->v)[c->card];
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.
 */
int pertence_cjt (struct conjunto *c, int elemento)
{
    int pertence = 0;
    for (int i=0; i < c->card; i++)
    {
        if ((c->v)[i] == elemento)
        {
            pertence = 1;
        }
    }
    return pertence;
}

/*
 * Retorna 1 se o conjunto c1 esta contido no conjunto c2 e 0 caso contrario.
 */
int contido_cjt (struct conjunto *c1, struct conjunto *c2)
{
    int i = 0;
    int pertence = 1;
    while(i <= c1->card - 1)
    {
        if(!pertence_cjt(c2, (c1->v)[i]))
        {
            pertence = 0;
        }
        i++;
    }
    return (pertence);
}

/*
 * Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.
 */
int sao_iguais_cjt (struct conjunto *c1, struct conjunto *c2)
{
    return(contido_cjt(c1, c2) && contido_cjt(c2, c1));
}

/*
 * Cria e retorna o conjunto diferenca entre c1 e c2, nesta ordem.
 * Retorna NULL se a operacao falhou.
 */
struct conjunto *diferenca_cjt (struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *novo;
    novo = cria_cjt(c1->card);
    for (int i = 0; i < c1->card; i++)
    {
        if(!pertence_cjt(c2, (c1->v)[i]))
        {
            insere_cjt(novo, (c1->v)[i]);
        }
    }
    return novo;
}

/*
 * Cria e retorna o conjunto interseccao entre os conjuntos c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
struct conjunto *interseccao_cjt (struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *novo;
    if (c1->card > c2->card)
    {
        novo = cria_cjt(c1->card);
        for (int i = 0; i < c1->card; i++)
        {
            if(pertence_cjt(c2, (c1->v)[i]))
            {
                insere_cjt(novo, (c1->v)[i]);
            }
        }
    }
    else
    {
        novo = cria_cjt(c2->card);
        for (int i = 0; i < c2->card; i++)
        {
            if(pertence_cjt(c1, (c2->v)[i]))
            {
                insere_cjt(novo, (c2->v)[i]);
            }
        }
    }
    return novo;
}

/*
 * Cria e retorna o conjunto uniao entre os conjunto c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
struct conjunto *uniao_cjt (struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *novo = cria_cjt(c1->max);
    for(int i=0; i < c1->card; i++)
    {
        insere_cjt(novo, (c1->v)[i]);
    }
    for(int i=0; i < c2->card; i++)
    {
        insere_cjt(novo, (c2->v)[i]);
    }
    return novo;
}

/*
 * Cria e retorna uma copia do conjunto c e NULL em caso de falha.
 */
struct conjunto *copia_cjt (struct conjunto *c)
{
    struct conjunto *novo;
    novo = cria_cjt(c->max);
    if (novo == NULL)
    {
        return NULL;
    }
    novo->max = c->max;
    novo->ptr = c->ptr;
    novo->card = c->card;
    for (int i=0; i < novo->card; i++)
    {
       (novo->v)[i] = (c->v)[i];
    }
    return novo;
}

/*
 * Cria e retorna um subconjunto com elementos aleatorios do conjunto c.
 * Se o conjunto for vazio, retorna um subconjunto vazio.
 * Se n >= cardinalidade (c) entao retorna o proprio conjunto c.
 * Supoe que a funcao srand () tenha sido chamada antes.
 */
struct conjunto *cria_subcjt_cjt (struct conjunto *c, int n)
{

    struct conjunto *novo;
    if(n >= c->card)
    {
        novo = copia_cjt(c);
    }
    if(n == 0)
    {
        novo = cria_cjt(n);
    }
    else
    {
        novo = cria_cjt(c->max);
        if(novo == NULL)
        {
            return NULL;
        }
        for(int i = 0; i <= n-1; i++)
        {
            insere_cjt(novo, (c->v)[rand() % n + 1]);
        }
    }
    return novo;
}

/*
 * Imprime os elementos do conjunto sempre em ordem crescente,
 * mesmo que a estrutura interna nao garanta isso.
 * Na impressao os elementos sao separados por um unico espaco
 * em branco entre os elementos, sendo que apos o ultimo nao
 * deve haver espacos em branco. Ao final imprime um \n.
 */
void imprime_cjt(struct conjunto *c) {
    int i, key, j;

    // Aplica o algoritmo de ordenação Insertion Sort
    for (i = 1; i < c->card; i++) {
        key = (c->v)[i];
        j = i - 1;

        while (j >= 0 && (c->v)[j] > key) {
            (c->v)[j + 1] = (c->v)[j];
            j = j - 1;
        }

        (c->v)[j + 1] = key;
    }

    // Imprime os elementos ordenados
    for (int i = 0; i < c->card; i++) {
        printf("%d ", (c->v)[i]);
    }
    printf("\n");
}

/*
 * As funcoes abaixo implementam um iterador que vao permitir
 * percorrer os elementos do conjunto.
 * O ponteiro ptr da struct (iterador) pode ser inicializado para apontar 
 * para o primeiro elemento e incrementado ate' o ultimo elemento 
 * do conjunto.
 */

/*
 * Inicializa ptr usado na funcao incrementa_iterador 
 */
void inicia_iterador_cjt (struct conjunto *c)
{
    c->ptr = 0;
}

/*
 * Devolve no parametro ret_iterador o elemento apontado e incrementa o iterador.
 * A funcao retorna 0 caso o iterador ultrapasse o ultimo elemento, ou retorna
 * 1 caso o iterador aponte para um elemento valido (dentro do conjunto).
 */
int incrementa_iterador_cjt(struct conjunto *c, int *ret_iterador) {
    if (c->ptr < c->card) {
        *ret_iterador = (c->v)[c->ptr];
        (c->ptr)++;
        return 1;
    } else {
        return 0;  // Retorna 0 quando o iterador ultrapassa o último elemento
    }
}

/*
 * Escolhe um elemento qualquer do conjunto para ser removido, o remove e
 * o retorna.
 * Nao faz teste se conjunto eh vazio, deixa para main fazer isso       
 */
int retira_um_elemento_cjt (struct conjunto *c)
{
    int aleat = rand() % c->card;
    int elem = (c->v)[aleat];
    retira_cjt(c, elem);
    return elem;
}
