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

#include "processing.h"
#include "temurah.h"

static const char const uconsonants[] = "BCDFGHJKLMNPQRSTVWXZ";
static const char const lconsonants[] = "bcdfghjklmnpqrstvwxz";

static const char const uvowels[] = "AEIOUY";
static const char const lvowels[] = "aeiouy";

static const char const *uconsgroup[5] = {
	"BHNT", "CJPV", "DKQW", "FLRX", "GMSZ"
};

static const char const *uvowgroup[3] = {
	"AO", "EU", "IY"
};

static const char const *lconsgroup[5] = {
	"bhnt", "cjpv", "dkqw", "flrx", "gmsz"
};

static const char const *lvowgroup[3] = {
	"ao", "eu", "iy"
};

static int compareChar(const void *a, const void *b) {
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
