# WebCrawler

This is a web crawler created mainly with C, using Python to get links and text within a web page. The crawler retrieves words from the webpage, puts it into a Trie data structure, and prints it as a list in the console. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

1. Your preferred Command Line / Terminal

### Installing

1. Download files to your preferred location and run "mingw32-make" if using Windows within the command line in the directory that the project is located.

2. Then, in the Command Line, input this command with the following template with three arguments in total: 

    `./webSearch "File of Pages" "Max Pages to Index"`
    
    `EX: ./webSearch "test.txt" 10`
    
    
    
NOTE: If you want to use your own website for testing purposes, create a .txt file and use the following template:

        "https://YourPreferredWebsiteHere" "Number of pages the you want to index"

        EX: "https://cnn.com" 5

When running the Webcrawler, you'll then replace "test.txt" with the .txt file that you created.

## Built With

* C 
* Python
