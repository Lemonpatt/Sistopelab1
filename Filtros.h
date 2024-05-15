#include"Bmp.h"

BMPImage* saturate_bmp(BMPImage* image, float factor);

BMPImage* greyScale_bmp(BMPImage* image);

BMPImage* Binarizar_bmp(BMPImage* image, float umbral);

int nearly_black(BMPImage * image, float umbral);