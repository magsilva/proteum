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


#include	<stdio.h>
#include        <sys/wait.h>
#include 	<unistd.h>

#include        <sys/time.h>
#include        <sys/times.h>
#include	<stdlib.h>

struct  itimerval       t1;


main(int argc, char *argv[])
{
int	ret_code, pid;

    pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "\nUnable to fork\n");
	return 2;
    }

    if (pid == 0)
    {
        t1.it_value.tv_sec = atoi(argv[1]);
        t1.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &t1, NULL);
        execv(argv[2], &argv[2]);
        fprintf(stderr, "\nCan not execv\n");
        exit(2);
    }

    pid = wait(&ret_code);
    if ( WIFEXITED(ret_code) )
	return 0;
    return 1;
}


   
