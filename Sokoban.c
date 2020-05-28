#include <ncurses.h>
#define D rand()%
#define O M[y+Y][x+X]
#define U M[y][x]
#define B(v,h)for(int v=0;v<h;v++)
#define T M[y+(2*Y)][x+(2*X)]
main(){int M[20][40],Y,X,s=time(0),i,j;R:srand(s);S:initscr();int r=0,d=0,t=0,y=10,x=20;B(y,20)B(x,40)U=35;B(i,1500){U=D 5?8:9;r=D 5?r:D 4;r==0&y>1?y--:r==1&x>1?x--:r==2&y<18?y++:r==3&x<38?x++:0;}B(i,2001){if(i<2000){r=D 4;t++;if(i==1999){if(t<1500){goto S;}else{B(y,20)B(x,40)U=(U==8|U==9?35:U);}}}else{t=1;B(y,20)B(x,40){mvprintw(y,x,"%c",U);U==79?t--:0;}t>0?j=5:0;r=getch();}r==0|r=='w'?Y=-1,X=0:r==1|r==97?Y=0,X=-1:r==2|r=='s'?Y=1,X=0:r==3|r=='d'?Y=0,X=1:0;if(r=='r'){goto R;}U=d?46:32;if(O!=35){d==1?d=2:0;y+=Y;x+=X;if(i<2000){if(U==79|U==48|U==9){y+=(-2*Y);x+=(-2*X);if(U==35|U==79|U==48|U==9){y+=Y;x+=X;t--;}else{O=(d==2)?48:79;T=(T==48|T==9)?46:32;}}}else{if(U==79|U==48){if(O==46){O=48;U==48?d=1:0;}else if(O!=35&O!=79&O!=48){O=79;U==48?d=1:0;}else{y-=Y;x-=X;d==2?d--:0;}}}U==46?d=1:d==2?d=0:0;}else{t--;}i==2000&j!=5?i--:0;U=64;}mvprintw(21,0,"WIN:)");getch();endwin();}