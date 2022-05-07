#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<bios.h>
#define LT 218
#define RT 191
#define LB 192
#define RB 217
#define TT 194
#define BT 193
#define HL 196
#define VL 179
#define LST 195
#define RST 180
#define CROSS 197
#define SPACE 32
#define BS 8
#define WIDTH 11
#define HEIGHT 1
#define MAXROWS 25
#define MAXCOLS 80
#define UP 18432
#define RIGHT 19712
#define LEFT 19200
#define DOWN 20480
#define ESC 283
#define ENTER 7181
#define LOW_F 8550
#define UPP_F 8518
void drawBox(int DIM);
void game_play(int DIM,int **mat);
void game();
void fillBox(int DIM,int**mat);
int swipe(int DIM,int**mat,int dir);
int game_check(int DIM,int**mat);
int main()//function entry point to the program
{

  clrscr();
  game();
  getch();
  return 0;
}
void game()
{
   int choice;
   int row;
   int col;
   int ind;
   int dim;
   int tot_mines;
   int ** mat=NULL;
   int val;
   while(1)
   {
	clrscr();
	printf("2048 GAME	\n");
	printf("1. 3 X 3\n");
	printf("2. 4 X 4\n");
	printf("3. 5 X 5\n");
	printf("4. 6 X 6\n");
	printf("5.EXIT\n");
	printf("Enter your choice :  ");
	scanf("%d",&choice);
	if(choice==5)
		break;
	if(choice<1||choice>5)
		continue;
	switch(choice)
	{
		case 1:
			dim=3;
			break;
		case 2: dim=4;
			break;
		case 3: dim=5;
			break;
		case 4: dim=6;
			break;

	}
	mat=(int**)calloc(dim,sizeof(int*));
	for(ind=0;ind<dim;ind+=1)
	{
		mat[ind]=(int*)calloc(dim,sizeof(int));
	}
/*	mat[0][2]=2;
	mat[0][1]=2;
	mat[3][2]=1024;
	mat[2][2]=1024;
	mat[1][2]=1024;
	mat[2][3]=4;
 */
	 drawBox(dim);
	 //fillBox(dim,mat);
	 game_play(dim,mat);
	getch();
   }
}
void game_play(int DIM,int **mat)
{
   int row;
   int col;
    int bcol;
    int brow;
    int key;
    int val;
    int move_flag=0;
    bcol= (MAXCOLS - ((DIM*WIDTH) + DIM +1 )) /2 ; //(given-used)/2
    brow = (MAXROWS - ((DIM*HEIGHT)+DIM+1)) /2;
    bcol+=(WIDTH/2)+1;
    brow+=1;
    randomize();
   while(1)
   {
     row=rand()%DIM;
     col=rand()%DIM;
     if(mat[row][col]==0)
     {
       if(rand()%2==0||rand()%3==0)
	mat[row][col]=2;
       else
	mat[row][col]=4;
       break;
     }
   }

   fillBox(DIM,mat);
   gotoxy(bcol+(col*(WIDTH+1)),brow+(row*(HEIGHT+1)));
   while(1)
    {
      while(bioskey(1)==0);
      key=bioskey(0);
      if(key==ESC)
	break;
      move_flag=0;

      switch(key)
      {
	case UP:
			move_flag=swipe(DIM,mat,1);
			fillBox(DIM,mat);
			break;
	case RIGHT:
			move_flag=swipe(DIM,mat,2);
			fillBox(DIM,mat);
			break;
	case DOWN:
			move_flag=swipe(DIM,mat,3);
			fillBox(DIM,mat);
			break;
	case LEFT:
			move_flag=swipe(DIM,mat,4);
			fillBox(DIM,mat);
			break;
     } //end of switch
     //getch();
     //fflush(stdin);
     if(move_flag==1)
     {
	 while(1)
	{
		row=rand()%DIM;
		col=rand()%DIM;
		if(mat[row][col]==0)
		{
		       if(rand()%2==0||rand()%3==0)
				mat[row][col]=2;
		       else
				mat[row][col]=4;
			break;
		}
	}//end of while
	   fillBox(DIM,mat);
	   gotoxy(bcol+(col*(WIDTH+1)),brow+(row*(HEIGHT+1)));
     }
     val=game_check(DIM,mat);
     if(val==0)
     {
      gotoxy(37,21);
      printf("Game Over");
      break;
     }
     if(val==2)
     {
      gotoxy(37,21);
      printf("You Won");
      break;
     }
   }//end of while
}
int game_check(int DIM,int**mat)
{
 int row;
 int col;
 for(row=0;row<DIM;row+=1)
  for(col=0;col<DIM;col+=1)
   if(mat[row][col]==2048)
	return 2;
 for(row=0;row<DIM;row+=1)
 {
  for(col=0;col<DIM;col+=1)
  {
   if(col==0&&mat[row][col]==0)
	return 1;
   if(col>0)
   {
    if(mat[row][col]==0||(mat[row][col]==mat[row][col-1]))
     return 1;
   }
  }
 }
  for(col=0;col<DIM;col+=1)
 {
  for(row=0;row<DIM;row+=1)
  {
   if(row==0&&mat[row][col]==0)
	return 1;
   if(row>0)
   {
    if(mat[row][col]==0||(mat[row][col]==mat[row-1][col]))
     return 1;
   }
  }
 }
 return 0;
}
int swipe(int DIM,int**mat,int dir)
{
 int * pair=NULL;
 int col;
 int holder;
 int traverser;
 int row;
 int move_flag=0;
 if(dir==1)//UP
 {
   for(col=0;col<DIM;col+=1)
   {
    pair=(int*)calloc(DIM,sizeof(int));

   holder=traverser=0;
   while(traverser<DIM)
   {
    if(mat[traverser][col]!=0)
    {
      if(holder==0)
      {
	mat[holder][col]=mat[traverser][col];
	if(holder!=traverser)
		move_flag=1;
	holder++;

      }
      else
      {
	if(mat[holder-1][col]==mat[traverser][col]&&pair[holder-1]==0)
	{
	 mat[holder-1][col]+=mat[traverser][col];
	 pair[holder-1]=1;
	 move_flag=1;
	}
	else
	{
	  mat[holder][col]=mat[traverser][col];
	  if(holder!=traverser)
	   move_flag=1;
	  holder++;

	}
      }
    }
     traverser++;
   }
   while(holder<DIM)
   {
    mat[holder][col]=0;
    holder+=1;
   }
      free(pair);
  }//end of col
 }//end of dir
 else if(dir==2)//RIGHT
 {
   for(row=0;row<DIM;row+=1)
   {
    pair=(int*)calloc(DIM,sizeof(int));
   holder=traverser=DIM-1;
   while(traverser>=0)
   {
    if(mat[row][traverser]!=0)
    {
      if(holder==(DIM-1))
      {
	mat[row][holder]=mat[row][traverser];
	if(holder!=traverser)
		move_flag=1;
	holder--;
      }
      else
      {
	if(mat[row][holder+1]==mat[row][traverser]&&pair[holder+1]==0)
	{
	 mat[row][holder+1]+=mat[row][traverser];
	 pair[holder+1]=1;
	 move_flag=1;
	}
	else
	{
	  mat[row][holder]=mat[row][traverser];
	  if(holder!=traverser)
	   move_flag=1;
	  holder--;

	}
      }
    }
     traverser--;
   }
   while(holder>=0)
   {
    mat[row][holder]=0;
    holder-=1;
   }
      free(pair);
  }//end of row
 }//end of dir
 else if(dir==3)//DOWN
 {
   for(col=0;col<DIM;col+=1)
   {
    pair=(int*)calloc(DIM,sizeof(int));
   holder=traverser=DIM-1;
   while(traverser>=0)
   {
    if(mat[traverser][col]!=0)
    {
      if(holder==(DIM-1))
      {
	mat[holder][col]=mat[traverser][col];
	if(holder!=traverser)
		move_flag=1;
	holder--;

      }
      else
      {
	if(mat[holder+1][col]==mat[traverser][col]&&pair[holder+1]==0)
	{
	 mat[holder+1][col]+=mat[traverser][col];
	 move_flag=1;
	 pair[holder+1]=1;
	}
	else
	{
	  mat[holder][col]=mat[traverser][col];
	  if(holder!=traverser)
	   move_flag=1;
	  holder--;

	}
      }
    }
     traverser--;
   }
   while(holder>=0)
   {
    mat[holder][col]=0;
    holder-=1;
   }
    free(pair);
  }//end of col
 }//end of dir
  else if(dir==4)//LEFT
 {
   for(row=0;row<DIM;row+=1)
   {
    pair=(int*)calloc(DIM,sizeof(int));
   holder=traverser=0;
   while(traverser<DIM)
   {
    if(mat[row][traverser]!=0)
    {
      if(holder==0)
      {
	mat[row][holder]=mat[row][traverser];
	if(holder!=traverser)
		move_flag=1;
	holder++;

      }
      else
      {
	if(mat[row][holder-1]==mat[row][traverser]&&pair[holder-1]==0)
	{
	 mat[row][holder-1]+=mat[row][traverser];
	 move_flag=1;
	 pair[holder-1]=1;
	}
	else
	{
	  mat[row][holder]=mat[row][traverser];
	  if(holder!=traverser)
	   move_flag=1;
	  holder++;

	}
      }
    }
     traverser++;
   }
   while(holder<DIM)
   {
    mat[row][holder]=0;
    holder+=1;
   }
   free(pair);
  }//end of row
 }//end of dir
 return move_flag;
}
void fillBox(int DIM,int**mat)
{
   int row,col;
    int bcol;
    int brow;
    bcol= (MAXCOLS - ((DIM*WIDTH) + DIM +1 )) /2 ; //(given-used)/2
    brow = (MAXROWS - ((DIM*HEIGHT)+DIM+1)) /2;
    bcol+=(WIDTH/2)+1;
    brow+=1;
   for(row=0;row<DIM;row+=1)
   {
	for(col=0;col<DIM;col+=1)
	{
	  gotoxy(bcol+(col*(WIDTH+1)),brow+(row*(HEIGHT+1)));
	  if(mat[row][col]!=0)
	  {
		  printf("%4d",mat[row][col]);
	  }
	  else
	  {
		  printf("    ");
	  }

	}
   }

}
void drawBox(int DIM)
{
    int brow,bcol,ctr1,ctr2,ctr3;
    clrscr();
    bcol= (MAXCOLS - ((DIM*WIDTH) + DIM +1 )) /2 ; //(given-used)/2
    brow = (MAXROWS - ((DIM*HEIGHT)+DIM+1)) /2;
   // brow+=2;
    gotoxy(34,brow-3);
    printf("2048 GAME ");

    gotoxy(bcol,brow);
	printf("%c",LT);
	for(ctr2=1;ctr2<=DIM;ctr2++)
	{
	    for(ctr1=1 ;ctr1<=WIDTH ; ctr1++)
		printf("%c",HL);
	    printf("%c",TT);
	}
	printf("%c%c",BS,RT);
	//end of first line
    for(ctr3=1;ctr3<=DIM;ctr3++)
    {
	gotoxy(bcol,++brow);
	for(ctr2=1;ctr2<=DIM;ctr2++)
	{
	   printf("%c",VL);
	   for(ctr1=1 ;ctr1<=WIDTH ; ctr1++)
		printf("%c",SPACE);
	}
	printf("%c",VL);
	gotoxy(bcol,++brow);
	printf("%c",LST);
	for(ctr2=1;ctr2<=DIM;ctr2++)
	{
	  for(ctr1=1 ;ctr1<=WIDTH ; ctr1++)
		printf("%c",HL);
	    printf("%c",CROSS);
	}
	printf("%c%c",BS,RST);
    }//end of middle part
       gotoxy(bcol,brow);
	printf("%c",LB);
	for(ctr2=1;ctr2<=DIM;ctr2++)
	{
	    for(ctr1=1 ;ctr1<=WIDTH ; ctr1++)
		printf("%c",HL);
	    printf("%c",BT);
	}
	printf("%c%c",BS,RB);
	//end of last line
     gotoxy(20,brow+2);
    printf("NOTE : Arrow keys - move , & ESC - quit ");
}
