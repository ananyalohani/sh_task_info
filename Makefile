#Do Not Change This File
compile:
	gcc test.c

run: compile
	./a.out

clean:
	rm a.out task_details.txt