#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#define MAXN 100



int main(int argc, char* argv[])
{
	

	int N=4,t=1;
	for(int i = 0;i < N;i++){
		int NOI,priority;//no of itearation and priority
		double sprob;//probablity for I/O
		long start_time; //start time

		scanf("%d%d%lf%ld",&NOI,&priority,&sprob,&start_time);

		if(fork() == 0)
		{
			char arrNOI[20];
			char arrprio[20];
			char arrsprob[20],arrstime[20];
			sprintf(arrNOI,"%d",NOI);
			sprintf(arrprio,"%d",priority);
			sprintf(arrsprob,"%lf",sprob);
			sprintf(arrstime,"%ld",start_time);
			execlp("xterm","xterm","-e","./process",arrNOI,arrprio,arrsprob,arrstime,(char*)NULL);
		}
		sleep(t);
	}

}