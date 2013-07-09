/*
 *      anagram.c
 *
 *      Copyright 2013 Marc Sylvestre <marc.sylvestre@manhydra.com>
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
 *      along with this program; if not, see <http://www.gnu.org/licenses/>,
 *      or if prefer good old fashion postal mail, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "processing.h"
#include "anagram.h"

static int compareChar(const void *a, const void *b) {
	return ( *(char*)a - *(char*)b );
}

/*
 * name: anagram
 * @param string, pointer to struct wordlist
 * @return void
 */
void anagram(char *word, struct wordlist *searchList) {
	size_t length = strlen(word);
	size_t mallocdLength = length + 5;

	char *temp = stripSpace(word);
	strToLowerCase(temp);
	int tempLength = strlen(temp);

	char unSortedWordPhrase[mallocdLength];
	char sortedWordPhrase[mallocdLength];

	size_t unSortedWordLength, sortedWordLength;
	int unSortRemainingLength, sortRemainingLength, i;

	if (length) {
		qsort(temp, tempLength, sizeof(char), compareChar);

		for (int index = 0; index < searchList->numwords; index++) {
			unSortedWordLength = 0;
			sortedWordLength = 0;
			unSortRemainingLength = length;
			sortRemainingLength = tempLength;

			i = index;

			while ((unSortRemainingLength > 0) && (i < searchList->numwords)) {
				if (unSortedWordLength) {
					strncat(unSortedWordPhrase, " ", sizeof(char));
					unSortRemainingLength -= sizeof(char);
				}

				unSortedWordLength = strlen(searchList->words[i]);
				sortedWordLength = unSortedWordLength;

				strncat(
					unSortedWordPhrase,
				    searchList->words[i],
				    (unSortedWordLength > unSortRemainingLength) ?
				    unSortRemainingLength : unSortedWordLength
				);
				strncat(
					sortedWordPhrase,
					searchList->words[i++],
				    (sortedWordLength > sortRemainingLength) ?
				    sortRemainingLength : sortedWordLength
				);

				unSortRemainingLength -= unSortedWordLength;
				sortRemainingLength -= sortedWordLength;
			}

			strToLowerCase(sortedWordPhrase);
			qsort(sortedWordPhrase, tempLength, sizeof(char), compareChar);

			if (strncmp(temp, sortedWordPhrase, tempLength) == 0)
				printf("%s\n", unSortedWordPhrase);

			memset(unSortedWordPhrase, '\0', mallocdLength);
			memset(sortedWordPhrase, '\0', mallocdLength);
		}
	}

	free(temp);
}
