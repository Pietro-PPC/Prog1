#include <stdio.h>
#include <stdlib.h>
#include "lib_lista.h"

int inicializa_lista(t_lista *l)
{
    t_nodo *sent_ini, *sent_fim;

    sent_ini = (t_nodo*) malloc(sizeof(t_nodo));
    sent_fim = (t_nodo*) malloc(sizeof(t_nodo));
    if (sent_ini == NULL || sent_fim == NULL)
    {
        free(sent_ini);
        free(sent_fim);
        return 0;
    }

    sent_ini->prox = sent_fim;
    sent_ini->prev = NULL;
    sent_fim->prox = NULL;
    sent_fim->prev = sent_ini;

    l->ini = sent_ini;
    l->fim = sent_fim;
    l->atual = NULL;
    l->tamanho = 0;
    return 1;
}

int lista_vazia(t_lista *l)
{
    return !(l->tamanho);
}

void destroi_lista(t_lista *l)
{
    t_nodo *p;
    while (!lista_vazia(l))
    {
        p = l->ini->prox;

        p->prev->prox = p->prox;
        p->prox->prev = p->prev;
        p->prox = NULL;
        p->prev = NULL;

        free(p);
        l->tamanho--;
    }
    l->ini->prox = NULL;
    l->fim->prev = NULL;
    free(l->ini);
    free(l->fim);
}

int insere_inicio_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l)
{
    t_nodo *novo;

    novo = (t_nodo*) malloc(sizeof(t_nodo));
    if (novo == NULL)
    {
        free(novo);
        return 0;
    }

    novo->tipo = tipo;
    novo->coord[0] = lin;
    novo->coord[1] = col;
    novo->vel = vel;
    novo->estado = estado;
    novo->prev = l->ini;
    novo->prox = l->ini->prox;

    novo->prev->prox = novo;
    novo->prox->prev = novo;
    l->tamanho++;

    return 1;
}

int tamanho_lista(t_lista *l)
{
    return l->tamanho;
}

int insere_fim_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l)
{
    t_nodo *novo;

    novo = (t_nodo*) malloc(sizeof(t_nodo));
    if (novo == NULL)
    {
        free(novo);
        return 0;
    }

    novo->tipo = tipo;
    novo->coord[0] = lin;
    novo->coord[1] = col;
    novo->vel = vel;
    novo->estado = estado;
    novo->prox = l->fim;
    novo->prev = l->fim->prev;

    novo->prox->prev = novo;
    novo->prev->prox = novo;
    l->tamanho++;
    
    return 1;
}

int remove_inicio_lista(t_lista *l)
{
    t_nodo *p;
    if(lista_vazia(l))
        return 0;

    p = l->ini->prox;
    p->prev->prox = p->prox;
    p->prox->prev = p->prev;

    p->prev = NULL;
    p->prox = NULL;
    free(p);
    l->tamanho--;

    return 1;
}

int remove_fim_lista(t_lista *l)
{
    t_nodo *p;
    if (lista_vazia(l))
        return 0;

    p = l->fim->prev;
    p->prev->prox = p->prox;
    p->prox->prev = p->prev;

    p->prox = NULL;
    p->prev = NULL;
    free(p);
    l->tamanho--;

    return 1;
}

int inicializa_atual_inicio(t_lista *l)
{
    if (lista_vazia(l))
        return 0;

    l->atual = l->ini->prox;
    return 1;
}

int inicializa_atual_fim(t_lista *l)
{
    if (lista_vazia(l))
        return 0;
    
    l->atual = l->fim->prev;
    return 1;
}

void incrementa_atual(t_lista *l)
{
    if (l->atual != NULL)
    {
        if (l->atual->prox == l->fim)
            l->atual = NULL;
        else 
            l->atual = l->atual->prox;
    }
}

void decrementa_atual(t_lista *l)
{
    if (l->atual != NULL)
    {
        if (l->atual->prev == l->ini)
            l->atual = NULL;
        else
            l->atual = l->atual->prev;
    }
}

int consulta_item_atual(int *tipo, int *lin, int *col, int *vel, int *estado, t_lista *l)
{
    /* As outras funcoes garantem que o atual nao aponte para sentinelas */
    if (l->atual == NULL)
        return 0;
    
    *tipo = l->atual->tipo;
    *lin = l->atual->coord[0];
    *col = l->atual->coord[1];
    *vel = l->atual->vel;
    *estado = l->atual->estado;

    return 1;
}

int modifica_item_atual(int tipo, int lin, int col, int vel, int estado, t_lista *l)
{
    if (l->atual == NULL)
        return 0;

    l->atual->tipo = tipo;
    l->atual->coord[0] = lin;
    l->atual->coord[1] = col;
    l->atual->vel = vel;
    l->atual->estado = estado;

    return 1;
}

int remove_item_atual(t_lista *l)
{
    if (l->atual == NULL)
        return 0;

    l->atual->prox->prev = l->atual->prev;
    l->atual->prev->prox = l->atual->prox;

    l->atual->prev = NULL;
    l->atual->prox = NULL;
    free(l->atual);
    l->tamanho--;

    return 1;
}
