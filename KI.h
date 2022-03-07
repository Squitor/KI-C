#ifndef HEADER_FILE
#define HEADER_FILE

#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
#include "time.h"
#include "math.h"

 typedef struct{
    int input;
    int output;
    int width;
    int length;
    int arraylength;
    int stable_size;
 } genom_parameter;

void init_genom_parameter(genom_parameter* _genom_para, int _input, int _output, int _width, int _length, int _stable_size);

float** stable_init(genom_parameter _genom_para);

void stable_knecht(genom_parameter _genom_para,float** _stable, float* _fitness,float major_c, float minor_c, float minor_f,int surivors );

float* brain(genom_parameter _genom_para, float* _genom, float* input);

#endif

/*
Example Implementation:

srand(10);
genom_parameter genom_para;
init_genom_parameter(&genom_para,2,2,2,2,10);
float** stable = stable_init(genom_para);
float* fitness = malloc(10*sizeof(float));
game(genom_para,stable,&fitness); //some implementation of a game wich uses the brain() funktion to calculate its plays;
stable_knecht(genom_para,stable, fitness,0.1,0.4,0.2,4);
*/
