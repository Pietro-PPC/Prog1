#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
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
#define BOMBA      "@"

#define EXPLOSAO_3X7 " \\ ' / -     - / , \\ "
#define EXPLOSAO_3X5 " \\'/ -   - /,\\ " 
#define EXPLOSAO_3X3 "\\'/- -/,\\"
#define EXPLOSAO_2X5 "\\ ' // , \\"
#define EXPLOSAO_1X1 "X"

#define ALTURA_BARREIRA 3
#define ALTURA_ALIENS 3
#define ALTURA_CANHAO 2
#define ALTURA_BLOCO 1

#define MAX_TAM_ALIEN 28

#define INTERVALO 40000
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
    int max_col;
    int min_col;
} t_info;

typedef struct s_jogo
{
    int iter;
    t_info aliens;
    t_individuo descricao_tipo[9];
    t_lista l_tela;
    t_lista l_projeteis;
} t_jogo;

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
        if (!insere_fim_lista(1, 7, (5 + 7*i), 10, 1, l_tela)) 
            return 0;
    
    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
            if (!insere_fim_lista(2, (11 + 4*i), (4 + 7*j), 10, 1, l_tela)) 
                return 0;

    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
            if (!insere_fim_lista(3, (19 + 4*i), (4 + 7*j), 10, 1, l_tela))
                return 0;
    
    return 1;
}

int insere_barreira(int lin, int col, t_lista *l_tela)
{
    int l_atual, c_atual;
    unsigned int i, ncol;

    ncol = strlen(BARREIRA) / ALTURA_BARREIRA;
    for (i = 0; i < strlen(BARREIRA); i++)
    {
        l_atual = lin + (i / ncol);
        c_atual = col + (i % ncol);
        
        if (BARREIRA[i] == BLOCOS[0])
        {
            if (!insere_fim_lista(4, l_atual, c_atual, 0, 1, l_tela))
                return 0;
        }
        else if (BARREIRA[i] == BLOCOS[1])
        {
            if (!insere_fim_lista(5, l_atual, c_atual, 0, 1, l_tela))
                return 0;
        }
    }

    return 1;
}

int insere_barreiras(t_lista *l_tela)
{
    int i;
    for (i = 0; i < 4; i++)
        if (!insere_barreira(31, (15 + 21*i), l_tela))
            return 0;

    return 1;
}

int insere_canhao(t_lista *l_tela)
{
    if (!insere_fim_lista(6, 35, 43, 0, 1, l_tela))
        return 0;
    return 1;
}

int inicializa_lista_tela( t_lista *l_tela )
{
    if (!inicializa_lista (l_tela)) return 0;
    
    if (!insere_nave_mae (l_tela)) return 0;
    if (!insere_aliens (l_tela)) return 0;
    if (!insere_barreiras (l_tela)) return 0;
    if (!insere_canhao (l_tela)) return 0;

    return 1;
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

void imprime_objeto(int tipo, int lin, int col, int estado, t_jogo *jogo)
{
    int l_atual, c_atual, larg;
    unsigned int i;
    char *desenho;
    
    desenho = jogo->descricao_tipo[tipo].estado[estado];
    larg =    jogo->descricao_tipo[tipo].larg;
    for (i = 0; i < strlen(desenho); i++)
    {
        l_atual = lin + (i / larg);
        c_atual = col + (i % larg);
        move(l_atual, c_atual);
        addch(desenho[i]);
    }
}

void imprime_tela(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    clear();
    imprime_borda();
    
    inicializa_atual_inicio(&jogo->l_tela);
    while(consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela))
    {
        imprime_objeto(tipo, lin, col, estado, jogo);
        incrementa_atual(&jogo->l_tela);
    }
    
    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_projeteis))
    { 
        imprime_objeto(tipo, lin, col, estado, jogo);
        incrementa_atual(&jogo->l_projeteis);
    }
    
    refresh();
}

/* funcoes de inicializacao */

void descreve_nave_mae(t_individuo *nave_mae)
{
    strcpy(nave_mae->estado[0], NAVE_MAE_1);
    strcpy(nave_mae->estado[1], NAVE_MAE_2);
    strcpy(nave_mae->estado[2], EXPLOSAO_3X7);
    nave_mae->alt = ALTURA_ALIENS;
    nave_mae->larg = strlen(NAVE_MAE_1) / ALTURA_ALIENS;
}

void descreve_alien_1(t_individuo *alien)
{     
    strcpy(alien->estado[0], ALIEN_1_1);
    strcpy(alien->estado[1], ALIEN_1_2);
    strcpy(alien->estado[2], EXPLOSAO_3X3);
    alien->alt = ALTURA_ALIENS;
    alien->larg = strlen(ALIEN_1_1) / ALTURA_ALIENS;
}

void descreve_alien_2(t_individuo *alien)
{
    strcpy(alien->estado[0], ALIEN_2_1);
    strcpy(alien->estado[1], ALIEN_2_2);
    strcpy(alien->estado[2], EXPLOSAO_3X5);
    alien->alt = ALTURA_ALIENS;
    alien->larg = strlen(ALIEN_2_1) / ALTURA_ALIENS;
}

void descreve_alien_3(t_individuo *alien)
{
    strcpy(alien->estado[0], ALIEN_3_1);
    strcpy(alien->estado[1], ALIEN_3_2);
    strcpy(alien->estado[2], EXPLOSAO_3X5);
    alien->alt = ALTURA_ALIENS;
    alien->larg = strlen(ALIEN_3_1) / ALTURA_ALIENS;
}

void descreve_bloco_1(t_individuo *bloco)
{
    char str_bloco[2];

    str_bloco[0] = BLOCOS[0];
    str_bloco[1] = '\0';
    strcpy(bloco->estado[0], str_bloco);
    strcpy(bloco->estado[1], str_bloco);
    strcpy(bloco->estado[2], EXPLOSAO_1X1);
    bloco->alt = 1;
    bloco->larg = 1;
}

void descreve_bloco_2(t_individuo *bloco)
{
    char str_bloco[2];

    str_bloco[0] = BLOCOS[1];
    str_bloco[1] = '\0';
    strcpy(bloco->estado[0], str_bloco);
    strcpy(bloco->estado[1], str_bloco);
    strcpy(bloco->estado[2], EXPLOSAO_1X1);
    bloco->alt = 1;
    bloco->larg = 1;
}

void descreve_canhao(t_individuo *canhao)
{
    strcpy(canhao->estado[0], CANHAO);
    strcpy(canhao->estado[1], CANHAO);
    strcpy(canhao->estado[2], EXPLOSAO_2X5);
    canhao->alt = ALTURA_CANHAO;
    canhao->larg = strlen(CANHAO) / ALTURA_CANHAO;
}

void descreve_tiro(t_individuo *tiro)
{
    strcpy(tiro->estado[0], TIRO);
    strcpy(tiro->estado[1], TIRO);
    tiro->alt = 1;
    tiro->larg = 1;
}

void descreve_bomba(t_individuo *bomba)
{
    strcpy(bomba->estado[0], BOMBA);
    strcpy(bomba->estado[1], BOMBA);
    bomba->alt = 1;
    bomba->larg = 1;
}

void descreve_tipos(t_individuo *descricao_tipo)
{
    descreve_nave_mae (&descricao_tipo[0]);
    descreve_alien_1  (&descricao_tipo[1]);
    descreve_alien_2  (&descricao_tipo[2]);
    descreve_alien_3  (&descricao_tipo[3]);
    descreve_bloco_1  (&descricao_tipo[4]);
    descreve_bloco_2  (&descricao_tipo[5]);
    descreve_canhao   (&descricao_tipo[6]);
    descreve_tiro     (&descricao_tipo[7]);
    descreve_bomba    (&descricao_tipo[8]);
}

void max_min_col(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado, larg;

    inicializa_atual_inicio(&jogo->l_tela);
    incrementa_atual(&jogo->l_tela);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);

    larg = jogo->descricao_tipo[tipo].larg;
    jogo->aliens.max_col = col + larg-1;
    jogo->aliens.min_col = col;

    while(tipo <= 3)
    {
        larg = jogo->descricao_tipo[tipo].larg;
        if (col + larg-1 > jogo->aliens.max_col)
            jogo->aliens.max_col = col + larg-1;
        else if (col < jogo->aliens.min_col)
            jogo->aliens.min_col = col;
        
        incrementa_atual(&jogo->l_tela);
        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    }
}

int inicializa_jogo(t_jogo *jogo)
{
    if(!inicializa_lista_tela(&jogo->l_tela) ||
       !inicializa_lista(&jogo->l_projeteis)   )
    {
        printf("Listas nao inicializadas com sucesso!\n");
        return 0;
    }

    descreve_tipos(jogo->descricao_tipo);

    jogo->aliens.sentido = 1;
    max_min_col(jogo);

    jogo->iter = 1;

    return 1;
}

/* funcoes de atualizacao */
void atualiza_sentido_aliens(t_jogo *jogo)
{
    if ((jogo->aliens.sentido == 1  && jogo->aliens.max_col >= 98) ||
        (jogo->aliens.sentido == -1 && jogo->aliens.min_col <= 1)    )
        jogo->aliens.sentido = 0;

    else if (jogo->aliens.sentido == 0 && jogo->aliens.max_col >= 98)
        jogo->aliens.sentido = -1;
    
    else if (jogo->aliens.sentido == 0 && jogo->aliens.min_col <= 1)
        jogo->aliens.sentido = 1;
}

void move_alien(int *lin, int *col, int *vel, int sentido)
{    
    if (sentido == 0)
    {
        (*lin)++;
        if (*vel < 20)
            (*vel)++;
    }
    else
        *col += sentido;
}

void atualiza_aliens(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    if (jogo->iter % (21 - vel) == 0)
    {
        while (tipo <= 3)
        {
            if (estado < 2)
                estado = !estado;            
            move_alien(&lin, &col, &vel, jogo->aliens.sentido);

            modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_tela);
            incrementa_atual(&jogo->l_tela);
            consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
        }
        max_min_col(jogo);
        atualiza_sentido_aliens(jogo);
    }    
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

void move_canhao_esquerda(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
   
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    while (tipo != 6)
    {
        incrementa_atual(&jogo->l_tela);
        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    }

    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    if (col > 1)
        col--;
    modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_tela);
}

void move_canhao_direita(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela); 
    while (tipo != 6)
    {
        incrementa_atual(&jogo->l_tela);
        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    }

    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    if (col + jogo->descricao_tipo[tipo].larg - 1 < 98)
        col++;
    modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_tela);
}

int atira(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_fim(&jogo->l_tela);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_tela);
    if (!insere_fim_lista(7, lin-1, col+2, 15, 1, &jogo->l_projeteis))
        return 0;
    return 1;
}

void atualiza_projeteis(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_projeteis))
    {
        if (lin == 1)
            remove_item_atual(&jogo->l_projeteis);
        else
        {
            lin--;
            modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_projeteis);
        }

        incrementa_atual(&jogo->l_projeteis);
    }
}

int main()
{
    t_jogo jogo;
    int tecla;

    if (!inicializa_jogo( &jogo ))
    {
        printf("Jogo nao inicializado com sucesso!\n");
        exit(1);
    }

    if (!inicializa_tela())
    {
        printf("Sua tela tem que ter pelo menos 38 linhas e 100 colunas!\n");
        exit(1);
    }
    
    tecla = getch();
    while(tecla != 'q')
    {
        imprime_tela(&jogo);
        inicializa_atual_inicio(&jogo.l_tela);
        incrementa_atual(&jogo.l_tela);
        
        atualiza_projeteis(&jogo);
        atualiza_aliens(&jogo);
/*      atualiza_barreiras(&jogo);
*/      
        tecla = getch();
        if (tecla == KEY_LEFT)
            move_canhao_esquerda(&jogo);
        else if (tecla == KEY_RIGHT)
            move_canhao_direita(&jogo);
        else if (tecla == ' ')
        {
            if (!atira(&jogo))
            {
                printf("Espaco limite excedido! Jogo abortado.");
                exit(1);
            }
        }

        jogo.iter++;
        usleep(INTERVALO);
    }
    endwin();

    destroi_lista( &jogo.l_tela );
    return 0;
}
