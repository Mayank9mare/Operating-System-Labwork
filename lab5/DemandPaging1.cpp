#include <bits/stdc++.h>

using namespace std;

map<int,int> tlb;
vector<int> frameList;

ofstream myfile("results.txt");

class pte{
    public:
    int frame,valid;
    pte(){
        frame = -1;
        valid = 0;
    }
};


class process{
    public:
    int tlbsz,frmsz,id,pageflt;
    vector<pte*> pt;
    vector<int> rs;
    queue<int> q;
    process(int idd,int s,int f,int m){
        pt.resize(m);
        for (int i =0;i<m;i++) {
            pt[i] = new pte();
        }
        tlbsz = s;
        frmsz = f;
        id = idd;
        pageflt = 0;
    }
    void search(int pg){
        rs.push_back(pg);
        if (tlb.find(pg) != tlb.end()) {
            // TLB HIT
            string op = "process: "+ to_string(id) + ": for page reference " + to_string(pg) + " TLB hit with frame No. " + to_string(tlb[pg]); 
            cout << op << endl;
            myfile << op;
            myfile << "\n";
            return;
        } else {
            q.push(pg);
            if (pt[pg]->valid == 1) {
                // page found in page table
                string op="process: "+ to_string(id) + ": for page reference " + to_string(pg) + " TLB miss -> Page table valid -> with frame No. " + to_string(pt[pg]->frame);
                cout << op << endl;
                myfile << op;
                myfile << "\n";
                // update tlb;
                tlb[pg] = pt[pg]->frame;
                if (tlb.size() > tlbsz) {
                    int a = q.front();
                    q.pop();
                    tlb.erase(a);
                }
            } else {
                pageflt++; 
                for (int i =0;i<frmsz;i++) {
                    if (frameList[i] == -1) {
                        frameList[i] = pg;
                        pt[pg]->frame = i;
                        pt[pg]->valid = 1;
                        tlb[pg] = i;
                        if (tlb.size() > tlbsz) {
                            int a = q.front();
                            q.pop();
                            tlb.erase(a);
                        }
                        string op = "process :" + to_string(id) + " for page reference " + to_string(pg) + " TLB miss -> page Invalid -> Load from disk -> stored at frame " + to_string(i);
                        cout << op<<endl;
                        myfile << op;
                        myfile << "\n";
                        return;
                    }
                }

                int imd = -1;
                int max_dis = INT_MIN;
                for (int i =0;i<frmsz;i++) {
                    int dis = 0;
                    bool b = false;
                    for (int m = rs.size()-2;m>=-1;m--) {
                        int val = rs[m];
                        dis ++;
                        if (val == frameList[i]) {
                            b =true;
                            break;
                        }
                    }
                    if (!b) {
                        dis = INT_MAX;
                    }
                    if (dis > max_dis) {
                        max_dis = dis;
                        imd = i;
                    }
                }
                frameList[imd] = pg;
                pt[pg]->frame = imd;
                pt[pg]->valid = 1;
                tlb[pg] = imd;
                if (tlb.size() > tlbsz) {
                    int a = q.front();
                    q.pop();
                    tlb.erase(a);
                }
                string op =  "process :" + to_string(id) + " for page reference " + to_string(pg) + " TLB miss -> page Invalid -> Load from disk -> frames not available -> apply LRU -> stored at frame " + to_string(imd);
                cout <<op <<endl;
                myfile <<op;
                myfile << "\n";
                return;

            }
        }


        

    }
    
};



int main(int argc, char * argv[]) {
    int k;// total np. of process
    cout << "enter number of processes: ";
    cin>>k;
    int m;//VAS
    cout << "maximum number of pages per process: ";
    cin>>m;
    int f;
    cout << "enter number of frames in main memory: ";
    cin >> f;//PAS
    int s; //size of tlb
    cout << "enter the maximum size of the TLB: ";
    cin >> s;
    frameList.resize(f);
    tlb.clear();
    srand(time(NULL));
    for (int i = 0;i<k;i++) {
        frameList.clear();
        frameList.resize(f,-1);
        tlb.clear();
        int id=i;
        int prom = rand() % m + m/2;
        process * pro = new process(id,s,f,prom);
        int range = rand() % 8*prom + 2*prom;
        vector<int> rs;
        for (int j = 0;j<range;j++) {
            rs.push_back(rand()%prom);
        }
        for (int j = 0;j<rs.size();j++) {
            myfile << to_string(rs[j]) + " ";
            cout << rs[j] << " ";
        }
        myfile << "\n";
        cout << endl;
        for (int j = 0;j<rs.size();j++) {
            pro->search(rs[j]);
        }
        string op =  "process: "+ to_string(id) + " Total page faults are : "+ to_string(pro->pageflt);
        cout << op << endl;
        myfile<<op+"\n";
        
    }
    myfile.close();
    // TLB * t = new TLB(s);


    return 0;
}