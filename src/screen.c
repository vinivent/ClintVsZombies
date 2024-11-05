/**
 * screen.c
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include "screen.h"
#include <stdbool.h>

void screenDrawSquareWithEntrances(screenColor borderColor)
{
    screenClear();
    screenBoxEnable();

    // Dimensões do quadrado
    int screenWidth = MAXX - MINX + 1;
    int screenHeight = MAXY - MINY + 1;

    // Tamanho das entradas (proporcional às dimensões)
    int entranceWidth = screenWidth / 4;   // Ajusta o divisor para controlar o tamanho
    int entranceHeight = screenHeight / 4; // Ajusta o divisor para controlar o tamanho

    // Calcular posições das entradas (centralizadas)
    int entranceStartX = (screenWidth - entranceWidth) / 2 + MINX;
    int entranceStartY = (screenHeight - entranceHeight) / 2 + MINY;

    // Desenhar o quadrado com entradas
    for (int y = MINY; y <= MAXY; y++)
    {
        for (int x = MINX; x <= MAXX; x++)
        {
        screenSetColor(borderColor, BLACK);
            bool isEntrance = false;

            // Verificar se está em uma entrada
            if (
                (y == MINY && x >= entranceStartX && x < entranceStartX + entranceWidth) ||  // Entrada superior
                (y == MAXY && x >= entranceStartX && x < entranceStartX + entranceWidth) ||  // Entrada inferior
                (x == MINX && y >= entranceStartY && y < entranceStartY + entranceHeight) || // Entrada esquerda
                (x == MAXX && y >= entranceStartY && y < entranceStartY + entranceHeight)    // Entrada direita
            )
            {
                isEntrance = true;
            }

            screenGotoxy(x, y);

            if (isEntrance)
            {
                printf(" "); // Espaço para a entrada
            }
            else if ((x == MINX || x == MAXX) && (y == MINY || y == MAXY))
            { // Cantos
                if (x == MINX && y == MINY)
                    printf("%c", BOX_UPLEFT);
                else if (x == MAXX && y == MINY)
                    printf("%c", BOX_UPRIGHT);
                else if (x == MINX && y == MAXY)
                    printf("%c", BOX_DWNLEFT);
                else
                    printf("%c", BOX_DWNRIGHT);
            }
            else if (x == MINX || x == MAXX)
            { // Bordas verticais
                printf("%c", BOX_VLINE);
            }
            else if (y == MINY || y == MAXY)
            { // Bordas horizontais
                printf("%c", BOX_HLINE);
            }
            screenSetColor(WHITE, BLACK);
        }
    }

    screenBoxDisable();
}

void screenInit(int drawBorders)
{
    screenClear();
    if (drawBorders)
        screenDrawSquareWithEntrances(GREEN);
    screenHomeCursor();
    screenHideCursor();
}

void screenDestroy()
{
    printf("%s[0;39;49m", ESC); // Reset colors
    screenSetNormal();
    screenClear();
    screenHomeCursor();
    screenShowCursor();
}

void screenGotoxy(int x, int y)
{
    x = (x < 0 ? 0 : x >= MAXX ? MAXX - 1
                               : x);
    y = (y < 0 ? 0 : y > MAXY ? MAXY
                              : y);

    printf("%s[f%s[%dB%s[%dC", ESC, ESC, y, ESC, x);
}

void screenSetColor( screenColor fg, screenColor bg)
{
    char atr[] = "[0;";

    if ( fg > LIGHTGRAY )
    {
        atr[1] = '1';
		fg -= 8;
    }

    printf("%s%s%d;%dm", ESC, atr, fg+30, bg+40);
}