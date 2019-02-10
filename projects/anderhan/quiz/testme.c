#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//Grabs a random character from a preset input pool
char inputChar(){
	int n = 0;

	//input pool
	char c_inputs[9] = {'[', '(', '{', ' ', 'a', 'x', '}', ')', ']'};

	//gets a random number from 0-8
	n = rand() % 9;

	//returns the character corresponding to the random number
	return c_inputs[n];
}


//Returns either "reset" or "nope!"
char* inputString(){
	int n = 0;

	//limiting options between "nope!" and "reset"
	char option1[6] = "nope!";
	char option2[6] = "reset";

	//gets a random number from 0-1
	n = rand() % 2;

	if (n == 0)
		return option1;

	else
		return option2;
}


int testme(int num)
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0, prev_state = 0;
  while (1){
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);
	
	 prev_state = state;
	
    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    
	 //if "1" is on the command line, change the state to something unexpected
	 if (c == ' ' && state == 3){
		 if (num == 1){
		 	state = 1;
		 }
		 else{
			 state = 4;
		 }
	 	}
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e' && s[2] == 's' && s[3] == 'e'
        && s[4] == 't' && s[5] == '\0' && state == 9){
//		 Commenting out these lines because I am not using them to show an error
		 printf("PASS \n");
		 //exit(200);	
		 return 0;
    }
	
	 if ((state != prev_state) && (state != prev_state + 1)){
		 printf("Current state: %d\nExpected state: %d or %d\n", state, prev_state, prev_state+1);
	  	 printf("Incorrect state, exiting program\n");
		 printf("FAILED\n"); 
		 return 0;
	 }
  }
}

//0 -> making sure the whole logic/branches work
//1 -> making sure if a state is incorrect my test finds it

int main(int argc, char *argv[])
{
    srand(time(NULL));
	 printf("input from user: %s\n", argv[1]);
	 int a = 0;
	 a = atoi(argv[1]);
	 printf("a: %d\n", a);
			
    testme(a);
    return 0;
}
