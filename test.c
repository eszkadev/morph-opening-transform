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
#include <time.h>
#include "operations.h"

#define EXIT_ERROR -1

int64_t timespecDiff( struct timespec* time_a, struct timespec* time_b )
{
    return ( ( time_a->tv_sec * 1000000000 ) + time_a->tv_nsec ) -
            ( ( time_b->tv_sec * 1000000000 ) + time_b->tv_nsec );
}

#define TEST(name, operation, operator, file1, file2) \
void name() { \
    printf( "%s ...\t", #name ); \
    BMP* bmp1 = BMP_ReadFile( file1 ); \
    BMP* bmp2; \
    IMAGE_MODEL* model2; \
    assert( bmp1 ); \
    IMAGE_MODEL* model1 = bmp_to_image_model( bmp1 ); \
    assert( model1 ); \
    /*print_model( model1 );*/ \
    if( strlen( file2 ) ) { \
        bmp2 = BMP_ReadFile( file2 ); \
        assert( bmp2 ); \
        model2 = bmp_to_image_model( bmp2 ); \
        assert( model2 ); \
        /*print_model( model2 );*/ \
    } \
    struct timespec before, after; \
    clock_gettime( CLOCK_MONOTONIC, &before ); \
    IMAGE_MODEL* output_model = operation( model1, operator ); \
    clock_gettime( CLOCK_MONOTONIC, &after ); \
    /*print_model( output_model );*/ \
    assert( output_model ); \
    if( strlen( file2 ) ) { \
        int result = compare_models( output_model, model2 ); \
        if( result == EQUALS ) printf( "OK" ); \
        else if( result == SIZE_DOESNT_MATCH ) printf( "Size doesn't match " ); \
        else if( result == CONTENT_IS_DIFFERENT ) ; \
        if( result != EQUALS ) { \
            printf( "FAILED" ); \
            char* output_file = (char*)malloc( sizeof(char) * ( strlen( #name ) + 8 ) ); \
            sprintf( output_file, "%s%s.bmp", #name, "out" ); \
            BMP* out = image_model_to_bmp( output_model ); \
            BMP_WriteFile( out, output_file ); \
            BMP_Free( out ); \
        } \
        free_image_model( model2 ); \
        BMP_Free( bmp2 ); \
    } \
    printf("\tTime: %lu ms\n", timespecDiff( &after, &before ) / 1000000L ); \
    free_image_model( model1 ); \
    free_image_model( output_model ); \
    BMP_Free( bmp1 ); \
} \

TEST( empty_3x3_erosion, erosion, CROSS, "test_data/3x3_empty.bmp", "test_data/3x3_empty.bmp" )
TEST( cross_3x3_erosion, erosion, CROSS, "test_data/3x3_cross.bmp", "test_data/3x3_cross_cross_erosion.bmp" )
TEST( wiki2_cross_erosion, erosion, CROSS, "test_data/wiki_2.bmp", "test_data/wiki_2_cross_erosion.bmp" )

TEST( empty_3x3_dilatation, dilatation, CROSS, "test_data/3x3_empty.bmp", "test_data/3x3_empty.bmp" )
TEST( wiki2_dilatation, dilatation, SQUARE, "test_data/wiki_2.bmp", "test_data/wiki_2_dilatation.bmp" )

TEST( wiki1_erosion, erosion, SQUARE, "test_data/wiki_1.bmp", "test_data/wiki_1_square_erosion.bmp" )
TEST( wiki1_opening, opening, SQUARE, "test_data/wiki_1.bmp", "test_data/wiki_1_square_opening.bmp" )

TEST( opening_1024, opening, SQUARE, "test_data/1024.bmp", "" )
TEST( opening_8096, opening, SQUARE, "test_data/8096.bmp", "" )

int main( int argc, char** argv )
{
    empty_3x3_erosion();
    cross_3x3_erosion();
    wiki2_cross_erosion();

    empty_3x3_dilatation();
    wiki2_dilatation();

    wiki1_erosion();
    wiki1_opening();

    opening_1024();
    opening_8096();

    return 0;
}
