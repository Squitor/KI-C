#include "KI.h"
#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
#include "time.h"
#include "math.h"

/*
typedef struct{
    int input;
    int output;
    int width;
    int length;
    int arraylength;
    int stable_size;
} genom_parameter;
*/

/* Initialize the genom parameters

	 input		        length		    width
        ______ 	__	     width		      ______  __     __	    
       /a a a/ /1/	      __  __  __	     /a a a/ /1/    /1/
width /b b b/ /2/ ->  width  /x/ /x/ /x/ -> output  /b b b/ /2/  = /2/ output
     /c c c/ /3/				   /c c c/ /3/    /3/

     input width and output must be larger than 0 
     length may be 0
TODO:

Macros for print and brain

*/

void init_genom_parameter(genom_parameter* _genom_para, int _input, int _output, int _width, int _length, int _stable_size){
    
    int arraylength = (_input * _width + _length * _width * _width + _width * _output);
    _genom_para -> input = _input;
    _genom_para -> output = _output;
    _genom_para -> width = _width;
    _genom_para -> length = _length;
    _genom_para -> arraylength = arraylength;
    _genom_para -> stable_size = _stable_size;
}

/*Creates a genom from the genom parameters*/

float* genom_init(genom_parameter _genom_para){
    float* genom; 
    genom = (float*) malloc((_genom_para.arraylength)* sizeof(float));
    
    return genom;
}

/*Fills a genom with random values between -1 and 1*/

void random_soup(genom_parameter _genom_para, float* _genom){
    for (int i = 0; i < _genom_para.arraylength; i++){
	_genom[i] = rand()% 2000000 * 0.000001 - 1;
	//_genom[i] = (float) i;
    }
}

/*Prints genom for debuging purposes*/

void print_genom(genom_parameter _genom_para, float* _genom){
    for (int i = 0;i < _genom_para.width; i++){
	for (int j = 0;j < _genom_para.input; j++){
	    printf("%f ",_genom[j+i*_genom_para.input]);
	}
	printf("\n");
    }
    printf("\n");
    for (int k = 0;k < _genom_para.length; k++){
	for (int i = 0;i < _genom_para.width; i++){
	    for (int j = 0;j < _genom_para.width; j++){
		printf("%f ",_genom[_genom_para.width*_genom_para.input+j+i*_genom_para.width+k*_genom_para.width*_genom_para.width]);
	    }
	    printf("\n");
	}
	printf("\n");
    }
    for (int i = 0;i < _genom_para.output; i++){
	for (int j = 0;j < _genom_para.width; j++){
	    printf("%f ",_genom[j+i*_genom_para.width + _genom_para.input * _genom_para.width+ _genom_para.width * _genom_para.width * _genom_para.length]);
	}
	printf("\n");
    }
    printf("\n");
}

/*
 Turns input into a value between 0 and 1
*/

float sigmoid(float _x){
    return (1/(1 + exp(- _x) ));
}

/*
    Calculates the output of the neural network from the input vektor 
    
    inbetween the matrixes the values get reduced by the sigmmoid function

*/


float* brain(genom_parameter _genom_para, float* _genom, float* input){
    float* vector = malloc((_genom_para.input + _genom_para.width + _genom_para.output)* sizeof(float));
    float* transvector = malloc((_genom_para.input + _genom_para.width + _genom_para.output)* sizeof(float));
  
    float carryer = 0;
    for (int i = 0;i < _genom_para.width; i++){
	for (int j = 0;j < _genom_para.input; j++){
	    carryer = carryer + _genom[j+i*_genom_para.input] * input[j];
	}
	vector[i] = sigmoid(carryer);
	carryer = 0;
    }
    
    for (int k = 0;k < _genom_para.length; k++){
	for (int i = 0;i < _genom_para.width; i++){
	    for (int j = 0;j < _genom_para.width; j++){
		carryer = carryer + _genom[_genom_para.width*_genom_para.input+j+i*_genom_para.width+k*_genom_para.width*_genom_para.width] * vector[j];
	    }
	    transvector[i] = sigmoid(carryer);
	    carryer = 0;
	}
	for (int l = 0; l < (_genom_para.input + _genom_para.width + _genom_para.output); l++ ){
	    vector[l] = transvector [l]; 
	}
    }
    
    for (int i = 0;i < _genom_para.output; i++){
	for (int j = 0;j < _genom_para.width; j++){
	    carryer = carryer + _genom[j+i*_genom_para.width + _genom_para.input * _genom_para.width+ _genom_para.width * _genom_para.width * _genom_para.length] * vector[j];
	}
	transvector[i] = sigmoid(carryer);
    }
    free(vector);
    return transvector;
}   

/*
major_c is the chance to have a major mutation at a note | (0;1)
minor_c is the chance to have a minor muataion at a note | (0;1)
minor_f is the factor for a minor change | random()*minor_f
*/

void mutation(genom_parameter _genom_para, float* _genom,float major_c, float minor_c, float minor_f ){
    for (int i = 0; i < _genom_para.arraylength; i++){
	if (rand()%1000000* 0.000001<major_c){
	    _genom[i] = rand()%2000000 * 0.000001 -1;
	}
	else if (rand()%1000000 *0.000001 < minor_c){
	    _genom[i] = _genom[i] + (rand()%2000000 *0.000001 -1)* minor_f;
	    if(_genom[i] > 1){
		_genom[i] = 1;
	    }
	    else if(_genom[i] < -1){
		_genom[i] = -1;
	    }
	}
    }
}

float** stable_init(genom_parameter _genom_para){
    float** stable = malloc(_genom_para.stable_size * sizeof(float*));
    for (int i = 0; i < _genom_para.stable_size;i++){
	stable[i] = genom_init(_genom_para);
	random_soup(_genom_para,stable[i]);
    }
    return stable;
}

/*
    Sorts the fitness function together with the stable
    
    stable and fitness must have their pointerlevel increased by call by adress
*/


void double_bubble_sort(genom_parameter _genom_para, float*** _stable, float** _fitness){ 
    for (int i = 0; i < _genom_para.stable_size;i++){
	for(int j = 0; j < (_genom_para.stable_size - 1);j++){
	    if (*(*_fitness + j) < *(*_fitness +j+1)){
		float saveslot = *(*_fitness + j+1);
		*(*_fitness +j+1) = *(*_fitness + j);
		*(*_fitness + j) = saveslot;
		
		float* savegenom = *(*_stable + j+1);
		*(*_stable + j + 1) = *(*_stable + j);
		*(*_stable + j) = savegenom;
	    }
        }
    }    
}


void genom_copy(genom_parameter _genom_para, float* _genom, float* _genom_destination){
    for (int i = 0; i < _genom_para.arraylength;i++){
	_genom_destination[i] = _genom[i];
    } 
}

/*
    Determens the best genoms form the fitness array;
    surivors determens the ammout of genoms used for the next generation
    the sees genom dos not get mutated
    

*/

void stable_knecht(genom_parameter _genom_para,float** _stable, float* _fitness,float major_c, float minor_c, float minor_f,int surivors ){
    double_bubble_sort(_genom_para, &_stable, &_fitness);
    //printf("Top Genom Fitness: %f\n", _fitness[0]);
    for (int j = 1; j <= _genom_para.stable_size/surivors; j++ ){
	for (int i = 0; i < surivors; i++){
	    if (j*surivors+i >= _genom_para.stable_size){
		j = _genom_para.stable_size;
		i = surivors;
	    }
	    else{
	        genom_copy(_genom_para, _stable[i], _stable[i+j*surivors]);
		mutation(_genom_para, _stable[ i+j*surivors], major_c, minor_c, minor_f);
	    }
	}
    }
}



/*
int main(){
    srand(5);
    genom_parameter gen_para;
    init_genom_parameter(&gen_para,2,2,1,0,3);
    float** stable = stable_init(gen_para);
    print_genom(gen_para,stable[0]);
    print_genom(gen_para,stable[1]);
    print_genom(gen_para,stable[2]);
    float* fitness = malloc(3* sizeof(float));
    fitness[0] = 1;fitness[1] = 2; fitness[2] = 3;
    stable_knecht(gen_para, stable, fitness, 0.4,0,0,1);
    print_genom(gen_para,stable[0]);
    print_genom(gen_para,stable[1]);
    print_genom(gen_para,stable[2]);
}
*/


