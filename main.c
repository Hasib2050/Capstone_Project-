/*
   

     Employee Management System 
     LOGIN       - password is needed to open the program
     FORGOT      - if you forget the password, use the recovery word
     1.  Add Employee
     2.  Show All Employees
     3.  Search by ID
     4.  Search by Name
     5.  Update Employee
     6.  Delete Employee
     7.  Mark Attendance (present / absent)
     8.  Salary Slip (salary - absent days)
     9.  Give Salary Raise (in percent)
     10. Sort by Salary
     11. Department Report
     12. Salary Report
     13. New Month (set all absent days to 0)
     14. Change Password
     0.  Exit

   Files used:
     employees.txt  - all employees
     password.txt   - password and recovery word

   
*/

#include <stdio.h>
#include <string.h>


typedef struct Employee     //EMP= struct Employee
{
    int   id;
    char  name[30];
    char  dept[30];
    char  desg[30];
    char  phone[20];
    float salary;
    int   absent;         //total number of absent days in a month
} EMP;

//An array of structure
EMP emp[100];

//Count of employee
int count = 0;

//Global variable for password
char password[30];
char recovery[30];


void save_password()
{
    FILE *fp;

    fp = fopen("password.txt", "w");
    fprintf(fp, "%s|%s\n", password, recovery); // I use '|' sign to separate passwords
    fclose(fp);
}

/* Runs only the very first time (when password.txt is not there) */
void first_time_setup()
{
    printf("\n  ==============================\n");
    printf("    FIRST TIME SETUP\n");
    printf("  ==============================\n");
    printf("  Create a password for this program.\n\n");

    printf("  New password  : ");
    scanf(" %29[^\n]", password);

    printf("  Recovery word : ");
    scanf(" %29[^\n]", recovery);

    save_password();

    printf("\n  Done! Remember your recovery word.\n");
    printf("  You need it if you forget the password.\n");
}

void load_password()
{
    FILE *fp;

    fp = fopen("password.txt", "r");
    if (fp == NULL)
    {
        first_time_setup();
        return;
    }

    fscanf(fp, "%29[^|]|%29[^\n]", password, recovery);
    fclose(fp);
}

/* Forgot password: the recovery word gives a new password */
int reset_password()
{
    char typed[30];

    printf("\n  Enter your recovery word: ");
    scanf(" %29[^\n]", typed);

    if (strcmp(typed, recovery) != 0)
    {
        printf("\n  Wrong recovery word.\n");
        return 0;
    }

    printf("  Enter new password: ");
    scanf(" %29[^\n]", password);
    save_password();

    printf("\n  Password reset done!\n");
    return 1;
}

/* Gives 1 if login is ok, 0 if not */
int login()
{
    char typed[30];
    int tries;
    int choice;

    printf("\n  ==============================\n");
    printf("    LOGIN\n");
    printf("  ==============================\n");

    for (tries = 1; tries <= 3; tries++)
    {
        printf("  Enter password (try %d of 3): ", tries);
        scanf(" %29[^\n]", typed);

        if (strcmp(typed, password) == 0)
        {
            printf("\n  Login successful. Welcome!\n");
            return 1;
        }
        printf("  Wrong password.\n\n");
    }

    printf("  Forgot your password?  1 = Yes   0 = Exit : ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        return reset_password();
    }
    return 0;
}

void change_password()
{
    char typed[30];

    printf("\n  Enter old password: ");
    scanf(" %29[^\n]", typed);

    if (strcmp(typed, password) != 0)
    {
        printf("\n  Wrong password.\n");
        return;
    }

    printf("  Enter new password: ");
    scanf(" %29[^\n]", password);
    save_password();

    printf("\n  Done! Password changed.\n");
}


/* =========================================================
   PART 2 : EMPLOYEE FILE
   ========================================================= */

void load_file()
{
    FILE *fp;
    struct Employee *p;

    fp = fopen("employees.txt", "r");
    if (fp == NULL)
    {
        return;                 /* no file yet, nothing to read */
    }

    p = emp;                    /* p points to the first employee */
    while (fscanf(fp, "%d %s %s %s %s %f %d",
                  &p->id, p->name, p->dept, p->desg,
                  p->phone, &p->salary, &p->absent) == 7)
    {
        p = p + 1;              /* go to the next employee */
        count = count + 1;
    }

    fclose(fp);
}

void save_file()
{
    FILE *fp;
    struct Employee *p;
    int i;

    fp = fopen("employees.txt", "w");

    for (i = 0; i < count; i++)
    {
        p = emp + i;            /* the i-th employee */
        fprintf(fp, "%d %s %s %s %s %.2f %d\n",
                p->id, p->name, p->dept, p->desg,
                p->phone, p->salary, p->absent);
    }

    fclose(fp);
}

/* Gives the position of the employee, or -1 if not found */
int find_employee(int id)
{
    struct Employee *p;
    int i;

    for (i = 0; i < count; i++)
    {
        p = emp + i;
        if (p->id == id)
        {
            return i;
        }
    }
    return -1;
}


/* =========================================================
   PART 3 : FEATURES
   ========================================================= */

/* ---------- 1. Add ---------- */
void add_employee()
{
    struct Employee *p;
    int id;

    if (count == 100)
    {
        printf("\n  Sorry, the list is full.\n");
        return;
    }

    printf("\n  Enter ID          : ");
    scanf("%d", &id);

    if (find_employee(id) != -1)
    {
        printf("\n  This ID is already used.\n");
        return;
    }

    p = emp + count;            /* the first empty place */
    p->id = id;

    printf("  Enter Name        : ");
    scanf("%29s", p->name);

    printf("  Enter Department  : ");
    scanf("%29s", p->dept);

    printf("  Enter Designation : ");
    scanf("%29s", p->desg);

    printf("  Enter Phone       : ");
    scanf("%19s", p->phone);

    printf("  Enter Salary      : ");
    scanf("%f", &p->salary);

    p->absent = 0;

    count = count + 1;
    save_file();
    printf("\n  Done! Employee added.\n");
}


/* ---------- 2. Show all ---------- */
void show_all()
{
    struct Employee *p;
    int i;

    if (count == 0)
    {
        printf("\n  No employee yet.\n");
        return;
    }

    printf("\n  ID    NAME           DEPARTMENT   DESIGNATION    PHONE        SALARY     ABSENT\n");
    printf("  -------------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        p = emp + i;
        printf("  %-5d %-14s %-12s %-14s %-12s %-10.2f %d\n",
               p->id, p->name, p->dept, p->desg, p->phone,
               p->salary, p->absent);
    }

    printf("  -------------------------------------------------------------------------------\n");
    printf("  Total employees: %d\n", count);
}


/* ---------- 3. Search by ID ---------- */
void search_by_id()
{
    struct Employee *p;
    int id;
    int position;

    printf("\n  Enter ID to search: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    p = emp + position;
    printf("\n  Name        : %s\n", p->name);
    printf("  Department  : %s\n", p->dept);
    printf("  Designation : %s\n", p->desg);
    printf("  Phone       : %s\n", p->phone);
    printf("  Salary      : %.2f\n", p->salary);
    printf("  Absent days : %d\n", p->absent);
}


/* ---------- 4. Search by Name ---------- */
void search_by_name()
{
    struct Employee *p;
    char name[30];
    int i;
    int found = 0;

    printf("\n  Enter name to search (same spelling): ");
    scanf("%29s", name);

    /* Check every employee one by one */
    for (i = 0; i < count; i++)
    {
        p = emp + i;
        if (strcmp(p->name, name) == 0)
        {
            printf("\n  ID          : %d\n", p->id);
            printf("  Name        : %s\n", p->name);
            printf("  Department  : %s\n", p->dept);
            printf("  Designation : %s\n", p->desg);
            printf("  Phone       : %s\n", p->phone);
            printf("  Salary      : %.2f\n", p->salary);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("\n  Employee not found.\n");
    }
}


/* ---------- 5. Update ---------- */
void update_employee()
{
    struct Employee *p;
    int id;
    int position;

    printf("\n  Enter ID to update: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    p = emp + position;

    printf("  New Name        : ");
    scanf("%29s", p->name);

    printf("  New Department  : ");
    scanf("%29s", p->dept);

    printf("  New Designation : ");
    scanf("%29s", p->desg);

    printf("  New Phone       : ");
    scanf("%19s", p->phone);

    printf("  New Salary      : ");
    scanf("%f", &p->salary);

    save_file();
    printf("\n  Done! Employee updated.\n");
}


/* ---------- 6. Delete ---------- */
void delete_employee()
{
    int id;
    int position;
    int i;

    printf("\n  Enter ID to delete: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    /* Move every employee after it one step back */
    for (i = position; i < count - 1; i++)
    {
        *(emp + i) = *(emp + i + 1);
    }

    count = count - 1;
    save_file();
    printf("\n  Done! Employee deleted.\n");
}


/* ---------- 7. Mark Attendance ---------- */
void mark_attendance()
{
    struct Employee *p;
    int id;
    int position;
    int choice;

    printf("\n  Enter ID: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    p = emp + position;

    printf("  Employee: %s\n", p->name);
    printf("  Today is:  1 = Present   2 = Absent : ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\n  Marked present.\n");
    }
    else if (choice == 2)
    {
        if (p->absent < 30)
        {
            p->absent = p->absent + 1;
        }
        save_file();
        printf("\n  Marked absent. Total absent days: %d\n", p->absent);
    }
    else
    {
        printf("\n  Wrong choice.\n");
    }
}


/* ---------- 8. Salary Slip ---------- */
void salary_slip()
{
    struct Employee *p;
    int id;
    int position;
    float per_day;
    float deduction;
    float net;

    printf("\n  Enter ID: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    p = emp + position;

    /* One month = 30 days */
    per_day   = p->salary / 30;
    deduction = per_day * p->absent;
    net       = p->salary - deduction;

    printf("\n  ==============================\n");
    printf("            SALARY SLIP\n");
    printf("  ==============================\n");
    printf("  Name         : %s\n", p->name);
    printf("  Department   : %s\n", p->dept);
    printf("  Designation  : %s\n", p->desg);
    printf("  ------------------------------\n");
    printf("  Basic Salary : %.2f\n", p->salary);
    printf("  Absent Days  : %d\n", p->absent);
    printf("  Deduction    : %.2f\n", deduction);
    printf("  ------------------------------\n");
    printf("  NET SALARY   : %.2f\n", net);
    printf("  ==============================\n");
}


/* ---------- 9. Give Salary Raise ---------- */
void give_raise()
{
    struct Employee *p;
    int id;
    int position;
    float percent;

    printf("\n  Enter ID: ");
    scanf("%d", &id);

    position = find_employee(id);
    if (position == -1)
    {
        printf("\n  Employee not found.\n");
        return;
    }

    p = emp + position;

    printf("  Old salary of %s: %.2f\n", p->name, p->salary);
    printf("  Raise in percent (example 10): ");
    scanf("%f", &percent);

    p->salary = p->salary + p->salary * percent / 100;

    save_file();
    printf("\n  Done! New salary: %.2f\n", p->salary);
}


/* ---------- 10. Sort by Salary (highest first) ---------- */
void sort_by_salary()
{
    struct Employee temp;
    int i;
    int j;

    if (count == 0)
    {
        printf("\n  No employee yet.\n");
        return;
    }

    /* Compare two neighbours again and again. Swap if wrong order. */
    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - 1 - i; j++)
        {
            if ((emp + j)->salary < (emp + j + 1)->salary)
            {
                temp = *(emp + j);
                *(emp + j) = *(emp + j + 1);
                *(emp + j + 1) = temp;
            }
        }
    }

    save_file();
    printf("\n  Sorted by salary (highest first).\n");
    show_all();
}


/* ---------- 11. Department Report ---------- */
void department_report()
{
    struct Employee *p;
    char dept[30];
    int i;
    int number = 0;
    float total = 0;

    printf("\n  Enter department name: ");
    scanf("%29s", dept);

    for (i = 0; i < count; i++)
    {
        p = emp + i;
        if (strcmp(p->dept, dept) == 0)
        {
            printf("  %-5d %-14s %-14s %.2f\n",
                   p->id, p->name, p->desg, p->salary);
            number = number + 1;
            total = total + p->salary;
        }
    }

    if (number == 0)
    {
        printf("\n  No employee in this department.\n");
        return;
    }

    printf("\n  Employees in %s : %d\n", dept, number);
    printf("  Total salary      : %.2f\n", total);
}


/* ---------- 12. Salary Report ---------- */
void salary_report()
{
    struct Employee *p;
    int i;
    float total = 0;
    float highest;
    float lowest;

    if (count == 0)
    {
        printf("\n  No employee yet.\n");
        return;
    }

    highest = emp->salary;      /* start with the first employee */
    lowest  = emp->salary;

    for (i = 0; i < count; i++)
    {
        p = emp + i;
        total = total + p->salary;

        if (p->salary > highest)
        {
            highest = p->salary;
        }
        if (p->salary < lowest)
        {
            lowest = p->salary;
        }
    }

    printf("\n  Total employees : %d\n", count);
    printf("  Total salary    : %.2f\n", total);
    printf("  Average salary  : %.2f\n", total / count);
    printf("  Highest salary  : %.2f\n", highest);
    printf("  Lowest salary   : %.2f\n", lowest);
}


/* ---------- 13. New Month ---------- */
void new_month()
{
    struct Employee *p;
    int i;
    int choice;

    printf("\n  Set absent days of everyone to 0?  1 = Yes   0 = No : ");
    scanf("%d", &choice);

    if (choice != 1)
    {
        printf("\n  Cancelled.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        p = emp + i;
        p->absent = 0;
    }

    save_file();
    printf("\n  Done! New month started.\n");
}

//The Main Function

int main()
{
    int choice = -1;
    int c;

    load_password();
    load_file();

    if (login() == 0)
    {
        printf("\n  Goodbye!\n");
        return 0;
    }



    while (choice != 0)
    {
        printf("\n");
        printf("  ==============================\n");
        printf("    EMPLOYEE MANAGEMENT SYSTEM\n");
        printf("  ==============================\n");
        printf("    1.  Add Employee\n");
        printf("    2.  Show All Employees\n");
        printf("    3.  Search by ID\n");
        printf("    4.  Search by Name\n");
        printf("    5.  Update Employee\n");
        printf("    6.  Delete Employee\n");
        printf("    7.  Mark Attendance\n");
        printf("    8.  Salary Slip\n");
        printf("    9.  Give Salary Raise\n");
        printf("    10. Sort by Salary\n");
        printf("    11. Department Report\n");
        printf("    12. Salary Report\n");
        printf("    13. New Month\n");
        printf("    14. Change Password\n");
        printf("    0.  Exit\n");
        printf("  ------------------------------\n");
        printf("  Enter your choice: ");

        /* if the user types a letter, ignore it */
        if (scanf("%d", &choice) != 1)
        {
            choice = 99;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
        }

        if (choice == 1)
        {
            add_employee();
        }
        else if (choice == 2)
        {
            show_all();
        }
        else if (choice == 3)
        {
            search_by_id();
        }
        else if (choice == 4)
        {
            search_by_name();
        }
        else if (choice == 5)
        {
            update_employee();
        }
        else if (choice == 6)
        {
            delete_employee();
        }
        else if (choice == 7)
        {
            mark_attendance();
        }
        else if (choice == 8)
        {
            salary_slip();
        }
        else if (choice == 9)
        {
            give_raise();
        }
        else if (choice == 10)
        {
            sort_by_salary();
        }
        else if (choice == 11)
        {
            department_report();
        }
        else if (choice == 12)
        {
            salary_report();
        }
        else if (choice == 13)
        {
            new_month();
        }
        else if (choice == 14)
        {
            change_password();
        }
        else if (choice == 0)
        {
            printf("\n  Goodbye!\n");
        }
        else
        {
            printf("\n  Wrong choice. Try again.\n");
        }
    }

    return 0;
}
