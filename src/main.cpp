#include <iostream>
#include <thread>
#include <ncurses.h>
#include <unistd.h> // ?
#

using std::cout ;
using std::endl ;
using std::string ;
#define WH LINES
#define WW COLS


std::wstring tetromino[7] ;
int nFieldWidth = 12 ;
int nFildHeight =18 ;

// base on the rotation of tetromino returns the index of particular position {of pixel}
int Rotate(int px, int py, int r, int w=4){
    // r is 1, or, 2, or 3, or 4
    switch (r%4)
    {
    case 0: return py*w+px;  //0 degrees rotation
    case 1: return 12 + py -px*w ; //90 degrees rotation
    case 2: return 15 -py*4-px;  //180 degrees rotation
    case 3: return 3 -py +px*w ; //270 degress rotation
    default : return 0 ;
    }

   
}


int main()
{
    // initializes the ncurses library and sets up the terminal to be used with ncurses.
    initscr();// initilize
    //turn off character echoing
    //remove curser//sets the terminal in raw mode                                                                                 
    //raw();
    //turn off character echoing
    noecho() ; //(?)
    // disable the cursor
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    timeout(100); // Set the timeout for non-blocking getch()


    //creating the assets 
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");


    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");


    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"...."); 
    
    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");
  
    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"...."); 


    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X."); 


    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");
    tetromino[6].append(L"...."); 

    
    //prints to the ncurses window                                               
    printw("");
    //to read a character from the keyboard in a terminal-based user interface (TUI)                                                                           
    int ch = getch() ;


    //clean up and restore the terminal to its normal mode.                      
    endwin() ;

  
    return 0;
}
