bin = bin
OBJ_dir = obj
OBJECTOS = client.o enterprise.o intermediary.o main.o memory.o process.o
CC = gcc

out: $(OBJECTOS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/AdmPor

%.o: src/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

clean:
	$(OBJ_dir) rm –f *.o 
	$(bin) rm –f AdmPor