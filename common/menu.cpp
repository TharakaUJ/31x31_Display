#include "menu.h"
#include "displayNumbers.h"
#include "pixel.h"
#include "controllerEndpoints.h"
#include "../games/gamesList.h"


void menu_init() {
    int gameId = 0;
    int gameCount = 2;

    // Initialize menu items
    drawCenteredTwoDigitNumber(gameId, Pixel{0, 0, 255});

    while (true) {
        // Update menu based on user input
        if (commandFlags[CMD_UP]) {
            gameId = (gameId - 1 + gameCount) % gameCount;
            commandFlags[CMD_UP] = false; // Reset command flag
            drawCenteredTwoDigitNumber(gameId, Pixel{0, 0, 255});
        }
        if (commandFlags[CMD_DOWN]) {
            gameId = (gameId + 1) % gameCount;
            commandFlags[CMD_DOWN] = false; // Reset command flag
            drawCenteredTwoDigitNumber(gameId, Pixel{0, 0, 255});
        }
        if (commandFlags[CMD_SELECT]) {
            // Start the selected game
            commandFlags[CMD_SELECT] = false; // Reset command flag


            switch (gameId) {
                case 0:
                    // Start Snake Game
                    setupSnakeGame();
                    break;
                case 1:
                    setupGalaxiaGame();
                    break;
                default:
                    break;
            }
            break;
        }
    }
    {
        /* code */
    }
    
    
}

void menu_update() {
    // 
}