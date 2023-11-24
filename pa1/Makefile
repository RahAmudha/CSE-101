Lex : Lex.o List.o
	gcc -g -std=c17 -Wall -o Lex Lex.o List.o

Lex.o : Lex.c List.h
	gcc -g -std=c17 -Wall -c Lex.c

ListClient: ListClient.o List.o
	gcc -g -std=c17 -Wall -o ListClient ListClient.o List.o

ListClient.o : ListClient.c List.h
	gcc -g -std=c17 -Wall -c ListClient.c

ListTest : ListTest.o List.o
	gcc -g -std=c17 -Wall -o ListTest ListTest.o List.o

ListTest.o : ListTest.c List.h
	gcc -g -std=c17 -Wall -c ListTest.c

List.o : List.c List.h
	gcc -g -std=c17 -Wall -c List.c

clean :
	rm -f Lex ListClient Lex.o ListClient.o List.o ListTest.o ListTest

checkClient : 
	valgrind --leak-check=full ./ListClient

checkLex :
	valgrind --leak-check=full ./Lex in3 blah3