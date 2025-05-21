#include "../common/controllerEndpoints.h"
#include "../common/displayNumbers.h"
#include <vector>
#include "../common/pixel.h"

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
int score = 0;

void clearFramebuffer()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            framebuffer[x][y] = Pixel{0, 0, 0};
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
        framebuffer[s.x][s.y] = Pixel{0, 255, 0}; // Green
    framebuffer[food.x][food.y] = Pixel{255, 0, 0}; // Red
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

    // Wrap-around logic
    if (newHead.x < 0)
        newHead.x = WIDTH - 1;
    else if (newHead.x >= WIDTH)
        newHead.x = 0;

    if (newHead.y < 0)
        newHead.y = HEIGHT - 1;
    else if (newHead.y >= HEIGHT)
        newHead.y = 0;

    // // Check for wall collision
    // if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT)
    // {
    //     gameOver = true;
    //     return;
    // }

    // Check for self-collision
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
        score++;
        if (score > 99)
            score = 99; // Limit score to 2 digits
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
        else
        {
            // Show game over visual
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    framebuffer[i][j] = Pixel{0, 0, 0}; // Red

            // Display score
            drawCenteredTwoDigitNumber(score, Pixel{255, 255, 255}); // Red
        }
        vTaskDelay(pdMS_TO_TICKS(std::max(100, 500 - score * 20))); // Game speed constrained to 100
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