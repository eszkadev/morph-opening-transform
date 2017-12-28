/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include "sequential_operations.h"

typedef struct
{
    char x;
    char y;
    char tab[3][3];
} MORPHOLOGICAL_OPERATOR;

static const MORPHOLOGICAL_OPERATOR MORPHOLOGICAL_OPERATORS[] =
{
    { 1, 1, { 0, 1, 0, 1, 1, 1, 0, 1, 0 } },
};

IMAGE_MODEL* erosion( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    int width = input->width;
    int height = input->height;
    int x;
    int y;

    IMAGE_MODEL* output = create_image_model( width, height );

    int y_offset = -1 * current_operator.y;
    int x_offset = -1 * current_operator.x;
    for( y = y_offset; y < height + y_offset; ++y )
    {
        for( x = x_offset; x < width + x_offset; ++x )
        {
            unsigned char r, g, b;

            char condition = 0;
            int i;
            int j;
            for( i = 0; i < 3; ++i )
            {
                for( j = 0; j < 3; ++j )
                {
                    if( x + j >= 0 && x + j < width &&
                        y + i >= 0 && y + i < height &&
                        current_operator.tab[ i ][ j ] )
                    {
                        if( input->data[ x + j ][ y + i ] )
                        {
                            condition = 1;
                            break;
                        }
                    }
                    else if( current_operator.tab[ i ][ j ] )
                    {
                        // pixels out of an image are empty
                        condition = 1;
                        break;
                    }
                }
                if( condition )
                    break;
            }

            if( x + current_operator.x >= 0 && x + current_operator.x < width &&
                y + current_operator.y >= 0 && y + current_operator.y < height )
            {
                if( condition )
                    output->data[ x + current_operator.x ][ y + current_operator.y ] = 255;
                else
                    output->data[ x + current_operator.x ][ y + current_operator.y ] = 0;
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
    int x;
    int y;

    IMAGE_MODEL* output = create_image_model( width, height );

    for( y = 0; y < height - 1; ++y )
    {
        for( x = 0; x < width - 1; ++x )
        {
            unsigned char r, g, b;

            char condition = 0;
            int i;
            int j;
            for( i = 0; i < 3; ++i )
            {
                for ( j = 0; j < 3; ++j )
                {
                    if( current_operator.tab[ i ][ j ] == 1 )
                    {
                        if( !input->data[ x + j ][ y + i ] )
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
                output->data[ x + current_operator.x ][ y + current_operator.y ] = 0;
            else
                output->data[ x + current_operator.x ][ y + current_operator.y ] = 255;
        }
    }

    return output;
}

IMAGE_MODEL* opening( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator )
{
    return dilatation( erosion( input, operator ), operator );
}
