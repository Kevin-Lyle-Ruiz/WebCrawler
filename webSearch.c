#include "webSearch.h"

#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#define BUFLEN	1024
char buf[BUFLEN];

int main(int argc, char** argv) {

	long seed;
	long maxPages;
	pageIndex* pages;

	if (argc < 3 || argc > 4) {
		fprintf(stderr, "USAGE: %s fileOfPages maxPagesToIndex [rand seed]\n", argv[0]);
		return -1;
	}

	if (argc >= 4) {
		seed = atol(argv[3]);
	}
	else {
		seed = time(NULL);
	}

	srand(seed);
	maxPages = atol(argv[2]);

	/*Index Start*/
	printf("Indexing...\n"); 

	pages = indexPagesFromFile(argv[1], maxPages); 
	if (pages == NULL) { 
		fprintf(stderr, "ERROR: INDEX NOT CREATED\n");
		return -1;
	}

	/* Clear memory */
	destroyPageIndex(pages); 

	return 0;
}