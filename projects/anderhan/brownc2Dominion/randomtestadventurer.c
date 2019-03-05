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


//checks the adventure card
int checkAdventureCard(struct gameState *post){
	int choice1 = 0, choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
	int result = 0, diff = 0, i = 0;	
	int deck_flag = 0;

	//creates a copy of the random game for later comparison
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	//places adventurer in random hand position
	handpos = rand() % post->handCount[post->whoseTurn];
	post->hand[post->whoseTurn][handpos] = adventurer;

	//gets hand size
	int handsize = 0;
	handsize = post->handCount[post->whoseTurn];
	
	//run card
	cardEffect(adventurer, choice1, choice2, choice3, post, 0, &bonus);
	
	/***********************************************************************************/
	printf("Test 1: Current player should receive 2 cards\n");
	
	//gets the difference between the expected and actual value of handCount
	diff = abs(post->handCount[post->whoseTurn] - (pre.handCount[pre.whoseTurn]+2));

	//should gain two cards in hand
	printf("	Hand count = %d, expected = %d, diff = %d\n", 
			 post->handCount[post->whoseTurn], pre.handCount[pre.whoseTurn] + 2, diff);
	
	result = pass_or_fail(post->handCount[post->whoseTurn], 
								 pre.handCount[pre.whoseTurn] + 2);

	if(result == 1){
		printf("	Hand count did not match expected\n");
		printf("FAIL\n\n");
	}
	
	if(result == 0){
		printf("	Hand count did match expected\n");
		printf("PASS\n\n");
	}

	
	/***********************************************************************************/
	printf("Test 2: The two new cards should be treasure cards\n");
	
	int money_flag1 = 0, money_flag2 = 0;
	char card1[8];
	char card2[8];
	int no_card_flag = 0, treasure_flag = 0, bad_card1 = 0, bad_card2 = 0;


	//checking if first new card is copper, silver, or gold
	if ((post->hand[post->whoseTurn][handsize] == copper) ||
		 (post->hand[post->whoseTurn][handsize] == silver) ||
		 (post->hand[post->whoseTurn][handsize] == gold)){
		money_flag1 = 1;

		//get which card it was
		if(post->hand[post->whoseTurn][handsize] == copper)
			strcpy(card1, "copper");
		else if(post->hand[post->whoseTurn][handsize] == silver)
			strcpy(card1, "silver");
		else if(post->hand[post->whoseTurn][handsize] == gold)
			strcpy(card1, "gold");
	}
	
	//checking if second new card is copper, silver, or gold
	if ((post->hand[post->whoseTurn][handsize+1] == copper) ||
		 (post->hand[post->whoseTurn][handsize+1] == silver) ||
		 (post->hand[post->whoseTurn][handsize+1] == gold)){
		money_flag2 = 1;
		
		//get which card it was
		if(post->hand[post->whoseTurn][handsize+1] == copper)
			strcpy(card2, "copper");
		else if(post->hand[post->whoseTurn][handsize+1] == silver)
			strcpy(card2, "silver");
		else if(post->hand[post->whoseTurn][handsize+1] == gold)
			strcpy(card2, "gold");
	}

	//set flag if both new cards are treasures
	if((money_flag1 == 1) && (money_flag2 == 1)){
		treasure_flag = 1;
	}
	
	//get the bad cards
	else{
		bad_card1 = post->hand[post->whoseTurn][handsize];
		bad_card2 = post->hand[post->whoseTurn][handsize+1];
	}
	

	if (treasure_flag == 0){
		printf("	Cards(numerical version): %d, %d\n", bad_card1, bad_card2);  
		printf("	The one or both of the added cards were not treasures\n");
		printf("FAIL\n\n");
	}

	if (treasure_flag == 1){
		printf("	Cards: %s, %s\n", card1, card2);
		printf("	The added cards were treasures\n");
		printf("PASS\n\n");
	}
	
	
	/***********************************************************************************/
	printf("Test 3: Check that the drawn cards were moved from deck to discard\n");

	//the amount that the deck has decrease before and after the card was played
	diff = pre.deckCount[pre.whoseTurn] - post->deckCount[post->whoseTurn];

	//-2 for the two cards that will go into the hand
	//the new discard amount should be two more than dff
	result = pass_or_fail(post->discardCount[post->whoseTurn], diff-2);

	printf("	deckCounts: %d, %d\n", pre.deckCount[pre.whoseTurn], post->deckCount[post->whoseTurn]);
	printf("	# of discarded cards: %d, expected: %d\n", post->discardCount[post->whoseTurn], diff-2);
	
	if (result == 1){
		printf("	Incorrect number of cards was discarded or a previous test FAIL\n");
		printf("FAIL\n\n");
	}

	if (result == 0){
		printf("	Correct number of cards was discarded \n");
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
	printf("Test 4: The Victory and Kingdom cards should remain the same\n");
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

	printf("****************************************************************\n");
	return 0;
}


int main(){	

	//setup
	int seed;
	int n, i, numPlayers; 
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
					 sea_hag, tribute, smithy, council_room};
	struct gameState Game;

	printf("*******************************************************\n");
	printf("               Adventurer Random Test                  \n");
	printf("*******************************************************\n");

	//increase n later
	//number of tests
	for (n = 0; n < 100; n++){
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
		printf("Player: %d, deckCount: %d, handCount: %d\n",
				 Game.whoseTurn, Game.deckCount[Game.whoseTurn], 
				 Game.handCount[Game.whoseTurn]);

		//check the adventure card
		checkAdventureCard(&Game);
	}
	return 0;
}
