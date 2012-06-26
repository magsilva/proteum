#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*  dayw = "Do Se Te Qa Qi Se Sa";

static char*  smon[]= {
  "Janeiro", "Fevereiro", "Março", "Abril",
  "Maio", "Junho", "Julho", "Agosto",
  "Setembro", "Outubro", "Novembro", "Dezembro",
};

char* cal(int , int );

int main(int argc, char* argv[])
{
  int y;
  int m;
  char* s;

  if(argc >= 3 || argc == 1)
  {
    /*
     * print out just month
     */
    if(argc == 1) {      /* current month */
	time_t t;
	struct tm *tm;
	t = time(0);
	tm = (struct tm *) localtime(&t);
	m = tm->tm_mon + 1;
	y = tm->tm_year + 1900;
    } else {
      m = atoi(argv[1]);
      if(m<1 || m>12) {
        printf("Cal: %s: mes invalido.\n", argv[0]);
        exit(1);
      }
      y = atoi(argv[2]);
      if(y<1 || y>9999) {
        printf("Cal: %s: ano invalido.\n", argv[1]);
        exit(2);
      }
    }
    printf("   %s %d\n", smon[m-1], y);
    printf("%s\n", dayw);
    s = cal(firstOfMonth(m,y),numberOfDays(m,y) );
    printf("%s\n\n", s);
  }
  else
  {
    int zi;
    /*
     * print out complete year
     */
    y = atoi(argv[1]);
    if(y<1 || y>9999) {
      printf("Cal: %s: ano invalido.\n", argv[0]);
      exit(2);
    }
    for (zi = 1; zi < 13; zi++)
    {
      printf("   %s %d\n", smon[zi-1], y);
      printf("%s\n", dayw);
      s = cal(firstOfMonth(zi,y),numberOfDays(zi,y) );
      printf("%s\n\n", s);
    }
  }
  return 0;
}


static int  mon[] = {
  0,
  31, 28, 31, 30,
  31, 30, 31, 31,
  30, 31, 30, 31,
};


int firstOfMonth(int m, int a)
{
   int k = 0, i;
   if (isLeap(a) && m > 2)
      k++;
   for (i = 0; i < m; i++)
      k += mon[i];
   if ( a == 1752 && m > 9 )
      k -= 11;
   k %= 7;
   k = (k + jan1(a)) % 7;
   return k;
}

int numberOfDays(int m, int a)
{
   if (isLeap(a) && m == 2)
      return 29;
   if ( a == 1752 && m == 9 )
      return 19;
   return mon[m];
}



int isLeap(int a)
{
  if ( a <= 1752 )
  {
      if ( a % 4 == 0 )
        return 1;
      else
        return 0;
  }
  else
  {
      if ( a % 400 == 0 )
        return 1;
      if ( a % 100 == 0 )
        return 0;
      if ( a % 4 == 0 )
        return 1;
  }
  return 0;
}


char* cal(int dds, int n)
{
  int k, cont;
  char* s = malloc(1024), q[10];
  if ( n == 19 )
      return "       1  2 14 15 16\n17 18 19 20 21 22 23\n24 25 26 27 28 29 30";
  s[0] = '\0';
  for (k = 0; k < dds; k++)
      strcat(s, "   ");
  cont = dds+1;
  for (k = 1; k <= n; k++, cont++)
  {
      if ( k < 10 ) strcat(s, " ");
      sprintf(q, "%d", k);
      strcat(s, q);
      if ( cont % 7 == 0)
          strcat(s, "\n");
      else
          strcat(s, " ");
  }
  return s;
}



/*
 *  return day of the week
 *  of jan 1 of given year
 */

int jan1(int yr)
{
  int y, d;

/*
 *  normal gregorian calendar
 *  one extra day per four years
 */

  y = yr;
  d = 4+y+(y+3)/4;

/*
 *  julian calendar
 *  regular gregorian
 *  less three days per 400
 */

  if(y > 1800) {
    d -= (y-1701)/100;
    d += (y-1601)/400;
  }

/*
 *  great calendar changeover instant
 */

  if(y > 1752)
    d += 3;

  return(d%7);
}

