
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
#define NPRO 1000


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
	// p->size++;
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
	return temp;
}

// #define SCHED_KEY 121
#define SCHED_KEY 232


#define RR 1000
#define PR 1001


struct msgbuf{
	long type;
	long pid;
	int priority;
	int msg;
	
};
int msqid;
// void printValue(struct msgbuf buff){
// 	printf("PID = %ld, PRIO= %d MSG = %d\n", buff.type, buff.priority, buff.msg);
// }
int rec_fun(long type, struct msgbuf* msg)
{
	memset(msg, 0, sizeof(struct msgbuf));
	
	size_t m = sizeof(struct msgbuf) - sizeof(long);
	int p = msgrcv(msqid,msg,m,type,0);
	if (p == -1){
		perror("msgrcv error detected");//error in recieving
		_exit(0);
	}
	return p;
}

int non_block_recv(long type, struct msgbuf* msg)
{
	memset(msg, 0, sizeof(struct msgbuf));
	size_t m = sizeof(struct msgbuf) - sizeof(long);
	int p = msgrcv(msqid, msg, m, type, IPC_NOWAIT);
	if (p == -1){
		if (errno == ENOMSG){
			return p;
		}
		else{
			perror("msgrcv error");
			_exit(0);
		}
	}
	return p;
}

int notify(pid_t procid){
	union sigval value;
	value.sival_int = SIGUSR1;
	return sigqueue(procid, SIGUSR1, value);
	return kill(procid, SIGUSR1);
}

int suspend(pid_t procid)
{
	union sigval value;
	value.sival_int = SIGUSR1;
	return sigqueue(procid, SIGUSR1, value);
	return kill(procid, SIGUSR2);
}

int checkForProcess_noblock(struct queue *que)
{
	struct msgbuf msgpro;
	int rst = non_block_recv(0, &msgpro);
	if (rst == -1)
		return rst;
	struct elem data;
	data.procid = msgpro.type;
	data.prio = msgpro.priority;
	if (msgpro.msg == 2)
	{
		enqueue(que, data);
	}
	else if (msgpro.msg == 4)
	{
		printf("Process %lld completed IO\n", data.procid);
		enqueue(que, data);
	}

	else
	{
		printf("Process %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
	}
}

int checkForProcess(struct queue *que)
{
	struct msgbuf msgpro;
	int rst = rec_fun(0, &msgpro);
	struct elem data;
	data.procid = msgpro.type;
	data.prio = msgpro.priority;
	if (msgpro.msg ==2)
	{
		enqueue(que, data);
	}
	else if (msgpro.msg == 4)
	{
		printf("Process %lld completed IO\n", data.procid);
		enqueue(que, data);
	}
	else
	{
		printf("Process %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
	}
}

int prio_checkForProcess_noblock(struct prio_queue *que);


int prio_checkForProcess(struct prio_queue *que)
{
	struct msgbuf msgpro;
	int rst = rec_fun(0, &msgpro);
	struct elem data;
	data.procid = msgpro.type;
	data.prio = msgpro.priority;
	if (msgpro.msg == 2)
	{
		pqPush(que, data);
	}
	else if (msgpro.msg == 4)
	{
		printf("Process %lld completed IO\n", data.procid);
		pqPush(que, data);
	}
	else
	{
		printf("Process %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
	}
}

void message_control()
{
	if (msgctl(msqid, IPC_RMID, NULL) < 0){
		perror("msgctl");_exit(0);
	}
	_exit(1);
}

void PR_sched(int time_quantum,int N);


void RR_sched(int time_quantum, int N);


int main(int argc, char* argv[])
{
	signal(SIGINT,message_control);
	if (argc < 4){
		printf("Algorithms : RR->0  PR->1\n");
		printf("Enter in order like: N, Algo, Time_quantum\n");
		exit(0);
	}
	int N = atoi(argv[1]);
	int time_quantum = atoi(argv[3]);
	int algo = atoi(argv[2]);
	printf("%d %d\n", N, time_quantum);
	msqid = msgget(SCHED_KEY, IPC_CREAT | 0666);
	if (msqid < 0){
		perror("msgget error");
		_exit(0);
	}

	if (algo== 0){
		RR_sched(time_quantum, N);
	}
	else if (algo == 1){
		PR_sched(time_quantum, N);
	}
	else{
		printf("Wrong Algorithm Number;Enter Again\n");
		return 0;
	}

	if (msgctl(msqid, IPC_RMID, NULL) < 0){
		perror("msgctl error");
		_exit(0);
	}

	return 0;
}
void RR_sched(int time_quantum, int N)
{
	struct queue ready_queue;
	init(&ready_queue);
	int tp = 0;
	while (isEmpty(&ready_queue))
	{
		struct msgbuf msgpro;
		rec_fun(0, &msgpro);
		struct elem data;
		data.procid = msgpro.type;
		data.prio = msgpro.priority;
		if (msgpro.msg != 2)
		{
			printf("Process %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
			continue;
		}
		enqueue(&ready_queue, data);
	}

	while (1)
	{
		struct msgbuf msgpro;
		int rst = non_block_recv(0, &msgpro);
		if (rst == -1)
			break;
		struct elem data;
		data.procid = msgpro.type;
		data.prio = msgpro.priority;
		if (msgpro.msg != 2)
		{
			printf("Process %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
			continue;
		}
		enqueue(&ready_queue, data);
	}


	while (tp < N){
		struct elem curr_process = deque(&ready_queue);

		notify(curr_process.procid);
		printf("Process id %lld running\n", curr_process.procid);
		int flag = 0;
		int i;
		for (i = 0; i < time_quantum; i++)
		{
			//printf("%d ",i);
			struct msgbuf msgpro;
			if (!(i % 100))
			{
				//printf("Running %ld %d\n",curr_process.procid,i);
			}
			int rst = non_block_recv(curr_process.procid, &msgpro);
			if (rst != -1){
				if (msgpro.msg == 3){
					printf("Process id %lld requests IO\n", curr_process.procid);
					flag = 1;
					break;
				}
				else if (msgpro.msg == 5){
					printf("Process id %lld terminated\n", curr_process.procid);
					while (non_block_recv(curr_process.procid, &msgpro) != -1);
					tp+=1;
					flag = 1;
					break;
				}
				else
				{
					printf("Process id %lld unexpectedly sent %d\n", curr_process.procid, msgpro.msg);
				}
			}
		}
		
		if (!flag)
		{
			suspend(curr_process.procid);
			struct msgbuf msgpro;
			int rst = non_block_recv(curr_process.procid, &msgpro);
			if (rst != -1)
			{

				if (msgpro.msg == 3){
					printf("Process id %lld requests Input/Output\n", curr_process.procid);
				}
				else if (msgpro.msg == 5){
					printf("Process id %lld terminated\n", curr_process.procid);
					while (non_block_recv(curr_process.procid, &msgpro) != -1);tp+=1;
				}
				else if (msgpro.msg == 4){
					enqueue(&ready_queue, curr_process);
				}
				else{
					printf("Process id %lld unexpectedly sent %d\n", curr_process.procid, msgpro.msg);
				}
			}
			else
				enqueue(&ready_queue, curr_process);
		}
		while (1)
		{
			if (checkForProcess_noblock(&ready_queue) == -1)
				break;
		}
		if (tp == N)
			break;
		while (isEmpty(&ready_queue))
			checkForProcess(&ready_queue);
		//print(&ready_queue);
	}
}
void PR_sched(int time_quantum, int N)
{
	struct prio_queue ready_queue;
	heap_start(&ready_queue);
	int tp = 0;
	while (pqEmpty(&ready_queue))
	{
		struct msgbuf msgpro;
		rec_fun(0, &msgpro);
		struct elem data;
		data.procid = msgpro.type;
		data.prio = msgpro.priority;
		if (msgpro.msg != 2)
		{
			printf("Process id %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
			continue;
		}
		pqPush(&ready_queue, data);
	}

	while (1)
	{
		struct msgbuf msgpro;
		int rst = non_block_recv(0, &msgpro);
		if (rst == -1)break;
		struct elem data;
		data.procid = msgpro.type;
		data.prio = msgpro.priority;
		if (msgpro.msg != 2)
		{
			printf("Process id %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
			continue;
		}
		pqPush(&ready_queue, data);
	}


	while (tp < N){
		struct elem curr_process = pqPop(&ready_queue);

		notify(curr_process.procid);
		printf("Process id %lld is running\n", curr_process.procid);
		int flag = 0;
		int i;
		for (i = 0; i < time_quantum; i++)
		{
			//printf("%d ",i);
			struct msgbuf msgpro;
			if (!(i % 100))
			{
				//printf("Running %ld %d\n",curr_process.procid,i);
			}
			int rst = non_block_recv(curr_process.procid, &msgpro);
			if (rst != -1)
			{
				if (msgpro.msg == 3)
				{
					printf("Process id %lld requests IO\n", curr_process.procid);
					flag = 1;
					break;
				}
				else if (msgpro.msg == 5)
				{
					printf("Process id %lld terminated\n", curr_process.procid);
					while (non_block_recv(curr_process.procid, &msgpro) != -1);
					tp++;
					flag = 1;
					break;
				}
				else
				{
					printf("Process id %lld unexpectedly sent %d\n", curr_process.procid, msgpro.msg);
				}
			}
		}
		//printf("\n");
		if (!flag)
		{
			suspend(curr_process.procid);
			struct msgbuf msgpro;
			int rst = non_block_recv(curr_process.procid, &msgpro);
			if (rst != -1){
				if (msgpro.msg == 3){
					printf("Process id %lld requests I/O\n", curr_process.procid);
				}
				else if (msgpro.msg == 5)
				{
					printf("Process id %lld terminated\n", curr_process.procid);
					while (non_block_recv(curr_process.procid, &msgpro) != -1);
					tp++;
				}
				else if (msgpro.msg == 4)
				{
					pqPush(&ready_queue, curr_process);
				}
				else
				{
					printf("Process id %lld unexpectedly sent %d\n", curr_process.procid, msgpro.msg);
				}
			}
			else
				pqPush(&ready_queue, curr_process);
		}
		while (1)
		{
			if (prio_checkForProcess_noblock(&ready_queue) == -1)
				break;
		}
		if (tp == N)
			break;
		while (pqEmpty(&ready_queue))prio_checkForProcess(&ready_queue);
		//print(&ready_queue);
	}
}
int prio_checkForProcess_noblock(struct prio_queue *que)
{
	struct msgbuf msgpro;
	int rst = non_block_recv(0, &msgpro);
	if (rst == -1)
		return rst;
	struct elem data;
	data.procid = msgpro.type;
	data.prio = msgpro.priority;
	if (msgpro.msg == 2)
	{
		pqPush(que, data);
	}
	else if (msgpro.msg == 4)
	{
		printf("Process id %lld completed I/O\n", data.procid);
		pqPush(que, data);
	}

	else
	{
		printf("Process id %ld unexpectedly sent %d\n", msgpro.type, msgpro.msg);
	}
}

