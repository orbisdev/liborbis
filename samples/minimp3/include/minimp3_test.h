#include <stdio.h>
#include <math.h>
#include <string.h>
#include <strings.h>

void select_file(char *file_name);

void minimp3_Init(int channel);

int  minimp3_Load(char *input_file_name);
int  minimp3_Play(void);
void minimp3_Loop(char *selected);

void minimp3_End(void);
