/*
 *      temurah.h
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

#ifndef __TEMURAH_H__
#define __TEMURAH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Replaces each letter of the English alphabet with the one that comes
 * after it when shiftmode is 1. The process is reversed when shiftmode
 * is -1.
 *
 * Letters are categorized into two groups: consonants and vowels, each
 * in two classes: uppercase and lowercase. Thus, letters are replaced
 * with those in their respective group and class.
 *
 * name: avgad
 * @param string, int
 * @return string
 */
char *avgad(char*, int);

/*
 * Wrapper for avgad for forward (positive) shiftmode.
 *
 * name: favgad
 * @param string
 * @return string
 */
char *f_avgad(char*);

/*
 * Wrapper for avgad for backward (negative) shiftmode.
 *
 * name: ravgad
 * @param string
 * @return string
 */
char *r_avgad(char*);

/*
 * Replaces the first letter of the English alphabet with the last letter,
 * the second with the next-to-last, and so on.
 *
 * Letters are categorized into two groups: consonants and vowels, each
 * in two classes: uppercase and lowercase. Thus, letters are replaced
 * with those in their respective group and class.
 *
 * name: atbash
 * @param string
 * @return string
 */
char *atbash(char*);

/*
 * Replaces the first letter of the English alphabet with the twelfth, the second
 * with the thirteenth, and so on.
 *
 * Letters are categorized into two groups: consonants and vowels, each
 * in two classes: uppercase and lowercase. Thus, letters are replaced
 * with those in their respective group and class.
 *
 * name: albam
 * @param string
 * @return string
 */
char *albam(char*);

/*
 * Replaces each letter of the English alphabet with the next found
 * within that letter's group.
 *
 * Letters are categorized into two groups: consonants and vowels, each
 * in two classes: uppercase and lowercase. Thus, letters are replaced
 * with those in their respective group and class.
 *
 * name: aikbekar
 * @param string
 * @return string
 */
char *aikbekar(char*);

#endif /* __TEMURAH_H__  */
