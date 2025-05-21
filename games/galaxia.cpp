#include "../common/controllerEndpoints.h"
#include "../common/displayNumbers.h"
#include "../common/pixel.h"
#include <vector>
#include "galaxia.h"

struct Point {
    int x, y;
};

std::vector<Point> bullets;
std::vector<Point> enemies;

Point player = {15, 30};  // Bottom center
bool gameOver = false;
int tickCount = 0;
int score = 0;

void clearFramebuffer() {
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            framebuffer[x][y] = Pixel{0, 0, 0};
}

void drawFrame() {
    clearFramebuffer();

    // Player
    framebuffer[player.x][player.y] = Pixel{0, 255, 255};  // Cyan

    // Bullets
    for (auto &b : bullets)
        framebuffer[b.x][b.y] = Pixel{255, 255, 255};  // White

    // Enemies
    for (auto &e : enemies)
        framebuffer[e.x][e.y] = Pixel{255, 0, 0};  // Red
}

void spawnEnemyRow() {
    for (int x = 2; x < WIDTH - 2; x += 3) {
        enemies.push_back(Point{x, 0});
    }
}

void moveBullets() {
    std::vector<Point> newBullets;
    for (auto &b : bullets) {
        Point nb = {b.x, b.y - 1};
        if (nb.y >= 0) newBullets.push_back(nb);
    }
    bullets = newBullets;
}

void moveEnemies() {
    std::vector<Point> newEnemies;
    for (auto &e : enemies) {
        Point ne = {e.x, e.y + 1};
        if (ne.y < HEIGHT) newEnemies.push_back(ne);
        else gameOver = true;
    }
    enemies = newEnemies;
}

void handleCollisions() {
    std::vector<Point> newBullets;
    std::vector<Point> newEnemies;

    for (auto &e : enemies) {
        bool hit = false;
        for (auto &b : bullets) {
            if (b.x == e.x && b.y == e.y) {
                hit = true;
                score++;
                break;
            }
        }
        if (!hit) newEnemies.push_back(e);
    }

    for (auto &b : bullets) {
        bool destroyed = false;
        for (auto &e : enemies) {
            if (b.x == e.x && b.y == e.y) {
                destroyed = true;
                break;
            }
        }
        if (!destroyed) newBullets.push_back(b);
    }

    enemies = newEnemies;
    bullets = newBullets;
}

void updatePlayer() {
    if (commandFlags[CMD_LEFT] && player.x > 0)
        player.x--;
    if (commandFlags[CMD_RIGHT] && player.x < WIDTH - 1)
        player.x++;
    if (commandFlags[CMD_SELECT])
        bullets.push_back(Point{player.x, player.y - 1});
    for (int i = 0; i < CMD_COUNT; i++)
        commandFlags[i] = false;
}

void runGalaxiaGame() {
    player = {15, 30};
    bullets.clear();
    enemies.clear();
    score = 0;
    tickCount = 0;
    gameOver = false;

    while (true) {
        if (commandFlags[CMD_BACK]) {
            commandFlags[CMD_BACK] = false;
            return;  // Exit game loop back to menu
        }

        if (!gameOver) {
            updatePlayer();
            moveBullets();
            if (tickCount % 10 == 0)
                moveEnemies();
            if (tickCount % 30 == 0)
                spawnEnemyRow();

            handleCollisions();
            drawFrame();
        } else {
            drawCenteredTwoDigitNumber(score, Pixel{255, 255, 255});
        }

        tickCount++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setupGalaxiaGame() {
    xTaskCreatePinnedToCore(
        [](void *) {
            runGalaxiaGame();
        },
        "GalaxiaGame", 4096, NULL, 1, NULL, 1);
}
