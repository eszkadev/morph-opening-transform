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

void erosion( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator );
void dilatation( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator );
void opening( IMAGE_MODEL* input, IMAGE_MODEL* output, MORPH_OPERATOR_ENUM operator );
