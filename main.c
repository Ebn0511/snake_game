#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define COLUNAS 40
#define LINHAS 19
#define MAX_SCORES 5

typedef struct {
    int x, y;
} Coordenada;

typedef struct {
    Coordenada *posicao;
    int tamanho;
} Cobra;

typedef struct {
    Coordenada posicao;
} Food;

Cobra cobra;
Food food;
int sair = 0;
int fimDoJogo = 0;
int direcaox = 1, direcaoy = 0;
int scores[MAX_SCORES];

void initializeGame();
void drawGame();
void updateGame();
void generateFood();
void saveScore(int score);
void loadScores();
void displayScores();

void generateFood() {
    int valid;
    do {
        valid = 1;
        food.posicao.x = rand() % COLUNAS;
        food.posicao.y = rand() % LINHAS;

        for (int i = 0; i < cobra.tamanho; i++) {
            if (cobra.posicao[i].x == food.posicao.x && cobra.posicao[i].y == food.posicao.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

void initializeGame() {
    screenHideCursor();
    keyboardInit();
    timerInit(55);  // Set the timer to 55ms updates
    loadScores();

    cobra.tamanho = 1;
    cobra.posicao = (Coordenada *)malloc(cobra.tamanho * sizeof(Coordenada));
    if (cobra.posicao == NULL) {
        printf("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    cobra.posicao[0].x = COLUNAS / 2;
    cobra.posicao[0].y = LINHAS / 2;
}

void drawBorders() {
    screenClear();
    printf("┌");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┐\n");
    for (int j = 0; j < LINHAS; j++) {
        printf("│");
        for (int i = 0; i < COLUNAS; i++)
            printf(" ");
        printf("│\n");
    }

    printf("└");
    for (int i = 0; i < COLUNAS; i++)
        printf("─");
    printf("┘");
}

void drawGame() {
    for (int i = 0; i < cobra.tamanho; i++) {
        screenGotoxy(cobra.posicao[i].x, cobra.posicao[i].y);
        printf("▓");
    }
    screenGotoxy(food.posicao.x, food.posicao.y);
    printf("❤");

    screenUpdate();
}

void updateGame() {
    if (fimDoJogo == 1) return;

    int newX = (cobra.posicao[0].x + direcaox + COLUNAS) % COLUNAS;
    int newY = (cobra.posicao[0].y + direcaoy + LINHAS) % LINHAS;

    if (newX == food.posicao.x && newY == food.posicao.y) {
        cobra.tamanho++;
        cobra.posicao = (Coordenada *)realloc(cobra.posicao, cobra.tamanho * sizeof(Coordenada));
        if (cobra.posicao == NULL) {
            perror("Erro ao realocar memória");
            exit(EXIT_FAILURE);
        }
        generateFood();
    } else {
        screenGotoxy(cobra.posicao[cobra.tamanho - 1].x, cobra.posicao[cobra.tamanho - 1].y);
        printf(" ");
    }

    for (int i = cobra.tamanho - 1; i > 0; i--) {
        cobra.posicao[i] = cobra.posicao[i - 1];
    }
    cobra.posicao[0].x = newX;
    cobra.posicao[0].y = newY;

    for (int i = 1; i < cobra.tamanho; i++) {
        if (cobra.posicao[i].x == newX && cobra.posicao[i].y == newY) {
            fimDoJogo = 1;
        }
    }

    screenGotoxy(newX, newY);
    printf("▓");
    screenUpdate();

    screenGotoxy(food.posicao.x, food.posicao.y);
    printf("❤");
    screenUpdate();
}

void saveScore(int score) {
    int i;
    for (i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i]) {
            break;
        }
    }

    if (i < MAX_SCORES) {
        for (int j = MAX_SCORES - 1; j > i; j--) {
            scores[j] = scores[j - 1];
        }
        scores[i] = score;

        FILE* file = fopen("scores.txt", "w");
        if (file != NULL) {
            for (int k = 0; k < MAX_SCORES; k++) {
                fprintf(file, "%d\n", scores[k]);
            }
            fclose(file);
        }
    }
}

void loadScores() {
    FILE* file = fopen("scores.txt", "r");
    if (!file) {
        for (int i = 0; i < MAX_SCORES; i++) {
            scores[i] = 0;
        }
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            fscanf(file, "%d", &scores[i]);
        }
        fclose(file);
    }
}

void displayScores() {
    printf("Top Scores:\n");
    for (int i = 0; i < MAX_SCORES; i++) {
        printf("%d: %d\n", i + 1, scores[i]);
    }
}

int main() {
    initializeGame();
    generateFood();
    drawBorders();
    drawGame();
    while (!sair) {
        if (timerTimeOver()) {
            if (keyhit()) {
                int tecla = readch();
                if (tecla == 27 || tecla == 'q') {
                    sair = 1;
                } else if (tecla == 'w' && direcaoy != 1) {
                    direcaox = 0;
                    direcaoy = -1;
                } else if (tecla == 'a' && direcaox != 1) {
                    direcaox = -1;
                    direcaoy = 0;
                } else if (tecla == 's' && direcaoy != -1) {
                    direcaox = 0;
                    direcaoy = 1;
                } else if (tecla == 'd' && direcaox != -1) {
                    direcaox = 1;
                    direcaoy = 0;
                }
            }

            updateGame();
            drawGame();
        }
    }

    screenGotoxy(COLUNAS / 2 - 5, LINHAS / 2);
    printf("Fim do Jogo! Pontuação: %d ponto(s) ", cobra.tamanho - 1);
    screenUpdate();
    getchar();

    saveScore(cobra.tamanho - 1);
    displayScores();

    free(cobra.posicao);
    screenShowCursor();
    keyboardDestroy();
    timerDestroy();
    return 0;
}




