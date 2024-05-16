#include"Filtros.h"


//Entradas: Recibe un puntero a una estructura BMPImage y un numero float( >= 0)
//Salidas: Retorna un puntero a una estructura BMPImage con sus datos de imagen saturados
//Descripcion: Funcion que satura los colores de la imagen multiplicando los colores por el float
BMPImage* saturate_bmp(BMPImage* image, float factor) {
    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            if (pixel.r * factor > 255){
                pixel.r = (unsigned char)255;
            }
            else {
                pixel.r = (unsigned char)(pixel.r * factor);
            }
            if (pixel.g * factor > 255){
                pixel.g = (unsigned char)255;
            }
            else {
                pixel.g = (unsigned char)(pixel.g * factor);
            }
            if (pixel.b * factor > 255){
                pixel.b = (unsigned char)255;
            }
            else {
                pixel.b = (unsigned char)(pixel.b * factor);
            }
            new_image->data[y * image->width + x] = pixel;
        }
    }

    return new_image;
}

//Entradas: Recibe un puntero a una estructura BMPImage
//Salidas: Retorna un puntero a una estructura BMPImage con sus datos de imagen ahora en tonos de grises
//Descripcion: Funcion que convierte una imagen a su version en escala de grises usando el espacio de color con valor luma ponderado de Rec.601
BMPImage* greyScale_bmp(BMPImage* image){
    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            unsigned char pixelGrey = pixel.r * 0.3 + pixel.g * 0.59 + pixel.b* 0.11;
            pixel.r = (unsigned char)(pixelGrey);
            pixel.g = (unsigned char)(pixelGrey);
            pixel.b = (unsigned char)(pixelGrey);
            new_image->data[y * image->width + x] = pixel;
        }
    }
    return new_image;
}

//Entradas: Recibe un puntero a una estructura BMPImage y un numero flotante (entre 0 y 1)
//Salidas: Retorna un puntero a una estructura BMPImage con su información de imagen binarizada
//Descripcion: Funcion que Binariza una imagen(la convierte en una imagen blanco y negro)
BMPImage* Binarizar_bmp(BMPImage* image, float umbral) {
    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);
    unsigned char umbralAdaptado = (unsigned char)(umbral * 255);

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            //Aqui se revisa para cada canal del pixel si logra pasar el umbral adaptado o no
            if (pixel.r < umbralAdaptado) {
                pixel.r = 0;
            } else {
                pixel.r = 255;
            }

            if (pixel.g < umbralAdaptado) {
                pixel.g = 0; 
            } else {
                pixel.g = 255; 
            }

            if (pixel.b < umbralAdaptado) {
                pixel.b = 0; 
            } else {
                pixel.b = 255;
            }

            new_image->data[y * image->width + x] = pixel;
        }
    }
    return new_image;
}

//Entradas:Recibe un puntero a una estructura BMPImage y un numero flotante (entre 0 y 1)
//Salidas: Retorna un numero entero (0 para false y 1 para true)
//Descripcion: Funcion que revisa si una imagen es "casi negra" basandose en el umbral (numero flotante) recibido, si es más alto que el umbral entonces retorna 1
int nearly_black(BMPImage * image, float umbral){
    float n_pixels = image->height * image->width;
    float contador = 0;
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            if (pixel.r == 0 && pixel.b == 0 && pixel.g == 0){
                contador++;
            }
        }
    }
    if (contador/n_pixels >= umbral){
        return 1;
    }
    else{
        return 0;
    }
}