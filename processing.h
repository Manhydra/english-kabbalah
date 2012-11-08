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
#include <string.h>
#include "config.h"


/* Structure to store information on file stream and size. */
struct filebuffer {
	char *fbuffer;
	long int fbsize;
	int fbtype; /* 0 = local, 1 = remote */
};

/* 
 * Structure to store a list of words and the total number of words in 
 * the list.
 */
struct wordlist {
	char **words;
	size_t numwords;
};

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
 * Covert upper cased letters to lower case while leaving already lower
 * case alone.
 * 
 * name: charToLowerCase
 * @param char
 * @return char
 */
char charToLowerCase(char);

/*
 * Covert upper cased letters to lower case in string while leaving 
 * already lower case alone.
 * 
 * name: strToLowerCase
 * @param string
 * @return string
 */
char *strToLowerCase(char*);

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
 * Constant for wordlist types
 */
enum listtype { wordtype, phrasetype };

/*
 * Generates a list of unique words and phrases from a given file.
 * 
 * name: generateLists
 * @param string, enum listtype, int, int
 * @return pointer to struct wordlist
 */
struct wordlist *generateLists(const char*, enum listtype, int, int);

#endif /* __PROCESSING_H__ */
