sample: sample.c
	gcc -o sample sample.c -pthread -Wall

clean:
	rm -rf *.o sample
