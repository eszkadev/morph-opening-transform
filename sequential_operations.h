/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <qdbmp.h>
#include "image_model.h"

// Morphological operators
typedef enum
{
    CROSS = 0,
    SQUARE = 1,
} MORPH_OPERATOR_ENUM;

IMAGE_MODEL* erosion( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator );
IMAGE_MODEL* dilatation( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator );
IMAGE_MODEL* opening( IMAGE_MODEL* input, MORPH_OPERATOR_ENUM operator );
