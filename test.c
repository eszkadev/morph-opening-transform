/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <qdbmp.h>
#include "sequential_operations.h"

#define EXIT_ERROR -1

#define TEST(name, operation, file1, file2) \
void name() { \
    printf( "%s ...\t", #name ); \
    BMP* bmp1 = BMP_ReadFile( file1 ); \
    assert( bmp1 ); \
    IMAGE_MODEL* model1 = bmp_to_image_model( bmp1 ); \
    assert( model1 ); \
    /*print_model( model1 );*/ \
    BMP* bmp2 = BMP_ReadFile( file2 ); \
    assert( bmp2 ); \
    IMAGE_MODEL* model2 = bmp_to_image_model( bmp2 ); \
    assert( model2 ); \
    /*print_model( model2 );*/ \
    IMAGE_MODEL* output_model = operation( model1, CROSS ); \
    /*print_model( output_model );*/ \
    assert( output_model ); \
    int result = compare_models( output_model, model2 ); \
    if( result == EQUALS ) printf( "OK\n" ); \
    else if( result == SIZE_DOESNT_MATCH ) printf( "Size doesn't match " ); \
    else if( result == CONTENT_IS_DIFFERENT ) ; \
    if( result != EQUALS ) { \
        printf( "FAILED\n" ); \
        char* output_file = (char*)malloc( sizeof(char) * ( strlen( #name ) + 8 ) ); \
        sprintf( output_file, "%s%s.bmp", #name, "out" ); \
        BMP* out = image_model_to_bmp( model2 ); \
        BMP_WriteFile( out, output_file ); \
        BMP_Free( out ); \
    } \
    free_image_model( model1 ); \
    free_image_model( model2 ); \
    free_image_model( output_model ); \
    BMP_Free( bmp1 ); \
    BMP_Free( bmp2 ); \
} \

TEST(cross_3x3, erosion, "test_data/3x3_cross.bmp", "test_data/3x3_cross_cross_erosion.bmp")
TEST(wiki2_cross_erosion, erosion, "test_data/wiki_2.bmp", "test_data/wiki_2_cross_erosion.bmp")

int main( int argc, char** argv )
{
    cross_3x3();
    wiki2_cross_erosion();

    return 0;
}
