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
    //carrega imagem
    Img pic;
    if(argc < 1)
    {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);


    //input
    char message[20];
    printf("Digite a mensagem: ");
    scanf("%s", message);

    char password[20];
    printf("\nDigite a senha: ");
    scanf("%s", password);


    //funcoes sobre a senha
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
    printf("mensagem cript %s\n",message);
    printf("inicio: %d, desloc: %d, cifra: %d\n",soma,media,desl);


    //esteganografia
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


    //load para descriptografar
    load("saida.bmp", &pic);



    char password1[20];
    printf("\nDigite a senha: ");
    scanf("%s", password1);


    //funcoes sobre a senha
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


    //decodificacao da esteganografia
    char message1[20];
    int k = 0;
    unsigned int pixel1, test1, cod1;
    printf("\nMensagem final: ");
    for(int i=soma1; i<pic.height*pic.width; i = i + media1)
    {
        if(k >= 20)
        {
            break;
        }
        pixel1 = pic.img[i].r;
        //printf("\n\naa final: %d",pixel1);
        test1 = pixel1 & 0X07;
        //printf(" bb final: %d",test1);
        cod1 = test1 << 5;
        //printf(" cc final: %d",cod1);

        pixel1 = pic.img[i].g;
        //printf("\n\naa final: %d",pixel1);
        test1 = pixel1 & 0X07;
        //printf(" bb final: %d",test1);
        cod1 = cod1 | (test1 << 2);
        //printf(" cc final: %d",cod1);

        pixel1 = pic.img[i].b;
        //printf("\n\naa final: %d",pixel1);
        test1 = pixel1 & 0X03;
        //printf(" bb final: %d",test1);
        cod1 = test1 | cod1;
        //printf(" cc final: %d",cod1);

        if(cod1 > 0)
        {
            message1[k] = cod1;
            printf("%c",cod1 - desl1);
        }
        else
        {
            break;
        }

        k++;
    }

    printf("\n");
    free(pic.img);
}
