#Do Not Change This File
compile: clean
	gcc test.c

run: compile
	./a.out

clean:
	rm a.out task_details.txt