#include <stdio.h>
#include <stdlib.h>

unsigned char histograma[256];

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pasaAgris()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("j.bmp", "rb");        // Imagen original a transformar
    outputImage = fopen("p2.bmp", "wb"); // Imagen transformada
    unsigned char r, g, b;               // Pixel
    printf("Hola\n");
    for (int i = 0; i < 54; i++)
        fputc(fgetc(image), outputImage); // Copia cabecera a nueva imagen
    while (!feof(image))
    { // Grises
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);

        unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
    }
    printf("Adios\n");
    fclose(image);
    fclose(outputImage);
}
/*
void guardaHistograma()
{
    FILE *f1;
    f1 = fopen("histo.csv", "w");

    printf("guardando\n");
    for (int i = 0; i < 256; ++i)
    {
        fprintf(f1, "%i,%i\n", i, histograma[i]);
    }
    fclose(f1);
}
*/

int main()
{
    pasaAgris();
}
