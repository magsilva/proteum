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


#include	<sys/types.h>
#include	<sys/wait.h>
#include        <sys/stat.h>
#include	<sys/time.h>
#include	<sys/times.h>
#include        <fcntl.h>
#include	<signal.h>
#include	<termios.h>
#include	<sys/ioctl.h>	 
#include	<sys/resource.h>
#include	<unistd.h>
#ifdef LINUX
#	include	<sys/poll.h>
#	include <errno.h>
#	include	<grp.h>
#	define	INFTIM	-1
#endif
#include	<stdio.h>
#include	<errno.h>


#include	<lib/gerais.h>

#define	BUFFSIZE	512  /* if this change, change exemuta.c */


extern char		slave_name[20];
extern FILE 		*input;
char		buff[BUFFSIZE];


int	glob_fdm = -1;
char	glob_name[20];

int aloca_master()
{
   if ( (glob_fdm = ptym_open(glob_name)) < 0)
   {
	 msg("can't open master pty");
	 return ERRO;
   }
   return OK;
}

void libera_master()
{
   close(glob_fdm);
   glob_fdm = -1;
}


static void sig_hup(int s)
{
   printf("\nRecebido SIGHUP: pid %d\n", getpid());
   return;
}
   

pid_t
pty_fork(int *ptrfdm, int pipe_out[], int pipe_err[])
{
	int		fdm, fds, k;
	pid_t	pid;
	char	pts_name[20];

/*/        fdm = glob_fdm;
//        strcpy(pts_name, glob_name); */
        
	if ( (fdm = ptym_open(pts_name)) < 0)
	{
	    msg("can't open master pty");
	    return ERRO;
	}

	strcpy(slave_name, pts_name);	/* return name of slave */

        /* /printf("\npty_fork: %d %s", fdm, pts_name); */
	pipe(pipe_out);
	pipe(pipe_err);

	if ( (pid = fork()) < 0)
	{   
	    msg("Can not fork");
	    return(ERRO);
 	}

	else if (pid == 0) {		/* child */
		/* /sleep(30); */
		if (setsid() < 0)
		{
		   msg("setsid error");
		   exit(1);
		}

		/* SVR4 acquires controlling terminal on open() */
		if ( (fds = ptys_open(fdm, pts_name)) < 0)
		{
		    printf("%d %s" , fdm, pts_name);
		    msg("can't open slave pty");
		    exit(1);
		}
		close(fdm);		/* all done with master in child */
		/* /printf("\nfdm: %d fds: %d\n", fdm, fds);fflush(stdin); */

#ifdef	LINUX
				/* 44BSD way to acquire controlling terminal */
    		if (ioctl(fds, TIOCSCTTY, (char *) 0) < 0)
    		{
		    msg("TIOCSCTTY error");
	  	    return ERRO;
   		}
#endif
		if (tty_copy(fds) == ERRO) /* set slave terminal mode */
		{
		    msg("tty_copy error");
		    perror(" ");
		    exit(1);
		}

		if (dup2(pipe_out[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
		    msg("dup2 error to stdout");
		    exit(1);
		}
		if (dup2(pipe_err[1], STDERR_FILENO) != STDERR_FILENO)
		{
		    msg("dup2 error to stderr");
		    exit(1);
		} 
		if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
		{
		    msg("dup2 error to stdin");
		    exit(1);
		}
		if (pipe_out[1] > STDERR_FILENO)
			close(pipe_out[1]);
		 if (pipe_err[1] > STDERR_FILENO)
			close(pipe_err[1]);
		if (fds > STDERR_FILENO)
		{
			 close(fds);
		}
		close(pipe_out[0]);
		close(pipe_err[0]);
		return 0;		/* child returns 0 just like fork() */

	} else {					/* parent */
		*ptrfdm = fdm;	/* return fd of master */
		close(pipe_out[1]);
		close(pipe_err[1]);
		return(pid);	/* parent returns pid of child */
	}
}

int playinput(int *ret_code, FILE *fp_in, int (*tr_in)(), FILE *fp_out, int (*tr_out)(), FILE *fp_err, int (*tr_err)(), long vtimer, char *prog, char *argv[])
{
	int	i, c, bytes, ret1, k, j;
	struct  itimerval	t1;
	long	tt2, tt1;
	struct	rlimit	resource_limit;
	int	fim_le;
	int	pipe_out[2], pipe_err[2], fdm;
	pid_t pid, pipe_fork();


	pid = pty_fork(&fdm, pipe_out, pipe_err);

   if (pid < 0)
      return ERRO;
   if ( pid == 0 ) /* child */
   {
 	resource_limit.rlim_cur = resource_limit.rlim_max = 0;	    
	setrlimit(RLIMIT_CORE, &resource_limit); /* tamanho maximo para arquivo
						CORE = 0 bytes */
 	if (vtimer != 0)
	{
           tick2timeval(vtimer, &t1.it_value); 
	   t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	   t1.it_interval.tv_usec = t1.it_value.tv_usec;
	   signal(SIGVTALRM, SIG_DFL);  
	   setitimer(ITIMER_VIRTUAL, &t1, NULL); 
	} 
	 execv(prog,argv ); 
	 return ERRO;
    }
    else
    {
	 tt1 = gettimedad();
	 fim_le = FALSE;
	 c = 0;
	 while ( waitpid(pid, &c, WNOHANG) != pid )
	 {
	    bytes = 0;

            if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_out[0],buff,BUFFSIZE);
            else  bytes = 0;

           if (tr_out != NULL && (k = tr_out(fp_out, buff, bytes,pid)) != bytes)
	    {
		if (k != ERRO) k = OK;
                goto fim;
	    }


            bytes = 0;
            if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_err[0],buff,BUFFSIZE);
            else  bytes = 0;

            if (tr_err != NULL && (k = tr_err(fp_err, buff, bytes, pid)) != bytes)
	    {
                 if (k != ERRO) k = OK;
		 goto fim;
	    }

            if ( ! fim_le)
            {
                   bytes = tr_in(buff, BUFFSIZE);

                   if (bytes < 0)
		   {
			k = ERRO;
			goto fim;
		   }
		   else
		   if (bytes == 0)
                   {
                       fim_le = TRUE;
                      /* / close(fdm); */
                    }
                    else
                    {
                        int t;
                           t = bytes;
                           bytes = writen(fdm, buff, bytes);
                           if (bytes < 0)
                           {
                                msg("Error writing to master");
				k = ERRO;
				goto fim;
			   }
                    }

              }
             }
          do {

                   if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_out[0],buff,BUFFSIZE);
                     else  bytes = 0;

                   if (tr_out != NULL && (k =tr_out(fp_out, buff, bytes, pid)) != bytes)
		   {
                        if (k != ERRO) k = OK;
			goto fim;
		   }
              } while (bytes > 0);

          do {

                   if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_err[0],buff,BUFFSIZE);
                     else  bytes = 0;

                   if (tr_err != NULL && (k = tr_err(fp_err, buff, bytes, pid))!=bytes)
		   {
                        if (k != ERRO) k = OK;
			goto fim;
		   }
               } while (bytes > 0);

    }
   k = OK;

fim:
   wait(NULL);

   close(pipe_out[0]);
   close(pipe_err[0]);
   close(fdm);
   if (ret_code != NULL) 
      *ret_code = c;

   return k;

}



/*-------------------------------------------------------------------------
Here starts code for non interactive test case replay */

pid_t
pipe_fork(int pipe_in[], int pipe_out[], int pipe_err[])
{
	pid_t	pid;

	if (pipe(pipe_in) == -1) {
		return ERRO;
	}
	if (pipe(pipe_out) == -1) {
		close(pipe_in[0]);
		close(pipe_in[1]);
	}
	if (pipe(pipe_err) == -1) {
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		return ERRO;
	}

	pid = fork();
	if (pid < 0) {
		msg("Can not fork");
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		return ERRO;
	} else  if (pid == 0) { /* child */
		if (dup2(pipe_out[1], STDOUT_FILENO) == -1) {
			msg("dup2 error to stdout");
			return ERRO;
		}
		if (dup2(pipe_err[1], STDERR_FILENO) == -1) {
	    		msg("dup2 error to stderr");
			return ERRO;
		}
		if (dup2(pipe_in[0], STDIN_FILENO) == -1) {
			msg("dup2 error to stdin");
			return ERRO;
		}

		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_err[0]);
		return 0; /* child returns 0 just like fork() */
	} else { /* parent */
		close(pipe_in[0]);
		close(pipe_out[1]);
		close(pipe_err[1]);
		return pid;	/* parent returns pid of child */
	}
}

/**
 * @param tr_out Handler function for data read from stdin. Its parameters are: byte buffer, size of byte buffer, pid of child process.
 */
int playbatch(int *ret_code, FILE *fp_in, int (*tr_in)(), FILE *fp_out, int (*tr_out)(), FILE *fp_err, int (*tr_err)(), long vtimer, char *prog, char *argv[])
{
	int pid;
	int pipe_out[2], pipe_err[2], pipe_in[2];

	pid = pipe_fork(pipe_in, pipe_out, pipe_err);
	if (pid < 0) {
		return ERRO;
	}

	if (pid == 0) { /* child */
		struct	rlimit	resource_limit;
		struct  itimerval interval_time;

		// Disable core dump
	 	resource_limit.rlim_cur = 0;
		resource_limit.rlim_max = 0; 
		setrlimit(RLIMIT_CORE, &resource_limit);

		// Set max time a process can run before signaling with SIGVTALRM
		if (vtimer != 0) {
			interval_time.it_interval.tv_sec = vtimer;
			interval_time.it_interval.tv_usec = vtimer * 1000000L;
			interval_time.it_value.tv_sec = vtimer;
			interval_time.it_value.tv_usec = vtimer * 1000000L;
			signal(SIGVTALRM, SIG_DFL);  // SIG_DFL is the default action for this signal, which is to terminate the process
			setitimer(ITIMER_VIRTUAL, &interval_time, NULL); 
		}
		execv(prog, argv); 
		exit(ERRO);
	} else {
		/* parent */
		int child_retcode, bytes, result;
		int fim_le = FALSE;

		while (waitpid(pid, &child_retcode, WNOHANG) != pid) {
			// Read data from child's output pipe 
			bytes = -1;
			if (ioctl(pipe_out[0], FIONREAD, &bytes) != -1) {
				if (bytes > 0) {
					bytes = read(pipe_out[0], buff, BUFFSIZE);
				}
			}
			if (bytes == -1) {
				result = ERRO;
				goto fim;
			}
			// Handle data
			if (tr_out != NULL) {
				result = tr_out(fp_out, buff, bytes, pid);
				if (result == ERRO || result != bytes) {
					result = ERRO;
					goto fim;
				}
			}

			// Read data from child's err pipe
			bytes = -1;
			if (ioctl(pipe_err[0], FIONREAD, &bytes) != -1) {
				if (bytes > 0) {
					bytes = read(pipe_err[0], buff, BUFFSIZE);
				}
			}
			if (bytes == -1) {
				result = ERRO;
				goto fim;
			}
			// Handle data
			if (tr_err != NULL) {
				result = tr_err(fp_err, buff, bytes, pid);
				if (result == ERRO || result != bytes) {
					result == ERRO;
					goto fim;
				}
			}
	
			if ( ! fim_le) {
				bytes = tr_in(fp_in, buff, BUFFSIZE);
				if (bytes < 0) {
					result = ERRO;
						goto fim;
				} else if (bytes == 0) {
					fim_le = TRUE;
					close(pipe_in[1]);
				} else {
					bytes = writepipe(pipe_in[1], buff, bytes);
					if (bytes < 0) {
						msg("Error writing to the pipe");
						result = ERRO;
						goto fim;
					} else {
						if (bytes == 0) {
						   	continue;  /* broken pipe! */ 
						}		
					}
				}
         	     	}
		}

		// Handle any remaining output data
		do {
			bytes = -1;
			if (ioctl(pipe_out[0], FIONREAD, &bytes) != -1) {
				if (bytes > 0) {
					bytes = read(pipe_out[0], buff, BUFFSIZE);
				}
			}
			if (bytes == -1) {
				result = ERRO;
				goto fim;
			}
			// Handle data
			if (tr_out != NULL) {
				result = tr_out(fp_out, buff, bytes, pid);
				if (result == ERRO || result != bytes) {
					result = ERRO;
					goto fim;
				}
			}
		} while	(bytes > 0);

		// Handle any remaining err data
		do {
			// Read data from child's err pipe
			bytes = -1;
			if (ioctl(pipe_err[0], FIONREAD, &bytes) != -1) {
				if (bytes > 0) {
					bytes = read(pipe_err[0], buff, BUFFSIZE);
				}
			}
			if (bytes == -1) {
				result = ERRO;
				goto fim;
			}
			// Handle data
			if (tr_err != NULL) {
				result = tr_err(fp_err, buff, bytes, pid);
				if (result == ERRO || result != bytes) {
					result == ERRO;
					goto fim;
				}
			}
		} while (bytes > 0);
fim:
		waitpid(pid, &child_retcode, 0);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_err[0]);
		if (ret_code != NULL) {
			*ret_code = child_retcode;
		}
		return result;
	}
}


int writepipe(int fd, char *buff, int n)
{
int k;

   sigignore(SIGPIPE);
   k = write(fd, buff, n);
   if (k <0 && errno == EPIPE) 
    	return 0;
   else 
       return k;
}

