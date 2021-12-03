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