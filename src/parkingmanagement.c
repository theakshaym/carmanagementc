// Parking Management using C (Single list method)

/*
TODO:
	* Change Vehicle number to string.
	-Add a boolean value to check if the lot is occupied-
	* Fix the infinite loop when entered characters in choice menus.
	* Remove all the goto: have functions do the same instead.
	-Replace spaces with escape character where possible.-
	-Try finding alternative for the colors. Or just remove it.-
	* Remove "Press any key to continue" prompt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#define MAXLOTS 10	  // Total parking lots
#define MINAMT 5		  // Minimum parking amount
#define AMTPERMIN 0.25 // Amount per minute of parking

int entryPromptAndMenus();
void createEmptyParkingLots();				// Creates MAXLOTS number of nodes and stores default value in fields i.e 0
int isFileExists(char name[]);				// checks if .log exists or not in the directory
void getAndStoreImportLog();					// Reads import-log files and stores it in Linked list
void checkInCar();								// To Check-in the vehicle
void checkOutCar();								// To Check-out the vehicle
void displayLotStatus();						// To display current parking lot status
int additionalSettings();						// For choice 4 in Menu
void createCheckOutLog(int cost, int tt); // To create a check-out log file
void createDayLog();								// Creates a daily log when user exits from program. Also creates import log which is needed to recover data

typedef struct node
{
	uint8_t lotStatus;
	int carNumber;
	int time; // Stores time in seconds
	uint8_t lotNumber;
	struct node *link;
} * node;

node first = NULL;
node g;
FILE *f; // To create logs
int charge;

int main()
{
	int exitCode = entryPromptAndMenus();
	printf("\nGoodbye!\n\n");
}

int entryPromptAndMenus()
{
	int fileStatus, i, j;
	char userChoice, programExitChoice;
	char filename[] = ".import.log";
	char s[64];
	node r;
	printf("\n\n\n\tParking Management System\t\n");

	for (j = 1; j <= 41; j++)
	{
		printf("*");
	}

	createEmptyParkingLots();

	fileStatus = isFileExists(filename); // To check if import log exists
	if (fileStatus == 1)
	{
		printf("\n\nLog exists. Retaining parking details..\n"
				 "Press any key to continue");
		getAndStoreImportLog();
	}
	else
	{
		printf("\n\nNo log file found. Values set to default.\n"
				 "Press any key to continue");
	}

	do
	{
		while ((getchar()) != '\n')
			;
		printf("\n\t       "
				 "MENU\t       \n");
		for (i = 1; i <= 33; i++)
		{ // Loop to print * above the Menu fonts.
			printf("*");
		}
		printf("\n"
				 "*"
				 "\t\t\t\t"
				 "*\n*"
				 "\t1.Check in\t\t"
				 "*\n*"
				 "\t2.Check out\t\t"
				 "*\n*"
				 "\t3.Display Status\t"
				 "*\n*"
				 "\t4.More settings \t"
				 "*\n*"
				 "\t5.Exit\t\t\t"
				 "*\n*\t\t\t\t*\n");
		for (i = 1; i <= 33; i++)
		{ // Loop to print * below the Menu fonts.
			printf("*");
		}
		printf("\n");
		printf("\nEnter your choice : ");
		scanf(" %c", &userChoice);
		switch (userChoice)
		{
		case '1':
			r = first;
			while (r != NULL)
			{
				if (r->carNumber == 0)
				{
					break;
				}
				else
				{
					r = r->link;
				}
			}
			if (r == NULL)
			{
				printf("\nAll lots are occupied.\n");
			}
			else
			{
				checkInCar();
			}
			break;
		case '2':
			checkOutCar();
			break;
		case '3':
			displayLotStatus();
			break;
		case '4':
			additionalSettings();
			break;
		case '5':
			do
			{
				printf("\nAre you sure you want to exit ("
						 "Y"
						 "\\"
						 "N"
						 ") : ");
				while ((getchar()) != '\n')
					;
				scanf(" %c", &programExitChoice);
				if (programExitChoice == 'y' || programExitChoice == 'Y')
				{
					createDayLog();
					return 0;
				}
				else if (programExitChoice == 'n' || programExitChoice == 'N')
				{
					break;
				}
				else
				{
					printf("\nInvalid key\n");
				}

			} while (programExitChoice != 'y' && programExitChoice != 'Y');
			break;

		default:
			printf("\n\nInvalid option.\n\n");
		}
	} while (1);
}

int isFileExists(char name[])
{
	if (f = fopen(name, "r"))
	{
		fclose(f);
		return 1;
	}
	return 0;
}

void createEmptyParkingLots()
{
	node p, q;
	int i = 2;
	first = (node)malloc(sizeof(struct node));
	first->lotStatus = 0;
	first->lotNumber = 1;
	first->carNumber = 0;
	first->time = 0;
	q = first;

	while (i <= MAXLOTS)
	{
		p = (node)malloc(sizeof(struct node));
		p->lotNumber = i;
		p->carNumber = p->time = 0;
		p->lotStatus = 0;
		first->link = p;
		first = p;
		i++;
	}
	first = q;
	p->link = NULL;
}

void getAndStoreImportLog()
{
	char filename[] = ".import.log";
	node p;
	p = first;
	f = fopen(".import.log", "a++");

	while (p != NULL)
	{
		fscanf(f, "%d%d%d%d", &p->lotStatus, &p->lotNumber, &p->carNumber, &p->time);
		p = p->link;
	}
	remove(filename);
}

void checkInCar()
{
	int n, cn, x = 0;
	char ch;
	node p, q, r;
beg:
	printf("\nLots available : \n\n");
	q = first;
	while (q != NULL)
	{
		if (q->lotStatus == 0)
		{
			printf("%d\t", q->lotNumber);
		}
		q = q->link;
	}
	printf("\n\nEnter lot number : ");
	scanf("%d", &n);
	if (n > MAXLOTS || n <= 0)
	{
		printf("\nInvalid lot number\n");
		printf("\nDo you want to return back to menu? ("
				 "Y"
				 "\\"
				 "N"
				 ") : ");
		scanf(" %c", &ch);
		if (ch == 'y' || ch == 'Y')
		{
			return;
		}
		else
		{
			goto beg;
		}
	}
	else
	{
		p = first;
		while (p->lotNumber != n)
		{
			p = p->link;
		}
		if (p->lotStatus == 0)
		{
		ret2:
			printf("Enter vehicle number (integer only) : ");
			scanf("%d", &cn);
			if (cn == 0)
			{
				printf("\nInvalid vehicle number\n\n");
				goto ret2;
			}
			else
			{
				p->carNumber = cn;
				p->time = time(NULL) / 60;
				p->lotStatus = 1;
			}
		}
		else
		{
			printf("\nLot already occupied.\n");
			r = first;
			while (r != NULL)
			{
				if (r->lotStatus == 0)
				{
					break;
				}
				else
				{
					r = r->link;
				}
			}
			if (r == NULL)
			{
				printf("\nAll lots are occupied.\n");
			}
			else
			{
				goto beg;
			}
		}
	}
}

void checkOutCar()
{
	node p;
	char key;
	int data, ch, cost, t, tt;
lab:
	printf("\nEnter lot number to be checked out : "); // lab, lab1, lab 2 are lables for goto to jump to
	scanf("%d", &data);

	if (data > MAXLOTS || data <= 0)
	{
	lab1:
		printf("Invalid lot number."
				 "\nPress 1 to continue check out\nPress 2 to return back to main menu\nChoice : ");
		goto lab2;
	lab2:
		scanf("%d", &ch);
		if (ch == 1)
		{
			goto lab;
		}
		else if (ch == 2)
		{
			return;
		}
		else
		{
			printf("\nInvalid option\n");
			printf("Press 1 to continue check-out\nPress 2 to return back to main menu\nChoice :");
			goto lab2;
		}
	}
	else
	{
		p = first;
		while (p->lotNumber != data)
		{
			p = p->link;
		}
		if (p->lotStatus == 0)
		{
			printf("\nLot is empty\n");
			printf("Press 1. to continue check out\nPress 2. to return back to main menu\nChoice :");
			goto lab2;
		}
		else
		{
			t = time(NULL) / 60;
			tt = t - (p->time);
			cost = tt * AMTPERMIN + MINAMT;
			g = p;
			createCheckOutLog(cost, tt);
			printf("\nOwner of vehicle "
					 "%d"
					 " has parked for "
					 "%d minutes "
					 "and has to pay "
					 "%d"
					 " rupees.\n\n"
					 "Check-out successful."
					 "\n\n",
					 p->carNumber, tt, cost);
			p->carNumber = p->time = 0;
			p->lotStatus = 0;
			printf("\nPress any key (alphabets or intergers) to continue..");
			scanf(" %c", &key);
			printf("\n\n");
		}
	}
}

void displayLotStatus()
{
	node p, r;
	int c = 1;
	char key;
	p = first;
	r = first;
	while (r != NULL)
	{
		if (r->lotStatus == 0)
		{
			r = r->link;
		}
		else
		{
			c = 0;
			r = r->link;
		}
	}
	if (c == 1)
	{
		printf("\nAll lots are empty.\n\n");
	}
	else
	{
		printf("\nLot\t\tStatus  \t\tVehicle number\n\n");
		while (p != NULL)
		{
			printf(" %d \t\t", p->lotNumber);
			if (p->lotStatus == 0)
			{
				printf("Empty   \t\t"
						 "     ----   \n\n");
				p = p->link;
			}
			else
			{
				printf("Occupied\t\t"
						 "     %d  \n\n",
						 p->carNumber);
				p = p->link;
			}
		}
	}
	printf("\nPress any key (alphabets or numbers) to continue...");
	scanf(" %c", &key);
	printf("\n");
}

int additionalSettings()
{
	int val, i;
	char userChoice;
	char name1[] = "checkout.log", name2[] = "day.log";
	do
	{
		printf("\n");
		for (i = 1; i <= 37; i++)
		{
			printf("*");
		}
		printf("\n*\t\t\t\t    *\n*"
				 "    1.Delete Check-out log file    "
				 "*\n*"
				 "    2.Delete Day log file\t    "
				 "*\n*"
				 "    3.Delete all log files\t    "
				 "*\n*"
				 "    4.Back\t\t\t    "
				 "*\n*\t\t\t\t    *\n");

		for (i = 1; i <= 37; i++)
		{
			printf("*");
		}
		printf("\n\nEnter your choice : ");
		while ((getchar()) != '\n')
			;
		userChoice = getchar(); //reads the buffer till the end and discards them (including newline)

		switch (userChoice)
		{
		case '1':
			val = isFileExists(name1);
			if (val == 1)
			{
				remove(name1);
				printf("\ncheckout.log deleted successfully\n\n");
			}
			else
			{
				printf("\nCheckout.log does not exist\n\n");
				printf("\nReturning back to additional settings\n");
			}
			break;
		case '2':
			val = isFileExists(name2);
			if (val == 1)
			{
				remove(name2);
				printf("\nday.log deleted successfully\n\n");
			}
			else
			{
				printf("\nDay.log does not exist\n\n");
				printf("\nReturning back to additional settings\n");
			}
			break;
		case '3':
			printf("\nLogs deleted\n\n");
			remove(name1);
			remove(name2);
			break;
		case '4':
			break;
		case '5':
			remove(name1);
			remove(name2);
			exit(0);
		case '6':
			createDayLog();
			exit(0);
		default:
			printf("\n\nInvalid choice\n");
		}
	} while (userChoice != '4');
}

void createCheckOutLog(int cost, int tt)
{
	time_t t1;
	time(&t1); // Contains current data and time in UTC
	f = fopen("checkout.log", "a++");

	if (f == NULL)
	{
		printf("\nLog cannot be created\n");
	}
	else
	{

		fprintf(f, "%s\nLot no : %d\nVehicle no : %d\nTotal minutes parked = %d\nAmount paid = %d rupees.\n\n\n", ctime(&t1), g->lotNumber, g->carNumber, tt, cost);
	}
}

void createDayLog()
{
	node p;
	p = first;
	f = fopen("day.log", "a++");
	time_t t;
	time(&t);

	fprintf(f, "%s\n", ctime(&t));
	while (p != NULL)
	{
		if (p->carNumber == 0)
		{
			p = p->link;
		}
		else
		{
			fprintf(f, "Lot no : %d\nVehicle number : %d\n\n\n", p->lotNumber, p->carNumber);
			p = p->link;
		}
	}
	f = fopen(".import.log", "a++");
	p = first;

	while (p != NULL)
	{
		fprintf(f, "%d %d %d %d ", p->lotStatus, p->lotNumber, p->carNumber, p->time);
		p = p->link;
	}
}