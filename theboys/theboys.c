#include <stdio.h>
#include <stdlib.h>
#include "lef.h"
#include "conjunto.h"
#include "fila.h"
#include "mundo.h"
/* demais includes */

/* funcoes que voce ache necessarias aqui */
int main (){
    /* declaracoes de variaveis aqui */

    srand (0); /* use zero, nao faca com time (0) */

	nodo_lef_t *lef = NULL;
	mundo w;
	insere_lef(&lef, 0, INICIALIZA, -1, -1);

    Inicializa_Mundo(w);
    /* coloque seu codigo aqui */

    while (w.clk < T_FIM_DO_MUNDO) {
		w.clk = lef->time;

		switch(lef->type) {
			case INICIALIZA:
				beginWorld(&w, &lef);
				break;
				
			case CHEGA:
				Chega(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case ESPERA:
				Espera(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case DESISTE:
				Desiste(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case AVISA:
				Avisa(lef->time, &(w.bases[lef->data2]), &lef);
				break; 

			case ENTRA:
				Entra(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case VIAJA:
				Viaja(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case SAI:
				Sai(lef->time, &(w.heroes[lef->data1]), &(w.bases[lef->data2]), &lef);
				break;

			case MISSAO:
				//printf("\nMISSAO TESTE");
				//imprime(w.missions[lef->data1].skillReq);
				missao(lef->time, &(w.missions[lef->data1]), &w, &lef);
				break;

			case FIM:
				Fim(lef->time, w);
				break;

			default:
				printf("Esse código não existe\n");
				break;
		}
		rmLef(&lef);
	}
	
	destroyLef(lef);

    return 0;
}
