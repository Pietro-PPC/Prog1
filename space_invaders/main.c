#include "lib_invaders.h"

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
    while (tecla != 'q')
    {
        while (tecla != 'q' && jogo.aliens.quant)
        {
            imprime_tela(&jogo);
            atualiza_projeteis(&jogo);

            atualiza_nave_mae(&jogo);
            if (!nave_mae_viva(&jogo))
            {
                alea = rand() % 300;
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
                printf("Espaco limite excedido! Jogo abortado.");
                exit(1);
            }

            if (fim_jogo(&jogo))
            {       
                imprime_tela(&jogo);
                while (tecla != 'q')
                    tecla = getch();
            }

            jogo.iter++;
            usleep(INTERVALO);
        }
        imprime_tela(&jogo);
        if (!jogo.aliens.quant)
        {
            reinicializa_jogo(&jogo);
            usleep(1000000);
        }
    }
    endwin();

    free(jogo.descricao_tipo);
    destroi_lista( &jogo.l_do_bem );
    destroi_lista( &jogo.l_do_mal );
    destroi_lista( &jogo.l_projeteis );
    return 0;
}
