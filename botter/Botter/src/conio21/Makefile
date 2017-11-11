CC=gcc
CXX=g++
AR=ar
RANLIB=ranlib

all: conio_test.exe conio_test_unicode.exe constream_test.exe

# 64bit versions
all64: conio_test_64.exe conio_test_unicode_64.exe constream_test_64.exe

conio.o: conio.c conio2.h
	$(CC) -Wall -O3 -s -c conio.c -o conio.o -m32

conio_unicode.o: conio.c conio2.h
	$(CC) -Wall -O3 -s -c conio.c -o conio_unicode.o -DUNICODE -m32

conio_test.exe: conio_test.c libconio.a
	$(CC) -Wall -O3 -s conio_test.c -o conio_test.exe -lconio -L. -I. -m32

conio_test_unicode.exe: conio_test.c libconio_unicode.a
	$(CC) -Wall -O3 -s conio_test.c -o conio_test_unicode.exe -lconio_unicode -L. -I. -DUNICODE -m32

constream_test.exe: constream_test.cpp constream libconio.a
	$(CXX) -Wall -O3 -s constream_test.cpp -o constream_test.exe -lconio -L. -I. -m32

libconio.a: conio.o
	$(AR) r libconio.a conio.o
	$(RANLIB) libconio.a

libconio_unicode.a: conio_unicode.o
	$(AR) r libconio_unicode.a conio_unicode.o
	$(RANLIB) libconio_unicode.a

conio_64.o: conio.c conio2.h
	$(CC) -Wall -O3 -s -c conio.c -o conio_64.o -m64

conio_unicode_64.o: conio.c conio2.h
	$(CC) -Wall -O3 -s -c conio.c -o conio_unicode_64.o -DUNICODE -m64

conio_test_64.exe: conio_test.c libconio_64.a
	$(CC) -Wall -O3 -s conio_test.c -o conio_test_64.exe -lconio_64 -L. -I. -m64

conio_test_unicode_64.exe: conio_test.c libconio_unicode_64.a
	$(CC) -Wall -O3 -s conio_test.c -o conio_test_unicode_64.exe -lconio_unicode_64 -L. -I. -DUNICODE -m64

constream_test_64.exe: constream_test.cpp constream libconio_64.a
	$(CXX) -Wall -O3 -s constream_test.cpp -o constream_test_64.exe -lconio_64 -L. -I. -m64

libconio_64.a: conio_64.o
	$(AR) r libconio_64.a conio_64.o
	$(RANLIB) libconio_64.a

libconio_unicode_64.a: conio_unicode_64.o
	$(AR) r libconio_unicode_64.a conio_unicode_64.o
	$(RANLIB) libconio_unicode_64.a

clean:
	rm -f *.o libconio*.a *.exe
