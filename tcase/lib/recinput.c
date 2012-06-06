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
#include        <sys/stat.h>
#include        <fcntl.h>
#include	<signal.h>
#include	<termios.h>
#include	<sys/ioctl.h>	 
#include	<unistd.h>

#ifdef LINUX
#	include	<sys/poll.h>
#	include <errno.h>
#	include	<grp.h>
#	define	INFTIM	-1
#else
#	include	<poll.h>
#	include	<stropts.h>
#endif

#include	<stdio.h>

#include	<lib/gerais.h>

#define	BUFFSIZE	512

char		slave_name[20];
FILE 		*input;

void	 tty_atexit(void);


int
loop1(int remfd)
{
int	bol, n, nread;
char	c, buff[BUFFSIZE];
struct pollfd	fds[2];

   fds[1].fd = STDIN_FILENO;	/* user's terminal input */
   fds[1].events = POLLIN;
   fds[0].fd = remfd;
   fds[0].events = POLLIN;
   n = 2;

   for ( ; ; ) 
   {
	if (poll(fds, n, INFTIM) <= 0)
	{
	   msg("poll error");
	   return ERRO;
	}

	if (fds[1].revents & POLLIN) 
	{	/* data to read on stdin */
	   nread = read(fds[1].fd, buff, BUFFSIZE);
	   if (nread < 0)
	   {
		msg("read error from stdin");
		return ERRO;
	   }
	   if (nread == 0)
	   {
		n--;
		fds[1].revents = 0;
	   }
	   fwrite(buff, 1, nread, input); fflush(input);
	   if (writen(remfd, buff, nread) != nread)
	   {
	   	msg("writen error to master pty");
		return ERRO;
	   }
	}

	if (fds[0].revents & POLLIN) 
	{
	   nread = read(fds[0].fd, buff, BUFFSIZE);
#ifdef LINUX
	   if (nread <= 0) // on Linux, at the end of the input, I get a -1
#else			   // so this is a workaround
	   if (nread == 0)
#endif
		break;
	   if (nread < 0)
	   {
		msg("Error reading from slave");
		perror("Cause: ");
		return ERRO;
	   }
	   if (writen(STDOUT_FILENO, buff, nread) != nread)
	   {
		msg("write error to stdout");
		return ERRO;
	   }
	}

	if (fds[0].revents & POLLHUP)
	{
	   do 
	   {
		nread = read(fds[0].fd, buff, BUFFSIZE);
#ifdef LINUX
                if (nread <= 0 ) break; // on Linux, at the end of the input,
#endif                                  //  I get a -1 so this is a workaround
	        if (nread < 0)
		{
		    msg("Error reading from slave");
		    return ERRO;
		}
	        if (writen(STDOUT_FILENO, buff, nread) != nread)
		{
		    msg("write error to stdout");
		    return ERRO;
		}
	   } while (nread > 0);
	   break; /* for */
	}
   }
   return OK;
}


pid_t
pty_fork2(int *ptrfdm, char *slave_name)
{
	int		fdm, fds;
	pid_t	pid;
	char	pts_name[20];

	if ( (fdm = ptym_open(pts_name)) < 0)
	{
	    msg("can't open master pty");
	    return ERRO;
	}

	strcpy(slave_name, pts_name);	/* return name of slave */
        /*/printf("\npty_fork2: %d %s", fdm, pts_name); */

	if ( (pid = fork()) < 0)
	{
	    msg("Can not fork");
	    return ERRO;
	}

	else if (pid == 0) {		/* child */
		/*/ sleep(30); */
		if (setsid() < 0)
		{
		   msg("setsid error");
		   exit(1);
		}

		/* SVR4 acquires controlling terminal on open() */
		if ( (fds = ptys_open(fdm, pts_name)) < 0)
		{
		    msg("can't open slave pty");
		    exit(1);
		}
		close(fdm);		/* all done with master in child */

#ifdef	LINUX
				/* 44BSD way to acquire controlling terminal */
    		if (ioctl(fds, TIOCSCTTY, (char *) 0) < 0)
    		{
		    msg("TIOCSCTTY error");
	  	    return ERRO;
   		}
#endif
		if (tty_copy(fds) == ERRO)
		   exit(1);
				/* slave becomes stdin/stdout/stderr of child */
		if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO ||
		    dup2(fds, STDERR_FILENO) != STDERR_FILENO ||
		    dup2(fds, STDIN_FILENO) != STDIN_FILENO )
		{
		    msg("dup2 error to slave");
		    exit(1);
		}
		if (fds > STDERR_FILENO)
		   close(fds);
		return 0;		/* child returns 0 just like fork() */

	} else {					/* parent */
		*ptrfdm = fdm;	/* return fd of master */
		return(pid);	/* parent returns pid of child */
	}
}

int
recinput(char *arqinput, char *argv[])
{
int	fdm, c;
pid_t	pid;
struct termios	orig_termios;
struct winsize	size;

   if (! isatty(0) || ! isatty(1))
   {
	msg("stdin and stdout must be a terminal");
	return ERRO;
   }
   input = criarq("", arqinput, "");
   if (input == NULL)
      return ERRO;

   if (tty_save(STDIN_FILENO) == ERRO)
	return ERRO;

   pid = pty_fork2(&fdm, slave_name);

   if (pid < 0)
   {
	return ERRO;
   }
   else 
   if (pid == 0) 
   {		/* child */
	if (execv(argv[0], argv) < 0)
	{
	   msg("can't execv: %s");
	   return ERRO;
	}
    }

   if (tty_raw(STDIN_FILENO) < 0)	/* user's tty to raw mode */
   {
	msg("tty_raw error");
        return ERRO;
   }

   c = loop1(fdm);	/* copies stdin -> ptym, ptym -> stdout */
   waitpid(pid, NULL, 0);
   fclose(input);
   close(fdm); 
   tty_copy(STDIN_FILENO);
   return c;
}

#ifdef 	LINUX

int
ptym_open(char *pts_name)
{
	int		fdm;
	char	*ptr1, *ptr2;

	strcpy(pts_name, "/dev/ptyXY");
	  /* array index: 0123456789 (for references in following code) */
	for (ptr1 = "pqrstuvwxyzPQRST"; *ptr1 != 0; ptr1++) {
		pts_name[8] = *ptr1;
		for (ptr2 = "0123456789abcdef"; *ptr2 != 0; ptr2++) {
			pts_name[9] = *ptr2;

						/* try to open master */
			if ( (fdm = open(pts_name, O_RDWR)) < 0) {
				if (errno == ENOENT)	/* different from EIO */
				   return(-1);		/* out of pty devices */
				else
				   continue;	/* try next pty device */
			}

			pts_name[5] = 't';	/* change "pty" to "tty" */
			return(fdm);	/* got it, return fd of master */
		}
	}
	return(-1);		/* out of pty devices */
}

int
ptys_open(int fdm, char *pts_name)
{
	struct group	*grptr;
	int		gid, fds;

	if ( (grptr = getgrnam("tty")) != NULL)
		gid = grptr->gr_gid;
	else
		gid = -1;	/* group tty is not in the group file */

		/* following two functions don't work unless we're root */
	chown(pts_name, getuid(), gid);
	chmod(pts_name, S_IRUSR | S_IWUSR | S_IWGRP);

	if ( (fds = open(pts_name, O_RDWR)) < 0) {
		close(fdm);
		return(-1);
	}
	return(fds);
}

#else

extern char	*ptsname(int);	/* prototype not in any system header */

int
ptym_open(char *pts_name)
{
	char	*ptr;
	int		fdm;

	strcpy(pts_name, "/dev/ptmx");	/* in case open fails */
	if ( (fdm = open(pts_name, O_RDWR)) < 0)
		return(-1);

	if (grantpt(fdm) < 0) {		/* grant access to slave */
		close(fdm);
		return(-2);
	}
	if (unlockpt(fdm) < 0) {	/* clear slave's lock flag */
		close(fdm);
		return(-3);
	}
	if ( (ptr = ptsname(fdm)) == NULL) {	/* get slave's name */
		close(fdm);
		return(-4);
	}

	strcpy(pts_name, ptr);	/* return name of slave */
	return(fdm);			/* return fd of master */
}
int
ptys_open(int fdm, char *pts_name)
{
	int		fds;

			/* following should allocate controlling terminal */
	if ( (fds = open(pts_name, O_RDWR)) < 0) {
		close(fdm);
		return(-5);
	}
	if (ioctl(fds, I_PUSH, "ptem") < 0) {
		close(fdm);
		close(fds);
		return(-6);
	}
	if (ioctl(fds, I_PUSH, "ldterm") < 0) {
		close(fdm);
		close(fds);
		return(-7);
	}
	if (ioctl(fds, I_PUSH, "ttcompat") < 0) {
		close(fdm);
		close(fds);
		return(-8);
	} 

	return(fds);
}

#endif
