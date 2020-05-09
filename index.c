#include "crawler.h"
#include "index.h"

#include <ctype.h>

pageIndex* indexPagesFromFile(const char* filename, int maxPages) {
	page** pages; 
	pageIndex* indexedPages;

	FILE* indexFile;
	char url[MAX_ADDR_LENGTH];
	int maxHops, hopNum;
	listNode* listStart;
	int i;

	indexFile = fopen(filename, "r");

	if (indexFile == NULL) {
		fprintf(stderr, "ERROR: Could not open the file\n");
		return NULL;
	}

	pages = malloc(sizeof(page*)*(maxPages+1));

	if (pages == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory\n");
		fclose(indexFile);
		return NULL;
	}

	indexedPages = malloc(sizeof(pageIndex));

	if (indexedPages == NULL) {
		fprintf(stderr, "ERROR: COULD NOT ALLOCATE MEMORY\n");
		fclose(indexFile);
		free(pages);
		return NULL;
	}

	indexedPages->pages = pages; 

	listStart = malloc(sizeof(listNode));

	if (listStart == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory\n");
		fclose(indexFile);
		free(pages);
		free(indexedPages);
		return NULL;
	}

	listStart->link = NULL;

	i = 0;
	while ((fscanf(indexFile, "%1000s %d",url,&maxHops) == 2) && i<maxPages) {
		hopNum = 0;
		while (1) { 
			if (i == 0) {
				strncpy(listStart->addr,url,MAX_ADDR_LENGTH);
				pages[i] = buildPage(url);
				
				if (pages[i] == NULL) {
					destroyList(listStart);
					fclose(indexFile);
					free(pages);
					destroyPageIndex(indexedPages);
					return NULL;
				}
				i++; 
			}
			else {
				if (!contains(listStart,url)) {
					insertBack(listStart,url);
					pages[i] = buildPage(url);

					if (pages[i] == NULL) {
						destroyList(listStart);
						fclose(indexFile);
						free(pages);
						destroyPageIndex(indexedPages);
						return NULL;
					}
					i++;
				}
			}
			hopNum++;

			if (hopNum<=maxHops && i<maxPages) {
				char nextURL[MAX_ADDR_LENGTH];

				if (getLink(url,nextURL,MAX_ADDR_LENGTH)) {
					strncpy(url,nextURL,MAX_ADDR_LENGTH);
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	indexedPages->numPages = i;
	fclose(indexFile);

	destroyList(listStart);

	return indexedPages;
}

/* Builds a page with information from the url */
page* buildPage(char* url) {
	page* thePage;

	int numWords = 0; 
	thePage = malloc(sizeof(page)); 

	if (thePage == NULL) {
		fprintf(stderr,"ERROR: COULD NOT ALLOCATE MEMORY");
		return NULL;
	}
	thePage->trie = indexPage(url, &numWords);
	thePage->totalTerms = numWords;

	strncpy(thePage->url,url,MAX_ADDR_LENGTH);
	return thePage;
}


// Indexes page & returns head of Trie
tnode* indexPage(const char* url,int* numWords) {
	char wordBuffer[MAX_WORDSIZE];
	int bIndex = 0;
	int wbIndex = 0;
	char buffer[MAX_BUFFERSIZE];
	tnode *head;

	fprintf(stdout,"%s\n",url);

	getText(url,buffer,MAX_BUFFERSIZE);

	head = initializeNewNode('\0');

	if (head == NULL) {
		printf("Unable to create head node of page: %s",url);
		return NULL;
	}

	while (buffer[bIndex] != '\0') {

		if (buffer[bIndex] >= 'a' && buffer[bIndex] <= 'z') {
			wordBuffer[wbIndex] = buffer[bIndex]; 
			wbIndex++;
		}
		else if (buffer[bIndex] >= 'A' && buffer[bIndex] <= 'Z') {
			wordBuffer[wbIndex] = tolower(buffer[bIndex]);
			wbIndex++;
		}
		else {
			if (wbIndex > 0) {
				wordBuffer[wbIndex] = '\0';
				printf("\t%s\n",wordBuffer);

				if (addWordOccurrence(wordBuffer,strlen(wordBuffer),head)) {
					printf("%s could not be added",wordBuffer);
				}

				*numWords++;
				wbIndex = 0;
			}
		}

		bIndex++;
	}

	return head;
}

// Adds word to Trie
int addWordOccurrence(const char* word, const int wordLength, tnode* node) {
	if (wordLength == 0) {
		node->frequency = node->frequency + 1;
		return 0;
	}
	else {
		int linkPos = word[0] - 'a';

		if (node->links[linkPos] == NULL) {
			tnode* newNode = initializeNewNode(word[0]);

			if (newNode == NULL) {
				printf("Node could not be allocated");
				return -1;
			}

			node->links[linkPos] = newNode;
			return addWordOccurrence(&word[1],wordLength-1,newNode);
		}
		else {			
			return addWordOccurrence(&word[1],wordLength-1,node->links[linkPos]); 
		}
	}

}

// Destroys Trie
void destroyTree(tnode* root) {
	int lIndex;
	for (lIndex=0;lIndex<ALPHA_LENGTH;lIndex++) {
		if (root->links[lIndex] != NULL) {
			destroyTree(root->links[lIndex]);
		}
	}

	free(root);
}

// Initializes new Node
tnode* initializeNewNode(char value) {
	int i;
	tnode* newNode = malloc(sizeof(tnode));

	if (newNode == NULL) {
		printf("New tnode could not be allocated");
		return NULL;
	}

	newNode -> frequency = 0;
	newNode -> value = value;

	for (i = 0; i < ALPHA_LENGTH; i++) {
		newNode->links[i] = NULL;
	}

	return newNode;
}

/* Frees memory from page index */
void destroyPageIndex(pageIndex* pages) {
	int i;

	for (i = 0; i < pages->numPages; i++) {
		page* thisPage = pages->pages[i];

		if (thisPage == NULL) {
			fprintf(stderr,"ERROR: Page does not exist\n");
			return;
		}

		freePage(thisPage);
	}

	free(pages->pages);
	free(pages);
}

/* Frees page */
void freePage(page* thisPage) {
	destroyTree(thisPage->trie);
	free(thisPage);
}

int getText(const char* srcAddr, char* buffer, const int bufSize) {
	FILE *pipe;
	int bytesRead;

	snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

	pipe = popen(buffer, "r");

	if (pipe == NULL) {
		fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
				buffer);
		return 0;
	}

	bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
	buffer[bytesRead] = '\0';

	pclose(pipe);

	return bytesRead;
}
