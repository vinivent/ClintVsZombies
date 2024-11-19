// Include

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

// Define

#define MAP_HEIGHT 24
#define MAP_WIDTH 72

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_BULLET YELLOW
#define MAX_BULLETS 8
#define ZOMBIE_SPAWN_INTERVAL 30
#define GAME_DURATION 35 * 75


// Structs

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
    int onScreen;
};

// Bullets
struct Bullet
{
    struct Position coords;
    int direction;
    int onScreen;
};

struct Node{
    char *name;
    int score;
    struct Node *next; 
};


// Variáveis Globais

struct Bullet *bullets;
struct Zombie *zombies;
int zombieCapacity = 10;
int lastSpawnFrame = 0;
int numZombies = 0;
int reloadTime = 0;
int score = 0;


// Chamadas das funções

void screenDrawMap();
struct Position getRandomEntrance();
int isWall(int x, int y);
int checkCollision(int x1, int y1, int x2, int y2);

void drawBullet(int x, int y);
void initBullet(struct Bullet *bullet, struct Clint *clint);
void updateBullet(struct Bullet *bullet);

void initClint(struct Clint *clint);
void drawClint(int x, int y);
void checkClintDamage(struct Clint *clint);

void showStartArt();
void showInstructions();
void showVictory();
void showGameOver();

void initZombie(struct Zombie *zombie);
void drawZombie(int x, int y);
void updateZombie(struct Zombie *zombie, struct Clint *clint);
void spawnZombie(struct Clint *clint, int frameCount, double elapsedTime);

void addNode(struct Node **head, char *name, int score);
void saveLeaderboard(struct Node *head);
struct Node* loadLeaderboard(void); 
void showLeaderboard(void); 
void freeList(struct Node *head);


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


// Função Main

int main()
{
    screenInit(0);
    keyboardInit();
    timerInit(75);
    srand(time(NULL));
    struct Node *leaderboard = loadLeaderboard();


    int showMenu = 1;

    while (1)
    {
        showStartArt();
        while (1)
        {
            if (keyhit())
            {
                int key = readch();
                if (key == '1')
                {
                    screenClear();
                    showMenu = 0;
                    break;
                }
                else if (key == '2')
                {
                    showInstructions();
                    showMenu = 1;
                }
                else if (key == '3')
                {
                    screenClear();
                    return 0;
                }
            }
        }

        time_t startTime, currentTime;
        time(&startTime);
        double elapsedTime = 0;
        int frameCount = 0;

        struct Clint clint;
        initClint(&clint);

        bullets = malloc(MAX_BULLETS * sizeof(struct Bullet));
        zombies = malloc(zombieCapacity * sizeof(struct Zombie));
        if (!bullets || !zombies)
        {
            return 1;
        }

        for (int i = 0; i < zombieCapacity; i++)
        {
            zombies[i].onScreen = 0;
        }

        while (1)
        { // Game Loop
            time(&currentTime);
            elapsedTime = difftime(currentTime, startTime);
            if (timerTimeOver())
            {
                frameCount++;
                double remainingTime = GAME_DURATION / 75 - elapsedTime;
                

                screenDrawMap();
                drawClint(clint.coords.x, clint.coords.y);
                spawnZombie(&clint, frameCount, elapsedTime);
                checkClintDamage(&clint);

                if (clint.ammo < MAX_BULLETS && reloadTime <= 0)
                {
                    reloadTime = 2500 / 75;
                    clint.ammo++;
                }
                else if (reloadTime > 0)
                {
                    reloadTime--;
                }

                screenGotoxy(MAP_WIDTH / 2 - 3, MAP_HEIGHT);
                printf("💀: %d", score);
                screenGotoxy(0, MAP_HEIGHT);
                printf("🔫: %d", clint.ammo);
                screenGotoxy(MAP_WIDTH + 1, 1);
                printf("⌛: ");
                if (remainingTime < 10) {
                    printf("0%.0lf", remainingTime);
                } else {
                    printf("%.0lf", remainingTime);
                }
                screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
                printf("                             ");
                screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
                for (int i = 0; i < clint.health; i++)
                    printf("❤️");

                if (elapsedTime >= GAME_DURATION / 75)
                {
                    showVictory();
                    while (1)
                    {
                        if (keyhit())
                        {
                            int key = readch();
                            if (key == 'r')
                            {
                                char playerName[100];
                                printf("Insira seu nome: ");
                                scanf("%s", playerName);
                                addNode(&leaderboard, playerName, score);
                                saveLeaderboard(leaderboard);

                                clint.health = 10;
                                clint.ammo = 8;
                                clint.coords.x = MAP_WIDTH / 2;
                                clint.coords.y = MAP_HEIGHT / 2;
                                score = 0;
                                numZombies = 0;
                                memset(bullets, 0, MAX_BULLETS * sizeof(struct Bullet));
                                memset(zombies, 0, zombieCapacity * sizeof(struct Zombie)); // Reseta o array de zumbi
                                screenClear();
                                time(&startTime);   // Reseta o tempo
                                lastSpawnFrame = 0; // Reseta o último spawn frame
                                frameCount = 0;     // Reseta o frame count
                                break;
                            }
                            else if (key == 'q')
                            {
                                char playerName[100];
                                printf("Insira seu nome: ");
                                scanf("%s", playerName);
                                addNode(&leaderboard, playerName, score);
                                saveLeaderboard(leaderboard);
                                showLeaderboard();
                                
                                freeList(leaderboard);
                                free(bullets);
                                free(zombies);
                                return 0;
                            }
                        }
                    }
                    break;
                }

                if (clint.health <= 0)
                { // Morrer
                    showGameOver();
                    while (1)
                    {
                        if (keyhit())
                        {
                            int key = readch();
                            if (key == 'r')
                            {
                                clint.health = 10;
                                clint.ammo = 8;
                                clint.coords.x = MAP_WIDTH / 2;
                                clint.coords.y = MAP_HEIGHT / 2;
                                score = 0;
                                numZombies = 0;
                                memset(bullets, 0, MAX_BULLETS * sizeof(struct Bullet));
                                memset(zombies, 0, zombieCapacity * sizeof(struct Zombie));
                                screenClear();
                                time(&startTime);
                                lastSpawnFrame = 0;
                                frameCount = 0;
                                break;
                            }
                            else if (key == 'q')
                            {   
                                showLeaderboard();
                                free(bullets);
                                free(zombies);
                                return 0;
                            }
                        }
                    }
                    break;
                }

                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (bullets[i].onScreen)
                    {
                        updateBullet(&bullets[i]);
                        if (bullets[i].onScreen)
                        {
                            drawBullet(bullets[i].coords.x, bullets[i].coords.y);
                        }
                    }
                }
                screenUpdate();
            }

            if (keyhit())
            {
                int key = readch(), newX = clint.coords.x, newY = clint.coords.y;

                if (key == 'w')
                    newY--, clint.direction = 0;
                else if (key == 'a')
                    newX--, clint.direction = 1;
                else if (key == 's')
                    newY++, clint.direction = 2;
                else if (key == 'd')
                    newX++, clint.direction = 3;
                else if (key == 'f' && clint.ammo > 0)
                    for (int i = 0; i < MAX_BULLETS; i++)
                        if (!bullets[i].onScreen)
                        {
                            initBullet(&bullets[i], &clint);
                            bullets[i].onScreen = 1;
                            clint.ammo--;
                            break;
                        }
                if (!isWall(newX, newY) && !isWall(newX + 1, newY) && newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
                {
                    clint.coords.x = newX;
                    clint.coords.y = newY;
                }
            }
        }

        free(bullets);
        free(zombies);
        bullets = NULL;
        zombies = NULL;
    }

    return 0;
}


//////////////////// Funções do Jogo ////////////////////



// Função que desenha o mapa

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

    // Entradas do mapa 
    struct Position mapEntrances[] = {
        {MAP_WIDTH / 2, 2},              // Entrada 1: Topo
        {2, MAP_HEIGHT / 2},             // Entrada 2: Esquerda
        {MAP_WIDTH / 2, MAP_HEIGHT - 1}, // Entrada 3: Baixo
        {MAP_WIDTH - 2, MAP_HEIGHT / 2}  // Entrada 4: Direita
    };

    int numEntrances = 4;

    // Posição aleatória em uma das entradas
    struct Position getRandomEntrance()
    {
        int randomIndex = rand() % numEntrances;
        return mapEntrances[randomIndex];
}



// Função que desenha o zumbi

void drawZombie(int x, int y)
{
    screenGotoxy(x, y);
    printf("🧟");
}

void initZombie(struct Zombie *zombie)
{
    struct Position entrance = getRandomEntrance();
    zombie->coords = entrance;
    zombie->health = 1; // Vida inicial do zumbi
}


void drawClint(int x, int y)
{
    screenGotoxy(x, y);
    printf("🤠");
}



// Função para posicionar Clint no centro do mapa
void initClint(struct Clint *clint)
{
    clint->coords.x = MAP_WIDTH / 2;
    clint->coords.y = MAP_HEIGHT / 2;

    clint->health = 10;
    clint->ammo = MAX_BULLETS;
}

void drawBullet(int x, int y)
{
    screenSetColor(COLOR_BULLET, BLACK);
    screenGotoxy(x, y);
    printf("•");
}



// Função para iniciar a bala

void initBullet(struct Bullet *bullet, struct Clint *clint)
{
    if (clint->direction == 0)
    {
        bullet->direction = clint->direction; // bala criada com a direção que o Clint olha
        bullet->coords.x = clint->coords.x;
        bullet->coords.y = clint->coords.y - 1; // bala criada um espaço na frente do Clint
        bullet->onScreen = 1;                   // bala na tela ao atirar
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



// Função que verifica se uma posição é uma parede

int isWall(int x, int y)
{
    return (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#');
}



// Função Geral do Zumbi (movimentação e colisão)

void updateZombie(struct Zombie *zombie, struct Clint *clint) {

        // Calcula a diferença entre as coordenadas do zumbi e do Clint
        int dx = clint->coords.x - zombie->coords.x;
        int dy = clint->coords.y - zombie->coords.y;

        // Mov. zumbi ao Clint
        if (abs(dx) > abs(dy))
        {
            zombie->coords.x += (dx > 0) - (dx < 0);
        }
        else
        {
            zombie->coords.y += (dy > 0) - (dy < 0);
        }

        // Verifica colisão com a parede *DEPOIS* de mover o zumbi
        if (isWall(zombie->coords.x, zombie->coords.y))
        {
            // Colisão com parede -> desfaz movimento
            if (abs(dx) > abs(dy))
            {
                zombie->coords.x -= (dx > 0) - (dx < 0);
            }
            else
            {
                zombie->coords.y += (dy > 0) - (dy < 0);
            }
        }
}



// Função que atualiza a bala

void updateBullet(struct Bullet *bullet)
{
    // Coordenadas da bala antes dela se movimentar
    int nextX = bullet->coords.x;
    int nextY = bullet->coords.y;

    // Direção da bala 
    if (bullet->direction == 0)
    { // Cima
        nextY--;
    }
    if (bullet->direction == 1)
    { // Esquerda
        nextX--;
    }
    if (bullet->direction == 2)
    { // Baixo
        nextY++;
    }
    if (bullet->direction == 3)
    { // Direita
        nextX++;
    }

    // Posição fora dos limites ou colidindo com parede
    if (isWall(nextX, nextY) || nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT)
    {
        bullet->onScreen = 0; 
        return;              
    }

    bullet->coords.x = nextX;
    bullet->coords.y = nextY;
}



// Função que checa colisão

int checkCollision(int x1, int y1, int x2, int y2)
{
    return x1 == x2 && y1 == y2;
}



// Função que gera os zumbis

void spawnZombie(struct Clint *clint, int frameCount, double elapsedTime)
{

    if (elapsedTime < GAME_DURATION / 75 && frameCount - lastSpawnFrame >= ZOMBIE_SPAWN_INTERVAL)
    {
        for (int i = 0; i < zombieCapacity; i++)
        {
            if (!zombies[i].onScreen)
            {
                initZombie(&zombies[i]);
                zombies[i].onScreen = 1;
                lastSpawnFrame = frameCount;
                break;
            }
        }
    }

    for (int i = 0; i < zombieCapacity; i++) {
        if (zombies[i].onScreen && zombies[i].health > 0) {
            updateZombie(&zombies[i], clint);
            drawZombie(zombies[i].coords.x, zombies[i].coords.y);

            // Colisão zumbi + bala
            for (int j = 0; j < MAX_BULLETS; j++)
            {
                if (bullets[j].onScreen && checkCollision(bullets[j].coords.x, bullets[j].coords.y, zombies[i].coords.x, zombies[i].coords.y))
                {
                    bullets[j].onScreen = 0; 
                    zombies[i].health--;     
                    if (zombies[i].health <= 0)
                    {
                        zombies[i].onScreen = 0;
                    }
                    score++; 
                }
            }
        }
    }
}




// Função colisão Clint + Zumbi

void checkClintDamage(struct Clint *clint)
{
    for (int i = 0; i < zombieCapacity; i++)
    {
        if (zombies[i].onScreen &&
            checkCollision(clint->coords.x, clint->coords.y, zombies[i].coords.x, zombies[i].coords.y))
        {
            clint->health--;
            break;
        }
    }
}



// Tela Inicial

void showStartArt()
{
    screenClear();
    FILE *file = fopen("menu/start.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo start.txt\n");
        return;
    }

    screenSetColor(RED, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);

    screenSetColor(WHITE, BLACK);
    printf("\n\n\t\t\t  Bem vindo ao Clint Vs Zombies!\n");
    printf("\n\n\t\t\t\tEscolha uma opção:\n");
    printf("\t\t\t\t1. Iniciar Jogo\n");
    printf("\t\t\t\t2. Instruções\n");
    printf("\t\t\t\t3. Sair\n");
}


// Tela de Instruções

void showInstructions()
{
    screenClear();
    FILE *file = fopen("menu/instructions.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo start.txt\n");
        return;
    }

    screenSetColor(LIGHTBLUE, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}



// Tela de Vitória

void showVictory()
{
    screenClear();
    FILE *file = fopen("menu/victory.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo victory.txt\n");
        return;
    }

    screenSetColor(GREEN, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}



// Tela de Derrota

void showGameOver()
{
    screenClear();
    FILE *file = fopen("menu/gameover.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo gameover.txt\n");
        return;
    }

    screenSetColor(RED, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch); 
    }

    fclose(file);

    screenSetColor(WHITE, BLACK);
    printf("\n\n\t\t\t\t   Não foi dessa vez!\n");
    printf("\t\t\t\t   Pontuação Final: %d\n", score);
    printf("\t\t\tInsira r para reiniciar o jogo ou q para sair.\n");

}



// Lista Encadeada para o Leaderboard

void addNode(struct Node **head, char *name, int score) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->name = strdup(name); 
    newNode->score = score;
    newNode->next = NULL;

    if (*head == NULL || score > (*head)->score) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    struct Node *current = *head;
    while (current->next != NULL && score <= current->next->score) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}


void saveLeaderboard(struct Node *head) {
    FILE *file = fopen("leaderboard/leaderboard.txt", "w");
  
    struct Node *current = head;
    while (current != NULL) {
        fprintf(file, "%s %d\n", current->name, current->score);
        current = current->next;
    }

    fclose(file);
}

struct Node* loadLeaderboard() {
    FILE *file = fopen("leaderboard/leaderboard.txt", "r");

    struct Node *head = NULL;
    char name[100];
    int score;

    while (fscanf(file, "%s %d", name, &score) == 2) {
        addNode(&head, name, score);
    }

    fclose(file);
    return head;
}

void showLeaderboard() {
    FILE *file = fopen("leaderboard/leaderboard.txt", "r");
    
    if (file == NULL) {
        printf("Erro ao abrir o arquivo leaderboard.txt\n");
        return;
    }

    char name[50];
    int points;  

    printf("\n\n--- Leaderboard ---\n");
    printf("  Jogador:      Pontos:\n");
    printf("  ---------------------\n");
    
    while (fscanf(file, "%s %d", name, &points) == 2) {
        printf("%-12s %d\n", name, points);
    }

    fclose(file);
}


void freeList(struct Node *head) {
    struct Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->name); 
        free(temp);
    }
}

