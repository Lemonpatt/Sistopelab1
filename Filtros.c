#include"Filtros.h"



//funcion para saturar los colores de la imagen, solo se multiplica el rgb por un factor
BMPImage* saturate_bmp(BMPImage* image, float factor) {
    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];
            pixel.r = (unsigned char)(pixel.r * factor);
            pixel.g = (unsigned char)(pixel.g * factor);
            pixel.b = (unsigned char)(pixel.b * factor);
            new_image->data[y * image->width + x] = pixel;
        }
    }

    return new_image;
}

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

BMPImage* Binarizar_bmp(BMPImage* image, float umbral) {
    BMPImage* new_image = (BMPImage*)malloc(sizeof(BMPImage));
    new_image->width = image->width;
    new_image->height = image->height;
    new_image->data = (RGBPixel*)malloc(sizeof(RGBPixel) * image->width * image->height);
    unsigned char umbralAdaptado = (unsigned char)(umbral * 255);

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBPixel pixel = image->data[y * image->width + x];

            if (pixel.r <= umbralAdaptado) {
                pixel.r = 0;
            } else {
                pixel.r = 255;
            }

            if (pixel.g <= umbralAdaptado) {
                pixel.g = 0; 
            } else {
                pixel.g = 255; 
            }

            if (pixel.b <= umbralAdaptado) {
                pixel.b = 0; 
            } else {
                pixel.b = 255;
            }

            new_image->data[y * image->width + x] = pixel;
        }
    }
    return new_image;
}

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