bin = bin
OBJ_dir = obj
OBJECTOS = client.o enterprise.o intermediary.o main.o memory.o process.o synchronization.o
CC = gcc

out: $(OBJECTOS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/AdmPor

%.o: src/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f $(OBJ_dir)/*.o
	rm -rf $(bin)/AdmPor 