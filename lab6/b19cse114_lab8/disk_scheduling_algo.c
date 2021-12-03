#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

typedef struct entry{
    int wt;
    int tat;
    int ct;
}ent;

typedef struct arrent{
    int val;
    int idx;
}arent;

int compfun(const void * a,const void * b){
    return ((*(struct arrent*)a).val - (*(struct arrent *)b).val);
}
int compfun1(const void * a,const void * b){
    return ((*(struct arrent*)b).val - (*(struct arrent *)a).val);
}
// assuming arrival time is same for all of the requests
void FCFS(int * rc,int noc,int hd,int rcl){
    int * arr = malloc(noc*sizeof(int));
    for(int i =0;i<noc;i++){
        arr[i] = 0;
    }

    int mv=0;
    
    ent * entar = (ent *)malloc(rcl * sizeof(ent));
    for(int i = 0;i<rcl;i++){
        entar[i].wt = 0;
        entar[i].tat = 0;
        entar[i].ct = 0;
    }
    int ct =0;
    for (int i = 0;i<rcl;i++){
        int mnt = abs(rc[i]-hd);
        hd = rc[i];
        mv += mnt;
        ct += 1*mnt + 100;
        entar[i].tat = ct-0;
        entar[i].ct = ct;
        entar[i].wt = entar[i].tat - 100;
    }

    float avgtat=0,avgwt = 0;
    


    printf("sequence of requests is: \n");
    for (int i = 0;i<rcl;i++){
        printf("%d ",rc[i]);
    }
    printf("\n");
    printf("waiting time for each request\n");
    for(int i = 0;i<rcl;i++){
        printf("%d ",entar[i].wt);
        avgwt += (entar[i].wt)/rcl;
    }
    printf("\naverage waiting time is: %f\n",avgwt);
    printf("turnaround time for each request\n");
    for(int i = 0;i<rcl;i++){
        printf("%d ",entar[i].tat);
        avgtat+= (entar[i].tat)/rcl;
    }
    printf("\naverage turnaround time is: %f\n",avgtat);
    printf("total cyliner movement is : %d\n",mv);

}

void SSTF(int * rc,int noc,int hd,int rcl){
    int * arr = malloc(noc*sizeof(int));
    for(int i =0;i<noc;i++){
        arr[i] = 0;
    }

    int mv=0;
    
    ent * entar = (ent *)malloc(rcl * sizeof(ent));
    for(int i = 0;i<rcl;i++){
        entar[i].wt = 0;
        entar[i].tat = 0;
        entar[i].ct = 0;
    }
    int ct =0;
    int seq=0;
    int * idxarr = malloc(rcl * sizeof(int));
    int index=0;
    while (seq < rcl){
        int min_dist = INT_MAX;
        int idx = -1;
        for (int i = 0;i<rcl;i++){
            if(arr[rc[i]]!=0) continue;
            int a = abs(rc[i]-hd);
            if (a < min_dist){
                min_dist = a;
                idx = i;
            }            
        }
        idxarr[index++] = idx;
        seq++;
        arr[rc[idx]] = seq;
        int mnt = abs(rc[idx]-hd);
        hd = rc[idx];
        mv += mnt;
        ct += 1*mnt + 100;
        entar[idx].tat = ct-0;
        entar[idx].ct = ct;
        entar[idx].wt = entar[idx].tat - 100;
    }

    float avgtat=0,avgwt = 0;

    printf("sequence of requests is: \n");
    for(int i= 0;i<rcl;i++){
        printf("%d ",rc[idxarr[i]]);
    }
    printf("\n");
    printf("waiting time for each request\n");
    for(int i = 0;i<rcl;i++){
        printf("%d ",entar[idxarr[i]].wt);
        avgwt += (entar[idxarr[i]].wt)/rcl;
    }
    printf("\naverage waiting time is: %f\n",avgwt);
    printf("turnaround time for each request\n");
    for(int i = 0;i<rcl;i++){
        printf("%d ",entar[idxarr[i]].tat);
        avgtat+= (entar[idxarr[i]].tat)/rcl;
    }
    printf("\naverage turnaround time is: %f\n",avgtat);
    printf("total cyliner movement is : %d\n",mv);
}

void SCAN(int * rc,int noc,int hd,int rcl){
    int * arr = malloc(noc*sizeof(int));
    for(int i =0;i<noc;i++){
        arr[i] = 0;
    }

    int mv=0;
    
    ent * entar = (ent *)malloc(rcl * sizeof(ent));
    for(int i = 0;i<rcl;i++){
        entar[i].wt = 0;
        entar[i].tat = 0;
        entar[i].ct = 0;
    }
    int ct =0;

    arent * gt = malloc(rcl*sizeof(arent));
    arent * lt = malloc(rcl*sizeof(arent));
    int gti = 0,lti = 0;
    for (int i =0;i<rcl;i++){
        if(rc[i]>=hd){
            gt[gti].idx = i;
            gt[gti].val= rc[i];
            gti++;
        }else{
            lt[lti].idx = i;
            lt[lti].val =rc[i];
            lti++;
        }
    }
    
    if (gti ==0){
        mv += noc-1-hd;
        ct += noc-1-hd;
        hd = noc-1;
    }
    if(gti >0){
        qsort(gt,gti,sizeof(arent),compfun);
        
        for (int i = 0;i<gti;i++){
            int mnt = abs(gt[i].val-hd);
            hd = gt[i].val;
            mv += mnt;
            ct += 1*mnt + 100;
            entar[gt[i].idx].tat = ct-0;
            entar[gt[i].idx].ct = ct;
            entar[gt[i].idx].wt = entar[gt[i].idx].tat - 100;
        }
        mv += noc-1-hd;
        ct += noc-1-hd;
        hd = noc-1;
    }
    if (lti >0){
        qsort(lt,lti,sizeof(arent),compfun1);
        for (int i = 0;i<lti;i++){
            int mnt = abs(lt[i].val-hd);
            hd = lt[i].val;
            mv += mnt;
            ct += 1*mnt + 100;
            entar[lt[i].idx].tat = ct-0;
            entar[lt[i].idx].ct = ct;
            entar[lt[i].idx].wt = entar[lt[i].idx].tat - 100;
        }
    }   

    float avgtat=0,avgwt = 0;

    printf("sequence of requests is: \n");
    for (int i = 0;i<gti;i++){
        printf("%d ",gt[i].val);
    }
    for (int i = 0;i<lti;i++){
        printf("%d ",lt[i].val);
    }
    printf("\n");
    printf("waiting time for each request\n");
    for(int i = 0;i<gti;i++){
        printf("%d ",entar[gt[i].idx].wt);
        avgwt += (entar[gt[i].idx].wt)/rcl;
    }
    for (int i = 0;i<lti;i++){
        printf("%d ",entar[lt[i].idx].wt);
        avgwt += (entar[lt[i].idx].wt)/rcl;
    }
    printf("\naverage waiting time is: %f\n",avgwt);
    printf("turnaround time for each request\n");
    for(int i = 0;i<gti;i++){
        printf("%d ",entar[gt[i].idx].tat);
        avgtat+= (entar[gt[i].idx].tat)/rcl;
    }
    for(int i = 0;i<lti;i++){
        printf("%d ",entar[lt[i].idx].tat);
        avgtat+= (entar[lt[i].idx].tat)/rcl;
    }
    printf("\naverage turnaround time is: %f\n",avgtat);
    printf("total cyliner movement is : %d\n",mv);
    
}

void CSCAN(int * rc,int noc,int hd,int rcl){
    int * arr = malloc(noc*sizeof(int));
    for(int i =0;i<noc;i++){
        arr[i] = 0;
    }

    int mv=0;
    
    ent * entar = (ent *)malloc(rcl * sizeof(ent));
    for(int i = 0;i<rcl;i++){
        entar[i].wt = 0;
        entar[i].tat = 0;
        entar[i].ct = 0;
    }
    int ct =0;

    arent * gt = malloc(rcl*sizeof(arent));
    arent * lt = malloc(rcl*sizeof(arent));
    int gti = 0,lti = 0;
    for (int i =0;i<rcl;i++){
        if(rc[i]>=hd){
            gt[gti].idx = i;
            gt[gti].val= rc[i];
            gti++;
        }else{
            lt[lti].idx = i;
            lt[lti].val =rc[i];
            lti++;
        }
    }
    
    if (gti ==0){
        mv += noc-1-hd;
        ct += noc-1-hd;
        hd = noc-1;
        mv += hd;
        ct += hd;
        hd = 0;
    }
    if(gti >0){
        qsort(gt,gti,sizeof(arent),compfun);
        
        for (int i = 0;i<gti;i++){
            int mnt = abs(gt[i].val-hd);
            hd = gt[i].val;
            mv += mnt;
            ct += 1*mnt + 100;
            entar[gt[i].idx].tat = ct-0;
            entar[gt[i].idx].ct = ct;
            entar[gt[i].idx].wt = entar[gt[i].idx].tat - 100;
        }
        mv += noc-1-hd;
        ct += noc-1-hd;
        hd = noc-1;
        mv += hd;
        ct += hd;
        hd = 0;
    }
    if (lti >0){
        qsort(lt,lti,sizeof(arent),compfun);
        for (int i = 0;i<lti;i++){
            int mnt = abs(lt[i].val-hd);
            hd = lt[i].val;
            mv += mnt;
            ct += 1*mnt + 100;
            entar[lt[i].idx].tat = ct-0;
            entar[lt[i].idx].ct = ct;
            entar[lt[i].idx].wt = entar[lt[i].idx].tat - 100;
        }
    }   

    float avgtat=0,avgwt = 0;

    printf("sequence of requests is: \n");
    for (int i = 0;i<gti;i++){
        printf("%d ",gt[i].val);
    }
    for (int i = 0;i<lti;i++){
        printf("%d ",lt[i].val);
    }
    printf("\n");
    printf("waiting time for each request\n");
    for(int i = 0;i<gti;i++){
        printf("%d ",entar[gt[i].idx].wt);
        avgwt += (entar[gt[i].idx].wt)/rcl;
    }
    for (int i = 0;i<lti;i++){
        printf("%d ",entar[lt[i].idx].wt);
        avgwt += (entar[lt[i].idx].wt)/rcl;
    }
    printf("\naverage waiting time is: %f\n",avgwt);
    printf("turnaround time for each request\n");
    for(int i = 0;i<gti;i++){
        printf("%d ",entar[gt[i].idx].tat);
        avgtat+= (entar[gt[i].idx].tat)/rcl;
    }
    for(int i = 0;i<lti;i++){
        printf("%d ",entar[lt[i].idx].tat);
        avgtat+= (entar[lt[i].idx].tat)/rcl;
    }
    printf("\naverage turnaround time is: %f\n",avgtat);
    printf("total cyliner movement is : %d\n",mv);
    
}
int main(int argc, char * argv[]){
    int n;
    printf("enter the number of cylinder: ");
    scanf("%d",&n);
    char c = getchar();
    printf("enter the cylinder request: ");
    char arr[100000];
    int idx = 0;
    char cc = getchar();
    arr[idx++] = cc;
    while (cc !='\n'){
        cc = getchar();
        arr[idx++] = cc;
    }

    arr[idx-1] = '\0';
    idx = idx-1;
    int value[100000];
    int vi=0;

    int i = 0;
    while (i < idx){
        if (arr[i] ==' ' || arr[i] == '\0'){
            i++;
            continue;
        } else if(arr[i] >='0' && arr[i] <='9'){
            char temp[10000];
            int tempidx =0;
            while(arr[i] !=' ' && arr[i]!='\0'){
                temp[tempidx++] = arr[i++];
            }
            temp[tempidx] = '\0';
            value[vi++] = atoi(temp);
        }
        i++;
    }
    printf("Enter the current head position: ");
    int hd;
    scanf("%d",&hd);
    printf("(FOR FCFS)\n");
    FCFS(value,n,hd,vi);
    printf("(FOR SSTF)\n");
    SSTF(value,n,hd,vi);
    printf("(FOR SCAN)\n");
    SCAN(value,n,hd,vi);
    printf("(FOR CSCAN)\n");
    CSCAN(value,n,hd,vi);
    
    return 0;
}