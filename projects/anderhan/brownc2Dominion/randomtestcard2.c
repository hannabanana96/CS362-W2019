#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

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

//checks the village card
int checkVillageCard(struct gameState *post){
	int choice1 = 0, choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
	int result = 0, diff = 0, i = 0;	

	//creates a copy of the random game for later comparison
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	//places village in random hand position
	handpos = rand() % post->handCount[post->whoseTurn];
	post->hand[post->whoseTurn][handpos] = village;
	
	//run card
	cardEffect(village, choice1, choice2, choice3, post, 0, &bonus);

	/***********************************************************************************/
	printf("Test 1: Current player should receive 1 cards\n");
	
	//gets the difference between the expected and actual value of handCount
	diff = abs(post->handCount[post->whoseTurn] - (pre.handCount[pre.whoseTurn]+1));

	//should gain two cards in hand
	printf("	Hand count = %d, expected = %d, diff = %d\n", 
			 post->handCount[post->whoseTurn], pre.handCount[pre.whoseTurn] + 1, diff);
	
	result = pass_or_fail(post->handCount[post->whoseTurn], 
								 pre.handCount[pre.whoseTurn] + 1);

	if(result == 1){
		printf("	Hand count did not match expected\n");
		printf("FAIL\n\n");
	}
	
	if(result == 0){
		printf("	Hand count did match expected\n");
		printf("PASS\n\n");
	}
	
	/***********************************************************************************/
	printf("Test 2: Current player deck should decrease by 1 card\n");
	
	//deck count should decrease by one
	printf("	Deck count = %d, expected = %d\n", post->deckCount[post->whoseTurn],
															  pre.deckCount[pre.whoseTurn]-1);
	result = pass_or_fail(post->deckCount[post->whoseTurn], 
								 pre.deckCount[pre.whoseTurn]-1);
	diff = abs(post->deckCount[post->whoseTurn] - (pre.deckCount[pre.whoseTurn]-1));
	
	if (result == 1){
		printf("	The deck did not decrease by 1, the difference was: %d\n", diff);
		printf("FAIL\n\n");
	}
	
	if (result == 0){
		printf("	The deck did decrease by 1\n");
		printf("PASS\n\n");
	}
	
	/***********************************************************************************/
	printf("Test 3: Current player gets two more actions\n");
	
	//+2 for the additional actions
	printf("	Actions = %d, expected = %d\n", post->numActions, pre.numActions+2);
	result = pass_or_fail(post->numActions, pre.numActions+2);
	if (result == 1){
		printf("	Incorrect number of actions\n");
		printf("FAIL\n\n");
	}

	if (result == 0){
		printf("	Correct number of actions\n");
		printf("PASS\n\n");
	}

	/***********************************************************************************/
	printf("Test 4: No state change should occur of the other players\n");

	//checks who the game thinks is the current player
	printf("	Current player: %d, expected %d\n", post->whoseTurn, pre.whoseTurn);
	result = pass_or_fail(post->whoseTurn, pre.whoseTurn);
	if (result == 1){
		printf("	The current player is incorrect\n");
		printf("FAIL\n\n");
	}

	if (result == 0){
		printf("	The current player is correct \n");
		printf("PASS\n\n");
	}

	int flag = 0;
	//set flag is the other players deck cards are not the same as they were previously
	printf("	Make sure the other player's deck was not touched\n");
	for(i = 0; i < 10; i++){
		result = pass_or_fail(post->deck[post->whoseTurn][i], pre.deck[pre.whoseTurn][i]); 
		if(result == 1){
			flag = 1;
			break;
		}
	}	
	if(flag == 1){
		printf("	The other player's deck was effected.\n");
		printf("FAIL\n\n");
	}
	if(flag == 0){
		printf("	The other player's deck was not effected.\n");
		printf("PASS\n\n");
	}

	/***************************************************************************/
	printf("Test 5: The Victory and Kingdom cards should remain the same\n");
	flag = 0;
	int effected_card = 0;
	for (i = 0; i < 27; i++){
		result = pass_or_fail(post->supplyCount[i], pre.supplyCount[i]);
		if(result == 1){
			flag = 1;
			effected_card = i;
			break;
		}
	}
	
	if(flag == 1){
		printf("	The Victory and Kingdom cards were changed.\n");
		printf("	Effected card: %d", effected_card);
		printf("FAIL\n\n");
	}
	
	if(flag == 0){
		printf("	The Victory and Kingdom cards were not changed.\n");
		printf("PASS\n\n");
	}
	
	printf("******************************************************************************\n");
	return 0;
	
}


int main(){	

	//setup
	int seed;
	int n, i, j, numPlayers; 
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
					 sea_hag, tribute, smithy, council_room};
	struct gameState Game;
	
	printf("*******************************************************\n");
	printf("               Village Random Test                     \n");
	printf("*******************************************************\n");

	//increase n later
	//number of tests
	for (n = 0; n < 3; n++){
		//gets random number of players, making sure its a valid number of players
		numPlayers = (rand() % (MAX_PLAYERS-2)) + 2;

		//creates a valid game state with random number of players
		seed = rand();
		initializeGame(numPlayers, k, seed, &Game);

		//adds randomization 
		Game.whoseTurn = rand() % numPlayers;				//upper bounded by number of players
		Game.deckCount[Game.whoseTurn] = rand() % MAX_DECK;	//bounded by the maximum number of cards in the deck
		Game.handCount[Game.whoseTurn] = (rand() % 5) + 1;	//reasonable random sized hand
		
		//randomly select from all of the cards
		for (i = 0; i < Game.deckCount[Game.whoseTurn]; i++){
			Game.deck[Game.whoseTurn][i] = rand() % 27;

		}
		
		printf("Test scenario:\nNum of players: %d\n", numPlayers);
		printf("Player: %d, deckCount: %d, handCount: %d\n\n",
				Game.whoseTurn, Game.deckCount[Game.whoseTurn], 
				Game.handCount[Game.whoseTurn]);
		
		//check the village card
		checkVillageCard(&Game);
	}
	return 0;
}
