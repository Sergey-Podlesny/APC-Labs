#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define nTimes 65536

int melody[] = {
392, 392, 293, 196, 196, 392, 392, 293, 196, 0
};
int clock[] = {
800, 400, 400, 400, 400, 400, 400, 400, 400, 0
};

void printWords() 
{
	unsigned char state;
	char stateWord[8];
	int i;
	outp(0x43, 0xE2); //11100010 0x40
	state = inp(0x40);
	for (i = 7; i >= 0; i--)
	{
		stateWord[i] = state % 2 + '0';
		state /= 2;

	}
	for (i = 0; i < 8; i++)
	{
		printf("%c", stateWord[i]);
	}
	printf("\n");
	outp(0x43, 0xE4); //11100100 0x41
	state = inp(0x41);
	for (i = 7; i >= 0; i--)
	{
		stateWord[i] = state % 2 + '0';
		state /= 2;

	}
	for (i = 0; i < 8; i++)
	{
		printf("%c", stateWord[i]);
	}
	printf("\n");
	outp(0x43, 0xE8); //11101000 0x42
	state = inp(0x42);
	for (i = 7; i >= 0; i--)
	{
		stateWord[i] = state % 2 + '0';
		state /= 2;

	}
	for (i = 0; i < 8; i++)
	{
		printf("%c", stateWord[i]);
	}
	printf("\n");
}

void setFrequency(unsigned int freq) 
{

	long base = 1193180, kd;
	outp(0x43, 0xB6);
	kd = base / freq;
	outp(0x42, kd % 256);
	kd /= 256;
	outp(0x42, kd);
}

void play() {


	for (int i = 0; melody[i] != 0 || clock[i] != 0; i++) 
	{

		setFrequency(melody[i]);
		outp(0x61, inp(0x61) | 3);
		delay(clock[i]);
		outp(0x61, inp(0x61) & 0xfc);

	}
}

int main() 
{
	char selection;

	while (1) 
	{
		printf("1: Play sound \n");
		printf("2: Print condition words\n");
		printf("0: Exit(e)\n\n");
		selection = getch();

		switch (selection) 
		{
		case '1':
			system("cls");
			play();	
			break;
		case '2':
			system("cls");
			printWords();
			getch();
			break;
		case '0':
			printf("Exiting...");
			return 0;
		default:
			printf("Error key! Please try again\n");
			break;
		}
	}
}