/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include "sequential_operations.h"

BMP* erosion( BMP* input )
{
    return input;
}

BMP* dilatation( BMP* input )
{
    return input;
}

BMP* opening( BMP* input )
{
    return dilatation( erosion( input ) );
}
