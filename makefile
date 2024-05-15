all: Crear_archivos.o Bmp.o Filtros.o main

Crear_archivos.o: Crear_archivos.c Crear_archivos.h
			g++ -g -c Crear_archivos.c

Bmp.o: Bmp.c Bmp.h
			g++ -g -c Bmp.c

Filtros.o: Filtros.c Filtros.h
			g++ -g -c Filtros.c

main: Crear_archivos.o Bmp.o Filtros.o main.c
	g++ -g Crear_archivos.o Bmp.o Filtros.o main.c -o main

clean:
	rm -f *.o Crear_archivos Bmp Filtros main
