#include "display.h"
#include "controllerEndpoints.h"
#include "displayNumbers.h"
#include "menu.h"
#include "platform.h"
#include <vector>
#include <cstdlib>

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
bool gameWon = false;
int score = 0;

// Places food on a random free cell. Returns false if the snake fills the
// whole board and there is nowhere left to put it.
bool spawnFood()
{
    static bool occupied[WIDTH][HEIGHT];
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            occupied[x][y] = false;

    int freeCells = WIDTH * HEIGHT;
    for (auto &s : snake)
    {
        if (!occupied[s.x][s.y])
        {
            occupied[s.x][s.y] = true;
            freeCells--;
        }
    }

    if (freeCells <= 0)
        return false;

    // Pick the n-th free cell so this always terminates
    int n = rand() % freeCells;
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (!occupied[x][y] && n-- == 0)
            {
                food.x = x;
                food.y = y;
                return true;
            }
        }
    }
    return false;
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
    if (!gameWon)
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
        // Grow
        score++;
        if (!spawnFood())
        {
            // Snake fills the board: player wins
            gameWon = true;
            gameOver = true;
        }
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
    gameWon = false;
    spawnFood();

    while (true)
    {
        if (commandFlags[CMD_BACK])
        {
            commandFlags[CMD_BACK] = false;
            menu_init();
            platformDeleteCurrentThread();
            return;
        }

        if (!gameOver)
        {
            gameTick();
        }
        else
        {
            // Show game over visual (green if the board was filled)
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    framebuffer[i][j] = gameWon ? CRGB::Green : CRGB::Red;

            // Display score
            drawCenteredTwoDigitNumber(score, CRGB::White);
        }
        platformDelay(std::max(100, 500 - score * 20)); // Game speed constrained to 100
    }
}

void setupSnakeGame()
{
    score = 0;

    createThread(
        "SnakeGameTask",
        [](void *)
        {
            runSnakeGame();
        },
        nullptr,
        4096,  // Stack size
        1      // Priority
    );
}