#include "lib_invaders.h"

/* Funcoes de inicializacao da lista de elementos */

int insere_nave_mae(t_lista *l_do_mal)
{
    if (!insere_fim_lista(0, 3, LARGURA_TELA, 20, 3, l_do_mal)) 
        return 0;

    return 1;
}

int insere_aliens( t_jogo *jogo )
{
    int i, j, vel;
    
    if (VEL_INICIAL + 1*jogo->fase <= 20)
        vel = VEL_INICIAL + 1*jogo->fase;
    else 
        vel = 20;

    for (i = 0; i < 11; i++)
    {
        if (!insere_fim_lista(1, 7, (5 + 7*i), vel, 1, &jogo->l_do_mal)) 
            return 0;
        (jogo->aliens.quant)++;
    }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
        {
            if (!insere_fim_lista(2, (11 + 4*i), (4 + 7*j), vel, 1, &jogo->l_do_mal)) 
                return 0;
            (jogo->aliens.quant)++;
        }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 11; j++)
        {
            if (!insere_fim_lista(3, (19 + 4*i), (4 + 7*j), vel, 1, &jogo->l_do_mal))
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
    if (!inicializa_lista(&jogo->l_projeteis)) return 0;
    if (!inicializa_lista (&jogo->l_do_mal))   return 0;
    if (!inicializa_lista (&jogo->l_do_bem))   return 0;
    
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
    
    for (i = 0; i < ALTURA_TELA; i += ALTURA_TELA-1)
        for (j = 0; j < LARGURA_TELA; j++)
            {
                move(i, j);
                addch('-');
            }

    for (i = 1; i < ALTURA_TELA; i++)
        for (j = 0; j < LARGURA_TELA; j += LARGURA_TELA-1)
        {
            move(i, j);
            addch('|');
        }
}

void imprime_objeto(int tipo, int lin, int col, int estado, t_jogo *jogo)
{
    int l_atual, c_atual, larg;
    unsigned int i;
    char desenho[MAX_TAM_ALIEN];
    
    if (estado != 3)
    {
        strcpy(desenho, jogo->descricao_tipo[tipo].estado[estado]);
        larg = jogo->descricao_tipo[tipo].larg;
        for (i = 0; i < strlen(desenho); i++)
        {
            l_atual = lin + (i / larg);
            c_atual = col + (i % larg);
            if (c_atual > 0 && c_atual < LARGURA_TELA-1 )
            {
                move(l_atual, c_atual);
                addch(desenho[i]);
            }
        }
    }
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
    mvprintw(1, 42, "Pontuacao: %d", jogo->pontos);

    refresh();
}

/* funcoes de inicializacao */

void descreve_nave_mae(t_individuo *nave_mae)
{
    strcpy(nave_mae->estado[0], NAVE_MAE_1);
    strcpy(nave_mae->estado[1], NAVE_MAE_2);
    strcpy(nave_mae->estado[2], EXPLOSAO_3X9);
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

void inicializa_atual_aliens(t_lista *l_do_mal)
{
    inicializa_atual_inicio(l_do_mal);
    incrementa_atual(l_do_mal);
}

void max_min(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado, alt, larg;

    inicializa_atual_aliens(&jogo->l_do_mal);
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

int inicializa_vetor_descricao(t_jogo *jogo)
{
    jogo->descricao_tipo = malloc(9 * sizeof(t_individuo));
    if (jogo->descricao_tipo == NULL)
        return 0;

    return 1;
}

int inicializa_jogo(t_jogo *jogo)
{
    jogo->aliens.quant = 0;
    jogo->fase = 0;
    jogo->n_tiros = 0;
    jogo->pontos = 0;
    
    if (!inicializa_vetor_descricao(jogo))
    {
        printf("Vetor de descricao nao inicializado com sucesso!\n");
        return 0;
    }

    if (!inicializa_listas_iniciais(jogo))
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

int reinicializa_jogo(t_jogo *jogo)
{
    destroi_lista( &jogo->l_do_bem );
    destroi_lista( &jogo->l_do_mal );
    destroi_lista( &jogo->l_projeteis );

    jogo->aliens.quant = 0;
    jogo->n_tiros = 0;
    jogo->fase++;
    
    if (!inicializa_listas_iniciais(jogo))
    {
        printf("Listas nao inicializadas com sucesso!\n");
        return 0;
    }

    jogo->aliens.sentido = 1;
    max_min(jogo);
    jogo->iter = 1;

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
    if (nlin < ALTURA_TELA || ncol < LARGURA_TELA)
    {
        endwin();
        return 0;
    }

    return 1;
}

/* funcoes de atualizacao */

void soma_pontos(int *pontos, int tipo)
{
    if (tipo == 0)
        *pontos += 50;
    else if (tipo == 1)
        *pontos += 10;
    else if (tipo == 2)
        *pontos += 5;
    else if (tipo == 3)
        *pontos += 3;
}

int interseccao(int a, int max_a, int b, int max_b)
{
    if (a <= b)
        return (b <= max_a);
    else /* (b < a) */
        return (a <= max_b);
}

int colisao(int tipo_a, int lin_a, int col_a, int tipo_b, int lin_b, int col_b, t_jogo *jogo)
{
    int linmax_a, linmax_b, colmax_a, colmax_b;

    linmax_a = lin_a + jogo->descricao_tipo[tipo_a].alt - 1;
    linmax_b = lin_b + jogo->descricao_tipo[tipo_b].alt - 1;
    colmax_a = col_a + jogo->descricao_tipo[tipo_a].larg - 1;
    colmax_b = col_b + jogo->descricao_tipo[tipo_b].larg - 1;
    
    if ( interseccao(lin_a, linmax_a, lin_b, linmax_b) &&
         interseccao(col_a, colmax_a, col_b, colmax_b)   )
         return 1;
    return 0;
}


void atualiza_sentido_aliens(t_jogo *jogo)
{
    
    if ((jogo->aliens.sentido == 1  && jogo->aliens.max_col >= LARGURA_TELA-2) ||
        (jogo->aliens.sentido == -1 && jogo->aliens.min_col <= 1)    )
    {
        jogo->aliens.sentido_ant = jogo->aliens.sentido;
        jogo->aliens.sentido = 0;
    }
    else if (jogo->aliens.sentido == 0 && jogo->aliens.sentido_ant == 1)
    {
        jogo->aliens.sentido_ant = jogo->aliens.sentido;
        jogo->aliens.sentido = -1;
    }
    else if (jogo->aliens.sentido == 0 && jogo->aliens.sentido_ant == -1)
    {
        jogo->aliens.sentido_ant = jogo->aliens.sentido;
        jogo->aliens.sentido = 1;
    }
    else
        jogo->aliens.sentido_ant = jogo->aliens.sentido;
        
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
    int tipo_p, lin_p, col_p, vel_p, estado_p;

    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (tipo_p == 7)
            if ( colisao(tipo_a, lin_a, col_a, tipo_p, lin_p, col_p, jogo))
            {
                remove_item_atual(&jogo->l_projeteis);
                jogo->n_tiros--;
                return 1;
            }
        incrementa_atual(&jogo->l_projeteis);
    }
    return 0;
}


void atualiza_aliens(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;
    int moveu;
   
    inicializa_atual_aliens(&jogo->l_do_mal);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal);
    moveu = (jogo->iter % (VEL_MAX+1 - vel) == 0);
    while (consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_mal))
    {
        if (moveu && estado < 2)
        {
            estado = !estado;
            move_alien(&lin, &col, &vel, jogo->aliens.sentido);
        }
        
        if (estado == 2)
        {
            remove_item_atual (&jogo->l_do_mal);
            soma_pontos (&jogo->pontos, tipo);
        }
        else
        {
            if (levou_tiro_alien(tipo, lin, col, jogo))
            {
                (jogo->aliens.quant)--;
                estado = 2;
            }
            modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_mal);
            incrementa_atual(&jogo->l_do_mal);
        }
    } 

    max_min(jogo);
    if (moveu)
        atualiza_sentido_aliens(jogo);
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
    if (col + jogo->descricao_tipo[tipo].larg - 1 < LARGURA_TELA-2)
        col++;
    modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_bem);
}

int atira(t_jogo *jogo)
{
    int tipo, lin, col, vel, estado;

    inicializa_atual_fim(&jogo->l_do_bem);
    consulta_item_atual(&tipo, &lin, &col, &vel, &estado, &jogo->l_do_bem);
    if (jogo->n_tiros < 3)
    {
        if (!insere_fim_lista(7, lin-1, col+2, 20, 1, &jogo->l_projeteis))
            return 0;
        jogo->n_tiros++;
    }
    return 1;
}

void atualiza_tiro(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo)
{
    if (lin == 1)
    {
        remove_item_atual(&jogo->l_projeteis);
        jogo->n_tiros--;
    }

    else if (jogo->iter % (VEL_MAX+1 - vel) == 0)
    {
        lin--;
        modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_projeteis);
    }
}

void atualiza_bomba(int tipo, int lin, int col, int vel, int estado, t_jogo *jogo)
{
    if (lin == ALTURA_TELA-2)
        remove_item_atual(&jogo->l_projeteis);
 
    else if (jogo->iter % (VEL_MAX+1 - vel) == 0)
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

int morreu_barreira(int tipo_b, int lin_b, int col_b, t_jogo *jogo)
{
    int tipo_p, lin_p, col_p, vel_p, estado_p;
    int tipo_a, lin_a, col_a, vel_a, estado_a;
    
    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (colisao(tipo_b, lin_b, col_b, tipo_p, lin_p, col_p, jogo))
        {
            remove_item_atual(&jogo->l_projeteis);
            if (tipo_p == 7)
                jogo->n_tiros--;
            return 1;
        }
        incrementa_atual(&jogo->l_projeteis);
    }

    inicializa_atual_aliens(&jogo->l_do_mal);
    while (consulta_item_atual(&tipo_a, &lin_a, &col_a, &vel_a, &estado_a, &jogo->l_do_mal))
    {
        if (colisao(tipo_a, lin_a, col_a, tipo_b, lin_b, col_b, jogo))
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
        else if (morreu_barreira(tipo, lin, col, jogo))
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

    aliens = jogo->aliens.quant+1;
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
            if (colisao(tipo_p, lin_p, col_p, tipo_c, lin_c, col_c, jogo))
            {
                remove_item_atual(&jogo->l_projeteis);
                estado_c = 2;
                modifica_item_atual(tipo_c, lin_c, col_c, vel_c, estado_c, &jogo->l_do_bem);
                return 1;
            }
        incrementa_atual(&jogo->l_projeteis);
    }

    inicializa_atual_aliens(&jogo->l_do_mal);
    while (consulta_item_atual(&tipo_a, &lin_a, &col_a, &vel_a, &estado_a, &jogo->l_do_mal))
    {
        if (colisao(tipo_a, lin_a, col_a, tipo_c, lin_c, col_c, jogo))
        {
            estado_c = 2;
            modifica_item_atual(tipo_c, lin_c, col_c, vel_c, estado_c, &jogo->l_do_bem);
            return 1;
        }
        incrementa_atual(&jogo->l_do_mal);
    }
    return 0;
}

int aliens_chao(t_jogo *jogo)
{
    return jogo->aliens.max_lin >= ALTURA_TELA-3; 
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
    modifica_item_atual(0, 3, -7, 20, 1, &jogo->l_do_mal);
}

int morreu_nave_mae(int tipo_n, int lin_n, int col_n, t_jogo *jogo)
{
    int tipo_p, lin_p, col_p, vel_p, estado_p;

    inicializa_atual_inicio(&jogo->l_projeteis);
    while (consulta_item_atual(&tipo_p, &lin_p, &col_p, &vel_p, &estado_p, &jogo->l_projeteis))
    {
        if (tipo_p == 7)
            if (colisao(tipo_n, lin_n, col_n, tipo_p, lin_p, col_p, jogo))
            {
                remove_item_atual(&jogo->l_projeteis);
                jogo->n_tiros--;
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
        moveu = (jogo->iter % (VEL_MAX+1 - vel ) == 0);
        if (moveu && estado < 2)
        {
            estado = !estado;
            col++;
        }

        if (estado == 2)
        {
            soma_pontos (&jogo->pontos, tipo);
            col = LARGURA_TELA-1;
        }
        else if (morreu_nave_mae(tipo, lin, col, jogo))
            estado = 2;

        if (col > LARGURA_TELA-2)
            estado = 3;

        modifica_item_atual(tipo, lin, col, vel, estado, &jogo->l_do_mal);
    }
}
