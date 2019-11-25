#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "lib_lista.h"

#define NAVE_MAE_1 " /MMMMM\\ AMoMoMoMA \\/'-'\\/ "
#define NAVE_MAE_2 " /MMMMM\\ AoMoMoMoA \\/'-'\\/ "
#define ALIEN_1_1  " A AMA/X\\"
#define ALIEN_1_2  " A AMA{ }"
#define ALIEN_2_1  ".v_v.}MWM{ / \\ "
#define ALIEN_2_2  ".v_v.}MWM{ } { "
#define ALIEN_3_1  " nmn dbMdb_/-\\_"
#define ALIEN_3_2  " nmn dbMdb |-| "
#define BARREIRA   " AMMMA AMMMMMAMM   MM"
#define BLOCOS     "AM"
#define CANHAO     " /^\\ MMMMM"
#define TIRO       "|"
#define BOMBA      "$"

#define EXPLOSAO_3X9 " \\  '  / - -   - - /  ,  \\ "
#define EXPLOSAO_3X5 " \\'/ -   - /,\\ " 
#define EXPLOSAO_3X3 "\\'/- -/,\\"
#define EXPLOSAO_2X5 "\\ ' // , \\"
#define EXPLOSAO_1X1 "X"

#define ALTURA_BARREIRA 3
#define ALTURA_ALIENS 3
#define ALTURA_CANHAO 2
#define ALTURA_BLOCO 1

#define MAX_TAM_ALIEN 28
#define VEL_INICIAL 14

#define ALTURA_TELA 38
#define LARGURA_TELA 100
#define VEL_MAX 20

#define INTERVALO 50000
/* DEFINES PARA TIPOS */

typedef struct s_individuo
{
    int alt;
    int larg;
    char estado[3][MAX_TAM_ALIEN];
} t_individuo;

typedef struct s_info
{
    int sentido;
    int sentido_ant;
    int quant;
    int max_col;
    int min_col;
    int max_lin;
} t_info;

typedef struct s_jogo
{
    int iter;
    int n_tiros;
    int pontos;
    int fase;
    t_info aliens;
    t_individuo *descricao_tipo;
    t_lista l_do_mal;
    t_lista l_do_bem;
    t_lista l_projeteis;
} t_jogo;

/* Funcoes de inicializacao da lista de elementos */

int insere_nave_mae(t_lista *l_do_mal);

int insere_aliens( t_jogo *jogo );

int insere_barreira(int lin, int col, t_lista *l_do_bem);

int insere_barreiras(t_lista *l_do_bem);

int insere_canhao(t_lista *l_do_bem);

int inicializa_listas_iniciais( t_jogo *jogo );

/* FUNCOES DE IMPRESSAO */

void imprime_borda();

void imprime_objeto(int tipo, int lin, int col, int estado, t_jogo *jogo);

void imprime_tela(t_jogo *jogo);

/* funcoes de inicializacao */

void descreve_nave_mae(t_individuo *nave_mae);

void descreve_alien_1(t_individuo *alien);

void descreve_alien_2(t_individuo *alien);

void descreve_alien_3(t_individuo *alien);

void descreve_bloco_1(t_individuo *bloco);

void descreve_bloco_2(t_individuo *bloco);

void descreve_canhao(t_individuo *canhao);

void descreve_tiro(t_individuo *tiro);

void descreve_bomba(t_individuo *bomba);

void descreve_tipos(t_individuo *descricao_tipo);

void inicializa_atual_aliens(t_lista *l_do_mal);

void max_min(t_jogo *jogo);

int inicializa_vetor_descricao(t_jogo *jogo);

int inicializa_jogo(t_jogo *jogo);

int reinicializa_jogo(t_jogo *jogo);

int inicializa_tela();

/* funcoes de atualizacao */

void soma_pontos(int *pontos, int tipo);

int interseccao(int a, int max_a, int b, int max_b);

int colisao(int tipo_a, int lin_a, int col_a, int tipo_b, int lin_b, int col_b, t_jogo *jogo);

void atualiza_sentido_aliens(t_jogo *jogo);

void move_alien(int *lin, int *col, int *vel, int sentido);

int levou_tiro_alien(int tipo_a, int lin_a, int col_a, t_jogo *jogo);


void atualiza_aliens(t_jogo *jogo);

void move_canhao_esquerda(t_jogo *jogo);

void move_canhao_direita(t_jogo *jogo);

int atira(t_jogo *jogo);

void atualiza_tiro(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo);

void atualiza_bomba(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo);

void atualiza_projeteis(t_jogo *jogo);

int morreu_barreira(int tipo_b, int lin_b, int col_b, t_jogo *jogo);

void atualiza_barreiras(t_jogo *jogo);

int solta_bomba(int tipo, int lin, int col, t_jogo *jogo);

int solta_bombas(t_jogo *jogo);

int canhao_morreu(t_jogo *jogo);

int aliens_chao(t_jogo *jogo);

int fim_jogo(t_jogo *jogo);

int nave_mae_viva(t_jogo *jogo);

void invoca_nave_mae(t_jogo *jogo);

int morreu_nave_mae(int tipo_n, int lin_n, int col_n, t_jogo *jogo);

void atualiza_nave_mae(t_jogo *jogo);
