/*
 *      gematria.h
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

#ifndef __GEMATRIA_H__
#define __GEMATRIA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEMOFFSET 6

/*
 * Returns the English Gematria numeric value of an alphabet
 * 
 * name: numcode
 * @param char
 * @return unsigned int
 */
unsigned int numCode(char);

/*
 * Returns the apha-character based on the English Gematria numeric value
 * 
 * name: charcode
 * @param int
 * @return char
 */
char charCode(int);

/*
 * Returns the English Gematria numeric value of a string
 * 
 * name: numeric_value
 * @param string
 * @return unsigned int
 */
unsigned int gemNumericValue(char*);

#endif /* __GEMATRIA_H__  */
