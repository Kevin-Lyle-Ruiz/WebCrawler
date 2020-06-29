# WebCrawler

This program was a class project where I made a web crawler using Python and C language. Python is used to scrape the words off a web page and then I use C to put the words into a Trie data structure. 

## Prerequisites

1. MingGW-64 Compiler installed. (http://mingw-w64.org/doku.php)

### Getting Started

1. Download repository to your preferred location and in the Windows command line, run ```mingw32-make``` in the directory that the project is located.

2. In the Windows command line, input the command listed below with three arguments in total: 

    `./webSearch ".txt file that contains list of web pages" "Max pages to index"
    
    `EX: ./webSearch "test.txt" 10`
    
    
    
NOTE: If you want to use your own website for testing purposes, create a .txt file and input the following template into the .txt file:

      "https://YourPreferredWebsiteHere.com" "Max pages to index"

      EX: "https://cnn.com" 5

When running the Webcrawler, you'll then replace "test.txt" with the .txt file that you created.

## Built With

* C 
* Python
