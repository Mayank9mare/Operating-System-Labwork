#include <bits/stdc++.h>

using namespace std;

int fifo(int f,vector<int> rs) {
    queue<int> q;
    vector<vector<int>> v(f,vector<int> (rs.size(),-1));
    int miss =0;
    int cycle = 0;
    for (int i =0;i<rs.size();i++) {
        int page = rs[i];
        bool find=false;
        bool emp = false;
        if (i != 0) {
            for (int j =0;j<f;j++) {
                v[j][i] = v[j][i-1];
            }
        }
        for (int j =0;j<f;j++) {
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =true;
                break;
            } else if (v[j][i] == -1){
                emp = true;
            }
        }
        if (!find) {
            q.push(page);
            cout << "0 ";
            miss++;
            if (emp) {
                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
            } else {
                int fif = q.front();
                q.pop();
                for (int ii =0;ii<f;ii++) {
                    if (v[ii][i] == fif) {
                        v[ii][i] = page; 
                        break;
                    }
                }
            }
            //replace page
        }
        else {
            cout << "1 ";
        } 
    }
    return miss;

}

int lru(int f,vector<int> rs){
   vector<vector<int>> v(f,vector<int> (rs.size(),-1));
    int miss =0;
    int cycle = 0;
    for (int i =0;i<rs.size();i++) {
        int page = rs.at(i);
        bool find=false;
        bool emp = false;
        if (i != 0) {
            for (int j =0;j<f;j++) {
                v[j][i] = v[j][i-1];
            }
        }
        for (int j =0;j<f;j++) {
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =true;
                break;
            } else if (v[j][i] == -1){
                emp = true;
            }
        }
        if (!find) {
            cout << "0 ";
            miss++;
            if (emp) {
                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
            } else {
                int imd = -1;
                int maxdis = INT_MIN;
                for (int m = 0;m<f;m++) {
                    int dis = 0;
                    bool b = false;
                    for (int p = i-1;p>-1;p--) {
                        int val = rs[p];
                        dis++;
                        if (val == v[m][i]) {
                            b=true;
                            break;
                        }
                    }
                    if (!b) {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis) {
                        maxdis = dis;
                        imd = m;
                    }
                }
                v[imd][i] = page;
            }
            //replace page
        }
        else {
            cout << "1 ";
        } 
    }
    return miss;
}

int opr(int f,vector<int> rs){
    vector<vector<int>> v(f,vector<int> (rs.size(),-1));
    int miss =0;
    int cycle = 0;
    for (int i =0;i<rs.size();i++) {
        int page = rs.at(i);
        bool find=false;
        bool emp = false;
        if (i != 0) {
            for (int j =0;j<f;j++) {
                v[j][i] = v[j][i-1];
            }
        }
        for (int j =0;j<f;j++) {
            if (v[j][i] !=-1 && v[j][i] == page) {
                find =true;
                break;
            } else if (v[j][i] == -1){
                emp = true;
            }
        }
        if (!find) {
            cout << "0 ";
            miss++;
            if (emp) {
                v[cycle][i] = page;
                cycle = (cycle + 1)%f;
            } else {
                int imd =-1;
                int maxdis = INT_MIN;
                for (int m = 0;m<f;m++) {
                    int dis = 0;
                    bool b = false;
                    for (int p = i+1;p<rs.size();p++) {
                        int val = rs[p];
                        dis++;
                        if (val == v[m][i]) {
                            b=true;
                            break;
                        }
                    }
                    if (!b) {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis) {
                        maxdis = dis;
                        imd = m;
                    }
                }
                v[imd][i] = page;                
            }
            //replace page
        }
        else {
            cout << "1 ";
        } 
    }
    return miss;
}

int main(int argc ,char * argv[]) {
    int n;
    cout << "enter number of times to run the algoriths : ";
    cin >> n;
    cout << "the algorithms will run for : " << n <<" times let's begin " << endl;
    cout << "enter the reference string as single string of numbers where each number represents page number that cpu wants" << endl; 
    while (n--) {
        int f;
        cout << "enter number of frames : ";
        cin >> f;
        string rs;
        cout << "enter reference string : ";
        cin.ignore();
        getline(cin,rs);
        // cout << rs << endl;
        vector<int> v;
        int i = 0;
        string p="";
        while (i < rs.length()) {
             if(isdigit(rs[i])){
                p+=rs[i];
                i++;
                continue;
            }
            else{
                v.push_back(stoi(p));
                p="";
                i++;
            }
        }
        if(p.size()){
            v.push_back(stoi(p));

        }
        // for (int i =0;i<v.size();i++) {
        //     cout << v[i] << endl;
        // }
        int a = fifo(f,v);
        cout << "\n (fifo) miss are : " << a <<  endl;
        int b = opr(f,v);
        cout << "\n (opr) miss are : " << b << endl;
        int c = lru(f,v);
        cout << "\n (lru) miss are : " << c << endl;
    }
    return 0;
}