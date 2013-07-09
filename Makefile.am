AM_CFLAGS = -Wall -std=gnu99
LIBS = $(LIBCURL)

bin_PROGRAMS = gematria temurah notarikon anagram
gematria_SOURCES = processing.c processing.h gematria.c gematria.h eq-gematria.c
temurah_SOURCES = processing.c processing.h temurah.c temurah.h eq-temurah.c
notarikon_SOURCES = processing.c processing.h notarikon.c notarikon.h eq-notarikon.c
anagram_SOURCES = processing.c processing.h anagram.c anagram.h eq-anagram.c

if BUILDENGQBL
bin_PROGRAMS += eng-qbl
eng_qbl_SOURCES = processing.c gematria.c temurah.c notarikon.c anagram.c eng-qbl.c
eng_qbl_LDADD = $(LIBREADLINE)
eng_qbl_DEPENDENCIES = $(LIBREADLINE)
endif

man_MANS = doc/english-kabbalah.1 doc/eng-qbl.1 doc/gematria.1 doc/temurah.1 doc/notarikon.1  doc/anagram.1
EXTRA_DIST = sample-texts/bible-books.tar.gz \
			 sample-texts/bible.txt.tar.gz \
			 sample-texts/madd.txt \
			 doc \
			 win32
