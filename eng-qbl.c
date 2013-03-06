/*
 *      eng-qbl.c
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
#include <readline/readline.h>
#include <readline/history.h>
#include "processing.h"
#include "gematria.h"
#include "temurah.h"
#include "notarikon.h"

static const char const *commands[4] = {"gematria", "temurah", "notarikon", "help"};

static struct wordlist *cli(char *command, char *input) {
	size_t lng = strlen(input);
	struct wordlist *list = NULL;
	char *tok = NULL;

	if (! (list = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;
	if (! (list->words = (char**) calloc(lng, sizeof(char*))) )
		return NULL;

	list->numwords = 0;
	list->words[list->numwords++] = strdup(command);

	while ( (tok = strtok(tok? NULL : input, " ")) )
		list->words[list->numwords++] = strdup(tok);

	return list;
}

static struct wordlist *searchResult(struct wordlist *list, enum listtype ltype, int numWordsInPhrase) {
	struct wordlist *searchList = NULL;
	if (ltype == phrasetype) {
		struct wordlist *phraseList = NULL;

		if (! (phraseList = generatePhraseList(list, numWordsInPhrase)) ) {
			fprintf(stderr, "Unable to generate list of phrases.");
			return NULL;
		}

		if (! (searchList = generateUniquePhraseList(phraseList)) ) {
			fprintf(stderr, "Unable to generate unique list of phrases.");
			return NULL;
		}

		clearWordList(phraseList);
	} else {
		if (! (searchList = generateUniqueWordList(list)) ) {
			fprintf(stderr, "Unable to generate unique list of words.");
			return NULL;
		}
	}

	return searchList;
}

void displayUsage(char *command) {
	if (strlen(command) > 5) {
		if (strncmpi(command+5, "loadfile", 8) == 0) {

			puts("\nUsage: FILE [FILE...]\n");
			puts("  Reads one or more files and generates a list of words into memory.\n  NOTE: Available in interactive mode only.\n");

		} else if (strncmpi(command+5, "quit", 4) == 0) {

			puts("\n  Exits the shell. It can't get any simpler than that.\n");

		} else if (strncmpi(command+5, commands[0], 8) == 0) {

			puts("\nUsage: [-sc] [-p num] Word | 'Phrase'\n"
				 "       [-sc] [-p num] -n numericvalue\n");
			puts("  Outputs an English Gematria value for an inputted word or phrase. Consult the manpage of 'gematria' for details.\n");

		} else if (strncmpi(command+5, commands[1], 7) == 0) {

			puts("\nUsage: [-a] [-bflrt] [-s] [-p num] Word | 'Phrase'\n");
			puts("  Ciphers an inputted word or phrase. Consult the manpage of 'temurah' for details.\n");

		} else if (strncmpi(command+5, commands[2], 9) == 0) {

			puts("\nUsage: [-f] acronym\n");
			puts("  Outputs a list of phrases from a given file where the first letter of each word in a phrase is found in the inputted acronym. Consult the manpage of 'notarikon' for details.\n");

		} else if (strncmpi(command+5, commands[3], 4) == 0) {

			puts("\n  Are you seriously looking for help on using 'help'? How much help do you really need???\n");

		}
	} else {
		puts("\nAvailable commands:");
		puts("  loadfile");
		puts("  gematria");
		puts("  temurah");
		puts("  notarikon");
		puts("  quit");
		puts("Type 'help <command>' for detailed help about a command.\n");
	}
}

int main(int argc, char **argv) {
	char *command = NULL, *userInput = NULL, *buf = NULL;
	int opt, numWordsInPhrase, searchFlag, searchCount, searchCountFlag;
	enum listtype ltype;
	struct wordlist *wList = NULL;
	struct wordlist *controls = NULL;

	if (argc > 1) {
		if (strncmpi(argv[1], commands[0], strlen(argv[1])) &&
			strncmpi(argv[1], commands[1], strlen(argv[1])) &&
			strncmpi(argv[1], commands[2], strlen(argv[1])) &&
			strncmpi(argv[1], commands[3], strlen(argv[1]))) {
			printf("Unknown command: %s.\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		struct wordlist ctl;
		ctl.numwords = argc;
		ctl.words = argv;
		buf = listToString(&ctl, 1);

		if (strncmpi(buf, commands[3], 4) == 0) {
			displayUsage(buf);
			free(buf);
			buf = NULL;
			exit(EXIT_FAILURE);
		}

		system(buf);
		free(buf);
		buf = NULL;
		exit(EXIT_SUCCESS);
	}

	puts("Welcome to the English Kabbalah shell!\nType 'help' to list the available commands and 'quit' (or CTRL-D) to leave the shell.");
	using_history();

	while (1) {
		command = readline("eng-qbl> ");
		if (!command) {
			free(command);
			clearWordList(wList);
			clear_history();
			puts("");
			break;
		}
		add_history(command);

		if (strncmpi(command, "help", 4) == 0) {

			displayUsage(command);

		} else if (strncmpi(command, "quit", 4) == 0) {

			free(command);
			clearWordList(wList);
			clear_history();
			exit(EXIT_SUCCESS);

		} else if (strncmpi(command, "loadfile", 8) == 0) {

			puts("\n  Enter one or more files and generates a list of words into memory.");
			puts("  Type CTRL-D to exit this section.\n");
			while (1) {
				userInput = readline("loadfile> ");

				if (userInput && strlen(userInput)) {
					add_history(userInput);
					controls = cli(command, userInput);
					for (int i = 1; i < controls->numwords; i++) {
						if (! (wList = generateWordListFromFile(wList, controls->words[i])) ) {
							fprintf(stderr, "Unable to generate list of words. "
											"%s is either corrupted or cannot be accessed at this time.\n"
											"If %s is a remote file, you'll need to insall libcurl and recompile %s to access it. "
											"Consult the README file for details.\n", controls->words[i], controls->words[i], PACKAGE);
						} else {
							printf("File \"%s\" loaded.\n", controls->words[i]);
						}
					}
					clearWordList(controls);
				} else {
					puts("");
					break;
				}
				free(userInput);
				userInput = NULL;
			}

		} else if (strncmpi(command, "gematria", 8) == 0) {

			puts("\n  Enter a word or phrase.");
			puts("  Type CTRL-D to exit this section.\n");
			int gemValue, valueFlag;
			while (1) {
				gemValue = searchCount = numWordsInPhrase = 0;
				searchFlag = searchCountFlag = valueFlag = 0;
				ltype = wordtype;

				userInput = readline("gematria> ");

				if (!userInput) {
					puts("");
					free(userInput);
					break;
				} else if (!strlen(userInput)) continue;

				optind = opterr = optopt = 0;
				static struct option long_options[] = {
				   {"search", 0, NULL, 's'},
				   {"phrases", 1, NULL, 'p'},
				   {"count", 0, NULL, 'c'},
				   {"gemval", 0, NULL, 'n'},
			       {0, 0, 0, 0}
				};

				add_history(userInput);
				controls = cli(command, userInput);

				while ((opt = getopt_long(controls->numwords, controls->words, "cnp:s", long_options, NULL)) != -1) {
					switch (opt) {
						case 'c':
							searchCountFlag = 1;
							searchCount = 0;
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
						break;
						default:
							displayUsage("help");
					}
				}

				if (valueFlag) gemValue = atoi(controls->words[optind]);
				else {
					buf = listToString(controls, optind);
					gemValue = gemNumericValue(buf);
					free(buf);
					buf = NULL;
				}

				if (gemValue < 0) gemValue *= -1;

				if (!valueFlag) {
					puts("\n---- Gematria Results --------------------------------");
					printf("%d", gemValue);
				}

				if (searchFlag && wList) {
					struct wordlist *searchList = searchResult(wList, ltype, numWordsInPhrase);

					puts("\n---- Search Results ----------------------------------");
					for (int i = 0; i < searchList->numwords; i++)
						if (gemValue == gemNumericValue(searchList->words[i])) {
							if (!searchCountFlag) {
								if (searchCount) printf(", ");
								printf("%s", searchList->words[i]);
							}
							searchCount++;
						}
					if (searchCountFlag) printf("Number of mathes: %d", searchCount);

					clearWordList(searchList);
				}

				puts("\n------------------------------------------------------\n");

				clearWordList(controls);

				free(userInput);
				userInput = NULL;
			}

		} else if (strncmpi(command, "temurah", 7) == 0) {

			puts("\n  Enter a word or phrase.");
			puts("  Type CTRL-D to exit this section.\n");
			while (1) {
				searchCount = numWordsInPhrase = 0;
				searchFlag = 0;
				ltype = wordtype;

				userInput = readline("temurah> ");

				if (!userInput) {
					puts("");
					free(userInput);
					break;
				} else if (!strlen(userInput)) continue;

				int f_avgadFlag = 0, r_avgadFlag = 0, atbashFlag = 0, albamFlag = 0, aikbekarFlag = 0;
				char *f_avgad_w = NULL,
					 *r_avgad_w = NULL,
					 *atbash_w = NULL,
					 *albam_w = NULL,
					 *aikbekar_w = NULL;

				optind = opterr = optopt = 0;
				static struct option long_options[] = {
				   {"all", 0, NULL, 'a'},
				   {"forward-avgad", 0, NULL, 'f'},
				   {"backward-avgad", 0, NULL, 'r'},
				   {"atbash", 0, NULL, 't'},
				   {"albam", 0, NULL, 'l'},
				   {"aikbekar", 0, NULL, 'b'},
				   {"search", 0, NULL, 's'},
				   {"phrases", 1, NULL, 'p'},
				   {0, 0, 0, 0}
				};

				add_history(userInput);
				controls = cli(command, userInput);

				while ((opt = getopt_long(controls->numwords, controls->words, "abflrstp:", long_options, NULL)) != -1) {
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
							searchCount = 0;
						break;
						case 't':
							atbashFlag = 1;
						break;
						default:
							displayUsage("help");
					}
				}

				buf = listToString(controls, optind);

				puts("\n---- Temurah Results ---------------------------------");
				if (f_avgadFlag)	{ f_avgad_w = f_avgad(buf);	  printf("Forward-Avgad:  %s\n", f_avgad_w);  }
				if (r_avgadFlag)	{ r_avgad_w = r_avgad(buf);	  printf("Backward-Avgad: %s\n", r_avgad_w);  }
				if (atbashFlag)		{ atbash_w = atbash(buf); 	  printf("Atbash:         %s\n", atbash_w);   }
				if (albamFlag)		{ albam_w = albam(buf);		  printf("Albam:          %s\n", albam_w);    }
				if (aikbekarFlag)	{ aikbekar_w = aikbekar(buf); printf("Aik Bekar:      %s\n", aikbekar_w); }

				free(buf);
				buf = NULL;

				if (searchFlag && wList) {
					struct wordlist *searchList = searchResult(wList, ltype, numWordsInPhrase);

					puts("\n---- Search Results ----------------------------------");
					if (f_avgadFlag) {
						for (int i = 0, searchCount = 0; i < searchList->numwords; i++)
							if (strncmpi(f_avgad_w, searchList->words[i], strlen(searchList->words[i])) == 0)
								searchCount++;

						printf("Forward-Avgad:  %d\n", searchCount);
					}
					if (r_avgadFlag) {
						for (int i = 0, searchCount = 0; i < searchList->numwords; i++)
							if (strncmpi(r_avgad_w, searchList->words[i], strlen(searchList->words[i])) == 0)
								searchCount++;

						printf("Backward-Avgad: %d\n", searchCount);
					}
					if (atbashFlag) {
						for (int i = 0, searchCount = 0; i < searchList->numwords; i++)
							if (strncmpi(atbash_w, searchList->words[i], strlen(searchList->words[i])) == 0)
								searchCount++;

						printf("Atbash:         %d\n", searchCount);
					}
					if (albamFlag) {
						for (int i = 0, searchCount = 0; i < searchList->numwords; i++)
							if (strncmpi(albam_w, searchList->words[i], strlen(searchList->words[i])) == 0)
								searchCount++;

						printf("Albam:          %d\n", searchCount);
					}
					if (aikbekarFlag) {
						for (int i = 0, searchCount = 0; i < searchList->numwords; i++)
							if (strncmpi(aikbekar_w, searchList->words[i], strlen(searchList->words[i])) == 0)
								searchCount++;

						printf("Aik Bekar:      %d\n", searchCount);
					}

					clearWordList(searchList);
				}

				free(f_avgad_w);
				free(r_avgad_w);
				free(atbash_w);
				free(albam_w);
				free(aikbekar_w);

				puts("------------------------------------------------------\n");

				clearWordList(controls);

				free(userInput);
				userInput = NULL;
			}

		} else if (strncmpi(command, "notarikon", 9) == 0) {

			puts("\n  Enter an acronym.");
			puts("  Type CTRL-D to exit this section.\n");
			while (1) {
				userInput = readline("notarikon> ");

				if (!userInput) {
					puts("");
					free(userInput);
					break;
				} else if (!strlen(userInput)) continue;

				add_history(userInput);

				puts("\n---- Notarikon Results -------------------------------");
				if (wList) notarikon(userInput, wList);
				else puts("You have not loaded a file from which to search, thus no result.");
				puts("------------------------------------------------------\n");

				free(userInput);
				userInput = NULL;
			}

		}

		free(command);
	}

	exit(EXIT_SUCCESS);
}
