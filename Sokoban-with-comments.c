#include <ncurses.h>
#define D rand()% // Self explanitory
#define O M[y+Y][x+X] // Get value at tile [y+Y][x+X]
#define U M[y][x] // Get value at tile [y][x]
#define B(v,h)for(int v=0;v<h;v++) // Shorthand for loop, int is included to creat local variables
#define T M[y+(2*Y)][x+(2*X)] // Get value at tile [y+(2*Y)][x+(2*X)]

main(){
    int M[20][40], // Holds the map and all its values
        Y, // Used against y for different directions
        X, // Used against x for different directions
        s=time(0), // Store the current seed (used to reset the level)
        i, // Iterator variable
        j; // Iterator variable

    R: // Reset everything (we jump to here when the user presses 'r')
        srand(s); // Init rand()
    S: // Reset level gen (we jump to here if a level doesn't turn out semi nice). Technically it could be after "initscr();", but gcc complained
        initscr();  // Init ncurses

    // Theses variables are defined here because they need to be reset to gen a new level
    int r=0, // Used during the first part of level gen to store next tile value. Then used to hold the movement direction of the player
        d=0, // Used as a flag to note if the user is over a marker
        t=0, // Used to check if the second part of level gen worked ok. Then used to check if the user has won
        // User's current location (also used by the second part of level gen). Starts in the middle of the screen.
        y=10, 
        x=20;

    // Loop through every tile...
    B(y,20)
        B(x,40)
            U=35; // ...and make them walls '#'

    // Augmented drunken walk algorithm runs 1500 times
    B(i,1500){
        U=D 5?8:9; // Set current tile to 8 (air tile which the generator hasn gone over) 4/5 times and 9 (bolder which the generator hasn't moved) the rest of the time
        r=D 5?r:D 4; // 1/5 times pick a new random direction, otherwise keep the old direction
        r==0&y>1?y--:r==1&x>1?x--:r==2&y<18?y++:r==3&x<38?x++:0; // Move the current tile to the direction noted in r if it doesn't go out of bounds
    }
    
    // Main Loop
    B(i,2001){
        if(i<2000){ // For the first 2000 iterations, work on level gen
            r=D 4; // Pick a random direction
            t++; // Increment t to represent generator moves, we will decrement it later if the move is invalid
            if(i==1999){ // The last iteration of the level gen
                if(t<1500){ // Test to make sure the generator moved at least 1500 times out of the 2000
                    goto S; // Jump to reset
                }else{ // Level is pretty good so...
                    // ...loop through every tile...
                    B(y,20)
                        B(x,40)
                            U=(U==8|U==9?35:U); // ... and if the tile is an 8 or a 9 set it to 35 '#' wall tile, otherwise leave it as is
                }
            }
        }else{
            t=1; // t will stay at 1 if all of the boulders are in the right place 
            // Loop through every tile
            B(y,20)
                B(x,40){
                    mvprintw(y,x,"%c",U); // Print the tile to the screen
                    U==79?t--:0; // If the current tile is 79 'O' (aka tile that is not in the right place) decrement t
                }
                t>0?j=5:0; // If t is greater than 0 (play has won) set j = 5
                r=getch(); // Get user input
        }
        // A lot of the following lines will be used for both generating and playing the level

        r==0|r=='w'?Y=-1,X=0:r==1|r==97?Y=0,X=-1:r==2|r=='s'?Y=1,X=0:r==3|r=='d'?Y=0,X=1:0; // Set X,Y offsets based on r (has either wasd keys from user input or 0-3 for level gen)
        if(r=='r'){ // If the user hit the reset key...
            goto R; // ...reset the level
        }
        U=d?46:32; // Assuming we will move, set the current location to 32:' ' or 46:'.' depending on d flag
        if(O!=35){ // If the tile we want to move to is not a wall
            d==1?d=2:0; // Set d to 2 if it is 1
            y+=Y;x+=X; // Move main location by offsets
            if(i<2000){ // If we are in level gen
                if(U==79|U==48|U==9){ // If the current tile is a bolder (79:'O') a bolder in the right place (48:'0') or an unmoved boulder (9) (aka we are "pulling" something)
                    y+=(-2*Y);x+=(-2*X); // Set generator location to behind where they were at the start of the turn 
                    if(U==35|U==79|U==48|U==9){ // If this new location is invalid (boulder, wall, etc)
                        y+=Y;x+=X; // Reset generator location to where it was at the start of the turn
                        t--; // Decrement to to indicate an invalid move
                    }else{ // The move is valid
                        // O here corresponds to the original generator location at the start of the turn aka where we want the boulder to go
                        O=(d==2)?48:79; // "Pull" the boulder, boulder in correct position 48:'0' if generator was standing on an end point, normal boulder 79:'O' otherwise
                        T=(T==48|T==9)?46:32; // Clear the boulders old location based on if the location is an endpoint
                    }
                }
            }else{ // Else it is a player turn
                if(U==79|U==48){ // If the current tile (place player wants to move to) is a boulder 
                    if(O==46){ // If the tile we want to push the boulder into is 46:'.' an endpoint
                        O=48; // Make that space into a boulder in the correct place 48:'0'
                        U==48?d=1:0; // If the space we moved off of is an endpoint denote that by setting d (b/c player will stand on this space)
                    }else if(O!=35&O!=79&O!=48){ // Else if the space we want to push the boulder to is not a wall or boulder
                        O=79; // Set the new boulder location to normal boulder 79:'O'
                        U==48?d=1:0; // If the space we moved off of is an endpoint denote that by setting d (b/c player will stand on this space)
                    }else{ // We can't move the boulder to the desired space
                        y-=Y;x-=X; // Revert player location
                        d==2?d--:0; // Decrement d back to 1 because we didn't actually do anything
                    }
                }
            }
            U==46?d=1:d==2?d=0:0; // Reset d to correct value
        }else{ // The tile is a wall and the move is invalid so... 
            t--; // ...decrement t to indicate bad move
        }
        i==2000&j!=5?i--:0; // If this is the last increment of the original for loop and the player hasn't won, decrement i so we stay in the loop
        U=64; // Set current location to player
    }
    mvprintw(21,0,"WIN:)"); // Print win message
    getch(); // Wait for user input so the see the win message
    endwin(); // Exit ncurses
}