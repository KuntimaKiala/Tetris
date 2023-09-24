#include <iostream>
#include <thread>
#include <ncurses.h>
#include <chrono>
#include <random>
#include <vector>
#define N_COLOMNS 4

using std::cout ;
using std::endl ;
using std::string ;
using std::vector ;
using namespace std::chrono_literals ;


std::wstring tetromino[7] ;
int nFieldWidth = 12 ;
int nFieldHeight =18 ;

int nScreenWidth = 120;
int nScreenHeight = 30;

// the board where we play the game
unsigned char * pField{nullptr} ;

// base on the rotation of tetromino returns the index of particular position {of pixel}
int Rotate(int px, int py, int r, int w=N_COLOMNS){
    // r is 1, or, 2, or 3, or 4, w = 4
    switch (r%w)
    {
    case 0: return py*w+px;  //0 degrees rotation
    case 1: return 12 + py -px*w ; //90 degrees rotation
    case 2: return 15 -py*4-px;  //180 degrees rotation
    case 3: return 3 -py +px*w ; //270 degress rotation
    default : return 0 ;
    }

   
}

bool DoesPieceFit(int nTetronimo, int nRotation, int nPosX, int nPosY, int w =N_COLOMNS){
    // w= 4
    for(int px{0}; px < w; px ++){
        for (int py{0}; py < w; py++)
        {
           //get index into piece 
           int pi{Rotate(px, py, nRotation, w)} ;
           // get index into field 
           int fi{(nPosY+py)*nFieldWidth +(nPosX +px)} ; // translation of the piece to the top

           // boundaries checks |to avoid touching memories that don't belong to us
           if(nPosX+px>=0 && nPosX+px < nFieldWidth){ //check that X translation is visible 
                if(nPosY+py>=0 && nPosY+py < nFieldHeight){ //check that Y translation is visible

                    //collision detection 
                    if(tetromino[nTetronimo][pi] == L'X' && pField[fi]!=0) {// remeber that :  pField[index] = (x==0 || x==nFieldWidth -1|| y == nFieldHeight-1) ? 9 : 0;
                        //tetromino[nTetronimo][pi] can only be be . or X
                        // pField[fi] can only be 0 or 9
                        return false ; // fail on first hit
                    }

                }

           }
        }


    }

    return true ;
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
    nodelay(stdscr, TRUE); // enable special keys [UP, Down, Right, Left, F1 etc... ]
    timeout(100); // Set the timeout for non-blocking getch()


    // Get the window size
    getmaxyx(stdscr, nScreenHeight, nScreenWidth);
    // Create a window for the game
    WINDOW *gameWin = newwin(nScreenHeight, nScreenWidth, 0, 0);
    box(gameWin, 0, 0);

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

    
    pField = new unsigned char[nFieldWidth*nFieldHeight]; // create the board 
    for(int x{0}; x < nFieldWidth; x++){ // the boundary on width
        for(int y{0}; y <nFieldHeight;y++){ // the boundary on height
            // remenber cuda ?
            int index = y*nFieldWidth + x ;
            //filling the board 
            // 9 in the boundaries  : when x touches width boundaries(0 and nFieldWidth-1) or y touches height -1 
            // 0 in the rest : playable zone 
            pField[index] = (x==0 || x==nFieldWidth -1|| y == nFieldHeight-1) ? 9 : 0;
        }

    }

    bool bGameState{false};

    


    std::random_device rd ;
    std::seed_seq ss{rd(), rd(), rd(), rd()} ;
    std::mt19937 mt{ss} ;

    std::uniform_int_distribution<int> randPiece(0, 6);

    int nCurrentPiece{randPiece(mt)} ; // picking the current piece randomly
    int nCurrenRotation{0} ;
    int nCurrentX{nFieldWidth /2} ;
    int nCurrentY{0}; 
    int pKey{0};
    bool bRotateHold{false} ; 
    int nSpeed{30} ; //game dificulty 
    int nSpeedCounter{0}; // game tick 
    bool bForceDown{0} ;
    int score{0};
    int nPieceCount{0};
    vector<int> vLines ;

    while (!bGameState)
    {   
        mvwprintw(gameWin, 7, 15, "SCORE: %d", score);
        pKey = 0 ;



        // GAME FPS 
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed) ;
        std::this_thread::sleep_for(100ms);
        // USER INPUT 
        //prints to the ncurses window                                               
        printw("");
        //to read a character from the keyboard in a terminal-based user interface (TUI)                                                                           
        int ch = getch() ;
        if ((ch==27)||(ch=='q')){
            // Check for the Escape key (ASCII value 27)
            // Handle the Escape key press
            break ;
        } 
        if(ch != ERR){

            switch (ch)
            {
            case 114 :
                pKey = 1 ;
                break;
            case 82 :
                pKey = 1 ;
                break ;
            case KEY_DOWN :
                pKey =2;
                break;
            case KEY_RIGHT :
                pKey=3;
                break ;
            case KEY_LEFT :
                pKey=4;
                break ;

            }    

        }

        // GAME LOGIC 
        
       
        //LEFT KEY
        if (pKey==4){ 
            // check first if the piece fit 
            if(DoesPieceFit(nCurrentPiece, nCurrenRotation, nCurrentX-1, nCurrentY)){
                // if the piece fits update the X position
                nCurrentX -= 1  ;
            }
        }
        //RIGHT KEY
        if (pKey ==3){
            // check first if the piece fit 
            if(DoesPieceFit(nCurrentPiece, nCurrenRotation, nCurrentX+1, nCurrentY)){
                // if the piece fits update the X position
                nCurrentX += 1 ;
            }


        }//DOWN KEY
        if (pKey == 2)
        {
           // check first if the piece fit 
            if(DoesPieceFit(nCurrentPiece, nCurrenRotation, nCurrentX, nCurrentY+1)){
                // if the piece fits update the X position
                nCurrentY += 1 ;
            }

        }
        bRotateHold = (pKey==1) ? true : false ;
        nCurrenRotation += (pKey==1 && bRotateHold && DoesPieceFit(nCurrentPiece, nCurrenRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0 ;
        
        if(bForceDown){
            //check first if the piece fit 
            if(DoesPieceFit(nCurrentPiece, nCurrenRotation, nCurrentX, nCurrentY +1)){
                nCurrentY++; // keep sending it down
            }
            else{
                //in case the piece did not fit 

                // 1-Lock the current piece in the field, it becames part of the board  
                for(int px{0}; px < N_COLOMNS; px++){ // the boundary on width
                    for(int py{0}; py <N_COLOMNS;py++){ // the boundary on height
                    // check if the element in the piece is X
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrenRotation)] == L'X'){
                            pField[(nCurrentY +py)*nFieldWidth +(nCurrentX + px)] = nCurrentPiece + 1 ;
                        }
                    }
                
                }
                nPieceCount++ ;
                if( nPieceCount%10 == 0){
                    if(nSpeed >=10) nSpeed--;
                }
                //2-Check if we got any lines | place to play 
                for(int py{0}; py < N_COLOMNS; py++){
                    if (nCurrentY + py < nFieldHeight - 1){
                        bool bLine = true ;
                        for (int px{1}; px < nFieldWidth -1; px++){
                            // check the entire line if there is still empty space | remember 0 maps to a space : L" ABCDEFG=#"[pField[y*nFieldWidth + x]]
                            bLine &=(pField[(nCurrentY +py)*nFieldWidth +px]) != 0;
                        }
                        if(bLine){
                            //If the Line is not empty rwe have to remove, first set  it to (=) |   remember 8 maps to a = : L" ABCDEFG=#"[pField[y*nFieldWidth + x]]
                             for (int px{1}; px < nFieldWidth-1; px++){
                                pField[(nCurrentY +py)*nFieldWidth +   px] = 8 ;
                             }
                             vLines.push_back(nCurrentY + py); // the number of the raw

                        }
                    }                
                } 
                
                if(!vLines.empty()) {
                    score += (1 << vLines.size()*100); 
                // 0000001: 0000001 << 2 --> 0000100 -->
                }
                // choose next piece randomly and set all is attributtes to default positions
                nCurrentPiece = randPiece(mt) ;
                nCurrentX = nFieldWidth / 2 ;
                nCurrentY = 0 ;
                nCurrenRotation = 0 ;
              

                // 4-if piece does not fit then game over 
                bGameState = !DoesPieceFit(nCurrentPiece, nCurrenRotation, nCurrentX, nCurrentY);
            }
            nSpeedCounter = 0 ;
        }

        // Draw Field
        for(int x{0}; x < nFieldWidth; x++){ // the boundary on width
            for(int y{0}; y <nFieldHeight;y++){ // the boundary on height 
            //L" ABCDEFG=#"[pField[y*nFieldWidth + x]] is mapping based on index, you get a letter 
            // if pField[y*nFieldWidth + x] = 0 | [0 as unsigned char ] 
            // then will have L" ABCDEFG=#"[0] zero here correspond to A
            // if pField[y*nFieldWidth + x] = 6 | [6 as unsigned char ] 
            // then will have L" ABCDEFG=#"[0] zero here correspond to G
             mvwaddch(gameWin, (y+7), (x+1), L" ABCDEFG=#"[pField[y*nFieldWidth + x]]);
            }
        }
        
        // here where we remove lines when the last row does not have empty spaces
        if(!vLines.empty()){
            score +=25 ;
            //mvwaddch(gameWin, nScreenHeight , nScreenWidth , '=' ); 
            std::this_thread::sleep_for(400ms);

            for (auto &v : vLines)
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					pField[px] = 0;
				}

			vLines.clear();

        }
        // Draw Current Piece | N_COLOMNS = 4 | we draw char values eg( the 65 will turn into 'A')
        for(int px{0}; px < N_COLOMNS; px++){ // the boundary on width
            for(int py{0}; py <N_COLOMNS;py++){ // the boundary on height
                // check if the element in the piece is X
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrenRotation)] == L'X'){
                     // 65 = A in ASCII 
                     //remeber this L" ABCDEFG=#"[pField[y*nFieldWidth + x]] ?
                     mvwaddch(gameWin, (nCurrentY+py+7), (nCurrentX+px+1), nCurrentPiece + 65); 
                     // and nCurrentPiece range from 0 to 6 : nCurrentPiece + 65 will be  A or B or C or D or E or F or G
                }
            
            }
        }
        //refresh
       
        wrefresh(gameWin);

       
    }



    //clean up and restore the terminal to its normal mode.                      
    endwin() ;
    cout << "GAME OVER" << endl ;
  
    return 0;
}
