/*
 *      gematria.c
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
#include "gematria.h"

/*
 * name: numcode
 * @param char
 * @return unsigned int
 */
unsigned int numCode(char c) {
	char upperalpha = 64, loweralpha = 96;
	if (c >= 'A' && c <= 'Z')
		return (unsigned int)((c - upperalpha) * GEMOFFSET);
	if (c >= 'a' && c <= 'z')
		return (unsigned int)((c - loweralpha) * GEMOFFSET);

	return 0;
}

/*
 * name: charcode
 * @param int
 * @return char
 */
char charCode(int c) {
	int alphacase[] = {64,96};
	int alphacaseswitch = rand() % 2;
	return (char)((c / GEMOFFSET) + alphacase[alphacaseswitch]);
}

/*
 * name: numeric_value
 * @param string
 * @return unsigned int
 */
unsigned int gemNumericValue(char *word) {
	int i;
	unsigned int value = 0;
	for (i = 0; i < strlen(word); ++i)
		value += numCode(word[i]);
	return value;
}
