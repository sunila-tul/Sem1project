
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include "Login_1.c"
#define TWOWHEEL 1
#define FOURWHEEL 2
int time1, time2, time3, time4, a[55];
int j;
/*to store parking details*/
struct pd
{
	char name[20];
	int age, d, m, year;
	unsigned int Phno;
	int Park_num;
	float arr_time;
	int num_of_slots;
} parkdetails[10], pd2;
// store bill amt
struct billpay
{
	int bill_amount;
} bp;

/* to store vehicle number, and its  row-col position in an array */
struct vehicle
{
	int num;
	int row;
	int col;
	int type;

	struct tm *at; // time
};
struct vehicle *twowheel[2][10];
struct vehicle *fourwheel[2][10];

int parkinfo[4][10]; /* a 2-D array to store number of vehicle parked */
int vehcount;		 /* to store total count of vehicles */
int twowheelcount;	 /* stores total count of twowheelers */
int fourwheelcount;	 /* stores total count of fourwheelers */

// main reservation function
int slot(int);
void book(void);
void bill();
void viewdetails(void);
void cancel(void);

/* creates a 2-D array of two wheelers and four wheelers class */
void display(); // display information
void changecol(struct vehicle *);
void get_arrival_time(int);
int randint();
int backupw();
int backupr();
struct vehicle *add(int, int, int, int);
struct vehicle *addonstart(int, int, int, int);
void login();
void del(struct vehicle *);
void getfreerowcol(int, int *);
void getrcbyinfo(int, int, int *);
void show();
void total_bill(int, struct tm *ct1, struct tm *ct2);

/* decrements the col. number by one this function is called when the data is shifted one place to left */

void changecol(struct vehicle *v)
{
	v->col--;
}

struct tm *datetime() // at=datetime();
{

	time_t ts;
	/*current calendar time of a system and as a parameter can pass a pointer to a variable of type time_t which is the type for defining in time.h
	another is when you pass address of a variable of type time_t and this time function is gonna assign calendar time to variable and
	 if you pass null value it returns current calendar time.*/
	struct tm *ct; // ct=localtime
	/*it returns the pointer to the broken time in the form of tm structure,structure contains all the deatils like sec,min, hour etc..
	time is presented in local time and also the parameter it takes is of type time_t can be obatined with a call on time funcion
	and also the structure which is used to hold the broken down time which is returned by the local time function will be statically
	allocated and is overwritten each time this local time function is called.so if you want to save the contents of that structurw then
	 you need to copy that*/

	ts = time(NULL);
	ct = localtime(&ts);
	/* scan the year, month and year from the input string */

	return ct;
}
/*----------------------------------------------------Arival record of vechile--------------------------------------------------------------------------------*/
int record(int veh, int type, int row, int col, struct tm *ct) // function declaration
{
	FILE *fp;
	fp = fopen("arival.dat", "a");

	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		exit(0);
	}

	fprintf(fp, "\n"); // fprintf--> format print function sends output that is formatted to a stream.
	fprintf(fp, "%d ", veh);
	fprintf(fp, "%d ", type);
	fprintf(fp, "%d ", row);
	fprintf(fp, "%d ", col);
	fprintf(fp, "%d/%d/%d\n", ct->tm_mday, ct->tm_mon + 1, ct->tm_year + 1900);
	/*tm_mday : It indicates the day of the month. Its range is from 1 to 31.
	tm_mon : It indicates the month ranging from 0 to 11.So, to print the actual value of the current month,
				you will have to add 1 to its value.
	*tm_year : It indicates the number of the current year starting from 1900. So, to print the actual value of the current year,
				you will have to add 1900 to its value.*/

	fprintf(fp, "%d:%d:%d\n", ct->tm_hour, ct->tm_min, ct->tm_sec);
	fclose(fp);
}

/*-----------------------------------------------------------------------------Departure record of vechile-------------------------------------------------------------------------*/

int record2(int veh, struct tm *ct)
{
	FILE *fp;
	fp = fopen("depart.dat", "a");

	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		return 0;
	}

	fprintf(fp, "\n");
	fprintf(fp, "%d ", veh);
	fprintf(fp, "\n%d/%d/%d", ct->tm_mday, ct->tm_mon + 1, ct->tm_year + 1900);
	fprintf(fp, "%d:%d:%d", ct->tm_hour, ct->tm_min, ct->tm_sec);
	fclose(fp);
}

/*---------------------------------------------------------------------------------geting time for arrival of vehicle------------------------------------------------------------------------*/
void get_arrival_time(int num)
{
	int veh, type, row, col;
	int mon, day, year, hour, min, sec;
	int mon2, day2, year2, hour2, min2, sec2;

	int mon1, day1, year1, hour1, min1, sec1;
	int mon3, day3, year3, hour3, min3, sec3;

	FILE *fp;
	fp = fopen("arival.dat", "r");

	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		getch();
	}

	while (!feof(fp)) //(fgetc(fp))!=EOF)
	{
		fscanf(fp, "\n");
		fscanf(fp, "%d ", &veh);
		fscanf(fp, "%d ", &type);
		fscanf(fp, "%d/%d/%d  ", &day, &mon, &year);
		fscanf(fp, "%d:%d:%d  ", &hour, &min, &sec);

		if (veh == num)
		{
			day2 = day;
			mon2 = mon;
			year2 = year;
			hour2 = hour;
			min2 = min;
			sec2 = sec;
		}
	}
	fclose(fp);

	/*--------------------------------------------------------------- time for departure of vehicle----------------------------------------------------------------------*/

	FILE *fp1;
	fp1 = fopen("depart.dat", "r");
	if (fp1 == NULL)
	{
		printf("Error!File does not exists \n");
		getch();
	}

	while (!feof(fp1)) //(fgetc(fp1))!=EOF)
	{
		fscanf(fp1, "\n");
		fscanf(fp1, "%d ", &veh);
		fscanf(fp1, "%d/%d/%d  ", &day1, &mon1, &year1);
		fscanf(fp1, "%d:%d:%d  ", &hour1, &min1, &sec1);

		if (veh == num)
		{
			day3 = day1;
			mon3 = mon1;
			year3 = year1;
			hour3 = hour1;
			min3 = min1;
			sec3 = sec1;
		}
	}
	fclose(fp1);

	/*------------------------------------------Function in algorithm to start calculating times.----------------------------------------------------------------*/
	int tsec1, tsec2, tsec3;
	tsec1 = sec3;
	tsec1 += min3 * 60;
	tsec1 += (hour3 * 60) * 60;
	tsec2 = sec2;
	tsec2 += min2 * 60;
	tsec2 += (hour2 * 60) * 60;
	tsec3 = tsec1 - tsec2;

	int second, minute, hours;
	int secc, temp1, temp2, temp3, temp4;
	second = tsec3 % 60;
	temp1 = tsec3 - second;
	temp2 = temp1 / 60;
	minute = temp2 % 60;
	temp4 = temp2 - minute;
	hours = temp4 / 60;
	printf("you have parked your vehicle for %d/%d/%d", hours, minute, second);
}

int randint() // generate random numbers in the range [0, RAND_MAX).
{
	int r;
	srand(time(NULL)); // The srand() function sets the starting point for producing a series of pseudo-random integers.
	r = rand() % 20;
	return r;
}
/*---------------------------------------------------------------------fine sheet-----------------------------------------------------------------*/

void finesheet(int veh, int type, int row, int col, struct tm *ct)
{
	FILE *fp;
	fp = fopen("finesheet.dat", "a");
	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		getch();
	}
	fprintf(fp, "\n");
	fprintf(fp, "%d ", veh);
	fprintf(fp, "%d ", type);
	fprintf(fp, "%d/%d/%d ", ct->tm_mday, ct->tm_mon + 1, ct->tm_year + 1900);
	fprintf(fp, "%d:%d:%d  ", ct->tm_hour, ct->tm_min, ct->tm_sec);
	fprintf(fp, "%d ", 50);
	fclose(fp);
}

/*-----------------------------------------------------------------------Record history---------------------------------------------------------------*/

int historyrec(int value)
{
	FILE *fp;
	if (value == 1)
	{

		fp = fopen("arival.dat", "r");
	}
	else if (value == 2)
	{
		fp = fopen("depart.dat", "r");
	}
	else if (value == 3)
	{

		fp = fopen("finesheet.dat", "r");
	}
	else
	{
		printf("invalid input");
		return 0;
	}

	if (fp == NULL)
	{
		printf("File does not exists \n");
		return 0;
	}
	char ch;
	ch = getc(fp);
	while (ch != EOF)
	{
		printf("%c", ch);
		ch = getc(fp);
	}
	return 0;
}
/*-------------------------------------------------------------------placing of vechile...........................................................*/
int backupw()
{
	int r, c;

	FILE *fp;
	fp = fopen("backupw.dat", "w");

	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		return 0;
	}

	for (r = 0; r < 4; r++)
		for (c = 0; c < 10; c++)
		{

			fprintf(fp, "\n");
			fprintf(fp, "%d ", parkinfo[r][c]);

			fprintf(fp, "%d ", r);
			fprintf(fp, "%d ", c);

			fprintf(fp, "%d ", vehcount);
			fprintf(fp, "%d ", twowheelcount);
			fprintf(fp, "%d ", fourwheelcount);
		}
	fclose(fp);
}

int backupr()
{
	int r, c;
	int park[4][10];
	FILE *fp;
	fp = fopen("backupr.dat", "r");
	if (fp == NULL)
	{
		printf("Error!File does not exists \n");
		return 0;
	}

	int rr, cc, veh, twowheeler, fourwheeler, numb;
	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 10; c++)
		{
			fscanf(fp, "\n");
			fscanf(fp, "%d ", &park[r][c]);
			numb = park[r][c];

			fscanf(fp, "%d ", &rr);
			fscanf(fp, "%d ", &cc);
			fscanf(fp, "%d ", &veh);
			fscanf(fp, "%d ", &twowheeler);
			fscanf(fp, "%d ", &fourwheeler);

			if (numb != 0)
			{
				if (r == 0 || r == 1)
					twowheel[r][c] = addonstart(1, numb, r, c);
				else
					fourwheel[r][c] = addonstart(2, numb, r, c);
			}
		}
		fclose(fp);
	}
}
/* --------------------------------------------------------adding a data of vehicle------------------------------------------------------------------- */
struct vehicle *addonstart(int t, int num, int row, int col)
{
	struct vehicle *v;
	int r;

	v = (struct vehicle *)malloc(sizeof(struct vehicle));
	/*malloc: used to dynamically allocate a single large block of memory with the specified size.
	It returns a pointer of type void which can be cast into a pointer of any form.*/

	v->type = t;
	v->row = row;
	v->col = col;

	if (t == TWOWHEEL)
		twowheelcount++;
	else
		fourwheelcount++;

	vehcount++;
	parkinfo[row][col] = num;
	v->at = datetime();
	record(num, t, row, col, datetime());
	backupw();
	r = randint();
	if (r < 5)
	{
		printf("\a");
		printf("\a");
		finesheet(num, t, row, col, datetime());
	}

	return v;
}

/* ------------------------------------------------------------deletes the data of the specified car from the array, if found ---------------------------------------------------------------*/
void del(struct vehicle *v)
{
	int c;

	for (c = v->col; c < 9; c++)
	{
		parkinfo[v->row][c] = parkinfo[v->row][c + 1];
	}
	parkinfo[v->row][v->col] = 0;

	if (v->type == TWOWHEEL)
		twowheelcount--;
	else
		fourwheelcount--;

	vehcount--;
	record2(v->num, datetime());
	backupw();
}

/* -------------------------------------------------------------get the row-col position for the vehicle to be parked --------------------------------------------------------*/
void getrowcol(int type, int *arr)
{
	int r, c, fromrow = 0, torow = 2;

	if (type == FOURWHEEL)
	{
		fromrow += 2;
		torow += 2;
	}

	for (r = fromrow; r < torow; r++)
	{
		for (c = 0; c < 10; c++)
		{
			if (parkinfo[r][c] == 0)
			{
				arr[0] = r;
				arr[1] = c;
				return;
			}
		}
	}

	if (r == 2 || r == 4)
	{
		arr[0] = -1;
		arr[1] = -1;
	}
}

/* ---------------------------------------------------------------------get the row-col position for the vehicle with specified number -----------------------------------------------*/
void getrcbyinfo(int type, int num, int *arr)
{
	int r, c, fromrow = 0, torow = 2;

	if (type == FOURWHEEL)
	{
		fromrow += 2;
		torow += 2;
	}

	for (r = fromrow; r < torow; r++)
	{
		for (c = 0; c < 10; c++)
		{
			if (parkinfo[r][c] == num)
			{
				arr[0] = r;
				arr[1] = c;
				return;
			}
		}
	}

	if (r == 2 || r == 4)
	{
		arr[0] = -1;
		arr[1] = -1;
	}
}

/*-------------------------------------------------------------------------- displays list of vehicles parked--------------------------------------------------------------------------------- */
void display()
{
	int r, c;

	printf("Twowheel ->\n");

	for (r = 0; r < 4; r++)
	{
		if (r == 2)
			printf("Fourwheel ->\n");

		for (c = 0; c < 10; c++)
			printf("%d\t", parkinfo[r][c]);
		printf("\n");
	}
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main()
{
	login();
	int j, n;
	time_t currentTime;
	time(&currentTime);
	printf("%s\n", ctime(&currentTime));
	printf("\n\t\t\t\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n");
	printf("\t\t\t\t\t\xdb                                               \xdb\n");
	printf("\t\t\t\t\t\xdb       =============================           \xdb\n");
	printf("\t\t\t\t\t\xdb        MODIFIED PARKING SYSTEM                \xdb\n");
	printf("\t\t\t\t\t\xdb       =============================           \xdb\n");
	printf("\t\t\t\t\t\xdb                                               \xdb\n");
	printf("\t\t\t\t\t\xdb         Brought To You By                     \xdb\n");
	printf("\t\t\t\t\t\xdb                                               \xdb\n");
	printf("\t\t\t\t\t\xdb          Sunila and Team                      \xdb\n");
	printf("\t\t\t\t\t\xdb                                               \xdb\n");
	printf("\t\t\t\t\t\xdb                                               \xdb\n");
	printf("\t\t\t\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n\n\n");
	printf(" \n\t Press Any Key To Continue:");

	getch();
	system("cls");

	int choice, type, number, row = 0, col = 0;
	int i, tarr[2];
	int finish = 1;
	backupr();

	system("cls");

	while (finish)
	{
		system("cls");

		printf("\n------------------ Parking Mangaement System-----------------\n");
		printf("1. Space Booking of Vehicle\n");
		printf("2. Total bill/receipt.\n");
		printf("3. Cancel booking\n");
		printf("4. Arrival of a vehicle\n");
		printf("5. Total no. of vehicles parked\n");
		printf("6. Total no. of twowheelers parked\n");
		printf("7. Total no. of fourwheelers parked\n");
		printf("8. Display order in which vehicles are parked\n");
		printf("9. Departure of vehicle\n");
		printf("10. Check History\n");
		printf("11. Exit\n");

		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			system("cls");
			book();
			break;

		case 2:
			system("cls");
			bill();
			break;

		case 3:
			system("cls");
			cancel();
			break;

		case 4:
			system("cls");
			printf("\nArrival: \n");

			type = 0;

			/* check for vehicle type */
			while (type != TWOWHEEL && type != FOURWHEEL)
			{
				printf("Enter vehicle type (1 for Twowheel / 2 for Fourwheel ): \n");
				scanf("%d", &type);
				if (type != TWOWHEEL && type != FOURWHEEL)
					printf("\nInvalid vehicle type.\n");
			}

			printf("Enter vehicle number: ");
			scanf("%d", &number);

		
			system("cls");
			if (type == TWOWHEEL || type == FOURWHEEL)
			{
				getrowcol(type, tarr);

				if (tarr[0] != -1 && tarr[1] != -1)
				{
					row = tarr[0];
					col = tarr[1];

					if (type == TWOWHEEL)
						twowheel[row][col] = addonstart(type, number, row, col);
					else
						fourwheel[row - 2][col] = addonstart(type, number, row, col);
					;
				}
				else
				{
					if (type == TWOWHEEL)
						printf("\nNo parking slot free to park a twowheel\n");
					else
						printf("\nNo parking slot free to park a fourwheel\n");
				}
			}
			else
			{
				printf("Invalid type\n");
				break;
			}

			printf("\nPress any key to continue...");
			getch();
			break;

		case 5:
			system("cls");
			printf("Total vehicles parked: %d\n", vehcount);
			printf("\nPress any key to continue...");
			getch();
			break;

		case 6:
			system("cls");
			printf("Total twowheelers parked: %d\n", twowheelcount);
			printf("\nPress any key to continue...");
			getch();
			break;

		case 7:
			system("cls");
			printf("Total fourwheelers parked: %d\n", fourwheelcount);
			printf("\nPress any key to continue...");
			getch();
			break;

		case 8:
			system("cls");
			printf("Display\n");
			display();
			printf("\nPress any key to continue...");
			getch();
			break;

		case 9:
			system("cls");
			printf("Departure\n");

			// Check for vehicle type
			while (type != TWOWHEEL && type != FOURWHEEL)
			{
				printf("Enter vehicle type (1 for Twowheel / 2 for Fourwheel): \n");
				scanf("%d", &type);
				if (type != TWOWHEEL && type != FOURWHEEL)
				{
					printf("\nInvalid vehicle type.\n");
				}
			}

			// Enter vehicle number
			printf("Enter vehicle number: ");
			scanf("%d", &number);

			// Validate vehicle type and number
			if (type == TWOWHEEL || type == FOURWHEEL)
			{
				getrcbyinfo(type, number, tarr);
				if (tarr[0] != -1 && tarr[1] != -1)
				{
					col = tarr[1];

					// If the vehicle is a Twowheel
					if (type == TWOWHEEL)
					{
						row = tarr[0];
						del(twowheel[row][col]);
						for (i = col; i < 9; i++)
						{
							twowheel[row][i] = twowheel[row][i + 1];
						}
						free(twowheel[row][i]);
						twowheel[row][i] = NULL;
					}
					// If the vehicle is a Fourwheel
					else
					{
						row = tarr[0] - 2;
						if (row >= 0)
						{
							del(fourwheel[row][col]);
							for (i = col; i < 9; i++)
							{
								fourwheel[row][i] = fourwheel[row][i + 1];
							}
							fourwheel[row][i] = NULL;
						}
					}
				}
				// Invalid vehicle number
				else
				{
					if (type == TWOWHEEL)
					{
						printf("\nInvalid Twowheel number, or a twowheel with such number has not been parked here.\n");
					}
					else
					{
						printf("\nInvalid Fourwheel number, or a fourwheel with such number has not been parked here.\n");
					}
				}
			}

			printf("\nPress any key to continue...");
			getch();
			break;

		case 11:

			system("cls");
			for (row = 0; row < 2; row++)
			{
				for (col = 0; col < 10; col++)
				{
					if (twowheel[row][col]->num != 0)
						free(twowheel[row][col]);
					if (fourwheel[row][col]->num != 0)
						free(fourwheel[row + 2][col]);
				}
			}
			finish = 0;
			getch();
			break;

		case 10:
			system("cls");
			int option;
			printf("RECORD TABLES\n");
			printf(" 1. for Arrival history\n");
			printf(" 2. for Departure history\n");
			printf("3. for Fine Sheet\n");
			scanf("%d", &option);
			historyrec(option);
			printf("\nPress any key to continue...");

			getch();
			break;
		}
	}
	return 0;
}
void book()
{
	int i, age, m, year, d;
	int num_of_seats;
	int park_section;
	char confirm, y, n;
	int c;
	int x = 0;
	char name[20];
	FILE *fp, *fq;
	fp = fopen("D:\\Parking_Booking.txt", "r+");
	if (fp == NULL)
	{
		fp = fopen("D:\\Parking_Booking.txt", "w+");

		if (fp == NULL)
		{
			puts("File cannot be opened ");
			exit(0);
		}
	}
	fq = fopen("Parking_Booking1.txt", "r+");
	if (fq == NULL)
	{
		fq = fopen("Parking_Booking1.txt", "w+");

		if (fq == NULL)
		{
			puts("File cannot be opened ");
			exit(0);
		}
	}
	system("cls");
	int type, number;
	int num_of_slots;
	printf("=======================================SPACE BOOKING===================================\n\n");
	while (type != TWOWHEEL && type != FOURWHEEL)
	{
		printf("Enter vehicle type (1 for Twowheel / 2 for Fourwheel ): \n");
		scanf("%d", &type);
		if (type != TWOWHEEL && type != FOURWHEEL)
			printf("\nInvalid vehicle type.\n");
	}

	printf("Enter vehicle number: ");
	scanf("%d", &number);
	printf("Enter the Number Of Customers:: ");
	fflush(stdin);
	scanf("%d", &j);
	for (i = 0; i < j; i++)
	{
		printf("\nEnter the Name of Customer %d:: ", i + 1);
		fflush(stdin);
		gets(parkdetails[i].name);
		printf("enter the %d arrival time: ", i + 1);
		fflush(stdin);
		scanf("%f", &parkdetails[i].arr_time);
		printf("\nEnter the Phone number of Customer %d:: ", i + 1);
		fflush(stdin);
		scanf("%lf", &parkdetails[i].Phno);
		printf("\nEnter the date of booking(dd/mm/yy):: ");
		fflush(stdin);
		scanf("%d/%d/%d", &parkdetails[i].d, &parkdetails[i].m, &parkdetails[i].year);
	}

	fwrite(&parkdetails, sizeof(parkdetails), 1, fp);
	printf("\n\nEnter Number of slots do you need::> ");
	fflush(stdin);
	scanf("%d", &num_of_slots);
	printf("\n\n>>Press Enter To View Available Parking<< ");
	getch();
	system("cls");
	viewdetails();
label:
	printf("\n\nEnter parking section: ");
	scanf("%d", &park_section);
	do
	{
		switch (park_section)
		{
		case 1:
		{

			time1 = 5;
			time2 = 00;
			time3 = 10;
			time4 = 00;
			printf("Total Bill Amount: Rs 50 \n", d);
		}
		break;
		case 2:
		{
			time1 = 6;
			time2 = 00;
			time3 = 12;
			time4 = 00;
			printf("Total Bill Amount: Rs 50 \n", d);
		}
		break;
		case 3:
		{

			time1 = 8;
			time2 = 00;
			time3 = 13;
			time4 = 00;
			printf("Total Bill Amount: Rs 50 \n", d);
		}
		break;
		case 4:

		{

			time1 = 14;
			time2 = 10;
			time3 = 20;
			time4 = 00;
			printf("Total Bill Amount: Rs 50 \n", d);
		}
		break;
		case 5:
		{
			time1 = 2;
			time2 = 00;
			time4 = 23;
			time4 = 00;
			printf("Total Bill Amount: Rs 50 \n", d);
		}
		break;
		default:
			printf("Enter Correct choice.....\n");
			goto label;
		}
	} while (x);
	{
		bp.bill_amount = 50;
		fwrite(&bp, sizeof(bp), 1, fq);
		printf("\n *****For Booking Slots*****\n");
		// Calling slot() function with number of passenger
		slot(j);
		printf("\n\nConfirm Reservation (y/n):>");
	start:
		scanf(" %c", &confirm);
		if (confirm == 'y')
		{
			printf("==================");
			printf("\n Reservation Done\n");
			printf("==================");
			printf("\nPress any key to go back to Main menu");
		}
		else
		{
			if (confirm == 'n')
			{
				printf("\nReservation Not Done!\nPress any key to go back to  Main menu!");
			}
			else
			{
				printf("\nInvalid choice entered! Enter again-----> ");
				goto start;
			}
		}
	}
	fclose(fp);
	fclose(fq);
	getch();
}

/*********************************************PRINTTICKET()*************************************************/
// Code for printing receipt
void bill()
{
	char confirm;

	time_t currentTime;
	time(&currentTime);
	int i, c;
	FILE *fp, *fq;
	fp = fopen("Parking_Booking.txt", "r+");
	if (fp == NULL)
	{
		fp = fopen("Parking_Booking.txt", "w+");

		if (fp == NULL)
		{
			puts("File cannot be opened ");
			exit(0);
		}
	}
	fq = fopen("Parking_Booking1.txt", "r+");
	if (fq == NULL)
	{
		fq = fopen("Parking_Booking2.txt", "w+");

		if (fq == NULL)
		{
			puts("File cannot be open2ed ");
			exit(0);
		}
	}
	printf("===============================Total Bill/Receipt===================================\n\n");
	printf("Enter the number of customers: ");
	scanf("%d", &j);
	fread(&parkdetails, sizeof(parkdetails), 1, fp);
	fread(&bp, sizeof(bp), 1, fq);

	printf("*********************** YOUR RECEIPT ****************************");
	for (i = 0; i < j; i++)
	{
		printf("\t\t\nCustomer %d Name: ", i + 1);
		puts(parkdetails[i].name);
		printf("\t\t\nPassenger %d Phone Number %.1lf", i + 1, parkdetails[i].Phno);
		printf("\t\t\nPassenger %d Date Of Reservation %d/%d/%d", i + 1, parkdetails[i].d, parkdetails[i].m, parkdetails[i].year);
	}
	printf("\t\t\n\nTotal Bill amount -: Rs %d", bp.bill_amount);
	printf("\t\t\n\nTime : %s", ctime(&currentTime));
	printf("\n");
	printf("\n\nConfirm Ticket (y/n):>");
start:
	scanf(" %c", &confirm);
	if (confirm == 'y')
	{
		printf("==================");
		printf("\n Reservation Done\n");
		printf("==================");
		printf("\nPress any key to go back to Main menu");
	}
	else
	{
		if (confirm == 'n')
		{
			printf("\nReservation Not Done!\nPress any key to go back to  Main menu!");
		}
		else
		{
			printf("\nInvalid choice entered! Enter again-----> ");
			goto start;
		}
	}
	fclose(fp);
	fclose(fq);
	getch();
}
void viewdetails(void)
{
	system("cls");
	printf("----------------------------------------------------------------------------------------------------------------");
	printf("\nParking_Section\t\tAvailableTime From\t\tAvailableTime TO\n");
	printf("----------------------------------------------------------------------------------------------------------------");
	printf("\n1\t\t\t\t05:00\t\t\t\t10:00");
	printf("\n2\t\t\t\t6:00\t\t\t\t12:00");
	printf("\n3\t\t\t\t08:00\t\t\t\t13:00");
	printf("\n4\t\t\t\t14:00\t\t\t\t20:00");
	printf("\n5\t\t\t\t02:00\t\t\t\t23:00");
}

/*********************************************SEATDETAILS()*************************************************/

// Function for chosing seats
int slot(int p)
{
	int i;
	printf("\t           -:::SLOTS:::-        \n");
	printf("\t01\t02\t03\t04\t05\n");
	printf("\t06\t07\t08\t09\t010\n");
	printf("\t11\t12\t13\t14\t15\n");
	printf("\t16\t17\t18\t19\t20\n");

	printf("\n\n\tEnter Slot Numbers: ");
	for (i = 0; i < p; i++)
		scanf("%d", &a[i]);
}

/*********************************************Function for Cancel Menu()*************************************************/
void cancel(void)
{
	int sec;
	system("cls");
	printf("\n\n---------Cancel a Booking----------\n");
label:
	printf("\nEnter Your Parking section: ");
	scanf("%d", &sec);
	if (sec >= 1 && sec <= 5)
	{
		printf("\nYour reservation is cancelled successfully.");
	}
	else
	{
		printf("\nPlease enter a valid booking number to cancel reservation!");
		goto label;
	}
	getch();
}
