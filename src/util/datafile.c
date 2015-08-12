/*
Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <string.h>
#include <time.h>
#include "datafile.h"
#include "fileio.h"

/**
 * Format a string with a date in the format Month-day-year (eg., "Feb-09-93").
 *
 * @param date String that will hold the date. It should has at least 10 bytes.
 *
 * @author Delamaro
 */
char *
monta_data(char *date)
{
	struct tm lt;
	time_t t;

	time(&t);
	localtime_r(&t, &lt);
	strftime(date, 10, "%h-%d-%y", &lt);
	return date;
}


/**
 * Save identification header for file.
 *
 * @param fp Pointer to the file where the identification header should be saved.
 * @param str Identification string.
 * @param version Proteum version string.
 *
 * @author Delamaro
 */
bool
grava_ident(FILE * fp, char * str, char * version)
{
	char date[16];

	if (posiciona(fp, 0l) == false) {
		fprintf(stderr, "Error seeking position into file %s", nomearq(fp));
		return false;
	}

	monta_data(date);
	if (fprintf(fp, "%s\n", str) == -1 || fprintf(fp, "%s\n", date) == -1 || fprintf(fp, "%s\n", version) == -1) {
		fprintf(stderr, "Error writing header to file %s", nomearq(fp));
		return false;
	}

	return true;
}


/**
 * Search for the identification string at the beginning of the file.
 *
 * @param fp Pointer to the file to be checked.
 * @param str Identification string to be looked for in the file.
 *
 * @author Delamaro
 */
bool
checa_ident(FILE *fp, char *str, char * version)
{
	char	buf[OFFSET0];
	int	i;

	if (posiciona(fp, 0l) == false) {
		fprintf(stderr, "Error seeking position into file %s", nomearq(fp));
		return false;
	}

	if (learq(fp, buf, OFFSET0) == false) {
		fprintf(stderr, "Error reading data from file %s", nomearq(fp));
		return false;
	}

	if (strncmp(buf, str, strlen(str))) {
		fprintf(stderr, "Could not find valid identifier in the file %s.%s", nomearq(fp), extarq(fp));
		return false;
	}

	for (i = 0; buf[i] != '\n' && i < OFFSET0; i++);
	i++;	/* here i points date of creation */

	for (; i < OFFSET0 && buf[i] != '\n'; i++);
	i++;	/* here i points version */

	if (strncmp(&buf[i], version, strlen(version)) ) {
		fprintf(stderr, "File %s.%s was created for another version of this tool (%s)", nomearq(fp), extarq(fp), version);
		return false;
	}

	return true;
}




