#include <bits/stdc++.h>
#define pb push_back
#define mk make_pair
#define ll long long
#define ss second
#define ff first
#define pll pair<ll,ll>
#define vll vector<ll>
#define mll map<ll,ll>
#define all(cont) cont.begin(), cont.end()

using namespace std;
int cmp(pair<int,pll> a,pair<int,pll> b){
    if(a.first<b.first)return 1;
    if(a.first==b.first)return a.second.first<b.second.first;
    return 0;
}
int fifo(int f, vector<int> rs)
{
    queue<int> q;//queue for pages
    vector<vector<int>> v(f, vector<int>(rs.size(), -1));
    int m = 0;
    int cycle = 0;
    for (int i = 0; i < rs.size(); i++)
    {
        int page = rs[i];
        int find = 0;
        int emp = 0;
        if (i != 0)
        {
            for (int j = 0; j < f; j++)
            {
                v[j][i] = v[j][i - 1];
            }
        }
        for (int j = 0; j < f; j++)
        {
            if (v[j][i] != -1 && v[j][i] == page)
            {
                find = 1;
                break;
            }
            else if (v[j][i] == -1)
            {
                emp = 1;
            }
        }
        if (!find)
        {
            q.push(page);
            cout << "0 ";
            m++;
            if (emp)
            {
                v[cycle][i] = page;
                cycle = (cycle + 1) % f;
            }
            else
            {
                int fif = q.front();
                cout<<"victim ("<<fif<<") ";
                q.pop();
                for (int ii = 0; ii < f; ii++)
                {
                    if (v[ii][i] == fif)
                    {
                        v[ii][i] = page;
                        break;
                    }
                }
            }
        }
        else
        {
            cout << "1 ";
        }
    }
    return m;
}

int lru(int f, vector<int> rs)
{
    vector<vector<int>> v(f, vector<int>(rs.size(), -1));
    int m = 0;
    int cycle = 0;
    for (int i = 0; i < rs.size(); i++)
    {
        int page = rs.at(i);
        int find = 0;
        int emp = 0;
        if (i != 0)
        {
            for (int j = 0; j < f; j++)
            {
                v[j][i] = v[j][i - 1];
            }
        }
        for (int j = 0; j < f; j++)
        {
            if (v[j][i] != -1 && v[j][i] == page)
            {
                find = 1;
                break;
            }
            else if (v[j][i] == -1)
            {
                emp = 1;//page not present yet
            }
        }
        if (!find)
        {
            cout << "0 ";
            m++;
            if (emp)
            {
                v[cycle][i] = page;
                cycle = (cycle + 1) % f;
            }
            else
            {
                int id = -1;
                int maxdis = INT_MIN;
                for (int m = 0; m < f; m++)
                {
                    int dis = 0;
                    bool b = false;
                    for (int p = i - 1; p > -1; p--)
                    {
                        int val = rs[p];
                        dis++;
                        if (val == v[m][i])
                        {
                            b = true;
                            break;
                        }
                    }
                    if (!b)
                    {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis)
                    {
                        maxdis = dis;
                        id = m;
                    }
                }
                cout<<"victim ("<<v[id][i]<<") ";
                v[id][i] = page;
            }
            //replace page
        }
        else
        {
            cout << "1 ";
        }
    }
    return m;
}

int opr(int f, vector<int> rs)
{
    vector<vector<int>> v(f, vector<int>(rs.size(), -1));
    int m = 0;
    int cycle = 0;
    for (int i = 0; i < rs.size(); i++)
    {
        
        
        int page = rs.at(i);
        int find = 0;//already present
        int emp = 0;
        if (i != 0)
        {
            for (int j = 0; j < f; j++)
            {
                v[j][i] = v[j][i - 1];
            }
        }
        for (int j = 0; j < f; j++)
        {
            if (v[j][i] != -1 && v[j][i] == page)
            {
                find = 1;break;
            }
            else if (v[j][i] == -1)
            {
                emp =1;
            }
        }
        if (!find)
        {
            cout << "0 ";
        m++;
            if (emp)
            {
                v[cycle][i] = page;
                cycle = (cycle + 1) % f;
            }
            else
            {
                int id = -1;
                int maxdis = INT_MIN;
                for (int m = 0; m < f; m++)
                {
                    int dis = 0;
                    bool b = false;
                    for (int p = i + 1; p < rs.size(); p++)
                    {
                        int val = rs[p];
                        dis++;
                        if (val == v[m][i])
                        {
                            b = true;
                            break;
                        }
                    }
                    if (!b)
                    {
                        dis = INT_MAX;
                    }
                    if (dis > maxdis)
                    {
                        maxdis = dis;
                        id = m;
                    }
                }
                cout<<"victim ("<<v[id][i]<<") ";
                v[id][i] = page;
            }
            //replace page
        }
        else
        {
            
            cout << "1 ";
        }
    }
    return m;
}

int main(int argc, char *argv[])
{

    int f;
    cout << "enter number of frames : ";
    cin >> f;
    string rs;
    cout << "enter reference string : ";
    cin.ignore();
    getline(cin, rs);
    // cout << rs << endl;
    vector<int> v;
    int i = 0;
    string p = "";
    cout<<"0 means page fault and 1 means page hit"<<endl;
    while (i < rs.length())
    {
        if (isdigit(rs[i]))
        {
            p += rs[i];
            i++;
            continue;
        }
        else
        {
            v.pb(stoi(p));
            p = "";
            i++;
        }
    }
    if (p.size())
    {
        v.pb(stoi(p));
    }

    int a = fifo(f, v);
    cout << "\n FIFO page faults are : " << a << endl;
    int b = opr(f, v);
    cout << "\n Optimal page replacement algo page faults are are : " << b << endl;
    int c = lru(f, v);
    cout << "\n LRU page faults are miss are : " << c << endl;

    return 0;
}