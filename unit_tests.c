// unit_tests.c
// Larry L. Kiser Oct. 22, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "analysis.h"
#include "unit_tests.h"

// Simple boolean assert function for unit testing
// DO NOT MODIFY THIS FUNCTION
int assert( int test_result, char error_format[], ... ) {
	va_list arguments ;
	static int test_number = 1 ;
	int result = 1 ;	// return 1 for test passed or 0 if failed

	if ( ! test_result ) {
		va_start( arguments, error_format ) ;
		printf( "Test # %d failed: ", test_number ) ;
		vprintf( error_format, arguments ) ;
		printf( "\n" ) ;
		result = 0 ;
	}
	test_number++ ;
	return result ;
}

//////////////////////////////////////////////////////////////////////////
// Begin unit tests //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Write your unit tests below. I have provided some simple tests to get started.
// Add, rearrange, delete, modify as you see fit to get a good set of tests for
// both boundry conditions and normal operation.
int test()
{
	int passcount = 0 ;
	int failcount = 0 ;
	int result;		// general purpose work variable for functions that return integers
	struct word_entry one_entry ;	// general purpose return struct for unit tests
	int count;		// get the sentence_count and unique_word_count
	char* word;		//Get the most common word after the given word.

	// Test 1 Verify that read_file handles a NULL file name pointer.
	result = read_file( NULL ) ;
	assert( result == 0,
		"A bad or NULL file name must return 0. Got %d", result )
		? passcount++ : failcount++ ;

	// Test 2 Verify that read_file handles an EMPTY file
	result = read_file( "empty_file.txt" ) ;
        assert( result == 0,
                "An empty file must return 0. Got %d", result )
                ? passcount++ : failcount++ ;

	// Test 3 READ a real file
	result = read_file( "test1.txt" ) ;
	assert( result == 1,
		"Failure reading ActivityJournal.txt. Got result code %d", result )
		? passcount++ : failcount++ ;

	// Test 4, 5, 6, 7, 8 FREE the list and confirm that the list is now empty
	// Also test function: GET_FIRST_WORD, GET_NEXT_WORD, GET_PREV_WORD, GET_LAST_WORD and GET_SENTENCE_COUNT by an empty list.
        free_list();
        one_entry = get_first_word() ;
        assert( one_entry.word_count == 0,
                "Expected the get 0 word_count since the list is now empty. Got word_count %d", one_entry.word_count )
                ? passcount++ : failcount++ ;
	// Test 5
	one_entry = get_next_word() ;
        assert( one_entry.word_count == 0,
                "Expected the get 0 word_count since the list is now empty. Got word_count %d", one_entry.word_count )
                ? passcount++ : failcount++ ;
	// Test 6
	one_entry = get_prev_word() ;
        assert( one_entry.word_count == 0,
                "Expected the get 0 word_count since the list is now empty. Got word_count %d", one_entry.word_count )
                ? passcount++ : failcount++ ;
	// Test 7
        one_entry = get_last_word() ;
        assert( one_entry.word_count == 0,
                "Expected the get 0 word_count since the list is now empty. Got word_count %d", one_entry.word_count )
                ? passcount++ : failcount++ ;
	// Test 8
        count = get_sentence_count() ;
        assert( count == 0,
                "Expected the get 0 word_count since the list is now empty. Got word_count %d", count )
                ? passcount++ : failcount++ ;

	// Test 9 Test the function GET_FIRST_WORD.
	// Read the "test1.txt" again and verify that the first word is "activity"
	result = read_file( "test1.txt" ) ;
	one_entry = get_first_word() ;
	assert( strcmp( one_entry.unique_word, "activity" ) == 0,
		"Expected to get 'activity' for the first word. Got unique_word %s", one_entry.unique_word )
		? passcount++ : failcount++ ;

	// Test 10 Test the function GET_FIRST_WORD for word_count
	assert( one_entry.word_count == 2,
                "Expected to get 2 because 'activity' occurs 2 times in the file. For word_count %d", one_entry.word_count)
                ? passcount++ : failcount++ ;

	// Test 11 Test the function GET_PREVIOUS_WORD for the head because CURRENT pointer now is at the head.
        one_entry = get_prev_word() ;
        assert( one_entry.word_count == 0,
                "Expected to get 0 word_count since the current has no previous node. Got word_count %d", one_entry.word_count )
                ? passcount++ : failcount++ ;

	// Test 12 Test the function GET_NEXT_WORD
        one_entry = get_next_word() ;
        assert( strcmp(one_entry.unique_word, "estimated") == 0,
                "Expected to get 'estimated'. Got unique_word %s", one_entry.unique_word )
                ? passcount++ : failcount++ ;

	// Test 13 Test the function GET_LAST_WORD. 
	// Also test if the apostrophy is added to the word.
        one_entry = get_last_word() ;
        assert( strcmp(one_entry.unique_word, "your's") == 0,
                "Expected to get 'your's'. Got last word %s", one_entry.unique_word )
                ? passcount++ : failcount++ ;

	// Test 14 Test the function GET_SENTENCE_COUNT.
	// Also test how the function handle '...' and '()' and a period preceeded by nothing
	count = get_sentence_count() ;
        assert( count == 4,
                "Expected to get 4 sentences. Got sen_count %d", count )
                ? passcount++ : failcount++ ;

	// Test 15, 16, 17 Test the function GET_UNIQUE_WORD_COUNT.
	// Test 15 Word that is not in the list
        count = get_unique_word_count("strawberry") ;
        assert( count == 0,
                "Expected to get 0 because 'strawberry' is not in the list. Got count %d", count )
                ? passcount++ : failcount++ ;

	// Test 16 Word that is in the list
        count = get_unique_word_count("file") ;
        assert( count == 3,
                "Expected to get 3 because 'file' occures 3 time in the file. Got count %d", count )
                ? passcount++ : failcount++ ;

	// Test 17 Word that is in the list
        count = get_unique_word_count("activity") ;
        assert( count == 2,
                "Expected to get 2 because 'activity' occures 2 time in the file. Got count %d", count )
                ? passcount++ : failcount++ ;

	// Test 18 Test GET_MOST_COMMON_AFTER_THIS_WORD
	// Word that has the second most common.
        word = get_most_common_word_after_this_word("file") ;
        assert( strcmp( word, "activity") == 0,
                "Expected to get 'activity' because 'activity' occurs 2 time, which is the most common word after 'file'. Got word %s", word )
                ? passcount++ : failcount++ ;

	// Test 19 Test GET_MOST_COMMON_AFTER_THIS_WORD
	//Word that doesn't have the second most common.
        word = get_most_common_word_after_this_word("name") ;
        assert( word == NULL,
                "Expected to get NULL because 'name' occurs just 1 time in the file." )
                ? passcount++ : failcount++ ;

	free_list();
	printf( "\nSummary of unit tests:\n%d tests passed\n%d tests failed\n\n", passcount, failcount ) ;

	return failcount ;
}
