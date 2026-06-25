#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ArvoreB.h"

typedef struct _Endereco Endereco;

typedef struct _Indice Indice;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

struct _Indice
{
    int indice;
    char cep[8];
};

int main(){
    srand(time(NULL));
    ArvoreB* a = ArvoreB_Abre("arvore.dat");
    FILE *arq1, *f;
    int posicao = 0;
    int num;
    int contador1 = 0;
    int contador2 = 0;
    Endereco u1;
    Indice u2;

    f = fopen("cep.dat", "rb");
    arq1 = fopen("indice1.dat", "wb");

    if((f == NULL) || (arq1 == NULL)){
        perror("Arquivo não aberto");
        return 1;
    }

    while(fread(&u1, sizeof(Endereco), 1, f) == 1){
    
        num = 1 + (rand() % 100);

        if( num <= 80){
            memcpy(u2.cep, u1.cep, 8);
            u2.indice = posicao;
            fwrite(&u2, sizeof(Indice), 1, arq1);
            contador1 += 1;
        }

        posicao += 1;
    }

    posicao = 0;
    fseek(f, 0, SEEK_SET);

    while(fread(&u1, sizeof(Endereco), 1, f) == 1){
        num = 1 + (rand() % 100);

        if(num <= 80){
            memcpy(u2.cep, u1.cep, 8);
            u2.indice = posicao;
            ArvoreB_Insere(a, u2.cep, u2.indice);
            contador2 +=1;
        }
        posicao += 1;
    }

    printf("%d ceps no indice1.dat\n", contador1);
    printf("%d ceps na arvoreB\n", contador2);
    fclose(arq1);
    fclose(f);
    ArvoreB_Fecha(a);
    return 0;
}