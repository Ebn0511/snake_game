# SNAKE_GAME

## Introdução
Este é um Jogo bastante conhecido e jogado por diversas pessoas pelo mundo, conhecido no brasil como jogo da cobrinha. Por conta de sua alta jogabilidade, decidimos faze-lo e encontramos algumas dificuldades.Dentro das dificuldades, tivemos como principais, a implementação do movimento da cobra e a construção das bordas do jogo.
## Compilação
Certifique-se de ter um compilador C instalado(GCC). Compile o codigo-fonte com o seguinte comando:
gcc -o snake_game main.c screen.c keyboard.c timer.c
Execução: ./snake_game
## Controles
Use as teclas 'W','A','S' e 'D'. Onde 'W' movimenta a cobra pra cima, 'A' movimenta a cobra pra esquerda, 'S' movimenta a cobra pra baixo e 'D' movimenta a cobra pra direita.
Para sair do jogo, pressione a tecla 'Q'.
## Funcionalidades
- A cobra inicia no centro da tela.
- O coração aparece aleatoriamente no campo.
- A cobra cresce ao comer o coração.
- O jogo termina se a cobra colidir consigo mesma.
## Bibliotecas
- stdio.h
- stdlib.h
- string.h
- screen.h
- keyboard.h
- timer.h
