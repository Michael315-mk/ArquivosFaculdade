#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; // Ao EspaÃ§o no final da linha + quebra de linha
};

int main(){
    FILE *a1, *a2;
    Endereco unidade1, unidade2;
    int aux, tamanho = 699308;
    int cont = 0;
    int sim = 0;
    a1 = fopen("cep_ordenado.dat", "rb");
    a2 = fopen("cep_ordenado1.dat", "rb");

    if((a1 == NULL) || (a2 == NULL)){
        return 1;
    }

    while(cont < tamanho){
        fread(&unidade1, sizeof(Endereco), 1, a1);
        fread(&unidade2, sizeof(Endereco), 1, a2);

        aux = strncmp(unidade1.cep, unidade2.cep, 8);

        if (aux == 0){
            sim++;
        }
        cont++;
    }
    printf("Total de Elementos: %d\nElementos Iguais: %d", tamanho, sim);

    fclose(a1);
    fclose(a2);
    return 0;
}