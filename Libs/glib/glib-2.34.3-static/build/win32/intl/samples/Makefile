all: sample.exe sample-win9x.exe

sample.exe:
	gcc -c ../win_iconv.c -D_UNICODE -DUNICODE
	g++ -c sample.cpp -D_UNICODE -DUNICODE
	g++ sample.o win_iconv.o -o sample.exe ../libintl.a -luser32 -lshlwapi

sample-win9x.exe:
	gcc -c ../win_iconv.c
	g++ -c sample.cpp
	g++ sample.o win_iconv.o -o sample-win9x.exe ../libintl-win9x.a -luser32 -lshlwapi

clean:
	rm -f *.exe *.o
