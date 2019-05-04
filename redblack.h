#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "color.h"

#define NegInfinity (-1000000000000)

#ifndef _RedBlack_H
#define _RedBlack_H

/*Essa biblioteca eh uma versao modificada da arvore redblack 
disponibilizada pelo Zentutu, no link:
 http://www.zentut.com/c-tutorial/c-red-black-tree/

Voce pode aprender tudo sobre essa estrutura de dados no link

*/

bool globalCheck;

typedef struct reg {
    long long int data;
    char regEvHex[20];
    long long int regEvDec;
    long long int regUs;
} reg;

struct RedBlackNode;
typedef struct RedBlackNode *Position;
typedef struct RedBlackNode *RedBlackTree;

typedef enum ColorType {
    Red,
    Black,
    None
} ColorType;

struct RedBlackNode {
    reg Element;
    RedBlackTree Left;
    RedBlackTree Right;
    RedBlackTree Down;
    int dup;
    ColorType Color;
    bool hey;
};


static Position NullNode = NULL; /* Precisa de inicializacao*/

/* Inicializa a arvore e o nullnode*/
RedBlackTree Initialize(void) {
   RedBlackTree T;
 
    // aloca nullnode
    if (NullNode == NULL) {
        NullNode = malloc(sizeof ( struct RedBlackNode));
        if (NullNode == NULL)
            printf("\n\nOut of space!!!\n\n");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
        NullNode->Element.data = 12345;
        NullNode->hey = 1;
        NullNode->dup = 0;
    }
 
    // aloca a raiz
    T = malloc(sizeof ( struct RedBlackNode));
    if (T == NULL)
        printf("\n\nOut of space!!!\n\n");
    T->Element.data = NegInfinity;
    T->Left = T->Right = NullNode;
    T->Color = Black;
    T->hey = 0;
    T->dup = 0;
 
    return T;
}

/* END */

void Output(RedBlackTree T, bool bound) {

    int i;
    RedBlackTree U;
    U = T;
    for(i=0;i<=T->dup;i++){
        if(bound)
            printf_green("\n %lli",U->Element.data);
        else
            printf_blue("\n %lli",U->Element.data);

        printf(";%s;",U->Element.regEvHex);
        //printf_green("%12.lli;",Element.regEvDec);
        printf_blue("%lli",U->Element.regUs);
        globalCheck = 1;
        U = U->Down;
    }

}

//Essa funcao imprime, em ordem, o intervalo
static void DoPrintInter(RedBlackTree T, long long int arg1, long long int arg2, int quant, long long int* vet) {
    int i;
    if(quant == 0 ){ //imprime sem restricoes de evento
        if (T != NullNode){
            if(T->Left->Element.data >= arg1){
                DoPrintInter(T->Left, arg1,arg2,0,vet);
            }
            else{
                 DoPrintInter(T->Left->Right,arg1,arg2,0,vet);
            }
            if((T->Element.data >= arg1) && (T->Element.data <= arg2)){
                if((T->Element.data == arg1) || (T->Element.data == arg2)){
                        Output(T,1);
                }
                else{
                        Output(T,0);
                }
            }

            if(T->Right->Element.data <= arg2){
                DoPrintInter(T->Right,arg1,arg2,0,vet);
            }
            else{
                DoPrintInter(T->Right->Left,arg1,arg2,0,vet);
            }
        }
    }
    else { //imprime com restricoes de evento
        if (T != NullNode){
            if(T->Left->Element.data >= arg1)
                DoPrintInter(T->Left, arg1,arg2,quant,vet);

            for(i=0;i<quant;i++){
                if(T->Element.regEvDec == vet[i]){
                    if((T->Element.data == arg1) || (T->Element.data == arg2))
                        Output(T,1);
                    else
                        Output(T,0);
                    i = quant;
                }
            }

            if(T->Right->Element.data <= arg2)
                DoPrintInter(T->Right,arg1,arg2,quant,vet);
        }
    }
    
}

//chama a funcao para imprimir o intervalo
void PrintTreeInter(RedBlackTree T, long long int arg1, long long int arg2,int quant,long long int* vet){
    DoPrintInter(T->Right,arg1,arg2,quant,vet);
    if(globalCheck == 0){
        printf_red("\nEndereço inexistente. Tente inserir outro intervalo ou restricoes");
    }
    globalCheck = 0;
}

//imprime no arquivo de saida
static void DoPrintFile(RedBlackTree T,FILE* saida) {
    int i;
    RedBlackTree U;
    U = T;

    if (T != NullNode){
        DoPrintFile(T->Left,saida);
            
        for(i=0;i<=T->dup;i++){
            fprintf(saida,"%lli",U->Element.data);

            fprintf(saida,";%s;",U->Element.regEvHex);
            //printf_green("%12.lli;",Element.regEvDec);
            fprintf(saida,"%lli\n",U->Element.regUs);
            U = U->Down;

        }

        DoPrintFile(T->Right,saida);

    }
  
}

void PrintFile(RedBlackTree T, char *filename){
    FILE* saida;
    saida = fopen(filename,"w");

    DoPrintFile(T->Right,saida);

    fclose(saida);
    return;
}

/*A funcao abaixo imprime a arvore em forma de cascata.
O codigo ficou extremamente complicado e, como a funcao
tem apenas finalidade de debug, ou seja, nao eh essencial,
sua interpretacao fica como exercicio ao leitor :)"
*/
void imprime(RedBlackTree T, int level, int *vet, int last,char D) {
    int i = 0;
    int check = 0;
    int check2 = 0;
    char str[300] = "";

    if (T != NullNode) {
        printf("\n");
        strcat(str, "\n");
        for (i = 0; i < level; i++) {
            if (check == 0) {
                printf(" ");
                strcat(str, " ");
            }
            else {
                if (check2 == 0) {
                    printf("%s", str);
                    check2++;
                }
                printf("-");
                if (vet[i - 1] == 1) {
                    vet[i - 1]++;
                }
                else if (vet[i - 1] == 2) {
                    vet[i - 1] = 3;
                }
            }
            if (i > 0) {
                if (vet[i] == 1){
                    printf("  !");
                    strcat(str, "  !");
                }
                else if (vet[i] == 2){
                    printf("  !");
                    strcat(str, "  !");
                }
                else if (vet[i] == 3) {
                    printf("   ");
                    strcat(str, "   ");
                }
            }
            if (last == i){
                check = 1;
            }
        }
        printf(">>");
        vet[level] = 1;
        last = level;
        if (T->Element.data == NegInfinity){
            printf_super_green("ARVORE\n");
        }
        else if (T->Color == Black)
            printf_super_blue(" %lli;%s...", T->Element.data, T->Element.regEvHex);
        else{
            printf_super_red(" %lli;%s... ", T->Element.data, T->Element.regEvHex);
        }
        if(D != ' ')
            printf_super_yellow(" %c ",D);
        if(T->dup > 0)
            printf_super_magenta(" %i ",T->dup);

        if (level == 0){//economiza espaco desnecessario 
            imprime(T->Right, 1, vet, 1,'R');
            imprime(T->Left, 1, vet, 1,'L');
        }
        else{  
            imprime(T->Right, level + 5, vet, last,'R');
            imprime(T->Left, level + 5, vet, last,'L');
        }
    }
    return;
}

//chama a funcao imprime com alguns parametros pre-definidos
void imprime_arvore(RedBlackTree T){
    int i = 0; //contador 
    int vet[1000]; //salva as linhas verticais
    for(i=0;i<999;i++){
        vet[i] = 0;
    }
    imprime(T, 0, vet, 0,' ');
    return;
}

//encontra um no especifio na arvore
Position Find(long long int X, RedBlackTree T){
    // printf("\nProcurando por %i enquanto em %i",X,T->Element);
    if (T == NullNode){
        NullNode->Color = None;
        return NullNode;
    }
    if (X < T->Element.data){
        return Find(X, T->Left);
    }
    if (X > T->Element.data){
        return Find(X, T->Right);
    }
    else{
        return T;
    }
}

//encontra o menor no da arvore
Position FindMin(RedBlackTree T){
    T = T->Right;
    while (T->Left != NullNode)
        T = T->Left;

    return T;
}
//encontra o maior no da arvore
Position FindMax(RedBlackTree T){
    while (T->Right != NullNode)
        T = T->Right;

    return T;
}

/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */

/* Update heights, then return new root */

//funcao auxiliar de balanceamento da arvore
static Position SingleRotateWithLeft(Position K2){
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */

/* Update heights, then return new root */

//funcao auxiliar de balanceamento da arvore
static Position SingleRotateWithRight(Position K1){
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    return K2; /* New root */
}

/* Perform a rotation at node X */
/* (whose parent is passed as a parameter) */

/* The child is deduced by examining Item */

//funcao auxiliar de balanceamento da arvore
static Position Rotate(long long int Item, Position Parent){

    if (Item < Parent->Element.data)
        return Parent->Left = Item < Parent->Left->Element.data ? SingleRotateWithLeft(Parent->Left) : SingleRotateWithRight(Parent->Left);
    else
        return Parent->Right = Item < Parent->Right->Element.data ? SingleRotateWithLeft(Parent->Right) : SingleRotateWithRight(Parent->Right);
}

static Position X, P, GP, GGP;

//funcao auxiliar de balanceamento da arvore
static void HandleReorient(long long int Item, RedBlackTree T){
    X->Color = Red; /* Do the color flip */
    X->Left->Color = Black;
    X->Right->Color = Black;

    if (P->Color == Red) /* Have to rotate */
    {
        GP->Color = Red;
        if ((Item < GP->Element.data) != (Item < P->Element.data))
            P = Rotate(Item, GP); /* Start double rotate */
        X = Rotate(Item, GGP);
        X->Color = Black;
    }
    T->Right->Color = Black; /* Make root black */
}

//insere registro na arvore
RedBlackTree Insert(reg Item, RedBlackTree T){
    bool equal =0;
    X = P = GP = T;
    NullNode->Element.data = Item.data;
    NullNode->hey = 1;
    while ((X->hey != 1) && (equal == 0))  /* Descend down the tree */
    {
        GGP = GP;
        GP = P;
        P = X;
        if (Item.data < X->Element.data){
            X = X->Left;
        }
        else if (Item.data > X->Element.data){
            X = X->Right;
        }
        else{
            equal = 1;
        }
        if(!equal){
            if (X->Left->Color == Red && X->Right->Color == Red)
                HandleReorient(Item.data, T);
        }
    }
    if(equal == 0){
        if (X != NullNode)
            return NullNode; /* Duplicate */

        X = malloc(sizeof(struct RedBlackNode));
        if (X == NULL)
            printf("\n\nOut of space!!!\n\n");
        X->Element.data = Item.data;
        strcpy(X->Element.regEvHex, Item.regEvHex);
        X->Element.regEvDec = Item.regEvDec;
        X->Element.regUs = Item.regUs;
        X->Left = X->Right = X->Down = NullNode;
        X->hey = 0;
        X->dup = 0;

        if (Item.data < P->Element.data) /* Attach to its parent */
            P->Left = X;
        else
            P->Right = X;
        HandleReorient(Item.data, T); /* Color it red; maybe rotate */
    }
    if(equal == 1){
         X = malloc(sizeof(struct RedBlackNode));
        if (X == NULL)
            printf("\n\nOut of space!!!\n\n");
        X->Element.data = Item.data;
        strcpy(X->Element.regEvHex, Item.regEvHex);
        X->Element.regEvDec = Item.regEvDec;
        X->Element.regUs = Item.regUs;
        X->Left = X->Right = X->Down = NullNode;
        X->hey = 0;
        X->dup = 0;
    
        while(P->dup > 0){
            P->dup++;
            P = P->Down;
        }
        if (P->dup == 0){ /* Attach to its parent */
            P->Down = X;
            P->dup++;
        }
    }
    return T;
}

//auxiliar para ponteiros
long long int Retrieve(Position P){
    return P->Element.data;
}
//Esvazia a arvore mas nao a apaga
static RedBlackTree MakeEmptyRec(RedBlackTree T){
    if (T != NullNode){
        MakeEmptyRec(T->Left);
        MakeEmptyRec(T->Right);
        MakeEmptyRec(T->Down);
        free(T);
    }
    return NullNode;
}

//esvazia tudo alem da raiz (nao apaga a arvore)
RedBlackTree MakeEmpty(RedBlackTree T){
    T->Right = MakeEmptyRec(T->Right);
    return T;
}

//apaga definitivamente a árvore, essencial para 
//evitar vazamentos de memoria
void RemoveTree (RedBlackTree T){
    T = MakeEmpty(T);
    free(T);
    free(NullNode);
    return;
}

#endif