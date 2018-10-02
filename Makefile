a.out: project1.c
	gcc -Wall project1.c -lpthread
clean:
	rm *.out
