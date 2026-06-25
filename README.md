# Estruturas de Dados em C

Coleção de trabalhos práticos desenvolvidos em C com foco em técnicas de busca binária, indexação e ordenação externa com arquivos.

Os arquivos de dados utilizados (`cep.dat`, `cep_ordenado.dat` e `owid-covid-data.csv`) não estão incluídos neste repositório por excederem o limite de tamanho do GitHub. Os links para download estarão disponibilizados pelo documento.

---

## Estrutura do Repositório

```text
/
├── CEP indexado com árvore B/
│   ├── ArvoreB-Busca.c
│   ├── ArvoreB.c
│   ├── ArvoreB.h
│   ├── criacao_sorteada.c
│   └── criar_saida1.c
│   
│   
│
├── Processamento de CSV Covid/
│   ├── CSVParser.c
│   ├── CSVParser.h
│   └── trabalho.c
│
├── TrabalhoBuscaBinaria/
│   └── BuscaBinaria.c
│
├── TrabalhoIndice/
│   ├── cria_indice.c
│   ├── ordena_indice.c
│   ├── procurar.c
│   └── main.c
│
└── TrabalhoOrdenaIntercalando/
    ├── ordena_separando.c
    └── main.c

```

---

## Arquivos de Dados

Os arquivos abaixo são compartilhados via Google Drive e devem ser baixados antes de compilar e executar os programas.

| Arquivo | Descrição | Tamanho aproximado | Download |
| --- | --- | --- | --- |
| `cep.dat` | Base original com 700.000 registros de CEP | ~200 MB | <a href="https://drive.google.com/file/d/1saOlmaJ08UEAX-ncYWRwcAgjKM5-4kor/view?usp=sharing" target="_blank">cep.dat<a> |
| `cep_ordenado.dat` | Base com 700.000 CEPs previamente ordenados | ~200 MB | <a href="https://drive.google.com/file/d/1ls8G4af-MqTUGlBR9kFuyLxgVMwxMTKF/view?usp=sharing" target="_blank">cep_ordenado.dat<a> |
| `owid-covid-data.csv` | Base de dados global da Covid-19 | ~80.2 MB | <a href="https://drive.google.com/file/d/1wlvEX7vmLqbt6lN5_NNIXVY1B35_ZlOd/view?usp=sharing" target="_blank">owid-covid-data.csv<a> |

---

## Trabalho 1 - Busca Binária em Arquivo (TrabalhoBuscaBinaria)

### Descrição

Implementa o algoritmo de busca binária diretamente sobre o arquivo `cep_ordenado.dat`, que contém 700.000 registros de CEP em ordem crescente. O programa não carrega o arquivo inteiro na memória: a cada iteração, calcula a posição central do intervalo e realiza uma leitura pontual com `fseek`, reduzindo o número de acessos ao disco de forma logarítmica.

### Estrutura de Arquivos

```text
TrabalhoBuscaBinaria/
└── BuscaBinaria.c      // Implementação completa da busca binária sobre o arquivo

```

O arquivo `cep_ordenado.dat` deve ser baixado separadamente (link na tabela acima) e colocado no mesmo diretório antes da execução.

### Compilação e Execução

```bash
gcc BuscaBinaria.c -o buscabinaria
./buscabinaria 20260180

```

### Saída Esperada

```text
TRAVESSA NESTOR VITOR   TIJUCA   RIO DE JANEIRO   RIO DE JANEIRO   RJ   20260180

```

---

## Trabalho 2 - Criação e Uso de Índice (TrabalhoIndice)

### Descrição

Demonstra a técnica de indexação de arquivos: em vez de varrer o arquivo original a cada consulta, cria-se uma estrutura auxiliar de índice contendo apenas o CEP e a posição do registro no arquivo original. O índice é muito menor do que o arquivo completo, pode ser ordenado e permite buscas binárias eficientes. Ao encontrar o CEP no índice, o programa usa a posição armazenada para recuperar os dados completos com uma única leitura direta no `cep.dat`.

O arquivo `cep.dat` deve ser baixado separadamente (link na tabela acima) e colocado no mesmo diretório.

### Estrutura de Arquivos

```text
TrabalhoIndice/
├── cria_indice.c       // Lê o cep.dat e gera o indice.dat com CEP e posição de cada registro
├── ordena_indice.c     // Ordena o indice.dat em ordem crescente de CEP e gera indice_ordenado.dat
├── procurar.c          // Realiza busca binária no indice_ordenado.dat e recupera o registro no cep.dat
└── main.c              // Auxiliar: exibe as 100 primeiras entradas do indice.dat ou indice_ordenado.dat

```

### Fluxo de Execução

```text
cep.dat
   |
   v
cria_indice.c  -->  indice.dat  (CEP + posição, sem ordenação)
                        |
                        v
              ordena_indice.c  -->  indice_ordenado.dat  (CEP + posição, ordenado)
                                           |
                                           v
                                      procurar.c  -->  busca binária no índice
                                                           |
                                                           posição encontrada
                                                           |
                                                           v
                                                       leitura direta no cep.dat
                                                           |
                                                           v
                                                       dados completos do CEP

```

### Descrição Detalhada de Cada Arquivo

**cria_indice.c**
Percorre sequencialmente o `cep.dat` e, para cada registro lido, anota o CEP e o offset em bytes daquele registro dentro do arquivo. Grava essas duas informações em um novo arquivo `indice.dat`. O resultado é uma estrutura enxuta que mapeia cada CEP à sua localização exata no arquivo original.

**ordena_indice.c**
Lê o `indice.dat` inteiro na memória, ordena as entradas por CEP em ordem crescente e salva o resultado em `indice_ordenado.dat`. Como o índice armazena apenas CEP e posição, ele ocupa uma fração do espaço do arquivo original, tornando essa etapa rápida mesmo para grandes volumes.

**procurar.c**
Recebe um CEP como argumento, executa busca binária no `indice_ordenado.dat` e, ao localizar a entrada correspondente, usa a posição armazenada para fazer uma única leitura direta no `cep.dat` com `fseek`, retornando os dados completos do logradouro.

**main.c**
Arquivo auxiliar de visualização. Exibe as 100 primeiras linhas do `indice.dat` ou do `indice_ordenado.dat` para inspecionar o estado da indexação antes e depois da ordenação.

---

## Trabalho 3 - Ordenação Externa com Intercalação de Blocos (TrabalhoOrdenaIntercalando)

### Descrição

Implementa um algoritmo de ordenação externa para arquivos que não cabem inteiramente na memória RAM. O processo divide o `cep.dat` em blocos menores, ordena cada bloco individualmente na memória e, em seguida, intercala os blocos dois a dois em ciclos sucessivos até restar um único arquivo completamente ordenado.

O algoritmo é especialmente relevante para cenários reais: o `cep.dat` possui 700.000 registros e ocupa aproximadamente 200 MB, o que já exige cuidado com o uso de memória. Em bases com 200.000.000 ou mais registros, a ordenação inteira em memória se tornaria inviável e essa abordagem passa a ser a única opção prática.

O arquivo `cep.dat` deve ser baixado separadamente (link na tabela acima) e colocado no mesmo diretório.

### Estrutura de Arquivos

```text
TrabalhoOrdenaIntercalando/
├── ordena_separando.c  // Implementação completa da ordenação externa por intercalação de blocos
└── main.c              // Auxiliar: compara o arquivo gerado com o cep_ordenado.dat para validação

```

### Etapas do Algoritmo

**Etapa 1 - Divisão**
O `cep.dat` é dividido em k blocos de tamanho igual (preferencialmente potência de 2). Cada bloco é lido completamente na memória, ordenado com `qsort` e gravado em um arquivo temporário separado.

**Etapa 2 - Intercalação**
Os arquivos temporários são combinados dois a dois. A cada ciclo, o número de arquivos é reduzido pela metade e o tamanho de cada arquivo dobra. O processo se repete até restar um único arquivo, que é o `cep.dat` completamente ordenado.

```text
Divisão inicial (exemplo com k = 8 blocos):

bloco_0.tmp  bloco_1.tmp  bloco_2.tmp  bloco_3.tmp
bloco_4.tmp  bloco_5.tmp  bloco_6.tmp  bloco_7.tmp

Ciclo 1 - intercalação dois a dois:

bloco_01.tmp  bloco_23.tmp  bloco_45.tmp  bloco_67.tmp

Ciclo 2:

bloco_0123.tmp  bloco_4567.tmp

Ciclo 3:

cep_ordenado_resultado.dat

```

**main.c - Validação**
Percorre simultaneamente o arquivo gerado pelo `ordena_separando.c` e o `cep_ordenado.dat` (ordenado via `qsort` convencional) e compara os registros posição a posição, confirmando que os dois resultados são idênticos.

---

## Trabalho 4 - Processamento de CSV Covid (Processamento de CSV Covid)

### Descrição

Realiza a leitura de uma grande base de dados global sobre a Covid-19 em formato CSV e extrai o total absoluto de casos e mortes concentrados na América do Sul. O programa lida de forma segura com colunas contendo dados faltantes (nulos ou em branco) e contorna o limite de memória lendo o arquivo de forma particionada.

O arquivo `owid-covid-data.csv` deve ser baixado separadamente (link na tabela de Dados) e mantido na mesma pasta do código-fonte.

### Estrutura de Arquivos

```text
Processamento de CSV Covid/
├── CSVParser.c     // Biblioteca auxiliar para leitura e separação correta de valores em CSV
├── CSVParser.h     // Cabeçalhos da biblioteca
└── trabalho.c      // Arquivo principal que processa os chunks do arquivo e acumula os dados

```

### Fluxo de Execução

O programa não tenta carregar os ~80.2 MB de dados diretamente na memória. Em vez disso:

1. Um buffer de alocação de memória (caminhão de dados) puxa um bloco do arquivo de texto.
2. O `CSVParser` organiza as linhas lidas.
3. O `trabalho.c` verifica através de uma blindagem lógica se a linha possui todas as colunas necessárias e procura as strings "South America" ou "América do Sul" na respectiva coluna.
4. Os valores inteiros das colunas de casos e mortes daquela linha são convertidos e acumulados, e o processo se repete até o final do arquivo.

---

## Trabalho 5 - Intersecção de Arquivos com Árvore B (CEP indexado com árvore B)

### Descrição

Demonstra na prática a funcionalidade e eficiência de uma Árvore B (estrutura central de bancos de dados) como Índice Sequencial Indexado (ISAM). O programa executa uma operação lógica de junção (*Join*) simulando dois bancos de dados grandes, criando um arquivo de saída (`saida1.dat`) que contém exclusivamente a intersecção de dados existentes simultaneamente em duas fontes.

### Estrutura de Arquivos

```text
CEP indexado com árvore B/
├── ArvoreB.c / ArvoreB.h     // Implementação de Árvore B (alocação de página, split, busca e inserção)
├── criacao_sorteada.c        // Executa passagens de inserção usando regras aleatórias
├── criar_saida1.c            // Realiza a intersecção eficiente dos arquivos criados
└── teste-insere.c / Busca.c  // Utilitários de manipulação da Árvore

```

### Fluxo de Geração e Intersecção

**1. Geração Sorteada (`criacao_sorteada.c`)**
O programa executa duas varreduras em cima do arquivo principal `cep.dat`:

* **Primeira varredura:** Sorteia e grava de forma sequencial o CEP e o índice no arquivo `indice1.dat`. A chance de um registro ser salvo é de 80%.
* **Segunda varredura:** Sorteia e insere os registros dentro da estrutura da Árvore B em um arquivo `arvore.dat`. A chance de inclusão também é de 80%, mas de forma totalmente independente do primeiro sorteio.

**2. A Junção (`criar_saida1.c`)**
Com os dois arquivos gerados, este programa lê as entradas do `indice1.dat` (o qual foi gerado sequencialmente) de forma linear. Para cada entrada, faz uma busca pontual dentro do `arvore.dat` utilizando o algoritmo `ArvoreB_Busca`. Se a busca retornar positivo, o registro provou existir em ambos os grupos e é copiado para o arquivo final de intersecção `saida1.dat`.

**A Matemática do Tamanho Final:**
Como as duas inserções funcionam à base de probabilidade independente:

* A probabilidade do registro ter entrado no arquivo 1 é de 80%.
* A probabilidade do mesmo registro ter entrado na Árvore B também é de 80%.
* A intersecção de dois eventos probabilísticos independentes é calculada por sua multiplicação: 80% * 80% = 64%.

Portanto, o `saida1.dat` resultará em aproximadamente 64% do tamanho total (e da quantidade de registros) do arquivo original `cep.dat`, comprovando a lógica da junção de dados.

---

## Ambiente e Compilação

Os programas foram desenvolvidos e testados em ambiente Windows com o compilador GCC.

```bash
gcc nome_do_arquivo.c -o nome_do_executavel
// Se for o processamento de csv
gcc nome_do_arquivo.c CSVParser.c -o nome_do_executavel
// Se for o Cep Indexado com árvore B
gcc nome_do_arquivo.c ArvoreB.c -o  nome_do_executavel

```

---

## Conceitos Abordados

* Busca binária em arquivo com acesso direto via `fseek` e `ftell`
* Indexação de arquivos e recuperação de registros por posição
* Ordenação externa por intercalação de blocos
* Gerenciamento de arquivos temporários em C
* Eficiência de acesso a disco em grandes volumes de dados
* Uso de `malloc` e bibliotecas utilitárias de leitura por *chunks* (`CSVParser`)
* Estruturação em Árvores B e manipulação paginada na memória
* Otimização de consultas complexas como Junções e Intersecções (Simulação de tabelas HASH/ISAM)


