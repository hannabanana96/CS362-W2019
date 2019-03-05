#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Cutpurse"



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


int main() {
	int seed = 500;
	int numPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, 
					 cutpurse, sea_hag, tribute, smithy, council_room};
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int result = 0;
	int i;
	int coppers[MAX_HAND];
	for (i = 0; i < MAX_HAND; i++){
		coppers[i] = copper;
	}	


	//start game
	initializeGame(numPlayers, k, seed, &G);

	printf("****************************************\n");
	printf("            %s Card Test              \n", TESTCARD);
	printf("****************************************\n");


	/************************** TEST 1 ***************************/
	printf("Test 1: Current player should receive 2 additional coins\n");


	//setting the hand count to 5
	for (i = 0; i < G.numPlayers; i++){
		G.handCount[i] = 5;
		memcpy(G.hand[i], coppers, sizeof(int)*G.handCount[i]);
	}
	
	
	//copy the state of the game for later comparison
	memcpy(&testG, &G, sizeof(struct gameState));
	
	//get card outcomes
	cardEffect(cutpurse, choice1, choice2, choice3, &testG, handpos, &bonus);

	int new_coins = 2;	
	printf("	Coin count = %d, expected = %d\n", testG.coins, G.coins + new_coins);
	result = pass_or_fail(testG.coins, G.coins + new_coins);	

	if (result == 1){
		printf("	Incorrect number of coins\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	Correct number of coins\n");
		printf("PASS\n\n");
	}

	/***********************************************************************/
	printf("Test 2: Each player should discard a copper if they have one\n");
	int j;
	int G_copper_ctr = 0;
	int testG_copper_ctr = 0;
	int results1;
	
	int flag = 0;
	//cycle through all players
	for (i = 0; i < G.numPlayers; i++){
		//make sure all other player have a copper removed from their hand
		if (i != G.whoseTurn){
			for (j = 0; j < G.handCount[i]; j++){
				//# of coppers originally in hand
				if (G.hand[i][j] == copper){
					G_copper_ctr++;
				}
				//# of coppers now in in hand
				if (testG.hand[i][j] == copper){
					testG_copper_ctr++;
				}
			}
			//There should be one less copper in other player's hand
			results1 = pass_or_fail(testG_copper_ctr, G_copper_ctr - 1);
			if (results1 == 1){
				flag = 1;
			}
		}
	}

	if(flag == 1){
		printf("	The copper from the other player's hand was not removed correctly\n");
		printf("FAILED\n\n");
	}

	if(flag == 0){
		printf("	The copper was successfully removed from the other player's hand\n");
		printf("PASS\n\n");
	}
	
	/*****************************************************************************/
	printf("Test 5: The Victory and Kingdom cards should remain the same\n");
	flag = 0;
	
	//checking all the supply counts of the Victory and Kingdom cards
	for (i = 0; i < 27; i++){
		result = pass_or_fail(testG.supplyCount[i], G.supplyCount[i]);
		if(result == 1){
			flag = 1;
			break;
		}
	}
	if(flag == 1){
		printf("	The Victory and Kingdom cards were changed.\n");
		printf("FAILED\n\n");
	}
	if(flag == 0){
		printf("	The Victory and Kingdom cards were not changed.\n");
		printf("PASS\n\n");
	}

	return 0;
}

