#include "mundo.h"
#include "fila.h"
#include "conjunto.h"
#include <math.h>
#include <time.h>
#include "theboys\lef.h"

int aleat(int min, int max){
    return rand() % (max + 1 - min) + min;
}

void chega(int t, heroi *h, base *b, nodo_lef_t **head) {
	h->base = b->id;
	int espera;
	h->coordenada.x = b->coordenadas.x;
	h->coordenada.y = b->coordenadas.y;

	if ((cardinalidade_cjt(b->numhHeroisPresentes) < (b->capacidade)) && fila_tamanho(b->espera) <= 0)
		espera = 1;
	else 
		espera = (h->paciencia) > (10 * fila_tamanho(b->espera));

	printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ", t, h->id, b->id, cardinalidade_cjt(b->numhHeroisPresentes), b->capacidade);
	
	if (espera) {
		insere_lef(head, t, ESPERA, h->id, b->id);
		printf("ESPERA\n");	
	} else {
		insere_lef(head, t, DESISTE, h->id, b->id);
		printf("DESISTE\n");
	}
}

void espera(int t, heroi *h, base *b, nodo_lef_t **head) {
	insere_lef(&(b->espera), h->id);
	insere_lef(head, t, AVISA, h->id, b->id);
	
	printf("%6d: ESPERA HEROI %2d BASE %d (%d)\n", t, h->id, b->id, fila_tamanho(b->espera));
}

void desiste(int t, heroi *h, base *b, nodo_lef_t **head) {
	//escolhe base d aleatoria	
	int Idaleatorio = aleat(0,N_BASES-1);

	insere_lef(head, t, VIAJA, h->id, Idaleatorio);
	
	printf("%6d: DESISTE HEROI %2d BASE %d\n", t, h->id, b->id);
}

void avisa(int t, base *b, nodo_lef_t **head) {
	int id;
	
	printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id, cardinalidade_cjt(b->numhHeroisPresentes), b->capacidade);
	fila_imprime(b->espera);
	printf("]\n");

	while ( (fila_tamanho(b->espera) > 0) && (cardinalidade_cjt(b->numhHeroisPresentes) < (b->capacidade)) ) {
		id = dequeue(&(b->espera));
		insert(&(b->numhHeroisPresentes), id);
		insere_lef(head, t, ENTRA, id, b->id); 
		printf("%6d: AVISA PORTEIRO BASE %d ADMITE %d\n", t, b->id, id);
	}
}

void entra(int t, heroi *h, base *b, nodo_lef_t **head) {
	int tpb = 15 + (h->paciencia)*aleat(1,20);
	insere_lef(head, t + tpb, SAI, h->id, b->id);
	
	printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id, cardinalidade_cjt(b->numhHeroisPresentes), b->capacidade, t+tpb);	
}

void sai(int t, heroi *h, base *b, nodo_lef_t **head) {
	int bId = aleat(0,7);

	insere_lef(head, t, VIAJA, h->id, bId);

	insere_lef(head, t, AVISA, -1, b->id); //nao utiliza heroi
	
	printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, h->id, b->id, cardinalidade_cjt(b->numhHeroisPresentes), b->capacidade);
}

void viaja(int t, heroi *h, base *d, nodo_lef_t **head) {
	unsigned int dx = (d->coordenadas.x) - (h->coordenada.x);
	unsigned int dy = (d->coordenadas.y) - (h->coordenada.y);
	
	
	int distancia = sqrt((dx*dx) + (dy*dy));
	int duracao = distancia/(h->velocidade);
	
	insertLef(head, t + duracao, CHEGA, h->id, d->id); 
	
	printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id, h->base, d->id, distancia, h->velocidade, t + duracao);
}

int distanciaAoQuadrado(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

// Função para trocar duas bases
void trocar(base *a, base *b) {
    base temp = *a;
    *a = *b;
    *b = temp;
}

void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
    }
}

void missao(int t, missao *m, mundo *w, nodo_lef_t **head) {
	(w->numTentativasDeMissoes)++;
    printf("%6d: MISSAO %d HAB REQ: [ ", t, m->id);
    if (m->habilidadesNec != NULL)
        imprime_cjt(m->habilidadesNec);
    printf("]\n");

	int bmp = -1;
    base basesProximas[N_BASES];

    // Copia as bases para o vetor nearest
    for (int i = 0; i < N_BASES; i++) 
    {
        basesProximas[i] = w->bases[i];
    }
    // Ordena as bases por proximidade à missão
    shellSort(basesProximas, N_BASES, m->coordenadas.x, m->coordenadas.y);

    // Inicializa o conjunto de habilidades
    conjunto *ConjuntoHabilidades = cria_cjt(N_HABILIDADES);
	conjunto *CJTHeroisPresentes = cria_cjt(N_HEROIS);

    int j = 0;
    int habilidadesEncontradas = 0;
    while (j < N_BASES && !habilidadesEncontradas) {
        CJTHeroisPresentes = basesProximas[j].numhHeroisPresentes;

        // Reseta skillU para cada base
        destroi_cjt(ConjuntoHabilidades);
        ConjuntoHabilidades = NULL;

        // Itera por cada herói presente na base
        for(int i = 0, i < cardinalidade_cjt(CJTHeroisPresentes))
        {
            int heroiId = CJTHeroisPresentes->data;
            // Unindo as habilidades do herói atual
            uniao_cjt(ConjuntoHabilidades, w->heroes[heroiId].skills, &ConjuntoHabilidades);
            presentes = presentes->prox;
        }
   
	
        if (contem(ConjuntoHabilidades, m->habilidadesNec)) {
            bmp = j;
            habilidadesEncontradas = 1;
        }
        j++;
    }
    
// Se encontrou uma base com as habilidades requeridas
	//printf("BMP: %d\n", bmp);
    if (bmp != -1) {
        int bId = nearest[bmp].id;
        conjunto *aux = w->bases[bId].presentes;
        while (aux != NULL) {
            w->heroes[aux->data].xp++;
            aux = aux->prox;
        }
        (w->cumpridas)++;
    } else {
        // Insere um evento para a próxima missão se não encontrar habilidades
        insertLef(head, t + 1440, MISSAO, m->id, -1);
    }

    // Libera o conjunto de habilidades
    destroi_cjt(skillU); // Liberar recursos alocados
}

void Inicializa_Missoes(mundo *w) {
	int numHabilidades;
    int habilidade;
	
	for (int i = 0; i<N_MISSOES; i++) {
		w->missoes[i].id = i;
		(w->missoes[i]).coordenadas.x = aleat(0, N_TAMANHO_MUNDO-1);
		(w->missoes[i]).coordenadas.y = aleat(0, N_TAMANHO_MUNDO-1);		
		numHabilidades = aleat(6, 10);
		
		w->missoes[i].habilidadesNec = cria_cjt(numHabilidades);
		
		for (int j = 0; j<numHabilidades; j++) {
			habilidade = aleat(1,10);
			insere_cjt(&(w->missoes[i].habilidadesNec), habilidade);
		}
	}

}

void Inicialida_Bases(mundo *w) {
	for (int i = 0; i<N_BASES; i++) {
		w->bases[i].id = i;
		w->bases[i].coordenadas.x = aleat(0, N_TAMANHO_MUNDO-1);
		w->bases[i].coordenadas.y = aleat(0, N_TAMANHO_MUNDO-1);
		w->bases[i].capacidade = aleat(3, 10);
		w->bases[i].numhHeroisPresentes = NULL;
		w->bases[i].espera = NULL;
	}
}

void inicializaHerois(mundo *w) {
	int numHabilidades, habilidade;

	for (int i = 0; i<N_HEROIS; i++) {
		w->herois[i].id = i;
		w->herois[i].xp = 0;
		w->herois[i].paciencia = aleat(0,100);
		w->herois[i].velocidade = aleat(50, 5000);
		
		numHabilidades = aleat(1,3);
		
		w->herois[i].habilidades = cria_cjt(numHabilidades);
		
		for (int j = 0; j<numHabilidades; j++) {
			habilidade = aleat(1,10);
			insere_cjt(&(w->herois[i].habilidades), habilidade);
		}
	}
}

void inicializaMissoes(mundo *w) {
	int numHabilidades, habilidade;
	
	for (int i = 0; i<N_MISSOES; i++) {
		w->missoes[i].id = i;
		w->missoes[i].coordenadas.x = aleat(0, N_TAMANHO_MUNDO-1);
		w->missoes[i].coordenadas.y = aleat(0, N_TAMANHO_MUNDO-1);		
		numHabilidades = aleat(6, 10);
		
	    w->missoes[i].habilidadesNec = cria_cjt(numHabilidades);
		
		for (int j = 0; j<numHabilidades; j++) {
			habilidade = aleat(1,10);
			insere_cjt(&(w->missoes[i].habilidadesNec), habilidade);
		}
	}

}

void inicializaMundo(mundo *w) {
	w->tempo = 0;

	w->numHerois = N_HEROIS;
	w->numBases = N_BASES;
	w->numMissoes = N_MISSOES;
	w->numHabilidades = N_HABILIDADES;
	
	w->numMissoesCumpridas = 0;
	w->numTentativasDeMissoes = 0;

	inicializaMissoes(w);
	inicializaBases(w);
	inicializaHerois(w);

	w->x = 0;
	w->y = 0;
}

void fim(int t, mundo w) {
	printf("%6d: FIM\n", t);

	for (int i=0; i<N_HEROIS; i++) {
		printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS [ ", i, w.herois[i].paciencia,
											w.herois[i].velocidade,
											w.herois[i].xp);
		
		conjunto *atual = w.herois[i].habilidades; 
		imprime(atual);
		printf("]\n");
	}
		double tryRatio = (double)w.tentativas / w.nMissions;
		double completude = (double)w.cumpridas / w.nMissions;
		printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
       		w.cumpridas, N_MISSOES,
       		100*completude,
       		tryRatio);

		

	for (int i = 0; i < N_HEROIS; i++) {
	  freeConjunto(w.heroes[i].skills);
	}
	
	for (int i = 0; i<N_BASES; i++) {
		freeQueue(w.bases[i].espera);
	}

	for (int i = 0; i < N_BASES; i++) {
	  freeConjunto(w.bases[i].presentes);
	  // Libere também a fila de espera, se aplicável
	}

	for (int i = 0; i < N_MISSOES; i++) {
	  freeConjunto(w.missions[i].skillReq);
	}

}
