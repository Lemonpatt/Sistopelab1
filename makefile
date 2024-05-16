all: Crear_archivos.o Bmp.o Filtros.o lab1

Crear_archivos.o: Crear_archivos.c Crear_archivos.h
			g++ -g -c Crear_archivos.c

Bmp.o: Bmp.c Bmp.h
			g++ -g -c Bmp.c

Filtros.o: Filtros.c Filtros.h
			g++ -g -c Filtros.c

lab1: Crear_archivos.o Bmp.o Filtros.o lab1.c
	g++ -g Crear_archivos.o Bmp.o Filtros.o lab1.c -o lab1

clean:
	rm -f *.o Crear_archivos Bmp Filtros lab1

