#
# Build libhashcash
#

lib_LIBRARIES		= libhashcash.a
libhashcash_a_CPPFLAGS	= -DREGEXP_POSIX -I${srcdir}/../libgnu
libhashcash_a_SOURCES	= \
  libhc.c utct.c sdb.c lock.c sstring.c random.c libsha1.c	\
  libfastmint.c fastmint_mmx_standard_1.c			\
  fastmint_mmx_compact_1.c fastmint_ansi_compact_1.c		\
  fastmint_ansi_standard_1.c fastmint_ansi_compact_2.c		\
  fastmint_ansi_standard_2.c fastmint_altivec_standard_1.c	\
  fastmint_altivec_standard_2.c fastmint_altivec_compact_2.c	\
  fastmint_ansi_ultracompact_1.c fastmint_library.c

TESTS			= sha1test
check_PROGRAMS		= $(TESTS)
sha1test_LDADD		= libsha1.o

include_HEADERS		= hashcash.h
noinst_HEADERS		= \
  hashcash.h libfastmint.h lock.h sdb.h utct.h sha1.h	\
  sstring.h timer.h types.h random.h
