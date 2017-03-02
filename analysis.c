
// Document Analysis project functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "analysis.h"
#include "unit_tests.h"

#define MAX_WORD_LENGTH 50

// Define your linked list here. It should be prefixed with static so it is only accessible
// from inside this module.
// Also, you will need to maintain the current location in the doubly linked list to support the next and previous functions.
struct node {
        struct word_entry entry;
        struct node* next;
	struct node* previous;
};

//The HEAD of the linked list
static struct node* head = NULL;
//The CURRENT pointer
static struct node* current = NULL;
//The COUNT of sentences
int sen_count = 0;

//COMPARE string1 and string2 //Return 0 if they are equal.
//Return -1 if string1 < string2, 1 if string1 > string2, alphabetically.
int compare( char* str1, char* str2 )
{
	int i = 0;
	int result = 0;
	while( i < strlen( str1 ) && i < strlen( str2 ) )
	{
		if ( str1[i] < str2[i] )
		{
			result = -1;
			break;
		}
		else if ( str1[i] > str2[i] )
		{
			result = 1;
			break;
		}
		i++;
	}
	//If two string have the same head, the longer string will be after the shorter one in alphabetical order.
	if ( (result == 0) && (strlen( str1 ) > strlen( str2 )) )
		result = 1;
	else if ( (result == 0) && (strlen( str1 ) < strlen( str2 )) )
                result = -1;
	return result;
}

//PUSH a new word to the doubled linked list 
//The UNIQUE_WORD will be added ALPHABETICALLY 
//If the word already existed, increase the WORD_COUNT 
void push( char* w )
{
	//Create new node that contains the word
	struct node* word;
        word = malloc( sizeof( struct node ) );
	word->entry.unique_word = w;
        word->entry.word_count = 1;
        word->next = NULL;
        word->previous = NULL;

	//If the list is empty, add to the TOP:
	if ( !head )
	{
		head = word;
		//Set CURRENT pointer to the HEAD of the list.
		current = head;
		return;
	}
	else
	{
        	struct node *p_work;
        	p_work = head;

		//Check if we should add it to the FRONT of the HEAD
		if ( compare( word->entry.unique_word, head->entry.unique_word ) < 0)
                {
                        word->next = head;
                        head->previous = word;
			//Point the HEAD and the CURRENT again to the first node.
                        head = word;
			current = head;
                        p_work = head;
                        return;
		}

		//Loop through the list and see if we can add in the MIDDLE of the list.
		//If the word already existed, increase the WORD_COUNT
	        while ( p_work->next )
        	{
			if ( (p_work->previous) && ( compare( word->entry.unique_word, p_work->entry.unique_word ) < 0 ) )
                	{
                        	word->next = p_work;
                        	word->previous = p_work->previous;
                        	p_work->previous->next = word;
                        	p_work->previous = word;
				p_work = head;
                        	return;
                	} else if ( compare( word->entry.unique_word, p_work->entry.unique_word ) == 0 )
			{
                        	p_work->entry.word_count++;
				p_work = head;
				free(word);
                        	return;
                	}
                	else
                	{
                        	p_work = p_work->next;
                	}
        	}

		//Check to add either to the FRONT or the BACK of the LAST node:
		if ( (p_work->previous) && ( compare( word->entry.unique_word, p_work->entry.unique_word ) < 0 ) )
                {
                        word->next = p_work;
                        word->previous = p_work->previous;
                        p_work->previous->next = word;
                        p_work->previous = word;
                        p_work = head;
                        return;
                }
		else if ( compare( word->entry.unique_word, p_work->entry.unique_word ) == 0 )
		{
			p_work->entry.word_count++;
                        p_work = head;
			free(word);
			return;
		}
		else
		//Add to the BACK of the list
		{
			p_work->next = word;
			word->previous = p_work;
			p_work = head;
		}
	}
}

// returns 0 if the file cannot be read or if the file does not contain any words as defined for this project.
// returns 0 if the pointer to the file name is NULL.
// returns 1 if the file was found, opened successfully, and successfully read into the doubly linked list.
int read_file( char *file_name )
{
	//If file name is NULL:
	if ( !file_name )
		return 0;

	FILE *file;
	file = fopen( file_name, "r" );
	char w[MAX_WORD_LENGTH +1];
        //To check if there is an ellipsis "..."
        int multiple_period = 0;
        int ch, i = 0;

	//Check if the file is empty
	if ( (ch = fgetc(file)) == EOF )
		return 0;

	fseek( file, 0, SEEK_SET );

	while( (ch = fgetc(file)) != EOF )
	{
		//Just accept valid word and the possessive character. Also count the period for number of sentences.
		if ( (!isspace(ch) && isalpha(ch)) || (ch == 39) || (ch == 46) )
		{
			if ((ch == 46) && (strlen(w) > 0) )
			{
				// If the number of period symbols is just more than 1 and preceded by a word, just count it as 1 sentence.
				multiple_period++;
				if (multiple_period == 1)
					sen_count++;
			}
			// Still add if it's an Apostrophe
			if (ch != 46)
			{
				w[i] = tolower(ch);
				i++;
			}
		} else
		{
		//End the word with '\0'
		w[i] = '\0';

		//Handel multiple whitespaces next to each other.
		if ( w[0] == '\0' )
			continue;

		//Create a copy of the string by malloc a new dynamic memory block.
		char* copy = malloc( strlen(w) + 1 );
		strcpy( copy, w);

		//Push the word to the linked list.
		push(copy);

		//Reset w and i and multiple_period
		w[0] = '\0';
               	i = 0;
		multiple_period = 0;
		}
	}
	return 1;
}


// This walks through the linked list and frees every node in the linked list.
// After freeing all nodes in the list be sure to set your head and tail
// pointers to NULL to indicate that the list is now empty.
void free_list()
{
	struct node* p_work;
	p_work = head;

	while ( head != NULL )
	{
		head = head->next;
		free( p_work->entry.unique_word );
		free( p_work );
		p_work = head;
	}
	current = head;
	sen_count = 0;
}

// Returns 0 in the word_count field if no first word (empty list). 
// Otherwise, returns a struct with the first unique word and its number of occurrences in the text file. 
struct word_entry get_first_word() 
{
	struct word_entry one_word ;

	if (!head)
	{
		one_word.unique_word = NULL;
        	one_word.word_count = 0;
	} else
	{
		one_word.unique_word = head->entry.unique_word;
		one_word.word_count = head->entry.word_count;
	}
	return one_word ;
}

// Returns 0 in the word_count field if no next word (previously reached end of list or it is an empty list). 
// Otherwise, returns a struct with the next unique word and its number of occurrences in the text file. 
struct word_entry get_next_word() 
{
	struct word_entry one_word ;

	if (!head || !current->next)
	{
		one_word.unique_word = NULL;
                one_word.word_count = 0;
	} else
	{
		current = current->next;
                one_word.unique_word = current->entry.unique_word;
                one_word.word_count = current->entry.word_count;

	}
	return one_word ;
}

// Returns 0 in the word_count field if no previous word (was already at beginning of the list or it is an empty list). 
// Otherwise, returns a struct with the previous unique word and its number of occurrences in the text file. 
struct word_entry get_prev_word() 
{
	struct word_entry one_word ;

	if (!head || !current->previous)
	{
		one_word.unique_word = NULL;
                one_word.word_count = 0;
	} else
	{
		current = current->previous;
                one_word.unique_word = current->entry.unique_word;
                one_word.word_count = current->entry.word_count;
	}
	return one_word ;
}

// Returns 0 in the word_count field if it is an empty list). 
// Otherwise, returns a struct with the last unique word and its number of occurrences in the text file. 
struct word_entry get_last_word() 
{
	struct word_entry one_word ;

	if (!head)
	{
		one_word.unique_word = NULL;
                one_word.word_count = 0;
	} else
	{
		struct node* p_work;
                p_work = head;
		while ( p_work->next )
		{
			p_work = p_work->next;
		}
		one_word.unique_word = p_work->entry.unique_word;
                one_word.word_count = p_work->entry.word_count;
		current = p_work;
	}

	return one_word ;
}

// Returns 0 if no sentences include an empty list and if there are no periods in the file. 
// Otherwise returns number of sentences. 
int get_sentence_count()
{
	return sen_count;
}

// returns 0 if word not found in the list. 
// Searches the list for the word and returns its number of occurrences. 
int get_unique_word_count( char *word_to_find ) 
{
	if ( !head )
		return 0 ;

	struct node* p_work;
        p_work = head;

	while ( p_work )
	{
		if ( strcmp( p_work->entry.unique_word, word_to_find ) == 0 )
		{
			return p_work->entry.word_count;
		} else
		{
			p_work = p_work->next;
		}
	}
	return 0;
}

// Returns a NULL pointer if word_to_find is not found in the list. 
// Searches the list for word_to_find and returns the word that most commonly occurs after it. 
// If word_to_find is ALWAYS the last word in the sentence it returns a pointer to an empty string. 
char *get_most_common_word_after_this_word( char *word_to_find ) 
{
	int current_count;
	int working_count = 0;
	char* ch;

	current_count = get_unique_word_count( word_to_find );
	if (current_count == 0)
		return (char *)NULL;
	else
	{
		struct node* p_work;
        	p_work = head;

        	while ( p_work )
        	{
                	if ( (p_work->entry.word_count < current_count) && (p_work->entry.word_count > working_count) )
                	{
				working_count = p_work->entry.word_count;
				ch = p_work->entry.unique_word;
                	} else
                	{
                        	p_work = p_work->next;
                	}
        	}
		if (working_count == 0)
			return (char *)NULL;
		else
			return ch;
	}

	//return (char *)NULL ;	// temporary code that indicates that the word is not found.
}

// Writes the sorted unique word list to a csv file.
// Receives one parameter which is the name of the file to be created.
// Returns 1 on a successful write of the file.
// Returns 0 on any failure.
// Test for a NULL or empty string in the file_name. Return 0 for failure if NULL or empty.
// Be sure to test for failure to open the file, failure to write to the file, and failure to close.
// You must have a header row exactly like this (without the quotes): "word,count"
// You must have one row for each unique word and its count (e.g. without quotes "student,5").
// It must be in sorted order and must be the complete list.
int write_unique_word_list_to_csv_file( char *file_name )
{
	return 0 ; // temporary code so it compiles and links.
}
