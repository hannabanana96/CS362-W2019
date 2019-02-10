#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define FUNCTION "playCard"

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
					 remodel, smithy, village, baron, steward};
	struct gameState G, testG;
	int results = 0, results1 = 0;
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0;

	printf("**********************************************\n");
	printf("             %s Function Test							\n", FUNCTION);
	printf("**********************************************\n");

	
	printf("Test 1: Check it only plays a card in the Action phase\n");
	
	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.phase = 1;											//set phase to Action phase -> 1
	testG.handCount[testG.whoseTurn] = 1;				//only one card in hand
	testG.hand[testG.whoseTurn][handPos] = smithy;	//placing an action card in hand

	results = playCard(handPos, choice1, choice2, choice3, &testG);

	if(results == 0){
		printf("	Incorrect response to non-Action phase\"\n");
		printf("FAILED\n\n");
	}
	if(results != 0){
		printf("	Correct response to non-Action phase\n");
		printf("PASS\n\n");		
	}
	
	/*************************************************************************/
	printf("Test 2: Check that a card can be played only if there are enough actions\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	//game automatically starts in action phase
	testG.numActions = 0;		//set # of Actions to 0, no card can be played

	results = playCard(handPos, choice1, choice2, choice3, &testG);
	
	if(results == 0){
		printf("	Incorrect response to 0 Actions\n");
		printf("FAILED\n\n");
	}
	if(results != 0){
		printf("	Correct response to 0 Actions\n");
		printf("PASS\n\n");		
	}
	/*************************************************************************/

	printf("Test 3: Check that an action/non-action card can be recognized\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	//game automatically starts in action phase
	testG.numActions = 2;										//setting num of actions to two
	testG.handCount[testG.whoseTurn] = 2;					//hand should have two cards
	testG.hand[testG.whoseTurn][handPos] = smithy;		//placing an action card in hand
	testG.hand[testG.whoseTurn][handPos + 1] = copper;	//placing a non-action card in hand

	//attempted action 1
	results = playCard(handPos, choice1, choice2, choice3, &testG);
	results1 = playCard(handPos + 1, choice1, choice2, choice3, &testG);

	if(results != 0){
		printf("	Did not identify the action card\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Identified the action card\n");
		printf("PASS\n\n");		
	}
	
	if(results1 == 0){
		printf("	Did not identify the non-action card\n");
		printf("FAILED\n\n");
	}
	if(results1 != 0){
		printf("	Identified the non-action card\n");
		printf("PASS\n\n");		
	}

	/*************************************************************************/
	printf("Test 4: Check that the number of actions decreases.\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.handCount[testG.whoseTurn] = 1;					//hand should have one card
	testG.hand[testG.whoseTurn][handPos] = smithy;		//placing an action card in hand

	//copying state of game for later comparison
	memcpy(&G, &testG, sizeof(struct gameState));
	
	playCard(handPos, choice1, choice2, choice3, &testG);

	//minus one action from previous
	results = pass_or_fail(testG.numActions, G.numActions - 1);

	if(results == 1){
		printf("	Number of current actions was incorrect\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Number of current actions was correct\n");
		printf("PASS\n\n");		
	}
	
	/*************************************************************************/
	printf("Test 5: Check that money bonus' get added to the total coins\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));
	memset(&G, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.handCount[testG.whoseTurn] = 1;					//hand should have one card
	testG.hand[testG.whoseTurn][handPos] = steward;		//placing an action card in hand
	choice1 = 2; 										//picking the +2 coin option for the steward

	//copying state of game for later comparison
	memcpy(&G, &testG, sizeof(struct gameState));

	playCard(handPos, choice1, choice2, choice3, &testG);
	results = pass_or_fail(testG.coins, G.coins + 2);

	if(results == 1){
		printf("	Bonus coins were not added correctly\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Bonus coins were added correctly\n");
		printf("PASS\n\n");		
	}


	return 0;
}
