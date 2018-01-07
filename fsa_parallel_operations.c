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

IMAGE_MODEL* erosion( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    int width = input->width;
    int height = input->height;
    int x;
    int y;

    IMAGE_MODEL* output = create_image_model( width, height );

    int cache_lines_per_row = width / CACHE_LINE_SIZE + 1;
    int cache_lines = cache_lines_per_row * height;
    int cache_line;

    int x_offset = current_operator.x;
    int y_offset = current_operator.y;

    unsigned char* input_data = input->data;
    unsigned char* output_data = output->data;

    #pragma omp parallel for shared(y_offset, x_offset, input_data, output_data, current_operator)
    for( cache_line = 0; cache_line < cache_lines; ++cache_line )
    {
        int x_begin = - x_offset + ( cache_line % cache_lines_per_row ) * CACHE_LINE_SIZE;
        int y = - y_offset + cache_line / cache_lines_per_row;
        int x;

        for( x = x_begin; x < x_begin + CACHE_LINE_SIZE && x < width && y < height; ++x )
        {
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

            if( x + current_operator.x >= 0 && x + current_operator.x < width &&
                y + current_operator.y >= 0 && y + current_operator.y < height )
            {
                if( condition )
                    output_data[ x + current_operator.x + ( y + current_operator.y ) * width ] = 255;
                else
                    output_data[ x + current_operator.x + ( y + current_operator.y ) * width ] = 0;
            }
        }
    }

    return output;
}

IMAGE_MODEL* dilatation( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    int width = input->width;
    int height = input->height;

    IMAGE_MODEL* output = create_image_model( width, height );

    int cache_lines_per_row = width / CACHE_LINE_SIZE + 1;
    int cache_lines = cache_lines_per_row * height;
    int cache_line;

    int x_offset = current_operator.x;
    int y_offset = current_operator.y;

    unsigned char* input_data = input->data;
    unsigned char* output_data = output->data;

    #pragma omp parallel for shared(y_offset, x_offset, input_data, output_data, current_operator)
    for( cache_line = 0; cache_line < cache_lines; ++cache_line )
    {
        int x_begin = -x_offset + ( cache_line % cache_lines_per_row ) * CACHE_LINE_SIZE;
        int y = -y_offset + cache_line / cache_lines_per_row;
        int x;

        for( x = x_begin; x < x_begin + CACHE_LINE_SIZE && x < width && y < height; ++x )
        {
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

            if( x + x_offset >= 0 && x + x_offset < width &&
                y + y_offset >= 0 && y + y_offset < height )
            {
                if( condition )
                    output_data[ x + x_offset + ( y + y_offset ) * width ] = 0;
                else
                    output_data[ x + x_offset + ( y + y_offset ) * width ] = 255;
            }
        }
    }

    return output;
}

IMAGE_MODEL* opening( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator )
{
    return dilatation( erosion( input, operator ), operator );
}
