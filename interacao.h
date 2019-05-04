#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "redblack.h"
#include "color.h"

#ifndef _INTERACAO_H
#define _INTERACAO_H

#define MAX_BUSCA 20

//limpa um arquivo
void ResetFile(char *filename){
    FILE* arquivo;
    arquivo = fopen(filename,"w");
    fclose(arquivo);
}

//converte uma string em hexadecimal para um inteiro em decimal
long long int hex_to_dec(char *stir){
    char str[100];
    strcpy(str,stir);
    unsigned n;

    sscanf(str, "%x", &n);
    return n;

}

//verifica a validade do input na funcao "buscador"
//serve como tratamento & prevencao de erros 
bool validade_arg(long long int arg1, long long int arg2, long long int min, long long int max){

    if( arg1 < min){
        printf("arg1 (%lli) < min (%lli)",arg1,min); return false;
    }
    if( arg2 < min) {
        printf("arg2 (%lli) < min (%lli)",arg2,min); return false;
    }
    if( arg1 > max) {
        printf("arg1 (%lli) > max (%lli)",arg1,max); return false;
    }
    if( arg2 > max) {
        printf("arg2 (%lli) < max (%lli)",arg2,max); return false;
    }


    return true;
}

//separa uma string em tres, usando o ';' como delimitador
//as tres partes sao armazenadas no registro
 void separa_string(reg *inp, char* stir){

    char token[100]; 
    char str[100];
    char b[20];

    strcpy(str,stir); //evita erro de segmentacao

    //pega a data
    strcpy(token,strtok(str,";"));
    inp->data = (long long int) strtol(token, (char **)NULL, 10);
    
    //pega o registro de evento em hexa
    strcpy(token,strtok(NULL,";"));
    strcpy(b,token);
    strcpy(inp->regEvHex,b);

    //armazena o registro do evento em dec
    //facilitara a busca no futuro
    inp->regEvDec = hex_to_dec(inp->regEvHex);

    //pega o registro do usuario
    strcpy(token,strtok(NULL,";"));
    inp->regUs = (long long int) strtol(token, (char **)NULL, 10);
    
    return;
}

//recebe um arquivo, le os registros e os armazena na memoria
int armazenador(char *filename, RedBlackTree T){

    int k = 0; //verifica fim de arquivo
    int i = 0; //salva numero de registros inseridos
    char str[100] = "";
    char line[400] = "";
    FILE* entrada;
    reg aux;

    //abre o arquivo solicitado
    strcpy(str,filename);
    entrada = fopen(str,"r");

    do{ //enquanto o arquivo nao acabar...
        k = fscanf(entrada,"%s",line); //le se uma linha
        if(k != EOF){
            separa_string(&aux,line); //a string eh processada 
            T = Insert(aux, T); // e inserida na arvore
            i++;
        }
    } while(k != EOF);

    //printf("\nForam inseridos %i registros\n",i);
    
    fclose(entrada);
    return i;
}

//procura na arvore um intervalo de registros
void buscador(char *input, RedBlackTree T){

    char str[200] =""; //copia o input para prevenir erros
    char token[100]; //auxiliar na interpretacao do input
    long long int arg1 = 0; //parametro 1 do intervalo de busca
    long long int arg2 = 0; //parametro 2 do intervalo de busca
    int quant = 0; //quantidade de filtros (identificadores)
    int argnum = 0; //numero de parametros no geral
    long long int vet[MAX_BUSCA]; //armazena filtros
    long long int min = 0; //menor elemento inserido
    long long int max = 0; //maior elemento inserido

    //auxiliares de leitura
    int aux= 0;
    int aux2= 0;
    int left = 0; //guarda o tamanho do input ainda nao lido

    //procuramos os limites da arvore a serem respeitados
    //evita acesso invalido
    min = FindMin(T)->Element.data;
    max = FindMax(T)->Element.data;

    strcpy(str,input); //input copiado, mais seguro
    aux = strlen(str); //aux pega o tamanho do input

    //abaixo analisaremos o tamanho da string de entrada 
    //antes e depois da concatenação para decidir a
    //quantidade dos parametros

    if(aux > 19){ //nesse caso, deve haver mais de um parametro
        strcpy(token,strtok(str," ,\n")); //le-se a data
        aux2 = strlen(token); //verificamos o tamanho
        //convertemos string -> inteiro
        arg1 = (long long int) strtol(token, (char **)NULL, 10);
    }
    else{ //existe apenas um parametro: a data
        //convertemos string -> inteiro
        arg1 = (long long int) strtol(str, (char **)NULL, 10);
        arg2 = arg1;
        aux2 = aux; //1 parametro = todos os parametros, nesse caso
    }

    //nesse if-else, decidimos a quantidade de argumentos
    if(aux == aux2){ //nesse caso, so ha um argumento
        argnum = 1;
    }
    else{ //nesse caso, ha mais de um argumento

        //lemos o que falta
        strcpy(token,strtok(NULL,"\n"));
        aux = strlen(token);
        //aux guarda o numero de agumentos que sobraram

        //lemos o proximo, convertemos...
        strcpy(str,token);
        strcpy(token,strtok(str," ,\n"));
        arg2 = (long long int) strtol(token, (char **)NULL, 10);
        aux2 = strlen(token);

        if(aux == aux2){ //se isso ocorreu, so existem dois
            argnum = 2;
        }
        else{ //caso contrario, sao tres ou mais
            argnum = 3;
        }
    }
    
    //nesse if-else, se valido, um caso para cada argumento
    if(validade_arg(arg1,arg2,min,max)){
        if(argnum == 1){ //Caso 1: apenas um parametro
            //nao iremos procurar um intervalo, mas um ponto
            //passamos o mesmo valor nos limites e
            //passamos 0 para o parametro 4 de "PrintTreeInter"
            //indicando que nao ha filtros alem dos limites
            PrintTreeInter(T,arg1,arg1,0,vet);
            return;
        }
        if(argnum == 2){ //Caso 2: dois parametros
            //imprime apenas o intervalo, sem filtros
            // do menor pro maior
            //passamos 0 para o parametro 4 de "PrintTreeInter"
            //indicando que nao ha filtros alem dos limites

            if(arg1 <= arg2){
                PrintTreeInter(T,arg1,arg2,0,vet);
            }
            else{
                PrintTreeInter(T,arg2,arg1,0,vet);
            }
            return;
        }
        if(argnum >= 3){ //Caso 3: tres ou mais argumentos
        //mais complicado do que os anteriores
        //serao lidos os filtros alem do limite

            strcpy(token,strtok(NULL,"\n"));
            strcpy(str,token);
            //comecamos lendo a primeira restricao
            //armazenamos sua versao decimal no vetor
            vet[quant] = hex_to_dec(str);
        
            aux = 1;
            left = strlen(str);

            //esse laco coloca os argumentos no vetor ate que
            //nao sobrem mais restricoes
            //"quant" guarda a quantidade de filtros de evento
            while (aux == 1) {
                if(quant == 0){
                    strcpy(token,strtok(str," "));
                    left = left - strlen(token)-1;
                }
                else{
                    strcpy(token,strtok(NULL," ,\n"));
                    left = left - strlen(token)-1;
                }
                aux2 = strlen(token);

                vet[quant] = hex_to_dec(token);
                quant++;

                if(left <= 0){
                    break;
                }

            }
            //procuramos os registros com restricoes
                PrintTreeInter(T,arg1,arg2,quant,vet);
                return;
        }
    }
    //tratando de entradas pequenas ou grandes demais...
    else{
        printf_red("\nEndereço inexistente (arg_inv). Tente inserir outro intervalo ou restricoes");
    }


}

#endif