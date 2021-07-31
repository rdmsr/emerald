.DEFAULT_TARGET: test

test: test.c
	gcc test.c -L../../build -l:libemerald.a -I ../lib -DHOST -o $@

run: 
	@./test
clean:
	rm -rf test
