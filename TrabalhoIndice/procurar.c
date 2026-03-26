#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct _Endereco Endereco;

typedef struct _IndiceEndereco Indereco;

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

struct _IndiceEndereco{
    char cep[8];
    unsigned int posicao;
};

int main(int argc, char ** argv){
    if( argc < 2){
        return 1;
    }
    FILE *arq1, *arq2;
    int tam, meio, fim, inicio, aux;
    Indereco *elemento1;
    Endereco *elemento2;
    
    elemento1 = (Indereco*) calloc(1, sizeof(Indereco));
    elemento2 = (Endereco*) calloc(1, sizeof(Endereco));

    arq1 = fopen("indice_ordenado.dat", "rb");
    arq2 = fopen("cep.dat", "rb");


    if((arq1 == NULL) || (arq2 == NULL)){
        return 1;
    }

    fseek(arq1, 0, SEEK_END);
    tam = ftell(arq1);
    tam = tam / sizeof(Indereco);
    rewind(arq1);

    fim = tam - 1;
    inicio = 0;
    
    while(inicio <= fim){
        meio = (fim + inicio)/2;
        fseek(arq1, meio * sizeof(Indereco), SEEK_SET);
        fread(elemento1, sizeof(Indereco), 1, arq1);
        
        aux = strncmp(argv[1], elemento1->cep, 8);
        printf("oi");
        if(aux == 0){
            fseek(arq2, elemento1->posicao * sizeof(Endereco), SEEK_SET);
            fread(elemento2,sizeof(Endereco), 1, arq2);
            printf("\n%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",elemento2->logradouro,elemento2->bairro,elemento2->cidade,elemento2->uf,elemento2->sigla,elemento2->cep);
            fclose(arq1);
            fclose(arq2);
            free(elemento1);
            free(elemento2);
            return 0;
        }
        else if( aux > 0){
            inicio = meio + 1;
        }
        else{
            fim = meio - 1;
        }
    }

    fclose(arq1);
    fclose(arq2);
    free(elemento1);
    free(elemento2);
    return 0;
}