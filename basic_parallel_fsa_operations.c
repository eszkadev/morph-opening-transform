/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include "operations.h"

#define CACHE_LINE_SIZE 64

typedef struct
{
    char x;
    char y;
    char tab[3][3];
} MORPHOLOGICAL_OPERATOR;

static const MORPHOLOGICAL_OPERATOR MORPHOLOGICAL_OPERATORS[] =
{
    { 1, 1, { 0, 1, 0, 1, 1, 1, 0, 1, 0 } },
    { 1, 1, { 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
};

void erosion( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    unsigned int width = input->width;
    unsigned int height = input->height;
    unsigned long long int size = width * height;

    int x;
    int y;

    int x_offset = current_operator.x;
    int y_offset = current_operator.y;

    unsigned char* input_data = input->data;
    unsigned char* output_data = output->data;

    unsigned long long int it;

    #pragma omp parallel for schedule(static, CACHE_LINE_SIZE)
    for( it = 0; it < size; ++it )
    {
        int x = it % width - x_offset;
        int y = it / width - y_offset;

        unsigned char r, g, b;

        char condition = 0;
        int i;
        int j;
        for( i = 0; i < 3; ++i )
        {
            for( j = 0; j < 3; ++j )
            {
                if( current_operator.tab[ i ][ j ] )
                {
                    int correct_position = ( x + j >= 0 && x + j < width
                        && y + i >= 0 && y + i < height ) ? 1 : 0;

                    if( correct_position && input_data[ x + j + ( y + i ) * width ] )
                    {
                        condition = 1;
                        break;
                    }
                    else if( !correct_position )
                    {
                        // pixels out of an image are empty
                        condition = 1;
                        break;
                    }
                }
            }
            if( condition )
                break;
        }

        if( condition )
            output_data[ it ] = 255;
        else
            output_data[ it ] = 0;
    }
}

void dilatation( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    unsigned int width = input->width;
    unsigned int height = input->height;
    unsigned long long int size = width * height;

    int x_offset = current_operator.x;
    int y_offset = current_operator.y;

    unsigned char* input_data = input->data;
    unsigned char* output_data = output->data;

    unsigned long long int it;

    #pragma omp parallel for schedule(static, CACHE_LINE_SIZE)
    for( it = 0; it < size; ++it )
    {
        int x = it % width - x_offset;
        int y = it / width - y_offset;

        unsigned char r, g, b;

        char condition = 0;
        int i;
        int j;
        for( i = 0; i < 3; ++i )
        {
            for( j = 0; j < 3; ++j )
            {
                if( current_operator.tab[ i ][ j ] )
                {
                    if( x + j >= 0 && x + j < width &&
                        y + i >= 0 && y + i < height &&
                        !input_data[ x + j + ( y + i ) * width ] )
                    {
                        condition = 1;
                        break;
                    }
                }
            }
            if( condition )
                break;
        }

        if( condition )
            output_data[ it ] = 0;
        else
            output_data[ it ] = 255;
    }
}

void opening( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator )
{
    IMAGE_MODEL* temp = create_image_model( input->width, input->height );

    erosion( input, temp, operator );
    dilatation( temp, output, operator );
}
