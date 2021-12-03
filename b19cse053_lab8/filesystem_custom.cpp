#include <bits/stdc++.h>

using namespace std;

#define N 5000
#define endl "\n"
int block_size;
int max_file_size;
int rem=N-2;

struct super_block{
    string emp_string=""; 
}sup ;

struct block{
    bool issuper;
    string emp_strings;
    int occupied;
    string filedata;
    int block_number;
    struct block * next;
    block(){
        issuper = false;
        emp_strings="";
        for(int i=0;i<N;i++) emp_strings+='0';
        emp_strings[0] = '1';//super block
        emp_strings[1] = '1';//fat
        occupied = 0;
        next = NULL;
    }    
};

vector<struct block *> blks;

struct file{
    int sz=0;
    string file_name;
    string data;
    int open;
    file(string daata,int size,string filename){
        data = daata;
        sz = size;
        file_name = filename;
        open =0;
    }
};

struct directory{
   
    string name;
    int size;
    unordered_map<string,int> fat;
    vector<file *> aof;
    directory(string s){
        name=s;
        size=0;
    }  
};

vector<directory*> dirs;
directory* mk_dir(){
    string dir_name;
    cout<<"Enter dir name: ";
    cin >> dir_name;
    directory * dir = new directory(dir_name);
    dirs.push_back(dir);
    return dir;
}

directory* add_file(directory* d){
    if(d== NULL) {
        cout <<"directory does not exist\n";
    }
    string name;
    cout<<"Enter File name: \n";
    cin>>name;
    cout<<"ENTER File size : \n";
    int sz;
    cin>>sz;
    if (d->fat.find(name) != d->fat.end()) {
        cout<< "file already exists\n";
        return NULL;
    } else {
        file* new_file=new file("some data",sz,name);
        int rq_size=ceil(sz*1.0/block_size);
        //cout<<rq_size;
        if(rem<rq_size){
            cout<<"Insufficient size"<<endl;
            return NULL;
        }
        int c1=0;
        int fg=0;
        while(c1!=rq_size){
            int prev;
            int curr;
            //int fg=0;
            for(int i=2;i<blks.size();i++){
                if(c1==rq_size)break;
                //cout<<c1<<endl;
                //cout<<i<<endl;
                if(blks[i]->occupied==0){
                    //cout<<i<<endl;
                    //cout<<blks[0]->emp_strings<<endl;
                    if(fg==0){
                        fg=1;
                        c1++;
                        d->fat[name]=i;
                        curr=i;
                        blks[i]->occupied=1;
                        blks[0]->emp_strings[i] = '1';
                        rem--;
                    }
                    else{
                             //fg=1;
                        c1++;
                        //d->fat[name]=i;
                        
                        curr=i;
                        blks[prev]->next=blks[curr];
                        blks[i]->occupied=1;
                        blks[0]->emp_strings[i] = '1';
                        rem--;
                        
                    }
                    prev=curr;
                    

                }
            } 
            
        }
        cout<<"FILE CREATED"<<endl;
        d->aof.push_back(new_file);
        
        
    }
    return d;
    
}
file* add_file2(directory* d,string name,string content="some data",int flagg = 2,int size=4){
    if(d== NULL) {
        cout <<"directory does not exist\n";
    }
    //string name;
    //cout<<"Enter File name: \n";
    //cin>>name;
    int sz;
    if (flagg ==2){
        cout<<"ENTER File size : \n";
        cin>>sz;
    }
    else {
        sz = size;
    }
    
   
    if (d->fat.find(name) != d->fat.end()) {
        cout<< "file already exists\n";
        return NULL;
    } else {
        file* new_file=new file(content,sz,name);
        int rq_size=ceil(sz*1.0/block_size);
        //cout<<rq_size;
        if(rem<rq_size){
            cout<<"Insufficient size"<<endl;
            return NULL;
        }
        int c1=0;
        int fg=0;
        while(c1!=rq_size){
            int prev;
            int curr;
            //int fg=0;
            for(int i=2;i<blks.size();i++){
                if(c1==rq_size)break;
                //cout<<c1<<endl;
                //cout<<i<<endl;
                if(blks[i]->occupied==0){
                    //cout<<i<<endl;
                    //cout<<blks[0]->emp_strings<<endl;
                    if(fg==0){
                        fg=1;
                        c1++;
                        d->fat[name]=i;
                        curr=i;
                        blks[i]->occupied=1;
                        blks[0]->emp_strings[i] = '1';
                        rem--;
                    }
                    else{
                             //fg=1;
                        c1++;
                        //d->fat[name]=i;
                        
                        curr=i;
                        blks[prev]->next=blks[curr];
                        blks[i]->occupied=1;
                        blks[0]->emp_strings[i] = '1';
                        rem--;
                        
                    }
                    prev=curr;
                    

                }
            } 
            
        }
        cout<<"FILE CREATED"<<endl;
        new_file->open=1;
        
        
        d->aof.push_back(new_file);
        return new_file;
        
        
        
    }
    return NULL;
    //return d;
    
}


void showfiles(directory * dir) {
    if(dir == NULL) {
        cout << "directory does not exists \n";
        return ;
    }
    cout << "showing files here \n";
    for (auto x:dir->fat) {
        cout << x.first << " " << x.second << "\n";
    }
}

void rm_dir(string dirname){
    int imd = -1;
    for (int i = 0;i<dirs.size();i++){
        if(dirs[i]->name == dirname) {
            imd = i;
            break;
        }
    }
    if(imd!=-1){
      for(int i=0;i<dirs[imd]->aof.size();i++){
        auto p=dirs[imd]->aof[i];
        int sz=dirs[imd]->aof[i]->sz;
        int rq_size=ceil(sz*1.0/block_size);
        rem+=rq_size;
        int k=dirs[imd]->fat[p->file_name];
        block* temp=blks[k];
        while(temp!=NULL){
            temp->occupied=0;
            temp=temp->next;
        }
    }
        dirs.erase(dirs.begin()+imd);
        return;
    }
    else cout << "directory does not exits\n";
}

file* open(directory * d){
    string filename;
    cout << "enter file name to be opened: ";
    cin >> filename;
    if(d->fat.count(filename) <= 0) {
        return add_file2(d,filename);
        
    }
    else {
        cout << "file opened successfully\n";
        for (int i = 0;i<d->aof.size();i++){
            if(d->aof[i]->file_name == filename) {
                d->aof[i]->open = 1;
                return d->aof[i];
            }
        }
    }
    return NULL;
}


file* close(directory * d,file* rw) {
    if(rw==NULL){
        cout<<"No file opened yet"<<endl;
        return rw;
    }
    string filename=rw->file_name;
    if (d->fat.count(filename) <= 0) {
        cout << "file does not exists\n";
        rw=NULL;
        return rw;
    }
    for (int i = 0;i<d->aof.size();i++){
        if(d->aof[i]->file_name == filename) {
            if (d->aof[i]->open == 1){
                d->aof[i]->open = 0;
                cout << "file closed successfully\n";
                break;
            } else {
                cout << "cant close file which is not opened yet\n";
            }
        }
    }
    rw=NULL;
    return rw;
}

void read(file * rw) {
    if(rw==NULL){
        cout<<"file not opened"<<endl;
        return;
    }
    cout << rw->data<< endl;
}

void write(file *rw) {
    if(rw==NULL){
        cout<<"file not opened"<<endl;
        return ;
    }
    string content="";
    cout << "enter what do you wnat to write: \n";
    cin.ignore();
    getline(cin,content);

    rw->data = rw->data +"\n"+ content;
}

directory * chdir() {
    string changeddir;
    cout << "Enter changed directory: ";
    // cin.ignore();
    cin >> changeddir;
    // getline(cin,changeddir);

    int imd = -1;
    for (int i = 0;i<dirs.size();i++){
        if(dirs[i]->name == changeddir) {
            imd = i;
            break;
        }
    }
    if (imd == -1){
        cout << "No suce directory exists\n";
        return NULL;
    }
    return dirs[imd];
}

int chdir1(string changeddir) {
    // string changeddir;
    // cout << "Enter changed directory: ";
    // cin.ignore();
    // getline(cin,changeddir);

    int imd = -1;
    for (int i = 0;i<dirs.size();i++){
        if(dirs[i]->name == changeddir) {
            imd = i;
            break;
        }
    }
    if (imd == -1){
        cout << "No suce directory exists\n";
        return -1;
    }
    return imd;
}
void copy() {
    string directory1,directory2;
    cout << "enter the source dir: ";
    //cin.ignore();
    // getline(cin,directory1);
    cin >> directory1;
    cout << "enter the dest directory: ";
    // getline(cin,directory2);
    cin >>directory2;

    string file1,file2;
    cout << "enter source file: ";
    // getline(cin,file1);
    // cin>> file1;
    // cout << "enter dest file: ";
    // getline(cin,file2);
    cin >> file1;

    int dir1= chdir1(directory1);
    int dir2 = chdir1(directory2);

    if (dir1 == -1) {
        cout << "dir1 is null\n";
        return;
    } if (dir2 == -1) {
        cout << "dir2 is null\n";
        return;
    }
    int imd1=-1,imd2=-1;
    for (int i = 0;i<dirs[dir1]->aof.size();i++){
        if(dirs[dir1]->aof[i]->file_name==file1){
            imd1 = i;
            break;
        }
    }

    // for (int i = 0;i<dirs[dir2]->aof.size();i++){
    //     if(dirs[dir2]->aof[i]->file_name==file2){
    //         imd2 = i;
    //         break;
    //     }
    // }

    // if(imd1 ==-1) {
    //     cout << "source file doesnot exist\n";
    //     return;
    // } 

    add_file2(dirs[dir2],file1,dirs[dir1]->aof[imd1]->data,3,dirs[dir1]->aof[imd1]->sz);
    // dirs[dir2]->aof[imd2]->data = dirs[dir1]->aof[imd1]->data;
}



int main(int argc, char * argv[]){
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    directory* cur_dir=NULL;
    cout<<"ENTER BLOCK SIZE: \n";
    cin >> block_size;
    cout << "ENTER MAX FILE SIZE: \n";
    cin>>max_file_size;
    file* rw_head=NULL,*rwh=NULL;
    blks.resize(N);
    // dirs.reserve(0);
    for (int i = 0;i<N;i++){
        blks[i] = new block();
        if (i == 0){
            blks[i]->issuper = true;
        }
    }
    directory * dir = mk_dir();
    directory * dir1 = mk_dir();
    cur_dir=dirs[0];
    add_file(cur_dir);
    add_file(cur_dir);
    add_file(cur_dir);
    add_file(cur_dir);
    add_file(cur_dir);
    // //add_file(dirs.back());
    // rm_dir(dirs.back());
    showfiles(cur_dir);
    rw_head=open(cur_dir);
    write(rw_head);
    // write(rw_head);
    read(rw_head);
    rw_head = close(cur_dir,rw_head);
    read(rw_head);
    cur_dir = chdir();
    add_file(cur_dir);
    add_file(cur_dir);
    add_file(cur_dir);

    showfiles(cur_dir);
    // cur_dir = change_dir

    copy();
    showfiles(cur_dir);
    // rwh=open(cur_dir);
    // // write(rw_head);
    // // write(rw_head);
    // read(rwh);
    // rwh = close(cur_dir,rwh);
    return 0;
}