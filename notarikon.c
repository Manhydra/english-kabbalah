/*
 *      notarikon.c
 *
 *      Copyright 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "processing.h"
#include "notarikon.h"

void notarikon(char *acronym, struct wordlist *searchList) {
	size_t lng;
	int found = 0, wIndex = 0, listIndex = 0;

	lng = strlen(acronym);
	while (wIndex < lng && listIndex < searchList->numwords) {
		if ( (acronym[wIndex] >= 'A' && acronym[wIndex] <= 'Z') ||
			 (acronym[wIndex] >= 'a' && acronym[wIndex] <= 'z') ) {
			if (tolower(acronym[wIndex]) != tolower(searchList->words[listIndex][0]))
				wIndex = 0;
			else {
				wIndex++;
				if (++found == lng) {
					for (int i = (lng-1); i >= 0; i--) {
						if (i < (lng-1)) printf(" ");
						printf("%s", searchList->words[listIndex-i]);
					}
					printf("\n");
				}
			}
		}
		if (found && !wIndex) {
			listIndex -= --found;
			found = 0;
		} else {
			if (found == lng) wIndex = 0;
			listIndex++;
		}
	}
}
