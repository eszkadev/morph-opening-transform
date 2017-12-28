/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <qdbmp.h>
#include "image_model.h"

#define EXIT_ERROR -1

void parse_input( int argc, char** argv, unsigned int* width, unsigned int* height, char** output_file );
void generate( IMAGE_MODEL* model );

int main( int argc, char** argv )
{
    srand( time( NULL ) );

    char* output_file = NULL;
    unsigned int width, height;

    parse_input( argc, argv, &width, &height, &output_file );

    printf( "Width: %d\nHeight: %d\nOut: %s\n", width, height, output_file );

    BMP* output_bmp;

    IMAGE_MODEL* output = create_image_model( width, height );

    generate( output );

    output_bmp = image_model_to_bmp( output );
    BMP_WriteFile( output_bmp, output_file );
    BMP_CHECK_ERROR( stderr, EXIT_ERROR );

    free_image_model( output );
    BMP_Free( output_bmp );

    return 0;
}

void parse_input( int argc, char** argv, unsigned int* width, unsigned int* height, char** output_file )
{
    if( argc != 4 )
    {
        fprintf( stderr, "Usage: ./generate_testcase <width> <height> <output file>\n" );
        exit( EXIT_ERROR );
    }
    else
    {
        sscanf( argv[ 1 ], "%d", width );
        sscanf( argv[ 2 ], "%d", height );

        *output_file = (char*)malloc( strlen( argv[ 3 ] ) * sizeof( char ) );
        strcpy( *output_file, argv[ 3 ] );

        char* dot_char = strchr( *output_file, '.' );
        if( dot_char == NULL )
        {
            fprintf( stderr, "Input file extension not provided\n" );
            exit( EXIT_ERROR );
        }
    }
}

void generate( IMAGE_MODEL* model )
{
    unsigned int SECTION_SIZE = 4;

    unsigned int width = model->width;
    unsigned int height = model->height;

    unsigned int threshold = 50;

    unsigned int x, y, x_start, y_start;
    for( y_start = 0; y_start < height; y_start += SECTION_SIZE * 3 / 4 )
    {
        for( x_start = 0; x_start < width; x_start += SECTION_SIZE * 3 / 4 )
        {
            threshold = rand() % 100;

            for( y = y_start; y < y_start + SECTION_SIZE && y < height; ++y )
            {
                for( x = x_start; x < x_start + SECTION_SIZE && x < width; ++x )
                {
                    int random = rand() % 100;
                    if( random >= threshold )
                        model->data[ x ][ y ] = 0;
                    else
                        model->data[ x ][ y ] = 255;
                }
            }
        }
        SECTION_SIZE = rand() % 7 + 3;
    }
}