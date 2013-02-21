AM_CFLAGS = -Wall -std=gnu99
LIBS = $(LIBCURL)

bin_PROGRAMS = gematria temurah notarikon
gematria_SOURCES = processing.c gematria.c eq-gematria.c
temurah_SOURCES = processing.c temurah.c eq-temurah.c
notarikon_SOURCES = processing.c notarikon.c eq-notarikon.c

man_MANS = doc/eng-qbl.1 doc/gematria.1 doc/temurah.1 doc/notarikon.1
EXTRA_DIST = sample-texts/bible-books.tar.gz \
			 sample-texts/bible.txt.tar.gz \
			 sample-texts/madd.txt \
			 doc \
			 win32
