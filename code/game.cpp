#include "display.h"
#include "controllerEndpoints.h" // for commandFlags
#include <vector>

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Point
{
    int x, y;
};

std::vector<Point> snake;
Point food;
Direction dir = RIGHT;
bool gameOver = false;

void clearFramebuffer()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            framebuffer[x][y] = CRGB::Black;
        }
    }
}

void spawnFood()
{
    while (true)
    {
        food.x = random(0, WIDTH);
        food.y = random(0, HEIGHT);
        bool collision = false;
        for (auto s : snake)
        {
            if (s.x == food.x && s.y == food.y)
            {
                collision = true;
                break;
            }
        }
        if (!collision)
            break;
    }
}

void updateDirection()
{
    if (commandFlags[CMD_UP] && dir != DOWN)
        dir = UP;
    if (commandFlags[CMD_DOWN] && dir != UP)
        dir = DOWN;
    if (commandFlags[CMD_LEFT] && dir != RIGHT)
        dir = LEFT;
    if (commandFlags[CMD_RIGHT] && dir != LEFT)
        dir = RIGHT;

    // Reset flags so it’s only applied once
    for (int i = 0; i < CMD_COUNT; i++)
        commandFlags[i] = false;
}

void drawSnake()
{
    clearFramebuffer();
    for (auto &s : snake)
        framebuffer[s.x][s.y] = CRGB::Green;
    framebuffer[food.x][food.y] = CRGB::Red;
}

void gameTick()
{
    if (gameOver)
        return;

    updateDirection();

    // Move snake
    Point head = snake.front();
    Point newHead = head;

    switch (dir)
    {
    case UP:
        newHead.y--;
        break;
    case DOWN:
        newHead.y++;
        break;
    case LEFT:
        newHead.x--;
        break;
    case RIGHT:
        newHead.x++;
        break;
    }

    // Check collision
    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT)
        gameOver = true;

    for (auto &s : snake)
    {
        if (s.x == newHead.x && s.y == newHead.y)
            gameOver = true;
    }

    if (gameOver)
        return;

    // Move snake
    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y)
    {
        spawnFood(); // Grow
    }
    else
    {
        snake.pop_back(); // Don’t grow
    }

    drawSnake();
}

void runSnakeGame()
{
    snake = {{15, 15}, {14, 15}, {13, 15}};
    dir = RIGHT;
    gameOver = false;
    spawnFood();

    while (true)
    {
        if (!gameOver)
        {
            gameTick();
        }
        // else
        // {
        //     // Show game over visual
        //     for (int i = 0; i < WIDTH; i++)
        //         for (int j = 0; j < HEIGHT; j++)
        //             framebuffer[i][j] = CRGB::Red;
        // }
        vTaskDelay(pdMS_TO_TICKS(2000)); // Game speed
    }
}

void setupSnakeGame()
{
    xTaskCreatePinnedToCore(
        [](void *)
        {
            runSnakeGame();
        },
        "SnakeGame", 4096, NULL, 1, NULL, 1);
}