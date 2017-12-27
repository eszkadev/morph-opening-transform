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
#define EROSION 1

#define TEST(name, operation, file1, file2) \
void name() { \
    printf( "%s ...\t", #name ); \
    BMP* bmp1 = BMP_ReadFile( file1 ); \
    assert( bmp1 ); \
    BMP* bmp2 = BMP_ReadFile( file2 ); \
    assert( bmp2 ); \
    BMP* out = NULL; \
    if( operation == EROSION ) \
        out = erosion( bmp1, CROSS ); \
    assert( out ); \
    if( compare_images( out, bmp2 ) ) printf( "OK\n" ); \
    else { \
        printf( "FAILED\n" ); \
        char* output_file = (char*)malloc( sizeof(char) * ( strlen( #name ) + 8 ) ); \
        sprintf( output_file, "%s%s.bmp", #name, "out" ); \
        BMP_WriteFile( out, output_file ); \
    } \
    BMP_Free( bmp1 ); \
    BMP_Free( bmp2 ); \
    BMP_Free( out ); \
} \

int compare_images( BMP* bmp1, BMP* bmp2 )
{
    unsigned int width1 = BMP_GetWidth( bmp1 );
    unsigned int height1 = BMP_GetWidth( bmp1 );
    unsigned int width2 = BMP_GetWidth( bmp2 );
    unsigned int height2 = BMP_GetWidth( bmp2 );

    unsigned int x, y;
    unsigned char r1, g1, b1;
    unsigned char r2, g2, b2;

    if( width1 != width2 || height1 != height2 )
    {
        printf("Size doesn't match!");
        return 0;
    }

    for( y = 0; y < height1; ++y )
    {
        for( x = 0; x < width1; ++x )
        {
            BMP_GetPixelRGB( bmp1, x, y, &r1, &g1, &b1 );
            BMP_GetPixelRGB( bmp2, x, y, &r2, &g2, &b2 );

            if( r1 != r2 || g1 != g2 || b1 != b2 )
                return 0;
        }
    }

    return 1;
}

TEST(cross_3x3, EROSION, "test_data/3x3_cross.bmp", "test_data/3x3_cross_cross_erosion.bmp")
TEST(wiki2_cross_erosion, EROSION, "test_data/wiki_2.bmp", "test_data/wiki_2_cross_erosion.bmp")

int main( int argc, char** argv )
{
    cross_3x3();
    wiki2_cross_erosion();

    return 0;
}
