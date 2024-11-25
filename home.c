// O que?
// O algoritmo utilizou matrizes dinâmicas para gerenciar o tabuleiro do jogo, estruturas struct para armazenar informações dos jogadores e dos rankings, 
// além de manipulação de arquivos para salvar os resultados das partidas. A lógica central do jogo inclui verificar vitórias, empates e validar jogadas.

// Como?
// A matriz dinâmica gerencia o tabuleiro, struct organiza dados de jogadores e rankings, e arquivos armazenam os resultados. 
// As jogadas são controladas via teclado, e funções determinam vitórias ou empates.

//Documentação:
//O código apresenta boa organização, com o uso de comentários para separar seções como funções principais, auxiliares e o main. 
//Funções e variáveis possuem nomes descritivos, mas poderia ser mais detalhado em partes cruciais, como a lógica de verificação do ganhador.

//Identação:
//A identação segue um padrão consistente em todas as funções, utilizando recuos claros que tornam o fluxo do código legível e bem estruturado.

//Clareza:
//O uso de estruturas como typedef para Jogador e Ranking contribui para a clareza, assim como funções que encapsulam ações específicas 
//(exibir menu, salvar ranking, verificar ganhador). Isso facilita a leitura e manutenção.

//Não economizar linhas de código:
//O código prioriza a legibilidade ao evitar condensar várias operações em uma única linha. Cada passo lógico está bem separado,
// respeitando o princípio de escrever código que seja fácil de entender.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

#define JOGADOR_X 'X'
#define JOGADOR_O 'O'
#define EMPATE 'E'
#define CARACTERE_BRANCO '_'

typedef struct {
    char nome[50];
    char simbolo;
} Jogador;

typedef struct {
    Jogador vencedor;
    Jogador perdedor;
} Ranking;

void exibirMenu();
void jogar(int contraComputador);
void verRanking();
void exibirCreditos();
void salvarRanking(Ranking ranking);
void reiniciarRanking();
int verificarGanhador(char **tabuleiro, int tamanho);
void exibirTabuleiro(char **tabuleiro, int tamanho);
void liberarTabuleiro(char **tabuleiro, int tamanho);
char **criarTabuleiroDinamico(int tamanho);

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");
    int opcao;

    while (1) {
        exibirMenu();
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar(0); 
                break;
            case 2:
                jogar(1); 
                break;
            case 3:
                verRanking();
                break;
            case 4:
                exibirCreditos();
                break;
            case 5:
                printf("\nSaindo do jogo...\n");
                exit(0);
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    }
}

void exibirMenu() {
    printf("\nMenu do Jogo da Velha\n");
    printf("1. Jogar contra outro jogador\n");
    printf("2. Jogar contra o computador\n");
    printf("3. Ver ranking\n");
    printf("4. Créditos\n");
    printf("5. Sair\n");
}


void jogar(int contraComputador) {
    int tamanho = 3;
    char **tabuleiro = criarTabuleiroDinamico(tamanho);
    char jogadorAtual = JOGADOR_X;
    char ganhador = EMPATE;
    int linha, coluna, jogadaValida, jogadas = 0;
    Jogador jogadorX, jogadorO;

    srand(time(NULL));

    if (!contraComputador) {
        printf("\nDigite o nome do Jogador X: ");
        scanf(" %[^\n]s", jogadorX.nome);
        jogadorX.simbolo = JOGADOR_X;

        printf("Digite o nome do Jogador O: ");
        scanf(" %[^\n]s", jogadorO.nome);
        jogadorO.simbolo = JOGADOR_O;
    } else {
        printf("\nDigite o nome do Jogador: ");
        scanf(" %[^\n]s", jogadorX.nome);
        jogadorX.simbolo = JOGADOR_X;

        strcpy(jogadorO.nome, "Computador");
        jogadorO.simbolo = JOGADOR_O;
    }

    while (1) {
        exibirTabuleiro(tabuleiro, tamanho);
        printf("\nJogador %c (%s), sua vez.\n", jogadorAtual, 
               (jogadorAtual == JOGADOR_X) ? jogadorX.nome : jogadorO.nome);

        if (contraComputador && jogadorAtual == JOGADOR_O) {
            do {
                linha = rand() % tamanho;
                coluna = rand() % tamanho;
            } while (tabuleiro[linha][coluna] != CARACTERE_BRANCO);
            printf("Computador escolheu: %d, %d\n", linha + 1, coluna + 1);
        } else {
            printf("Digite a linha e coluna (1-%d): ", tamanho);
            while (1) {
                scanf("%d %d", &linha, &coluna);

                // Verificando se a linha e a coluna estão dentro do intervalo permitido
                if (linha < 1 || linha > tamanho || coluna < 1 || coluna > tamanho) {
                    printf("Entrada inválida! A linha e a coluna devem estar entre 1 e %d. Tente novamente: ", tamanho);
                } else {
                    break; // Sai do loop se a entrada for válida
                }
            }
            linha--; // Ajuste para 0-indexed
            coluna--; // Ajuste para 0-indexed
        }

        jogadaValida = (linha >= 0 && linha < tamanho && coluna >= 0 && coluna < tamanho && tabuleiro[linha][coluna] == CARACTERE_BRANCO);
        if (!jogadaValida) {
            printf("\nJogada inválida! Tente novamente.\n");
            continue;
        }

        tabuleiro[linha][coluna] = jogadorAtual;
        jogadas++;

        ganhador = verificarGanhador(tabuleiro, tamanho);
        if (ganhador != EMPATE || jogadas == tamanho * tamanho) {
            break;
        }

        jogadorAtual = (jogadorAtual == JOGADOR_X) ? JOGADOR_O : JOGADOR_X;
    }

    exibirTabuleiro(tabuleiro, tamanho);
    if (ganhador != EMPATE) {
        printf("\nO jogador %c (%s) venceu!\n", ganhador, 
               (ganhador == JOGADOR_X) ? jogadorX.nome : jogadorO.nome);

        Ranking ranking = {
            .vencedor = (ganhador == JOGADOR_X) ? jogadorX : jogadorO,
            .perdedor = (ganhador == JOGADOR_X) ? jogadorO : jogadorX,
        };
        salvarRanking(ranking);
    } else {
        printf("\nO jogo terminou em empate!\n");
    }

    liberarTabuleiro(tabuleiro, tamanho);
}


void verRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    char linha[50];
    int opcao;

    if (!arquivo) {
        printf("\nNenhum ranking disponível.\n");
    } else {
        printf("\n=== Ranking ===\n");
        while (fgets(linha, sizeof(linha), arquivo)) {
            printf("%s", linha);
        }
        fclose(arquivo);
    }

    printf("\n1. Voltar ao menu\n");
    printf("2. Reiniciar Ranking\n");
    printf("3. Sair\n");

    while (1) {
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            return; 
        } else if (opcao == 2) {
            reiniciarRanking(); 
            return; 
        } else if (opcao == 3) {
            printf("\nSaindo do jogo...\n");
            exit(0); 
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

void salvarRanking(Ranking ranking) {
    FILE *arquivo = fopen("ranking.txt", "a");
    if (!arquivo) {
        printf("\nErro ao salvar no ranking.\n");
        return;
    }

    fprintf(arquivo, "Vencedor: %s (%c), Perdedor: %s (%c)\n", 
            ranking.vencedor.nome, ranking.vencedor.simbolo, 
            ranking.perdedor.nome, ranking.perdedor.simbolo);
    fclose(arquivo);
}

void reiniciarRanking() {
    FILE *arquivo = fopen("ranking.txt", "w");
    if (!arquivo) {
        printf("\nErro ao reiniciar o ranking.\n");
        return;
    }
    fclose(arquivo);
    printf("\nRanking reiniciado com sucesso!\n");
}

int verificarGanhador(char **tabuleiro, int tamanho) {
    int i; // Removido "j"

    for (i = 0; i < tamanho; i++) {
        if (tabuleiro[i][0] != CARACTERE_BRANCO && 
            tabuleiro[i][0] == tabuleiro[i][1] && 
            tabuleiro[i][1] == tabuleiro[i][2]) return tabuleiro[i][0];

        if (tabuleiro[0][i] != CARACTERE_BRANCO && 
            tabuleiro[0][i] == tabuleiro[1][i] && 
            tabuleiro[1][i] == tabuleiro[2][i]) return tabuleiro[0][i];
    }

    if (tabuleiro[0][0] != CARACTERE_BRANCO && 
        tabuleiro[0][0] == tabuleiro[1][1] && 
        tabuleiro[1][1] == tabuleiro[2][2]) return tabuleiro[0][0];

    if (tabuleiro[0][2] != CARACTERE_BRANCO && 
        tabuleiro[0][2] == tabuleiro[1][1] && 
        tabuleiro[1][1] == tabuleiro[2][0]) return tabuleiro[0][2];

    return EMPATE;
}


void exibirTabuleiro(char **tabuleiro, int tamanho) {
    printf("\nTabuleiro:\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

char **criarTabuleiroDinamico(int tamanho) {
    char **tabuleiro = (char **)malloc(tamanho * sizeof(char *));
    for (int i = 0; i < tamanho; i++) {
        tabuleiro[i] = (char *)malloc(tamanho * sizeof(char));
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j] = CARACTERE_BRANCO;
        }
    }
    return tabuleiro;
}

void liberarTabuleiro(char **tabuleiro, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

void exibirCreditos() {
    int opcao;

    printf("\n=== Créditos ===\n");
    printf("Jogo da Velha desenvolvido por Yuri Fernandes, Luiz Fernando, Pedro Soares, Murilo e Gabriel Nascimento.\n\n");
    printf("Este programa implementa um jogo da velha em C. O jogador pode optar por dois modos:\n");
    printf("- Modo jogador contra jogador (2 jogadores).\n");
    printf("- Modo jogador contra o computador.\n\n");
    printf("O jogo possui uma interface interativa no terminal que exibe o tabuleiro em tempo real e permite registrar os resultados no ranking.\n");
    printf("Também é possível consultar ou reiniciar o ranking a qualquer momento.\n");
    printf("Esperamos que você aproveite esta experiência!\n\n");

    printf("\n1. Voltar ao menu\n");
    printf("2. Sair\n");

    while (1) {
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            return; 
        } else if (opcao == 2) {
            printf("\nSaindo do jogo...\n");
            exit(0); 
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}
