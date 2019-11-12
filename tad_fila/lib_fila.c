#include <stdio.h>
#include <stdlib.h>
#include "lib_fila.h"

int inicializa_fila(tipo_fila *F)
{
	t_nodo *sent_ini, *sent_fim;

	sent_ini = (t_nodo*) malloc(sizeof(t_nodo));
	sent_fim = (t_nodo*) malloc(sizeof(t_nodo));

	if (sent_ini == NULL || sent_fim == NULL)
	{
		if (sent_fim != NULL)
			free(sent_fim);
		else if (sent_ini != NULL)
			free(sent_ini);
		return 0;
	}

	sent_ini->prox = sent_fim;
	sent_ini->prev = NULL;
	sent_fim->prox = NULL;
	sent_fim->prev = sent_ini;

	F->ini = sent_ini;
	F->atual = NULL;
	F->fim = sent_fim;
	F->tam = 0;

	return 1;
}

int fila_vazia(tipo_fila *F)
{
	return !(F->tam);
}

int tamanho(tipo_fila *F)
{
	return F->tam;
}

int enfileira(int id, int t, tipo_fila *F)
{
	t_nodo *novo;

	novo = (t_nodo*) malloc(sizeof(t_nodo));
	if (novo == NULL)
		return 0;
	
	novo->chave.id = id;
	novo->chave.autonomia = t;
	novo->chave.espera = 0;
	novo->prox = F->fim;
	novo->prev = F->fim->prev;

	novo->prev->prox = novo;
	novo->prox->prev = novo;
	F->tam++;

	return 1;
}

int desenfileira(int *id, int *t, tipo_fila *F)
{
	t_nodo *p;
	if (fila_vazia(F))
		return 0;

	p = F->ini->prox;
	p->prev->prox = p->prox;
	p->prox->prev = p->prev;

	*id = p->chave.id;
	*t = p->chave.autonomia;
	p->prox = NULL;
	p->prev = NULL;
	free(p);
	F->tam--;

	return 1;
}

int remove_fila(int *id, int t, tipo_fila *F)
{
	t_nodo *p;

	if (fila_vazia(F))
		return 0;

	p = F->ini->prox;
	F->fim->chave.autonomia = t;
	while ( t != p->chave.autonomia )
		p = p->prox;
	
	if (p == F->fim)
		return 0;
	
	p->prev->prox = p->prox;
	p->prox->prev = p->prev;
	p->prev = NULL;
	p->prox = NULL;

	*id = p->chave.id;
	free(p);
	F->tam--;

	return 1;	
}

void imprime_fila(tipo_fila *F)
{
	t_nodo *p;

	p = F->ini->prox;
	while (p != F->fim)
	{
		printf("%d", p->chave.id);
		if (p->chave.autonomia != -1)
			printf("(%d)", p->chave.autonomia);
		if (p->prox != F->fim)
			printf(", ");
		p = p->prox;
	}
}

/* FUNCOES EXTRAS */

int inicializa_atual_inicio(tipo_fila *F)
{
	if (fila_vazia(F))
		return 0;

	F->atual = F->ini->prox;
	return 1;	
}

void incrementa_atual(tipo_fila *F)
{
	if (F != NULL)
	{
		if (F->atual->prox == F->fim)
			F->atual = NULL;
		else
			F->atual = F->atual->prox;
	}
}

int consulta_atual (int *id, int *t, tipo_fila *F)
{
	if (F->atual == NULL)
		return 0;
	
	*id = F->atual->chave.id;
	*t  = F->atual->chave.autonomia;
	return 1;
}

int altera_atual (int id, int t, tipo_fila *F)
{
	if (F->atual == NULL)
		return 0;

	F->atual->chave.id = id;
	F->atual->chave.autonomia = t;
	return 1;
}
