/*
 *      eq-anagram.c
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
#include "anagram.h"

void displayUsage() {
	fprintf(stderr,
		"\n===============================================================\n"
		"English Anagram\n"
		"Copyright (C) 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>\n\n"

		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n\n"

		"Usage: anagram -f {FILE|URL}, --file={FILE|URL} wordphrase\n"
		"===============================================================\n\n");
		exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int opt;
	char wordphrase[256];

	struct wordlist *searchFiles = NULL;
	struct wordlist *searchList = NULL;

	if (argc < 2) displayUsage();

	if (argc < 3) fgets(wordphrase, 256, stdin);
	else strncpy(wordphrase, argv[2], 256);

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
		fgets(wordphrase, 256, stdin);
		wordphrase[strlen(wordphrase)-1] = '\0';
	} else {
		char *buf = NULL;
		struct wordlist ctl;
		ctl.numwords = argc;
		ctl.words = argv;
		buf = listToString(&ctl, optind);
		strncpy(wordphrase, buf, 256);
		free(buf);
	}

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

	anagram(wordphrase, searchList);

	clearWordList(searchList);

	exit(EXIT_SUCCESS);
}
