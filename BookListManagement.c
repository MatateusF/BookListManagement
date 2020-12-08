#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Questão 2.1Question 2.1
 * NOTE: the commented numbers represent the values  
 * observed after executing the command in the terminal
 */
#define MAX_TITLE 250		// 201
#define MAX_ISBN 15			// 12
#define MAX_AUTHORS 150		// 93
#define MAX_PUB_NAME 100	// 53
#define MAX_LN 512
#define MAX_BOOKS 300
#define SEPARADOR ";"
#define SIZE 20

typedef struct book {
	char title[MAX_TITLE];
	char isbn[MAX_ISBN];
	char authors[MAX_AUTHORS];
	char publisher[MAX_PUB_NAME];
} Book;

int getFirstPosition(char *);					
int getLastPosition(char *);					
char *strTrim(char *);						
int readBook(Book *, FILE *);					
int loadList(Book [], size_t, FILE *);			
int cmpFuncTitle(const void *, const void *);	
void sortTitle(Book [], size_t);				
int cmpFuncIsbn(const void *, const void *);		
void sortIsbn(Book *[], size_t);				

/*
 * Objective: Application program capable of accessing the file, filling in 
 * 			the data and respond to the commands "t", "i [isbn]" and "q";
 * 			to answer the command "i [isbn]" an array of pointers to the
 * 			Book structure, looking for [isbn] passed using the bsearch function
 */
int main() {
	char str[SIZE];
	char *firstChar;
	Book list[MAX_BOOKS], *auxList[MAX_BOOKS];	// Lists with books	
	FILE *f = fopen("dados.csv", "r");

	// Loads books with information into the structure
	int numBooks = loadList(list, sizeof list, f);
	printf("Total books read: %d\n", numBooks);

	sortTitle(list, numBooks);

	// Array of pointers to Book, required to respond to the command 'i [isbn]'
	for(int j = 0; j < numBooks; j++)
		auxList[j] = &list[j];

	// Home menu
	printf("\nEnter one of the following commands:\n");
	printf("  t        -> Displays the list of books ordered by title;\n");
	printf("  i [isbn] -> Displays data related to the book identified by [isbn];\n");
	printf("  q        -> End the program!\n  --> ");

	while(fgets(str, SIZE, stdin)) {
		// Just accept 't\n' 'q\n' and 'i [isbn]', everything else gives "Invalid command"
		if((str[0] == 'i' && str[1] != ' ') || (str[0] != 'i' && str[1] != '\n'))
			printf("\nInvalid command! Try again!\n");

		else {
			firstChar = strtok(str, " ");
			
			switch(*firstChar) {
				case 't':
					for(int j = 0; j < numBooks; j++)
						printf("[%d] \tTitle: %s\n\tAuthor(s): %s\n\tPublisher: %s\n\tISBN: %s\n\n", j+1, list[j].title, list[j].authors, list[j].publisher, list[j].isbn);

					break;

				case 'i':
					sortIsbn(auxList, numBooks);

					char *secondISBN = strtok(NULL, "\n");

					if(secondISBN == NULL) {
						printf("\nInvalid command! Try again!\n");
						break;
					}
					
					Book **bookFound;
					Book auxFirst, *auxSecond;	// Auxiliary variables to transform char * into Book *

					// Passing from char * to Book * because the key 
					// needs to have the same type as the array
					strcpy(auxFirst.isbn, secondISBN);
					auxSecond = &auxFirst;

					// Search for the book with the last ISBN
					bookFound = (Book **) bsearch(&auxSecond, auxList, numBooks, sizeof(Book *), cmpFuncIsbn);

					if(bookFound != NULL)
						printf("\n  Title: %s\n  Author(s): %s\n  Publisher: %s\n  ISBN: %s\n\n", (*bookFound)->title, (*bookFound)->authors, (*bookFound)->publisher, (*bookFound)->isbn);
					else
						printf("\nThe book with that ISBN was not found! Try again!\n");
				
					break;

				case 'q':
					return 0;

				default:
					printf("\nInvalid command! Try again!\n");
					break;
			}
		}
		
		// Home menu
		printf("\nEnter one of the following commands:\n");
		printf("  t        -> Displays the list of books ordered by title;\n");
		printf("  i [isbn] -> Displays data related to the book identified by [isbn];\n");
		printf("  q        -> End the program!\n  --> ");
	}
	
	return 0;
}

/*
 *	Auxiliary function
 * 	Name: getFirstPosition
 * 	(Return) Objective: (int) get the first position of the string without spaces, tabs or newlines
 */
int getFirstPosition(char *str) {
	int i = 0;
    
	// Getting the first position
	while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i += 1;
    
	return i;
}

/*
 *	Auxiliary function
 * 	Name: getLastPosition
 * 	(Return) Objective: (int) get the last position of the string without spaces, tabs or newlines
 */
int getLastPosition(char *str) {
	int i = strlen(str) - 1;
    
	// Getting the last position
	while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i -= 1;
    
	return i;
}

/*
 * 	Name: strTrim
 * 	(Return) Objective: (char *) eliminates separator characters, if any, at the beginning or at the end 
 * 						of the string indicated by str and place the string terminator after the last
 * 						existing word, returning a pointer to the beginning of the first word
 */
char *strTrim(char *str) {
	if(!str) 
		return NULL;

	int i;
	int len = (getLastPosition(str) - getFirstPosition(str)) + 1;
	int start = getFirstPosition(str);
	
	for(i = 0; i < len; i++)
		str[i] = str[start+i];
	
	str[i] = '\0';
	
	return str;
}

/*
 * 	Name: readBook
 * 	(Return) Objective: (int) obtains information from a book by reading a line from file f, 
 * 						identified the line fields and copying the desired information to the
 * 						structure; returns 1 on success and 0 if there is no data to read
 */
int readBook(Book *b, FILE *f) {
	if(f == NULL || b == NULL)
		return 0;
		
	char str[MAX_LN + 1];		// Due to \0
	char *firstToken, *secondToken, *thirdToken, *fourthToken;
	
	// Reading a line from the file
	if(fgets(str, sizeof(str), f) == NULL)
		return 0;		
	
	// Title storage in the structure
	firstToken = strtok(str, SEPARADOR);
	strcpy(b->title, strTrim(firstToken));
	
	// ISBN storage in the structure
	secondToken = strtok(NULL, SEPARADOR);
	strcpy(b->isbn, strTrim(secondToken));
	
	// Skip the ISBN-13 column
	strtok(NULL, SEPARADOR);
	
	// Authors storage in the structure
	thirdToken = strtok(NULL, SEPARADOR);
	strcpy(b->authors, strTrim(thirdToken));
	
	// Publisher storage in the structure
	fourthToken = strtok(NULL, SEPARADOR);
	strcpy(b->publisher, strTrim(fourthToken));
	
	return 1;
}

/*
 * 	Name: loadList
 * 	(Return) Objective: (int) loads the list of books into the list array (which has the  
 * 						maximum size dimension) from file f and returns the number of books read
 */
int loadList(Book list[], size_t size, FILE *f) {
	if(f == NULL)
		return 0;
		
	int booksRead = 0;
	
	for(int i = 0; i < (size/sizeof(list[0])); i++)
		if(readBook(&list[i], f))
			booksRead++;
	
	return booksRead;
}

/*
 * 	Nome: cmpFuncTitle
 * 	(Return) Objective: (int) returns 0 if the titles passed in parameter are the same,
 * 						<0 if the title of the first parameter is less than the second or
 * 						>0 if the title of the first parameter is greater than the second
 */
int cmpFuncTitle(const void *book1, const void *book2) {
	Book *b1 = (Book *)book1;
	Book *b2 = (Book *)book2;
	
    return strcmp(b1->title, b2->title);
}

/*
 * 	Name: sortTitle
 * 	(Return) Objective: (void) sorts them in alphabetical order by title  
 *						in list, to answer the command "t" 
 */
void sortTitle(Book list[], size_t num) {
	// Ascending sorting of the list array according to the cmpFuncTitle function
	qsort(list, num, sizeof(Book), cmpFuncTitle);
}

/*
 * 	Name: cmpFuncIsbn
 * 	(Return) Objective: (int) returns 0 if the ISBNs passed in parameter are the same,
 * 							<0 if the ISBN of the first parameter is less than the second or
 * 							>0 if the ISBN of the first parameter is greater than the second
 */
int cmpFuncIsbn(const void *book1, const void *book2) {
	Book **b1 = (Book **)book1;
	Book **b2 = (Book **)book2;
	
    return strcmp((*b1)->isbn, (*b2)->isbn);
}

/*
 * 	Name: sortIsbn
 * 	(Return) Objective: (void) sorts, in numerical ISBN order, those in books 
 *						in auxList, to answer the command "i [isbn]" 
 */
void sortIsbn(Book *auxList[], size_t num) {
	// Ascending sorting of the array list according to the cmpFuncIsbn function
	qsort(auxList, num, sizeof(Book *), cmpFuncIsbn);
}
