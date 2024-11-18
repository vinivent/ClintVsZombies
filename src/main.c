#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <time.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 72

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_BULLET YELLOW
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
    "############################                 ###########################",
    "############################                 ###########################",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "                                                                        ",
    "                                                                        ",
    "                                                                        ",
    "                                                                        ",
    "                                                                        ",
    "                                                                        ",
    "                                                                        ",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "###                                                                   ##",
    "############################                 ###########################",

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

struct Zombie *zombies;
int numZombies = 0;

void initZombie(struct Zombie *zombie)
{
    struct Position entrance = getRandomEntrance();
    zombie->coords = entrance;
    zombie->health = 1; // Defina a vida inicial do zumbi
}

void drawZombie(int x, int y)
{
    screenGotoxy(x, y);
    printf("🧟");
}

void drawClint(int x, int y)
{
    screenGotoxy(x, y);
    printf("🤠");
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

struct Bullet *bullets;
int reloadTime = 0;

int score = 0;

int checkCollision(int x1, int y1, int x2, int y2)
{
    return x1 == x2 && y1 == y2;
}

void spawnZombie(struct Clint *clint)
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
            clint->health--; 
            usleep(100000); 
            break;
        }
    }
}

void menu() {
    screenSetColor(WHITE, BLACK);
    printf("╔════════════════════════════════════╗\n");
    printf("║                                    ║\n");
    printf("║     🌵  BEM-VINDO AO JOGO!  🌵     ║\n");
    printf("║                                    ║\n");
    printf("║      1. Iniciar Jogo               ║\n");
    printf("║      2. Instruções                 ║\n");
    printf("║      3. Sair                       ║\n");
    printf("║                                    ║\n");
    printf("╚════════════════════════════════════╝\n");
}

void showInstructions() {
    system("clear"); 

    printf("╔════════════════════════════════════════╗\n");
    printf("║       📚 INSTRUÇÕES DO JOGO 📚         ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    printf("Use as teclas WASD para mover Clint (🤠) pelo mapa.\n");
    printf("Pressione 'f' para atirar!\n");
    printf("Evite os zumbis (🧟) e tente sobreviver!\n\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");

    getchar(); 
}

void showVictory() {
    screenSetColor(WHITE, BLACK);
    printf("╔════════════════════════════════════╗\n");
    printf("║             ════════════           ║\n");
    printf("║     🌵     ║ PARABÉNS! ║     🌵   ║\n");
    printf("║             ════════════           ║\n");
    printf("║    VOCÊ DERROTOU TODOS OS ZUMBIS   ║\n");
    printf("║         PARA JOGAR NOVAMENTE       ║\n");
    printf("║        PRESSIONE A TECLA  'R'      ║\n");
    printf("║             PARA SAIR              ║\n");
    printf("║        PRESSIONE A TECLA  'Q'      ║\n");
    printf("╚════════════════════════════════════╝\n");
}

void showDefeat() {
    screenClear();
    screenSetColor(RED, BLACK);
    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║      ════════════════════════      ║\n");
    printf("║  🪦 ║ QUE PENA, VOCÊ MORREU ║ 🪦   ║\n");
    printf("║      ════════════════════════      ║\n");
    printf("║ OS ZUMBIS CONSEGUIRAM TE ENCONTRAR ║\n");
    printf("║         PARA TENTAR NOVAMENTE      ║\n");
    printf("║        PRESSIONE A TECLA  'R'      ║\n");
    printf("║             PARA SAIR              ║\n");
    printf("║        PRESSIONE A TECLA  'Q'      ║\n");
    printf("╚════════════════════════════════════╝\n");
}

int main() {
    screenInit(0); keyboardInit(); timerInit(60);

    int option = 0, drawMenu = 0;
    while (1) {
        if (!drawMenu) { menu(); drawMenu = 1; }

        if (keyhit()) {
            int key = readch();
            if (key == '1') { system("clear"); break; }
            else if (key == '2') { showInstructions(); system("clear"); drawMenu = 0; }
            else if (key == '3') { system("clear"); return 0; }
        }
    }

    struct Clint clint;
    initClint(&clint);
    bullets = malloc(MAX_BULLETS * sizeof(struct Bullet));
    zombies = malloc(MAX_ZOMBIES * sizeof(struct Zombie));
    if (!bullets || !zombies) return -1;

    while (1) {
        if (timerTimeOver()) {
            screenDrawMap();
            drawClint(clint.coords.x, clint.coords.y);
            spawnZombie(&clint); checkClintDamage(&clint);

            if (score == 10) {
                screenClear();
                showVictory();
                
                while (1) {
                    if (keyhit()) {
                        int key = readch();
                        if (key == 'r') {
                            clint.health = 10;
                            clint.ammo = 8;
                            clint.coords.x = MAP_WIDTH / 2;
                            clint.coords.y = MAP_HEIGHT / 2;
                            score = numZombies = 0;
                            memset(bullets, 0, MAX_BULLETS * sizeof(struct Bullet));
                            memset(zombies, 0, MAX_ZOMBIES * sizeof(struct Zombie));
                            system("clear"); 
                            break;
                        } else if (key == 'q') {free(bullets); free(zombies);  return 0;}
                    }
                }
            }

            if (clint.ammo < MAX_BULLETS && reloadTime <= 0) { reloadTime = 2500 / 75; clint.ammo++; }
            else if (reloadTime > 0) reloadTime--;

            if (numZombies < MAX_ZOMBIES && rand() % 100 < 2) {
                for (int i = 0; i < MAX_ZOMBIES; i++) {
                    if (!zombies[i].onScreen) {
                         initZombie(&zombies[i]);
                          zombies[i].onScreen = 1;
                           numZombies++; 
                           break; 
                    }
                }
            }

            screenGotoxy(MAP_WIDTH / 2 - 3, MAP_HEIGHT); printf("💀: %d", score);

            screenGotoxy(0, MAP_HEIGHT); printf("🔫: %d", clint.ammo);

            screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT); printf("                             ");
            screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
            for (int i = 0; i < clint.health; i++) printf("❤️");

            if (clint.health <= 0) {
                screenGotoxy(MAP_WIDTH / 2 - 6, MAP_HEIGHT + 2);
                screenSetColor(RED, BLACK); showDefeat(); fflush(stdout);

                while (1) {
                    if (keyhit()) {
                        int key = readch();
                        if (key == 'r') { // Reset game
                            clint.health = 10;
                            clint.ammo = 8;
                            clint.coords.x = MAP_WIDTH / 2;
                            clint.coords.y = MAP_HEIGHT / 2;
                            score = numZombies = 0;
                            memset(bullets, 0, MAX_BULLETS * sizeof(struct Bullet));
                            memset(zombies, 0, MAX_ZOMBIES * sizeof(struct Zombie));
                            system("clear"); 
                            break;
                        } else if (key == 'q') { free(bullets); free(zombies); return 0; }
                    }
                }
            }

            fflush(stdout);
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].onScreen) { 
                    updateBullet(&bullets[i]); 
                    if (bullets[i].onScreen) {
                        drawBullet(bullets[i].coords.x, bullets[i].coords.y);
                    }
                }
            }
            screenUpdate();
        }

        if (keyhit()) {
            int key = readch(), newX = clint.coords.x, newY = clint.coords.y;

            if (key == 'w') newY--, clint.direction = 0;
            else if (key == 'a') newX--, clint.direction = 1;
            else if (key == 's') newY++, clint.direction = 2;
            else if (key == 'd') newX++, clint.direction = 3;
            else if (key == 'f' && clint.ammo > 0)
                for (int i = 0; i < MAX_BULLETS; i++)
                    if (!bullets[i].onScreen) { initBullet(&bullets[i], &clint); bullets[i].onScreen = 1; clint.ammo--; break; }

            if (!isWall(newX, newY) && !isWall(newX + 1, newY) && newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
                clint.coords.x = newX, clint.coords.y = newY;
        }
    }

    free(bullets); free(zombies);
    return 0;
}