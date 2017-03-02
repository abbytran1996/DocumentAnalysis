// analysis.h
// L. Kiser Nov. 20, 2015

// document analysis project struct
struct word_entry
{
	char *unique_word ;
	int word_count ;
} ;

// document analysis project function prototypes
// The unit tests need access to these functions.
extern int read_file( char *file_name ) ;
extern void free_list() ;
extern struct word_entry get_first_word() ;
extern struct word_entry get_next_word() ;
extern struct word_entry get_prev_word() ;
extern struct word_entry get_last_word() ;
extern int get_sentence_count() ;
extern int get_unique_word_count( char *word_to_find ) ;
extern char *get_most_common_word_after_this_word( char *word_to_find ) ;
extern int write_unique_word_list_to_csv_file( char *file_name ) ;
