#include <stdio.h>
#include "resetpins.c"
#include <string.h>
#include <wiringPi.h>

int isPinValid (int);
void executeCommand (int,int[],int);
void handleCommand (int,char *[]);
void printHelpText (void);

int ALLVALIDPINS[28]={ 3,5,7,8,10,11,12,13,15,16,18,19,21,22,23,24,26,27,28,29,31,32,33,35,36,37,38,40 };
int NOOFVALIDPINS=28;
int ALLINVALIDPINS[10] = {4,6,9,14,17,20,25,30,34,39};
int NOOFINVALIDPINS=10;
#define SUPPORTTEXT "Use sudo ./setpinsto -help for usage information\n"

void main (int argc, char *argv[])
{
	handleCommand (argc,argv);
}

void handleCommand (int argc, char *argv[])
{
	if ( 0==strcmp("-help",argv[1] ) )
	{
		printHelpText();
	}
	else
	{
		int state = -1;

		if ( 0==strcmp("-LOW",argv[1]) )
			state = LOW;
		else if ( 0==strcmp("-HIGH",argv[1]) )
			state = HIGH;

		if ( 0==strcmp("all",argv[2]) )
		{
			executeCommand (NOOFVALIDPINS,ALLVALIDPINS,state);
		}
		else
		{
			int itmp[argc];
			int cmdoffset = 2;

			for ( int i=cmdoffset; i<argc; i++ )
				itmp[i-cmdoffset] = atoi(argv[i]);

			wiringPiSetup();
			executeCommand ((argc-cmdoffset),itmp,state);
		}
	}
}

void printHelpText (void)
{
	printf ("SYNTAX:\t sudo ./setpinsto command [pins]\n");
	printf ("\tcommand:\n");
	printf ("\t\t -help\t print help text\n");
	printf ("\t\t -LOW\t set pins to low state\n");
	printf ("\t\t -HIGH\t set pins to high state\n");
	printf ("\tpins:\n");
	printf ("\t\t all\t reset all valid pins\n");
	printf ("\t\t x y ..\t pins for wich to set the state ");
	printf ("(valid pins are:");
	for (int i=0; i<NOOFVALIDPINS; i++)
		printf(" %d",ALLVALIDPINS[i]);
	printf(")\n");
	printf ("\tExamples:\n");
	printf ("\t\t sudo ./setpinsto -help\n");
	printf ("\t\t sudo ./setpinsto -LOW all\n");
	printf ("\t\t sudo ./setpinsto -HIGH 3 5 7 8 12\n");

}

void executeCommand ( int argc, int pins[], int state )
{
	for (int i=0; i<argc; i++)
	{
		if ( isPinValid (pins[i]) )
		{
			if ( LOW==state )
				printf ("Pin %d is set to LOW\n" , pins[i] );
			else if ( HIGH==state )
				printf ("Pin %d is set to HIGH\n" , pins[i] );
			else
			{
				printf ("State is invalid please use HIGH or LOW to define state! Nothing Done.\n");
				printf (SUPPORTTEXT);
				break;
			}

			pinMode ( pins[i], OUTPUT );
			digitalWrite ( pins[i], state );
		}
		else
		{
			printf ("Pin %d is not valid for RPI3! Nothing Done.\n", pins[i] );
			printf (SUPPORTTEXT);
		}
	}
}

int isPinValid (int pinno)
{
	if ( ALLVALIDPINS[0] > pinno )
	{
		return FALSE;
	}

	if ( ALLVALIDPINS[NOOFVALIDPINS-1] < pinno )
	{
		return FALSE;
	}

	for ( int i=0; i < NOOFINVALIDPINS; i++ )
	{
		if ( ALLINVALIDPINS[i] == pinno ) { return FALSE; }
		if ( ALLINVALIDPINS[i] > pinno ) { return TRUE; }
	}

	return TRUE;
}
