/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <qdbmp.h>

typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned char** data;
} IMAGE_MODEL;

#define EQUALS 0
#define SIZE_DOESNT_MATCH 1
#define CONTENT_IS_DIFFERENT 2

IMAGE_MODEL* bmp_to_image_model( BMP* bmp );
BMP* image_model_to_bmp( IMAGE_MODEL* model );

int compare_models( IMAGE_MODEL* model1, IMAGE_MODEL* model2 );
void print_model( IMAGE_MODEL* model );

IMAGE_MODEL * create_image_model( unsigned int width, unsigned int height );
void free_image_model( IMAGE_MODEL* model );