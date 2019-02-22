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


//checks the cutpurse card
int checkCutpurseCard(struct gameState *post, int numPlayers){
	int choice1 = 0, choice2 = 0, choice3 = 0, handpos = 0, bonus = 0;
	int result = 0, diff = 0, i = 0, j = 0;	

	//creates a copy of the random game for later comparison
	struct gameState pre;			
	memcpy(&pre, post, sizeof(struct gameState));

	//places cutpurse in random hand position
	handpos = rand() % post->handCount[post->whoseTurn];
	post->hand[post->whoseTurn][handpos] = cutpurse;

	//run card
	cardEffect(cutpurse, choice1, choice2, choice3, post, handpos, &bonus);

	//sets up treasure count holders
	int pre_num_copper[MAX_PLAYERS];
	int pre_num_silver[MAX_PLAYERS];
	int pre_num_gold[MAX_PLAYERS];
	int post_num_copper[MAX_PLAYERS];
	int post_num_silver[MAX_PLAYERS];
	int post_num_gold[MAX_PLAYERS];
	int pre_total[MAX_PLAYERS];
	int post_total[MAX_PLAYERS];
	
	//sets holders to 0
	for (i = 0; i < MAX_PLAYERS; i++){
		pre_num_copper[i] = 0;
		pre_num_silver[i] = 0;
		pre_num_gold[i] = 0;
		pre_total[i] = 0;
	}
	
	//sets holders to 0
	for (i = 0; i < MAX_PLAYERS; i++){
		post_num_copper[i] = 0;
		post_num_silver[i] = 0;
		post_num_gold[i] = 0;
		post_total[i] = 0;
	}
	
	//gets number of treasures in each player's hand before Cutpurse was run
	for (i = 0; i < pre.numPlayers; i++){
		for (j = 0; j < pre.handCount[i]; j++){
			//counts number of coppers in each player's hand
			if (pre.hand[i][j] == copper){
				pre_num_copper[i]++;
			}
			//counts silvers
			else if (pre.hand[i][j] == silver){
				pre_num_silver[i]++;
			}
			//counts golds
			else if (pre.hand[i][j] == gold){
				pre_num_gold[i]++;
			}
		}
		pre_total[i] = pre_num_copper[i]*1 + pre_num_silver[i]*2 + pre_num_gold[i]*3;	
	}
	
	//gets number of treasures in each player's hand after Cutpurse was run
	for (i = 0; i < post->numPlayers; i++){
		for (j = 0; j < post->handCount[i]; j++){
			//counts number of coppers in each player's hand
			if (post->hand[i][j] == copper){
				post_num_copper[i]++;
			}
			//counts silvers
			else if (post->hand[i][j] == silver){
				post_num_silver[i]++;
			}
			//counts golds
			else if (post->hand[i][j] == gold){
				post_num_gold[i]++;
			}
		}
		post_total[i] = post_num_copper[i]*1 + post_num_silver[i]*2 + post_num_gold[i]*3;
	}
		
	/***********************************************************************************/
	printf("Test 1: Current player, player %d, should receive 2 additional coins\n",
			post->whoseTurn);

	//+2 for the bonus from Cutpurse
	printf("	Coin count = %d, expected = %d\n", post->coins, pre_total[pre.whoseTurn]+2);
	result = pass_or_fail(post->coins, pre_total[pre.whoseTurn]+2);

	if (result == 1){
		printf("	Incorrect number of coins\n");
		printf("FAIL\n\n");
	}

	if (result == 0){
		printf("	Correct number of coins\n");
		printf("PASS\n\n");
	}

	/***********************************************************************************/
	printf("Test 2: Each other player should discard a copper if they have one\n");
	
	int flag = 0;
	//cycle through all players
	for (i = 0; i < numPlayers; i++){
		//make sure all other players have a copper removed from their hand
		if (i != post->whoseTurn){
			//if the player has a copper
			if (pre_num_copper[i] != 0){
				//There should be one less copper in other player's hand
				result = pass_or_fail(post_num_copper[i], pre_num_copper[i] - 1);
				if (result == 1){
					flag = 1;
				}
			}
		}
	}

	if(flag == 1){
		printf("	The copper from the other player's hand was not removed correctly\n");
		//cycle through all players
		for (i = 0; i < numPlayers; i++){
			//don't care about current player's hand
			if (i != post->whoseTurn){
				//if the player has a copper
				if (pre_num_copper[i] != 0){
					printf("	Coppers before: %d, coppers after: %d\n", pre_num_copper[i],
							post_num_copper[i]);
				}
			}
		}
		printf("FAIL\n\n");
	}

	if(flag == 0){
		printf("	If there was a copper, it was successfully removed from the other player's hand\n");
		//cycle through all players
		for (i = 0; i < numPlayers; i++){
			//don't care about current player's hand
			if (i != post->whoseTurn){
				//if the player has a copper
				if (pre_num_copper[i] != 0){
					printf("	Coppers before: %d, coppers after: %d\n", pre_num_copper[i],
							post_num_copper[i]);
				}
			}
		}
		printf("PASS\n\n");
	}
	
	/***********************************************************************************/
	printf("Test 3: The number of silvers and golds should stay the same for all players\n");
	
	int silver_flag = 0;
	int gold_flag = 0;
	int which_player_silver[MAX_PLAYERS];
	int which_player_gold[MAX_PLAYERS];
	int ctr1 = 0, ctr2 = 0;
	for (i = 0; i < MAX_PLAYERS; i++){
		which_player_silver[i] = 0;
		which_player_gold[i] = 0;
	}
	
	//compare's number of golds and silvers in a player's hand
	//if there is an incorrect amount, gets player
	for (i = 0; i < numPlayers; i++){
		silver_flag = pass_or_fail(pre_num_silver[i], post_num_silver[i]);
		gold_flag = pass_or_fail(pre_num_gold[i], post_num_gold[i]);
		if(silver_flag == 1){
			which_player_silver[ctr1] = i;
			ctr1++;
		}
		if(gold_flag == 1){
			which_player_gold[ctr2] = i;
			ctr2++;
		}
	}
	
	//if incorrect number of silvers in hand
	if (silver_flag == 1){
		//display which players had the incorrect amount
		for (i = 0; i < numPlayers; i++){
			if (which_player_silver[i] != 0){
				printf("Player %d's amount of silvers was changed incorrectly\n",
						which_player_silver[i]);
			}

		}
		printf("FAIL\n\n");
	}
	
	//if incorrect number of golds in hand
	if (gold_flag == 1){
		//display which players had the incorrect amount
		for (i = 0; i < numPlayers; i++){
			if (which_player_gold[i] != 0){
				printf("	Player %d's amount of golds was changed incorrectly\n",
						which_player_gold[i]);
			}
		}
		printf("FAIL\n\n");
	}
	
	//if all players had the correct amount of gold and silvers
	if((gold_flag == 0) && (silver_flag == 0)){
		printf("	The gold and silvers of all players stayed the same\n");
		printf("PASS\n\n");
	}
	
	/***********************************************************************************/
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
	
	int num_copper[MAX_PLAYERS];
	int num_silver[MAX_PLAYERS];
	int num_gold[MAX_PLAYERS];
	
	printf("*******************************************************\n");
	printf("               Cutpurse Random Test                  \n");
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
		
		for (i = 0; i < MAX_PLAYERS; i++){
			num_copper[i] = 0;
			num_silver[i] = 0;
			num_gold[i] = 0;
		}
		
		//fill each player's hand with random cards
		//and keep track of their number of coppers
		for (i = 0; i < numPlayers; i++){
			Game.handCount[i] = (rand() % 5) + 1;			//reasonable random sized hand
			for (j = 0; j < Game.handCount[i]; j++){
				Game.hand[i][j] = rand() % 27;				//random cards
				//counts number of coppers in each player's hand
				if (Game.hand[i][j] == copper){
					num_copper[i]++;
				}
				else if (Game.hand[i][j] == silver){
					num_silver[i]++;
				}
				else if (Game.hand[i][j] == gold){
					num_gold[i]++;
				}
			}
		}
		
			
		printf("Test scenario:\nNum of players: %d\n", numPlayers);
		
		for (i = 0; i < numPlayers; i++){
			printf("Player: %d, handCount: %d, coppers: %d, silvers: %d, golds: %d\n",
					i, Game.handCount[i], num_copper[i], num_silver[i], num_gold[i]);
		}
		printf("\n");
		
		//check the cutpurse card
		checkCutpurseCard(&Game, numPlayers);
	}
	return 0;
}
