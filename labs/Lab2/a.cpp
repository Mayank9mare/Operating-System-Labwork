#include <windows.h>
#include <bits/stdc++.h>
#include <mysql.h>

using namespace std;
int qstate;
MYSQL *conn;
MYSQL_ROW row;
MYSQL_RES *res;
class dbsystem
{
public:
    static void connectionfunction()
    {
        conn = mysql_init(0);
        if (conn)
        {
            cout << "database connected" << endl;
        }
        else
        {
            cout << "failed to connect " << endl;
        }

        //192.168.1.9
        conn = mysql_real_connect(conn, "localhost", "admin", "admin", "employees", 0, NULL, 0);
        if (conn)
        {
            cout << "connection successful " << endl;
        }
        else
        {
            cout << "connection failed" << endl;
        }
    }
};

void RegisterEmployee();
void displayEmployee();

int main(int argc, char *argv[])
{

    //dbsystem::connectionfunction();

    dbsystem sys;
    sys.connectionfunction();

    RegisterEmployee();
    displayEmployee();
    return 0;
}
void RegisterEmployee()
{
    string emp_id, emp_name, dept, salary;

    cout << "enter employee name : ";
    getline(cin, emp_name);

    cout << "enter employee id : ";
    cin >> emp_id;

    cout << "enter dept : ";
    cin >> dept;

    cout << "enter salary : ";
    cin >> salary;

    string insert_query = "INSERT INTO employee_info (Id, Name, Dept, Salary) VALUES ('" + emp_id + "', '" + emp_name + "', '" + dept + "', '" + salary + "')";

    //cout << insert_query << endl;
    const char *q = insert_query.c_str();
    //cout << q << endl;
    qstate = mysql_query(conn, q);
    //cout << qstate << endl;
    if (!qstate)
    {
        cout << "employee successfully added " << endl;
    }
    else
    {
        cout << "failed" << endl;
    }

    //cout << insert_query << endl;
}

void displayEmployee()
{
    char choose;

    qstate = mysql_query(conn, "SELECT * FROM employee_info");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        printf("--------------------------------------------------------\n");
        printf("|                  employee information                 |\n");
        printf("--------------------------------------------------------\n");
        //printf("| %-5s | %-10s | %-25s  | %-30s  |\n","emp_id","emp_name","dept","salary");
        while ((row = mysql_fetch_row(res)))
        {
            printf("| %-5s | %-15s | %-5s | %-5s    |\n", row[0], row[1], row[2], row[3]);
            //cout << "emp_id: " << row[0] << " emp_name: " << row[1] << " dept: " << row[2] << " salary: " << row[3] << endl;
        }
        printf("|-------------------------------------------------------|\n");
    }
    else
    {
        cout << "error displaying tables " << endl;
    }
}