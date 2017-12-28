/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include "image_model.h"
#include <stdlib.h>


/* Normalize values */
inline unsigned char _normalizePixelValue( unsigned char value )
{
    if ( value > 50 )
        return 255;
    else
        return 0;
}

IMAGE_MODEL* bmp_to_image_model( BMP* bmp )
{
    if( !bmp )
        return NULL;

    unsigned int width = BMP_GetWidth( bmp );
    unsigned int height = BMP_GetHeight( bmp );

    IMAGE_MODEL* model = create_image_model( width, height );

    unsigned int x, y;
    unsigned char r, g, b;
    for( y = 0; y < height; ++y )
    {
        for( x = 0; x < width; ++x )
        {
            BMP_GetPixelRGB( bmp, x, y, &r, &g, &b );
            model->data[ x ][ y ] = _normalizePixelValue( r );
        }
    }

    return model;
}

BMP* image_model_to_bmp( IMAGE_MODEL* model )
{
    if( !model )
        return NULL;

    unsigned int width = model->width;
    unsigned int height = model->height;

    BMP* bmp = BMP_Create( width, height, 24 );

    unsigned int x, y;
    unsigned char pixel;
    for( y = 0; y < height; ++y )
    {
        for( x = 0; x < width; ++x )
        {
            pixel = model->data[ x ][ y ];
            BMP_SetPixelRGB( bmp, x, y, pixel, pixel, pixel );
        }
    }

    return bmp;
}

IMAGE_MODEL* create_image_model( unsigned int width, unsigned int height )
{
    IMAGE_MODEL* model = (IMAGE_MODEL*)malloc( sizeof( IMAGE_MODEL ) );
    unsigned char** data = (unsigned char**)malloc( sizeof( unsigned char* ) * width );

    unsigned int x;
    for( x = 0; x < width; ++x )
    {
        data[ x ] = (unsigned char*)malloc( sizeof( unsigned char ) * height );
#ifdef _DEBUG
        unsigned int y;
        for( y = 0; y < height; ++y )
        {
            data[ x ][ y ] = 55;
        }
#endif
    }

    model->width = width;
    model->height = height;
    model->data = data;

    return model;
}

void free_image_model( IMAGE_MODEL* model )
{
    unsigned int width = model->width;
    unsigned int x;

    for( x = 0; x < width; ++x )
    {
        free( model->data[ x ] );
    }

    free( model );
}

void print_model( IMAGE_MODEL* model )
{
    printf( "\n" );
    unsigned int x, y;
    for( x = 0; x < model->width; ++x )
    {
        for( y = 0; y < model->height; ++y )
        {
            printf( model->data[ x ][ y ] ? "." : "X" );
        }
        printf( "\n" );
    }
}

int compare_models( IMAGE_MODEL* model1, IMAGE_MODEL* model2 )
{
    unsigned int width1 = model1->width;
    unsigned int height1 = model1->height;
    unsigned int width2 = model2->width;
    unsigned int height2 = model2->height;

    unsigned int x, y;
    unsigned char pixel1, pixel2;

    if( width1 != width2 || height1 != height2 )
    {
        return SIZE_DOESNT_MATCH;
    }

    for( y = 0; y < height1; ++y )
    {
        for( x = 0; x < width1; ++x )
        {
            pixel1 = model1->data[ x ][ y ];
            pixel2 = model2->data[ x ][ y ];

            if( pixel1 != pixel2 )
            {
                printf( "[%d, %d] %x expected: %x\n", x, y, pixel1, pixel2 );
                return CONTENT_IS_DIFFERENT;
            }
        }
    }

    return EQUALS;
}
