#include "fila.h"
#include "lef.h"
#include "conjunto.h"
#include <math.h>
#include <time.h>

#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MISSAO 7 
#define FIM 8
#define INICIALIZA 9

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS 50
#define N_BASES 8
#define N_MISSOES 5256

typedef struct coordenada{
    int x;
    int y;
} coord;

typedef struct heroi {
	int id; 
	int xp;
	int base;
	int paciencia; //0..100
	int velocidade; //50..5000
	conjunto *habilidades;
	int numHabilidades;
	coord coordenada;
} heroi;

typedef struct base {
	int id;
	coord coordenadas;
	int capacidade; 
	conjunto *numhHeroisPresentes;
	nodo *espera; 
} base;

typedef struct missao {
	int id;
    coord coordenadas;
	conjunto *habilidadesNec;
} missao;

typedef struct mundo {
	int tempo;
	int numHerois;
	int numBases;
	int numMissoes;
	int numHabilidades;
	int numMissoesCumpridas;
	int numTentativasDeMissoes;
	heroi herois[N_HEROIS];
	base bases[N_BASES];
	missao missoes[N_MISSOES];
	coord coordenadas;
} mundo;

void Chega(int t, heroi *h, base *b, nodo_lef_t **head);
void Espera(int t, heroi *h, base *b, nodo_lef_t **head);
void Desuste(int t, heroi *h, base *b, nodo_lef_t **head);
void Avisa(int t, base *b, nodo_lef_t **head);
void Entra(int t, heroi *h, base *b, nodo_lef_t **head);
void Sai(int t, heroi *h, base *b, nodo_lef_t **head);
void Viaja(int t, heroi *h, base *d, nodo_lef_t **head);
void Missao(int t, missao *m, mundo *w, nodo_lef_t **head);
void fim(int t, mundo w);
void Comecar_Mundo(mundo *w, nodo_lef_t **lef);
void Inicialida_Bases(mundo *w);
void Inicializa_Herois(mundo *w);
void Inicializa_Missoes(mundo *w);
void Inicializa_Mundo(mundo *w);