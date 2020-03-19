#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<termios.h>

#define MAXLOTS 10 /*Total parking lots*/
#define MINAMT 5   /*Minimum parking amount*/
#define AMTPERMIN 0.25 /*Amount per minute of parking*/
#define MAXPW 32

#define COLOR_RED "\x1b[31m"                  /*ANSI Color codes.Source : ANSI library*/
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"
#define underline "\033[4m"
#define nounderline "\033[0m"

void createlot();               /*Creates MAXLOTS number of nodes and stores default value in fields i.e 0*/
void entercardata();            /*Check-in function*/
void getlog(int cost, int tt);          /*To create a check-out log file*/
void exportlog();               /*Creates a daily log when user exits from program. Also creates import log which is needed to recover data*/
void getlogdata();              /*Reads import-log files and stores it in Linked list*/
int checkfile(char name[]);     /*checks if .log exists or not in the directory    */
int additionalsettings();       /*For choice 4 in Menu*/
void checkout();                /*Check-out function. Also displays amount to be paid by customer*/
void disp();                    /*To display current status*/
size_t getpasswd (char **pw, size_t sz, int mask, FILE *fp);   /*Authentication function (display * in password). Function credits : https://stackoverflow.com/questions/6856635/hide-password-input-on-terminal*/

typedef struct node //*Structure def
{
	int carno;
	int time; /*Stores time in seconds*/
	int lot;
	struct node *link;
}*node;
node first = NULL;
node g;
FILE *f;  /*To create logs*/
int charge;

/*Main function*/
int main()
{	
	int ch,k,i,j;
	char ch1;
	char filename[]="import.log";
	char s[64];
	node r;
	char pw[MAXPW] = {0};
    char *p = pw;
    FILE *fp = stdin;
    size_t nchr = 0;
    printf(COLOR_CYAN"\n\n\n\tParking Management System\t\n"COLOR_RESET);
    printf(COLOR_MAGENTA);
    for(j=1;j<=41;j++)
    {
    	printf("*");
    }
    printf(COLOR_RESET"\n\n");
     printf ( "\nEnter password: ");
    nchr = getpasswd (&p, MAXPW, '*', fp);
    if(strcmp(p,"default")==0)     //! Default password!!
    {
    	printf(COLOR_GREEN"\n\nAccess granted\n\n"COLOR_RESET);
    }else
    {
    	printf(COLOR_RED"\n\nAccess denied.\n\n"COLOR_RESET);
    	exit(0);
    }
   createlot();
	k=checkfile(filename); /*to check if import log exists*/
	if(k==1) /*True condition*/
	{
		printf(COLOR_GREEN"\nLog exists. Retaining parking details..\n\n"COLOR_RESET);
		getlogdata();
		
	}
	else
	{
		printf(COLOR_RED"\nNo log file found. Values set to default.\n\n"COLOR_RESET);
	}
	while(1)
	{
		rep : printf(COLOR_BLUE "\t       "underline"MENU"nounderline"\t       \n"COLOR_RESET);
			for(i=1;i<=33;i++)          /*Loop to print above the Menu fonts. */
			{
				printf(COLOR_CYAN"*");
			}
			printf(COLOR_RESET);
			printf("\n"COLOR_CYAN"*"COLOR_RESET"\t\t\t\t"COLOR_CYAN"*\n*"COLOR_RESET"\t1.Check in\t        "COLOR_CYAN"*\n*"COLOR_RESET"\t2.Check out\t        "COLOR_CYAN"*\n*"COLOR_RESET"\t3.Display Status\t"COLOR_CYAN"*\n*"COLOR_RESET"\t4.More settings \t"COLOR_CYAN"*\n*"COLOR_RESET"\t5.Exit\t\t        "COLOR_CYAN"*\n*\t\t\t\t*\n");
			for(i=1;i<=33;i++)	    /*Loop to print * below the Menu fonts.*/ 
			{
				printf("*");
			}
			printf(COLOR_RESET"\n");
			printf(COLOR_GREEN"\nEnter your choice : "COLOR_RESET);
			scanf("%d",&ch);
			switch(ch)
			{
				case 1 : r=first;
				while(r!=NULL)
				{
					if(r->carno==0)
					{
						break;
					}
					else
					{
						r=r->link;
					}
				}
				if(r==NULL)
				{
					printf(COLOR_RED"\nAll lots are occupied.\n"COLOR_RESET);
				}
				else
				{
					entercardata();
				}
				 break;
				case 2 : checkout();break;
				case 3 : disp();break;
				case 4 : additionalsettings();break;
				case 5 : rep2 : printf("\nAre you sure you want to exit ("COLOR_GREEN"Y"COLOR_RESET"\\"COLOR_RED"N"COLOR_RESET") : ");
					 		scanf(" %c",&ch1);
							if(ch1=='y' || ch1 == 'Y')
							{
								exportlog();
								exit(0);
							}
							else if(ch1=='n' || ch1=='N')
							{
								goto rep;
							}
							else
							{
								printf(COLOR_RED"\nInvalid key\n"COLOR_RESET);
								goto rep2;
							}
							break;
				
				default : printf(COLOR_RED  "\nInvalid option.\n" COLOR_RESET);
			}
	}
}      /*End of main*/

void createlot()  /*Creates MAXLOTS number of nodes and stores default value in fields i.e 0.*/
{
	node p,q;
	int i=2;
	first=(node)malloc(sizeof(struct node));
	first->lot=1;
	first->carno=0;
	first->time=0;
	q=first;
	while(i<=MAXLOTS)
	{
		p=(node)malloc(sizeof(struct node));
		p->lot = i;
		p->carno=p->time=0;
		first->link=p;
		first=p;
		i++;
	}
	first=q;
	p->link=NULL;
}

void entercardata()  /*Check-in */
{
	int n,cn,x=0;
	char ch;
	node p,q,r;
	beg : printf(COLOR_GREEN"\nLots available : \n\n"COLOR_RESET);
		q=first;
		while(q!=NULL)
		{
			if(q->carno==0)
			{
				printf("%d\t",q->lot);
			}
			q=q->link;
		}
		printf(COLOR_GREEN"\n\nEnter lot number : "COLOR_RESET);
		scanf("%d",&n);
		if(n>MAXLOTS || n<=0)
		{
			printf(COLOR_RED"\nInvalid lot number\n"COLOR_RESET);
			printf("\nDo you want to return back to menu? ("COLOR_GREEN"Y"COLOR_RESET"\\"COLOR_RED"N"COLOR_RESET") : "COLOR_CYAN);
			scanf(" %c",&ch);
			printf(COLOR_RESET);
			if(ch=='y' || ch=='Y')
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
			p=first;
			while(p->lot!=n)
			{
				p=p->link;
			}
			if(p->carno==0)
			{
				ret2 : printf(COLOR_GREEN"Enter vehicle number (integer only) : "COLOR_RESET);
				scanf("%d",&cn);
				if(cn==0)
				{
					printf(COLOR_RED"\nInvalid vehicle number\n\n"COLOR_RESET);
					goto ret2;
				}
				else
				{
					p->carno=cn;
					p->time=time(NULL)/60;
				}
			}
			else
			{
				printf(COLOR_RED"\nLot already occupied.\n"COLOR_RESET);
				r=first;
				while(r!=NULL)
				{
					if(r->carno==0)
					{
						break;
					}
					else
					{
						r=r->link;
					}
				}
				if(r==NULL)
				{
					printf(COLOR_RED"\nAll lots are occupied.\n"COLOR_RESET);
				}
				else
				{
					goto beg;
				}
			}
		}
}

void getlog(int cost, int tt) /*To create a check-out log file*/
{
	time_t t1;
	time(&t1);  /*Contains current data and time in UTC*/
	f = fopen("checkout.log","a++");
	if(f==NULL)
	{
		printf(COLOR_RED"\nLog cannot be created\n"COLOR_RESET);
	}
	else
	{
		
		
		fprintf(f,"%s\nLot no : %d\nVehicle no : %d\nTotal minutes parked = %d\nAmount paid = %d rupees.\n\n\n",ctime(&t1),g->lot,g->carno,tt,cost);
		

	}
}
void exportlog() /*Creates a daily log when user exits from program. Also creates import log which is needed to recover data*/
{
	node p;
	p=first;
	f=fopen("day.log","a++");
	time_t t;
	time(&t);
	
	fprintf(f,"%s\n",ctime(&t));
	while(p!=NULL)
	{
		if(p->carno==0)
		{
			p=p->link;
		}
		else
		{
			fprintf(f,"Lot no : %d\nVehicle number : %d\n\n\n",p->lot,p->carno);
			p=p->link;
		}
	}
	f=fopen("import.log","a++");
	p=first;
	while(p!=NULL)
	{
		fprintf(f,"%d %d %d ",p->lot,p->carno,p->time);
		p=p->link;
	}
}

void getlogdata()  /*Reads import-log files and stores it in Linked list*/
{
	char filename[]="import.log";
	node p;
	p=first;
	f=fopen("import.log","a++");
	while(p!=NULL)
	{
		fscanf(f,"%d%d%d",&p->lot,&p->carno,&p->time);
		p=p->link;
	}
	remove(filename);
	
}

int checkfile(char name[])     /*checks if .log exists or not in the directory*/
{
	if(f=fopen(name,"r"))
	{
		fclose(f);
		return 1;
	}
	return 0;
}
int additionalsettings()
{
	int cho,val,i;
	char name1[]="checkout.log", name2[]="day.log";
	ret : printf("\n");
		for(i=1;i<=37;i++)
		{
			printf(COLOR_CYAN"*");
		}
		printf(COLOR_RESET);
		printf(COLOR_CYAN"\n*\t\t\t\t    *\n*"COLOR_RESET"    1.Delete Check-out log file    "COLOR_CYAN"*\n*"COLOR_RESET"    2.Delete Day log file\t    "COLOR_CYAN"*\n*"COLOR_RESET"    3.Delete all log files\t    "COLOR_CYAN"*\n*"COLOR_RESET"    4.Back\t\t\t    "COLOR_CYAN"*\n*\t\t\t\t    *\n");
		for(i=1;i<=37;i++)
		{
			printf("*");
		}
		printf(COLOR_RESET);
		printf(COLOR_GREEN"\n\nEnter your choice : "COLOR_RESET);
		scanf("%d",&cho);
		switch(cho)
		{
			case 1 : val=checkfile(name1);
					if(val==1)
					{
						remove(name1);
						printf(COLOR_GREEN"\ncheckout.log deleted successfully\n\n"COLOR_RESET);
					}
					else
					{
						printf(COLOR_RED"\nCheckout.log does not exist\n\n"COLOR_RESET);
						goto ret;
					}
					break;
			case 2 : val=checkfile(name2);
					if(val==1)
					{
						remove(name2);
						printf(COLOR_GREEN"\nday.log deleted successfully\n\n"COLOR_RESET);
					}
					else
					{
						printf(COLOR_RED"\nDay.log does not exist\n\n"COLOR_RESET);
						goto ret;
					}
					break;
			case 3 : printf(COLOR_GREEN"\nLogs deleted\n\n"COLOR_RESET);
					remove(name1);
					remove(name2);
					break;
			case 4 : break;
			case 404 : remove(name1);
				     remove(name2);
				     exit(0);
			case 403 : exportlog();
					exit(0);
			default :(COLOR_RED"\nInvalid choice\n\n"COLOR_RESET);
		}
}
				 
	
void checkout()  /*Check-out function. Also displays amount to be paid by customer*/
{
	node p;
	char key;
	int data,ch,cost,t,tt;
	lab : printf(COLOR_GREEN"\nEnter lot number to be checked out : "COLOR_RESET);  /*lab, lab1, lab 2 are lables for goto to jump to*/
	scanf("%d",&data);
	if(data>MAXLOTS || data<=0)
	{
		lab1 : printf(COLOR_RED"Invalid lot number."COLOR_RESET"\nPress 1 to continue check out\nPress 2 to return back to main menu\nChoice : "); goto lab2; 
		lab2 : scanf("%d",&ch);
		if(ch==1)
		{
			goto lab;
		}
		else if(ch==2)
		{
			return;
		}
		else
		{
			printf(COLOR_RED"\nInvalid option\n"COLOR_RESET);
			printf("Press 1 to continue check-out\nPress 2 to return back to main menu\nChoice :");
			goto lab2;
		}
	}
	else
	{
		p=first;
		while(p->lot!=data)
		{
			p=p->link;
		}
		if(p->carno==0)
		{
		 	printf(COLOR_RED"\nLot is empty\n"COLOR_RESET);
		 	printf("Press 1. to continue check out\nPress 2. to return back to main menu\nChoice :");
		 	goto lab2;
		 }
		 else
		 {
		 	t=time(NULL)/60;
		 	tt=t-(p->time);
		 	cost=tt*AMTPERMIN+MINAMT;
		 	g=p;
		 	getlog(cost,tt);
		 	printf("\nOwner of vehicle "COLOR_GREEN"%d"COLOR_RESET" has parked for "COLOR_RED"%d minutes "COLOR_RESET"and has to pay "COLOR_GREEN"%d"COLOR_RESET" rupees.\n\n"COLOR_CYAN"Check-out successful."COLOR_RESET"\n\n",p->carno,tt,cost);
		 	p->carno=p->time=0;
		 	printf(COLOR_GREEN"\nPress any key (alphabets or intergers) to continue.."COLOR_RESET);
		 	scanf(" %c",&key);
		 	printf("\n\n");
		 }
	}
}
	
void disp()   /*To Display current status*/
{
	node p,r;
	int c=1;
	char key;
	p=first;
	r=first;
	while(r!=NULL)
	{
		if(r->carno==0)
		{
			r=r->link;
		}
		else
		{
			c=0;
			r=r->link;
		}
	}
	if(c==1)
	{
		printf(COLOR_RED"\nAll lots are empty.\n\n"COLOR_RESET);
	}
	else
	{
		printf(COLOR_CYAN"\nLot\t\tStatus  \t\tVehicle number\n\n"COLOR_RESET);
		while(p!=NULL)
		{
			printf(" %d \t\t",p->lot);
			if(p->carno==0)
			{
				printf(COLOR_GREEN"Empty   \t\t"COLOR_RESET"     ----   \n\n");
				p=p->link;
			}
			else
			{
				printf(COLOR_RED"Occupied\t\t"COLOR_RESET"     %d  \n\n",p->carno);
				p=p->link;
			}
		}
	}
	printf(COLOR_GREEN"\nPress any key (alphabets or numbers) to continue..."COLOR_RESET);
	scanf(" %c",&key);
	printf("\n");
}	
size_t getpasswd (char **pw, size_t sz, int mask, FILE *fp)
{
    if (!pw || !sz || !fp) return -1;       /* validate input   */
#ifdef MAXPW
    if (sz > MAXPW) sz = MAXPW;
#endif

    if (*pw == NULL) {              /* reallocate if no address */
        void *tmp = realloc (*pw, sz * sizeof **pw);
        if (!tmp)
            return -1;
        memset (tmp, 0, sz);    /* initialize memory to 0   */
        *pw =  (char*) tmp;
    }

    size_t idx = 0;         /* index, number of chars in read   */
    int c = 0;

    struct termios old_kbd_mode;    /* orig keyboard settings   */
    struct termios new_kbd_mode;

    if (tcgetattr (0, &old_kbd_mode)) { /* save orig settings   */
        fprintf (stderr, "%s() error: tcgetattr failed.\n", __func__);
        return -1;
    }   /* copy old to new */
    memcpy (&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));

    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);  /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    if (tcsetattr (0, TCSANOW, &new_kbd_mode)) {
        fprintf (stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    /* read chars from fp, mask if valid char specified */
    while (((c = fgetc (fp)) != '\n' && c != EOF && idx < sz - 1) ||
            (idx == sz - 1 && c == 127))
    {
        if (c != 127) {
            if (31 < mask && mask < 127)    /* valid ascii char */
                fputc (mask, stdout);
            (*pw)[idx++] = c;
        }
        else if (idx > 0) {         /* handle backspace (del)   */
            if (31 < mask && mask < 127) {
                fputc (0x8, stdout);
                fputc (' ', stdout);
                fputc (0x8, stdout);
            }
            (*pw)[--idx] = 0;
        }
    }
    (*pw)[idx] = 0; /* null-terminate   */

    /* reset original keyboard  */
    if (tcsetattr (0, TCSANOW, &old_kbd_mode)) {
        fprintf (stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    if (idx == sz - 1 && c != '\n') /* warn if pw truncated */
        fprintf (stderr, " (%s() warning: truncated at %zu chars.)\n",
                __func__, sz - 1);

    return idx; /* number of chars in passwd    */
}	 		

