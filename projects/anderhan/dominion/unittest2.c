#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define FUNCTION "buyCard"

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
	int seed = 1000;
	int numPlayers = 2;
	int k[10] = {adventurer, council_room, feast, gardens, mine, 
					 remodel, smithy, village, baron, great_hall};
	struct gameState G, testG;
	int results = 0;
	int supplyPos = 0;	


	printf("**********************************************\n");
	printf("             %s Function Test							\n", FUNCTION);
	printf("**********************************************\n");

	
	printf("Test 1: Checking that the phase stays in the buying phase\n");
	
	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.coins = 3;			//coins being set a known value
	supplyPos = silver;		//going to buy a known card that can be bought

	buyCard(supplyPos, &testG);
	results = pass_or_fail(testG.phase, 1);	//1 = buy phase (found in interface.c code)
	if(results == 1){
		printf("	Phase is not \"buying\"\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Phase is \"buying\"\n");
		printf("PASS\n\n");		
	}
	
	/*************************************************************************/
	printf("Test 2: Check that nothing can be bought if there aren't enough buys\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.coins = 3;			//coins being set a known value
	testG.numBuys = 0;		//set # of buys to 0, nothing should be bought
	supplyPos = silver;		//going to buy a known card that can be bought

	results = buyCard(supplyPos, &testG);
	if(results == 0){
		printf("	Incorrect response to 0 buys\n");
		printf("FAILED\n\n");
	}
	if(results != 0){
		printf("	Correct response to 0 buys\n");
		printf("PASS\n\n");		
	}

	/*******************************************************************************/
	printf("Test 3: Check that nothing can be bought if the supply count for that card is < 1\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.coins = 3;						//coins being set a known value
	supplyPos = silver;					//going to buy a known card that can be bought
	testG.supplyCount[silver] = 0;	//setting supply count for known card to 0
												//should not be able to buy it

	results = buyCard(supplyPos, &testG);
	if(results == 0){
		printf("	Incorrect response to 0 supply count\n");
		printf("FAILED\n\n");
	}
	if(results != 0){
		printf("	Correct response to 0 supply count\n");
		printf("PASS\n\n");		
	}
	
	/*******************************************************************************/
	printf("Test 4: Check that nothing can be bought if the player doesn't have enought coins\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.coins = 2;		//not enough coins to by a silver, should not be able to buy it
	supplyPos = silver;	//going to buy a known card that can be bought

	results = buyCard(supplyPos, &testG);
	if(results == 0){
		printf("	Incorrect response to insufficient money\n");
		printf("FAILED\n\n");
	}
	if(results != 0){
		printf("	Correct response to insufficient money\n");
		printf("PASS\n\n");		
	}
	
	/*******************************************************************************/
	printf("Test 5: Check that the bought card goes into the discard pile\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.coins = 3;						//enough coins for a silver
	supplyPos = silver;					//going to buy a known card that can be bought

	//copying state of game for later comparison
	memcpy(&G, &testG, sizeof(struct gameState));

	buyCard(supplyPos, &testG);

	//look through new discard deck searching for card	
	int i, test_card_flag = 0, card_flag = 0;
	for (i = 0; i < testG.discardCount[testG.whoseTurn]; i++){
		if (testG.discard[testG.whoseTurn][i] == silver){
			test_card_flag++;
		}
	}

	//look through original discard deck searching for card
	for (i = 0; i < G.discardCount[G.whoseTurn]; i++){
		if (G.discard[G.whoseTurn][i] == silver){
			card_flag++;
		}
	}
	
	results = pass_or_fail(test_card_flag, card_flag + 1);

	if(results == 1){
		printf("	Bought card did not get added to the discard pile\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Bought card successfully went to the discard pile\n");
		printf("PASS\n\n");		
	}
	
	/*******************************************************************************/
	printf("Test 6: Check that the supply count decreased by 1\n");

	//using same game state as previous test

	results = pass_or_fail(testG.supplyCount[silver], G.supplyCount[silver] - 1);

	if(results == 1){
		printf("	The supply count did not decrease correctly\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	The supply count decreased correctly\n");
		printf("PASS\n\n");		
	}
	
	/*******************************************************************************/
	printf("Test 7:Check that the correct amount of coins decreased\n");

	//using same game state as previous test

	results = pass_or_fail(testG.coins, G.coins - getCost(supplyPos));

	if(results == 1){
		printf("	Incorrect number of coins were removed\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Correct number of coins were removed\n");
		printf("PASS\n\n");		
	}
	return 0;
}
