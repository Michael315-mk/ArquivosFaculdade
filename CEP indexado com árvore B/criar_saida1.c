#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ArvoreB.h"

typedef struct _Endereco Endereco;

typedef struct _Indice Indice;


struct _Indice
{
    int indice;
    char cep[8];
};




int main(){
    FILE *s, *f;
    Indice u1;
    int posicao;
    int contador = 0;
    ArvoreB* a = ArvoreB_Abre("arvore.dat");

    s = fopen("indice1.dat", "rb");
    f = fopen("saida1.dat", "wb");

    if ((s == NULL) || (f == NULL)){
        perror("Arquivo não foi aberto");
        return 1;
    }

    while(fread(&u1, sizeof(Indice), 1, s) == 1){
        posicao = ArvoreB_Busca(a, u1.cep);

        if( posicao != -1){
            fwrite(&u1, sizeof(Indice), 1, f);
            contador += 1;
        }
    }

    printf("%d ceps no arquivo saida\n", contador);
    ArvoreB_Fecha(a);
    fclose(s);
    fclose(f);
    return 0;
}