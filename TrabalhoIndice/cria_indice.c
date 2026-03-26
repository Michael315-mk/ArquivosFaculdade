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
    FILE *arq1, *arq2;
    Endereco *unidade1;
    Indereco *unidade2;
    int contador = 0;

    arq1 = fopen("cep.dat", "rb");
    arq2 = fopen("indice.dat", "wb");

    if ((arq1 == NULL) || (arq2 == NULL)){
        return 1;
    }

    unidade1 = (Endereco*) calloc(1, sizeof(Endereco));
    unidade2 = (Indereco*) calloc(1, sizeof(Indereco));

    while ((fread(unidade1, sizeof(Endereco), 1, arq1)) == 1)
    {
        strncpy(unidade2->cep,unidade1->cep, 8);
        unidade2->posicao = contador;

        fwrite(unidade2, sizeof(Indereco), 1, arq2);
        contador++;
    }
    
    fclose(arq1);
    fclose(arq2);
    
    return 0;
}
