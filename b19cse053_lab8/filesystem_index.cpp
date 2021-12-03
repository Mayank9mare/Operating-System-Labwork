#include <bits/stdc++.h>

using namespace std;
#define N 5000
int block_size;
int max_file_size;
int rem =N;

struct single_indirect_pointer{
    vector<vector<int>> md;
    single_indirect_pointer(){
        md.resize(32);
        for (int i = 0;i<32;i++){
            md[i].resize(16,-1);
        }
    }
};

struct double_indirect_pointer{
    vector<vector<vector<int>>> md;
    double_indirect_pointer(){
        md.resize(32);
        for (int i = 0;i<32;i++){
            md[i].resize(32);
            for (int j=0;j<32;j++){
                md[i][j].resize(16,-1);
            }
        }
    }
};


struct block{
    bool issuper;
    int occupied;
    string filedata;
    int block_number;
    block(){
        issuper = false;
        occupied = 0;
    }    
};

vector<struct block *> blks;

struct inode {
    string file_name;
    string file_size;
    string type;
    vector<int> directpointers;
    single_indirect_pointer * sip;
    double_indirect_pointer * dip;
    inode(){
        directpointers.resize(5);
        sip = new single_indirect_pointer();
        dip = new double_indirect_pointer();
    }
};

struct file{
    int sz=0;
    string file_name;
    string data;
    int open;
    struct inode *inod;
    file(string daata,int size,string filename){
        data = daata;
        sz = size;
        file_name = filename;
        open =0;
        inod = new inode();
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
        rem-=rq_size;
        int c=0;
        vector<int> nob;
        for (int i = 1;i<N && c<rq_size;i++){
            if (blks[i]->occupied == 0){
                nob.push_back(i);
                blks[i]->occupied = 1;
                c++;
            }
        }
        d->fat[name] = nob[0];
        if (nob.size() <= 5){
            for (int i = 0;i<nob.size();i++)
                new_file->inod->directpointers[i] = nob[i];
        } else {
            for (int i = 0;i<5;i++)
                new_file->inod->directpointers[i] = nob[i];
            if (nob.size()-5 <= 32*16) {
                int t = 5;
                for (int i = 0;i<32;i++) {
                    int flag = 0;
                    for (int j = 0;j<16;j++) {
                        new_file->inod->sip->md[i][j] = nob[t];
                        t++;
                        if (t == nob.size()) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag) {
                        break;
                    }
                }
            } else {
                int t = 5;
                for (int i = 0;i<32;i++) {
                    for (int j = 0;j<16;j++) {
                        new_file->inod->sip->md[i][j] = nob[t];
                        t++;  
                    }
                }
                for (int i = 0;i<32;i++){
                    int flag = 0;
                    for (int j = 0;j<32;j++){
                        for(int k = 0;k<16;k++) {
                            new_file->inod->dip->md[i][j][k] = nob[t];
                            t++;
                            if(t == nob.size()) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag) {
                            break;
                        }
                    }
                    if(flag) {
                        break;
                    }
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
        rem-=rq_size;
        int c=0;
        vector<int> nob;
        for (int i = 1;i<5000 && c<rq_size;i++){
            if (blks[i]->occupied == 0){
                nob.push_back(i);
                blks[i]->occupied = 1;
                c++;
            }
        }
        d->fat[name] = nob[0];
        if (nob.size() <= 5){
            for (int i = 0;i<nob.size();i++)
                new_file->inod->directpointers[i] = nob[i];
        } else {
            for (int i = 0;i<5;i++)
                new_file->inod->directpointers[i] = nob[i];
            if (nob.size()-5 <= 32*16) {
                int t = 5;
                for (int i = 0;i<32;i++) {
                    int flag = 0;
                    for (int j = 0;j<16;j++) {
                        new_file->inod->sip->md[i][j] = nob[t];
                        t++;
                        if (t == nob.size()) {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag) {
                        break;
                    }
                }
            } else {
                int t = 5;
                for (int i = 0;i<32;i++) {
                    for (int j = 0;j<16;j++) {
                        new_file->inod->sip->md[i][j] = nob[t];
                        t++;  
                    }
                }
                for (int i = 0;i<32;i++){
                    int flag = 0;
                    for (int j = 0;j<32;j++){
                        for(int k = 0;k<16;k++) {
                            new_file->inod->dip->md[i][j][k] = nob[t];
                            t++;
                            if(t == nob.size()) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag) {
                            break;
                        }
                    }
                    if(flag) {
                        break;
                    }
                }
            }
        }
        cout<<"FILE CREATED"<<endl;
        if(flagg==2) new_file->open=1;
        d->aof.push_back(new_file);
        return new_file;
    }
    return NULL;
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
        for (int i = 0;i<dirs[imd]->aof.size();i++) {
            int rq_size=ceil(dirs[imd]->aof[i]->sz*1.0/block_size);
            rem += rq_size;
            vector<int> a = dirs[imd]->aof[i]->inod->directpointers;
            vector<vector<int>> b = dirs[imd]->aof[i]->inod->sip->md;
            vector<vector<vector<int>>> c= dirs[imd]->aof[i]->inod->dip->md;
            int a0 = 0,a1=0,a2=0;
            for (int i = 0;i<a.size();i++) {
                if (a[i] == -1) {
                    a0 =  1;
                    break;
                }
                blks[a[i]]->occupied=0;
            }
            if(!a0) {
                for (int i = 0;i<32;i++) {
                    for (int j =0;j<16;j++) {
                        if(b[i][j] == -1) {
                            a1=1;
                            break;
                        }
                        if(!a1)
                            blks[b[i][j]]->occupied = 0;
                    }
                    if(a1) {
                        break;
                    }
                }
            }
            if (!a1 && !a0) {
                for (int i = 0;i<32;i++) {
                    for (int j = 0;j<32;j++) {
                        for (int k = 0;k<16;k++) {
                            if (c[i][j][k] == -1) {
                                a2 = 1;
                                break;
                            }
                            blks[c[i][j][k]]->occupied = 0;
                        }
                        if(a2) {
                            break;
                        }
                    }
                    if(a2){
                        break;
                    }

                }
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

    rw->data = rw->data +" "+ content;
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

    add_file2(dirs[dir2],file1,dirs[dir1]->aof[imd1]->data,3,dirs[dir1]->aof[imd1]->sz);

}


int main(int argc, char * argv[]){
    // freopen("input1.txt", "r", stdin);
    // freopen("output1.txt", "w", stdout);
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
    // read(rw_head);
    cur_dir = chdir();
    add_file(cur_dir);
    add_file(cur_dir);
    add_file(cur_dir);

    showfiles(cur_dir);
    // // cur_dir = change_dir

    copy();
    showfiles(cur_dir);
    cout << rem << "\n";
    rm_dir("diro1");
    cout << rem << "\n";
    for (int i = 0;i<dirs.size();i++) {
        cout << dirs[i]->name << " ";
    }
    // blks.resize(N);
    return 0;
}