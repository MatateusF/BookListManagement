# BookListManagement


Consider information about a list of books stored in a text file like the attached example,
transcribed from the website “openisbn.com”, whose lines contain the following data string:
Title ; ISBN ; ISBN-13 ; Author(s) ; Editor ; Key words ; Number of pages ; Publication date ; 
Kind of binding ; Price.

It's proposed to create the Book type as an element of a data structure to represent the data of the
books.

  typedef struct book {
      char title\[MAX_TITLE];
      char isbn\[MAX_ISBN];
      char authors\[MAX_AUTHORS];
      char publisher\[MAX_PUB_NAME];
  } Book;

It's intended a program that reads the list of books, from a file, storing it in a struct array with
type Book, and allows the presentation of data, in response to commands introduced through standard input.

Considering the creation of variables without resorting to dynamic allocation, admit a simplified scenario
in that there are maximum dimensions for the information elements -> books, title characters and characters
names of authors or publisher. These dimensions can be identified from the data file, with the help of Linux
commands (eg.: cat _file_ | cut -d";" -f _field_number_ | wc _-options_). 
It's recommended to oversize the observed quantities.

It's suggested to read the input file using the _fgets_ function, using a memory space oversized relative to
the longest line. You can add data to the sample file, or create others files, if it's convenient for testing
and demonstrating the program.

After reading the file and filling in the data structure, the program should wait, in cycle, for the
following commands:
  -> t - Displays the list of all books, indicating, for each: title, author, editor and ISBN; 
          The produced list is ordered by title;
  -> i \[isbn] Displays the data related to the book with the indicated ISBN;
          If it does not exist, it displays a advice;
  -> q - Exit the program.
