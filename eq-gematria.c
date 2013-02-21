/*
 *      eq-gematria.c
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
#include "gematria.h"

void displayUsage() {
	fprintf(stderr,
		"\n===============================================================\n"
		"English Gematria\n"
		"Copyright (C) 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>\n\n"

		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n\n"

		"  -s | --search  Optional: Text file (local or remote) in which to search for word or phrase of equivalent gematria value.\n"
		"  -p | --phrases Optional: To conduct phrase search, where each resulting phrase has num words.\n"
		"  -c | --count   Optional: Output the number of finds (words or phrases) only.\n\n"

		"Usage: gematria [-s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num] [-c | --count]] Word | 'Phrase'\n"
		"                [-s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num] [-c | --count]] -n | --gemval numericvalue\n"
		"===============================================================\n\n");
		exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int opt,
		theValue,
		numWordsInPhrase = 2,
		valueFlag = 0,
		searchFlag = 0,
		countFlag = 0;

	struct wordlist *searchFiles = NULL;

	char gemWord[256];

	enum listtype ltype = wordtype;

	static struct option long_options[] = {
	   {"search", 1, NULL, 's'},
	   {"phrases", 1, NULL, 'p'},
	   {"count", 0, NULL, 'c'},
	   {"gemval", 0, NULL, 'n'},
       {0, 0, 0, 0}
   };

	while ((opt = getopt_long(argc, argv, "cnp:s:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'c':
				countFlag = 1;
			break;
			case 'n':
				valueFlag = 1;
			break;
			case 'p':
				ltype = phrasetype;
				numWordsInPhrase = atoi(optarg);
			break;
			case 's':
				searchFlag = 1;
				searchFiles = generateWordList(searchFiles, optarg);
			break;
			default:
				displayUsage();
		}
	}

	if (optind >= argc) {
		if (valueFlag)
			scanf("%d", &theValue);
		else {
			fgets(gemWord, 256, stdin);
			theValue = gemNumericValue(gemWord);
		}
	} else {
		if (valueFlag)
			theValue = atoi(argv[optind]);
		else {
			strncpy(gemWord, argv[optind], 256);
			theValue = gemNumericValue(gemWord);
		}
	}

	if (theValue < 0) theValue *= -1;

	if (!valueFlag && !searchFlag) printf("%d\n", theValue);

	if (searchFlag) {
		struct wordlist *wList = NULL;
		struct wordlist *searchList = NULL;

		for (int i = 0; i < searchFiles->numwords; i++) {
			if (! (wList = generateWordListFromFile(wList, searchFiles->words[i])) ) {
				fprintf(stderr, "Unable to generate list of words. "
								"%s is either corrupted or cannot be accessed at this time.\n"
								"If %s is a remote file, you'll need to insall libcurl and recompile %s to access it. "
								"Consult the README file for details.\n", searchFiles->words[i], searchFiles->words[i], PACKAGE);
				exit(EXIT_FAILURE);
			}
		}
		clearWordList(searchFiles);

		if (ltype == phrasetype) {
			struct wordlist *phraseList = NULL;

			if (! (phraseList = generatePhraseList(wList, numWordsInPhrase)) ) {
				fprintf(stderr, "Unable to generate list of phrases.");
				exit(EXIT_FAILURE);
			}

			if (! (searchList = generateUniquePhraseList(phraseList)) ) {
				fprintf(stderr, "Unable to generate list of unique phrases.");
				exit(EXIT_FAILURE);
			}

			clearWordList(phraseList);
			clearWordList(wList);
		} else {
			if (! (searchList = generateUniqueWordList(wList)) ) {
				fprintf(stderr, "Unable to generate list of unique words.");
				exit(EXIT_FAILURE);
			}
			clearWordList(wList);
		}

		int i, count;

		for (i = 0, count = 0; i < searchList->numwords; i++)
			if (theValue == gemNumericValue(searchList->words[i])) {
				if (!countFlag) printf("%s\n", searchList->words[i]);
				count++;
			}

		if (countFlag) printf("%i\n", count);

		clearWordList(searchList);
	}

	exit(EXIT_SUCCESS);
}
