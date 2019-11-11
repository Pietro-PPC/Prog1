#include <stdio.h>

typedef struct t_aviao{
	int id;
	int autonomia;
} t_aviao;

typedef struct t_nodo{
	struct t_nodo *prev;
	struct t_nodo *prox;
	t_aviao chave;
} t_nodo;

typedef struct tipo_fila{
	t_nodo *ini;
	t_nodo *atual;
	t_nodo *fim;
	int tam;
} tipo_fila;

/*
	Inicializa a fila. Se o processo for feito com sucesso, retorna 1.
	caso contrário, retorna 0.
*/
int inicializa_fila(tipo_fila *F);

/*
	Se a fila estiver vazia, retorna 1. Caso contario, retorna 0.
*/
int fila_vazia(tipo_fila *F);

/*
	Caso a fila nao esteja vazia, a funcao retorna o seu tamanho. Caso
	contrario, retorna -1.
*/
int tamanho(tipo_fila *F);

/*
	Enfileira um elemento de ID id e tempo de autonomia t. Retorna 1
	se o item foi enfileirado com sucesso. Caso contrario, retorna 0.
*/
int enfileira(int id, int t, tipo_fila *F);

/*
	Desenfileira o primeiro elemento da fila e retorna o seu ID em id e
	o tempo de autonomia em t. Caso nao puder ser desenfileirado um elemento,
	a funcao retorna 0.
*/
int desenfileira(int *id, int *t, tipo_fila *F);

/*
	Remove o elemento de tempo de autonomia t da fila. O ID do elemento eh
	retornado em id e a funcao retorna 1. Caso nao haja nenhum elemento de 
	autonomia t na fila, a funcao retorna 0.
*/
int remove_fila(int *id, int t, tipo_fila *F);

/*
	Imprime a fila. No caso de algum elemento ter tempo de autonomia 
	menor que 0, a funcao imprime apenas o ID do elemento.
*/
void imprime_fila(tipo_fila *F);

int inicializa_atual_inicio(tipo_fila *F);

void incrementa_atual(tipo_fila *F);

int consulta_atual(int *id, int *t, tipo_fila *F);

int altera_atual(int id, int t, tipo_fila *F);
