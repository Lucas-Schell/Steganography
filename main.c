#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct
{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct
{
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc < 1)
    {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    char message[20];
    printf("input message: ");
    scanf("%s", message);

    char password[20];
    printf("\ninput password: ");
    scanf("%s", password);


    int soma = 0;
    for (int i = 0; i < strlen(password); i++)
    {
        soma=soma + password[i];
    }
    int media = soma/strlen(password);

    int desl = 'a' - password[0];
    if(strlen(password) > 1)
    {
        desl = desl - ('z' - password[1]);
    }

    for (int i =0; i< strlen(message); i++)
    {
        message[i] = message[i] + desl;
    }

    printf("inicio: %d, desloc: %d, cifra: %d\n",soma,media,desl);
    int j = 0;
    unsigned int pixel, test, cod;
    for(int i=soma; i<pic.height*pic.width; i = i + media)
    {
        if(j > strlen(message))
        {
            break;
        }
        pixel = pic.img[i].r;
        test = pixel & 0XF8;
        cod = 0XE0 & message[j];
        cod = test | (cod >> 5);
        pic.img[i].r = cod;

        pixel = pic.img[i].g;
        test = pixel & 0XF8;
        cod = 0X1C & message[j];
        cod = test | (cod >> 2);
        pic.img[i].g = cod;

        pixel = pic.img[i].b;
        test = pixel & 0XFC;
        cod = 0X03 & message[j];
        cod = test | cod;
        pic.img[i].b = cod;
        j++;
    }
    SOIL_save_image("saida.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, pic.img);

    printf("\n");

    free(pic.img);

    load("saida.bmp", &pic);


    char password1[20];
    printf("\ninput password: ");
    scanf("%s", password1);

    int soma1 = 0;
    for (int i = 0; i < strlen(password1); i++)
    {
        soma1=soma1 + password1[i];
    }
    int media1 = soma1/strlen(password1);

    int desl1 = 'a' - password1[0];
    if(strlen(password1) > 1)
    {
        desl1 = desl1 - ('z' - password1[1]);
    }

    printf("inicio: %d, desloc: %d, cifra: %d\n",soma1,media1,desl1);


}
