/*
 *      processing.h
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

#ifndef __PROCESSING_H__
#define __PROCESSING_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "config.h"

/* Structure to store information on file stream and size. */
struct filebuffer {
	char *fbuffer;
	long int fbsize;
};

/*
 * Structure to store a list of words and the total number of words in
 * the list.
 */
struct wordlist {
	char **words;
	size_t numwords;
	size_t size;
};

/*
 * Constant for wordlist types
 */
enum listtype { wordtype, phrasetype };

/*
 * Returns string containing only alphabet characters
 * name: stripNonAlpha
 * @param string
 * @return string
 */
char *stripNonAlpha(char*);

/*
 * Returns string containing no potential hypertext markup (e.g., <>)
 * name: stripMarkupTags
 * @param string
 * @return string
 */
char *stripMarkupTags(char*);

/*
 * Based on strncmp, compares strings using case insensitivy for the
 * purpose of eliminating traces of duplicate words (A = a, Z = z, etc.).
 *
 * name: strncmpi
 * @param string, string, size_t
 * @return int
 */
int strncmpi (const char*, const char*, size_t);

/*
 * Processes a file stream to retrieve its content and size.
 *
 * name: filebuffer
 * @param string
 * @return pointer to struct filebuffer
 */
struct filebuffer *proccessFile(const char*);

/*
 * Generates a list of words from a given string and loads the list onto
 * an existing wordlist. If the wordlist is NULL, a new one is created.
 *
 * name: generateWordList
 * @param pointer to struct wordlist, string
 * @return pointer to struct wordlist
 */
struct wordlist *generateWordList(struct wordlist*, char*);

/*
 * Generates a list of words from a given filename and loads the list onto
 * an existing wordlist. If the wordlist is NULL, a new one is created.
 *
 * name: generateWordList
 * @param pointer to struct wordlist, string
 * @return pointer to struct wordlist
 */
struct wordlist *generateWordListFromFile(struct wordlist*, const char*);

/*
 * Generates a list of uniquely sorted words from a given list of words.
 *
 * name: generateUniqueWordList
 * @param pointer to struct wordlist
 * @return pointer to struct wordlist
 */
struct wordlist *generateUniqueWordList(struct wordlist*);

/*
 * Generates a list of phrases, each of which are of a specified
 * number of words, from a given list of words.
 *
 * name: generatePhraseList
 * @param pointer to struct wordlist, int
 * @return pointer to struct wordlist
 */
struct wordlist *generatePhraseList(struct wordlist*, int);

/*
 * Generates a list of uniquely sorted phrases from a given list of phrases.
 *
 * name: generateUniquePhraseList
 * @param pointer to struct wordlist
 * @return pointer to struct wordlist
 */
struct wordlist *generateUniquePhraseList(struct wordlist*);

/*
 * Returns a list of words as a string.
 *
 * name: listToString
 * @param pointer to struct wordlist, int
 * @return string
 */
char *listToString(struct wordlist*, int);

/*
 * Destroys wordlist items, freeing memory
 *
 * name: clearWordList
 * @param pointer to struct wordlist
 * @return void
 */
void clearWordList(struct wordlist*);

#endif /* __PROCESSING_H__ */
