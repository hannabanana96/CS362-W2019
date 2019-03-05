#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define FUNCTION "updateCoins"

//returns 0 if the two numbers are the same
//returns 1 if the two numbers are different
int pass_or_fail(int a, int b){
	if (a == b){
		return 0;
	}

	else{
		return 1;
	}
}


int main(){
	int i, p, handCount;
	int seed = 1000;
	int numPlayers = 2;
	int maxBonus = 10;
	int bonus;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
					 remodel, smithy, village, baron, great_hall};
	struct gameState g_copper, g_silver, g_gold, g_empty;
	int maxHandCount = 5;
	int results = 0;
	int copper_flag = 0, silver_flag = 0, gold_flag = 0, empty_flag = 0;


	int coppers[MAX_HAND];
	int silvers[MAX_HAND];
	int golds[MAX_HAND];
	int empty[MAX_HAND];

	for (i = 0; i < MAX_HAND; i++){
		coppers[i] = copper;
		silvers[i] = silver;
		golds[i] = gold;
		empty[i] = province;		//picked a random non-treasure card
	}



	printf("**********************************************\n");
	printf("             %s Function Test							\n", FUNCTION);
	printf("**********************************************\n");

	//runs tests for all players
	for (p = 0; p < numPlayers; p++){
		//cycle through all possible handcounts
		for (handCount = 1; handCount < (maxHandCount + 1); handCount++){
			//cycle through all possible bonus coin values
			for (bonus = 0; bonus < (maxBonus + 1); bonus++){
			
				//clears game state
				memset(&g_copper, 23, sizeof(struct gameState));
				memset(&g_silver, 23, sizeof(struct gameState));
				memset(&g_gold, 23, sizeof(struct gameState));
				memset(&g_empty, 23, sizeof(struct gameState));
			
				//start game
				initializeGame(numPlayers, k, seed, &g_copper);
				initializeGame(numPlayers, k, seed, &g_silver);
				initializeGame(numPlayers, k, seed, &g_gold);
				initializeGame(numPlayers, k, seed, &g_empty);

				//set up hand size
				g_copper.handCount[p] = handCount;
				g_silver.handCount[p] = handCount;
				g_gold.handCount[p] = handCount;
				g_empty.handCount[p] = handCount;

				//fill hand with respective cards
				memcpy(g_copper.hand[p], coppers, sizeof(int) * handCount);
				memcpy(g_silver.hand[p], silvers, sizeof(int) * handCount);
				memcpy(g_gold.hand[p], golds, sizeof(int) * handCount);
				memcpy(g_empty.hand[p], empty, sizeof(int) * handCount);

				//check copppers
				updateCoins(p, &g_copper, bonus);
				results = pass_or_fail(g_copper.coins, (handCount*1) + bonus);
				if(results == 1){
					copper_flag = 1;
				}

				//check silvers
				updateCoins(p, &g_silver, bonus);
				results = pass_or_fail(g_silver.coins, (handCount*2) + bonus);
				if(results == 1){
					silver_flag = 1;
				}

				//check gold
				updateCoins(p, &g_gold, bonus);
				results = pass_or_fail(g_gold.coins, (handCount*3) + bonus);
				if(results == 1){
					gold_flag = 1;
				}

				//check empty
				updateCoins(p, &g_empty, bonus);
				results = pass_or_fail(g_empty.coins, bonus);
				if(results == 1){
					empty_flag = 1;
				}
			}
		}
	}

	printf("Test 1: Check adding coppers for all players and bonus amounts\n");
	if(copper_flag == 1){
		printf("	Coppers were not added correctly\n");
		printf("FAILED\n\n");
	}
	if(copper_flag == 0){
		printf("	Coppers were added correctly\n");
		printf("PASS\n\n");		
	}

	printf("Test 2: Check adding silvers for all players and bonus amounts\n");
	if(silver_flag == 1){
		printf("	Silvers were not added correctly\n");
		printf("FAILED\n\n");
	}
	if(silver_flag == 0){
		printf("	Silvers were added correctly\n");
		printf("PASS\n\n");		
	}

	printf("Test 3: Check adding golds for all players and bonus amounts\n");
	if(gold_flag == 1){
		printf("	Golds were not added correctly\n");
		printf("FAILED\n\n");
	}
	if(gold_flag == 0){
		printf("	Golds were added correctly\n");
		printf("PASS\n\n");		
	}

	printf("Test 4: Check that a non-treasure card was not added for all players and bonus amounts\n");
	if(empty_flag == 1){
		printf("	Empties were not added correctly\n");
		printf("FAILED\n\n");
	}
	if(empty_flag == 0){
		printf("	Empties were added correctly\n");
		printf("PASS\n\n");		
	}

	return 0;
}
