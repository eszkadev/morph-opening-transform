/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <qdbmp.h>

// Morphological operators
typedef enum
{
    CROSS = 0,
} MORPH_OPERATOR_ENUM;

BMP* erosion( BMP* input, MORPH_OPERATOR_ENUM operator );
BMP* dilatation( BMP* input, MORPH_OPERATOR_ENUM operator );
BMP* opening( BMP* input, MORPH_OPERATOR_ENUM operator );
