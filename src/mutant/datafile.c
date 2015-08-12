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

/**
 * Read file with mutation operators.
 */
int    
LeArqOp(char dir[], char file[], OPERADOR_MUTACAO tab[])
{
	FILE    *fp;
	char    strop[10];
	int     x, i;
	double  max;

	fp = abrearq(dir, file, "", 0);
	if (fp == NULL) {
		fprintf(stderr, "Invalid operator file %s", file);
		return ERRO;
	}
	while (fscanf(fp, "%s %lf %d", strop, &x, &max) == 3) {
		i = busca_op(strop);
		if (i >= 0) {
			do {
				if (x >= 0.0 && x <= 1.0) {
					tab[i].percentage = x;
				} else {
					fprintf(stderr, "Invalid percentage for operator %i in the file %s: %lf", i, file, x);
					return ERRO;
				}
				tab[i].maximum = max;
		                i = next_match(strop, i);
			} while (i >= 0);
		} else {
			fprintf(stderr, "Invalid mutant operator %d in the file %s", i, file);
			return ERRO;
		}
	}
	return OK;
}
