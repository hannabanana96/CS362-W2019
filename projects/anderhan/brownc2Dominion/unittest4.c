#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define FUNCTION "discardCard"

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
	int handPos = 0;
	int trashFlag = 0, test_card_flag = 0, card_flag = 0;
	int i;
	int maxHandCount = 5;

	int smithys[MAX_HAND];
	
	for (i = 0; i < MAX_HAND; i++){
		smithys[i] = smithy;
	}


	printf("**********************************************\n");
	printf("             %s Function Test							\n", FUNCTION);
	printf("**********************************************\n");

	printf("Test 1: If the trashFlag is set, the card should not go into the played pile\n");
	
	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	trashFlag = 1;						//card is being trashed, should not go in played pile
	testG.handCount[testG.whoseTurn] = 1;				//only one card in hand
	testG.hand[testG.whoseTurn][handPos] = smithy;	//placing card in hand

	//copy state of game for later comparison
	memcpy(&G, &testG, sizeof(struct gameState));

	discardCard(handPos, testG.whoseTurn, &testG, trashFlag);

	results = pass_or_fail(testG.playedCardCount, G.playedCardCount);

	if(results == 1){
		printf("	Card was incorrectly put in the played pile\"\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Card was not put in the played pile\n");
		printf("PASS\n\n");		
	}
	
	/*************************************************************************/
	printf("Test 2: If the trashflag is not set, the card should go into the played pile\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));
	memset(&G, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	trashFlag = 0;						//card is not being trashed, should go in played pile
	testG.handCount[testG.whoseTurn] = 1;					//only one card in hand
	testG.hand[testG.whoseTurn][handPos] = smithy;		//placing card in hand

	//copy state of game for later comparison
	memcpy(&G, &testG, sizeof(struct gameState));

	discardCard(handPos, testG.whoseTurn, &testG, trashFlag);

	results = pass_or_fail(testG.playedCardCount, G.playedCardCount + 1);

	if(results == 1){
		printf("	The played card count did not increase\"\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	The played card count increased\n");
		printf("PASS\n\n");		
	}

	//checking to see if the card as "played" on test
	test_card_flag = 0;
	for (i = 0; i < testG.playedCardCount; i++){
		if(testG.playedCards[i] == smithy){
			test_card_flag++;
		}
	}

	//checking to see if the card was "played" on comparison state
	card_flag = 0;
	for (i = 0; i < G.playedCardCount; i++){
		if(G.playedCards[i] == smithy){
			card_flag++;
		}
	}

	results = pass_or_fail(test_card_flag, card_flag + 1);

	if(results == 1){
		printf("	Card was not put in the played pile\"\n");
		printf("FAILED\n\n");
	}
	if(results == 0){
		printf("	Card was put in the played pile\n");
		printf("PASS\n\n");		
	}

	/*************************************************************************/
	printf("Test 3: Check if the card was removed from the hand\n");
	
	//clearing game state
	memset(&testG, 23, sizeof(struct gameState));
	memset(&G, 23, sizeof(struct gameState));

	//start game
	initializeGame(numPlayers, k, seed, &testG);

	//test set up
	testG.handCount[testG.whoseTurn] = maxHandCount;
	memcpy(testG.hand[testG.whoseTurn], smithys, sizeof(int) * maxHandCount);
	trashFlag = 0;
	memcpy(&G, &testG, sizeof(struct gameState));

	int handCount_flag = 0, test_smithy_ctr = 0, smithy_ctr = 0;
	card_flag = 0;
	int j;
	
	//checking the removal of a card from the entire hand. 
	//Removing just from handpos 0 tests all the states of removal
	for (i = 0; i < maxHandCount; i++){
		test_smithy_ctr = 0;
		smithy_ctr = 0;
		discardCard(0, testG.whoseTurn, &testG, trashFlag);

		//checks if the hand count decreased
		results = pass_or_fail(testG.handCount[testG.whoseTurn], 
					 				  G.handCount[G.whoseTurn] - (i+1));
		if(results == 1){
			handCount_flag = 1;
		}

		//checks that the card is no longer in the hand
		for (j = 0; j < maxHandCount; j++){
			if (testG.hand[testG.whoseTurn][j] == smithy){
				test_smithy_ctr++;
			}
			if (G.hand[G.whoseTurn][j] == smithy){
				smithy_ctr++;
			}
		}
		results1 = pass_or_fail(test_smithy_ctr, smithy_ctr - (i+1));
		if (results1 == 1){
			card_flag = 1;
		}
	}

	if(handCount_flag == 1){
		printf("	Hand count did not decrease\n");
		printf("FAILED\n\n");
	}
	if(handCount_flag == 0){
		printf("	Hand count decreased successfully\n");
		printf("PASS\n\n");		
	}
	
	if(card_flag == 1){
		printf("	Card was not still in hand\n");
		printf("FAILED\n\n");
	}
	if(card_flag == 0){
		printf("	Card was not in hand\n");
		printf("PASS\n\n");		
	}

	return 0;
}
