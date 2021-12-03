#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<time.h>


void merge(int a[],int l,int m,int r){
	//int len=r-l+1;
	int b[r+1];
	int i=l;
	int j=m+1;
	int k=l;
	while(i<=m && j<=r){
		if(a[i]<a[j]){
			b[k++]=a[i++];
		}
		else{
			b[k++]=a[j++];
		}

	}
	for(;i<=m;i++){
		b[k++]=a[i];
	}
	for(;j<=r;j++){
		b[k++]=a[j];
	}
	for(i=l;i<=r;i++){
		a[i]=b[i];
		//printf("%d\n",a[i]);
	}
	//printf("\n");
	
}

void merge_sort(int a[],int l,int r){
	//int length=r-l+1;

	if(l>=r)return ;
	int m=l+(r-l)/2;
	pid_t lpid,rpid;
	lpid=fork();
	if(lpid<0){
		perror("Left child not created");
		_exit(-1);
	}
	else if(lpid==0){
		merge_sort(a,l,m);
		_exit(0);
	}
	else{
		rpid=fork();
		if(rpid<0){
			perror("Right child not created");
			_exit(-1);
		}
		else if(rpid==0){
			merge_sort(a,m+1,r);
			_exit(0);
		}
	}
	int s;
	waitpid(lpid,&s,0);
	waitpid(rpid,&s,0);
	merge(a,l,m,r);
	return ;
}

void generate_random(int n,char * filename){
	FILE *fptr;
	fptr=fopen(filename,"w");
	if(fptr==NULL){
		printf("error\n");
		return ;
	}
	for(int i=0;i<n;i++){
		fprintf(fptr, "%d ",rand()%1000);
	}
	fclose(fptr);

	return;

}



int main(int argc , char * argv[]){
	int shmid;
	key_t key=IPC_PRIVATE;
	int *a;
	int length=100;
	size_t SHM_SIZE=sizeof(int)*length;
	if((shmid=shmget(key,SHM_SIZE,IPC_CREAT | 0666))<0){
		perror("shmget");
		_exit(1);
	}
	if((a=shmat(shmid,NULL,0))==(int*)-1){
		perror("shmat");
		_exit(1);
	}
	srand(time(0));

	// generate_random(length,argv[1]);

	FILE *fptr1;
	fptr1=fopen(argv[1],"r");
	if(fptr1==NULL){
		printf("Error");
		return 0;
	}

	int c = fgetc(fptr1);
	if (c == EOF) {
		printf("File is empty so generating random numbers and storing in it \n");
		generate_random(length,argv[1]);
	}

	FILE * fptr;
	fptr = fopen(argv[1],"r");
	if (fptr == NULL) {
		printf("Error\n");
		return 0;
	}
	for(int i=0;i<length;i++){
		fscanf(fptr,"%d ",&a[i]);
	 	
	}

	fclose(fptr);
    printf("Before: ");
    for(int i=0;i<length;i++){
    	printf("%d ",a[i]);
    }
    printf("\n");
	//fillData(shm_array,length);
	merge_sort(a,0,length-1);
	printf("After: ");
	for(int i=0;i<length;i++){
		printf("%d ",a[i]);
	}

	FILE * fpr;
	fpr = fopen("output.txt","w");
	if (fpr == NULL) {
		printf("Error\n");
		return 0;
	}

	for (int i =0;i<length;i++) {
		fprintf(fpr, "%d ",a[i]);
	}
	
	fclose(fpr);
	printf("\n");
	if(shmdt(a)==-1){
		perror("shmdt");
		_exit(1);
	}
	if(shmctl(shmid,IPC_RMID,NULL)==-1){
		perror("shmctl");
		_exit(1);
	}
	//generate_random(10);


	

	return 0;
}