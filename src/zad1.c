#include <stdio.h>
#include <stdlib.h> //exit, atof
#include <fcntl.h>

#include "fun.h"

FILE *input, *output;


/**
* argv[1] - nazwa pliku z wiadomoscia do zdeszyfrowania
*/
int main(int argc, const char* argv[])
{
	char *names[] = {"./kryp/1a", "./kryp/2a", "./kryp/3a", "./kryp/4a", "./kryp/5a", "./kryp/6a", "./kryp/7a", "./kryp/8a", "./kryp/9a", "./kryp/10a", "./kryp/11a", "./kryp/12a", "./kryp/13a", "./kryp/14a", "./kryp/15a", "./kryp/16a", "./kryp/17a", "./kryp/18a", "./kryp/19a", "./kryp/20a"};
	unsigned char buffor[8];
	unsigned char **crypt;
	unsigned char *c;
	long file_length = 0L;
	int msgs_length[20];
	int nr = 0;
	int i, temp, k;
	int max_length = 0;
	int c_length = 0;


	if ( argc < 2)
	{
		printf("Za malo argumentow\n");
		exit(-1);
	}
	
	crypt = malloc(20 * sizeof(unsigned char*));

	for (i = 0; i < 20; i++)
	{
		if ( (input = fopen(names[i], "rb")) == NULL)
		{
			printf("fopen error.");
			exit(-1);
		}

		fseek(input, 0L, SEEK_END);
		file_length = ftell(input);

		file_length -= 1;

		if ( file_length % 8 != 0 )
		{
			printf("Plik uszkodzony");
		}
		nr = file_length / 8;

		crypt[i] = malloc(nr * sizeof(unsigned char));
		msgs_length[i] = nr;
		if (max_length < nr) max_length = nr;

		fseek(input, 0L, SEEK_SET);

		for (k = 0; k < nr; k++)
		{
			fread(&buffor, sizeof(unsigned char), 8, input);
			temp = str2int(buffor, 8, 2);
			crypt[i][k] = (unsigned char)temp;
		}

		fclose(input);
	}

/*
	Wczytanie pliku z zaszyfrowaną wiadomością C
*/
	if ( (input = fopen(argv[1], "rb")) == NULL)
	{
		printf("fopen error.");
		exit(-1);
	}
	fseek(input, 0L, SEEK_END);
	file_length = ftell(input);
	file_length -= 1;
	if ( file_length % 8 != 0 )
	{
		printf("Plik uszkodzony");
	}
	nr = file_length / 8;
	c = malloc(nr * sizeof(unsigned char));
	c_length = nr;
	fseek(input, 0L, SEEK_SET);
	for (k = 0; k < nr; k++)
	{
		fread(&buffor, sizeof(unsigned char), 8, input);
		temp = str2int(buffor, 8, 2);
		c[k] = (unsigned char)temp;
	}
	fclose(input);
/** 
*	Koniec wczytywania c
*/


	int j;
	unsigned char **msg;
	msg = malloc(20 * sizeof(unsigned char*));
	for (i = 0; i < 20; i++) msg[i] = malloc(c_length * sizeof(unsigned char));

	for (i = 0; i < c_length; i++)
	{
		for (j = 0; j < 20; j++)
		{
			msg[j][i] = c[i] ^ crypt[j][i];
		}
	}

	int max;
	int max_value;
	int counter;
	unsigned char *enc;
	enc = malloc(c_length * sizeof(unsigned char));

	for (i = 0; i < c_length; i++)
	{
		max = 0;
		max_value = 0;
		for (j = 0; j < 128; j++)
		{
			if ( isprint((unsigned char)j) )
			{
				counter = 0;
				for (k = 0; k < 20; k++)
				{
					if ( isalpha(msg[k][i] ^ (unsigned char)j) || (unsigned char)(msg[k][i] ^ (unsigned char)j) == ' ') counter += 1;
				}
				if (counter > max)
				{
					max = counter;
					max_value = j;
				}
			}
		}
		enc[i] = (unsigned char)max_value;
	}

	for (i = 0; i < c_length; i++)
	{
		printf("%c", enc[i] );
	}

	char m[c_length+1];
	printf("\nPodaj odczytana wiadomosc: \n");
	fgets(m, c_length+1, stdin);

	printf("\nPozostale wiadomosci: \n");

	for (i = 0; i < 20; i++)
	{
		for (k = 0; k < c_length; k++)
		{
			printf("%c", msg[i][k] ^ m[k]);
		}
		printf("\n");
	}

	for (i = 0; i < 20; i++)
	{
		free(crypt[i]);
		free(msg[i]);
	}
	free(crypt);
	free(msg);
	free(enc);

	exit(0);
}
