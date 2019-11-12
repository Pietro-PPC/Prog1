#include <stdio.h>
#include <stdlib.h>
#include "lib_fila.h"

#define FILAS_ATERRISSAGEM 4
#define FILAS_DECOLAGEM 3
#define NUM_PISTAS 3
#define MAX_ATERRISS9AGENS 3
#define MAX 10000

typedef struct vet_filas
{
	tipo_fila *v;
	int tam;
} vet_filas;

typedef struct dados_media
{
	int quant;
	int soma;
} dados_media;

typedef struct complementar
{
	int sem_combustivel;
	int UT_enfileiramento[MAX];
	int UT_atual;
	dados_media m_decolagens;
	dados_media m_aterrissagens;
	tipo_fila caidos;
} complementar;

int inicializa_vet_filas(vet_filas *vetor, int tam)
{
	int i;

	vetor->v = (tipo_fila*) malloc(tam * sizeof(tipo_fila));
	if (vetor->v == NULL)
		return 0;
	vetor->tam = tam;

	for (i = 0; i < tam; i++)
		if (!inicializa_fila(&vetor->v[i]))
			return 0;

	return 1;
}

int menor_fila(vet_filas *vetor)
{
	int i, menor = 0;
	
	for (i = 1; i < vetor->tam; i++)
		if (tamanho(&vetor->v[i]) < tamanho(&vetor->v[menor]))
			menor = i;
	return menor;
}

int maior_fila(vet_filas *vetor)
{
	int i, maior = 0;
	
	for (i = 1; i < vetor->tam; i++)
		if (tamanho(&vetor->v[i]) > tamanho(&vetor->v[maior]))
			maior = i;
	return maior;
}

void imprime_filas(vet_filas *vetor, char tipo_fila[15])
{
	int i;
	for (i = 0; i < vetor->tam; i++)
	{
		
		printf("fila %s %d: ", tipo_fila, i+1);
		imprime_fila(&vetor->v[i]);
		printf("\n");
	}
}

void decrementa_combustiveis(vet_filas *vetor)
{
	int i, id, autonomia;

	for (i = 0; i < vetor->tam; i++)
	{
		inicializa_atual_inicio(&vetor->v[i]);
		while (vetor->v[i].atual != NULL)
		{
			consulta_atual (&id, &autonomia, &vetor->v[i]);
			autonomia--;
			altera_atual (id, autonomia, &vetor->v[i]);
			incrementa_atual (&vetor->v[i]);
		}
	}
}

void remove_sem_combustivel(vet_filas *vetor, int pistas[], complementar *extras)
{
	int i, id, pousos = 0;
	
	for (i = 0; i < vetor->tam; i++)
	{
		while (remove_fila(&id, 0, &vetor->v[i]))
		{
			if (pousos < 3)
			{
				pousos++;
				pistas[3 - pousos] = 0;
				extras->m_aterrissagens.quant++;
				extras->m_aterrissagens.soma += (extras->UT_atual - extras->UT_enfileiramento[id]);
			}
			else
				enfileira(id, -1, &extras->caidos);
		}
	}
	
	extras->sem_combustivel += pousos;
}

void reinicia_pistas(int pistas[])
{
	int i;
	for (i = 0; i < NUM_PISTAS; i++)
		pistas[i] = 1;
}

void imprime_dados_extras(complementar *extras)
{
	printf("Tempo medio para decolagem: ");
	if (extras->m_decolagens.quant > 0)
		printf("%d\n", (extras->m_decolagens.soma / extras->m_decolagens.quant) );
	else
		printf("0\n");

	printf("Tempo medio para aterrissagem: ");
	if (extras->m_aterrissagens.quant > 0)
		printf("%d\n", (extras->m_aterrissagens.soma / extras->m_aterrissagens.quant) );
	else
		printf("0\n");
	
	printf("Numero de aeronaves que aterrissaram sem combustivel: %d\n", extras->sem_combustivel);

	printf("IDs das aeronaves que cairam: ");
	imprime_fila(&extras->caidos);
	printf("\n");
}

void decola_restantes(vet_filas *decolagem, int pistas[], complementar *extras)
{
	int i, id, t;

	for (i = 0; i < NUM_PISTAS; i++)
		if (pistas[i])
		{
			if (desenfileira(&id, &t, &decolagem->v[i]))
			{
				pistas[i] = 0;
				extras->m_decolagens.quant++;
				extras->m_decolagens.soma += (extras->UT_atual - extras->UT_enfileiramento[id]);
			}
			else if (desenfileira(&id, &t, &decolagem->v[ maior_fila(decolagem) ]))
			{
				pistas[i] = 0;
				extras->m_decolagens.quant++;
				extras->m_decolagens.soma += (extras->UT_atual - extras->UT_enfileiramento[id]);
			}
		}
}

void aterrissa_restantes(vet_filas *aterrissagem, int pistas[], complementar *extras)
{
	int i, id, t;
	
	for (i = 0; i < 2; i++)
		if (pistas[i])
		{
			if (tamanho(&aterrissagem->v[2*i]) >= tamanho(&aterrissagem->v[2*i + 1] ))
			{
				if (desenfileira(&id, &t, &aterrissagem->v[2*i]))
				{
					pistas[i] = 0;
					extras->m_aterrissagens.quant++;
					extras->m_aterrissagens.soma += (extras->UT_atual - extras->UT_enfileiramento[id]);
				}
			}		
			else if (desenfileira(&id, &t, &aterrissagem->v[2*i + 1]))
			{
				pistas[i] = 0;
				extras->m_aterrissagens.quant++;
				extras->m_aterrissagens.soma += (extras->UT_atual - extras->UT_enfileiramento[id]);
			}
		}
}

int inicializa_complementares(complementar *extras)
{
	if (!inicializa_fila(&extras->caidos))
		return 0;
	extras->sem_combustivel = 0;
	extras->m_decolagens.quant = 0;
	extras->m_decolagens.soma = 0;
	extras->m_aterrissagens.quant = 0;
	extras->m_aterrissagens.soma = 0;
	return 1;	
}

int main()
{
	int i, j;
	int t_total, num_de, num_at, id_at = 1, id_de = 2, autonomia;
	int pistas[NUM_PISTAS];
	vet_filas aterrissagem, decolagem;
	complementar extras;

	inicializa_vet_filas(&aterrissagem, FILAS_ATERRISSAGEM);
	inicializa_vet_filas(&decolagem, FILAS_DECOLAGEM);
	inicializa_complementares(&extras);

	scanf("%d", &t_total);
	for (i = 1; i <= t_total; i++)
	{
		extras.UT_atual = i;
		decrementa_combustiveis(&aterrissagem);

		/* le e enfileira aterrissagens */
		scanf("%d", &num_at);
		for (j = 0; j < num_at; j++)
		{
			scanf("%d", &autonomia);
			enfileira(id_at, autonomia, &aterrissagem.v[ menor_fila(&aterrissagem) ]);
			extras.UT_enfileiramento[id_at] = i;
			id_at += 2;
		}

		/* le e enfileira decolagens */
		scanf("%d", &num_de);
		for (j = 0; j < num_de; j++)
		{
			enfileira(id_de, -1, &decolagem.v[ menor_fila(&decolagem) ]);
			extras.UT_enfileiramento[id_de] = i;
			id_de += 2;
		}
		
		reinicia_pistas(pistas);
		remove_sem_combustivel(&aterrissagem, pistas, &extras);
		
		if ( i % 2 == 1 )
			decola_restantes(&decolagem, pistas, &extras);
		else
		{
			aterrissa_restantes(&aterrissagem, pistas, &extras);
			decola_restantes(&decolagem, pistas, &extras);
		}

		printf("Unidade de tempo %d\n", i);
		imprime_filas(&aterrissagem, "aterrissagem");
		imprime_filas(&decolagem, "decolagem");
		imprime_dados_extras(&extras);

		printf("\n");
	}

	return 0;
}
