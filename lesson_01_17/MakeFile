mylib=libcal.a
CC=gcc 

$(mylib):add.o sub.o
	$(CC) -shared -o $(mylib) $^
%.o:%.c
	$(CC) -fPIC -c $<

.PHONY:clean 
clean:
	rm -f $(mylib) ./*.o

.PHONY:output
output:
	mkdir -p mlib/include
	mkdir -p mlib/lib 
	cp ./*.h mlib/include
	cp ./*.so mlib/lib 

