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
    { 1, 1, { 0, 1, 0, 1, 0, 1, 0, 1, 0 } },
};

/* Normalize values */
inline char GetPixelValue( unsigned char* value )
{
    if ( value > 130 )
        return 255;
    else
        return 0;
}

BMP* erosion( BMP* input, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    BMP* output = BMP_CreateCopy( input );
    int width = BMP_GetWidth( input );
    int height = BMP_GetHeight( input );
    int x;
    int y;

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
                        BMP_GetPixelRGB( input, x + j, y + i, &r, &g, &b );
                        if( GetPixelValue( r ) )
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
                BMP_SetPixelRGB( output, x + current_operator.x, y + current_operator.y, 255, 255, 255 );
        }
    }

    return output;
}

BMP* dilatation( BMP* input, MORPH_OPERATOR_ENUM operator )
{
    MORPHOLOGICAL_OPERATOR current_operator = MORPHOLOGICAL_OPERATORS[ operator ];
    BMP* output = BMP_CreateCopy( input );
    int width = BMP_GetWidth( input );
    int height = BMP_GetHeight( input );
    int x;
    int y;

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
                        BMP_GetPixelRGB( input, x + j, y + i, &r, &g, &b );
                        if( !GetPixelValue( r ) )
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
                BMP_SetPixelRGB( output, x + current_operator.x, y + current_operator.y, 0, 0, 0 );
        }
    }

    return output;
}

BMP* opening( BMP* input, MORPH_OPERATOR_ENUM operator )
{
    return dilatation( erosion( input, operator ), operator );
}
