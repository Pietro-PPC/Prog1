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

/*
  insere nave mae no fim da lista desejada
*/
int insere_nave_mae(t_lista *l_do_mal);

/*
  insere todos os aliens no fim da lista desejada
*/
int insere_aliens( t_jogo *jogo );

/*
  insere todos os blocos de uma barreira no fim da lista desejada
*/
int insere_barreira(int lin, int col, t_lista *l_do_bem);

/*
  insere varias barreiras no fim da lista desejada
*/
int insere_barreiras(t_lista *l_do_bem);

/*
  insere canhao no fim da lista desejada
*/
int insere_canhao(t_lista *l_do_bem);

/*
  insere todos os elementos necessarios nas listas do jogo
*/
int inicializa_listas_iniciais( t_jogo *jogo );

/* FUNCOES DE IMPRESSAO */

/*
  imprime a borda do jogo na tela do ncurses
*/
void imprime_borda();

/*
  imprime o objeto do tipo especificado na linha e coluna especificadas
*/
void imprime_objeto(int tipo, int lin, int col, int estado, t_jogo *jogo);

/*
  imprime todos os elementos vivos do jogo
*/
void imprime_tela(t_jogo *jogo);

/* funcoes de inicializacao */

/*
  Atribui strings definidas no inicio do programa a descricao da nave mae,
  alem da sua altura e largura
*/
void descreve_nave_mae(t_individuo *nave_mae);

/*
  Atribui strings definidas no inicio do programa a descricao do alien 1,
  alem da sua altura e largura
*/
void descreve_alien_1(t_individuo *alien);

/*
  Atribui strings definidas no inicio do programa a descricao do alien 2,
  alem da sua altura e largura
*/
void descreve_alien_2(t_individuo *alien);

/*
  Atribui strings definidas no inicio do programa a descricao do alien 3,
  alem da sua altura e largura
*/
void descreve_alien_3(t_individuo *alien);

/*
  Atribui strings definidas no inicio do programa a descricao do bloco tipo 1,
  alem da sua altura e largura
*/
void descreve_bloco_1(t_individuo *bloco);

/*
  Atribui strings definidas no inicio do programa a descricao do bloco tipo 2,
  alem da sua altura e largura
*/
void descreve_bloco_2(t_individuo *bloco);

/*
  Atribui strings definidas no inicio do programa a descricao do canhao,
  alem da sua altura e largura
*/
void descreve_canhao(t_individuo *canhao);

/*
  Atribui strings definidas no inicio do programa a descricao do tiro do canhao,
  alem da sua altura e largura
*/
void descreve_tiro(t_individuo *tiro);

/*
  Atribui strings definidas no inicio do programa a descricao da bomba,
  alem da sua altura e largura
*/
void descreve_bomba(t_individuo *bomba);

/*
  Chama todas as funcoes acima para descricao dos tipos.
*/
void descreve_tipos(t_individuo *descricao_tipo);

/*
  Inicializa o apontador atual nos aliens para lista de seres do mal
*/
void inicializa_atual_aliens(t_lista *l_do_mal);

/*
  Acha a maior linha e coluna e a menor coluna que contem aliens para auxilio
  na sua movimentacao
*/
void max_min(t_jogo *jogo);

/*
  Aloca o vetor de descricao dos tipos 
*/
int inicializa_vetor_descricao(t_jogo *jogo);

/*
  Inicializa todas as variaveis que devem ser inicializadas antes do inicio do jogo
*/
int inicializa_jogo(t_jogo *jogo);

/*
  Reinicializa todas as variaveis necessarias apos o jogador matar todos os aliens 
  de certa fase
*/
int reinicializa_jogo(t_jogo *jogo);

/*
  Inicializa a tela do ncurses com todas as configuracoes necessarias
*/
int inicializa_tela();

/* FUNCOES DE ATUALIZACAO */

/*
  Soma os pontos pela morte de um alien ou nave mae do tipo passado por parametro
*/
void soma_pontos(int *pontos, int tipo);

/*
  Retorna 1 se ha interseccao entre os intervalos a-max_a e b-max_b
  caso contrario retorna 0
*/
int interseccao(int a, int max_a, int b, int max_b);

/*
  Retorna 1 se ha colisao entre elementos dos tipos especificados e linha e coluna especificadas
*/
int colisao(int tipo_a, int lin_a, int col_a, int tipo_b, int lin_b, int col_b, t_jogo *jogo);

/*
  Atualiza o sentido em que os aliens vao de acordo com sua posicao na tela
*/
void atualiza_sentido_aliens(t_jogo *jogo);

/*
  Move o alien pela tela de acordo com seu sentido e velocidade
*/
void move_alien(int *lin, int *col, int *vel, int sentido);

/*
  Testa se algum dos tiros atingiu o alien
*/
int levou_tiro_alien(int tipo_a, int lin_a, int col_a, t_jogo *jogo);

/*
  Chama todas as funcoes que atualizam sentido, posicao, velocidade e estado dos
  aliens
*/
void atualiza_aliens(t_jogo *jogo);

/*
  Movimenta canhao para a esquerda
*/
void move_canhao_esquerda(t_jogo *jogo);

/*
  Movimenta canhao para a direita
*/
void move_canhao_direita(t_jogo *jogo);

/*
  Insere um tiro em cima do canhao
  Retorna 0 se o tiro nao pode ser inserido. Caso contrario, retorna 1
*/
int atira(t_jogo *jogo);

/*
  Atualiza posicao do tiro ou o remove se ele chegou ao final da tela
*/
void atualiza_tiro(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo);

/*
  Atualiza posicao da bomba ou a remove se ela chegou ao final da tela
*/
void atualiza_bomba(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo);

/*
  Atualiza posicao de tiros e bombas
*/
void atualiza_projeteis(t_jogo *jogo);

/*
  Retorna 1 se a barreira levou um tiro ou uma bomba
  Retorna 0 caso contrario
*/
int morreu_barreira(int tipo_b, int lin_b, int col_b, t_jogo *jogo);

/*
  Atualiza o estado das barreiras de acordo com a situacao atual do jogo
*/
void atualiza_barreiras(t_jogo *jogo);

/*
  Insere uma bomba na posicao especificada
*/
int solta_bomba(int tipo, int lin, int col, t_jogo *jogo);

/*
  Escolhe aleatoriamente alienigenas para soltarem bombas
*/
int solta_bombas(t_jogo *jogo);

/*
  Testa se o canhao levou uma bomba ou encostou em um alien
*/
int canhao_morreu(t_jogo *jogo);

/*
  Testa se algum alien chegou no chao
*/
int aliens_chao(t_jogo *jogo);

/*
  Testa se o jogo acabou ou pela morte do canhao ou por algum alien conseguir pousar
*/
int fim_jogo(t_jogo *jogo);

/*
  Testa se a nave mae esta viva
*/
int nave_mae_viva(t_jogo *jogo);

/*
  Insere a nave mae no canto superior esquerdo da tela
*/
void invoca_nave_mae(t_jogo *jogo);

/*
  Testa se a nave mae morreu por um tiro
*/
int morreu_nave_mae(int tipo_n, int lin_n, int col_n, t_jogo *jogo);

/*
  Atualiza o local da nave mae e seu estado.
*/
void atualiza_nave_mae(t_jogo *jogo);
