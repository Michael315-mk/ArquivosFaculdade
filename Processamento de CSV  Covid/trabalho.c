#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVParser.h"

#define READ_BUF_SIZE 8192

typedef struct {
    long total_casos;
    long total_mortes;
} DadosCovid;

void processa_linha(char** cols, int ncols, void* userData) {
    DadosCovid* dados = (DadosCovid*) userData;

    if (ncols > 9) {
        if (strcmp(cols[1], "South America") == 0 || strcmp(cols[1], "América do Sul") == 0) {
            dados->total_casos += atol(cols[5]);
            dados->total_mortes += atol(cols[8]);
        }
    }
}

int main(){
    DadosCovid dados = {0,0};

    char *buf = (char*) malloc(READ_BUF_SIZE);

    CSVParser csv;
    CSVParser_init(&csv);

    FILE *f = fopen("owid-covid-data.csv", "rb");

    if (f != NULL) {
        int qt = fread(buf, 1, READ_BUF_SIZE, f);
        
        while (qt > 0) {
            CSVParser_processLines(&csv, buf, qt, processa_linha, &dados);
            qt = fread(buf, 1, READ_BUF_SIZE, f);
        }
        fclose(f);

        CSVParser_processLines(&csv, "\n", 1, processa_linha, &dados);
        printf("--- Dados da America do Sul ---\n");
        printf("Total de Casos: %ld\n", dados.total_casos);
        printf("Total de Mortes: %ld\n", dados.total_mortes);
    } else {
        printf("Não foi possivel abrir o arquivo CSV.\n");
    }


    free(buf);
    return 0;
}