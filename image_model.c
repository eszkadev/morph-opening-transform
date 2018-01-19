/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include "image_model.h"
#include <stdlib.h>
#include <stdio.h>

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
            model->data[ x + y * width ] = _normalizePixelValue( r );
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
            pixel = model->data[ x + y * width ];
            BMP_SetPixelRGB( bmp, x, y, pixel, pixel, pixel );
        }
    }

    return bmp;
}

IMAGE_MODEL* create_image_model( unsigned int width, unsigned int height )
{
    unsigned long long int size = width * height;
    IMAGE_MODEL* model = (IMAGE_MODEL*)_mm_malloc( sizeof( IMAGE_MODEL ), 64 );
    unsigned char* data = (unsigned char*)_mm_malloc( sizeof( unsigned char ) * size, 64 );

    unsigned int x;

    model->width = width;
    model->height = height;
    model->data = data;

    return model;
}

void free_image_model( IMAGE_MODEL* model )
{
    _mm_free( model->data );
    _mm_free( model );
}

void print_model( IMAGE_MODEL* model )
{
    printf( "\n" );
    unsigned int x, y;
    for( x = 0; x < model->width; ++x )
    {
        for( y = 0; y < model->height; ++y )
        {
            printf( model->data[ x + y * model->width ] ? "." : "X" );
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
            pixel1 = model1->data[ x + y * width1 ];
            pixel2 = model2->data[ x + y * width2 ];

            if( pixel1 != pixel2 )
            {
                printf( "[%d, %d] %x expected: %x\n", x, y, pixel1, pixel2 );
                return CONTENT_IS_DIFFERENT;
            }
        }
    }

    return EQUALS;
}

void save_image_model( IMAGE_MODEL* model, char* path )
{
    FILE* file = fopen( path, "w" );

    if( file )
    {
        unsigned int x, y;
        unsigned int width = model->width;
        unsigned int height = model->height;

        fprintf( file, "%d %d\n", width, height );

        for( y = 0; y < height; ++y )
        {
            for( x = 0; x < width; ++x )
            {
                if( model->data[ x + y * width ] )
                    putc( '.', file );
                else
                    putc( 'X', file );
            }
            fprintf( file, "\n" );
        }
    }
}

IMAGE_MODEL* load_image_model( char* path )
{
    IMAGE_MODEL* model = NULL;

    FILE* file = fopen( path, "r" );

    if( file )
    {
        unsigned int width, height;
        fscanf( file, "%u %u\n", &width, &height );

        model = create_image_model( width, height );
        unsigned char* line = (unsigned char*)_mm_malloc( sizeof( unsigned char ) * ( width + 1 ), 64 );
        unsigned int x, y;

        for( y = 0; y < height; ++y )
        {
            if( fgets( line, width + 1, file ) != (char*)line )
            {
                _mm_free( line );
                return NULL;
            }

            for( x = 0; x < width; ++x )
            {
                if( line[ x ] == '.' )
                    model->data[ x + y * width ] = 255;
                else if( line[ x ] == 'X' )
                    model->data[ x + y * width ] = 0;
                else
                {
                    fprintf( stderr, "Invalid file! contains: '%c' (%d) character at position [%d, %d].\n",
                        line[ x ], (int)line[ x ], x, y );
                    _mm_free( line );
                    return NULL;
                }
            }

            // new line
            fgetc( file );
        }

        _mm_free( line );
    }

    return model;
}
