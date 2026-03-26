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


int main(int argc, char ** argv) { 
    FILE *arq1;
    Indereco *unidade;
    arq1 = fopen("indice_ordenado.dat", "rb");
    

    if (arq1 == NULL){
        return 1;
    }

    unidade = (Indereco*) calloc(1, sizeof(Indereco));
    
    for(int x = 0; x < 100; x++){
        fread(unidade, sizeof(Indereco), 1, arq1);
        printf("Indice: %d\nCep: %.8s\n\n", unidade->posicao, unidade->cep);
    }

    free(unidade);
    fclose(arq1);
    return 0;
}
