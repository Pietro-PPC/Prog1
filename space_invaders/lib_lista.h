/*
  Feito por Pietro Polinari Cavassin, adaptacao da biblioteca criada por
  Marcos Castiho para a disciplina CI1001 - Programacao I.

  Define a estrutura para o TAD lista duplamente encadeada, cujos
  nodos possuem os dados dos elementos da tela para a construcao do 
  space invaders.
  A lista possui ponteiros para o inicio, fim, e um elemento atual, 
  que pode transitar pela lista conforme o uso das funcoes.
  Nesse arquivo estao os prototipos das funcoes da biblioteca.
*/

/*
  DESCRICAO DOS ELEMENTOS DE UM NODO:
  tipo:
    0 - nave_mae
    1 - alien_1
    2 - alien_2
    3 - alien_3
    4 - barreira_1
    5 - barreira_2
    6 - canhao
    7 - tiro
    8 - bomba
  coord:
      coordenadas do canto superior esquerdo do objeto
  vel:
      velocidade do elemento
  estado:
    0 - vivo 1
    1 - vivo 2
    2 - morrendo
    3 - morto (apenas para nave_mae)
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
  Cria uma lista e aloca suas sentinelas.
  Retorna 1 se a alocacao foi bem sucedida ou 0 caso contrario
*/
int inicializa_lista(t_lista *l);

/*
  Retorna 1 se a lista estiver vazia e 0 caso contrario
*/
int lista_vazia(t_lista *l);

/*
  Remove elementos da lista e aponta inicio e fim para NULL
*/
void destroi_lista(t_lista *l); 

/*
  Insere dados especificados no inicio da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int insere_inicio_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l);

/*
  Retorna o tamanho da lista
*/
int tamanho_lista(t_lista *l); 

/*
  Insere dados especificados no final da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int insere_fim_lista(int tipo, int lin, int col, int vel, int estado, t_lista *l); 

/*
  Remove o primeiro elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int remove_inicio_lista(t_lista *l); 

/*
  Remove o último elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int remove_fim_lista(t_lista *l); 

/* 
  Inicializa o ponteiro atual no primeiro elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int inicializa_atual_inicio(t_lista *l);

/* 
  Inicializa o ponteiro atual no ultimo elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrario.
*/
int inicializa_atual_fim(t_lista *l); 

/*
  Faz o ponteiro atual apontar para o próximo nodo da lista. 
  Se atual não tem próximo, retorna NULL.
*/
void incrementa_atual(t_lista *l);

/*
  Faz o ponteiro atual apontar para o nodo anterior da lista l e retorna 
  este ponteiro. Se atual não tem anterior, retorna NULL.
*/
void decrementa_atual(t_lista *l);

/*
  Retorna os valores contidos no nodo apontado pelo ponteiro atual. 
  Se atual não for válido a função retorna 0, senao retorna 1.
*/
int consulta_item_atual(int *tipo, int *lin, int *col, int *vel, int *estado, t_lista *l);

/*
  Atribui os parametros passados ao nodo apontado pelo ponteiro atual.
*/
int modifica_item_atual(int tipo, int lin, int col, int vel, int estado, t_lista *l);

/*
  Remove o elemento apontado por atual da lista l.
  Faz o atual apontar para o sucessor do nodo removido.
  Retorna 1 se houve sucesso e zero caso contrário.
*/
int remove_item_atual(t_lista *l);
