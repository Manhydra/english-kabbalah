/*
 *      eq-notarikon.c
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
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include <getopt.h>
#include "processing.h"
#include "notarikon.h"

void displayUsage() {
	fprintf(stderr,
		"\n===============================================================\n"
		"English Notarikon\n"
		"Copyright (C) 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>\n\n"

		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n\n"

		"Usage: notarikon -f {FILE|URL}, --file={FILE|URL} acronym\n"
		"===============================================================\n\n");
		exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int opt;
	char acronym[256];

	struct wordlist *searchFiles = NULL;
	struct wordlist *searchList = NULL;

	if (argc < 2) displayUsage();

	if (argc < 3) fgets(acronym, 256, stdin);
	else strncpy(acronym, argv[2], 256);

	static struct option long_options[] = {
	   {"file", 1, NULL, 'f'},
       {0, 0, 0, 0}
   };

	while ((opt = getopt_long(argc, argv, "f:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'f':
				searchFiles = generateWordList(searchFiles, optarg);
			break;
			default:
				displayUsage();
		}
	}

	if (optind >= argc) {
		fgets(acronym, 256, stdin);
		acronym[strlen(acronym)-1] = '\0';
	}
	else strncpy(acronym, argv[optind], 256);

	for (int i = 0; i < searchFiles->numwords; i++) {
		if (! (searchList = generateWordListFromFile(searchList, searchFiles->words[i])) ) {
			fprintf(stderr, "Unable to generate list of words. "
							"%s is either corrupted or cannot be accessed at this time.\n"
							"If %s is a remote file, you'll need to insall libcurl and recompile %s to access it. "
							"Consult the README file for details.\n", searchFiles->words[i], searchFiles->words[i], PACKAGE);
			exit(EXIT_FAILURE);
		}
	}
	clearWordList(searchFiles);

	notarikon(acronym, searchList);

	clearWordList(searchList);

	exit(EXIT_SUCCESS);
}
