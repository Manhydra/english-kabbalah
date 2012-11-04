/*
 *      temurah.c
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

#include <getopt.h>
#include "processing.h"
#include "temurah.h"

int compareChar(const void *a, const void *b) {
  return ( *(char*)a - *(char*)b );
}

/* 
 * name: avgad
 * @param string, int
 * @return string
 */
char *avgad(char *word, int shift) {
	char *w = strdup(word);
	int lng = strlen(w);
	int *vFound;
	
	if (shift > 0) {
		for (int i = 0; i < lng; ++i) {
			if (w[i] >= 'A' && w[i] <= 'Z') {
				vFound = (int*) bsearch(&*(w + i), uvowels, strlen(uvowels),
				          sizeof(char), compareChar);
				if (!vFound) {
					for (int j = 0; j < strlen(uconsonants); ++j) {
						if (w[i] == uconsonants[j]) {
							w[i] = (w[i] == uconsonants[strlen(uconsonants)-1]) 
							? uconsonants[0] : uconsonants[j + 1];
							break;
						}
					}
				} else {
					for (int j = 0; j < strlen(uvowels); ++j) {
						if (w[i] == uvowels[j]) {
							w[i] = (w[i] == uvowels[strlen(uvowels)-1]) 
							? uvowels[0] : uvowels[j + 1];
							break;
						}
					}
				}
			}
			if (w[i] >= 'a' && w[i] <= 'z') {
				vFound = (int*) bsearch(&*(w + i), lvowels, strlen(lvowels),
				          sizeof(char), compareChar);
				if (!vFound) {
					for (int j = 0; j < strlen(lconsonants); ++j) {
						if (w[i] == lconsonants[j]) {
							w[i] = (w[i] == lconsonants[strlen(lconsonants)-1]) 
							? lconsonants[0] : lconsonants[j + 1];
							break;
						}
					}
				} else {
					for (int j = 0; j < strlen(lvowels); ++j) {
						if (w[i] == lvowels[j]) {
							w[i] = (w[i] == lvowels[strlen(lvowels)-1]) 
							? lvowels[0] : lvowels[j + 1];
							break;
						}
					}
				}
			}
		}
	} else if (shift < 0) {
		for (int i = 0; i < lng; ++i) {
			if (w[i] >= 'A' && w[i] <= 'Z') {
				vFound = (int*) bsearch(&*(w + i), uvowels, strlen(uvowels),
				          sizeof(char), compareChar);
				if (!vFound) {
					for (int j = 0; j < strlen(uconsonants); ++j) {
						if (w[i] == uconsonants[j]) {
							w[i] = (w[i] == uconsonants[0]) 
							? uconsonants[strlen(uconsonants)-1] : uconsonants[j - 1];
							break;
						}
					}
				} else {
					for (int j = 0; j < strlen(uvowels); ++j) {
						if (w[i] == uvowels[j]) {
							w[i] = (w[i] == uvowels[0]) 
							? uvowels[strlen(uvowels)-1] : uvowels[j - 1];
							break;
						}
					}
				}
			}
			if (w[i] >= 'a' && w[i] <= 'z') {
				vFound = (int*) bsearch(&*(w + i), lvowels, strlen(lvowels),
				          sizeof(char), compareChar);
				if (!vFound) {
					for (int j = 0; j < strlen(lconsonants); ++j) {
						if (w[i] == lconsonants[j]) {
							w[i] = (w[i] == lconsonants[0]) 
							? lconsonants[strlen(lconsonants)-1] : lconsonants[j - 1];
							break;
						}
					}
				} else {
					for (int j = 0; j < strlen(lvowels); ++j) {
						if (w[i] == lvowels[j]) {
							w[i] = (w[i] == lvowels[0]) 
							? lvowels[strlen(lvowels)-1] : lvowels[j - 1];
							break;
						}
					}
				}
			}
		}
	}
	
	return w;
}

/*
 * name: f_avgad
 * @param string
 * @return string
 */
char *f_avgad(char *word) {
	return avgad(word, 1);
}

/*
 * name: r_avgad
 * @param string
 * @return string
 */
char *r_avgad(char *word) {
	return avgad(word, -1);
}

/*
 * name: atbash
 * @param string
 * @return string
 */
char *atbash(char *word) {
	char *w = strdup(word);
	int lng = strlen(w);
	int *vFound;
	
	for (int i = 0; i < lng; ++i) {
		if (w[i] >= 'A' && w[i] <= 'Z') {
			vFound = (int*) bsearch(&*(w + i), uvowels, strlen(uvowels),
			          sizeof(char), compareChar);
			if (!vFound) {
				for (int j = 0; j < strlen(uconsonants); ++j) {
					if (w[i] == uconsonants[j]) {
						w[i] = uconsonants[(strlen(uconsonants)-1) - j];
						break;
					}
				}
			} else {
				for (int j = 0; j < strlen(uvowels); ++j) {
					if (w[i] == uvowels[j]) {
						w[i] = uvowels[(strlen(uvowels)-1) - j];
						break;
					}
				}
			}
		}
		if (w[i] >= 'a' && w[i] <= 'z') {
			vFound = (int*) bsearch(&*(w + i), lvowels, strlen(lvowels),
			          sizeof(char), compareChar);
			if (!vFound) {
				for (int j = 0; j < strlen(lconsonants); ++j) {
					if (w[i] == lconsonants[j]) {
						w[i] = lconsonants[(strlen(lconsonants)-1) - j];
						break;
					}
				}
			} else {
				for (int j = 0; j < strlen(lvowels); ++j) {
					if (w[i] == lvowels[j]) {
						w[i] = lvowels[(strlen(lvowels)-1) - j];
						break;
					}
				}
			}
		}
	}
	
	return w;
}

/*
 * name: albam
 * @param string
 * @return string
 */
char *albam(char *word) {
	char *w = strdup(word);
	int lng = strlen(w);
	int *vFound;
	
	for (int i = 0; i < lng; ++i) {
		if (w[i] >= 'A' && w[i] <= 'Z') {
			vFound = (int*) bsearch(&*(w + i), uvowels, strlen(uvowels),
			          sizeof(char), compareChar);
			if (!vFound) {
				for (int j = 0; j < strlen(uconsonants); ++j) {
					if (w[i] == uconsonants[j]) {
						if ( j >= strlen(uconsonants)/2 )
							w[i] = uconsonants[j-(strlen(uconsonants)/2)];
						else
							w[i] = uconsonants[j+(strlen(uconsonants)/2)];
						
						break;
					}
				}
			} else {
				for (int j = 0; j < strlen(uvowels); ++j) {
					if (w[i] == uvowels[j]) {
						if ( j >= strlen(uvowels)/2 )
							w[i] = uvowels[j-(strlen(uvowels)/2)];
						else
							w[i] = uvowels[j+(strlen(uvowels)/2)];
						
						break;
					}
				}
			}
		}
		if (w[i] >= 'a' && w[i] <= 'z') {
			vFound = (int*) bsearch(&*(w + i), lvowels, strlen(lvowels),
			          sizeof(char), compareChar);
			if (!vFound) {
				for (int j = 0; j < strlen(lconsonants); ++j) {
					if (w[i] == lconsonants[j]) {
						if ( j >= strlen(lconsonants)/2 )
							w[i] = lconsonants[j-(strlen(lconsonants)/2)];
						else
							w[i] = lconsonants[j+(strlen(lconsonants)/2)];
						
						break;
					}
				}
			} else {
				for (int j = 0; j < strlen(lvowels); ++j) {
					if (w[i] == lvowels[j]) {
						if ( j >= strlen(lvowels)/2 )
							w[i] = lvowels[j-(strlen(lvowels)/2)];
						else
							w[i] = lvowels[j+(strlen(lvowels)/2)];
						
						break;
					}
				}
			}
		}
	}
	
	return w;
}

/*
 * name: aikbekar
 * @param string
 * @return string
 */
char *aikbekar(char *word) {
	char *w = strdup(word);
	int lng = strlen(w);
	int *charFound;
	
	for (int i = 0; i < lng; ++i) {
		if (w[i] >= 'A' && w[i] <= 'Z') {
			for (int rowIndex = 0; rowIndex < 5; ++rowIndex) {
				charFound = (int*) bsearch(&*(w + i), uconsgroup[rowIndex], strlen(uconsgroup[rowIndex]), 
						 sizeof(char), compareChar);
				if (charFound) {
					for (int colIndex = 0; colIndex < strlen(uconsgroup[rowIndex]); ++colIndex) {
						if (w[i] == uconsgroup[rowIndex][colIndex]) {
							w[i] = (w[i] == uconsgroup[rowIndex][strlen(uconsgroup[rowIndex])-1]) 
							? uconsgroup[rowIndex][0] : uconsgroup[rowIndex][colIndex + 1];
							break;
						}
					}
					break;
				}
			}
			for (int rowIndex = 0; rowIndex < 3; ++rowIndex) {
				charFound = (int*) bsearch(&*(w + i), uvowgroup[rowIndex], strlen(uvowgroup[rowIndex]), 
						 sizeof(char), compareChar);
				if (charFound) {
					for (int colIndex = 0; colIndex < strlen(uvowgroup[rowIndex]); ++colIndex) {
						if (w[i] == uvowgroup[rowIndex][colIndex]) {
							w[i] = (w[i] == uvowgroup[rowIndex][strlen(uvowgroup[rowIndex])-1]) 
							? uvowgroup[rowIndex][0] : uvowgroup[rowIndex][colIndex + 1];
							break;
						}
					}
					break;
				}
			}
		}
		if (w[i] >= 'a' && w[i] <= 'z') {
			for (int rowIndex = 0; rowIndex < 5; ++rowIndex) {
				charFound = (int*) bsearch(&*(w + i), lconsgroup[rowIndex], strlen(lconsgroup[rowIndex]), 
						 sizeof(char), compareChar);
				if (charFound) {
					for (int colIndex = 0; colIndex < strlen(lconsgroup[rowIndex]); ++colIndex) {
						if (w[i] == lconsgroup[rowIndex][colIndex]) {
							w[i] = (w[i] == lconsgroup[rowIndex][strlen(lconsgroup[rowIndex])-1]) 
							? lconsgroup[rowIndex][0] : lconsgroup[rowIndex][colIndex + 1];
							break;
						}
					}
					break;
				}
			}
			for (int rowIndex = 0; rowIndex < 3; ++rowIndex) {
				charFound = (int*) bsearch(&*(w + i), lvowgroup[rowIndex], strlen(lvowgroup[rowIndex]), 
						 sizeof(char), compareChar);
				if (charFound) {
					for (int colIndex = 0; colIndex < strlen(lvowgroup[rowIndex]); ++colIndex) {
						if (w[i] == lvowgroup[rowIndex][colIndex]) {
							w[i] = (w[i] == lvowgroup[rowIndex][strlen(lvowgroup[rowIndex])-1]) 
							? lvowgroup[rowIndex][0] : lvowgroup[rowIndex][colIndex + 1];
							break;
						}
					}
					break;
				}
			}
		}
	}
	
	return w;
}

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
		"  -s | --search		 Optional: Text file in which to search for enciphered world or phrase.\n"
		"  -p | --phrases		 Optional: To conduct phrase search, where each resulting phrase has num words.\n\n"
		
		"Usage: temurah [ -s FILE | --search=FILE [-p num | --phrases=num]] -f | --forward-avgad Word | 'Phrase'\n"
		"		        [ -s FILE | --search=FILE [-p num | --phrases=num]] -r | --backward-avgad Word | 'Phrase'\n"
		"		        [ -s FILE | --search=FILE [-p num | --phrases=num]] -t | --atbash Word | 'Phrase'\n"
		"		        [ -s FILE | --search=FILE [-p num | --phrases=num]] -l | --albam Word | 'Phrase'\n"
		"		        [ -s FILE | --search=FILE [-p num | --phrases=num]] -b | --aikbekar Word | 'Phrase'\n"
		"		        [ -s FILE | --search=FILE [-p num | --phrases=num]] -a | --all Word | 'Phrase'\n"
		"===============================================================\n\n");
		exit(EXIT_FAILURE);	
}

int main(int argc, char **argv) {
	int opt, numWordsInPhrase = 2, f_avgadFlag = 0, r_avgadFlag = 0, atbashFlag = 0, 
		albamFlag = 0, aikbekarFlag = 0, searchFlag = 0;
	char temWord[256], *searchFile = NULL;
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
				searchFile = optarg;
			break;
			case 't':
				atbashFlag = 1;
			break;
			break;
			default:
				displayUsage();
		}
	}
	
	if (optind >= argc) fgets(temWord, 256, stdin);
	else strncpy(temWord, argv[optind], 256);
	
	if (f_avgadFlag)	printf("%s\n", f_avgad(temWord));
	if (r_avgadFlag)	printf("%s\n", r_avgad(temWord));
	if (atbashFlag)		printf("%s\n", atbash(temWord));
	if (albamFlag)		printf("%s\n", albam(temWord));
	if (aikbekarFlag)	printf("%s\n", aikbekar(temWord));
	
	if (searchFlag) {
		struct wordlist *searchList = NULL;
		
		if (numWordsInPhrase < 2) numWordsInPhrase = 2;
		if (! (searchList = generateLists(searchFile, ltype, numWordsInPhrase, 1)) ) {
			fprintf(stderr, "Unable to generate search list. %s is either corrupted or not provided.\n", searchFile);
			exit(EXIT_FAILURE);
		}
		
		int i, count;		
		
		if (f_avgadFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncmpi(f_avgad(temWord), searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;
			
			printf("%i\n", count);
		}
		if (r_avgadFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncmpi(r_avgad(temWord), searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;
			
			printf("%i\n", count);
		}
		if (atbashFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncmpi(atbash(temWord), searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;
			
			printf("%i\n", count);
		}
		if (albamFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncmpi(albam(temWord), searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;
			
			printf("%i\n", count);
		}
		if (aikbekarFlag) {
			for (i = 0, count = 0; i < searchList->numwords; i++)
				if (strncmpi(aikbekar(temWord), searchList->words[i], strlen(searchList->words[i])) == 0)
					count++;
			
			printf("%i\n", count);
		}
		
		for (i = 0; i < searchList->numwords; i++)
			free(searchList->words[i]);
		
		free(searchList->words);
		free(searchList);
	}
	
	exit(EXIT_SUCCESS);
}
