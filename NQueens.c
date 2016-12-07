#include <stdio.h>
#include <math.h>


#define MUT_PROB 0.8

float findMax(float array[], int length, int *index){
	float max = 0.0;
	int i;
	for(i = 0; i < length; i++){
		if(max < array[i]){
			max = array[i];
			*index = i;
		}
	}
	return max;
}

float findMin(float array[], int length, int *index){
	float min = 10000.0;
	int i;
	for(i = 0; i < length; i++){
		if(min > array[i]){
			min = array[i];
			*index = i;
		}
	}
	return min;
}

void generate(float array[], int length){
	printf("{\n");
	int i;
	for(i = 0; i < length; i++){
        printf("{ x : %d   y:   } %f\n", i+1, array[i]);
	}
	printf("}\n");
}


void Queens(){

	float epsilon = 0.0001;
	int popSize = 200, chromLen = 8;
	int positions[popSize][chromLen], matingPool[10][chromLen], conflicts[popSize];
	float fitness[popSize], best[200], worst[200];
	int pickElements[] = {1,2,3,4,5,6,7,8};
	int pickChromos[popSize];
    int i,j;

	for(i = 0; i < popSize; i++){
		pickChromos[i] = i + 1;
	}

	printf("generating the inituialial population \n");

	for(i = 0; i < popSize; i++){
		int range = chromLen;
		for(j = 0; j < chromLen; j++){
			int position = rand() % range;
			positions[i][j] = pickElements[position];

			int temp = pickElements[position];
			pickElements[position] = pickElements[range - 1];
			pickElements[range - 1] = temp;

			range--;
		}
	}

        printf("initial population generation completed");

    int gen;
	for(gen = 0; gen < 200; gen++){


		for(i = 0; i < popSize; i++){
            int currQueen;
			for(currQueen = 0; currQueen < chromLen - 1; currQueen++){
				for(j = currQueen + 1; j < chromLen; j++){
					if((j-currQueen) == abs(positions[i][j] - positions[i][currQueen]))
						conflicts[i]++;
				}
			}
		}

		for(i = 0; i < popSize; i++){
			fitness[i] = 1.0/(conflicts[i] + epsilon);
		}


		int bestIndex = 0, worstIndex = 0;
		best[gen] = findMax(fitness, popSize, &bestIndex);
		worst[gen] = findMin(fitness, popSize, &worstIndex);


		int matingIndex = 0, range = popSize;
		for(i = 0; i < 5; i++){
			int tournament[5];
			float tournamentFitness[5];
			for(j = 0; j < 5; j++){
				int position = rand() % range;
				tournament[j] = pickChromos[position];
				tournamentFitness[j] = fitness[tournament[j]];

				int temp = pickChromos[position];
				pickChromos[position] = pickChromos[range - 1];
				pickChromos[range - 1] = temp;

				range--;
			}
			int bestIndex = 0;
			int secondBestIndex = 0;
			findMax(tournamentFitness, 5, &bestIndex);
			findMax(tournamentFitness, 4, &secondBestIndex);

			for(j = 0; j < chromLen; j++){
				matingPool[matingIndex][j] = positions[bestIndex][j];
				matingPool[matingIndex + 1][j] = positions[secondBestIndex][j];
			}
			matingIndex += 2;
		}


		matingIndex = 0;
		for(i = 0; i < 5; i++){
			int child1[chromLen], child2[chromLen];
			int randomPt1 = rand() % chromLen;
			int randomPt2 = rand() % chromLen;

            int startInd, endInd;

            if(randomPt1 < randomPt2){
                int startInd = randomPt1;
                int endInd = randomPt2;
            }
			else{
                int startInd = randomPt2;
                int endInd = randomPt1;
			}
			for(j = 0; j < chromLen; j++){
				if(j >= startInd && j <= endInd){
					child1[j] = matingPool[matingIndex + 1][j];
					child2[j] = matingPool[matingIndex][j];
				}
				else{
					child1[j] = matingPool[matingIndex][j];
					child2[j] = matingPool[matingIndex + 1][j];
				}
			}

			for(j = 0; j < chromLen; j++){
				matingPool[matingIndex][j] = child1[j];
				matingPool[matingIndex + 1][j] = child2[j];
			}
			matingIndex += 2;
		}


		for(i = 0; i < 10; i++){
			float generatedProb = (rand() % 10)/10;
			if(generatedProb < MUT_PROB){
				int swapPos1 = rand() % chromLen;
				int swapPos2 = rand() % chromLen;

				int temp = matingPool[i][swapPos1];
				matingPool[i][swapPos1] = matingPool[i][swapPos2];
				matingPool[i][swapPos2] = temp;
			}
		}


		range = popSize;
		for(i = 0; i < 10; i++){
			int worstPos = 0;
			findMin(fitness, range, &worstPos);

			float temp = fitness[worstPos];
			fitness[worstPos] = fitness[range - 1];
			fitness[range - 1] = temp;

			for(j = 0; j < chromLen; j++){
				positions[worstPos][j] = matingPool[i][j];
			}

			range--;
		}
	}

	printf("best values of the generation\n");
	generate(best, 200);
	printf("worst values of the generation\n");
	generate(worst, 200);
}

int main(){

	printf("program to solve N- QUEENS PROBLEM");
	Queens();
	return (0);
}
