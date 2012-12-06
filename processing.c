/*
 *      processing.c
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
#ifdef HAVE_CURL_CURL_H
 #include <curl/curl.h>
#endif

/*
 * name: stripNonAlpha
 * @param string
 * @return string
 */
char *stripNonAlpha(char *w) {
	int wl = strlen(w);
	int n = 0;
	char *naTemp = NULL;

	if (w[wl] == '\n') w[wl--] = '\0';

	if (! (naTemp = (char*) malloc(wl)) )
		return NULL;

	for (int i = 0; i < wl; ++i) {
		if ( (w[i] >= 'A' && w[i] <= 'Z') ||
			 (w[i] >= 'a' && w[i] <= 'z') ||
			  w[i] == ' ' )
			naTemp[n++] = w[i];
		else if (w[i] == '\r' || w[i] == '\n')
			naTemp[n++] = ' ';
	}
	naTemp[n] = '\0';

	return naTemp;
}

/*
 * name: stripMarkupTags
 * @param string
 * @return string
 */
char *stripMarkupTags(char *w) {
	int wl = strlen(w);
	int n = 0;
	char *mtTemp = NULL;

	if (! (mtTemp = (char*) malloc(wl)) )
		return NULL;

	for (int i = 0; i < wl; ++i) {
		if (w[i] == '<') {
			if (w[i+1] == 's' &&
				w[i+2] == 'c' &&
				w[i+3] == 'r' &&
				w[i+4] == 'i' &&
				w[i+5] == 'p' &&
				w[i+6] == 't') {
				i += 7;
				while (w[++i] != '<' && w[i] != '\0') continue;
				if (w[i+1] == '/' &&
					w[i+2] == 's' &&
					w[i+3] == 'c' &&
					w[i+4] == 'r' &&
					w[i+5] == 'i' &&
					w[i+6] == 'p' &&
					w[i+7] == 't') i += 7;
			}
			while (w[++i] != '>' && w[i] != '\0') continue;
			if (w[++i] != '<' && w[i] != '\0') mtTemp[n++] = w[i];
			else --i;
		} else mtTemp[n++] = w[i];
	}
	mtTemp[n] = '\0';

	return mtTemp;
}

/*
 * name: charToLowerCase
 * @param char
 * @return char
 */
char charToLowerCase(char c) {
	if (c < 96) c += 32;
	return c;
}

/*
 * name: strToLowerCase
 * @param string
 * @return string
 */
char *strToLowerCase(char *word) {
	int lng = strlen(word);

	for (int i = 0; i < lng; ++i)
		word[i] = charToLowerCase(word[i]);

	return word;
}

/*
 * name: strncmpi
 * @param string, string, size_t
 * @return int
 */

int strncmpi (const char *s1, const char *s2, size_t n) {
	unsigned char c1 = '\0';
	unsigned char c2 = '\0';

	if (n >= 4) {
		size_t n4 = n >> 2;
		do {
			c1 = (unsigned char) charToLowerCase(*s1++);
			c2 = (unsigned char) charToLowerCase(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) charToLowerCase(*s1++);
			c2 = (unsigned char) charToLowerCase(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) charToLowerCase(*s1++);
			c2 = (unsigned char) charToLowerCase(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) charToLowerCase(*s1++);
			c2 = (unsigned char) charToLowerCase(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
		} while (--n4 > 0);
		n &= 3;
    }

	while (n > 0) {
		c1 = (unsigned char) charToLowerCase(*s1++);
		c2 = (unsigned char) charToLowerCase(*s2++);
		if (c1 == '\0' || c1 != c2)
			return c1 - c2;
		n--;
	}

	return c1 - c2;
}

#ifdef HAVE_CURL_CURL_H
/*
 * Curl callback function for processing remote files
 *
 * name: curlCallback
 * @param void*, size_t, size_t, void*
 * @return size_t
 */
static size_t curlCallback(void *contents, size_t size, size_t nmemb, void *data) {
	size_t realsize = size * nmemb;
	struct filebuffer *fb = (struct filebuffer*) data;

	char *smt = stripMarkupTags((char*)contents);
	char *sna = stripNonAlpha(smt);
	size_t sna_size = strlen(sna);
	free(smt);

	if (! (fb->fbuffer = (char*) realloc(fb->fbuffer, fb->fbsize + sna_size + 1)) ) {
		fprintf(stderr, "Can't allocate memory for buffer. Returning 0 bytes.\n");
		return 0;
	}

	strncpy(&(fb->fbuffer[fb->fbsize]), sna, sna_size);
	fb->fbsize += sna_size;
	fb->fbuffer[fb->fbsize] = '\0';
	free(sna);

	return realsize;
}
#endif

/*
 * name: proccessFile
 * @param string
 * @return pointer to struct filebuffer
 */
struct filebuffer *proccessFile(const char *pFile) {
	struct filebuffer *fb = NULL;
	FILE *srchFile;
#ifdef HAVE_CURL_CURL_H
	CURL *webRes;
	if (strncmpi(pFile, "https:", 6) == 0 ||
	    strncmpi(pFile, "http:", 5) == 0) {
		fb = (struct filebuffer*) malloc(sizeof(struct filebuffer));
		fb->fbuffer = (char*) malloc(1);
		fb->fbsize = 0;
		curl_global_init(CURL_GLOBAL_ALL);
		webRes = curl_easy_init();
		curl_easy_setopt(webRes, CURLOPT_URL, pFile);
		curl_easy_setopt(webRes, CURLOPT_WRITEFUNCTION, curlCallback);
		curl_easy_setopt(webRes, CURLOPT_WRITEDATA, (void*) fb);
		curl_easy_setopt(webRes, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_perform(webRes);
		curl_easy_cleanup(webRes);
		curl_global_cleanup();
	} else
#endif
	if ((srchFile = fopen(pFile, "r"))) {
		fb = (struct filebuffer*) malloc(sizeof(struct filebuffer));
		char *tmpbuff = NULL;

		fseek(srchFile, 0, SEEK_END);
	  	fb->fbsize = ftell(srchFile);
	  	rewind(srchFile);

		if (! (tmpbuff = (char*) malloc(sizeof(char) * fb->fbsize)) ) {
			fprintf(stderr, "Can't allocate memory for temporary buffer for file %s.\n", pFile);
			return NULL;
		}

	  	if (fread(tmpbuff, sizeof(char), fb->fbsize, srchFile) != fb->fbsize) {
			fprintf(stderr, "Can't read file %s.\n", pFile);
			return NULL;
		}

		fclose(srchFile);

		char *snabuff = stripNonAlpha(tmpbuff);
		size_t sna_size = strlen(snabuff);
		free(tmpbuff);

		if (! (fb->fbuffer = (char*) malloc(sizeof(char) * sna_size)) ) {
			fprintf(stderr, "Can't allocate memory for buffer.\n");
			return NULL;
		}

	  	strncpy(fb->fbuffer, snabuff, sna_size);
	  	fb->fbsize = sna_size;

	  	free(snabuff);
	}

  	return fb;
}

/*
 * name: generateLists
 * @param string, enum listtype, int, int
 * @return pointer to struct wordlist
 */
struct wordlist *generateLists(const char *pFile, enum listtype ltype, int phraseNumWords, int removeDuplicates) {
	if (phraseNumWords < 0) phraseNumWords = 2;

	/* ********** Begin File Process ********** */
	struct filebuffer *FB = NULL;
	if (! (FB = proccessFile(pFile)) ) return NULL;
	/* ********** End File Process ********** */

	/* ********** Begin Lists Collection ********** */
	int i, j, found;
	struct wordlist *unsortedWords = NULL;
	struct wordlist *unsortedPhrases = NULL;
	struct wordlist *uniqueWords = NULL;
	struct wordlist *uniquePhrases = NULL;

	if (! (unsortedWords = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;

	if (! (unsortedWords->words = (char**) calloc(FB->fbsize/sizeof(char*), sizeof(char*))) )
		return NULL;

	unsortedWords->words[0] = strtok(FB->fbuffer, " ");
	for (unsortedWords->numwords = 0, i = 1; i < FB->fbsize; ++i) {
		if ( !(unsortedWords->words[i] = strtok(NULL, " ")) ) break;
		unsortedWords->numwords++;
	}

	if (ltype == phrasetype) {
		if (! (unsortedPhrases = (struct wordlist*) malloc(sizeof(struct wordlist))) )
			return NULL;

		if (! (unsortedPhrases->words = (char**) calloc(unsortedWords->numwords, sizeof(char*))) )
			return NULL;

		if (unsortedWords->numwords < phraseNumWords)
			phraseNumWords = unsortedWords->numwords;

		int phraseIndices = phraseNumWords - 1;

		int phraseSize[unsortedWords->numwords];
		for (i = phraseIndices; i < unsortedWords->numwords; ++i) {
			phraseSize[i] = 0;
			for (j = phraseIndices; j > -1; --j)
				phraseSize[i] += strlen(unsortedWords->words[i-j])+1;
		}

		for (unsortedPhrases->numwords = 0, i = phraseIndices; i < unsortedWords->numwords; ++i) {
			if (! (unsortedPhrases->words[i-phraseIndices] = (char*) malloc(phraseSize[i] * sizeof(char))) )
				return NULL;

			unsortedPhrases->words[i-phraseIndices][0] = '\0';

			for (j = phraseIndices; j > -1; --j) {
				if (j < phraseIndices)
					strncat(unsortedPhrases->words[i-phraseIndices], " ", 1);

				strncat(unsortedPhrases->words[i-phraseIndices], unsortedWords->words[i-j], phraseSize[i-j]);
			}
			unsortedPhrases->numwords++;
		}
	}
	/* ********** End Lists Collection ********** */

	/* ********** Begin Lists Sort ********** */
	if (! (uniqueWords = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;

	if (! (uniqueWords->words = (char**) calloc(unsortedWords->numwords, sizeof(char*))) )
		return NULL;

	for (uniqueWords->numwords = 0, i = 0; i < unsortedWords->numwords; ++i) {
		found = 0;
		for (j = 0; j < unsortedWords->numwords; ++j) {
			if (uniqueWords->words[j] == NULL) break;
			if (strncmpi(uniqueWords->words[j], unsortedWords->words[i], strlen(unsortedWords->words[i])) == 0) {
				found = 1; break;
			}
		}
		if (!found) {
			uniqueWords->words[j] = strdup(unsortedWords->words[i]);
			uniqueWords->numwords++;
		}
	}

	if (ltype == phrasetype) {
		if (! (uniquePhrases = (struct wordlist*) malloc(sizeof(struct wordlist))) )
			return NULL;

		if (! (uniquePhrases->words = (char**) calloc(unsortedPhrases->numwords, sizeof(char*))) )
			return NULL;

		for (uniquePhrases->numwords = 0, i = 0; i < unsortedPhrases->numwords; ++i) {
			found = 0;
			for (j = 0; j < unsortedPhrases->numwords; ++j) {
				if (uniquePhrases->words[j] == NULL) break;
				if (strncmpi(uniquePhrases->words[j], unsortedPhrases->words[i], strlen(unsortedPhrases->words[i])) == 0) {
					found = 1; break;
				}
			}
			if (!found) {
				uniquePhrases->words[j] = strdup(unsortedPhrases->words[i]);
				uniquePhrases->numwords++;
			}
		}
	}
	/* ********** End Lists Sort ********** */

	struct wordlist *collection = NULL;

	if (removeDuplicates) {
		collection = (ltype == phrasetype) ? uniquePhrases : uniqueWords;

		free(unsortedWords->words);
		free(unsortedWords);

		if (ltype == phrasetype) {
			for (i = 0; i < unsortedPhrases->numwords; ++i)
				free(unsortedPhrases->words[i]);

			free(unsortedPhrases->words);
			free(unsortedPhrases);
		}
	} else {
		collection = (ltype == phrasetype) ? unsortedPhrases : unsortedWords;

		for (i = 0; i < uniqueWords->numwords; ++i)
			free(uniqueWords->words[i]);

		free(uniqueWords->words);
		free(uniqueWords);

		if (ltype == phrasetype) {
			for (i = 0; i < uniquePhrases->numwords; ++i)
				free(uniquePhrases->words[i]);

			free(uniquePhrases->words);
			free(uniquePhrases);
		}
	}

	free(FB->fbuffer);
	free(FB);

	return collection;
}
