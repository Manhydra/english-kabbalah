/*
 *      eq-temurah.c
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
#include "temurah.h"

void displayUsage() {
	fprintf(stderr, "\n===============================================================\n"
		"English Temurah\n"
		"Copyright (C) 2012 Marc Sylvestre <marc.sylvestre@manhydra.com>\n\n"

		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n\n"

		"  -a | --all			 Word or phrase is enciphered using all methods.\n"
		"  -f | --forward-avgad  Word or phrase is enciphered using Forward-shift Avgad.\n"
		"  -r | --backward-avgad Word or phrase is enciphered using Backward-shift Avgad.\n"
		"  -t | --atbash		 Word or phrase is enciphered using Atbash.\n"
		"  -l | --albam			 Word or phrase is enciphered using Albam.\n"
		"  -b | --aikbekar		 Word or phrase is enciphered using Aik Bekar.\n"
		"  -s | --search		 Optional: Text file (local or remote) in which to search for enciphered world or phrase.\n"
		"  -p | --phrases		 Optional: To conduct phrase search, where each resulting phrase has num words.\n\n"

		"Usage: temurah [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -f | --forward-avgad Word | 'Phrase'\n"
		"		        [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -r | --backward-avgad Word | 'Phrase'\n"
		"		        [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -t | --atbash Word | 'Phrase'\n"
		"		        [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -l | --albam Word | 'Phrase'\n"
		"		        [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -b | --aikbekar Word | 'Phrase'\n"
		"		        [ -s {FILE|URL} | --search={FILE|URL} [-p num | --phrases=num]] -a | --all Word | 'Phrase'\n"
		"===============================================================\n\n");
		exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	int opt,
		numWordsInPhrase = 2,
		f_avgadFlag = 0,
		r_avgadFlag = 0,
		atbashFlag = 0,
		albamFlag = 0,
		aikbekarFlag = 0,
		searchFlag = 0;

	char temWord[256],
		 *f_avgad_w = NULL,
		 *r_avgad_w = NULL,
		 *atbash_w = NULL,
		 *albam_w = NULL,
		 *aikbekar_w = NULL;

	struct wordlist *searchFiles = NULL;

	enum listtype ltype = wordtype;

	static struct option long_options[] = {
	   {"all", 0, NULL, 'a'},
	   {"forward-avgad", 0, NULL, 'f'},
	   {"backward-avgad", 0, NULL, 'r'},
	   {"atbash", 0, NULL, 't'},
	   {"albam", 0, NULL, 'l'},
	   {"aikbekar", 0, NULL, 'b'},
	   {"search", 1, NULL, 's'},
	   {"phrases", 1, NULL, 'p'},
       {0, 0, 0, 0}
   };

	if (argc < 2) displayUsage();

	while ((opt = getopt_long(argc, argv, "abflrs:tp:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'a':
				f_avgadFlag = 1;
				r_avgadFlag = 1;
				atbashFlag = 1;
				albamFlag = 1;
				aikbekarFlag = 1;
			break;
			case 'b':
				aikbekarFlag = 1;
			break;
			case 'f':
				f_avgadFlag = 1;
			break;
			case 'l':
				albamFlag = 1;
			break;
			case 'p':
				ltype = phrasetype;
				numWordsInPhrase = atoi(optarg);
			break;
			case 'r':
				r_avgadFlag = 1;
			break;
			case 's':
				searchFlag = 1;
				searchFiles = generateWordList(searchFiles, optarg);
			break;
			case 't':
				atbashFlag = 1;
			break;
			default:
				displayUsage();
		}
	}

	if (optind >= argc) {
		fgets(temWord, 256, stdin);
		temWord[strlen(temWord)-1] = '\0';
	}
	else {
		char *buf = NULL;
		struct wordlist ctl;
		ctl.numwords = argc;
		ctl.words = argv;
		buf = listToString(&ctl, optind);
		strncpy(temWord, buf, 256);
		free(buf);
	}

	if (f_avgadFlag)	{ f_avgad_w = f_avgad(temWord);	  puts(f_avgad_w);  }
	if (r_avgadFlag)	{ r_avgad_w = r_avgad(temWord);	  puts(r_avgad_w);  }
	if (atbashFlag)		{ atbash_w = atbash(temWord); 	  puts(atbash_w);   }
	if (albamFlag)		{ albam_w = albam(temWord);		  puts(albam_w);    }
	if (aikbekarFlag)	{ aikbekar_w = aikbekar(temWord); puts(aikbekar_w); }

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
				fprintf(stderr, "Unable to generate unique list of phrases.");
				exit(EXIT_FAILURE);
			}

			clearWordList(phraseList);
			clearWordList(wList);
		} else {
			if (! (searchList = generateUniqueWordList(wList)) ) {
				fprintf(stderr, "Unable to generate unique list of words.");
				exit(EXIT_FAILURE);
			}
			clearWordList(wList);
		}

		int i, count;

		if (f_avgadFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncasecmp(f_avgad_w, searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;

			printf("%i\n", count);
		}
		if (r_avgadFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncasecmp(r_avgad_w, searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;

			printf("%i\n", count);
		}
		if (atbashFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncasecmp(atbash_w, searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;

			printf("%i\n", count);
		}
		if (albamFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncasecmp(albam_w, searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;

			printf("%i\n", count);
		}
		if (aikbekarFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncasecmp(aikbekar_w, searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;

			printf("%i\n", count);
		}

		clearWordList(searchList);
	}

	free(f_avgad_w);
	free(r_avgad_w);
	free(atbash_w);
	free(albam_w);
	free(aikbekar_w);

	exit(EXIT_SUCCESS);
}
