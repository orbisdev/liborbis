#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <kernel.h>     // sceKernelUsleep()

#include "pieces.inl"
#include "primlib.h"

/* Constants */
#define DELAY 16
#define F_COLS 10
#define F_ROWS 20
#define VELOCITY 0.5

/* Function headers*/

void drawControlsAndScore(int, int);
static void initField();

static int setNewPieceInField(int);
void movePiece(int);
void moveLeft();
void moveRight();
int moveDown(int speed_multiplicator);
void rotatePiece();
void checkIfLine();
void endGame();

/* Global variables */
static int field[F_COLS][F_ROWS];
static int drawGrill;
static int playing; /* 1 keep playing, 0 exit */
static int actualC;
static int actualR;
double actualY;
static int actualPiece;
static int nextPiece;
static int rotation;
static int score;

/* Main function */
int init_tetris()
{
    /* Local variables */
    int keyPressed;

    /* Init the seed for random numbers */
    srand(time(NULL));

    /* Init graphics */
    //if(initGraph()) exit(3);

    /* Init variables */
    playing = 1;
    drawGrill = -1;
    rotation = 0;
    score = 0;

    /* Puts all the positions to 0 */
    initField();

    /* Getting the first piece */
    actualPiece = random()%7;
    nextPiece = random()%7;
    setNewPieceInField(actualPiece);

    /*do
    {
        // Clear screen
        filledRect(0, 0, ATTR_WIDTH-1, ATTR_HEIGHT-1, BLACK);

        keyPressed = pollkey();

        //1
        movePiece(keyPressed); // updates
        //2
        drawField(drawGrill);
        drawNextPiece();

        updateScreen();
        //SDL_Delay(DELAY);
    }while(playing == 1);
    */

    return 0;
}

/* ********************** DRAW FUNCTIONS ********************** */
void drawField(int drawGrill)
{
    double screenWidthPadding;
    double screenHeightPadding;
    int squareWidth;
    int counter_r;
    int counter_c;
    int padding;
    int increment;

    squareWidth = 20;
    screenWidthPadding  = (ATTR_WIDTH  - (F_COLS * squareWidth)) / 2;
    screenHeightPadding = (ATTR_HEIGHT - (F_ROWS * squareWidth)) / 2;

    /* Decoration of the field */
    rect(screenWidthPadding - 5,               screenHeightPadding - 5,
         screenWidthPadding + 20 * F_COLS + 4, screenHeightPadding + 20 * F_ROWS + 3, BLUE);
    rect(screenWidthPadding - 3,               screenHeightPadding - 3,
         screenWidthPadding + 20 * F_COLS + 6, screenHeightPadding + 20 * F_ROWS + 6, BLUE);

    padding = 30;
    increment = 30;

    drawControlsAndScore(padding, increment);

    for(counter_c = 0; counter_c < F_COLS; counter_c++)
    {
        for(counter_r = 0; counter_r < F_ROWS; counter_r++)
        {
            int x1 = screenWidthPadding + counter_c * 20;
            int x2 = screenWidthPadding + (counter_c + 1) * 20;
            int y1 = screenHeightPadding + counter_r * 20;
            int y2 = screenHeightPadding + (counter_r + 1) * 20;

            if(field[counter_c][counter_r] == 0)
            {
                if(drawGrill == 1)
                    rect(x1, y1, x2, y2, WHITE);
            }
            else
            {
                if(field[counter_c][counter_r] == 2)
                {
                    filledRect(x1, y1, x2, y2, YELLOW);
                    rect(x1, y1, x2, y2, BLACK);
                }
                else if(field[counter_c][counter_r] == 1)
                {
                    filledRect(x1, y1, x2, y2, GREEN);
                    rect(x1, y1, x2, y2, BLACK);
                }
                else if(field[counter_c][counter_r] == 3)
                {
                    filledRect(x1, y1, x2, y2, RED);
                }
            }
        }
    }
}

void drawControlsAndScore(int padding, int increment)
{
    double screenWidthPadding;
    double screenHeightPadding;
    int squareWidth;
    char str[100]; 

    squareWidth = 20;
    screenWidthPadding  = (ATTR_WIDTH  - (F_COLS * squareWidth)) / 2;
    screenHeightPadding = (ATTR_HEIGHT - (F_ROWS * squareWidth)) / 2;

    sprintf(str, "CONTROLS:");
    int tx = get_aligned_x(str, RIGHT);  // center text

    print_text(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "CONTROLS:");

    padding += increment;
    print_text(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "square");
    textout(ATTR_WIDTH - screenWidthPadding + 164, screenHeightPadding + padding, "pause music", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "help you", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "left", RED);
    textout(ATTR_WIDTH - screenWidthPadding + 164, screenHeightPadding + padding, "-> move the", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "piece to the left", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "right", RED);
    textout(ATTR_WIDTH - screenWidthPadding + 164, screenHeightPadding + padding, "-> move the", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "piece to the right", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "down", RED);
    textout(ATTR_WIDTH - screenWidthPadding + 164, screenHeightPadding + padding, "-> piece to floor", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "cross", RED);
    textout(ATTR_WIDTH - screenWidthPadding + 164, screenHeightPadding + padding, "-> rotate piece", YELLOW);
    padding += increment;
    textout(ATTR_WIDTH - screenWidthPadding + 25, screenHeightPadding + padding, "circle", RED);
    textout(ATTR_WIDTH - screenWidthPadding + 49, screenHeightPadding + padding, "resume music", YELLOW);
    padding += increment;

    sprintf(str, "%d", score);

    textout(50, screenHeightPadding + padding, "SCORE: ", YELLOW);
    textout(50 + 128, screenHeightPadding + padding, str, YELLOW);
}

void drawNextPiece()
{
    double screenWidthPadding;
    double screenHeightPadding;
    int squareWidth;
    int counterC;
    int counterR;

    int x1;
    int x2;
    int y1;
    int y2;

    squareWidth = 20;
    screenWidthPadding  = (ATTR_WIDTH  - (F_COLS * squareWidth)) / 8;
    screenHeightPadding = (ATTR_HEIGHT - (F_ROWS * squareWidth)) / 2;

    textout(screenWidthPadding, screenHeightPadding + 5, "NEXT PIECE:", YELLOW);

    rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * 4 + 10, screenHeightPadding + 20 * 6 + 5, RED);
    rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * 4 + 13, screenHeightPadding + 20 * 6 + 8, RED);

    for(counterC = 0; counterC < 4; counterC++)
    {
        for(counterR = 0; counterR < 4; counterR++)
        {
            x1 = screenWidthPadding  + (counterC + 1) * 20;
            x2 = screenWidthPadding  + (counterC + 2) * 20;
            y1 = screenHeightPadding + (counterR + 2) * 20;
            y2 = screenHeightPadding + (counterR + 3) * 20;

            if(pieces[nextPiece][0][counterC][counterR] == 1)
            {
                filledRect(x1, y1, x2, y2, GREEN);
                rect(x1, y1, x2, y2, BLACK);  // grill
            }else if(pieces[nextPiece][0][counterC][counterR] == 2)
            {
                filledRect(x1, y1, x2, y2, YELLOW);
                rect(x1, y1, x2, y2, BLACK);  // grill
            }
            if(drawGrill == 1) rect(x1, y1, x2, y2, BLACK);  // grill
        }
    }
}

/* INIT FIELD FUNCTION */
static void initField()
{
    int counter_c;
    for(counter_c = 0; counter_c < F_COLS; counter_c++)
    {
        int counter_f;
        for(counter_f = 0; counter_f < F_ROWS; counter_f++)
        {
            field[counter_c][counter_f] = 0;
        }
    }
}

/* LOGIC FUNCTIONS */
static int setNewPieceInField(int type)
{
    int counterC;
    int counterR;

    actualC = 3;
    actualR = 0;
    actualY = 0;
    rotation = 0;

    for(counterC = 0; counterC < 4; counterC++)
    {
        for(counterR = 0; counterR < 4; counterR++)
        {
            if(field[actualC + counterC][counterR] == 0)
            {
                field[actualC + counterC][counterR] = pieces[type][rotation][counterC][counterR];
            }else if(pieces[type][rotation][counterC][counterR] != 0)
                return -1;
        }
    }

    return 0;
}

void movePiece(int key)
{
    int wait;

    switch(key)
    {
        /*case SDLK_1:
            drawGrill = -drawGrill;
            break;
        case SDLK_LEFT:
            moveLeft();
            break;
        case SDLK_RIGHT:
            moveRight();
            break;
        case SDLK_SPACE:
            do
            {
                wait = moveDown();
            }
            while(wait != 1);
            break;
        case SDLK_RETURN:
            rotatePiece();
            break;
        case SDLK_ESCAPE:
            playing = 0;
            exit(0);
            break;*/
        default:
            moveDown(1);
            break;
    }
}

void moveLeft()
{
    int counterC;
    int counterR;
    int isPosible;

    isPosible = 1; /* 1 is posible, 0 no */

    for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
    {
        for(counterR = 0; counterR < 4 && isPosible == 1; counterR++)
        {
            if(actualC + counterC >= 0 && actualC + counterC < F_COLS)
            {
                int insidePosition;
                int insidePositionLeft;

                insidePosition = field[actualC + counterC][actualR + counterR];

                if(insidePosition == 1 || insidePosition == 2)
                {
                    if(actualC + counterC > 0)
                    {
                        insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];

                        if(insidePositionLeft != 3)
                        {
                            /* OK */
                        }
                        else
                        {
                            isPosible = 0;
                            break;
                        }
                    }
                    else
                    {
                        isPosible = 0;
                        break;
                    }
                }
            }
        }
    }

    if(isPosible == 1)
    {
        for(counterC = 0; counterC < 4; counterC++)
        {
            for(counterR = 0; counterR < 4; counterR++)
            {
                if(actualC + counterC >= 0 && actualC + counterC < F_COLS)
                {
                    int insidePosition;
                    int insidePositionRight;

                    insidePosition = field[actualC + counterC][actualR + counterR];

                    if(insidePosition == 1 || insidePosition == 2)
                    {
                        if(actualC + counterC > 0 && actualC + counterC < F_COLS)
                        {
                            field[actualC + counterC - 1][actualR + counterR] = insidePosition;

                            if(actualC + counterC + 1 < F_COLS)
                            {
                                insidePositionRight = field[actualC + counterC + 1][actualR + counterR];

                                if(insidePositionRight == 0 || insidePositionRight == 3)
                                {
                                    field[actualC + counterC][actualR + counterR] = 0;
                                }
                            }
                            else
                            {
                                field[actualC + counterC][actualR + counterR] = 0;
                            }

                        }
                    }
                }

            }
        }

        actualC -= 1;
    }

    sceKernelUsleep(100000);

    moveDown(1);
}

void moveRight()
{
    int counterC;
    int counterR;
    int isPosible;

    isPosible = 1; /* 1 is posible, 0 no */

    for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
    {
        for(counterR = 0; counterR < 4 && isPosible == 1; counterR++)
        {
            if(actualC + counterC < F_COLS)
            {
                int insidePosition;
                int insidePositionRight;

                insidePosition = field[actualC + counterC][actualR + counterR];

                if(insidePosition == 1 || insidePosition == 2)
                {
                    if(actualC + counterC + 1 < F_COLS)
                    {
                        insidePositionRight = field[actualC + counterC + 1][actualR + counterR];

                        if(insidePositionRight != 3)
                        {
                            /* OK */
                        }
                        else
                        {
                            isPosible = 0;
                            break;
                        }
                    }
                    else
                    {
                        isPosible = 0;
                        break;
                    }
                }
            }
        }
    }

    if(isPosible == 1)
    {
        for(counterC = 3; counterC >= 0; counterC--)
        {
            for(counterR = 0; counterR < 4; counterR++)
            {
                if(actualC + counterC  + 1 < F_COLS)
                {
                    int insidePosition;
                    int insidePositionLeft;

                    insidePosition = field[actualC + counterC][actualR + counterR];

                    if(insidePosition == 1 || insidePosition == 2)
                    {
                        if(actualC + counterC + 1 < F_COLS)
                        {
                            field[actualC + counterC + 1][actualR + counterR] = insidePosition;

                            if(actualC + counterC > 0)
                            {
                                insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];

                                if(insidePositionLeft == 0 || insidePositionLeft == 3)
                                {
                                    field[actualC + counterC][actualR + counterR] = 0;
                                }
                            }
                            else
                            {
                                field[actualC + counterC][actualR + counterR] = 0;
                            }

                        }
                    }
                }

            }
        }

        actualC += 1;
        debugNetPrintf(DEBUG, "%i\n", actualC);
    }

    sceKernelUsleep(100000);

    moveDown(1);
}

int moveDown(int speed_multiplicator)
{

    int counterC;
    int counterR;
    int isPosible;

    isPosible = 1;

    for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
    {
        for(counterR = 0; counterR < 4; counterR++)
        {
            if(actualR + counterR < F_ROWS && actualC + counterC < F_COLS)
            {
                if(field[actualC + counterC][actualR + counterR] == 1 || field[actualC + counterC][actualR + counterR] == 2)
                {
                    if(actualR + counterR > F_ROWS - 2)
                    {
                        isPosible = 0;
                        break;
                    }
                    else if(field[actualC + counterC][actualR + counterR + 1] == 3)
                    {
                        isPosible = 0;
                        break;
                    }
                }
            }
        }
    }

    actualY += VELOCITY * speed_multiplicator;

    if(actualY >= actualR * 20)
    {
        actualY = actualR * 20;

        if(isPosible == 1)
        {
            for(counterC = 0; counterC < 4; counterC++)
            {
                for(counterR = 3; counterR >= 0; counterR--)
                {
                    if(actualR + counterR + 1 < F_ROWS && actualC + counterC < F_COLS)
                    {
                        if(field[actualC + counterC][actualR + counterR] != 3 && field[actualC + counterC][actualR + counterR] != 0)
                        {
                            field[actualC + counterC][actualR + counterR + 1] = field[actualC + counterC][actualR + counterR];

                            if(actualR + counterR - 1 < 0 || field[actualC + counterC][actualR + counterR - 1] == 3)
                                field[actualC + counterC][actualR + counterR] = 0;
                            else
                                field[actualC + counterC][actualR + counterR] = field[actualC + counterC][actualR + counterR - 1];
                        }
                    }
                }
            }
            actualR += 1;
        }
        else
        {
            for(counterC = 0; counterC < 4; counterC++)
            {
                for(counterR = 0; counterR < 4 && actualR + counterR < F_ROWS; counterR++)
                {
                    if(actualR + counterR< F_ROWS && actualC + counterC < F_COLS)
                    {
                        if(field[actualC + counterC][actualR + counterR] == 1 || field[actualC + counterC][actualR + counterR] == 2)
                        {
                            field[actualC + counterC][actualR + counterR] = 3;
                        }
                    }

                }
            }

            actualPiece = nextPiece;
            nextPiece = random()%7;

            checkIfLine();

            if(setNewPieceInField(actualPiece) == -1)  endGame();
            return 1;
        }
    }
    return 0;
}

void rotatePiece()
{
    int pos;
    int pos_f;
    int c;
    int r;
    int y;
    int yActual;
    int exit;
    int isPosible;

    isPosible = 0;

    exit = 0;
    y = 99;
    yActual = 99;

    if(rotation == 3)
        rotation = 0;
    else
        rotation += 1;

    for(c = 0; c < 4 && y == 99; c++)
    {
        for(r = 0; r < 4; r++)
        {
            pos = pieces[actualPiece][rotation][c][r];

            if(pos == 2)
            {
                /*printf("Encuentra el centro\n");*/
                y = c;
                break;
            }
        }
    }

    for(c = 0; c < 4 && isPosible == 0; c++)
    {
        for(r = 0; r < 4; r++)
        {
            if(actualC + c >= 0 && actualC + c < F_COLS && actualR + r >= 0 && actualR + r < F_ROWS - 1)
            {
                pos_f = field[actualC + c][actualR + r];
                pos = pieces[actualPiece][rotation][c][r];

                debugNetPrintf(DEBUG, "posf: %i ---- pos: %i\n", pos_f, pos);

                if(pos_f == 3 && pos != 0)
                {
                    isPosible = 1;
                    debugNetPrintf(DEBUG, "entra, isposible = %i\n", isPosible);
                    break;
                }

                if(pos_f == 2)
                {
                    yActual = c;
                }

            }
            else
            {
                if(pos != 0)
                {
                    isPosible = 1;
                    break;
                }
            }

        }
    }

    debugNetPrintf(DEBUG, "yActual(%i) - y(%i) = %i\n", yActual, y, yActual - y);

    debugNetPrintf(DEBUG, "isposible %i\n", isPosible);
    if(actualC + yActual - y < 0 || isPosible == 1)
    {
        /* Imposible to do the rotation */
        rotation = rotation - 1;
    }
    else
    {
    debugNetPrintf(DEBUG, "isposible %i AUN ASI ENTRA\n", isPosible);
        for(c = 3; c >= 0 && exit != 1 && isPosible == 0; c--)
        {
            for(r = 3; r >= 0 && exit != 1; r--)
            {
                pos = pieces[actualPiece][rotation][c][r];

                if(actualC + c >= 0 && actualC + c < F_COLS)
                {
                    if(actualR + r >= 0 && actualR + r < F_ROWS - 1)
                    {
                        pos_f = field[actualC + c][actualR + r];

                        if(pos_f != 3)
                        {
                            /*printf("Efectua el cambio\n");*/
                            field[actualC + c][actualR + r] = pos;
                        }
                        else
                        {
                            if(pos != 0)
                                exit = 1;
                        }
                    }
                    else
                    {
                        if(pos != 0)
                            exit = 1;
                    }
                }
                else
                {
                    if(pos != 0)
                        exit = 1;
                }
            }
        }

        if(exit == 1)
            rotation = rotation - 1;

    }

}

void checkIfLine()
{
    int columns;
    int rows;

    int line;

    line = 1; /* 1 line complete, 0 no*/

    for(rows = F_ROWS - 1; rows >= 0; rows--)
    {
        line = 1;
        for(columns = 0; columns < F_COLS; columns++)
        {
            if(field[columns][rows] != 3)
            {
                line = 0;
                break;
            }
        }

        if(line == 1)
        {
            int rows2;
            score += 50;
            debugNetPrintf(DEBUG,"Score:%d\n", score);

            for(rows2 = rows; rows2 > 0; rows2--)
            {
                for(columns = 0; columns < F_COLS; columns++)
                {
                    if(rows2 - 1 == 0 )
                    {
                        field[columns][rows2 - 1] = 0;
                    }

                    field[columns][rows2] = field[columns][rows2 - 1];

                }
            }
            rows++;
        }
    }
}


void endGame()
{
    int key;
    char str[100];

    initField();

    sprintf(str, "%d", score);

    textout(ATTR_WIDTH / 3 + 145, ATTR_HEIGHT / 2 - 25, "SCORE", YELLOW);
    textout(ATTR_WIDTH / 3 + 215, ATTR_HEIGHT / 2 - 25, str, YELLOW);

    textout(ATTR_WIDTH / 3 + 25, ATTR_HEIGHT / 2 - 25, "GAME OVER", RED);
    textout(ATTR_WIDTH / 3 - 25, ATTR_HEIGHT / 2 , "PRESS ENTER TO PLAY AGAIN OR ESC TO EXIT", RED);

    debugNetPrintf(DEBUG,"gameEnd!, Final Score:%d\n", score);
    /*while(key != SDLK_RETURN && key != SDLK_ESCAPE)
    {
        key = getkey();
    }

    switch(key)
    {
        case SDLK_RETURN:
            playing = 1;
            actualPiece = random()%7;

            setNewPieceInField(actualPiece);
            break;
        case SDLK_ESCAPE:
            playing = 0;
            exit(0);
            break;
    }*/
    sceKernelUsleep(2000000);
    init_tetris();
}
