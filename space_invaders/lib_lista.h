/*
  Feito por Marcos Castiho para a disciplina CI1001 - Programacao I
  Em 18/10/2019.
  Define a estrutura de dados para o Tipo Abstrato de Dados Lista
  como sendo uma lista duplamente encadeada com sentinelas no inicio
  e no final. A cabeca da lista tem ponteiros para o inicio, fim e
  para um elemento qualquer da lista, alem do tamanho da lista.
  Contem os prototipos publicos das funcoes que manipulam o TAD.
*/

/*
  tipo:
    0 - nave_mae
    1 - alien_1
    2 - alien_2
    3 - alien_3
    4 - barreira_1
    5 - barreira_2
    6 - canhao
  coord:
      coordenadas do canto superior esquerdo do objeto
  vel:
      velocidade do elemento
  estado:
    0 - vivo 1
    1 - vivo 2
    2 - morrendo
*/

struct t_nodo {
    int tipo;
    int coord[2];
    int vel;
    int estado;
    struct t_nodo *prox;
    struct t_nodo *prev;
};
typedef struct t_nodo t_nodo;

struct t_lista {
    t_nodo *ini;
    t_nodo *atual;
    t_nodo *fim;
    int tamanho;
};
typedef struct t_lista t_lista;

/*
  Cria uma lista vazia. Ela eh duplamente encadeada e tem sentinelas no
  inicio e no final. Tambem tem um apontador para um elemento qualquer.
*/
int inicializa_lista(t_lista *l); /* OK */

/*
  Retorna 1 se a lista está vazia e zero caso contrário.
*/
int lista_vazia(t_lista *l); /* OK */

/*
  Remove todos os elementos da lista e faz com que ela aponte para NULL.
*/
void destroi_lista(t_lista *l); /* OK */

/*
  Insere o elemento item no início da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_inicio_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l); /* OK */

/*
  Retorna o tamanho da lista em *tam.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int tamanho_lista(t_lista *l); /* OK */

/*
  Insere o elemento item no final da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_fim_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l); /* OK */

/*
  Remove o primeiro elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_inicio_lista(t_lista *l); /* OK */

/*
  Remove o último elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_fim_lista(t_lista *l); /* OK */

/* 
  Inicializa o ponteiro atual para o primeiro elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_inicio(t_lista *l); /* OK */

/* 
  Inicializa o ponteiro atual para o ultimo elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_fim(t_lista *l); /* OK */

/*
  Faz o ponteiro atual apontar para o próximo nodo da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o último, isto é, não 
  tem próximo, retorna NULL.
*/
void incrementa_atual(t_lista *l); /* OK */

/*
  Faz o ponteiro atual apontar para o nodo anterior da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o primeiro, isto é, não 
  tem anterior, retorna NULL.
*/
void decrementa_atual(t_lista *l); /* OK */

/*
  Retorna em *item o valor contido na chave apontada pelo ponteiro atual. 
  Se atual não for válido a função retorna zero senão retorna 1.
*/
int consulta_item_atual(int *tipo, int *lin, int *col, int *vel, int *estado, t_lista *l); /* OK */


int modifica_item_atual(int tipo, int lin, int col, int vel, int estado, t_lista *l);

/*
  Remove o elemento apontado por atual da lista l e o retorna em *item.
  Faz o atual apontar para o sucessor do nodo removido.
  Retorna 1 se houve sucesso e zero caso contrário.
*/
int remove_item_atual(t_lista *l); /* OK */
