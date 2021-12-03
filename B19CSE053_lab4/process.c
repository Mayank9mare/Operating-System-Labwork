#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h> 
const int SCHED_KEY= 232;
#define tval timeval
// #define READY 2
// #define IO 3
// #define IOOVER 4
// #define TERMINATE 5
// #define NPRO 100
struct elem{
    long long int procid;
    int prio;
};

struct queue{
    struct elem array[NPRO];
    int rear,front;
};

struct prio_queue{
    struct elem heap[NPRO+1];
    int size;
};


void init(struct queue * q) {
    q->rear = 0;
    q->front = 0;
} 
 
int isFull(struct queue * q) {
    return ((q->rear+1)%NPRO== q->front)?1:0;
}

int isEmpty(struct queue *q ) {
    return (q->front == q->rear);
}

int enqueue(struct queue * q,struct elem e) {
    if (isFull(q)) {
        return -1;
    }
    q->array[q->rear++] = e;
    q->rear %= NPRO;
}

struct elem deque(struct queue * q){
    struct elem d = q->array[q->front++];
    q->front %= NPRO;
    return d;
}


void heap_start(struct prio_queue * p) {
    p->size = 0;
    struct elem m;
    m.procid = 0;
    m.prio = INT_MIN;
    p->heap[0] = m;
}

int pqEmpty(struct prio_queue * p) {
    return p->size ==0;
}

int pqFull(struct prio_queue *p) {
    return (p->size==NPRO);
}
void heapifymX(int index,struct prio_queue *p){
    if(index == 0){
        return ;
    }
    int li = 2*index;
    int ri = 2*index+1;
    int largest = index;
    if(li<=p->size && p->heap[largest].prio < p->heap[li].prio){
        largest = li;
    }if(ri<= p->size && p->heap[largest].prio<p->heap[ri].prio){
        largest = ri;
    }
    if(largest != index){
        struct elem temp = p->heap[largest];
        p->heap[largest] = p->heap[index];
        p->heap[index] = temp;
        heapifymX(largest,p);
    }
}
void heapifyX_arr(struct prio_queue * p){
    int a = p->size/2;
    for(int i=a;i>-1;i--){
        heapifymX(i,p);
    }
}

void pqPush(struct prio_queue * p,struct elem e) {
	p->heap[++p->size] = e;
    heapifyX_arr(p);
}


struct elem pqPop(struct prio_queue * p) {
    int index = 1;
    // p->size--;
    struct elem temp = p->heap[index];
    p->heap[index] = p->heap[p->size];
    p->heap[p->size--] = temp;
    heapifyX_arr(p);
}


 typedef struct msgbuf
{
	long type;
	long pid;
	int priority;
	int msg;
}ms;

pid_t pid;
int priority;
int msqid;

int tsub (struct tval *result, struct tval *x, struct tval *y){
  if (x->tv_usec<y->tv_usec) {
    int nsec = (y->tv_usec-x->tv_usec) / 1000000 + 1;
    y->tv_usec-= 1000000 * nsec;
    y->tv_sec+= nsec;
  }
  if (x->tv_usec-y->tv_usec >1000000) {
    int nsec = (x->tv_usec-y->tv_usec)/1000000;
    y->tv_usec += 1000000*nsec;
    y->tv_sec -= nsec;
  }
  result->tv_sec= x->tv_sec - y->tv_sec;
  result->tv_usec= x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
  return 0;
}

int sendmsg(int msg) {

	ms buf={.type=pid, .pid =pid,.priority=priority,.msg=msg}; //1change
	size_t msgsize = sizeof(ms) - sizeof(long);

	int p = msgsnd(msqid, &buf, msgsize, 0);
	if (p == -1) {
		perror("msgsnd");
		_exit(0);
	}
	return p;
}

//time_t ltime;
struct tval ltime;
struct tval total_waiting_time;
int notified = 0;
void sigma_suspend(int sig);

void sigma_notify(int sig)
{
	notified = 1;
}

int main(int argc, char *argv[])
{
	//time_t start_time;
	struct tval start_time,t2,result;
	struct tval response_time, total_turnaround_time;
	total_waiting_time.tv_sec= 0;
	total_waiting_time.tv_usec=0;

	struct sigaction saction;
	saction.sa_handler = sigma_notify;
	sigaction(SIGUSR1, &saction, NULL);

	struct sigaction saction2;
	saction2.sa_handler = sigma_suspend;
	sigaction(SIGUSR2, &saction2, NULL);

	gettimeofday(&start_time, NULL);
	//printf("%d\n",argc);
	if (argc < 5) {
		printf("process NOI priority sleep_p sleep_t\n");
		_exit(0);
	}
	pid = getpid();
	int NOI = atoi(argv[1]);
	priority = atoi(argv[2]);
	double sleep_p = atof(argv[3]);
	long sleep_t = atoi(argv[4]);
	printf("%d %d %lf %ld\n", NOI, priority, sleep_p, sleep_t);
	msqid = msgget(SCHED_KEY, 0666);
	if (msqid == -1) {
		perror("msgget"); 
		_exit(2);
	}
	srand(time(NULL));
	int i;
	sendmsg(2);
	ltime = start_time;
	sigma_suspend(0);
	gettimeofday(&t2, NULL);
	//response_time = (t2.tv_usec - start_time.tv_usec);
	timersub(&t2,&start_time,&ltime);
	if(ltime.tv_sec>=0 && ltime.tv_usec>=0) {
		response_time =ltime;
	}
	for (i = 0; i < NOI; i++)
	{
		double r = rand() * 1.0 / INT_MAX;
		if (r < sleep_p)
		{
			signal(SIGUSR2,SIG_IGN);
			sendmsg(3);
			printf("PID:%d IO Time start\n", pid);
			sleep(sleep_t);
			printf("PID:%d IO Time end\n", pid);   
			//ltime = time(0);
			sendmsg(4);
			sigma_suspend(0);
			signal(SIGUSR2,sigma_suspend);
		}
		printf("PID:%d, %d\n", pid, i);
	}
	gettimeofday(&t2, NULL);
	//total_turnaround_time = (t2.tv_sec - start_time.tv_sec);
	timersub(&t2,&start_time,&ltime);
	if(ltime.tv_sec>=0 && ltime.tv_usec>=0) {
		total_turnaround_time =ltime;
	}
	sendmsg(5);
	printf("writing final results to the file\n");
	FILE *fp;
	char s[100];
	sprintf(s,"output%d.txt",pid);
	fp=fopen(s,"w");
	fprintf(fp,"%d %d %lf %ld\n", NOI, priority, sleep_p, sleep_t);
	fprintf(fp,"%d %ld %ld %ld\n",pid,response_time.tv_usec,total_waiting_time.tv_usec,total_turnaround_time.tv_usec);
	fclose(fp);
	
	return 0;
}
void sigma_suspend(int sig) {
	sigset_t mask, oldmask;

	sigemptyset (&mask);
	struct tval t1,t2,result;
	sigaddset (&mask, SIGUSR2);
	gettimeofday(&t1, NULL);
	sigprocmask (SIG_BLOCK, &mask, &oldmask);//waiting for signal
	while (!notified)
		sigsuspend (&oldmask);
	notified = 0;
	gettimeofday(&t2, NULL);
	timersub(&t2,&t1,&ltime);
	if(ltime.tv_sec>=0 && ltime.tv_usec>=0) {timeradd(&total_waiting_time,&ltime,&total_waiting_time);}
	sigprocmask (SIG_UNBLOCK, &mask, NULL);

}

