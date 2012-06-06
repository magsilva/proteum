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
#else
#	include	<poll.h>
#	include	<stropts.h>
#	include	<sys/filio.h>
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

int
playinput(int *ret_code, int input, int (*tr_in)(),int (*tr_out)(), 
	  int (*tr_err)(), long vtimer, long rtimer,
	  char *prog, char *argv[])
{
int	i, c, bytes, ret1, k, j;
struct  itimerval	t1;
long	tt2, tt1;
struct	rlimit	rlp;
int	fim_le;
struct timeval	sele_time;
fd_set	fdset;
int	pipe_out[2], pipe_err[2], fdm;
pid_t pid, pipe_fork();



   pid = pty_fork(&fdm, pipe_out, pipe_err);

   if (pid < 0)
      return ERRO;
   if ( pid == 0 ) /* child */
   {
 	rlp.rlim_cur = rlp.rlim_max = 0;	    
	setrlimit(RLIMIT_CORE, &rlp); /* tamanho maximo para arquivo
						CORE = 0 bytes */
 	if (vtimer != 0)
	{
           tick2timeval(vtimer, &t1.it_value); 
	   t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	   t1.it_interval.tv_usec = t1.it_value.tv_usec;
	   signal(SIGVTALRM, SIG_DFL);  
	   setitimer(ITIMER_VIRTUAL, &t1, NULL); 
	} 
	if (rtimer != 0)
	{
           tick2timeval(rtimer, &t1.it_value); 
	   t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	   t1.it_interval.tv_usec = t1.it_value.tv_usec;
	   signal(SIGALRM, SIG_DFL);  
	   setitimer(ITIMER_REAL, &t1, NULL); 
	 }  
	 execv(prog,argv ); 
	 return ERRO;
    }
    else
    {
	 tt1 = gettimedad();
	 fim_le = FALSE;

	 while ( waitpid(pid, &c, WNOHANG) != pid )
	 {
	    bytes = 0;

            if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_out[0],buff,BUFFSIZE);
            else  bytes = 0;

           if (tr_out != NULL && (k = tr_out(buff, bytes,pid)) != bytes)
	    {
		if (k != ERRO) k = OK;
                goto fim;
	    }


            bytes = 0;
            if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_err[0],buff,BUFFSIZE);
            else  bytes = 0;

            if (tr_err != NULL && (k = tr_err(buff, bytes, pid)) != bytes)
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
                   /* sometimes, itimer doesn't work then this is a
                        guarantee for aborting looping mutants */
              tt2 = gettimedad() - tt1;
              if (rtimer != 0 && tt2 > rtimer)
              {
                  kill(pid, SIGKILL);
		  k = 1; c = -1;
		  goto fim;
               } 
          }
          do {

                   if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_out[0],buff,BUFFSIZE);
                     else  bytes = 0;

                   if (tr_out != NULL && (k =tr_out(buff, bytes, pid)) != bytes)
		   {
                        if (k != ERRO) k = OK;
			goto fim;
		   }
              } while (bytes > 0);

          do {

                   if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_err[0],buff,BUFFSIZE);
                     else  bytes = 0;

                   if (tr_err != NULL && (k = tr_err(buff, bytes, pid))!=bytes)
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

   pipe(pipe_out);
   pipe(pipe_err);
   pipe(pipe_in);

     if ( (pid = fork()) < 0)
   {   
	msg("Can not fork");
	close(pipe_out[1]);
	close(pipe_out[0]);
	close(pipe_err[1]);
	close(pipe_err[0]);
	close(pipe_in[1]);
	close(pipe_in[0]);
	return ERRO;
    }

    else 
    if (pid == 0) 
    {		/* child */
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
	if (dup2(pipe_in[0], STDIN_FILENO) != STDIN_FILENO)
	{
	    msg("dup2 error to stdin");
	    exit(1);
	}
	if (pipe_out[1] > STDERR_FILENO)
		close(pipe_out[1]);
	 if (pipe_err[1] > STDERR_FILENO)
		close(pipe_err[1]);
	if (pipe_in[0] > STDERR_FILENO)
		close(pipe_in[0]);
	close(pipe_out[0]);
	close(pipe_err[0]);
	close(pipe_in[1]);
	return 0;		/* child returns 0 just like fork() */

   } 
   else 
   {					/* parent */
	close(pipe_out[1]);
	close(pipe_err[1]);
	close(pipe_in[0]);
	return(pid);	/* parent returns pid of child */
   }
}





int playbatch(int *ret_code, int input, int (*tr_in)(),int (*tr_out)(), 
	  int (*tr_err)(), long vtimer, long rtimer,
	   char *prog, char *argv[])
{
int	i, pid, c, bytes, ret1, k, j;
struct  itimerval	t1;
long	tt2, tt1;
struct	rlimit	rlp;
int	fim_le;
struct timeval	sele_time;
fd_set	fdset;
int	pipe_out[2], pipe_err[2], pipe_in[2];
struct  tms xtms;


   pid = pipe_fork(pipe_in, pipe_out, pipe_err);

   if (pid < 0)
      return ERRO;
   if ( pid == 0 ) /* child */
   {
 	rlp.rlim_cur = rlp.rlim_max = 0;	    
	setrlimit(RLIMIT_CORE, &rlp); /* tamanho maximo para arquivo
						CORE = 0 bytes */
/*	if (vtimer != 0)
	{
           tick2timeval(vtimer, &t1.it_value); 
	   t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	   t1.it_interval.tv_usec = t1.it_value.tv_usec;
	   signal(SIGVTALRM, SIG_DFL);  
	   setitimer(ITIMER_VIRTUAL, &t1, NULL); 
	}
	if (rtimer != 0)
	{
           tick2timeval(rtimer, &t1.it_value); 
	   t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	   t1.it_interval.tv_usec = t1.it_value.tv_usec;
	   signal(SIGALRM, SIG_DFL);  
	   setitimer(ITIMER_REAL, &t1, NULL); 
	 } */
	 execv(prog,argv ); 
	 return ERRO;
    }
    else
    {
	 tt1 = times(&xtms);
	 fim_le = FALSE;

	 while ( waitpid(pid, &c, WNOHANG) != pid )
	 {
	    bytes = 0;

            if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_out[0],buff,BUFFSIZE);
            else  bytes = 0;

            if (tr_out != NULL && (k = tr_out(buff, bytes,pid)) != bytes)
	    {
		if (k != ERRO) k = OK;
                goto fim;
	    }

            bytes = 0;
            if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                  bytes = read(pipe_err[0],buff,BUFFSIZE);
            else  bytes = 0;

            if (tr_err != NULL && (k = tr_err(buff, bytes, pid)) != bytes)
	    {
                 if (k != ERRO ) k = OK;
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
                       close(pipe_in[1]);
                    }
                    else
                    {
                        int t;
                           t = bytes;
                           bytes = writepipe(pipe_in[1], buff, bytes);
                           if (bytes < 0)
                           {
                                msg("Error writing to the pipe");
				k = ERRO;
				goto fim;
			   }
			   else
			   if (bytes == 0)
			   	continue;  /* / broken pipe! */ 
                    }

              }
                   /* sometimes, itimer doesn't work then this is a
                        guarantee for aborting looping mutants */
              tt2 = times(&xtms) - tt1;
              if (rtimer != 0 && tt2 > rtimer)
              {
                  kill(pid, SIGKILL);
		  k = 1; c = -1;
		  goto fim;
               }
          }
          do {

                   if (ioctl(pipe_out[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_out[0],buff,BUFFSIZE);
                     else  bytes = 0;

                   if (tr_out != NULL && (k = tr_out(buff, bytes, pid)) != bytes)
		   {
                        if (k != ERRO) k = OK;
			goto fim;
		   }
              } while (bytes > 0);

          do {

                   if (ioctl(pipe_err[0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_err[0],buff,BUFFSIZE);
                     else  bytes = 0;


                   if (tr_err != NULL && (k = tr_err(buff, bytes, pid)) != bytes)
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
   close(pipe_in[1]);
   if (ret_code != NULL) 
      *ret_code = c;

   return k;

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

