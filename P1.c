#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void create_sem(key_t klucz);
void semafor_p(int sem_nr);
void semafor_v(int sem_nr);

FILE *plik;
int semafor;

int main()
{
	key_t klucz=30;
	
	create_sem(klucz);
	
	semafor_p(2);
	
	plik=fopen("wynik.txt","a");
	if(plik==NULL)
	{
		perror("Blad otwarca pliku (P1)");
		exit(1);
	}
	printf("t11 procesu o PID=%d\n", getpid());
	fprintf("t11 procesy o PID=%d\n", getpid());
	sleep(1);
	printf("t12 procesu o PID=%d\n", getpid());
	fprintf("t12 procesu o PID=%d\n", getpid());
	sleep(1);
	fclose(plik);

	semafor_v(3);
}

void create_sem(key_t klucz)
{
	semafor=semget(klucz,5,IPC_CREAT|0666);
	if(semafor==-1)
	{
		perror("Nie udalo sie utworzyc semafora (P1)");
		exit(1);
	}
}

void semafor_p(int sem_nr)
{
	int zmiana;
	struct sembuf bufor_sem;
	bufor_sem.sem_num=sem_nr;
	bufor_sem.sem_op=-1;
	bufor_sem.sem_flg=0;
	zmiana=semop(semafor,&bufor_sem,1);
	if(zmiana==-1)
	{
		perror("Nie zamknieto semafora (P1)");
		exit(1);
	}
	printf("Semafor zamkniety (P1)\n");
}

void semafor_v(int sem_nr)
{
	int zmiana;
	struct sembuf bufor_sem;
	bufor_sem.sem_num=sem_nr;
	bufor_sem.sem_op=1;
	bufor_sem.sem_flg=SEM_UNDO;
	zmiana=semop(semafor,&bufor_sem,1);
	if(zmiana==-1)
	{
		perror("Nie otwarto semafora (P1)");
		exit(1);
	}
	printf("Semafor otwarty (P1)\n");
}