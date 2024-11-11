#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <time.h>

#define MAP_HEIGHT 22
#define MAP_WIDTH 69

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_CLINT YELLOW
#define COLOR_ZOMBIE RED
#define COLOR_BULLET BLUE
#define MAX_ZOMBIES 10
#define MAX_BULLETS 8

// Posição no plano 2D
struct Position
{
    int x, y;
};

// Clint
struct Clint
{
    struct Position coords;
    int health;
    int direction;
    int ammo;
};

// Zombies
struct Zombie
{
    struct Position coords;
    int health;
    int stepCounter;
    int onScreen;
};

// Bullets
struct Bullet
{
    struct Position coords;
    int direction;
    int onScreen;
};

// Mapa do jogo
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "#########################                    ########################",
    "#########################                    ########################",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "                                                                     ",
    "                                                                     ",
    "                                                                     ",
    "                                                                     ",
    "                                                                     ",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "###                                                                ##",
    "#########################                    ########################",

};

void screenDrawMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            char cell = map[y][x];
            screenGotoxy(x, y);

            switch (cell)
            {
            case '#':
                screenSetColor(COLOR_WALL, BLACK);
                printf("▓");
                break;
            default:
                screenSetColor(COLOR_FLOOR, BLACK);
                printf(" ");
                break;
            }
        }
    }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}

// Entradas do mapa (defina as coordenadas das entradas)
struct Position mapEntrances[] = {
    {MAP_WIDTH / 2, 2},              // Entrada 1: Topo
    {2, MAP_HEIGHT / 2},             // Entrada 2: Esquerda
    {MAP_WIDTH / 2, MAP_HEIGHT - 1}, // Entrada 3: Baixo
    {MAP_WIDTH - 2, MAP_HEIGHT / 2}  // Entrada 4: Direita
};
int numEntrances = sizeof(mapEntrances) / sizeof(mapEntrances[0]);

// Função para gerar uma posição aleatória em uma das entradas
struct Position getRandomEntrance()
{
    int randomIndex = rand() % numEntrances;
    return mapEntrances[randomIndex];
}

struct Zombie zombies[MAX_ZOMBIES];
int numZombies = 0;

void initZombie(struct Zombie *zombie)
{
    struct Position entrance = getRandomEntrance();
    zombie->coords = entrance;
    zombie->health = 1; // Defina a vida inicial do zumbi
}

void drawZombie(int x, int y)
{
    screenSetColor(COLOR_ZOMBIE, BLACK);
    screenGotoxy(x, y);
    printf("▓");
}

void drawClint(int x, int y)
{
    screenSetColor(COLOR_CLINT, BLACK);
    screenGotoxy(x, y);
    printf("▓");
}

void drawBullet(int x, int y)
{
    screenSetColor(COLOR_BULLET, BLACK);
    screenGotoxy(x, y);
    printf("•");
}

// Função para posicionar Clint no centro do mapa
void initClint(struct Clint *clint)
{
    clint->coords.x = MAP_WIDTH / 2;
    clint->coords.y = MAP_HEIGHT / 2;

    clint->health = 10;
    clint->ammo = MAX_BULLETS;
}

void initBullet(struct Bullet *bullet, struct Clint *clint)
{
    if (clint->direction == 0)
    {
        bullet->direction = clint->direction; // Ela é criada com a direção que o Clint está olhando
        bullet->coords.x = clint->coords.x;
        bullet->coords.y = clint->coords.y - 1; // Ela é criada um espaço na frente do Clint
        bullet->onScreen = 1;                   // Atirou, então agora a bala está na tela
    }
    if (clint->direction == 1)
    {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x - 1;
        bullet->coords.y = clint->coords.y;
        bullet->onScreen = 1;
    }
    if (clint->direction == 2)
    {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x;
        bullet->coords.y = clint->coords.y + 1;
        bullet->onScreen = 1;
    }
    if (clint->direction == 3)
    {
        bullet->direction = clint->direction;
        bullet->coords.x = clint->coords.x + 1;
        bullet->coords.y = clint->coords.y;
        bullet->onScreen = 1;
    }
}

// Função para verificar se uma posição é uma parede
int isWall(int x, int y)
{
    return (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#');
}

void updateZombie(struct Zombie *zombie, struct Clint *clint)
{
    // Incrementa o contador de movimento do zumbi
    zombie->stepCounter++;

    // Define o intervalo de passos para o movimento
    int movementInterval = 3; // Zumbi move a cada 3 chamadas da função

    // Apenas move o zumbi se o contador de movimento atingir o intervalo
    if (zombie->stepCounter >= movementInterval)
    {
        // Reseta o contador para reiniciar o atraso de movimento
        zombie->stepCounter = 0;

        // Calcula a diferença entre as coordenadas do zumbi e do Clint
        int dx = clint->coords.x - zombie->coords.x;
        int dy = clint->coords.y - zombie->coords.y;

        // Move o zumbi em direção ao Clint (um passo de cada vez)
        if (abs(dx) > abs(dy))
        {
            zombie->coords.x += (dx > 0) ? 1 : -1;
        }
        else
        {
            zombie->coords.y += (dy > 0) ? 1 : -1;
        }

        // Verifica colisão com a parede *DEPOIS* de mover o zumbi
        if (isWall(zombie->coords.x, zombie->coords.y))
        {
            // Se colidir com a parede, desfaz o movimento
            if (abs(dx) > abs(dy))
            {
                zombie->coords.x -= (dx > 0) ? 1 : -1;
            }
            else
            {
                zombie->coords.y -= (dy > 0) ? 1 : -1;
            }
        }
    }
}

void updateBullet(struct Bullet *bullet)
{
    // Determine as próximas coordenadas da bala antes de movê-la
    int nextX = bullet->coords.x;
    int nextY = bullet->coords.y;

    // Dependendo da direção da bala, calcule a próxima posição
    if (bullet->direction == 0)
    { // Para cima
        nextY--;
    }
    if (bullet->direction == 1)
    { // Para a esquerda
        nextX--;
    }
    if (bullet->direction == 2)
    { // Para baixo
        nextY++;
    }
    if (bullet->direction == 3)
    { // Para a direita
        nextX++;
    }

    // Verifique se a próxima posição está fora dos limites ou é uma parede
    if (isWall(nextX, nextY) || nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT)
    {
        bullet->onScreen = 0; // A bala foi destruída
        return;               // Não mova a bala
    }

    // Se não houver colisão, atualize a posição da bala
    bullet->coords.x = nextX;
    bullet->coords.y = nextY;
}

struct Bullet bullets[MAX_BULLETS];
int reloadTime = 0;

int score = 0;

int checkCollision(int x1, int y1, int x2, int y2)
{
    return x1 == x2 && y1 == y2;
}

void updatesZombie(struct Clint *clint)
{
    for (int i = 0; i < MAX_ZOMBIES; i++)
    { // Iterar sobre todos os zumbis possíveis
        if (zombies[i].onScreen && zombies[i].health > 0)
        {
            updateZombie(&zombies[i], clint);
            drawZombie(zombies[i].coords.x, zombies[i].coords.y);

            for (int j = 0; j < MAX_BULLETS; j++)
            {
                if (bullets[j].onScreen && checkCollision(bullets[j].coords.x, bullets[j].coords.y, zombies[i].coords.x, zombies[i].coords.y))
                {
                    bullets[j].onScreen = 0;
                    zombies[i].health--;
                    score++;
                    if (zombies[i].health <= 0)
                    {
                        zombies[i].onScreen = 0;
                    }
                }
            }
        }
    }
}

void checkClintDamage(struct Clint *clint)
{
    for (int i = 0; i < numZombies; i++)
    {
        if (checkCollision(clint->coords.x, clint->coords.y, zombies[i].coords.x, zombies[i].coords.y))
        {
            clint->health--; // Reduz a vida de Clint
            // Talvez adicione uma pequena espera antes de causar dano novamente
            usleep(100000); // 100ms de espera, ajuste se necessário
            break;
        }
    }
}

int main()
{
    srand(time(NULL)); // Initialize random seed

    screenInit(0); // No borders this time
    keyboardInit();
    timerInit(75); // Update every 75ms
    struct Clint clint;
    initClint(&clint);

    while (1)
    {
        if (timerTimeOver())
        {
            screenDrawMap();

            // Draw Clint
            screenSetColor(COLOR_CLINT, BLACK);
            drawClint(clint.coords.x, clint.coords.y);

            updatesZombie(&clint);
            checkClintDamage(&clint);

            // Atualiza o tempo de recarga e verifica se há balas disponíveis
            if (clint.ammo < MAX_BULLETS && reloadTime <= 0)
            {
                reloadTime = 2500 / 75; // Recarrega uma bala a cada ciclo
                clint.ammo++;
            }
            else if (reloadTime > 0)
            {
                reloadTime--;
            }

            if (numZombies < MAX_ZOMBIES && rand() % 100 < 2)
            { // 5% de chance de gerar um zumbi a cada loop
                for (int i = 0; i < MAX_ZOMBIES; i++)
                {
                    if (!zombies[i].onScreen)
                    { // Encontra um slot vazio para um novo zumbi
                        initZombie(&zombies[i]);
                        zombies[i].onScreen = 1;
                        numZombies++;
                        break; // Sai do loop após criar um zumbi
                    }
                }
            }

            // Mostra a pontuação na tela
            screenGotoxy(MAP_WIDTH / 2 - 6, MAP_HEIGHT); // Centraliza o texto
            screenSetColor(WHITE, BLACK);
            printf("Eliminações: %d", score);

            // Mostra a munição na tela
            screenGotoxy(0, MAP_HEIGHT); // Posiciona o texto
            screenSetColor(WHITE, BLACK);
            printf("︻┳═-: %d", clint.ammo);

            // Limpa a linha dos corações antes de atualizar
            screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
            screenSetColor(RED, BLACK);
            printf("                      "); // Espaço suficiente para cobrir a linha

            // Exibe a quantidade correta de corações
            screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
            for (int i = 0; i < clint.health; i++)
            {
                printf("♥ ");
            }

            if (clint.health <= 0) {
                screenGotoxy(MAP_WIDTH / 2 - 6, MAP_HEIGHT + 2);
                screenSetColor(RED, BLACK);
                printf("GAME OVER!");
                fflush(stdout);
                break; // Sai do loop principal para encerrar o jogo
            }


            fflush(stdout); // Garante que a saída seja exibida imediatamente

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].onScreen)
                {
                    updateBullet(&bullets[i]);
                    if (bullets[i].onScreen)
                    { // Verifica novamente após a atualização
                        drawBullet(bullets[i].coords.x, bullets[i].coords.y);
                    }
                }
            }

            screenUpdate();
        }

        if (keyhit())
        {

            int key = readch();
            int newX = clint.coords.x;
            int newY = clint.coords.y;

            switch (key)
            {
            case 'w':
                newY--;
                clint.direction = 0;
                break;
            case 'a':
                newX--;
                clint.direction = 1;
                break;
            case 's':
                newY++;
                clint.direction = 2;
                break;
            case 'd':
                newX++;
                clint.direction = 3;
                break;
            case 'f':
                if (clint.ammo > 0)
                {
                    for (int i = 0; i < MAX_BULLETS; i++)
                    {
                        if (!bullets[i].onScreen)
                        {
                            initBullet(&bullets[i], &clint);
                            bullets[i].onScreen = 1;
                            clint.ammo--;
                            break;
                        }
                    }
                }
                break;
            }

            // Limitação das paredes
            if (!isWall(newX, newY) && newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
            {
                clint.coords.x = newX;
                clint.coords.y = newY;
            }
        }
    }

    return 0;
}
