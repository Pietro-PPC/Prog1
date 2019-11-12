#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "lib_lista.h"

/* A altura das naves, dos aliens e das barreiras sempre eh 3 */
#define NAVE_MAE_1 " /MMMMM\\ AMoMoMoMA \\/'-'\\/ "
#define NAVE_MAE_2 " /MMMMM\\ AoMoMoMoA \\/'-'\\/ "
#define ALIEN_1_1  " A AMA/X\\"
#define ALIEN_1_2  " A AMA{ }"
#define ALIEN_2_1  ".v_v.}MWM{ / \\ "
#define ALIEN_2_2  ".v_v.}MWM{ } { "
#define ALIEN_3_1  " nmn dbMdb_/-\\_"
#define ALIEN_3_2  " nmn dbMdb |-| "
#define BARREIRA   " AMMMA AMMMMMAMM   MM"
#define CANHAO     " /^\\ MMMMM"

#define MAX_TAM_NAVE 28
#define TAM_BARREIRA 22
#define TAM_CANHAO 11

#define INTERVALO 5000

typedef struct nave_alien
{
	int alt;
	int larg;
	char forma1[MAX_TAM_NAVE];
	char forma2[MAX_TAM_NAVE];
} nave_alien;

/* 
   As posicoes lin e col do bloco sao determinadas em relacao ao 
   canto superior esquerdo da barreira a qual ele pertence
*/
typedef struct bloco
{
	int lin;
	int col;
	int estado;
	char forma;
} bloco;

typedef struct barreira
{
	int tam;
	bloco blocos[TAM_BARREIRA];
} barreira;

typedef struct canhao
{
	int alt;
	int larg;
	char forma[TAM_CANHAO];
} canhao;

typedef struct entidades
{
	int forma;
	nave_alien naves[4];
	barreira barreiras[4];
	canhao jogador;
} entidades;

/* Funcoes de inicializacao da lista de elementos */

int insere_nave_mae(t_lista *l_tela)
{
	if (!insere_fim_lista(0, 3, 1, 0, 1, l_tela)) 
		return 0;

	return 1;
}

int insere_aliens( t_lista *l_tela )
{
	int i, j;
	
	for (i = 0; i < 11; i++)
		if (!insere_fim_lista(1, 7, (5 + 7*i), 0, 1, l_tela)) 
			return 0;
	
	for (i = 0; i < 2; i++)
		for (j = 0; j < 11; j++)
			if (!insere_fim_lista(2, (11 + 4*i), (4 + 7*j), 0, 1, l_tela)) 
				return 0;

	for (i = 0; i < 2; i++)
		for (j = 0; j < 11; j++)
			if (!insere_fim_lista(3, (19 + 4*i), (4 + 7*j), 0, 1, l_tela))
				return 0;

	return 1;
}

int insere_barreiras(t_lista *l_tela)
{
	int i;
	for (i = 0; i < 4; i++)
		if (!insere_fim_lista(4, 31, (15 + 21*i), 0, 1, l_tela))
			return 0;

	return 1;
}

int insere_canhao(t_lista *l_tela)
{
	if (!insere_fim_lista(5, 35, 43, 0, 1, l_tela))
		return 0;
	return 1;
}

int inicializa_lista_tela( t_lista *l_tela )
{
	if (!inicializa_lista (l_tela)) return 0;
	
	if (!insere_nave_mae  (l_tela)) return 0;
	if (!insere_aliens    (l_tela)) return 0;
	if (!insere_barreiras (l_tela)) return 0;
	if (!insere_canhao    (l_tela)) return 0;

	return 1;
}

int inicializa_tela()
{
	int nlin, ncol;

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	getmaxyx(stdscr, nlin, ncol);
	if (nlin < 38 || ncol < 100)
	{
		endwin();
		return 0;
	}

	return 1;
}

/* funcoes de definicao de entidades */

void define_nave_mae(entidades *tabuleiro)
{
	tabuleiro->naves[0].alt = 3;
	tabuleiro->naves[0].larg = strlen(NAVE_MAE_1) / 3;
	strcpy( tabuleiro->naves[0].forma1, NAVE_MAE_1 );
	strcpy( tabuleiro->naves[0].forma2, NAVE_MAE_2 );
}

void define_alien_1(entidades *tabuleiro)
{
	tabuleiro->naves[1].alt = 3;
	tabuleiro->naves[1].larg = strlen(ALIEN_1_1) / 3;
	strcpy( tabuleiro->naves[1].forma1, ALIEN_1_1 );
	strcpy( tabuleiro->naves[1].forma2, ALIEN_1_2 );
}

void define_alien_2(entidades *tabuleiro)
{	
	tabuleiro->naves[2].alt = 3;
	tabuleiro->naves[2].larg = strlen(ALIEN_2_1) / 3;
	strcpy( tabuleiro->naves[2].forma1, ALIEN_2_1 );
	strcpy( tabuleiro->naves[2].forma2, ALIEN_2_2 );
}

void define_alien_3(entidades *tabuleiro)
{
	tabuleiro->naves[3].alt = 3;
	tabuleiro->naves[3].larg = strlen(ALIEN_3_1) / 3;
	strcpy( tabuleiro->naves[3].forma1, ALIEN_3_1 );
	strcpy( tabuleiro->naves[3].forma2, ALIEN_3_2 );
}

void define_naves(entidades *tabuleiro)
{
	define_nave_mae(tabuleiro);
	define_alien_1(tabuleiro);
	define_alien_2(tabuleiro);
	define_alien_3(tabuleiro);
}

void define_barreira(int num, entidades *tabuleiro)
{
	barreira *atual;
	int ind_bloc = 0, ind_bar, ncol;
	
	ncol = strlen(BARREIRA) / 3;
	atual = &(tabuleiro->barreiras[num]);
	for (ind_bar = 0; ind_bar < strlen(BARREIRA); ind_bar++)
		if (BARREIRA[ind_bar] != ' ')
		{
			atual->blocos[ind_bloc].forma = BARREIRA[ind_bar];
			atual->blocos[ind_bloc].estado = 1;
			atual->blocos[ind_bloc].lin = ind_bar / ncol;
			atual->blocos[ind_bloc].col = ind_bar % ncol;
			ind_bloc++;
		}
	atual->tam = ind_bloc;
}

void define_barreiras(entidades *tabuleiro)
{
	int cont;
	for (cont = 0; cont < 4; cont++)
		define_barreira(cont, tabuleiro);
}

void define_canhao(entidades *tabuleiro)
{
	tabuleiro->jogador.alt = 2;
	tabuleiro->jogador.larg = strlen(CANHAO) / 2;
	strcpy(tabuleiro->jogador.forma, CANHAO);
}

void define_forma_inicial(entidades *tabuleiro)
{
	tabuleiro->forma = 0;
}

void define_entidades(entidades *tabuleiro)
{
	define_forma_inicial(tabuleiro);
	define_naves(tabuleiro);
	define_barreiras(tabuleiro);
	define_canhao(tabuleiro);
}

/* FUNCOES DE IMPRESSAO */

void imprime_borda()
{
	int i, j;
	
	for (i = 0; i < 38; i += 37)
		for (j = 0; j < 100; j++)
			{
				move(i, j);
				addch('-');
			}

	for (i = 1; i < 37; i++)
		for (j = 0; j < 100; j += 99)
		{
			move(i, j);
			addch('|');
		}
}

void imprime_alien(int lin, int col, int tipo, entidades *tabuleiro)
{
	int i, j, nlin, ncol;
	char *forma_atual;
	
	if (!tabuleiro->forma)
		forma_atual = tabuleiro->naves[tipo].forma1;
	else
		forma_atual = tabuleiro->naves[tipo].forma2;
	
	nlin = tabuleiro->naves[tipo].alt;
	ncol = tabuleiro->naves[tipo].larg;
	for (i = 0; i < nlin; i++)
		for (j = 0; j < tabuleiro->naves[tipo].larg; j++)
			{
				move(lin+i, col+j);
				addch(forma_atual[ncol*i + j]);
			}
}

void imprime_barreira(int lin, int col, int num_bar, entidades *tabuleiro)
{
	int i, tam;
	bloco atual;
	
	tam = tabuleiro->barreiras[num_bar].tam;
	for (i = 0; i < tam; i++)
	{
		atual = tabuleiro->barreiras[num_bar].blocos[i];
		move(lin + atual.lin, col + atual.col);
		addch(atual.forma);
	}
}

void imprime_canhao(int lin, int col, entidades *tabuleiro)
{
	int i, j;
	canhao *can;
	
	can = &(tabuleiro->jogador);
	for (i = 0; i < can->alt; i++)
		for (j = 0; j < can->larg; j++)
		{
			move(lin+i, col+j);
			addch(can->forma[can->larg*i + j]);
		}
}

void imprime_tela(t_lista *l_tela, entidades *tabuleiro)
{
	int tipo, lin, col, vel, estado, cont_bar = 0, i;

	clear();
	imprime_borda();

	inicializa_atual_inicio(l_tela);
	for (i = 0; i < l_tela->tamanho; i++)
	{
		consulta_item_atual(&tipo, &lin, &col, &vel, &estado, l_tela);

		if (tipo <= 3)
			imprime_alien(lin, col, tipo, tabuleiro);
		else if (tipo == 4)
			imprime_barreira(lin, col, cont_bar++, tabuleiro);
		else if (tipo == 5)
			imprime_canhao(lin, col, tabuleiro);

		incrementa_atual(l_tela);
	}
	refresh();
}

void move_aliens(t_lista *l_tela)
{
	int maiorcol, menorcol;
	
	inicializa_atual_inicio(l_tela);
	while(t_lista->atual.tipo < 1 || t_lista->atual.tipo > 3)
		incrementa_atual(l_tela);
	
	while(t_lista->atual.tipo >= 1 && t_lista->atual.tipo <= 3)	
	{
		maiorcol
		incrementa_atual(l_tela);
	}
}

int main()
{

	t_lista l_tela;
	entidades tabuleiro;
	int iter = 1;

	inicializa_lista_tela( &l_tela );
	define_entidades(&tabuleiro);
	
	if (!inicializa_tela())
	{
		printf("Sua tela tem que ter pelo menos 38 linhas e 100 colunas!\n");
		exit(1);
	}

	while(1)
	{
		imprime_tela(&l_tela, &tabuleiro);
		
		if (iter % 100 == 0)
			move_aliens(t_lista *l_tela);
		
		iter++;
		usleep(INTERVALO);
	}
	endwin();

	destroi_lista( &l_tela );
	return 0;
}
