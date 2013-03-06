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
			if (tolower(w[i+1]) == 's' &&
				tolower(w[i+2]) == 'c' &&
				tolower(w[i+3]) == 'r' &&
				tolower(w[i+4]) == 'i' &&
				tolower(w[i+5]) == 'p' &&
				tolower(w[i+6]) == 't') {
				i += 7;
				while (w[++i] != '<' && w[i] != '\0') continue;
				if (w[i+1] == '/' &&
					tolower(w[i+2]) == 's' &&
					tolower(w[i+3]) == 'c' &&
					tolower(w[i+4]) == 'r' &&
					tolower(w[i+5]) == 'i' &&
					tolower(w[i+6]) == 'p' &&
					tolower(w[i+7]) == 't') i += 7;
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
			c1 = (unsigned char) tolower(*s1++);
			c2 = (unsigned char) tolower(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) tolower(*s1++);
			c2 = (unsigned char) tolower(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) tolower(*s1++);
			c2 = (unsigned char) tolower(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
			c1 = (unsigned char) tolower(*s1++);
			c2 = (unsigned char) tolower(*s2++);
			if (c1 == '\0' || c1 != c2)
				return c1 - c2;
		} while (--n4 > 0);
		n &= 3;
    }

	while (n > 0) {
		c1 = (unsigned char) tolower(*s1++);
		c2 = (unsigned char) tolower(*s2++);
		if (c1 == '\0' || c1 != c2)
			return c1 - c2;
		n--;
	}

	return c1 - c2;
}

#if defined(HAVE_CURL_CURL_H) && (defined(HAVE_LIBCURL) || defined(HAVE_LIBCURLDLL))
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
#if defined(HAVE_CURL_CURL_H) && (defined(HAVE_LIBCURL) || defined(HAVE_LIBCURLDLL))
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

		char *smt = stripMarkupTags(tmpbuff);
		char *sna = stripNonAlpha(smt);
		size_t sna_size = strlen(sna);

		free(smt);
		free(tmpbuff);

		if (! (fb->fbuffer = (char*) malloc(sizeof(char) * sna_size)) ) {
			fprintf(stderr, "Can't allocate memory for buffer.\n");
			return NULL;
		}

	  	strncpy(fb->fbuffer, sna, sna_size);
	  	fb->fbsize = sna_size;

	  	free(sna);
	}

  	return fb;
}

/*
 * name: generateWordList
 * @param pointer to struct wordlist, string
 * @return pointer to struct wordlist
 */
struct wordlist *generateWordList(struct wordlist *wList, char *str) {
	if (!strlen(str))
		return NULL;

	if (wList == NULL) {
		if (! (wList = (struct wordlist*) malloc(sizeof(struct wordlist))) )
			return NULL;

		wList->words = NULL;
		wList->numwords = 0;
		wList->size = 0;
	}

	wList->size += strlen(str);

	if (! (wList->words = (char**) realloc(wList->words, wList->size)) )
		return NULL;

	char *tok = NULL;

	while ( (tok = strtok(tok? NULL : str, " ")) )
		wList->words[wList->numwords++] = strdup(tok);

	return wList;
}

/*
 * name: generateWordListFromFile
 * @param pointer to struct wordlist, string
 * @return pointer to struct wordlist
 */
struct wordlist *generateWordListFromFile(struct wordlist *wList, const char *pFile) {
	struct filebuffer *FB = NULL;
	if (! (FB = proccessFile(pFile)) ) return NULL;

	wList = generateWordList(wList, FB->fbuffer);

	free(FB->fbuffer);
	free(FB);

	return wList;
}

/*
 * name: generateUniqueWordList
 * @param pointer to struct wordlist
 * @return pointer to struct wordlist
 */
struct wordlist *generateUniqueWordList(struct wordlist *wlist) {
	struct wordlist *uniqueWords = NULL;
	int i, j, found;

	if (! (uniqueWords = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;

	if (! (uniqueWords->words = (char**) calloc(wlist->numwords, sizeof(char*))) )
		return NULL;

	for (uniqueWords->numwords = 0, i = 0; i < wlist->numwords; ++i) {
		found = 0;
		for (j = 0; j < wlist->numwords; ++j) {
			if (uniqueWords->words[j] == NULL) break;
			if (strncmpi(uniqueWords->words[j], wlist->words[i], strlen(wlist->words[i])) == 0) {
				found = 1; break;
			}
		}
		if (!found) {
			uniqueWords->words[j] = strdup(wlist->words[i]);
			uniqueWords->numwords++;
		}
	}

	return uniqueWords;
}

/*
 * name: generatePhraseList
 * @param pointer to struct wordlist, int
 * @return pointer to struct wordlist
 */
struct wordlist *generatePhraseList(struct wordlist *wlist, int phraseNumWords) {
	struct wordlist *unsortedPhrases = NULL;
	int i, j;
	if (phraseNumWords < 2) phraseNumWords = 2;

	if (! (unsortedPhrases = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;

	if (! (unsortedPhrases->words = (char**) calloc(wlist->numwords, sizeof(char*))) )
		return NULL;

	if (wlist->numwords < phraseNumWords)
		phraseNumWords = wlist->numwords;

	int phraseIndices = phraseNumWords - 1;

	int phraseSize[wlist->numwords];
	for (i = phraseIndices; i < wlist->numwords; ++i) {
		phraseSize[i] = 0;
		for (j = phraseIndices; j > -1; --j)
			phraseSize[i] += strlen(wlist->words[i-j])+1;
	}

	for (unsortedPhrases->numwords = 0, i = phraseIndices; i < wlist->numwords; ++i) {
		if (! (unsortedPhrases->words[i-phraseIndices] = (char*) malloc(phraseSize[i] * sizeof(char))) )
			return NULL;

		unsortedPhrases->words[i-phraseIndices][0] = '\0';

		for (j = phraseIndices; j > -1; --j) {
			if (j < phraseIndices)
				strncat(unsortedPhrases->words[i-phraseIndices], " ", 1);

			strncat(unsortedPhrases->words[i-phraseIndices], wlist->words[i-j], phraseSize[i-j]);
		}
		unsortedPhrases->numwords++;
	}

	return unsortedPhrases;
}

/*
 * name: generateUniquePhraseList
 * @param pointer to struct wordlist
 * @return pointer to struct wordlist
 */
struct wordlist *generateUniquePhraseList(struct wordlist *plist) {
	struct wordlist *uniquePhrases = NULL;
	int i, j, found;

	if (! (uniquePhrases = (struct wordlist*) malloc(sizeof(struct wordlist))) )
		return NULL;

	if (! (uniquePhrases->words = (char**) calloc(plist->numwords, sizeof(char*))) )
		return NULL;

	for (uniquePhrases->numwords = 0, i = 0; i < plist->numwords; ++i) {
		found = 0;
		for (j = 0; j < plist->numwords; ++j) {
			if (uniquePhrases->words[j] == NULL) break;
			if (strncmpi(uniquePhrases->words[j], plist->words[i], strlen(plist->words[i])) == 0) {
				found = 1; break;
			}
		}
		if (!found) {
			uniquePhrases->words[j] = strdup(plist->words[i]);
			uniquePhrases->numwords++;
		}
	}

	return uniquePhrases;
}

/*
 * name: listToString
 * @param pointer to struct wordlist, int
 * @return string
 */
char *listToString(struct wordlist *wList, int index) {
	char *buf = NULL;
	size_t bufsize = 0;

	while (index < wList->numwords) {
		buf = (char*) realloc(buf, bufsize + strlen(wList->words[index]) + 2);
		strncpy(&buf[bufsize], wList->words[index], strlen(wList->words[index]));
		bufsize += strlen(wList->words[index]);
		buf[bufsize] = ' ';
		buf[++bufsize] = '\0';
		index++;
	}

	return buf;
}

/*
 * name: clearWordList
 * @param pointer to struct wordlist
 * @return void
 */
void clearWordList(struct wordlist *list) {
	if (list) {
		for (int i = 0; i < list->numwords; i++)
			free(list->words[i]);

		list->numwords = 0;
		free(list);
	}
	list = NULL;
}
