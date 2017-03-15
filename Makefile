CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -o main main.c

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

cache-miss: main
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./main


clean:
	$(RM) main
