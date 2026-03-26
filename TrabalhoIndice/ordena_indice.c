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

struct _IndiceEndereco
{
    char cep[8];
    unsigned int posicao;
};

int compara(const void *e1, const void *e2)
{
    return strncmp(((Indereco *)e1)->cep, ((Indereco *)e2)->cep, 8);
}

int main()
{
    FILE *a1, *a2;
    Indereco *unidade;
    int tam;

    a1 = fopen("indice.dat", "rb");
    a2 = fopen("indice_ordenado.dat", "wb");

    if ((a1 == NULL) || (a2 == NULL))
    {
        return 1;
    }

    fseek(a1, 0, SEEK_END);
    tam = ftell(a1);
    rewind(a1);
    tam = tam / sizeof(Indereco);

    unidade = (Indereco *)calloc(tam, sizeof(Indereco));

    fread(unidade, sizeof(Indereco), tam, a1);
    qsort(unidade, tam, sizeof(Indereco), compara);
    fwrite(unidade, sizeof(Indereco), tam, a2);

    fclose(a1);
    fclose(a2);
    free(unidade);
    return 0;
}