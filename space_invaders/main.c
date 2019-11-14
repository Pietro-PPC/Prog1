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
#define BLOCOS     "AM"
#define CANHAO     " /^\\ MMMMM"

#define EXPLOSAO_3X3 "\\'/- -/,\\"
#define EXPLOSAO_3X5 " \\'/ -   - /,\\ " 
#define EXPLOSAO_3X7 " \\ ' / -     - / , \\ "
#define EXPLOSAO_1X1 "X"
#define EXPLOSAO_2X5 "\\ ' // , \\"

#define ALTURA_BARREIRA 3
#define ALTURA_ALIENS 3
#define ALTURA_CANHAO 2
#define ALTURA_BLOCO 1

#define INTERVALO 50000


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

/* AUTOMATIZAR DETECÇÃO DE LETRAS? */
int insere_barreira(int lin, int col, t_lista *l_tela)
{
    int i, ncol, l_atual, c_atual;

    ncol = strlen(BARREIRA) / 3;
    for (i = 0; i < strlen(BARREIRA); i++)
    {
        l_atual = lin + (i / ncol);
        c_atual = col + (i % ncol);
        if (BARREIRA[i] == 'A')
        {
            if (!insere_fim_lista(4, l_atual, c_atual, 0, 1, l_tela))
                return 0;
        }
        else if (BARREIRA[i] == 'M')
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

void atribui_desenho_alien(char *desenho, int tipo, int estado)
{
    if (estado == 0)
    {
        if (tipo == 0)
            strcpy(desenho, NAVE_MAE_1);
        else if (tipo == 1)
            strcpy(desenho, ALIEN_1_1);
        else if (tipo == 2)
            strcpy(desenho, ALIEN_2_1);
        else /* (tipo == 3) */
            strcpy(desenho, ALIEN_3_1);
    }
    else if (estado == 1)
    {
        if (tipo == 0)
            strcpy(desenho, NAVE_MAE_2);
        else if (tipo == 1)
            strcpy(desenho, ALIEN_1_2);
        else if (tipo == 2)
            strcpy(desenho, ALIEN_2_2);
        else /* (tipo == 3) */
            strcpy(desenho, ALIEN_3_2);
    }
    else if (estado == 2)
    {
        if (tipo == 0)
            strcpy(desenho, EXPLOSAO_3X7);
        else if (tipo == 1)
            strcpy(desenho, EXPLOSAO_3X3);
        else /* (tipo <= 3) */
            strcpy(desenho, EXPLOSAO_3X5); 
    }
}

void atribui_desenho_barreira(char *desenho, int tipo, int estado)
{
    char str_bloco[2];
    
    if (estado != 2)
        if (tipo == 4)
        {
            str_bloco[0] = BLOCOS[0];
            str_bloco[1] = '\0';
            strcpy(desenho, str_bloco);
        }
        else /* (tipo == 5) */
        {
            str_bloco[0] = BLOCOS[1];
            str_bloco[1] = '\0';
            strcpy(desenho, str_bloco);
        }

    else /* (estado == 2) */
        strcpy(desenho, EXPLOSAO_1X1);
}

void atribui_desenho_canhao(char *desenho, int estado)
{
    if (estado != 2)
        strcpy(desenho, CANHAO);
    else /* (estado == 2) */
        strcpy(desenho, EXPLOSAO_2X5);
}

void atribui_desenho(char *desenho, int tipo, int estado)
{
    if (tipo <= 3)
        atribui_desenho_alien(desenho, tipo, estado);
    else if (tipo <= 5)
        atribui_desenho_barreira(desenho, tipo, estado);
    else /* (tipo == 6) */
        atribui_desenho_canhao(desenho, estado);
}

int altura_desenho(int tipo)
{
    if (tipo <= 3)
        return ALTURA_ALIENS;
    if (tipo <= 5)
        return ALTURA_BLOCO;
    if (tipo == 6)
        return ALTURA_CANHAO;
    return 0;
}

void imprime_objeto(int tipo, int lin, int col, int estado)
{
    int i, l_atual, c_atual, alt, larg;
    char desenho[28];
    
    atribui_desenho(desenho, tipo, estado);
    
    alt = altura_desenho(tipo);
    larg = strlen(desenho) / alt;
    for (i = 0; i < strlen(desenho); i++)
    {
        l_atual = lin + (i / larg);
        c_atual = col + (i % larg);
        move(l_atual, c_atual);
        addch(desenho[i]);
    }
}

void imprime_tela(t_lista *l_tela)
{
    int tipo, lin, col, vel, estado, i;

    clear();
    imprime_borda();
    
    inicializa_atual_inicio(l_tela);
    for (i = 0; i < l_tela->tamanho; i++)
    {
        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, l_tela);
        imprime_objeto(tipo, lin, col, estado);
        incrementa_atual(l_tela);
    }
    refresh();
}

void alterna_aliens(t_lista *l_tela)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_inicio(l_tela);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, l_tela);
    while (tipo <= 3)
    {
        if (estado < 2)
            modifica_item_atual(tipo, lin, col, vel, !estado, l_tela);
        incrementa_atual(l_tela);
        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, l_tela);
    }
}

int main()
{
    t_lista l_tela;
    int iter = 1;

    inicializa_lista_tela( &l_tela );
    
    if (!inicializa_tela())
    {
        printf("Sua tela tem que ter pelo menos 38 linhas e 100 colunas!\n");
        exit(1);
    }

    while(1)
    {
        if ( iter % 10 == 0 )
        {
            alterna_aliens(&l_tela);
        }

        imprime_tela(&l_tela);

        iter++;
        usleep(INTERVALO);
    }
    endwin();

    destroi_lista( &l_tela );
    return 0;
}
