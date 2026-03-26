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
    char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

int compara(const void *e1, const void *e2)
{
    return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}

void criar(int qtde)
{
    FILE *arq1, *arq2;
    Endereco *unidade;
    char nome[30];
    int cont = 0;
    int quant_bytes, quant_items;
    int vet[qtde];

    arq1 = fopen("cep.dat", "rb");

    if (arq1 == NULL)
    {
        return;
    }

    fseek(arq1, 0, SEEK_END);
    quant_bytes = ftell(arq1);
    quant_items = quant_bytes / sizeof(Endereco);
    rewind(arq1);

    for (int i = 0; i < qtde; i++)
    {
        vet[i] = quant_items / qtde;
    }
    for (int i = 0; i < (quant_items % qtde); i++)
    {
        vet[i] += 1;
    }

    while (cont < qtde)
    {
        snprintf(nome, 30, "%d.dat", cont);

        // Abrir arquivo para escrita
        arq2 = fopen(nome, "wb");
        if (arq2 == NULL)
        {
            return;
        }

        unidade = (Endereco *)calloc(vet[cont], sizeof(Endereco));

        fread(unidade, sizeof(Endereco), vet[cont], arq1);
        qsort(unidade, vet[cont], sizeof(Endereco), compara);
        fwrite(unidade, sizeof(Endereco), vet[cont], arq2);
        cont++;
        fclose(arq2);
        free(unidade);
    }

    fclose(arq1);
}

void unir(char *name1, char *name2, char *name3)
{
    FILE *n1, *n2, *n3;
    Endereco *unidade1, *unidade2;
    int aux;
    unidade1 = (Endereco *)calloc(1, sizeof(Endereco));
    unidade2 = (Endereco *)calloc(1, sizeof(Endereco));

    n1 = fopen(name1, "rb");
    n2 = fopen(name2, "rb");
    n3 = fopen(name3, "wb");

    if ((n1 == NULL) || (n2 == NULL) || (n3 == NULL))
    {
        return;
    }

    fread(unidade1, sizeof(Endereco), 1, n1);
    fread(unidade2, sizeof(Endereco), 1, n2);

    while (!feof(n1) && !feof(n2))
    {
        aux = strncmp(unidade1->cep, unidade2->cep, 8);
        if (aux < 0)
        {
            fwrite(unidade1, sizeof(Endereco), 1, n3);
            fread(unidade1, sizeof(Endereco), 1, n1);
        }
        else
        {
            fwrite(unidade2, sizeof(Endereco), 1, n3);
            fread(unidade2, sizeof(Endereco), 1, n2);
        }
    }

    while (!feof(n1))
    {
        fwrite(unidade1, sizeof(Endereco), 1, n3);
        fread(unidade1, sizeof(Endereco), 1, n1);
    }

    while (!feof(n2))
    {
        fwrite(unidade2, sizeof(Endereco), 1, n3);
        fread(unidade2, sizeof(Endereco), 1, n2);
    }

    fclose(n1);
    fclose(n2);

    remove(name1);
    remove(name2);

    fclose(n3);
}

int main(int argc, char **argv)
{
    char name1[30], name2[30], name3[30];
    int num_divisoes = 16;
    int num_inicio, num_final, aux;

    num_inicio = 0;
    num_final = aux = num_divisoes;

    criar(num_divisoes);

    while ((num_inicio + 2) <= num_final)
    {
        snprintf(name1, 30, "%d.dat", num_inicio);
        snprintf(name2, 30, "%d.dat", num_inicio + 1);
        snprintf(name3, 30, "%d.dat", num_final);
        unir(name1, name2, name3);
        num_inicio += 2;
        num_final += 1;
    }
    rename(name3, "cep_ordenado.dat");
    return 0;
}