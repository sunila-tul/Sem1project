#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct ctm {
    char fname[20];
    char lname[20];
    unsigned phone;
    char email[30]; 
    char food[20];
    char username[20];
    char password[20];
    char code[20];
};

void displayCurrentDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


void resetPassword(FILE *file) {
    struct ctm c;
    char newcode[20];
    unsigned phone2;
    int idfound = 0;

    printf("Enter your contact number: ");
    scanf("%u", &phone2);
    while (getchar() != '\n'); //input buffer hatauna
    printf("Enter your secret code: ");
    fgets(newcode, sizeof(newcode), stdin);
    newcode[strcspn(newcode, "\n")] = 0; //clear occurence of character in new line 
    rewind(file); //file ko suru ma lagxa 
    while (fread(&c, sizeof(c), 1, file)) {
        if (c.phone == phone2 && strcmp(c.code, newcode) == 0) // check gareko sign in ra signup ko phone ra code milxa ki nai
		 {
            printf("ID found!\n");
            printf("Enter new password: ");
            fgets(c.password, sizeof(c.password), stdin);
            c.password[strcspn(c.password, "\n")] = 0;
            fseek(file, sizeof(c), SEEK_CUR); // file ponter lai record ko suru ma lagney
            fwrite(&c, sizeof(c), 1, file);
            fflush(file); //confirms changes are made
            printf("Password reset successful!\n");
            idfound = 1;
            break;
        }
    }

    if (!idfound) {
        printf("ID not found or code is incorrect!\n");
    }
}

void signUp(FILE *file) {
    struct ctm c;

    printf("Enter your First Name: ");
    fgets(c.fname, sizeof(c.fname), stdin);
    c.fname[strcspn(c.fname, "\n")] = 0;
    printf("Enter your Last Name: ");
    fgets(c.lname, sizeof(c.lname), stdin);
    c.lname[strcspn(c.lname, "\n")] = 0;
    printf("Enter your contact number: ");
    scanf("%u", &c.phone);
    while (getchar() != '\n');
    printf("Enter your email: ");
    fgets(c.email, sizeof(c.email), stdin);
    c.email[strcspn(c.email, "\n")] = 0;
    printf("Enter a secret code (Please do not share!): ");
    fgets(c.code, sizeof(c.code), stdin);
    c.code[strcspn(c.code, "\n")] = 0;
    printf("Enter a username: ");
    fgets(c.username, sizeof(c.username), stdin);
    c.username[strcspn(c.username, "\n")] = 0;
    printf("Enter a password: ");
    fgets(c.password, sizeof(c.password), stdin);
    c.password[strcspn(c.password, "\n")] = 0;
    while (1) {
        printf("Re-Enter your password: ");
        char confirmPassword[20];
        fgets(confirmPassword, sizeof(confirmPassword), stdin);
        confirmPassword[strcspn(confirmPassword, "\n")] = 0;
        if (strcmp(c.password, confirmPassword) == 0) {
            fwrite(&c, sizeof(c), 1, file);
            printf("User signed up successfully!\n");
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
        }
    }
}

int signIn(FILE *file) {
    char username[20], password[20];
    struct ctm c;
    int found = 0;
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Enter your password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    rewind(file); // file ko suru ma lagxa
    while (fread(&c, sizeof(c), 1, file)) {
        if (strcmp(c.username, username) == 0 && strcmp(c.password, password) == 0) {
            found = 1;
            break;
        }
    }

    return found;
}

void login() {
    system("cls");
    FILE *file;
    int choice, signedIn=0;
    file = fopen("parking.txt", "a+");
    if (file == NULL) {
        printf("Unable to open file");
        exit(1);
    }
    while (signedIn!=1) {
        printf("Select an option:\n");
        printf("1. Sign Up\n");
        printf("2. Sign In\n");
        printf("3. Forgot password?\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        system("cls");

        switch (choice) { //sabai ma mathi banayeko funtionhrau apply garney
            case 1:
                signUp(file);
                break;
            case 2:
                signedIn = signIn(file); //signedin ma file as a parameter pass gareko
                if (signedIn) 
				{
                    printf("Login successful!\n");
                    // exit(0);
                } else {
                    printf("Invalid username or password. Please try again.\n");
                }
                break;
            case 3:
                resetPassword(file);
                break;
            case 4:
                fclose(file);
                exit(0);
            default:
                printf("Invalid choice. Please try again with input from 1 to 4.\n");
        }
    }

    fclose(file);

}