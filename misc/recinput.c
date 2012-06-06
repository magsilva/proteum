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


#include        <sys/types.h>
#include        <sys/wait.h>
#include        <sys/ioctl.h>
#include	<stdlib.h>


#ifndef		LINUX
#include        <sys/filio.h>
#endif

#include        <lib/gerais.h>


#define         CODE_ERROR      (WORD) (-2)


main(int argc, char *argv[])
{
char	*exec, *param;
FILE 	*out;

   if (argc < 3)
   {
	msg("Insuficient Parameters");
	msg("Usage: recinput exec-filename param [out-filename]");
	exit(1);
   }

   param = argv[2];

   exec = argv[1];

   if (argc >3 )
	out = criarq("", argv[3], "", 1); 
   else
	out = stdout;

   if (out == NULL)
   {
	msg("Error creating output file");
	exit(2);
   }

   

   if (grab_input(exec, param, out) == ERRO)
	exit(3);

   fecharq(out);

   exit(0);
}

msg(char *x)
{
        fprintf(stderr, "\nrecinput: %s\n", x);
}


/*************************************************************************
GRAB_INPUT:
	This function executes a program, grabs its input and write
it to a file.

Parameters:

char	*exec:	Name of the file to be executed
char	*param:	Parameters of the execution
FILE	*fp:	File where to write the input
*************************************************************************/
grab_input(char	*exec, char *param, FILE *fp)
{
int	pipe_io[2][2];
int	pid, i, ret, c, bytes;
static	char buf[1024];
char *p;

   sprintf(buf, "%s %s", exec, param);
   pipe(pipe_io[0]);
   pipe(pipe_io[1]);

   switch (pid = fork())
       {
        case -1:
            msg("********* Execution Error. ***********");
            return ERRO;
        case 0:
            dup2(pipe_io[0][0], 0);
            for (i = maxfile(); i > 2; i--)
                close(i);
            execl(SHELL, SHELL, "-c", buf, NULL);
            return ERRO;

        default:
             while ( (i = waitpid(pid, &c, WNOHANG)) != pid)
                {
                   if ( (i = ioctl(0,FIONREAD,&bytes)) != -1 && bytes > 0)
                           {
                               bytes = read(0, buf,  bytes);
                            }
                     else  bytes = 0;

                   if (i < 0)
                        close(pipe_io[0][1]);
                   else
                   {
                        write(pipe_io[0][1], buf, bytes);
                        fwrite(buf, 1, bytes, fp);
                    }
                }

             ret = c;
             if ( (ret = trata_retcode(c, pid)) == ERRO)
                     {
                        return ERRO;
                     }
        }
   close(pipe_io[0][0]);
   close(pipe_io[0][1]);
   close(pipe_io[1][0]);
   close(pipe_io[1][1]);


/* ------------descobre tamanho da entrada--------- */
   fflush(fp);
   return OK;
}



trata_retcode(x, pid)
int     x, pid;
{
static char     *tab_signal[] = {
                "",
                "hangup",
                "interrupt",
                "quit",
                "illegal instruction",
                "trace trap ",
                "abort",
                "EMT instruction",
                "arithmetic exception",
                "kill",
                "bus error",
                "segmentation violation",
                "bad argument to system call",
                "write on a pipe or other socket with no one to read it",
                "alarm clock",
                "software termination signal",
                "urgent condition present on IO channel",
                "stop",
                "stop signal generated from keyboard",
                "continue after stop",
                "child status has changed",
                "background read attempted from control terminal",
                "background write attempted to control terminal",
                "I/O is possible ",
                "cpu time limit exceeded ",
                "file size limit exceeded ",
                "virtual time alarm ",
                "profiling timer alarm",
                "window changed",
                "resource lost"
                                };
int     r;


   if (WIFSTOPPED(x))
             {
                r = WSTOPSIG(x);
                printf("\n\n(Stoped): Code %d\n", r);
                fflush(stdout);
                kill(pid, 9);
                return ERRO;

              }
        else
   if (WIFSIGNALED(x))
             {
                r = WTERMSIG(x);
                printf("\n\n(Aborted: Code %d)\n", r);

                if (r < LEN(tab_signal))
                        printf("(%s)\n", tab_signal[r]);
                fflush(stdout);
                return CODE_ERROR;

              }
        else
   if (WIFEXITED(x))
             {
                r = WEXITSTATUS(x);
                return r;

              }
        else  return ERRO;
}


 

