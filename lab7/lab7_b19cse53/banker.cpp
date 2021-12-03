#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> cur_need,max_need,allocated,cnd,mnd,ald;
vector<int> available,ad;

void getmax_need(int p,int r){
    for (int i =0;i<p;i++) {
        for (int j = 0;j<r;j++) {
            cin >> max_need[i][j];
            mnd[i][j] = max_need[i][j];
        }
    }
}


void getallocated(int p,int r){
    for (int i =0;i<p;i++) {
        for (int j = 0;j<r;j++) {
            cin >> allocated[i][j];
            ald[i][j] = allocated[i][j];
        }
    }

}

void getavailable(int r) {
    for (int i =0;i<r;i++) {
        cin >> available[i];
        ad[i] = available[i];
    }
}

void getneedmatrix(int p,int r) {
    for (int i = 0;i<p;i++) {
        for (int j = 0;j<r;j++){
            cur_need[i][j] = max_need[i][j]-allocated[i][j];
            cnd[i][j] = cur_need[i][j];
        }
    }
}

bool checkvalid(int m,int r) {
    for (int i=0;i<r;i++) {
        if (cur_need[m][i] > available[i]){
            return false;
        } 
    }
    return true;
}

bool checkvalidreq(int m,int r,vector<int> vm) {
    for (int i=0;i<r;i++) {
        if (cur_need[m][i] < vm[i]){
            return false;
        } 
    }
    return true;
}

bool validforavailable(int m,int r,vector<int> vm) {
    for (int i =0;i<r;i++) {
        if(available[i] < vm[i]){
            return false;
        }
    }
    return true;
}


void update_available(int m ,int r){
    for (int i =0;i<r;i++) {
        available[i] += allocated[m][i];
    }
}

void subavai(int r,vector<int> vm) {
    for (int i =0;i<r;i++) {
        available[i] -= vm[i];
    }
}

void subneed(int m,int r,vector<int> vm) {
    for (int i =0;i<r;i++) {
        cur_need[m][i] -= vm[i];
    }
}

void addallocated(int m,int r,vector<int> vm) {
    for (int i =0;i<r;i++) {
        allocated[m][i] += vm[i];
    }
}

void resetavai() {
    available = ad;
}

void resetneed(int p,int r){
    for (int i = 0;i<p;i++) {
        for (int j = 0;j<r;j++){
            cur_need[i][j] = cnd[i][j];
        }
    }
}
void resetallocated(int p ,int r) {
    for (int i =0;i<p;i++) {
        for (int j = 0;j<r;j++) {
            allocated[i][j]=ald[i][j];
        }
    }
}

int applybanker(int p,int r){
    vector<int> v;
    unordered_set<int> us;
    bool flag=false;
    while(us.size()< p) {
        int myflag = 0;
        for (int i =0;i<p;i++) {
            if (checkvalid(i,r) && (us.find(i) == us.end())) {
                // cout << "value " << i << " not found in us" << endl; 
                us.insert(i);
                myflag = 1;
                v.push_back(i);
                update_available(i,r);
                // // cout << "new available \n";
                // for (int i =0;i<r;i++) {
                //     cout << available[i] << " ";
                // }
                // cout << endl;
            }
        }
        if (!myflag) {
            flag = true;
            break;
        }
    }

    resetavai();
    if (flag) {
        // cout << "system is unsafe\n";
        return 0;
    }

    cout << "system's safe sequence is:\n";
    for (int i =0;i<v.size();i++) {
        cout<<"P"<<v[i] << " ";
    }
    cout << endl;
    return 1;

}

void printmat(vector<vector<int>> ver,int p,int r) {
    for (int i =0;i<p;i++) {
        for (int j = 0;j<r;j++) {
            cout << ver[i][j] << " ";
        }
        cout << endl;
    }
}

void takerequests(int p,int r){
    int nreq;
    vector<int> req;
    cout << "how many requests you want to enter: ";
    cin >> nreq;
    req.resize(r);
    while (nreq--) {
        int pro;
        cin >> pro;
        for (int i =0;i<r;i++) {
            cin >> req[i];
        }
        if (checkvalidreq(pro,r,req)) {
            if (validforavailable(pro,r,req)){
                
                addallocated(pro,r,req);
                subneed(pro,r,req);
                subavai(r,req);

                int runbanker = applybanker(p,r);
                if (!runbanker) {
                    cout << "system no longer is safe\n";
                }

                resetavai();
                resetallocated(p,r);
                resetneed(p,r);

                // printmat(allocated,p,r);
                // printmat(cur_need,p,r);
                // for (int i = 0;i<r;i++) {
                //     cout << available[i] <<  " "; 
                // }
                // cout << endl;

            }else {
                cout << "not enough resources available\n";
            }
        } else {
            cout << "request is greater than the need\n";
        }

    }
}

int main(int argc, char * argv[]) {
    int r,p;
    cout << "enter number of processes: ";
    cin >> p;
    cout << "enter number of resources: ";
    cin >> r;

    cur_need.resize(p);
    cnd.resize(p);
    max_need.resize(p);
    mnd.resize(p);
    allocated.resize(p);
    ald.resize(p);
    for (int i =0;i<p;i++) {
        cur_need[i].resize(r);
        max_need[i].resize(r);
        allocated[i].resize(r);
        cnd[i].resize(r);
        mnd[i].resize(r);
        ald[i].resize(r); 
    }

    available.resize(r);
    ad.resize(r);

    cout << "enter the current allocated matrix:\n";
    getallocated(p,r);
    cout << "enter the maximum allocation required per process:\n";
    getmax_need(p,r);
    cout << "enter available resources\n";
    getavailable(r);

    getneedmatrix(p,r);
    // for (int i =0;i<p;i++) {
    //     for (int j = 0;j<r;j++) {
    //         cout << cur_need[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    
    int d = applybanker(p,r);
    if (d) {
        takerequests(p,r);
    }
    else {
        cout << "system is unsafe\n";
    }

        
    return 0;
}



/*
1 2 3
2 0 3
3 1 1
0 2 0

3 3 6
5 4 6
4 3 3
3 5 4
*/