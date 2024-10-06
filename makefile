build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

valgrind:
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe

run_fast:
	./app.exe -f

run_custom1:
	./app.exe -y 2016 -q

run_custom2:
	./app.exe -y 2008 -f

built_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe

run_test:
	./test.exe