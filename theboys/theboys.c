#include <stdio.h>
#include <stdlib.h>
#include "lef.h"
#include "conjunto.h"
#include "fila.h"
/* demais includes */

/* funcoes que voce ache necessarias aqui */

struct coordenadas
{
    int x;
    int y;
};

struct H
{
    int id;
    struct conjunto *habilidades;
    int paciencia;
    int velocidade;
    int xp;
    int baseID;
};

struct B 
{
    int id;
    int lotacao;
    int presentes;
    int espera;
    struct coordenadas local;
};

struct M
{
    int id;
    struct conjunto *habilidades;
    struct coordenadas local;
};

struct W
{
    int N_herois;
    struct H *h;
    int N_bases;
    struct B *b;
    int N_missoes;
    struct M *m;
    int N_habilidades;
    struct coordenadas tamanho;
    int relogio;
};

int gera_aleat(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void inicializa_mundo()
{
    struct W *mundo;
    if (!(mundo = malloc(sizeof(struct W)))) {
        return NULL;
    }
    mundo->relogio = 0;
    mundo->N_habilidades = 10;
    mundo->N_herois = 50;
    mundo->N_bases = 8;
    mundo->N_missoes = 5256;
    (mundo->tamanho).x = 20000;
    (mundo->tamanho).y = 20000;
    struct H *h;
    struct M *m;
    struct B *b;

    if(!(h = malloc(mundo->N_herois * sizeof(struct H))))
    {
        return NULL;
    }
        if(!(b = malloc(mundo->N_bases * sizeof(struct B))))
    {
        return NULL;
    }
        if(!(m = malloc(mundo->N_herois * sizeof(struct M))))
    {
        return NULL;
    }
    mundo->b = b;
    mundo->m = m;
    mundo->h = h;

}

void inicializarHeroi(struct H *heroi, int id) {
    heroi->id = id;
    heroi->paciencia = rand() % 101; // Paciência de 0 a 100
    heroi->velocidade = rand() % 4951 + 50; // Velocidade de 50 a 5000 metros/minuto
    heroi->xp = 0;
    heroi->baseID = -1; // Inicializar como -1 (fora de qualquer base)

    // Inicializar habilidades do herói
    int tamanhoConjunto = rand() % 3 + 1; // Tamanho aleatório de 1 a 3
    inicializarConjunto(&heroi->habilidades, tamanhoConjunto);
}

void inicializarBase(struct B *base, int id) {
    base->id = id;
    base->local.x = rand() % 20000;
    base->local.y = rand() % 20000;
    base->lotacao = rand() % 8 + 3; // Lotação de 3 a 10 heróis
    base->presentes = 0; // Inicializar sem heróis presentes
    base->espera = 0; // Inicializar fila vazia
}

void inicializarMissao(struct M *missao, int id) {
    missao->id = id;
    missao->local.x = rand() % 20000;
    missao->local.y = rand() % 20000;

    // Inicializar habilidades necessárias para a missão
    int tamanhoConjunto = rand() % 5 + 6; // Tamanho aleatório de 6 a 10
    inicializarConjunto(&missao->habilidades, tamanhoConjunto);
}

int main (){
    /* declaracoes de variaveis aqui */

    srand (0); /* use zero, nao faca com time (0) */

    /* coloque seu codigo aqui */

    return 0;
}
