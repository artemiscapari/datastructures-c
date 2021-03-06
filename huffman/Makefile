# Use C11, debugging and warnings flags
define CFLAGS
-D_POSIX_C_SOURCE=200809L \
-std=c11 \
-g3 \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes
endef

LDFLAGS = -fsanitize=address

SOURCES = \
	read_data.c load_tree.c compute_frequencies.c compute_tree.c \
	print_tree.c compute_code_table.c print_code_table.c encode_data.c \
	count_tree.c make_tree.c fixed_tree.c print_code.c stack.c

HEADERS = huffman.h stack.h

all: encode decode

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

encode: encode.o $(SOURCES:.c=.o)
	$(CC) -o $@ $^ $(LDFLAGS)

decode: decode.o $(SOURCES:.c=.o)
	$(CC) -o $@ $^ $(LDFLAGS)

tarball: huffman_submit.tar.gz

huffman_submit.tar.gz: $(SOURCES) $(HEADERS) Makefile encode.c decode.c
	tar -czf $@ $^

clean:
	rm -f encode decode
	rm -f *.o *~
