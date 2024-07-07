all: main.o check_functions.o drawing_figures_functions.o drawing_line_functions.o input_functions.o mirror_function.o parse_functions.o png_functions.o outside_ornament.o
	gcc  main.o check_functions.o drawing_figures_functions.o drawing_line_functions.o input_functions.o mirror_function.o parse_functions.o png_functions.o outside_ornament.o -o cw -lpng -lm
main.o: main.c
	gcc -c main.c
check_functions.o: check_functions.c check_functions.h
	gcc -c check_functions.c
drawing_figures_functions.o: drawing_figures_functions.c drawing_line_functions.h
	gcc -c drawing_figures_functions.c
drawing_line_functions.o: drawing_line_functions.c drawing_line_functions.h
	gcc -c drawing_line_functions.c
input_functions.o: input_functions.c input_functions.h
	gcc -c  input_functions.c
mirror_function.o: mirror_function.c mirror_function.h
	gcc -c mirror_function.c
parse_functions.o: parse_functions.c parse_functions.h
	gcc -c parse_functions.c
outside_ornament.o: outside_ornament.c outside_ornament.h
	gcc -c outside_ornament.c
png_functions.o: png_functions.c png_functions.h
	gcc -c png_functions.c
clean:
	rm *.o
