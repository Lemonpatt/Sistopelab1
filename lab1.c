#include "Filtros.h"
#include "dirent.h"

int main(int argc, char *argv[]) {

    char *nombre_prefijo = NULL;
    char *nombre_imagenes[100] = {NULL};
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
                //Nos aseguramos que reciba solamente uno de los 3 prefijos permitidos
                nombre_prefijo = optarg;
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
                if (factor_saturacion < 0.0) {
                    fprintf(stderr, "El factor de saturación debe ser positivo.\n");
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

    //Nos aseguramos que reciba solamente uno de los 3 prefijos permitidos o que haya uno al menos
    if (nombre_prefijo == NULL){
        fprintf(stderr, "Debe incluir el prefijo de las imagenes a procesar (-N).\n");
        exit(EXIT_FAILURE);   
    
    }
    if (strcmp(nombre_prefijo, "imagen") && strcmp(nombre_prefijo, "img") && strcmp(nombre_prefijo, "photo")){
        fprintf(stderr, "El parametro para el prefijo debe ser imagen, img o photo.\n");
        exit(EXIT_FAILURE);   
    
    }

     // Abrir el directorio actual
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Error al abrir el directorio actual");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int sufijo = 1;
    while ((entry = readdir(dir)) != NULL) {
        // Verificar si es un directorio y si comienza con el prefijo deseado
        if (strncmp(entry->d_name, nombre_prefijo, strlen(nombre_prefijo)) == 0) {
            // Verificar si el nombre de la carpeta tiene el formato adecuado (prefijo_sufijo)
            const char *num_str = entry->d_name + strlen(nombre_prefijo);  // Apuntar al número después del prefijo
            if (sscanf(num_str, "_%d", &sufijo) == 1){
                // Es una imagen con el formato adecuado, guardar el nombre
                 // Copiar el nombre de la imagen sin la extensión (.bmp)
                char *nombre_sin_extension = (char *)malloc(strlen(entry->d_name) - 4);  // Reservar memoria para el nuevo nombre
                strncpy(nombre_sin_extension, entry->d_name, strlen(entry->d_name) - 4);
                printf("nombre guardado: %s\n ", nombre_sin_extension);
                nombre_imagenes[cantidad_imagenes] = nombre_sin_extension;
                cantidad_imagenes++;

            }
            // Verificar si se alcanzó el máximo de nombres a guardar
            if (cantidad_imagenes == 100) {
                fprintf(stderr, "Advertencia: se ha alcanzado el límite máximo de nombres de imagenes a procesar.\n");
                    break;
                }
            sufijo++;
        }
        
    }


    // Verificar si se proporcionaron todos los parámetros obligatorios
    if (nombre_imagenes[0] == NULL) {
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
    while(cantidad_imagenes > i){

        char nombre_imagen[400];
        sprintf(nombre_imagen, "%s.bmp", nombre_imagenes[i]);
        
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
        sprintf(nombre_imagen, "%s/%s_Saturated.bmp", nombre_carpeta, nombre_imagenes[i]);

        BMPImage* new_image = saturate_bmp(image, factor_saturacion);
        write_bmp(nombre_imagen, new_image);


        csv = fopen(nombre_archivo_csv, "a");

        //Revisamos si se hacen los siguientes filtros
        if (cantidad_filtros > 1){
            sprintf(nombre_imagen, "%s/%s_Gris.bmp", nombre_carpeta, nombre_imagenes[i]);
            BMPImage* new_imageG = greyScale_bmp(image);
            write_bmp(nombre_imagen, new_imageG);

            if (cantidad_filtros == 3){
                
                sprintf(nombre_imagen, "%s/%s_Binario.bmp", nombre_carpeta, nombre_imagenes[i]);
                BMPImage* new_imageB = Binarizar_bmp(new_imageG, umbral_binarizacion);
                write_bmp(nombre_imagen, new_imageB);

                resultadoNB = nearly_black(new_imageB, umbral_clasificacion);
                fprintf(csv, "%s-Binarizada; %d\n", nombre_imagenes[i], resultadoNB);
                csv = NULL;
                free_bmp(new_imageB);
            }
            //Se ve nearly black solo si no se ha hecho todavia
            if (csv != NULL){
                resultadoNB = nearly_black(new_imageG, umbral_clasificacion);
                fprintf(csv, "%s-Gris; %d\n", nombre_imagenes[i], resultadoNB);
                csv = NULL;
            }
            free_bmp(new_imageG);

        }
        if (csv != NULL){
            resultadoNB = nearly_black(new_image, umbral_clasificacion);
            fprintf(csv, "%s-Saturada; %d\n", nombre_imagenes[i], resultadoNB);
            csv = NULL;
        }

        free_bmp(image);
        free_bmp(new_image);

        i++;

    }
    return 0;
}