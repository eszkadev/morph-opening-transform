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
    IMAGE_MODEL* model1; \
    if( strstr( file1 + strlen( file1 ) - 4, "bmp" ) ) \
    { \
        BMP* bmp1 = BMP_ReadFile( file1 ); \
        if( !bmp1 ) exit( EXIT_ERROR ); \
        model1 = bmp_to_image_model( bmp1 ); \
        BMP_Free( bmp1 ); \
    } else \
        model1 = load_image_model( file1 ); \
    if( !model1 ) exit( EXIT_ERROR ); \
    /*print_model( model1 );*/ \
    struct timespec before, after; \
    IMAGE_MODEL* temp = create_image_model( model1->width, model1->height ); \
    IMAGE_MODEL* output_model = create_image_model( model1->width, model1->height ); \
    clock_gettime( CLOCK_MONOTONIC, &before ); \
    operation( model1, output_model, operator, temp ); \
    clock_gettime( CLOCK_MONOTONIC, &after ); \
    free_image_model( temp ); \
    free_image_model( model1 ); \
    /*print_model( output_model );*/ \
    if( !output_model ) exit( EXIT_ERROR ); \
    if( strlen( file2 ) ) { \
        BMP* bmp2 = BMP_ReadFile( file2 ); \
        if( !bmp2 ) exit( EXIT_ERROR ); \
        IMAGE_MODEL* model2 = bmp_to_image_model( bmp2 ); \
        BMP_Free( bmp2 ); \
        if( !model2 ) exit( EXIT_ERROR ); \
        /*print_model( model2 );*/ \
        int result = compare_models( output_model, model2 ); \
        free_image_model( model2 ); \
        if( result == EQUALS ) printf( "OK" ); \
        else if( result == SIZE_DOESNT_MATCH ) printf( "Size doesn't match " ); \
        else if( result == CONTENT_IS_DIFFERENT ) ; \
        if( result != EQUALS ) { \
            printf( "FAILED" ); \
            char* output_file = (char*)malloc( sizeof(char) * ( strlen( #name ) + 8 ) ); \
            sprintf( output_file, "%s%s.bmp", #name, "out" ); \
            BMP* out = image_model_to_bmp( output_model ); \
            if( !out ) exit( EXIT_ERROR ); \
            BMP_WriteFile( out, output_file ); \
            BMP_Free( out ); \
            free( output_file ); \
        } \
    } \
    printf("\tTime: %lu ms\n", timespecDiff( &after, &before ) / 1000000L ); \
    free_image_model( output_model ); \
} \

TEST( empty_3x3_erosion, erosion, CROSS, "test_data/3x3_empty.bmp", "test_data/3x3_empty.bmp" )
TEST( cross_3x3_erosion, erosion, CROSS, "test_data/3x3_cross.bmp", "test_data/3x3_cross_cross_erosion.bmp" )
TEST( wiki2_cross_erosion, erosion, CROSS, "test_data/wiki_2.bmp", "test_data/wiki_2_cross_erosion.bmp" )

TEST( empty_3x3_dilatation, dilatation, CROSS, "test_data/3x3_empty.bmp", "test_data/3x3_empty.bmp" )
TEST( wiki2_dilatation, dilatation, SQUARE, "test_data/wiki_2.bmp", "test_data/wiki_2_dilatation.bmp" )

TEST( wiki1_erosion, erosion, SQUARE, "test_data/wiki_1.bmp", "test_data/wiki_1_square_erosion.bmp" )
TEST( wiki1_opening, opening, SQUARE, "test_data/wiki_1.bmp", "test_data/wiki_1_square_opening.bmp" )

TEST( opening_1024, opening, SQUARE, "test_data/1024.bmp", "" )
TEST( opening_2048, opening, SQUARE, "test_data/2048.model", "" )
TEST( opening_4096, opening, SQUARE, "test_data/4096.model", "" )
TEST( opening_8192, opening, SQUARE, "test_data/8192.model", "" )
TEST( opening_16384, opening, SQUARE, "test_data/16384.model", "" )
TEST( opening_32768, opening, SQUARE, "test_data/32768.model", "" )
TEST( opening_48000, opening, SQUARE, "test_data/48000.model", "" )
TEST( opening_56000, opening, SQUARE, "test_data/56000.model", "" )

int main( int argc, char** argv )
{
    printf( "============== Unit testing ===============\n" );

    empty_3x3_erosion();
    cross_3x3_erosion();
    wiki2_cross_erosion();

    empty_3x3_dilatation();
    wiki2_dilatation();

    wiki1_erosion();
    wiki1_opening();

    printf( "=========== Performance testing ===========\n" );

    //opening_1024();
    //opening_2048();
    //opening_4096();
    //opening_8192();
    opening_16384();
    //opening_32768();
    //opening_48000();
    opening_56000();

    return 0;
}
