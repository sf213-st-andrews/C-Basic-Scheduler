CC	= gcc

all: printchars simple_calculator sched batch_sched

programs: printchars simple_calculator

# sched: starter.c
# 	$(CC) -o sched starter.c

# sched: first_sched.c
# 	$(CC) -o sched first_sched.c

# sched: second_sched.c
# 	$(CC) -o sched second_sched.c

# sched: third_sched.c
# 	$(CC) -o sched third_sched.c

sched: fourth_sched.c
	$(CC) -o sched fourth_sched.c

batch_sched: third_sched.c
	$(CC) -o batch_sched third_sched.c

printchars: printchars.c
	$(CC) -o printchars printchars.c

simple_calculator: simple_calculator.c
	$(CC) -o simple_calculator simple_calculator.c

test: test.c
	$(CC) -o test test.c


# process.o: process.c
# 	$(CC) -c process.o process.c
	

clean:
	rm -f *.o printchars sched simple_calculator test
