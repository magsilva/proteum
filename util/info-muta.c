
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>




#define		STR_NMUTA	"MUTANT #"
#define		STR_EQUIV	"\tStatus Equivalent Active"
#define		STR_EQUIVALL	"\tStatus Equivalent"
#define         STR_ANOM	"\tStatus Anomalous Active"
#define         STR_ANOMALL     "\tStatus Anomalous"
#define         STR_ALIVE	"\tStatus Alive Active"
#define         STR_ALIVEINACTIVE 	"\tStatus Alive Inactive"
#define         STR_ALIVEALL    "\tStatus Alive"
#define         STR_DEAD	"\tStatus Dead Active"
#define         STR_DEADALL    	"\tStatus Dead"

#define		EQUIV		1
#define		ALIVE		2
#define		DEAD		4
#define		ANOM		8
#define		ACTIVE		16
#define		INACTIVE	32

int main(argc, argv)
int	argc;
char	*argv[];
{
int	i, ntcase, l, param;
char	buf[300];
FILE	*fp;
char	*p, *q, *sep = " ";

   param = 0;
   for (i = 1; i < argc; i++)
   {
	if (strcmp(argv[i], "-equiv") == 0)
		param |= EQUIV;
	else

        if (strcmp(argv[i], "-alive") == 0)
                param |= ALIVE;
        else
        if (strcmp(argv[i], "-dead") == 0)
                param |= DEAD;
        else
        if (strcmp(argv[i], "-anomalous") == 0)
                param |= ANOM;
        else
        if (strcmp(argv[i], "-active") == 0)
                param |= ACTIVE;
	else 
        if (strcmp(argv[i], "-inactive") == 0)
                param |= INACTIVE;
	else
        if (strcmp(argv[i], "-n") == 0)
                sep = "\n";
	else
		goto l1;
   }

   switch (param)
   {
	case 0:
	case EQUIV:
	   q = STR_EQUIVALL;
	   break;
	case EQUIV | ACTIVE:
	case ACTIVE:
	   q = STR_EQUIV;
	   break;
        case ALIVE:
           q = STR_ALIVEALL;
           break;
        case ALIVE | ACTIVE:
           q = STR_ALIVE;
           break;
        case DEAD:
           q = STR_DEADALL;
           break;
        case DEAD | ACTIVE:
           q = STR_DEAD;
           break;
        case ANOM:
           q = STR_ANOMALL;
           break;
        case ANOM | ACTIVE:
           q = STR_ANOM;
           break;
	case ALIVE | INACTIVE:
	   q = STR_ALIVEINACTIVE;
	   break;
	default:
	   l1:
	   fprintf(stderr, "\nInvalid parameters\n");
	   exit(1);
   }

   fp = stdin;
   l = 0;
   
   do {
		if (feof(fp))
			break;
		p = fgets(buf, 100, fp);
		if (strncmp(buf, STR_NMUTA, strlen(STR_NMUTA)) != 0)
		   continue;
		sscanf( &buf[strlen(STR_NMUTA)], "%d", &ntcase);

		p = fgets(buf, 100, fp);
	//	printf("\n%s %d", buf, strlen(buf));
	//	printf("\n%s %d\n", q, strlen(q));
		if (strncmp(buf, q, strlen(q)) != 0)
		     continue;
		l++;
		printf("%d%s", ntcase, sep);

	} while (1);
	fprintf(stderr, "\n%d mutants\n----------------------\n\n\n", l);
	return 0;

}
   


