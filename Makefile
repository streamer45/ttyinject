all:
	gcc ttyinject.c -Wall -Werror -O2 -o ttyinject

clean:
	rm ttyinject
