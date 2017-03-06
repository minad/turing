C++ = g++ -Wall
PROG = nn

turing:
	$(C++) -o turing *.cpp

clean:
	rm -f core *~ turing 
