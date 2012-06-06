/* Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado
# 
# 
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
# 
*/


#include	"btree.h"


main1()
{
BTREE   *bt;
int     cmp_long(), k;
void    prlong();
long    l;
FILE    *fp;
struct	{long l; char buf[200];}  reg;

   if ((fp = fopen("file1.dat", "r+")) != NULL)
   {
        fclose(fp);
        bt = BTREE_open("file1.dat", NULL, sizeof(long), cmp_long, prlong);
   }
   else
        bt = BTREE_create("file1.dat", NULL , sizeof(long), cmp_long, prlong);

   if (bt == NULL)
   {
        printf("Error creating BTREE");
   }

   do {
	printf("\nKey: ");
	scanf("%ld", &(reg.l));
	printf("\nName: ");
	gets(reg.buf);
        gets(reg.buf);
	if (BTREE_insert_data(bt, &(reg.l), &reg, 
		sizeof(reg.l)+strlen(reg.buf)+1) == -1)
	{
	   printf("\nKEY ALREADY EXISTS\n");
	}
   } while (reg.l != 0);

   do {
        printf("\nKey: ");
        scanf("%ld", &l);
        if (BTREE_get_data(bt, &l, &reg, &k) < 0) 
	if (k < 0)
	{
	   printf("\nKey does not exist\n");
	}
	else
	{
	   printf("\nRETREIVED-------\nKey: %ld\nName: %s\n", reg.l, reg.buf);
	}
   } while (l != 0);
}

main()
{
BTREE	*bt;
int	cmp_long(), k;
void	prlong();
long	l;
FILE	*fp;

   if ((fp = fopen("file2.dat", "r+")) != NULL)
   {
	fclose(fp);
	bt = BTREE_open("file2.dat", NULL, sizeof(long), cmp_long, prlong);
   }
   else
        bt = BTREE_create("file2.dat", NULL, sizeof(long), cmp_long, prlong);

   if (bt == NULL)
   {
	printf("Error creating BTREE");
   }
   BTREE_close(bt);

   bt = BTREE_open("file2.dat", NULL, sizeof(long), cmp_long, prlong);
   if (bt == NULL)
   {
        printf("Error opening BTREE");
   } 
   do
   {
	printf("\nNext key: ");
	scanf("%ld", &l);
	if ( l >= 0)
	{
          BTREE_seq_key(bt, &l, &k);
          printf("\nSequence number: %d", k);
   	  if (BTREE_insert_key(bt, &l, 0, 1) == -1)
	      printf("\nKEY ALREADY EXISTS\n");
	  BTREE_seq_key(bt, &l, &k);
	  printf("\nSequence number: %d", k);
	}
	else
	{
	  l *= -1;
	  if (BTREE_delete_key(bt, &l) == -1)
              printf("\nKEY ALREADY DELETED\n");
        }

   	BTREE_print(bt); 
/*	BTREE_print_nth(bt); */
   } while (l != 0);
   BTREE_close(bt);

}

cmp_long(long *x, long *y)
{
   return ( (*x < *y) ? -1 :
	  ( (*y < *x) ? 1  : 0));
}

void prlong(long *x)
{
   printf("%ld ", *x);
}
 
msg(char *x)
{
   printf("\n%s\n", x);
}
