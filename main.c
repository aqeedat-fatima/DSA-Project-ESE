#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "C:/raylib/raylib/examples/shapes/raygui.h"
#include <conio.h>
#include <ctype.h>

float posX, posY, boxheight = 60, boxwidth = 60, space = 7;
float keywidth = 40, keyheight = 50, keyspace = 6, count = 0; int total = -1;
const int screenWidth = 800;
const int screenHeight = 900;
int keyboard[3][10] = { 0 };
char keys[3][10] = { 0 };
Vector2 keylocation, charloc;
int mouseX, mouseY, click = 0, row = 0, col = 0;
char ch[2] = { 'a','\0' };
char letters[6][5];
int lettersRow = 0, lettersCol = 0;
int grid[6][5] = { 0 };
char Check[6] = "Check";
int gameWon = 0, gameLost = 0, menu = 1, instructions = 0;

char Word[50];
Font helvetica;

void InKeys(void);
void DrawInterface(void);
void DrawKeyboard(void);
void RandomWordGenerator(void);
void CompareWords(void);

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Wordle");

    SetTargetFPS(10);

    helvetica = GetFontDefault();
    InKeys();

    RandomWordGenerator();
    

    while (!WindowShouldClose())
    {

        DrawInterface();

        mouseX = GetMouseX();
        mouseY = GetMouseY();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            click = 1;
        else
            click = 0;

        if (gameWon == 1)
            click = 0;

        if (click && instructions) {
            if ((mouseX > 300 && mouseX < 500) && (mouseY > 600 && mouseY < 640)) {
                instructions = 0;
                continue;

            }
        }

        if (click && menu == 0) {
            if ((mouseX > 747 && mouseX < 773) && (mouseY > 72 && mouseY < 98)) {
                instructions = 1;
                continue;
            }
        }

        if (click && menu && instructions == 0) {
            if ((mouseX > 250 && mouseX < 550) && (mouseY > 350 && mouseY < 410)) {
                menu = 0;
                continue;
            }
            else if ((mouseX > 250 && mouseX < 550) && (mouseY > 430 && mouseY < 490)) instructions = 1;
            else if ((mouseX > 250 && mouseX < 550) && (mouseY > 510 && mouseY < 570)) break;
        }

        if (click == 1)
        {
            if (mouseX < ((screenWidth - ((keywidth + keyspace) * 10)) / 2))continue;
            if (mouseX > ((screenWidth - ((keywidth + keyspace) * 10)) / 2) + (10 * keywidth) + (9 * keyspace))continue;
            if (mouseY < 550 + keyheight + keyspace)continue;
            if (mouseY > 550 + 4 * keyheight + 3 * keyspace)continue;

            if (mouseY < 550 + 1.5 * keyspace + 2 * keyheight)row = 1;
            else if (mouseY < 550 + 2.5 * keyspace + 3 * keyheight)row = 2;
            else row = 3;

            if (row == 1)
            {
                mouseX -= ((screenWidth - (10 * keywidth + 9 * keyspace)) / 2);
                mouseX += keyspace + keywidth;
                col = mouseX / (keyspace + keywidth);
            }
            else if (row == 2)
            {
                mouseX -= (screenWidth - (9 * keywidth + 8 * keyspace)) / 2;
                mouseX += keyspace + keywidth;
                col = mouseX / (keyspace + keywidth);
                if (col < 1 || col>9)continue;
            }
            else
            {
                mouseX -= (screenWidth - (9 * keywidth + 8 * keyspace)) / 2;
                mouseX += keyspace + keywidth;
                col = mouseX / (keyspace + keywidth);
                if (col < 1)
                    col = 1;
                if (col > 9)
                    col = 9;

            }

        }
        else continue;


        if (keys[row - 1][col - 1] == '?' && lettersCol > 0) {
            lettersCol--;
            letters[lettersRow][lettersCol] = '\0';
        }
        else if (keys[row - 1][col - 1] == '?' && lettersCol == 0)
            lettersCol = lettersCol;
        else if (keys[row - 1][col - 1] == '!' && lettersCol == 5) {
            CompareWords();
            lettersRow++;
            lettersCol = 0;
        }
        else if (keys[row - 1][col - 1] == '!' && lettersCol != 5) {
            lettersCol = lettersCol;
        }
        else if (lettersCol < 5) {
            letters[lettersRow][lettersCol] = keys[row - 1][col - 1];
            lettersCol++;
        }


    }
    
    
    CloseWindow();

    return 0;
}

void DrawInterface(void)
{

    int value;

    BeginDrawing();

    ClearBackground(BLACK);

    if (instructions) {

        DrawText("Instructions", 310, 225, 30, WHITE);
        DrawLine(250, 270, 550, 270, WHITE);

        Rectangle instructionBox = { 150, 200, 500, 500 };
        DrawRectangleRoundedLines(instructionBox, 0.1, 16, 1, WHITE);

        Rectangle backbut = { 300, 600, 200, 40 };
        DrawRectangleRoundedLines(backbut, 0.2, 16, 1, WHITE);
        DrawText("Back", 380, 610, 20, WHITE);

        DrawText("- Guess the word in 6 tries", 180, 300, 22, WHITE);
        DrawText("- Each guess must be a five-letter", 180, 330, 22, WHITE);
        DrawText("  word", 180, 360, 22, WHITE);
        DrawText("- The color of the tiles change", 180, 390, 22, WHITE);
        DrawText("  to show how close your guess was", 180, 420, 22, WHITE);
        DrawText("- Green indicates correct letter and", 180, 450, 22, WHITE);
        DrawText("  position", 180, 480, 22, WHITE);
        DrawText("- Yellow indicates correct letter", 180, 510, 22, WHITE);
        DrawText("  but wrong position", 180, 540, 22, WHITE);

    }

    if (menu==1 && instructions == 0) {

        DrawText("WORDLE", 320, 250, 40, WHITE);
        Rectangle menuButton = { 250, 350, 300, 60};

        DrawRectangleRoundedLines(menuButton, 0.2, 4, 1, WHITE);
        DrawText("Start", 375, 370, 20, WHITE);
        menuButton.y += 80;
        DrawRectangleRoundedLines(menuButton, 0.2, 4, 1, WHITE);
        DrawText("Instructions", 345, 450, 20, WHITE);
        menuButton.y += 80;
        DrawRectangleRoundedLines(menuButton, 0.2, 4, 1, WHITE);
        DrawText("Quit", 385, 530, 20, WHITE);

    }

    if (menu == 0 && instructions == 0) {
        Vector2 head = { 330,50 };
        DrawTextEx(helvetica, "Wordle", head, 40, 2, WHITE);


        for (int i = 0; i < 6; i++)
        {
            posX = (screenWidth - ((boxwidth + space) * 5)) / 2;
            posY = 150 + (i * (boxheight + space));

            for (int j = 0; j < 5; j++)
            {
                value = grid[i][j];
                switch (value) {
                case 0: DrawRectangleLines(posX, posY, boxwidth, boxheight, DARKGRAY); break;
                case 1: DrawRectangle(posX, posY, boxwidth, boxheight, LIME); break;
                case 2: DrawRectangle(posX, posY, boxwidth, boxheight, GOLD); break;
                case 3: DrawRectangle(posX, posY, boxwidth, boxheight, LIGHTGRAY); break;
                }

                ch[0] = letters[i][j];
                keylocation.x = posX + 20, keylocation.y = posY + 20;
                DrawTextEx(helvetica, ch, keylocation, 30, 2, WHITE);

                posX += (boxheight + space);
            }
        }

        DrawLine(0, 115, screenWidth, 115, DARKGRAY);
        DrawCircleLines(760, 85, 13, GRAY);
        head.x = 756;
        head.y = 78;

        DrawTextEx(helvetica, "?", head, 20, 2, GRAY);

        DrawKeyboard();

        //To show word
        //DrawText(Word, 40, 100, 20, WHITE);

        if (gameWon) {

            Rectangle endScreen = { 150, 600, 500, 200 };
            DrawRectangleRounded(endScreen, 0.2, 2, BLACK);
            DrawRectangleRoundedLines(endScreen, 0.2, 2, 2, WHITE);
            DrawText("You have guessed the", 210, 650, 35, WHITE);
            DrawText("    word correctly!", 210, 690, 35, WHITE);
        }

        else if (gameLost) {
            Rectangle endScreen = { 150, 600, 500, 200 };
            DrawRectangleRounded(endScreen, 0.2, 2, BLACK);
            DrawRectangleRoundedLines(endScreen, 0.2, 2, 2, WHITE);
            DrawText("The word was: ", 215, 650, 35, WHITE);
            DrawText(Word, 480, 650, 35, WHITE);
        }

    }
    

    EndDrawing();
}

void DrawKeyboard(void) {

    int value = 0; Rectangle keyrec = { 0,0,0,0 };
    posX = ((screenWidth - (10 * keywidth + 9 * keyspace)) / 2);
    posY = 550 + keyheight + keyspace;
    for (int j = 0; j < 10; j++)
    {
        value = keyboard[0][j];
        keyrec.x = posX;
        keyrec.y = posY;
        keyrec.width = keywidth;
        keyrec.height = keyheight;

        switch (value) {
        case 0: DrawRectangleRounded(keyrec, 0.15, 4, GRAY); break;
        case 1: DrawRectangleRounded(keyrec, 0.15, 4, DARKGRAY); break;
        case 2: DrawRectangleRounded(keyrec, 0.15, 4, GOLD); break;
        case 3: DrawRectangleRounded(keyrec, 0.15, 4, LIME); break;
        }

        ch[0] = keys[0][j];
        keylocation.x = posX + keywidth / 2 - 4;
        keylocation.y = posY + keyheight / 2 - 5;
        DrawTextEx(helvetica, ch, keylocation, 15, 2, WHITE);
        posX += (keywidth + keyspace);

    }

    posX = ((screenWidth - (9 * keywidth + 8 * keyspace)) / 2);
    posY = 606 + keyheight + keyspace;
    for (int j = 0; j < 9; j++)
    {
        value = keyboard[1][j];
        keyrec.x = posX;
        keyrec.y = posY;
        keyrec.width = keywidth;
        keyrec.height = keyheight;

        switch (value) {
        case 0: DrawRectangleRounded(keyrec, 0.15, 4, GRAY); break;
        case 1: DrawRectangleRounded(keyrec, 0.15, 4, DARKGRAY); break;
        case 2: DrawRectangleRounded(keyrec, 0.15, 4, GOLD); break;
        case 3: DrawRectangleRounded(keyrec, 0.15, 4, LIME); break;
        }
        ch[0] = keys[1][j];
        keylocation.x = posX + keywidth / 2 - 4;
        keylocation.y = posY + keyheight / 2 - 5;
        DrawTextEx(helvetica, ch, keylocation, 15, 2, WHITE);
        posX += (keywidth + keyspace);
    }

    posX = ((screenWidth - (10 * keywidth + 9 * keyspace)) / 2);
    posY = 662 + keyheight + keyspace;
    float largkeywidth = 63.5;
    if (keyboard[2][0] == 0) DrawRectangle(posX, posY, largkeywidth, keyheight, GRAY);
    keylocation.x = posX + largkeywidth / 5 - 1;
    keylocation.y = posY + keyheight / 2 - 5;
    DrawTextEx(helvetica, "ENTER", keylocation, 12, 2, WHITE);


    posX = ((screenWidth - (10 * keywidth + 9 * keyspace)) / 2) + largkeywidth + keyspace;
    posY = 662 + keyheight + keyspace;
    for (int j = 1; j < 8; j++)
    {
        value = keyboard[2][j];
        keyrec.x = posX;
        keyrec.y = posY;
        keyrec.width = keywidth;
        keyrec.height = keyheight;

        switch (value) {
        case 0: DrawRectangleRounded(keyrec, 0.15, 4, GRAY); break;
        case 1: DrawRectangleRounded(keyrec, 0.15, 4, DARKGRAY); break;
        case 2: DrawRectangleRounded(keyrec, 0.15, 4, GOLD); break;
        case 3: DrawRectangleRounded(keyrec, 0.15, 4, LIME); break;
        }
        ch[0] = keys[2][j];
        keylocation.x = posX + keywidth / 2 - 4;
        keylocation.y = posY + keyheight / 2 - 5;
        DrawTextEx(helvetica, ch, keylocation, 15, 2, WHITE);

        posX += (keywidth + keyspace);
    }

    posX = (((screenWidth - (10 * keywidth + 9 * keyspace)) / 2)) + largkeywidth + keyspace + 7 * keywidth + 7 * keyspace;
    posY = 662 + keyheight + keyspace;
    if (keyboard[2][0] == 0) DrawRectangle(posX, posY, largkeywidth, keyheight, GRAY);
    keylocation.x = posX + largkeywidth / 3 - 1;
    keylocation.y = posY + keyheight / 2 - 5;
    DrawTextEx(helvetica, "DEL", keylocation, 12, 2, WHITE);


}

void InKeys(void) {

    keys[0][0] = 'Q';
    keys[0][1] = 'W';
    keys[0][2] = 'E';
    keys[0][3] = 'R';
    keys[0][4] = 'T';
    keys[0][5] = 'Y';
    keys[0][6] = 'U';
    keys[0][7] = 'I';
    keys[0][8] = 'O';
    keys[0][9] = 'P';
    keys[1][0] = 'A';
    keys[1][1] = 'S';
    keys[1][2] = 'D';
    keys[1][3] = 'F';
    keys[1][4] = 'G';
    keys[1][5] = 'H';
    keys[1][6] = 'J';
    keys[1][7] = 'K';
    keys[1][8] = 'L';
    keys[2][0] = '!';
    keys[2][1] = 'Z';
    keys[2][2] = 'X';
    keys[2][3] = 'C';
    keys[2][4] = 'V';
    keys[2][5] = 'B';
    keys[2][6] = 'N';
    keys[2][7] = 'M';
    keys[2][8] = '?';

}

void RandomWordGenerator(void)
{

    FILE* fp;
    fp = fopen("wordList (1).txt", "r");

    int line_count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++;
    }
    fclose(fp);
    fp = fopen("wordList (1).txt", "r");
    srand(time(0));
    int random_line = rand() % line_count;

    fseek(fp, 0, SEEK_SET);

    int current_line = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (current_line == random_line) {
            strcpy_s(Word, sizeof(Word), line);
            break;
        }
        current_line++;
    }
    fclose(fp);
}

void CompareWords(void) {

    for (int i = 0; i < 5; i++)
        Check[i] = letters[lettersRow][i];
    Check[5] = '\0';
    int r = 0, c = 0, correct = 0;

    for (int i = 0; i < 5; i++) {
        grid[lettersRow][i] = 3;
        for (int k = 0; k <= 2; k++)
            for (int l = 0; l < 10; l++) {
                if (k == 1 && l == 9) continue;
                if (k == 2 && l == 0) continue;
                if (k == 2 && l == 9) continue;
                if (keys[k][l] == Check[i]) {
                    r = k;
                    c = l;
                    break;
                }
            }
        if (keyboard[r][c] == 0)
        keyboard[r][c] = 1;

        for (int j = 0; j < 5; j++) {
            if (Check[i] == toupper(Word[j])) {
                if (i == j) {
                    grid[lettersRow][i] = 1;
                    keyboard[r][c] = 3;
                    correct++;
                    break;
                }
                else {
                    if (keyboard[r][c] != 3)
                    keyboard[r][c] = 2;
                    
                    grid[lettersRow][i] = 2;
                }
            }
        }
    }

    if (lettersRow == 5 && correct<5) gameLost = 1;
    if (correct == 5)
        gameWon = 1;


}