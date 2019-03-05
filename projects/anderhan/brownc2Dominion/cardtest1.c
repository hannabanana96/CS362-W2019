#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Smithy"



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


	/************************** TEST 1 ***************************/
	printf("Test 1: Current player should receive 3 cards\n");

	//copy the state of the game for later comparison
	memcpy(&testG, &G, sizeof(struct gameState));
	
	//get card outcomes
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	//three cards should be added to the hand
	newCards = 3;
	printf("	Hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 
												G.handCount[thisPlayer] + newCards - discarded);
	result = pass_or_fail(testG.handCount[thisPlayer], 
								 G.handCount[thisPlayer] + newCards - discarded);	

	if (result == 1){
		printf("	The number cards in hand did not increase by 3\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	The number of cards in hand is correct\n");
		printf("PASS\n\n");
	}

	/**************************** TEST 2 **************************/
	printf("Test 2: Current deck should decrease by 3 cards\n");

	//deck count should decrease by three
	printf("	Deck count = %d, expected = %d\n", testG.deckCount[thisPlayer],
												G.deckCount[thisPlayer] - newCards + shuffledCards);
	result = pass_or_fail(testG.deckCount[thisPlayer], 
								 G.deckCount[thisPlayer] - newCards + shuffledCards);

	if (result == 1){
		printf("	The deck did not decrease by 3\n");
		printf("FAILED\n\n");
	}
	
	if (result == 0){
		printf("	The deck did decrease by 3\n");
		printf("PASS\n\n");
	}

	/*************************** TEST 3 *******************************/
	printf("Test 3: No state change should occur of the other players\n");

	//checks who the game thinks is the current player
	printf("	Current player: %d, expected %d\n", testG.whoseTurn, G.whoseTurn);
	result = pass_or_fail(testG.whoseTurn, G.whoseTurn);
	if (result == 1){
		printf("	The current player is incorrect\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	The current player is correct \n");
		printf("PASS\n\n");
	}

	int i = 0;
	int flag = 0;
	//set flag if the other players deck cards are not the same as they were previously
	printf("	Make sure the other player's deck was not touched\n");
	for(i = 0; i < 10; i++){
		result = pass_or_fail(testG.deck[1][i], G.deck[1][i]); 
		if(result == 1){
			flag = 1;
			break;
		}
	}	
	if(flag == 1){
		printf("	The other player's deck was effected.\n");
		printf("FAILED\n\n");
	}
	if(flag == 0){
		printf(" The other player's deck was not effected.\n");
		printf("PASS\n\n");
	}

	flag = 0;
	//set flag if the other players hand cards are not the same as they were previously
	printf("	Make sure the other player's hand was not touched\n");
	for(i = 0; i < 10; i++){
		result = pass_or_fail(testG.hand[1][i], G.hand[1][i]); 
		if(result == 1){
			flag = 1;
			break;
		}
	}	
	
	if(flag == 1){
		printf("	The other player's hand was effected.\n");
		printf("FAILED\n\n");
	}
	if(flag == 0){
		printf("	The other player's hand was not effected.\n");
		printf("PASS\n\n");
	}

	/*************************** TEST 4 *******************************/
	printf("Test 4: The Victory and Kingdom cards should remain the same\n");
	flag = 0;
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

