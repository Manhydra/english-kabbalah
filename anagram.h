/*
 *      anagram.h
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

#ifndef __ANAGRAM_H__
#define __ANAGRAM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Outputs a list of words or phrases from a given file(s) where
 * each letter of the inputted word or phrase, and the quantity
 * thereof, is found.
 *
 * name: anagram
 * @param string, pointer to struct wordlist
 * @return void
 */
void anagram(char*, struct wordlist*);

#endif /* __ANAGRAM_H__ */
