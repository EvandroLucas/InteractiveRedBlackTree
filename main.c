#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interacao.h"
#include "color.h"
#include "redblack.h"
#include <time.h>


int main() {

    char comando[50] = " "; //recebe o nome do comando. ex: armazenar
    char input[50] = ""; //recebe os parametros. ex: sample.txt
    bool stop = 0; //condicao de parada
    bool armazenado = 0; //verifica se o registro foi preenchido
    RedBlackTree T; //arvore binaria de pesquisa balanceada
    RedBlackTree U;
    //para medir o tempo de execução e outros testes
    /*int numregs; //numero de registros
    clock_t begin;
    clock_t end;
    double t1 = 0;
    double t2 = 0;
    FILE * temp;*/
    long long int arg = 0;

    T = Initialize(); //gera uma árvore red-black
    T = MakeEmpty(T); //esvazia a árvore

    //apresentacao
    printf_super_green("\n      ");
    printf_yellow(" BUSCA & Armazenamento ");
    printf_super_green("           \n");

    printf("\nSegue aqui as opções: \n");
    printf("  - armazenador: puxa dados do arquivo para memoria\n");
    printf("  - buscador: procura dados na memoria\n");
    printf("  - imprimir: imprime, em cores, a arvore red-black\n");
    printf("  - salvar: salva os dados da memoria no arquivo 'saida.txt'\n");
    printf("  - limpar: limpa o arquvo 'saida.txt'\n");
    printf("Digite qualquer uma delas seguida do parametro adequado.\n");

    //funcionamento da interface, no geral
    while(stop == 0){ //ate terceira ordem, o programa sempre roda
        printf("\n");

        scanf("%s",comando); //le o comando

        //armazenador, joga do arquivo para a memoria
        if((strcmp("armazenador", comando) == 0) || (strcmp("armazenar", comando) == 0) || (strcmp("store", comando) == 0)){
            scanf("%s",input);
            printf("\n >> ");
            printf_cyan("Armazenando arquivo na memoria...\n");
            //begin = clock();
            //numregs = armazenador(input,T);
            armazenador(input,T);
            //end = clock();
            //t1 = (double)(end - begin) / CLOCKS_PER_SEC;
            printf(" >> ");
            printf_green("Armazenado com sucesso!");
            armazenado = 1;
        }
        //buscador, procura na memoria
        else if( (strcmp("buscador",comando) == 0) || (strcmp("buscar",comando) == 0) || (strcmp("search",comando) == 0) ){
            
            if(armazenado == 1){
                fgets(input,200,stdin);
                //begin = clock();
                buscador(input,T);
                //end = clock();
                //t2 = (double)(end - begin) / CLOCKS_PER_SEC;
            }
            else{
                printf_red("Comando invalido: nao foi armazenado registro algum");
            }
        }
        //imprime a arvore em cores
        else if((strcmp("imprimir",comando) == 0) || ((strcmp("print",comando) == 0))){
            
            if(armazenado == 1)
                imprime_arvore(T);
            else
                printf_red("Comando invalido: nao foi armazenado registro algum");
        }
        //salva em 'saida.txt' tudo o que foi jogado na memoria
        else if((strcmp("salvar",comando) == 0) || (strcmp("save",comando) == 0) ){
            printf("\n >> ");
            printf_cyan("Salvando dados em ''saida.txt''...\n");
            PrintFile(T,"saida.txt");
            printf(" >> ");
            printf_green("Salvo em ''saida.txt'' com sucesso!\n");
        }
        //limpa o arquivo 'saida.txt'
        else if((strcmp("clean",comando) == 0) || (strcmp("reset",comando) == 0)  || (strcmp("limpar",comando) == 0) ){
            printf("\n >> ");
            printf_cyan("Apagando dados de''saida.txt''...\n");
            ResetFile("saida.txt");
            printf(" >> ");
            printf_green("Dados em ''saida.txt'' apagados com sucesso!\n");
        }
        //para o programa
        else if( (strcmp("achar",comando) == 0) || (strcmp("find",comando) == 0) ){
            scanf("%lli",&arg);
           // begin = clock();
            U = Find(arg,T);
            //end = clock();
            //t2 = (double)(end - begin) / CLOCKS_PER_SEC;
            printf_cyan("\nProcurando por %lli...\n",arg);
            printf_green("Encontramos: %lli;%s;%lli \n",U->Element.data,U->Element.regEvHex,U->Element.regUs);
            stop = 1;
        }
        else if( (strcmp("parar",comando) == 0) || (strcmp("stop",comando) == 0) ){
            printf_magenta("\nFim do procedimento, arvore removida da memoria");
            stop = 1;
        }
        //para o programa
        else{
            printf_red("\nCOMANDO INVÁLIDO\n");
            stop = 1;
        }

    }
    /*printf_green("\n\n Inseridos      : %i elementos",numregs);
    printf_green("\n\n Tempo armazenar: %lf s",t1);
    printf_green("\n\n Tempo buscar   : %lf s",t2);

        temp = fopen("tempos.txt","a");
        fprintf(temp,"%i %lf %lf\n",numregs,t1,t2);
        fclose(temp);
    */

    RemoveTree(T); //apaga a arvore para evitar leaks de memoria

    //fim
    printf("\n\n");
    printf_super_red("          END          \n\n");
    return 0;
}