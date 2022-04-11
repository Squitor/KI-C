#include "KI.h"
#include "stdlib.h"
#include "stdio.h"

int* init_game(){
    int* gamefield = malloc(9*sizeof(int));
    for (int i = 0; i < 9; i++){
	gamefield[i] = 0;
    }
    return gamefield;
}

int check_winner(int* gamefield){
    for (int i = 0; i < 3;i++){
	if (gamefield[i*3] == gamefield[i*3+1] && gamefield[i*3] == gamefield[i*3+2]){
	    return gamefield[i*3];
	}
	if (gamefield[i] == gamefield[i+3] && gamefield[i]== gamefield[i+6]){
	    return gamefield[i];
	}
    }
    if (gamefield[0] == gamefield[4] && gamefield[0] == gamefield[8]){
        return gamefield[0];
    }
    if (gamefield[2] == gamefield[4] && gamefield[2] == gamefield[6]){
        return gamefield[2];
    } 
    return 0;
}


void print_map(int* gamefield){
    for (int i = 0; i < 3;i++ ){
	printf("%i %i %i\n",gamefield[i*3],gamefield[i*3+1],gamefield[i*3+2]);
    }
}

int validate_play(int play,int* gamefield){
    if (gamefield[play] == 0){
	return 1;
    }
    else{
	return 0;
    }
}

int* sort_play(float* play){
    int* sort = malloc(sizeof(int)*9);
    for (int i = 0; i < 9;i++){
	sort[i] = i; 
    }
    for (int i = 0; i < 9;i++){
	for (int j = 0;j < 8;j++){
	    if (play[j] < play[j+1]){
		float savef = play[j+1];
		play[j+1] = play[j];
		play[j] = savef;
		
		int save;
		save = sort[j+1];
		sort[j+1] = sort[j];
		sort[j] = save;
	    }
	}
    }
    return sort;
}

void find_play(int* gamefield, float* play, int player){
    int* sort = sort_play(play);
    for (int i = 0; i < 9; i++){
	if (validate_play(sort[i], gamefield)){
	    gamefield[sort[i]] = player;
	    free(sort);
	    i = 9;
	}
    }
} 

float* random_play(){
    float* play = malloc(sizeof(float)*9);
    for (int i = 0; i < 9; i++){
	play[i] = random();
    }
    return play;
}

float playing(genom_parameter gen_para,float* genom){
    int* gamefield = init_game();
    float* play;
    int outcome;
    int i = 0;
    while (1){
	play = random_play();
	find_play(gamefield, play,1);
	free(play);
	outcome = check_winner(gamefield);
	if (outcome != 0){
	    free(gamefield);
	    return 0.0;
	}
	i++;
	if(i == 9){
	    free(gamefield);
	    return 0.0;
	}
	play = brain(gen_para, genom, (float*) gamefield);
	find_play(gamefield, play, 2);
	free(play);
	
	outcome = check_winner(gamefield);
	if (outcome != 0){
	    free(gamefield);
	    return 1.0;
	}
	i++;
    }
}

int main(){
    int size = 1000;
    genom_parameter gen_para;
    init_genom_parameter(&gen_para, 9, 9, 9, 2, size);
    float** stable = stable_init(gen_para);
    float fitness[size];
    float average;
    for (int l = 0; l < 10000; l++){
	average = 0;
	for (int i = 0; i<size;i++){
    	    fitness[i] = 0;
    	    for (int j = 0;j<100;j++){
    	        fitness[i] = fitness[i] + playing(gen_para, stable[i]);
    	    }
	    average = fitness[i] + average;
    	}
    	stable_knecht(gen_para, stable, fitness, 0.005, 0.01, 0.1, size/2);
	printf("Generation: %i Average Fitness: %f\n", l, average/size);
    }
    free(stable);
}
