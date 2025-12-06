#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[20];

int loginSystem();
void getHiddenPassword(char *pwd);

void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();

void addstudent();
void displaystudent();
void searchstudent();
void updatestudent();
void deletestudent();

/* ======================
   HIDE PASSWORD FUNCTION
   ====================== */
void getHiddenPassword(char *pwd) {
    char ch;
    int i = 0;

    while (1) {
        ch = getch();

        if (ch == 13) { // ENTER
            pwd[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8) { // BACKSPACE
            if (i > 0) { 
                i--; 
                printf("\b \b"); 
            }
        }
        else {
            pwd[i++] = ch;
            printf("*");
        }
    }
}

/* ==============
   MAIN FUNCTION
   ============== */
int main() {
    if (loginSystem())
        mainMenu();
    else
        printf("\nLogin Failed, Exiting...\n");

    return 0;
}

/* ==================
   LOGIN FUNCTION
   ================== */
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];
    int attempts = 3;

    while (attempts > 0) {

        printf("\n====== LOGIN SCREEN ======\n");
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        getHiddenPassword(password);

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials.txt not found!\n");
            return 0;
        }

        int found = 0;

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) {
            printf("\nLogin Successful! Welcome %s\n", currentUser);
            return 1;
        } else {
            attempts--;
            printf("\nInvalid Credentials! Attempts left: %d\n", attempts);
        }
    }
    return 0;
}

/* ==================
   MAIN MENU ROUTING
   ================== */
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

/* ================
   ADMIN MENU
   ================ */
void adminMenu() {
    int choice;
    while (1) {
        printf("\n==== ADMIN MENU ====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addstudent(); break;
            case 2: displaystudent(); break;
            case 3: searchstudent(); break;
            case 4: updatestudent(); break;
            case 5: deletestudent(); break;
            case 6: printf("Logging out....\n"); return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ================
   STAFF MENU
   ================ */
void staffMenu() {
    int choice;
    while (1) {
        printf("\n==== STAFF MENU ====\n");
        printf("1. Display All Records\n");
        printf("2. Search Record\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1: displaystudent(); break;
            case 2: searchstudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* ================
   GUEST MENU
   ================ */
void guestMenu() {
    printf("\n==== GUEST MENU ====\n");
    displaystudent();
}

/* ===========================
   FILE OPERATIONS FUNCTIONS
   =========================== */

/* ADD STUDENT */
void addstudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student s;

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);

    printf("Enter Name: ");
    scanf("%s", s.name);

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("\n? Student Added Successfully!\n");
}

/* DISPLAY STUDENTS */
void displaystudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    printf("\n==== STUDENT RECORDS ====\n");

    if (!fp) {
        printf("No records found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/* SEARCH STUDENT */
void searchstudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;
    int roll, ch;
    char name[50];
    int found = 0;

    if (!fp) { printf("No file found.\n"); return; }

    printf("\nSearch by:\n1. Roll\n2. Name\nEnter choice (1 or 2): ");
    scanf("%d", &ch);

    if (ch == 1) {
        printf("\nEnter Roll Number: ");
        scanf("%d", &roll);

        while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
            if (s.roll == roll) {
                printf("\n? Record Found: %d %s %.2f\n", s.roll, s.name, s.marks);
                found = 1;
            }
        }
    } 
    else if (ch == 2) {
        printf("\nEnter Name: ");
        scanf("%s", name);

        while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
            if (strcmp(s.name, name) == 0) {
                printf("\n? Record Found: %d %s %.2f\n", s.roll, s.name, s.marks);
                found = 1;
            }
        }
    }
    else {
        printf("Invalid choice!\n");
        fclose(fp);
        return;
    }

    if (!found)
        printf("\n? No matching record found!\n");

    fclose(fp);
}

/* UPDATE STUDENT */
void updatestudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student s;
    int ch, roll, found = 0;
    char name[50];

    if (!fp) { printf("No File Found!\n"); return; }

    printf("\nUpdate by:\n1. Roll\n2. Name\nEnter choice (1 or 2): ");
    scanf("%d", &ch);

    if (ch == 1) {
        printf("\nEnter Roll Number: ");
        scanf("%d", &roll);
    } 
    else if (ch == 2) {
        printf("\nEnter Name: ");
        scanf("%s", name);
    }
    else {
        printf("\nInvalid choice!\n");
        fclose(fp);
        fclose(temp);
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {

        if ((ch == 1 && s.roll == roll) || (ch == 2 && strcmp(s.name, name) == 0)) {

            printf("Enter New Roll: ");
            scanf("%d", &s.roll);
            printf("Enter New Name: ");
            scanf("%s", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            found = 1;
        }

        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("\n? Record Updated Successfully!\n");
    else printf("\n? Record Not Found!\n");
}

/* DELETE STUDENT */
void deletestudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student s;
    int ch, roll, found = 0;
    char name[50];

    if (!fp) { printf("No file found!\n"); return; }

    printf("\nDelete by:\n1. Roll\n2. Name\nEnter choice (1 or 2): ");
    scanf("%d", &ch);

    if (ch == 1) {
        printf("\nEnter Roll Number: ");
        scanf("%d", &roll);
    } 
    else if (ch == 2) {
        printf("\nEnter Name: ");
        scanf("%s", name);
    }
    else {
        printf("\nInvalid choice!\n");
        fclose(fp);
        fclose(temp);
        return;
    }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {

        if ((ch == 1 && s.roll == roll) || (ch == 2 && strcmp(s.name, name) == 0))
            found = 1;
        else
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("\n? Record Deleted!\n");
    else printf("\n? Record Not Found!\n");
}

