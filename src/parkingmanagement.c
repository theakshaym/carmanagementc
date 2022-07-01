// Parking Management using C (Single list method)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

#define MAXLOTS 10	  // Total parking lots
#define MINAMT 5		  // Minimum parking amount
#define AMTPERMIN 0.25 // Amount per minute of parking

int entryPromptAndMenus();
void createEmptyParkingLots();												 // Creates MAXLOTS number of nodes and stores default value in fields i.e 0
int isFileExists(char name[]);												 // checks if .log exists or not in the directory
void getAndStoreImportLog();													 // Reads import-log files and stores it in Linked list
void checkInCar();																 // To Check-in the vehicle
int isVehicleNumberValid(const char *string, const char *pattern); // Regex Pattern Matching for Vehicle Number
int displayOccupiedLots();														 // Display Currently occupied lots for CheckOut Function
void checkOutCar();																 // To Check-out the vehicle
void displayLotStatus();														 // To display current parking lot status
void additionalSettings();														 // For choice 4 in Menu
void createCheckOutLog(int cost, int tt);									 // To create a check-out log file
void createDayLog();																 // Creates a daily log when user exits from program. Also creates import log which is needed to recover data
typedef struct node
{
	int lotStatus;
	char vehicleNumber[15];
	int time; // Stores time in seconds
	int lotNumber;
	struct node *link;
} * node;

node first = NULL;
node g;
FILE *f; // To create logs

int main()
{
	int exitCode = entryPromptAndMenus();
	printf("\nGoodbye!\n\n");
}

int entryPromptAndMenus()
{
	int fileStatus, i, j;
	char userChoice, programExitChoice;
	char filename[] = "import.log";
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
				 "Press Enter/Return to continue..");
		getAndStoreImportLog();
	}
	else
	{
		printf("\n\nNo log file found. Values set to default.\n"
				 "Press Enter/Return to continue..");
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
	if ((f = fopen(name, "r")))
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
	strcpy(first->vehicleNumber, "Empty");
	first->time = 0;
	q = first;

	while (i <= MAXLOTS)
	{
		p = (node)malloc(sizeof(struct node));
		p->lotNumber = i;
		p->time = 0;
		strcpy(first->vehicleNumber, "Empty");
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
	char fileName[] = "import.log";
	node p;
	char vehicleNumber[12];
	p = first;
	f = fopen("import.log", "r");

	while (p != NULL)
	{
		fscanf(f, "%d%d%d %s", &p->lotStatus, &p->lotNumber, &p->time, vehicleNumber);
		strcpy(p->vehicleNumber, vehicleNumber);
		p = p->link;
	}
	fclose(f);
	//remove(fileName);
}

void checkInCar()
{
	int checkInLotNumber, cn;
	char userVehicleNumber[15];
	int continueTheLoop = 1;
	const char *vehicleRegex = "(^[a-zA-Z]+[0-9]+[a-zA-Z]+[0-9]+$)|(^[0-9]+$)";
	char ch;
	node p, q, r;
	do
	{
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
		scanf("%d", &checkInLotNumber);
		while ((getchar()) != '\n')
			;
		if (checkInLotNumber > MAXLOTS || checkInLotNumber <= 0)
		{
			printf("\n\nInvalid lot number\n");
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
		}
		else
		{
			p = first;
			while (p->lotNumber != checkInLotNumber)
			{
				p = p->link;
			}
			if (p->lotStatus == 0)
			{
				printf("Enter vehicle number : ");
				scanf("%s", userVehicleNumber);
				if (isVehicleNumberValid(userVehicleNumber, vehicleRegex))
				{
					strcpy(p->vehicleNumber, userVehicleNumber);
					p->time = time(NULL) / 60;
					p->lotStatus = 1;
					printf("\nCheck-in Successful!\n\n");
					continueTheLoop = 0;
				}
				else
				{
					printf("\nInvalid Vehicle Number. Possible Formats:\n\nKA31F1036\n1036\n\n");
					printf("Press Enter/Return to continue..\n");
					while ((getchar()) != '\n')
						;
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
			}
		}
	} while (continueTheLoop);
}

int isVehicleNumberValid(const char *string, const char *pattern)
{
	regex_t re;
	if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0)
		return 0;
	int status = regexec(&re, string, 0, NULL, 0);
	regfree(&re);
	if (status != 0)
		return 0;
	return 1;
}

int displayOccupiedLots()
{
	node p, r;
	int lotCurrentStatus = 1;
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
			lotCurrentStatus = 0;
			r = r->link;
		}
	}
	if (lotCurrentStatus == 1)
	{
		printf("\nAll lots are empty.\nPress Enter/Return to go back to main menu..");
		while ((getchar()) != '\n')
			;
		return 0;
	}
	else
	{
		printf("\nOccupied Lots:\n");
		printf("\nLot\t\tVehicle number\n\n");
		while (p != NULL)
		{
			if (p->lotStatus == 0)
			{
				p = p->link;
			}
			else if (p->lotStatus == 1)
			{
				printf(" %d \t\t", p->lotNumber);
				printf("%s\n\n",
						 p->vehicleNumber);
				p = p->link;
			}
		}
		return 1;
	}
}

void checkOutCar()
{
	node p;
	int checkOutLotNumber, amountDue, currentTimeInMinutes, totalTimeParked;
	int continueTheLoop = 1;

	do
	{
		if (displayOccupiedLots())
		{
			printf("\nEnter lot number to be checked out : ");
			scanf("%d", &checkOutLotNumber);

			if (checkOutLotNumber > MAXLOTS || checkOutLotNumber <= 0)
			{
				printf("\nInvalid lot number.\nPress Enter/Return to go back to menu..");
				while ((getchar()) != '\n')
					;
				return;
			}
			else
			{
				p = first;
				while (p->lotNumber != checkOutLotNumber)
				{
					p = p->link;
				}
				if (p->lotStatus == 0)
				{
					printf("\nLot is empty\n");
					printf("\nPress Enter/Return to go back to main menu..");
					while ((getchar()) != '\n')
						;
					continueTheLoop = 0;
				}
				else
				{
					currentTimeInMinutes = time(NULL) / 60;
					totalTimeParked = currentTimeInMinutes - (p->time);
					amountDue = totalTimeParked * AMTPERMIN + MINAMT;
					g = p;
					createCheckOutLog(amountDue, totalTimeParked);
					printf("\nOwner of vehicle "
							 "%s"
							 " has parked for "
							 "%d minutes "
							 "and has to pay "
							 "%d"
							 " rupees.\n\n"
							 "Check-out successful."
							 "\n\n",
							 p->vehicleNumber, totalTimeParked, amountDue);
					p->time = 0;
					strcpy(p->vehicleNumber, "Empty");
					p->lotStatus = 0;
					continueTheLoop = 0;
					printf("\nPress Enter/Return to continue..");
					while ((getchar()) != '\n')
						;
				}
			}
		}
		else
		{
			continueTheLoop = 0;
		}
	} while (continueTheLoop);
}

void displayLotStatus()
{
	node p, r;
	int lotCurrentStatus = 1;
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
			lotCurrentStatus = 0;
			r = r->link;
		}
	}
	if (lotCurrentStatus == 1)
	{
		printf("\nAll lots are empty.\n");
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
						 "     %s  \n\n",
						 p->vehicleNumber);
				p = p->link;
			}
		}
	}
	printf("\nPress Enter/Return to go back to main menu..");
	while ((getchar()) != '\n')
		;
}

void additionalSettings()
{
	int fileStatus, i;
	char userChoice;
	char checkoutLog[] = "checkout.log", dayLog[] = "day.log";
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
		userChoice = getchar(); // reads the buffer till the end and discards them (including newline)

		switch (userChoice)
		{
		case '1':
			fileStatus = isFileExists(checkoutLog);
			if (fileStatus == 1)
			{
				remove(checkoutLog);
				printf("\ncheckout.log deleted successfully\n\n");
			}
			else
			{
				printf("\nCheckout.log does not exist\n\n");
				printf("\nReturning back to additional settings\n");
			}
			break;
		case '2':
			fileStatus = isFileExists(dayLog);
			if (fileStatus == 1)
			{
				remove(dayLog);
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
			remove(checkoutLog);
			remove(dayLog);
			break;
		case '4':
			break;
		case '5':
			remove(checkoutLog);
			remove(dayLog);
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

		fprintf(f, "%s\nLot no : %d\nVehicle no : %s\nTotal minutes parked = %d\nAmount paid = %d rupees.\n\n\n", ctime(&t1), g->lotNumber, g->vehicleNumber, tt, cost);
	}
	fclose(f);
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
		if (p->lotStatus == 0)
		{
			p = p->link;
		}
		else
		{
			fprintf(f, "Lot no : %d\nVehicle number : %s\n\n\n", p->lotNumber, p->vehicleNumber);
			p = p->link;
		}
	}
	fclose(f);
	f = fopen("import.log", "w");
	p = first;

	while (p != NULL)
	{
		fprintf(f, "%d %d %d %s ", p->lotStatus, p->lotNumber, p->time, p->vehicleNumber);
		p = p->link;
	}
	fclose(f);
}