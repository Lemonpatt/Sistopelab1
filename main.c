#include "Filtros.h"


int main(int argc, char *argv[]) {
    char *nombre_prefijo[3] = {NULL};
    int cantidad_filtros = 0;
    double factor_saturacion = 1.0;
    double umbral_binarizacion = -1.0;
    double umbral_clasificacion = -1.0;
    char *nombre_carpeta = NULL;
    char *nombre_archivo_csv = NULL;
    int opt;
    int cantidad_imagenes = 0;

    // Procesar las opciones de línea de comandos
    while ((opt = getopt(argc, argv, "N:f:p:u:v:C:R:")) != -1) {
        switch (opt) {

            case 'N':
                //Nos aseguramos que reciba un máximo de 3 imágenes
                // Procesar múltiples nombres de archivo después de '-N'
                while (cantidad_imagenes < 4) {
                    if (argv[optind-1][0] == '-'){
                        optind--;
                        break;
                    }
                    nombre_prefijo[cantidad_imagenes] = strdup(argv[optind-1]);
                    cantidad_imagenes++;
                    if (cantidad_imagenes == 4){
                        fprintf(stderr, "La cantidad maxima de imagenes es 3.\n");
                        exit(EXIT_FAILURE);
                    }
                    optind++;

                }
                break;
            case 'f':
                cantidad_filtros = atoi(optarg);
                if (cantidad_filtros < 1 || cantidad_filtros > 3) {
                    fprintf(stderr, "La cantidad de filtros debe ser de 1 a 3.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                factor_saturacion = atof(optarg);

                //Limite del factor de saturación?
                if (factor_saturacion < -2.0 || factor_saturacion > 2.0) {
                    fprintf(stderr, "El factor de saturación debe estar entre -2 y 2??.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'u':
                umbral_binarizacion = atof(optarg);
                //Umbral de binarización debe estar entre 0 y 1
                if (umbral_binarizacion < 0.0 || umbral_binarizacion > 1.0) {
                    fprintf(stderr, "El umbral de binarización debe estar entre 0 y 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                umbral_clasificacion = atof(optarg);
                if (umbral_clasificacion < 0.0 || umbral_clasificacion > 1.0) {
                    fprintf(stderr, "El umbral de clasificación debe estar entre 0 y 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'C':
                nombre_carpeta = optarg;
                break;

            case 'R':
                nombre_archivo_csv = optarg;
                break;

            default:
                fprintf(stderr, "Uso: %s -N <nombre_prefijo> -f <cantidad_filtros> -p <factor_saturacion> -u <umbral_binarizacion> -v <umbral_clasificacion> -C <nombre_carpeta> -R <nombre_archivo_csv>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificar si se proporcionaron todos los parámetros obligatorios
    if (nombre_prefijo[0] == NULL) {
        fprintf(stderr, "No se puede realizar la ejecución sin una imagen.\n");
        exit(EXIT_FAILURE);
    }

    if (nombre_carpeta == NULL || nombre_archivo_csv == NULL) {
        fprintf(stderr, "Es obligatorio especificar el nombre de la carpeta y csv con resultados.\n");
        exit(EXIT_FAILURE);
    }

    if (cantidad_filtros == 0) {
        cantidad_filtros = 3;
    }
    if (factor_saturacion == 0.0) {
        factor_saturacion = 1.0;
    }

    if (umbral_binarizacion == -1.0) {
        umbral_binarizacion = 0.5;
    }
    if (umbral_clasificacion == -1.0) {
        umbral_clasificacion = 0.5;
    }
    // Mostrar los parámetros obtenidos
    //printf("Nombre del prefijo de las imágenes: %s\n", nombre_prefijo);
    printf("Cantidad de filtros a aplicar: %d\n", cantidad_filtros);
    printf("Factor de saturación del filtro: %f\n", factor_saturacion);
    printf("Umbral para binarizar la imagen: %f\n", umbral_binarizacion);
    printf("Umbral para clasificación: %f\n", umbral_clasificacion);
    printf("Nombre de la carpeta resultante con las imágenes: %s\n", nombre_carpeta);
    printf("Nombre del archivo CSV con las clasificaciones resultantes: %s\n", nombre_archivo_csv);


    make_folder(nombre_carpeta);
    make_csv(nombre_archivo_csv, umbral_clasificacion);



    int i = 0;
    FILE *csv = NULL;
    int resultadoNB;
    while(nombre_prefijo[i] != NULL){

        char nombre_imagen[400];
        sprintf(nombre_imagen, "%s.bmp", nombre_prefijo[i]);
        
        printf("Imagen siendo procesada: %s\n", nombre_imagen);

        //Leemos la imagen
        BMPImage* image = read_bmp(nombre_imagen);
        if (!image) {
            return 1; //Ya se ve este error dentro de read_bmp porque se repite afuera?
        }

        //Segumente no será necesario dsps
        printf("Ancho de la imagen: %d\n", image->width);
        printf("Alto de la imagen: %d\n", image->height);

        //Otorgamos el filepath al nombre para escribirlo alli
        sprintf(nombre_imagen, "%s/%s_Saturated.bmp", nombre_carpeta, nombre_prefijo[i]);

        BMPImage* new_image = saturate_bmp(image, factor_saturacion);
        write_bmp(nombre_imagen, new_image);

        //Revisamos si se hacen los siguientes filtros
        if (cantidad_filtros > 1){
            sprintf(nombre_imagen, "%s/%s_Gris.bmp", nombre_carpeta, nombre_prefijo[i]);
            BMPImage* new_imageG = greyScale_bmp(image);
            write_bmp(nombre_imagen, new_imageG);

            if (cantidad_filtros == 3){
                
                sprintf(nombre_imagen, "%s/%s_Binario.bmp", nombre_carpeta, nombre_prefijo[i]);
                BMPImage* new_imageB = Binarizar_bmp(new_imageG, umbral_binarizacion);
                write_bmp(nombre_imagen, new_imageB);


                csv = fopen(nombre_archivo_csv, "a");
                resultadoNB = nearly_black(new_imageB, umbral_clasificacion);
                fprintf(csv, "%s-Binarizada; %d\n", nombre_prefijo[i], resultadoNB);
                if (i == cantidad_imagenes-1){
                    free_bmp(new_imageB);
                }
            }
            //Se ve nearly black solo si no se ha hecho todavia
            if (csv == NULL){
                csv = fopen(nombre_archivo_csv, "a");
                resultadoNB = nearly_black(new_imageG, umbral_clasificacion);
                fprintf(csv, "%s-Gris; %d\n", nombre_prefijo[i], resultadoNB);
            }

            if (i == cantidad_imagenes-1){
                free_bmp(new_imageG);
            }
        }

        if (csv == NULL){
            csv = fopen(nombre_archivo_csv, "a");
            resultadoNB = nearly_black(new_image, umbral_clasificacion);
            fprintf(csv, "%s-Saturada; %d\n", nombre_prefijo[i], resultadoNB);
        }
        fclose(csv);

        if (i == cantidad_imagenes-1)
        {
            free_bmp(image);
            free_bmp(new_image);
        }
        i++;
    }
    return 0;
}
    

    /*
    const char* filename = "rb.bmp";
    BMPImage* image = read_bmp(filename);
    if (!image) {
        return 1;
    }

    printf("Ancho de la imagen: %d\n", image->width);
    printf("Alto de la imagen: %d\n", image->height);

    // Acceder a los píxeles de la imagen
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            printf("Pixel (%d, %d): R=%d, G=%d, B=%d\n", x, y, pixel.r, pixel.g, pixel.b);
        }
    }

    BMPImage* new_image = saturate_bmp(image, 1.1f);
    write_bmp("saturated.bmp", new_image);

    BMPImage* new_imageG = greyScale_bmp(image);
    write_bmp("Gris.bmp", new_imageG);

    BMPImage* new_imageB = Binarizar_bmp(new_imageG, 0.3);
    write_bmp("Binario.bmp", new_imageB);

    free_bmp(image);
    free_bmp(new_image);
    free_bmp(new_imageG);
    free_bmp(new_imageB);
    return 0;
}



/*



/*



*/


