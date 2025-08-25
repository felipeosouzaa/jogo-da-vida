/****************************************************************************************
Trabalho Computacional - Programação Estruturada/Programação II - 2025/1
Felipe Souza
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  


struct tabuleiro {
    char nomeEntrada[256];   
    char nomeSaida[300];     
    int linhas;              
    int colunas;             
    int geracoes;            
    char **celulas;          
};


void abrirArquivo(struct tabuleiro *t);
void criarNomeSaida(struct tabuleiro *t);
int lerTabuleiro(struct tabuleiro *t);
char **alocarMatriz(int linhas, int colunas);
void liberarMatriz(char **m, int linhas);
int contarVizinhos(struct tabuleiro *t, int x, int y);
void rodarGeracoes(struct tabuleiro *t);
void escreverSaida(struct tabuleiro *t, FILE *arquivo, int numeroGeracao);

//as funções

//opção 1 
void abrirArquivo(struct tabuleiro *t) {
    FILE *file;
    char temp[256];

    printf("Digite o nome do arquivo de entrada: ");
    scanf("%s", temp);

    file = fopen(temp, "r");
    if (file == NULL) {
        printf("Falha ao abrir o arquivo! Voltando ao menu inicial!\n");
        return;
    }
    fclose(file);

    strcpy(t->nomeEntrada, temp); 
}

/*  cria o nome do arquivo d saída */
void criarNomeSaida(struct tabuleiro *t) {
    char *ponto = strchr(t->nomeEntrada, '.'); 
    int pos;

    if (ponto != NULL) {
        pos = ponto - t->nomeEntrada;                
        strncpy(t->nomeSaida, t->nomeEntrada, pos);  
        t->nomeSaida[pos] = '\0';                    
    } else {
        strcpy(t->nomeSaida, t->nomeEntrada); 
    }

    strcat(t->nomeSaida, "_out.txt"); 
}

// opção 2 
int lerTabuleiro(struct tabuleiro *t) {
    FILE *file;
    int i, j;
    char valor;

    file = fopen(t->nomeEntrada, "r");
    if (file == NULL) {
        printf("Falha ao abrir o arquivo para leitura!\n");
        return 0;
    }

    // faz as leituras
    fscanf(file, "%d %d %d", &t->linhas, &t->colunas, &t->geracoes);

    // alocamento da martriz
    t->celulas = alocarMatriz(t->linhas, t->colunas);

    // preenche a matriz
    for (i = 0; i < t->linhas; i++) {
        for (j = 0; j < t->colunas; j++) {
            fscanf(file, " %c", &valor);
            t->celulas[i][j] = valor; 
        }
    }

    fclose(file);
    return 1; 
}

// alocamento dinamico da matriz
char **alocarMatriz(int linhas, int colunas) {
    int i;
    char **m = (char **) malloc(linhas * sizeof(char *));
    if (m == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    for (i = 0; i < linhas; i++) {
        m[i] = (char *) malloc(colunas * sizeof(char));
        if (m[i] == NULL) {
            printf("Erro ao alocar memória!\n");
            exit(1);
        }
    }
    return m;
}



void liberarMatriz(char **m, int linhas) {
    int i;
    for (i = 0; i < linhas; i++) {
        free(m[i]);
    }
    free(m);
}

// cont vizinhos vivos


int contarVizinhos(struct tabuleiro *t, int x, int y) {
    int i, j;
    int vivos = 0;

    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            int nx = x + i;
            int ny = y + j;

            // verifica se vz e ele
            if (nx >= 0 && nx < t->linhas && ny >= 0 && ny < t->colunas) {
                if (!(nx == x && ny == y)) {
                    if (t->celulas[nx][ny] == '0') { // '0' = vivo
                        vivos++;
                    }
                }
            }
        }
    }
    return vivos;
}

//roda

void rodarGeracoes(struct tabuleiro *t) {
    FILE *out = fopen(t->nomeSaida, "w");
    if (out == NULL) {
        printf("Erro ao criar arquivo de saída!\n");
        return;
    }

    char **prox = alocarMatriz(t->linhas, t->colunas);

    int g, i, j;
    for (g = 0; g <= t->geracoes; g++) {
        
        escreverSaida(t, out, g);

        if (g < t->geracoes) {
            // calcula próxima geração
            for (i = 0; i < t->linhas; i++) {
                for (j = 0; j < t->colunas; j++) {
                    int vizinhos = contarVizinhos(t, i, j);
                    if (t->celulas[i][j] == '0') { 
                        if (vizinhos < 2 || vizinhos > 3) {
                            prox[i][j] = '.'; 
                        } else {
                            prox[i][j] = '0'; 
                        }
                    } else { // celula morta
                        if (vizinhos == 3) {
                            prox[i][j] = '0'; // nasce
                        } else {
                            prox[i][j] = '.'; //morta
                        }
                    }
                }
            }

           
            for (i = 0; i < t->linhas; i++) {
                for (j = 0; j < t->colunas; j++) {
                    t->celulas[i][j] = prox[i][j];
                }
            }
        }
    }

    liberarMatriz(prox, t->linhas);
    fclose(out);
}

//faz geração no arquivo
void escreverSaida(struct tabuleiro *t, FILE *arquivo, int numeroGeracao) {
    int i, j;
    fprintf(arquivo, "Geracao %d:\n", numeroGeracao);

    for (i = 0; i < t->linhas; i++) {
        for (j = 0; j < t->colunas; j++) {
            fprintf(arquivo, "%c ", t->celulas[i][j]); 
        }
        fprintf(arquivo, "\n");
    } 
}



//jogar tudo na main agr






int main() {
    struct tabuleiro jogo;
    int opcao = 0;
    int arquivoAberto = 0;
    int tabuleiroLido = 0;

    while (1) {
        printf("Menu:\n");
        printf("1 - Abrir arquivo de entrada\n");
        printf("2 - Preencher tabuleiro\n");
        printf("3 - Rodar geracoes\n");
        printf("-1 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == -1) {
            break;
        }

        if (opcao == 1) {
            abrirArquivo(&jogo);
            criarNomeSaida(&jogo);
            arquivoAberto = 1;
        }
        else if (opcao == 2) {
            if (!arquivoAberto) {
                printf("Arquivo nao foi aberto ainda!\n");
            } else {
                if (lerTabuleiro(&jogo)) {
                    tabuleiroLido = 1;
                }
            }
        }
        else if (opcao == 3) {
            if (!tabuleiroLido) {
                printf("Tabuleiro nao foi carregado ainda!\n");
            } else {
                rodarGeracoes(&jogo);
            }
        }
        else {
            printf("Opcao invalida!\n");
        }
    }

    if (tabuleiroLido) {
        liberarMatriz(jogo.celulas, jogo.linhas);
    }

    return 0;
}