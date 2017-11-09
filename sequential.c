/*
 * Morphological Opening
 * Author: Szymon Kłos
 * 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse_input( int argc, char** argv, char** input_file, char** output_file );

int main( int argc, char** argv )
{
    char* output_file = NULL;
    char* input_file = NULL;

    parse_input( argc, argv, &input_file, &output_file );

    printf( "In: %s\nOut: %s\n", input_file, output_file );

    return 0;
}

int parse_input( int argc, char** argv, char** input_file, char** output_file )
{
    if( argc <= 1 )
    {
        fprintf( stderr, "Usage: ./sequential <input file> <output file>" );
        exit( -1 );
    }

    int input_length = strlen( argv[1] );
    *input_file = (char*)malloc( ( input_length + 1 ) * sizeof( char ) );
    strncpy( *input_file, argv[1], input_length );
    (*input_file)[input_length] = '\0';

    if( argc == 2 )
    {
        // only input provided, add _out to the file name and use as output

        static const char sufix[] = "_out";
        const int sufix_length = strlen( sufix );
        *output_file = (char*)malloc( ( input_length + sufix_length + 1 ) * sizeof( char ) );

        char* dot_char = strchr( *input_file, '.' );
        if( dot_char == NULL )
        {
            fprintf( stderr, "Input file extension not provided" );
            exit( -1 );
        }

        int dot_position = ( dot_char - *input_file );
        int chars_left = input_length + sufix_length - dot_position;
        strncpy( *output_file, argv[1], dot_position );
        (*output_file)[dot_position] = '\0';
        strncat( *output_file, sufix, chars_left );
        chars_left -= sufix_length;
        strncat( *output_file, dot_char, chars_left );
        (*output_file)[input_length + sufix_length] = '\0';
    }
    else
    {
        int output_length = strlen( argv[2] );
        *output_file = (char*)malloc( ( output_length + 1 ) * sizeof( char ) );
        strncpy( *output_file, argv[2], output_length );
        (*output_file)[output_length] = '\0';
    }
}