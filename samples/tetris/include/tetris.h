#ifndef __TETRIS_H__
#define __TETRIS_H__


int init_tetris(void);

void drawField(int);
void drawNextPiece(void);

int  moveDown(int speed_multiplicator);
void moveLeft();
void moveRight();

void rotatePiece();

#endif // __TETRIS_H__
