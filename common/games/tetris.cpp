#include "display.h"
#include "controllerEndpoints.h"
#include "displayNumbers.h"
#include "tetris.h"
#include "menu.h"
#include <vector>
#include <cstdlib>

const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;
const int FIELD_OFFSET_X = (WIDTH - FIELD_WIDTH) / 2;
const int FIELD_OFFSET_Y = 5;

struct Block
{
    int x, y;
};

using Tetromino = std::vector<Block>;

Tetromino currentPiece;
std::vector<Block> settledBlocks;
int tickTetris = 0;
bool gameOverTetris = false;

Tetromino shapes[7] = {
    {{0, 0}, {1, 0}, {-1, 0}, {-2, 0}},  // I
    {{0, 0}, {-1, 0}, {1, 0}, {1, -1}},  // J
    {{0, 0}, {-1, 0}, {1, 0}, {-1, -1}}, // L
    {{0, 0}, {0, -1}, {1, 0}, {1, -1}},  // O
    {{0, 0}, {-1, 0}, {0, -1}, {1, -1}}, // S
    {{0, 0}, {-1, 0}, {1, 0}, {0, -1}},  // T
    {{0, 0}, {1, 0}, {0, -1}, {-1, -1}}, // Z
};

Tetromino spawnPiece()
{
    Tetromino shape = shapes[rand() % 7];
    for (auto &b : shape)
    {
        b.x += FIELD_WIDTH / 2;
        b.y += 0;
    }
    return shape;
}

void drawTetrisFrame()
{
    clearFramebuffer();

    // Draw current piece
    for (auto &b : currentPiece)
    {
        int gx = b.x + FIELD_OFFSET_X;
        int gy = b.y + FIELD_OFFSET_Y;
        if (gx >= 0 && gx < WIDTH && gy >= 0 && gy < HEIGHT)
            framebuffer[gx][gy] = CRGB::Blue;
    }

    // Draw settled blocks
    for (auto &b : settledBlocks)
    {
        int gx = b.x + FIELD_OFFSET_X;
        int gy = b.y + FIELD_OFFSET_Y;
        if (gx >= 0 && gx < WIDTH && gy >= 0 && gy < HEIGHT)
            framebuffer[gx][gy] = CRGB::White;
    }
}

bool checkCollision(const Tetromino &piece)
{
    for (auto &b : piece)
    {
        if (b.x < 0 || b.x >= FIELD_WIDTH || b.y >= FIELD_HEIGHT)
            return true;
        for (auto &s : settledBlocks)
        {
            if (b.x == s.x && b.y == s.y)
                return true;
        }
    }
    return false;
}

void settleCurrentPiece()
{
    for (auto &b : currentPiece)
        settledBlocks.push_back(b);
}

Tetromino movePiece(const Tetromino &piece, int dx, int dy)
{
    Tetromino moved = piece;
    for (auto &b : moved)
    {
        b.x += dx;
        b.y += dy;
    }
    return moved;
}

Tetromino rotatePiece(const Tetromino &piece)
{
    Tetromino rotated = piece;
    int cx = piece[0].x;
    int cy = piece[0].y;
    for (auto &b : rotated)
    {
        int x = b.x - cx;
        int y = b.y - cy;
        b.x = cx - y;
        b.y = cy + x;
    }
    return rotated;
}

void clearFullLines()
{
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--)
    {
        int count = 0;
        for (auto &b : settledBlocks)
        {
            if (b.y == y)
                count++;
        }

        if (count == FIELD_WIDTH)
        {
            // Remove that line
            settledBlocks.erase(
                std::remove_if(settledBlocks.begin(), settledBlocks.end(),
                               [y](Block &b)
                               { return b.y == y; }),
                settledBlocks.end());

            // Drop everything above
            for (auto &b : settledBlocks)
            {
                if (b.y < y)
                    b.y++;
            }

            y++; // Re-check same line
        }
    }
}

void updateTetrisPlayer()
{
    if (commandFlags[CMD_LEFT])
    {
        Tetromino moved = movePiece(currentPiece, -1, 0);
        if (!checkCollision(moved))
            currentPiece = moved;
    }
    if (commandFlags[CMD_RIGHT])
    {
        Tetromino moved = movePiece(currentPiece, 1, 0);
        if (!checkCollision(moved))
            currentPiece = moved;
    }
    if (commandFlags[CMD_DOWN])
    {
        Tetromino moved = movePiece(currentPiece, 0, 1);
        if (!checkCollision(moved))
            currentPiece = moved;
    }
    if (commandFlags[CMD_SELECT])
    {
        Tetromino rotated = rotatePiece(currentPiece);
        if (!checkCollision(rotated))
            currentPiece = rotated;
    }

    for (int i = 0; i < CMD_COUNT; i++)
        commandFlags[i] = false;
}

void runTetrisGame()
{
    currentPiece = spawnPiece();
    settledBlocks.clear();
    tickTetris = 0;
    gameOverTetris = false;

    while (true)
    {
        if (commandFlags[CMD_BACK])
        {
            commandFlags[CMD_BACK] = false;
            menu_init();
            vTaskDelete(NULL);
            return;
        }

        if (!gameOverTetris)
        {
            updateTetrisPlayer();

            if (tickTetris % 5 == 0)
            {
                Tetromino moved = movePiece(currentPiece, 0, 1);
                if (!checkCollision(moved))
                {
                    currentPiece = moved;
                }
                else
                {
                    settleCurrentPiece();
                    clearFullLines();
                    currentPiece = spawnPiece();
                    if (checkCollision(currentPiece))
                    {
                        gameOverTetris = true;
                    }
                }
            }

            drawTetrisFrame();
        }
        else
        {
            drawCenteredTwoDigitNumber(settledBlocks.size() / 4, CRGB::Red);
        }

        tickTetris++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setupTetrisGame()
{
    xTaskCreatePinnedToCore(
        [](void *)
        {
            runTetrisGame();
        },
        "TetrisGame", 4096, NULL, 1, NULL, 1);
}
