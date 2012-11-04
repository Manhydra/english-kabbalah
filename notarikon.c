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

#include <unistd.h>
#include "processing.h"

#define MAX_LENGTH 256

void displayUsage(const char *progname) {
	fprintf(stderr, "\n===============================================================\n"
		"English Notarikon\n"
		"Copyright (C) 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>\n\n"
		
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n\n"
		
		"Usage: %s FILE acronym\n"
		"===============================================================\n\n", progname);
		exit(EXIT_FAILURE);	
}

int main(int argc, char **argv) {
	char acronym[MAX_LENGTH];
	struct wordlist *searchList = NULL;
	int numWordsInPhrase = 2;
	size_t lng;
	int found = 0, wIndex = 0, listIndex = 0;
	
	if (argc < 2) displayUsage(argv[0]);
	
	if (argc < 3) fgets(acronym, MAX_LENGTH, stdin);
	else strncpy(acronym, argv[2], MAX_LENGTH);
	
	if (! (searchList = generateLists(argv[1], wordtype, numWordsInPhrase, 0)) ) {
		fprintf(stderr, "Unable to generate search list. %s is either corrupted or not provided.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	lng = strlen(acronym);
	while (wIndex < lng && listIndex < searchList->numwords) {
		if ( (acronym[wIndex] >= 'A' && acronym[wIndex] <= 'Z') || (acronym[wIndex] >= 'a' && acronym[wIndex] <= 'z') ) {
			if (charToLowerCase(acronym[wIndex]) != charToLowerCase(searchList->words[listIndex][0])) {
				wIndex = 0;
			} else {
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
	
	free(searchList->words);
	free(searchList);
	
	exit(EXIT_SUCCESS);
}
