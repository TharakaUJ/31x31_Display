#include "display.h"
#include "controllerEndpoints.h"
#include "displayNumbers.h"
#include <vector>
#include "galaxia.h"
#include <cstdlib>

struct Point
{
    int x, y;
};

std::vector<Point> bullets;
std::vector<Point> enemies;

Point player = {15, 29}; // Bottom center
bool gameOverGalaxia = false;
int tickCount = 0;
int scoreGalaxia = 0;

void drawFrame()
{
    clearFramebuffer();

    // Player as a triangle
    if (player.y + 1 < HEIGHT)
        framebuffer[player.x][player.y] = CRGB::Blue; // Tip of triangle
    {
        if (player.x > 0)
            framebuffer[player.x - 1][player.y + 1] = CRGB::Blue; // Bottom-left
        if (player.x < WIDTH - 1)
            framebuffer[player.x + 1][player.y + 1] = CRGB::Blue; // Bottom-right
    }

    // Bullets
    for (auto &b : bullets)
        framebuffer[b.x][b.y] = CRGB::White;

    // Enemies
    for (auto &e : enemies)
        framebuffer[e.x][e.y] = CRGB::Green;
}

void spawnEnemyRow()
{
    for (int x = 2; x < WIDTH - 2; x += 3)
    {
        if (rand() % 2 == 0)
        { // 50% chance to spawn an enemy at this position
            enemies.push_back(Point{x, 0});
        }
    }
}

void moveBullets()
{
    std::vector<Point> newBullets;
    for (auto &b : bullets)
    {
        Point nb = {b.x, b.y - 1};
        if (nb.y >= 0)
            newBullets.push_back(nb);
    }
    bullets = newBullets;
}

void moveEnemies()
{
    std::vector<Point> newEnemies;
    for (auto &e : enemies)
    {
        Point ne = {e.x, e.y + 1};
        if (ne.y < HEIGHT)
            newEnemies.push_back(ne);
        else
            gameOverGalaxia = true;
    }
    enemies = newEnemies;
}

void handleCollisions()
{
    std::vector<Point> newBullets;
    std::vector<Point> newEnemies;

    for (auto &e : enemies)
    {
        bool hit = false;
        for (auto &b : bullets)
        {
            if (b.x == e.x && b.y == e.y)
            {
                hit = true;
                scoreGalaxia++;
                break;
            }
        }
        if (!hit)
            newEnemies.push_back(e);
    }

    for (auto &b : bullets)
    {
        bool destroyed = false;
        for (auto &e : enemies)
        {
            if (b.x == e.x && b.y == e.y)
            {
                destroyed = true;
                break;
            }
        }
        if (!destroyed)
            newBullets.push_back(b);
    }

    enemies = newEnemies;
    bullets = newBullets;
}

void updatePlayer()
{
    if (commandFlags[CMD_LEFT] && player.x > 0)
        player.x--;
    if (commandFlags[CMD_RIGHT] && player.x < WIDTH - 1)
        player.x++;
    if (commandFlags[CMD_SELECT])
        bullets.push_back(Point{player.x, player.y - 1});
    for (int i = 0; i < CMD_COUNT; i++)
        commandFlags[i] = false;
}

void runGalaxiaGame()
{
    player = {15, 29};
    bullets.clear();
    enemies.clear();
    scoreGalaxia = 0;
    tickCount = 0;
    gameOverGalaxia = false;

    while (true)
    {
        if (commandFlags[CMD_BACK])
        {
            commandFlags[CMD_BACK] = false;
            return; // Exit game loop back to menu
        }

        if (!gameOverGalaxia)
        {
            updatePlayer();
            moveBullets();
            if (tickCount % 10 == 0)
                moveEnemies();
            if (tickCount % 30 == 0)
                spawnEnemyRow();

            handleCollisions();
            drawFrame();
        }
        else
        {
            drawCenteredTwoDigitNumber(scoreGalaxia, CRGB::Red);
        }

        tickCount++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setupGalaxiaGame()
{

    scoreGalaxia = 0;

    xTaskCreatePinnedToCore(
        [](void *)
        {
            runGalaxiaGame();
        },
        "GalaxiaGame", 4096, NULL, 1, NULL, 1);
}