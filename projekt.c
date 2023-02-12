#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int semafor;

void create_sem(key_t klucz);
void ini_sem();
void clear_sem();

int main()
{
	char *procesy[3]={"P1","P2","P3"};
	char tablica[10];
	key_t klucz=30;
	int i;

	create_sem(klucz);
	ini_sem();
	
	for(i=0;i<5;i++)
	{
		switch(fork())
		{
			case -1:
				perror("Blad funkcji fork()");
				clear_sem();
				exit(1);
			case 0:
				tablica[0]='\0';
				strcat(tablica, "./");
				strcat(tablica, procesy[i]);
				strcat(tablica, ".out");
				int ex=execl(tablica, procesy[i],NULL);
				if(ex==-1)
				{
					perror("Blad funkcji execl()");
					exit(1);
				}
		}
	}
	int kod, status;
	for(i=0;i<5;i++)
	{
		kod=wait(&status);
		if(kod==-1)
		{
			perror("Blad funkcji wait()");
			exit(1);
		}
	}
	clear_sem();
}

void create_sem(key_t klucz)
{
	semafor=semget(klucz,5,IPC_CREAT|0666);
	if(semafor==-1)
	{
		perror("Nie udalo sie utworzyc semafora");
		exit(1);
	}
	printf("Semafor zostal utworzony\n");
}

void ini_sem()
{
	int i;
	for(i=0;i<5;i++)
	{
		int ustaw_sem;
		ustaw_sem=semctl(semafor,i,SETVAL,0);
		if(ustaw_sem==-1)
		{
			perror("Blad inicjalizacji semafora");
			exit(1);
		}
	}
	printf("Semafor zostal ustawionny\n");
}

void clear_sem()
{
	int usun_sem;
	usun_sem=semctl(semafor,5,IPC_RMID, NULL);
	if(usun_sem==-1)
	{
		perror("Semafor nie zostal usuniety");
		exit(1);
	}
}