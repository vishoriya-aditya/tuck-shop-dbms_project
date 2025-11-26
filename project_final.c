#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_U 200
#define MAX_B 500
#define MAX_IT 15

typedef struct
{
    int uid;
    char nm[40];
    char mail[60];
    char pw[20];
    float spent;
} usr;

typedef struct
{
    int bid;
    int uid;
    int ic;
    char iname[MAX_IT][40];
    int qty[MAX_IT];
    float rate[MAX_IT];
    float amt;
    char dt[11];
} bill;

typedef struct
{
    int m;
    int y;
    float amt;
} inv;

typedef struct
{
    int uid;
    int star;
    char txt[120];
} rev;

/* GLOBAL ARRAYS */
usr u_arr[MAX_U];
bill b_arr[MAX_B];
int u_cnt = 0;
int b_cnt = 0;
int next_uid = 1;
int next_bid = 1;

/* PROTOTYPES */
void getdt(char *s);
int loadu(void);
void saveu(void);
int loadb(void);
void saveb(void);
int findu(int need_pw);
void dayrec(int idx, const char *d, int *cnt, float *sum);
float invrec(float *a, int n, int i);
void adm_cust(void);
float bill_tot(bill *b);
void adm_bill(void);
void adm_day(void);
void adm_inv(void);
void adm_menu(void);
int u_reg(void);
void u_add(int uid);
void u_bills(int uid);
void u_rev(int uid);
void u_prof(int uid);
void user_menu(void);
void adm_exp(void);
void top_spent(void);
void inv_max(void);
void adm_stock(void);



void getdt(char *s)
{
    time_t t = time(NULL);
    struct tm *tmv = localtime(&t);
    strftime(s, 11, "%d-%m-%Y", tmv);
}

void print_user_login_menu(void)
{
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                     USER LOGIN / REG                         |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Existing User                                           |");
    puts("  |   2. New User                                                |");
    puts("  |   3. Back                                                    |");
    puts("  '--------------------------------------------------------------'");
    puts("");
}

void print_admin_panel_menu(void)
{
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                           ADMIN PANEL                        |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Customer Management                                     |");
    puts("  |   2. Generate Bill                                           |");
    puts("  |   3. Daily Earnings                                          |");
    puts("  |   4. Monthly Investment                                      |");
    puts("  |   5. Expense Tracker                                         |");
    puts("  |   6. Top Spending Customer                                   |");
    puts("  |   7. Highest Investment Period                               |");
    puts("  |   8. Stock Checker                                           |");
    puts("  |   9. Back to Main                                            |");
    puts("  '--------------------------------------------------------------'");
    puts("");
}

void print_user_panel_menu(void)
{
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                           USER PANEL                         |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Add Items (New Bill)                                    |");
    puts("  |   2. See Bill                                                |");
    puts("  |   3. Give Review                                             |");
    puts("  |   4. View Profile                                            |");
    puts("  |   5. Logout                                                  |");
    puts("  '--------------------------------------------------------------'");
    puts("");
}

void print_admin_customer_module(void)
{
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                   ADMIN  CUSTOMER  MODULE                    |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. List All Customers                                      |");
    puts("  |   2. Add New Customer                                        |");
    puts("  |   3. Find Customer                                           |");
    puts("  |   4. Delete Customer                                         |");
    puts("  |   5. Show All Reviews                                        |");
    puts("  |   6. Total Customer Count                                    |");
    puts("  |   7. Back                                                    |");
    puts("  '--------------------------------------------------------------'");
    puts("");
}

int loadu(void)
{
    FILE *fp = fopen("users.txt", "r");
    if (!fp)
    {
        u_cnt = 0;
        return 0;
    }
    u_cnt = 0;
    while (u_cnt < MAX_U)
    {
        usr u;
        if (fscanf(fp, "%d|%39[^|]|%59[^|]|%19[^|]|%f\n",&u.uid, u.nm, u.mail, u.pw, &u.spent) != 5)
        {
            break;
        }
        u_arr[u_cnt] = u;
        if (u.uid >= next_uid)
        {
            next_uid = u.uid + 1;
        }
        u_cnt++;
    }
    fclose(fp);
    return u_cnt;
}

void saveu(void)
{
    FILE *fp = fopen("users.txt", "w");
    if (!fp)
    {
        return;
    }
    int i;
    for (i = 0; i < u_cnt; i++)
    {
        fprintf(fp, "%d|%s|%s|%s|%.2f\n",
                u_arr[i].uid,
                u_arr[i].nm,
                u_arr[i].mail,
                u_arr[i].pw,
                u_arr[i].spent);
    }
    fclose(fp);
}

int findu(int need_pw)
{
    int opt;
    int id;
    char mail[60];
    char pw[20];
    int i;

    printf("Search user by 1.ID 2.Email : ");
    if (scanf("%d", &opt) != 1)
    {
        while (getchar() != '\n'){}
        return -1;
    }

    if (opt == 1)
    {
        printf("Enter id: ");
        scanf("%d", &id);
        for (i = 0; i < u_cnt; i++)
        {
            if (u_arr[i].uid == id)
            {
                if (need_pw)
                {
                    printf("Enter password: ");
                    while (getchar() != '\n'){}
                    fgets(pw, sizeof(pw), stdin);
                    int l = strlen(pw);
                    if (l > 0 && pw[l - 1] == '\n')
                    {
                        pw[l - 1] = '\0';
                    }
                    if (strcmp(u_arr[i].pw, pw) != 0)
                    {
                        printf("Wrong password\n");
                        return -1;
                    }
                }
                return i;
            }
        }
        printf("No such user\n");
        return -1;
    }
    else
    {
        printf("Enter email: ");
        while (getchar() != '\n'){}
        fgets(mail, sizeof(mail), stdin);
        int l = strlen(mail);
        if (l > 0 && mail[l - 1] == '\n')
        {
            mail[l - 1] = '\0';
        }
        for (i = 0; i < u_cnt; i++)
        {
            if (strcmp(u_arr[i].mail, mail) == 0)
            {
                if (need_pw)
                {
                    printf("Enter password: ");
                    fgets(pw, sizeof(pw), stdin);
                    l = strlen(pw);
                    if (l > 0 && pw[l - 1] == '\n')
                    {
                        pw[l - 1] = '\0';
                    }
                    if (strcmp(u_arr[i].pw, pw) != 0)
                    {
                        printf("Wrong password\n");
                        return -1;
                    }
                }
                return i;
            }
        }
        printf("No such user\n");
        return -1; }}
void adm_stock(void)
{
    int ch;
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                        STOCK  CHECKER                        |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Add / Update Stock Item                                 |");
    puts("  |   2. View All Stock                                          |");
    puts("  '--------------------------------------------------------------'");
    puts("");
    printf("Enter choice: ");
    if (scanf("%d", &ch) != 1)
    {
        while (getchar() != '\n'){}
        return;
    }

    if (ch == 1)
    {
        char item[40];
        int qty;

        while (getchar() != '\n'){}
        printf("Enter item name: ");
        fgets(item, sizeof(item), stdin);
        int l = strlen(item);
        if (l > 0 && item[l - 1] == '\n') item[l - 1] = '\0';

        printf("Enter quantity to add/update: ");
        scanf("%d", &qty);

        FILE *fp = fopen("stock.txt", "a");
        if (!fp)
        {
            printf("File error\n");
            return;
        }

        fprintf(fp, "%s|%d\n", item, qty);
        fclose(fp);

        printf("Stock updated successfully\n");
    }
    else if (ch == 2)
    {
        FILE *fp = fopen("stock.txt", "r");
        if (!fp)
        {
            printf("No stock data found\n");
            return;
        }

        char item[40];
        int qty;
        int total = 0;

        printf("\n--- Current Stock ---\n");
        while (fscanf(fp, "%39[^|]|%d\n", item, &qty) == 2)
        {
            printf("%s : %d units\n", item, qty);
            total++;
        }
        fclose(fp);

        if (total == 0)
            printf("No stock available\n");
    }
}

int u_reg(void)
{
    if (u_cnt >= MAX_U)
    {
        printf("User limit reached\n");
        return -1;
    }
    usr u;
    u.uid = next_uid++;
    while (getchar() != '\n')
    {
    }
    printf("Enter name: ");
    fgets(u.nm, sizeof(u.nm), stdin);
    int l = strlen(u.nm);
    if (l > 0 && u.nm[l - 1] == '\n')
    {
        u.nm[l - 1] = '\0';
    }
    printf("Enter email: ");
    fgets(u.mail, sizeof(u.mail), stdin);
    l = strlen(u.mail);
    if (l > 0 && u.mail[l - 1] == '\n')
    {
        u.mail[l - 1] = '\0';
    }
    int i;
    for (i = 0; i < u_cnt; i++)
    {
        if (strcmp(u_arr[i].mail, u.mail) == 0)
        {
            printf("Email already used\n");
            return -1;
        }
    }


    u.pw[0] = '\0';
    u.spent = 0.0f;
    u_arr[u_cnt] = u;
    u_cnt++;
    saveu();
    puts(".--------------------------------.");
    printf("Your user id is %d\n", u.uid);
    puts(".--------------------------------.");
    return u.uid;
}

void u_add(int uid)
{
    int i;
    int idx = -1;
    for (i = 0; i < u_cnt; i++)
    {
        if (u_arr[i].uid == uid)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("User not found\n");
        return;
    }
    int n;
    printf("\nHow many items: ");
    if (scanf("%d", &n) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    if (n <= 0 || n > MAX_IT)
    {
        printf("Invalid\n");
        return;
    }
    bill b;
    b.bid = next_bid++;
    b.uid = uid;
    b.ic = n;
    while (getchar() != '\n')
    {
    }
    for (i = 0; i < n; i++)
    {
        printf("Item %d name: ", i + 1);
        fgets(b.iname[i], sizeof(b.iname[i]), stdin);
        int l = strlen(b.iname[i]);
        if (l > 0 && b.iname[i][l - 1] == '\n')
        {
            b.iname[i][l - 1] = '\0';
        }
        printf("Qty: ");
        scanf("%d", &b.qty[i]);
        printf("Rate: ");
        scanf("%f", &b.rate[i]);
        while (getchar() != '\n')
        {
        }
    }
    getdt(b.dt);
    b.amt = bill_tot(&b);
    if (b_cnt < MAX_B)
    {
        b_arr[b_cnt] = b;
        b_cnt++;
        saveb();
    }
    u_arr[idx].spent += b.amt;
    saveu();
    printf("\nBill created with ID %d\n", b.bid);
    printf("Total amount: %.2f\n", b.amt);
}

void u_bills(int uid)
{
    int ch;
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                       USER BILL OPTIONS                      |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. View All Your Bills                                     |");
    puts("  |   2. Search Bill by ID                                       |");
    puts("  '--------------------------------------------------------------'");
    puts("");
    printf("Enter choice: ");
    if (scanf("%d", &ch) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    if (ch == 1)
    {
        int i;
        int f = 0;
        for (i = 0; i < b_cnt; i++)
        {
            if (b_arr[i].uid == uid)
            {
                int j;
                printf("\nBill ID: %d\n", b_arr[i].bid);
                printf("Date: %s\n", b_arr[i].dt);
                printf("Items:\n");
                for (j = 0; j < b_arr[i].ic; j++)
                {
                    printf("%d. %s x %d @ %.2f\n",
                           j + 1,
                           b_arr[i].iname[j],
                           b_arr[i].qty[j],
                           b_arr[i].rate[j]);
                }
                printf("Total: %.2f\n", b_arr[i].amt);
                f = 1;
            }
        }
        if (!f)
        {
            printf("No bills yet\n");
        }
    }
    else if (ch == 2)
    {
        int id;
        printf("Enter bill id: ");
        if (scanf("%d", &id) != 1)
        {
            while (getchar() != '\n')
            {
            }
            return;
        }
        int i;
        int f = 0;
        for (i = 0; i < b_cnt; i++)
        {
            if (b_arr[i].uid == uid && b_arr[i].bid == id)
            {
                int j;
                printf("\nBill ID: %d\n", b_arr[i].bid);
                printf("Date: %s\n", b_arr[i].dt);
                printf("Items:\n");
                for (j = 0; j < b_arr[i].ic; j++)
                {
                    printf("%d. %s x %d @ %.2f\n",
                           j + 1,
                           b_arr[i].iname[j],
                           b_arr[i].qty[j],
                           b_arr[i].rate[j]);
                }
                printf("Total: %.2f\n", b_arr[i].amt);
                f = 1;
                break;
            }
        }
        if (!f)
        {
            printf("Bill not found\n");
        }
    }
}

void u_prof(int uid)
{
    int i;
    int idx = -1;
    for (i = 0; i < u_cnt; i++)
    {
        if (u_arr[i].uid == uid)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("User not found\n");
        return;
    }
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                           PROFILE                            |");
    puts("  |--------------------------------------------------------------|");
    printf("  |   ID            : %-40d     |\n", u_arr[idx].uid);
    printf("  |   Name          : %-40s     |\n", u_arr[idx].nm);
    printf("  |   Email         : %-40s     |\n", u_arr[idx].mail);
    printf("  |   Total Spent   : %-40.2f   |\n", u_arr[idx].spent);
    puts("  '--------------------------------------------------------------'");
    puts("");
}

float bill_tot(bill *b)
{
    int i;
    float s = 0.0f;
    for (i = 0; i < b->ic; i++)
    {
        s += b->qty[i] * b->rate[i];
    }
    return s;
}

void user_menu(void)
{
    int ch;
    int uid = -1;
    while (1)
    {
        print_user_login_menu();
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }
	if (ch == 1)
        {
 	   int idx = findu(0);
   if (idx != -1)
        {
      	       uid = u_arr[idx].uid;
        	       printf("Login ok, welcome %s\n", u_arr[idx].nm);
                 break;
         }
   }
        else if (ch == 2)
        {
            uid = u_reg();
            if (uid != -1)
            {
                printf("Registered and logged in\n");
                break;
            }
        }
        else if (ch == 3)
        {
            return;
        }
    }
    if (uid == -1)
    {
        return;
    }
    while (1)
    {
        print_user_panel_menu();
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }
        if (ch == 1)
        {
            u_add(uid);
        }
        else if (ch == 2)
        {
            u_bills(uid);
        }
        else if (ch == 3)
        {
            u_rev(uid);
        }
        else if (ch == 4)
        {
            u_prof(uid);
        }
        else if (ch == 5)
        {
            break;
        }
    }
}

void adm_cust(void)
{
    int ch;
    while (1)
    {
        print_admin_customer_module();
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n') {
            }
            continue;
        } if (ch == 1)
        {
            int i;
            printf("\nID\tName\t\tEmail\t\tSpent\n");
            for (i = 0; i < u_cnt; i++) {
                printf("%d\t%s\t\t%s\t\t%.2f\n",
                       u_arr[i].uid,
                       u_arr[i].nm,
                       u_arr[i].mail,
                       u_arr[i].spent);
            }
        }else if (ch == 2){
            if (u_cnt >= MAX_U)
            {
                printf("User limit reached\n");
                continue;
            }
            usr u;
            u.uid = next_uid++;
            printf("Enter name: ");
            while (getchar() != '\n')
            {
            }
            fgets(u.nm, sizeof(u.nm), stdin);
            int l = strlen(u.nm);
            if (l > 0 && u.nm[l - 1] == '\n')
            {
                u.nm[l - 1] = '\0';
            }
            printf("Enter email: ");
            fgets(u.mail, sizeof(u.mail), stdin);
            l = strlen(u.mail);
            if (l > 0 && u.mail[l - 1] == '\n')
            {
                u.mail[l - 1] = '\0';
            }
            int i;
            int dup = 0;
            for (i = 0; i < u_cnt; i++)
            {
                if (strcmp(u_arr[i].mail, u.mail) == 0)
                {
                    dup = 1;
                    break;
                }
            }
            if (dup)
            {
                printf("Email already used\n");
                continue;
            }
            printf("Set password: ");
            fgets(u.pw, sizeof(u.pw), stdin);
            l = strlen(u.pw);
            if (l > 0 && u.pw[l - 1] == '\n')
            {
                u.pw[l - 1] = '\0';
            }
            u.spent = 0.0f;
            u_arr[u_cnt] = u;
            u_cnt++;
            saveu();
            printf("New customer ID: %d\n", u.uid);
        }
        else if (ch == 3)
        {
            int idx = findu(0);
            if (idx != -1)
            {
                printf("ID:%d Name:%s Email:%s Spent:%.2f\n",
                       u_arr[idx].uid,
                       u_arr[idx].nm,
                       u_arr[idx].mail,
                       u_arr[idx].spent);
            }
        }
        else if (ch == 4)
        {
            int idx = findu(0);
            if (idx == -1)
            {
                continue;
            }
            int id = u_arr[idx].uid;
            int i, j;
            for (i = idx; i < u_cnt - 1; i++)
            {
                u_arr[i] = u_arr[i + 1];
            }
            u_cnt--;
            saveu();
            int k = 0;
            for (j = 0; j < b_cnt; j++)
            {
                if (b_arr[j].uid != id)
                {
                    b_arr[k++] = b_arr[j];
                }
            }
            b_cnt = k;
            saveb();
            printf("Customer and related bills removed\n");
        }
        else if (ch == 5)
        {
            FILE *fp = fopen("reviews.txt", "r");
            if (!fp)
            {
                printf("No reviews yet\n");
            }
            else
            {
                rev r;
                printf("\nUID\tStar\tText\n");
                while (fscanf(fp, "%d|%d|%119[^\n]\n",
                              &r.uid, &r.star, r.txt) == 3)
                {
                    printf("%d\t%d\t%s\n", r.uid, r.star, r.txt);
                }
                fclose(fp);
            }
        }
        else if (ch == 6)
        {
            printf("Total customers: %d\n", u_cnt);
        }
        else if (ch == 7)
        {
            break;
        }
    }
}

void dayrec(int idx, const char *d, int *cnt, float *sum)
{
    if (idx >= b_cnt)
    {
        return;
    }
    if (strcmp(b_arr[idx].dt, d) == 0)
    {
        (*cnt)++;
        (*sum) += b_arr[idx].amt;
    }
    dayrec(idx + 1, d, cnt, sum);
}

void adm_day(void)
{
    char d[11];
    int ch;
    printf("\n1. Enter date  2. Use today : ");
    if (scanf("%d", &ch) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    if (ch == 1)
    {
        printf("Enter date (DD-MM-YYYY): ");
        while (getchar() != '\n')
        {
        }
        fgets(d, sizeof(d), stdin);
        int l = strlen(d);
        if (l > 0 && d[l - 1] == '\n')
        {
            d[l - 1] = '\0';
        }
    }
    else
    {
        getdt(d);
    }
    int cnt = 0;
    float sum = 0.0f;
    dayrec(0, d, &cnt, &sum);
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                        DAILY  EARNINGS                       |");
    puts("  |--------------------------------------------------------------|");
    printf("  |   Date            : %-40s |\n", d);
    printf("  |   Bills Processed : %-40d |\n", cnt);
    printf("  |   Total Earnings  : %-40.2f |\n", sum);
    puts("  '--------------------------------------------------------------'");
    puts("");
}

float invrec(float *a, int n, int i)
{
    if (i >= n) {
        return 0.0f;
    }
    return a[i] + invrec(a, n, i + 1);
}

void adm_inv(void)
{
    int m, y;
    printf("\nEnter month (1-12): ");
    if (scanf("%d", &m) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    printf("Enter year: ");
    if (scanf("%d", &y) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    int ch;
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                     MONTHLY   INVESTMENT                     |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Add New Investment                                      |");
    puts("  |   2. View Total for Month                                    |");
    puts("  '--------------------------------------------------------------'");
    puts("");
    printf("Enter choice: ");
    if (scanf("%d", &ch) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    FILE *fp = fopen("invest.txt", "a+");
    if (!fp)
    {
        printf("File error\n");
        return;
    }
    if (ch == 1)
    {
        float a;
        printf("Enter amount: ");
        scanf("%f", &a);
        fprintf(fp, "%d|%d|%.2f\n", m, y, a);
        printf("Saved\n");
    }
    fclose(fp);
    fp = fopen("invest.txt", "r");
    if (!fp)
    {
        printf("File error\n");
        return;
    }
    float arr[200];
    int n = 0;
    inv v;
    while (fscanf(fp, "%d|%d|%f\n", &v.m, &v.y, &v.amt) == 3)
    {
        if (v.m == m && v.y == y)
        {
            if (n < 200)
            {
                arr[n] = v.amt;
                n++;
            }
        }
    }
    fclose(fp);
    float tot = invrec(arr, n, 0);
    printf("Total investment for %02d-%d : %.2f\n", m, y, tot);
}

void adm_bill(void)
{
    int uid;
    printf("\nEnter customer id for bill: ");
    if (scanf("%d", &uid) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    int i;
    int idx = -1;
    for (i = 0; i < u_cnt; i++)
    {
        if (u_arr[i].uid == uid)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("Customer not found\n");
        return;
    }
    int n;
    printf("How many items: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX_IT)
    {
        printf("Invalid count\n");
        return;
    }
    bill b;
    b.bid = next_bid++;
    b.uid = uid;
    b.ic = n;
    while (getchar() != '\n')
    {
    }
    for (i = 0; i < n; i++)
    {
        printf("Item %d name: ", i + 1);
        fgets(b.iname[i], sizeof(b.iname[i]), stdin);
        int l = strlen(b.iname[i]);
        if (l > 0 && b.iname[i][l - 1] == '\n')
        {
            b.iname[i][l - 1] = '\0';
        }
        printf("Qty: ");
        scanf("%d", &b.qty[i]);
        printf("Rate: ");
        scanf("%f", &b.rate[i]);
        while (getchar() != '\n')
        {
        }
    }
    getdt(b.dt);
    b.amt = bill_tot(&b);
    if (b_cnt < MAX_B)
    {
        b_arr[b_cnt] = b;
        b_cnt++;
        saveb();
    }
    u_arr[idx].spent += b.amt;
    saveu();
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                          BILL SUMMARY                        |");
    puts("  |--------------------------------------------------------------|");
    printf("  |   Bill ID        : %-40d |\n", b.bid);
    printf("  |   Date           : %-40s |\n", b.dt);
    printf("  |   Customer       : %-40s |\n", u_arr[idx].nm);
    printf("  |   Total Amount   : %-40.2f |\n", b.amt);
    puts("  '--------------------------------------------------------------'");
    puts("");
}

void adm_menu(void)
{
    int ch;
    while (1)
    {
        print_admin_panel_menu();
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }

        if (ch == 1)
        {
            adm_cust();
        }
        else if (ch == 2)
        {
            adm_bill();
        }
        else if (ch == 3)
        {
            adm_day();
        }
        else if (ch == 4)
        {
            adm_inv();
        }
        else if (ch == 5)
        {
            adm_exp();
        }
        else if (ch == 6)
        {
            top_spent();
        }
        else if (ch == 7)
        {
            inv_max();
        }
        else if (ch == 8)
        {
            adm_stock();
        }
        else if (ch == 9)
        {
            break;
        }
    }
}

int loadb(void)
{
    FILE *fp = fopen("bills.txt", "r");
    if (!fp)
    {
        b_cnt = 0;
        return 0;
    }
    b_cnt = 0;
    while (b_cnt < MAX_B)
    {
        bill b;
        if (fscanf(fp, "%d|%d|%d|%10[^|]|%f\n",
                   &b.bid, &b.uid, &b.ic, b.dt, &b.amt) != 5)
        {
            break;
        }
        int i;
        for (i = 0; i < b.ic; i++)
        {
            if (fscanf(fp, "%39[^|]|%d|%f\n",
                       b.iname[i], &b.qty[i], &b.rate[i]) != 3)
            {
                break;
            }
        }
        b_arr[b_cnt] = b;
        if (b.bid >= next_bid)
        {
            next_bid = b.bid + 1;
        }
        b_cnt++;
    }
    fclose(fp);
    return b_cnt;
}

void saveb(void)
{
    FILE *fp = fopen("bills.txt", "w");
    if (!fp)
    {
        return;
    }
    int i, j;
    for (i = 0; i < b_cnt; i++)
    {
        bill *b = &b_arr[i];
        fprintf(fp, "%d|%d|%d|%s|%.2f\n",
                b->bid, b->uid, b->ic, b->dt, b->amt);
        for (j = 0; j < b->ic; j++)
        {
            fprintf(fp, "%s|%d|%.2f\n",
                    b->iname[j], b->qty[j], b->rate[j]);
        }
    }
    fclose(fp);
}

void u_rev(int uid)
{
    rev r;
    r.uid = uid;
    printf("\nRate shop from 1 to 5: ");
    if (scanf("%d", &r.star) != 1)
    {
        while (getchar() != '\n')
        {
        }
        return;
    }
    while (getchar() != '\n')
    {
    }
    printf("Write short review: ");
    fgets(r.txt, sizeof(r.txt), stdin);
    int l = strlen(r.txt);
    if (l > 0 && r.txt[l - 1] == '\n')
    {
        r.txt[l - 1] = '\0';
    }
    FILE *fp = fopen("reviews.txt", "a");
    if (!fp)
    {
        printf("File error\n");
        return;
    }
    fprintf(fp, "%d|%d|%s\n", r.uid, r.star, r.txt);
    fclose(fp);
    printf("Thanks for feedback\n");
}

void adm_exp(void)
{
    int ch;
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                        EXPENSE TRACKER                       |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Add New Expense                                         |");
    puts("  |   2. View Category-wise Totals                               |");
    puts("  '--------------------------------------------------------------'");
    puts("");
    printf("Enter choice: ");
    if (scanf("%d", &ch) != 1)
    {
        while (getchar() != '\n'){}
        return;
    }

    if (ch == 1)
    {
        char cat[30];
        float amt;
        while (getchar() != '\n'){}
        printf("Enter category (stock/electricity/repair): ");
        fgets(cat, sizeof(cat), stdin);
        int l = strlen(cat);
        if (l > 0 && cat[l - 1] == '\n') cat[l - 1] = '\0';

        printf("Enter amount: ");
        if (scanf("%f", &amt) != 1)
        {
            while (getchar() != '\n'){}
            return;
        }

        FILE *fp = fopen("expense.txt", "a");
        if (!fp)
        {
            printf("File error\n");
            return;
        }

        char d[11];
        getdt(d);
        fprintf(fp, "%s|%s|%.2f\n", d, cat, amt);
        fclose(fp);

        printf("Expense saved\n");
    }
    else if (ch == 2)
    {
        FILE *fp = fopen("expense.txt", "r");
        if (!fp)
        {
            printf("No expense data found\n");
            return;
        }

        float stock = 0, ele = 0, rep = 0;
        char dt[11], cat[30];
        float amt;

        while (fscanf(fp, "%10[^|]|%29[^|]|%f\n", dt, cat, &amt) == 3)
        {
            if (strcmp(cat, "stock") == 0)
                stock += amt;
            else if (strcmp(cat, "electricity") == 0)
                ele += amt;
            else if (strcmp(cat, "repair") == 0)
                rep += amt;
        }

        fclose(fp);

        puts("");
        puts("  .--------------------------------------------------------------.");
        puts("  |                      EXPENSE   SUMMARY                       |");
        puts("  |--------------------------------------------------------------|");
        printf("  |   Stock Expense        : %10.2f                         |\n", stock);
        printf("  |   Electricity Expense  : %10.2f                         |\n", ele);
        printf("  |   Repair Expense       : %10.2f                         |\n", rep);
        puts("  |--------------------------------------------------------------|");
        printf("  |   Total Expense        : %10.2f                         |\n", stock+ele+rep);
        puts("  '--------------------------------------------------------------'");
        puts("");
    }
}

void top_spent(void)
{
    if (u_cnt == 0)
    {
        printf("No customers found\n");
        return;
    }
    int i;
    int idx = 0;
    for (i = 1; i < u_cnt; i++)
    {
        if (u_arr[i].spent > u_arr[idx].spent)
        {
            idx = i;
        }
    }
    puts(" '|------------------------------------|'");
    printf("|\n--- Top Spending Customer ---|\n");
    puts(" '|------------------------------------|'");
    printf("|ID   : %d                           |\n", u_arr[idx].uid);
    printf("|Name : %s                           |\n", u_arr[idx].nm);
    printf("|Email: %s                           |\n", u_arr[idx].mail);
    printf("|Spent: %.2f                         |\n", u_arr[idx].spent);
    puts(" '|------------------------------------|'");
}

void inv_max(void)
{
    FILE *fp = fopen("invest.txt", "r");
    if (!fp)
    {
        printf("No investment data found\n");
        return;
    }
    int m_arr[200];
    int y_arr[200];
    float sum_arr[200];
    int n = 0;
    int m, y;
    float a;
    while (fscanf(fp, "%d|%d|%f\n", &m, &y, &a) == 3)
    {
        int i;
        int pos = -1;
        for (i = 0; i < n; i++)
        {
            if (m_arr[i] == m && y_arr[i] == y)
            {
                pos = i;
                break;
            }
        }
        if (pos == -1)
        {
            if (n < 200)
            {
                m_arr[n] = m;
                y_arr[n] = y;
                sum_arr[n] = a;
                n++;
            }
        }
        else
        {
            sum_arr[pos] += a;
        }
    }
    fclose(fp);
    if (n == 0)
    {
        printf("No valid investment entries\n");
        return;
    }
    int max_i = 0;
    int i;
    for (i = 1; i < n; i++)
    {
        if (sum_arr[i] > sum_arr[max_i])
        {
            max_i = i;
        }
    }
    printf("\n--- Highest Investment Period ---\n");
    printf("Month-Year : %02d-%d\n", m_arr[max_i], y_arr[max_i]);
    printf("Total Invested: %.2f\n", sum_arr[max_i]);
}

void print_main_menu(void)
{
    puts("");
    puts("  .--------------------------------------------------------------.");
    puts("  |                        TUCK   SHOP                           |");
    puts("  |                    TUCK SHOP MANAGEMENT                      |");
    puts("  |--------------------------------------------------------------|");
    puts("  |   1. Admin                                                   |");
    puts("  |   2. User                                                    |");
    puts("  |   3. Exit                                                    |");
    puts("  '--------------------------------------------------------------'");
    puts("");
    fputs("Enter choice: ", stdout);
}

int main(void)
{
    loadu();
    loadb();
    int ch;
    while (1)
    {
        print_main_menu();
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }
        if (ch == 1)
        {
            char pw[20];
            printf("Enter admin password (admin): ");
            while (getchar() != '\n')
            {
            }
            fgets(pw, sizeof(pw), stdin);
            int l = strlen(pw);
            if (l > 0 && pw[l - 1] == '\n')
            {
                pw[l - 1] = '\0';
            }
            if (strcmp(pw, "admin") == 0)
            {
                adm_menu();
            }
            else
            {
                printf("Wrong password\n");
            }
        }
        else if (ch == 2)
        {
            user_menu();
        }
        else if (ch == 3)
        {
            break;
        }
    }
    saveu();
    saveb();
    return 0;}
