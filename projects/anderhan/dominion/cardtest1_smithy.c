#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Smithy"

int pass_or_fail(int a, int b){
	if (a == b){
		printf("	TEST SUCCESSFULLY COMPLETED\n");
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
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int newCards = 0;
	int discarded = 1;
	int thisPlayer = 0;
	int shuffledCards = 0;
	int result = 0;

	//start game
	initializeGame(numPlayers, k, seed, &G);

	printf("****************************************\n");
	printf("            %s Card Test              \n", TESTCARD);
	printf("****************************************\n");

	printf("Test 1: Current player should receive 3 cards\n");

	//copy the state of the game for later comparison
	memcpy(&testG, &G, sizeof(struct gameState));
	
	//get card outcomes
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	//three cards should be added to the hand
	newCards = 3;
	printf("	Hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 
															  G.handCount[thisPlayer] + newCards - discarded);
	result = pass_or_fail(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);	

	//
	if (result == 1){
		printf("	The number cards in hand did not increase by 3\n");
		printf("TEST FAILED\n");
	}

	if (result == 0){
		printf("	The number of cards in hand is correct\n");
		printf("PASS\n");
	}

	//deck count should decrease by three
	printf("\n	Deck count = %d, expected = %d\n", testG.handCount[thisPlayer],
															 G.handCount[thisPlayer] - newCards + shuffledCards);
	result = pass_or_fail(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] + newCards - discarded);
	if (result == 1)
		printf("	The deck did not decrease by 3\n");

	//check if Smithy was discarded
	printf("Checking if the Smithy was discarded\n");


}

