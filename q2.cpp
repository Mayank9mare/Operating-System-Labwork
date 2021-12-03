#include<bits/stdc++.h>
#define pb push_back
#define mk make_pair
#define ll long long
#define pll pair<ll,ll>
#define vll vector<ll>
#define mll map<ll,ll>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

using namespace std;
#define N 100
 struct Node{
     int d;
     struct Node* next;
 };
 void enqueue(struct Node** head,int data){
     struct Node* nn =(struct Node*)malloc(sizeof(struct Node));
     nn->d=data;
     nn->next=NULL;
     struct Node* temp=*head;
     if(*head==NULL){
         *head=nn;
         return;
     }
     while(temp->next!=NULL){
         temp=temp->next;
     }
     temp->next=nn;

 }
 int denque(struct Node** head){
      struct Node* temp=*head;
     *head=(*head)->next;
     return temp->d;
 }
 int isEmpty(struct Node* head){
     if(head==NULL){
         return 1;
     }
     return 0;
 }
 int front(struct Node* p){
     if(p==NULL){
         printf("empty");
     }
     else{
     while(p->next!=NULL){
         p=p->next;
         
     }
     return p->d;
     }
 }
 int back(struct Node* p){
     if(!isEmpty(p)){
     return p->d;
     }
 }
 void printList(struct Node* p){
     while(p!=NULL){
         printf("%d ",p->d);
         p=p->next;
     }
     printf("\n");
 }
int start[N];
int burst[N];

int main(){
    printf("enter number of processes: ");
    int n;

    scanf("%d",&n);
    printf("enter starting time for each process: ");
    for(int i=0;i<n;i++){
        scanf("%d",&start[i]);

    }
    printf("enter burst time for each process: ");

for(int i=0;i<n;i++){
    scanf("%d",&burst[i]);
}
int t;
printf("enter time quantum t");
scanf("%d",&t);
//sorting 
vector<pair<int,int>> v;
map<int,int> m;
for(int i=0;i<n;i++){
    v.push_back({start[i],i});
}
sort(v.begin(),v.end());
vector<pair<int,pair<int,int>>> a;
for(int i=0;i<n;i++){
    a.push_back({start[i],{burst[i],i}});
}
sort(a.begin(),a.end());
int b[N];
for(int i=0;i<n;i++){
    b[i]=a[i].second.first;
}
queue<pair<int,int>> q;
int s=start[0];
int end[N];
int vis[N];
memset(vis,0,sizeof(vis));
vis[0]=1;
q.push({0,a[0].second.second});
while(q.size()){
    pll p=q.front();
    q.pop();
    int f=0;
    if(t>=b[p.first]){
        s+=b[p.first];
        end[p.second]=s;
        b[p.first]=0;
    }
    else{
        f=1;
        s+=t;
        b[p.first]-=t;

    }
    for(int i=0;i<n;i++){
        if(a[i].first<=s && vis[i]==0){
            vis[i]=1;
            q.push({i,a[i].second.second});
        }
    }
    if(f){
        q.push(p);
    }
}
for(int i=0;i<n;i++){
    cout<<"Process "<<i+1<<" "<<"Start time ="<<start[i]<<" "<<"Burst time= "<<burst[i]<<" "<<"Completion time = "<<end[i]<<" "<<"Turn aroudn time "<< end[i]-start[i]<<endl;

}


}