#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Adventurer"



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
	int estates[MAX_HAND];	//non treasure card
	int i;
	for (i = 0; i < MAX_HAND; i++){
		estates[i] = estate;
	}	


	//start game
	initializeGame(numPlayers, k, seed, &G);

	printf("****************************************\n");
	printf("            %s Card Test              \n", TESTCARD);
	printf("****************************************\n");


	/************************** TEST 1 ***************************/
	printf("Test 1: Current player should receive 2 cards\n");

	//setting the hand count to 5
	int handCount = 5;
	for (i = 0; i < G.numPlayers; i++){
		G.handCount[i] = handCount;
		memcpy(G.hand[i], estates, sizeof(int)*G.handCount[i]);
	}
		
	//copy the state of the game for later comparison
	memcpy(&testG, &G, sizeof(struct gameState));
	
	//get card outcomes
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	//should gain two cards
	printf("	Hand count = %d, expected = %d\n", testG.handCount[testG.whoseTurn], 
															  G.handCount[G.whoseTurn]+2);
	result = pass_or_fail(testG.handCount[testG.whoseTurn], G.handCount[G.whoseTurn] + 2);

	if (result == 1){
		printf("	Hand count did not increase correctly\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	Hand count increased correctly\n");
		printf("PASS\n\n");
	}

	/************************** TEST 2 ***************************/
	printf("Test 2: The two new cards should be treasure cards\n");
	
	int money_flag1 = 0;
	int money_flag2 = 0;

	//checking that the new cards are copper, silver, or gold
	if ((testG.hand[testG.whoseTurn][handCount] == copper) ||
		 (testG.hand[testG.whoseTurn][handCount] == silver) ||
		 (testG.hand[testG.whoseTurn][handCount] == gold)){
		money_flag1 = 1;
	}

	if ((testG.hand[testG.whoseTurn][handCount+1] == copper) ||
		 (testG.hand[testG.whoseTurn][handCount+1] == silver) ||
		 (testG.hand[testG.whoseTurn][handCount+1] == gold)){
		money_flag2 = 1;
	}

	result = pass_or_fail(money_flag1, money_flag2);

	if (result == 1){
		printf("	The two added cards were not treasures or a previous test failed\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	The added cards were treasures\n");
		printf("PASS\n\n");
	}

	/*************************** TEST 3 *******************************/
	printf("Test 3: Check that the drawn cards have been discarded\n");

	int diff;

	//the amount that the deck has decrease before and after the card was played
	diff = G.deckCount[G.whoseTurn] - testG.deckCount[testG.whoseTurn];

	//+2 for the two cards that will go into the hand
	//the new discard amount should be two more than dff
	result = pass_or_fail(testG.discardCount[testG.whoseTurn], diff+2);

	if (result == 1){
		printf("	Incorrect number of cards was discarded or a previous test failed\n");
		printf("FAILED\n\n");
	}

	if (result == 0){
		printf("	Correct number of cards was discarded \n");
		printf("PASS\n\n");
	}



	/*************************** TEST 4 *******************************/
	printf("Test 4: No state change should occur of the other players\n");

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

	int flag = 0;
	//set flag is the other players deck cards are not the same as they were previously
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
		printf("	The other player's deck was not effected.\n");
		printf("PASS\n\n");
	}

	flag = 0;
	//set flag is the other players hand cards are not the same as they were previously
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

	/***************************************************************************/
	printf("Test 5: The Victory and Kingdom cards should remain the same\n");
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

