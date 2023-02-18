#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		printf("Numero di parametri errato\n");
		exit(1);
	}

	int p1p0[2];
	int fd, pid, status, boolean=0, countG=0, countL, nread;
	char ricerca[1024];
	char letturaPipe[1024];
	fd=open(argv[1], O_RDONLY);
	if(fd>0)
	{
		//file esiste
		close(fd);
	}
	else
	{
		printf("File non esiste\n");
		exit(1);
	}
	while(boolean==0)
	{
		printf("Inserisci una stringa (digita fine per uscire): ");
		scanf("%s", ricerca);
		if(strcmp(ricerca, "fine")==0)

		{
			boolean=1;
		}
		else
		{
			pipe(p1p0);
			pid=fork();
			
			if(pid==0)
			{	//figlio
				close(p1p0[0]);
				close(1);
				dup(p1p0[1]);
				close(p1p0[1]);
				execlp("grep", "grep", "-c", ricerca, argv[1], NULL);
				exit(1);
			}
			close(p1p0[1]);
			while((nread=read(p1p0[0], letturaPipe, sizeof(letturaPipe)))>0)
			{
				countL=atoi(letturaPipe);
				printf("La parola \"%s\" si è ripetuta per %d volte\n", ricerca, countL);
				countG= countG+countL;
			}
			close(p1p0[0]);
			wait(&status);
		}
	}
	printf("Abbiamo trovato per %d volte le parole cercate\n", countG);
}
