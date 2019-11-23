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
    int quant;
    int max_col;
    int min_col;
    int max_lin;
} t_info;

typedef struct s_jogo
{
    int iter;
    int tiros;
    t_info aliens;
    t_individuo descricao_tipo[9];
    t_lista l_do_mal;
    t_lista l_do_bem;
    t_lista l_projeteis;
} t_jogo;

/* Funcoes de inicializacao da lista de elementos */

int insere_nave_mae(t_lista *l_do_mal)
{
    if (!insere_fim_lista(0, 3, 99, 19, 3, l_do_mal)) 
        return 0;

    return 1;
}

int insere_aliens( t_jogo *jogo )
{
    int i, j;
    
    for (i = 0; i < 11; i++)
    {
        if (!insere_fim_lista(1, 7, (5 + 7*i), 11, 1, &jogo->l_do_mal)) 
            return 0;
        (jogo->aliens.quant)++;
    }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
        {
            if (!insere_fim_lista(2, (11 + 4*i), (4 + 7*j), 11, 1, &jogo->l_do_mal)) 
                return 0;
            (jogo->aliens.quant)++;
        }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
        {
            if (!insere_fim_lista(3, (19 + 4*i), (4 + 7*j), 11, 1, &jogo->l_do_mal))
                return 0;
            (jogo->aliens.quant)++;
        }

    return 1;
}

int insere_barreira(int lin, int col, t_lista *l_do_bem)
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
            if (!insere_fim_lista(4, l_atual, c_atual, 0, 1, l_do_bem))
                return 0;
        }
        else if (BARREIRA[i] == BLOCOS[1])
        {
            if (!insere_fim_lista(5, l_atual, c_atual, 0, 1, l_do_bem))
                return 0;
        }
    }

    return 1;
}

int insere_barreiras(t_lista *l_do_bem)
{
    int i;
    for (i = 0; i < 4; i++)
        if (!insere_barreira(31, (15 + 21*i), l_do_bem))
            return 0;

    return 1;
}

int insere_canhao(t_lista *l_do_bem)
{
    if (!insere_fim_lista(6, 35, 43, 20, 1, l_do_bem))
        return 0;
    return 1;
}

int inicializa_listas_iniciais( t_jogo *jogo )
{
    if (!inicializa_lista (&jogo->l_do_mal)) return 0;
    if (!inicializa_lista (&jogo->l_do_bem)) return 0;
    
    if (!insere_nave_mae (&jogo->l_do_mal))  return 0;
    if (!insere_aliens (jogo))               return 0;
    if (!insere_barreiras (&jogo->l_do_bem)) return 0;
    if (!insere_canhao (&jogo->l_do_bem))    return 0;

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

int imprime_objeto(int tipo, int lin, int col, int estado, t_jogo *jogo)
{
    int l_atual, c_atual, larg;
    unsigned int i;
    char *desenho;
    
    if (estado == 3)
        return 0;
    desenho = jogo->descricao_tipo[tipo].estado[estado];
    larg =    jogo->descricao_tipo[tipo].larg;
    for (i = 0; i < strlen(desenho); i++)
    {
        l_atual = lin + (i / larg);
        c_atual = col + (i % larg);
        if (c_atual > 0 && c_atual < 99 )
        {
            move(l_atual, c_atual);
            addch(desenho[i]);
        }
    }
    return 1;
}

void imprime_tela(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    clear();
    imprime_borda();
    
    inicializa_atual_inicio(&jogo->l_do_bem);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem))
    {
        imprime_objeto(tipo, lin, col, estado, jogo);
        incrementa_atual(&jogo->l_do_bem);
    }
    
    inicializa_atual_inicio(&jogo->l_do_mal);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal))
    {
        imprime_objeto(tipo, lin, col, estado, jogo);
        incrementa_atual(&jogo->l_do_mal);
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

void max_min(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado, alt, larg;

    inicializa_atual_inicio(&jogo->l_do_mal);
    incrementa_atual(&jogo->l_do_mal);  /* primeiro item eh nave mae */
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);

    larg = jogo->descricao_tipo[tipo].larg;
    alt = jogo->descricao_tipo[tipo].alt;
    jogo->aliens.max_col = col + larg-1;
    jogo->aliens.min_col = col;
    jogo->aliens.max_lin = lin + alt-1;

    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal))
    {
        larg = jogo->descricao_tipo[tipo].larg;
        if (col + larg-1 > jogo->aliens.max_col)
            jogo->aliens.max_col = col + larg-1;
        else if (col < jogo->aliens.min_col)
            jogo->aliens.min_col = col;

        if (lin + alt-1 > jogo->aliens.max_lin)
            jogo->aliens.max_lin = lin + alt-1;
        
        incrementa_atual(&jogo->l_do_mal);
    }
}

int inicializa_jogo(t_jogo *jogo)
{
    jogo->aliens.quant = 0;
    
    if(!inicializa_listas_iniciais(jogo) ||
       !inicializa_lista(&jogo->l_projeteis)   )
    {
        printf("Listas nao inicializadas com sucesso!\n");
        return 0;
    }

    jogo->aliens.sentido = 1;
    max_min(jogo);

    descreve_tipos(jogo->descricao_tipo);
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

int levou_tiro_alien(int tipo_a, int lin_a, int col_a, t_jogo *jogo)
{
    int tipo_p, lin_p, col_p, vel_p, estado_p, alt, larg;

    alt = jogo->descricao_tipo[tipo_a].alt;
    larg = jogo->descricao_tipo[tipo_a].larg;
    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (lin_a <= lin_p && (lin_a + alt) > lin_p && 
            col_a <= col_p && (col_a + larg) > col_p && 
            tipo_p == 7)
        {
            remove_item_atual(&jogo->l_projeteis);
            return 1;
        }
        incrementa_atual(&jogo->l_projeteis);
    }
    return 0;
}

void inicializa_atual_aliens(t_lista *l_do_mal)
{
    inicializa_atual_inicio(l_do_mal);
    incrementa_atual(l_do_mal);
}

void atualiza_aliens(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    int moveu;
   
    inicializa_atual_aliens(&jogo->l_do_mal);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);
    moveu = (jogo->iter % (21 - vel) == 0);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal))
    {
        if (moveu && estado < 2)
        {
            estado = !estado;
            move_alien(&lin, &col, &vel, jogo->aliens.sentido);
        }
        
        if (estado == 2)
        {
            remove_item_atual(&jogo->l_do_mal);
            (jogo->aliens.quant)--;
        }
        else
        {
            if (levou_tiro_alien(tipo, lin, col, jogo))
                estado = 2;
            modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_mal);
            incrementa_atual(&jogo->l_do_mal);
        }
    }

    if (moveu)
    {
        max_min(jogo);
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
    
    inicializa_atual_fim(&jogo->l_do_bem);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    if (col > 1)
        col--;
    modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_bem);
}

void move_canhao_direita(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    
    inicializa_atual_fim(&jogo->l_do_bem);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    if (col + jogo->descricao_tipo[tipo].larg - 1 < 98)
        col++;
    modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_bem);
}

int atira(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_fim(&jogo->l_do_bem);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    if (!insere_fim_lista(7, lin-1, col+2, 20, 1, &jogo->l_projeteis))
        return 0;
    return 1;
}

void atualiza_tiro(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo)
{
    if (lin == 1)
        remove_item_atual(&jogo->l_projeteis);

    else if (jogo->iter % (21 - vel) == 0)
    {
        lin--;
        modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_projeteis);
    }
}

void atualiza_bomba(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo)
{
    if (lin == 36)
        remove_item_atual(&jogo->l_projeteis);
 
    else if (jogo->iter % (21 - vel) == 0)
    {
        lin++;
        modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_projeteis);
    }
}

void atualiza_projeteis(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_projeteis))
    {
        if (tipo == 7)
            atualiza_tiro(tipo, lin, col, vel, estado, jogo);
        else if (tipo == 8)
            atualiza_bomba(tipo, lin, col, vel, estado, jogo);

        incrementa_atual(&jogo->l_projeteis);
    }
}

int colisao_alien_barreira(int tipo_a, int lin_a, int col_a, int lin_b, int col_b, t_jogo *jogo)
{
    int alt, larg;

    alt = jogo->descricao_tipo[tipo_a].alt;
    larg = jogo->descricao_tipo[tipo_a].larg;

    if ( lin_b >= lin_a && lin_b < lin_a + alt  &&  
         col_b >= col_a && col_b < col_a + larg   )
        return 1;
    return 0;
}

int morreu_barreira(int lin_b, int col_b, t_jogo *jogo)
{
    int tipo_p, lin_p, col_p, vel_p, estado_p;
    int tipo_a, lin_a, col_a, vel_a, estado_a;
    
    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (lin_p == lin_b && col_p == col_b)
        {
            remove_item_atual(&jogo->l_projeteis);
            return 1;
        }
        incrementa_atual(&jogo->l_projeteis);
    }

    inicializa_atual_aliens(&jogo->l_do_mal);
    while (consulta_item_atual(&tipo_a, &lin_a, &col_a, &vel_a, &estado_a, &jogo->l_do_mal))
    {
        if (colisao_alien_barreira(tipo_a, lin_a, col_a, lin_b, col_b, jogo))
            return 1;
        incrementa_atual(&jogo->l_do_mal);
    }

    return 0;
}

void atualiza_barreiras(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    
    inicializa_atual_inicio(&jogo->l_do_bem);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    while (tipo <= 5)
    {
        if (estado == 2)
            remove_item_atual(&jogo->l_do_bem);
        else if (morreu_barreira(lin, col, jogo))
        {
            estado = 2;
            modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_bem);
            incrementa_atual(&jogo->l_do_bem);
        }
        else
            incrementa_atual(&jogo->l_do_bem);

        consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    }
}

int solta_bomba(int tipo, int lin, int col, t_jogo *jogo)
{
    int alt, larg;
    
    alt = jogo->descricao_tipo[tipo].alt;
    larg = jogo->descricao_tipo[tipo].larg;
    if (!insere_fim_lista(8, lin + alt, col + larg/2, 15, 1, &jogo->l_projeteis))
        return 0;
    return 1;
}

int solta_bombas(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    int aliens, num;

    aliens = jogo->aliens.quant;
    inicializa_atual_aliens(&jogo->l_do_mal);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);

    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal))
    {
        num = rand();
        if (num % (40*aliens) == 0)
            if (!solta_bomba(tipo, lin, col, jogo))
                return 0; 
        incrementa_atual(&jogo->l_do_mal);
    }
    return 1;
}

int colisao_canhao_bomba(int lin_p, int col_p, int lin_c, int col_c, int tipo_c, t_jogo *jogo)
{
    int alt, larg;
    
    alt  = jogo->descricao_tipo[tipo_c].alt;
    larg = jogo->descricao_tipo[tipo_c].larg;
    
    if (lin_p >= lin_c && lin_p  <  lin_c + alt  &&
        col_p >= col_c && col_p  <  col_c + larg    )
        return 1;

    return 0;
}

int interseccao(int a, int max_a, int b, int max_b)
{
    if (a <= b)
        return (b <= max_a);
    else /* (b < a) */
        return (a <= max_b);
}

int colisao_canhao_alien(int tipo_a, int lin_a, int col_a, int tipo_c, int lin_c, int col_c, t_jogo *jogo)
{
    int linmax_a, linmax_c, colmax_a, colmax_c;

    linmax_a = lin_a + jogo->descricao_tipo[tipo_a].alt - 1;
    linmax_c = lin_c + jogo->descricao_tipo[tipo_c].alt - 1;
    colmax_a = col_a + jogo->descricao_tipo[tipo_a].larg - 1;
    colmax_c = col_c + jogo->descricao_tipo[tipo_c].larg - 1;
    
    if ( interseccao(lin_a, linmax_a, lin_c, linmax_c) &&
         interseccao(col_a, colmax_a, col_c, colmax_c)   )
         return 1;
    return 0;
}

int canhao_morreu(t_jogo *jogo)
{
    int tipo_a, lin_a, col_a, vel_a, estado_a;
    int tipo_c, lin_c, col_c, vel_c, estado_c;
    int tipo_p, lin_p, col_p, vel_p, estado_p;

    inicializa_atual_fim (&jogo->l_do_bem);
    consulta_item_atual (&tipo_c, &lin_c, &col_c, &vel_c, &estado_c, &jogo->l_do_bem);

    inicializa_atual_inicio (&jogo->l_projeteis);
    while (consulta_item_atual (&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (tipo_p == 8)
            if (colisao_canhao_bomba(lin_p, col_p, lin_c, col_c, tipo_c, jogo))
            {
                remove_item_atual(&jogo->l_projeteis);
                estado_c = 2;
                modifica_item_atual(tipo_c, lin_c, col_c, vel_c, estado_c, &jogo->l_do_bem);
                return 1;
            }
        incrementa_atual(&jogo->l_projeteis);
    }

    if (jogo->aliens.max_col)
    {
        inicializa_atual_aliens(&jogo->l_do_mal);
        while (consulta_item_atual(&tipo_a, &lin_a, &col_a, &vel_a, &estado_a, &jogo->l_do_mal))
        {
            if (colisao_canhao_alien(tipo_a, lin_a, col_a, tipo_c, lin_c, col_c, jogo))
            {
                estado_c = 2;
                modifica_item_atual(tipo_c, lin_c, col_c, vel_c, estado_c, &jogo->l_do_bem);
                return 1;
            }
            incrementa_atual(&jogo->l_do_mal);
        }
    }
    return 0;
}

int aliens_chao(t_jogo *jogo)
{
    return jogo->aliens.max_lin >= 35; 
}

int fim_jogo(t_jogo *jogo)
{
    if (canhao_morreu(jogo))
        return 1;
    if (aliens_chao(jogo))
        return 1;
    return 0;
}

int nave_mae_viva(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    
    inicializa_atual_inicio(&jogo->l_do_mal);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);
    return (estado != 3);
}

void invoca_nave_mae(t_jogo *jogo)
{
    inicializa_atual_inicio(&jogo->l_do_mal);
    modifica_item_atual(0, 3, -7, 19, 1, &jogo->l_do_mal);
}

int morreu_nave_mae(int tipo_n, int lin_n, int col_n, t_jogo *jogo)
{
    int tipo_p, lin_p, col_p, vel_p, estado_p;

    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (tipo_p == 7)
            if (colisao_canhao_alien(tipo_n, lin_n, col_n, tipo_p, lin_p, col_p, jogo))
            {
                remove_item_atual(&jogo->l_projeteis);
                return 1;
            }

        incrementa_atual(&jogo->l_projeteis);
    }
    return 0;
}

void atualiza_nave_mae(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado, moveu;
    
    inicializa_atual_inicio(&jogo->l_do_mal);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);
    
    if (estado != 3)
    {
        moveu = (jogo->iter % (21 - vel ) == 0);
        if (moveu && estado < 2)
        {
            estado = !estado;
            col++;
        }

        if (estado == 2)
            col = 99;
        else if (morreu_nave_mae(tipo, lin, col, jogo))
            estado = 2;

        if (col > 98)
            estado = 3;

        modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_mal);
    }
}

int main()
{
    t_jogo jogo;
    int tecla, alea;
    
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
    
    srand(time(NULL));

    tecla = getch();
    while(tecla != 'q' && jogo.aliens.quant)
    {
        imprime_tela(&jogo);
        
        atualiza_projeteis(&jogo);

        atualiza_nave_mae(&jogo);
        if (!nave_mae_viva(&jogo))
        {
            alea = rand() % 500;
            if (alea == 0)
                invoca_nave_mae(&jogo);
        }

        atualiza_aliens(&jogo);
        atualiza_barreiras(&jogo);
      
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

        if (!solta_bombas(&jogo))
        {
            printf("Espaco limite excedido! jogo abortado.");
            exit(1);
        }

        if (fim_jogo(&jogo))
        {
            imprime_tela(&jogo);
            while(tecla != 'q')
                tecla = getch();
        }

        jogo.iter++;
        usleep(INTERVALO);
    }
    endwin();

    destroi_lista( &jogo.l_do_bem );
    destroi_lista( &jogo.l_do_mal );
    destroi_lista( &jogo.l_projeteis );
    return 0;
}
