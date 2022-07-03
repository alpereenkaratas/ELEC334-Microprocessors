/*Alperen Karatas - 1801022022 - ELEC 334 HW#1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 17    // N is size parameter of maze.

int enq_i[100];     //(For autonomously work)A queue, holding row parameter of added coordinate.
int enq_j[100];     //(For autonomously work)A queue, holding column parameter of added coordinate.

int deq_i[100];     //(For autonomously work)A queue, holding row parameter of extracted coordinate.
int deq_j[100];     //(For autonomously work)A queue, holding column parameter of extracted coordinate.

int rear_for_auto=-1;    //(This is for the queue data structure for autonomously mode.)
                        //Initial value of the rear is -1,because at the beginning queue is empty.
                        //Rear value will increase when the element is added to the queue.


int rear=-1;    //(This is for the queue data structure)
                //Initial value of the rear is -1,because at the beginning queue is empty.
                //Rear value will increase when the element is added to the queue.

int front=-1;   //(This one too is for the queue data structure)
                //Initial value of the front is -1,because at the beginning queue is empty.
                //Front value will be 0 when the first element is added to the queue.

char locations[100][100];     //Travelled locations queue

char maze[N][N] =      //Our maze.
    {
		 {'#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
         {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		 {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#'},
         {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
		 {'#', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#'},
         {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'},
		 {'#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
		 {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
         {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#'},
		 {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
		 {'#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#'},
         {'#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
		 {'#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#'},
         {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'},
		 {'#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#'},
		 {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
         {'#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#'}
	};

void printMaze(){   //Print maze.
    int a=16;
    for(int i=0;i<N;i++){
        if(a>=0 && a<10)
            printf("%d    ",a);
        else
            printf("%d   ",a);
        a--;
        for(int j=0;j<N;j++){
            printf("%c  ",maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int k=0;k<17;k++){
        if(k==0)
            printf("     %d  ",k);
        if(k>0&&k<10)
            printf("%d  ",k);
        if(k>=10)
            printf("%d ",k);
    }
}

char solution_path[N][N];    //(For autonomously work)We use this array to draw the solution path.

void print_solution_path(){  //Print the solution path.

    for(int i=0;i<N;i++){      //Filling blanks in the solution path with '1'.
        for(int j=0;j<N;j++){
            if(solution_path[i][j]=='1')
                maze[i][j]='1';
        }
    }
    printf("Maze with solution path:\n\n"); //Shows the solution path of maze.
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%c  ",maze[i][j]);
        }
        printf("\n");
    }
}
/*For autonomously work)*/
int mark_path(i,j){  //In this function,we are marking the roads that we travelled by the right way.

    if(i==16 && j==9 && maze[i][j]==' '){              //Here,we write the destination point coordinates for i and j.
        solution_path[i][j]='1';    //Mark the route.
        enqueue_push(i,j);          //Enqueue the marked elements.
        return 1;
    }
    /*Making sure that we are stay on track(not outside of the maze),
    there is no block/wall on the road and roads the way we go is not visited.*/
    if(maze[i][j]==' ' && solution_path[i][j]=='#' && i>=0 && i<N && j>=0 && j<N){

        solution_path[i][j]='1'; //It means we can add this point of the way into our solution path.
        enqueue_push(i,j);

        if(mark_path(i+1,j)==1) // Move to downwards
            return 1;

        if(mark_path(i,j+1)==1) // Move to rightwards
            return 1;

        if(mark_path(i-1,j)==1) // Move to upwards
            return 1;

        if(mark_path(i,j-1)==1) // Move to leftwards
            return 1;

        solution_path[i][j]='#';   // If all the conditions fails,then we have to backtrack.
        dequeue_push(i,j);         //Dequeue the unmarked elements.
        }

    return 0; // If destination point is outside the maze or there is not a solution path.
}

void enqueue_push(int i,int j){     //(This one is for autonomously mode) Since we are working on 2-dimensional array,
                                    //two different queues were preferred for two separate parameters(i,j).
    rear_for_auto++;
    enq_i[rear_for_auto]=i;
    enq_j[rear_for_auto]=j;

}

void dequeue_push(int i,int j){     //(This one is for autonomously mode) The function used for the coordinate pairs to be removed from the queue.
                                    //This coordinate pairs assigned to two seperate arrays to be used in "print_moves" function.
    for(int k=0;k<rear_for_auto+1;k++){
        if(enq_i[k]==i && enq_j[k]==j){
            deq_i[k]=i;
            deq_j[k]=j;
        }
    }
}

void print_moves_auto(){                 //(This one is for autonomously mode)Function for showing the number of moves and followed path.
    int total_moves=0;
    for(int k=0;k<rear_for_auto+1;k++){
        if(enq_i[k]==deq_i[k] && enq_j[k]==deq_j[k])
            continue;
        printf("%d,%d\n",enq_i[k],enq_j[k]);
        total_moves++;
    }
    printf("\nTotal moves: %d\n",total_moves);
}

void logger(int check){
    if(check!=5){
        push_func("D8");push_func("C8");push_func("C7");push_func("B7");push_func("B6");
        push_func("B5");push_func("A5");push_func("A4");push_func("B4");push_func("B3");
        push_func("B2");push_func("A2");push_func("A1");push_func("B1");push_func("C1");
        push_func("D1");push_func("D2");push_func("E2");push_func("F2");push_func("F1");
        push_func("E1");
    }
    if(check==5)
        test_func();

    if(check!=5)
        print_moves(1);
}

void push_func(char str[]){     //In this function, we're doing enqueue operation
    if (front == -1)
      front = 0;
    rear++;
    strcpy(locations[rear],str);
}

void print_moves(int check){    //The task of this function, printing path and printing total moves.

    if(check==1){
        for(int i=0;i<rear+1;i++){
            if(i==rear)
                printf("%s",locations[i]);
            else
            printf("%s, ",locations[i]);
        }
    }
    if(check==5){
        for(int i=1;i<rear+1;i++){
            if(i==rear)
                printf("%s",locations[i]);
            else
                printf("%s, ",locations[i]);
        }
        rear--;
    }
    printf("\n\nTotal moves: %d\n\n",rear+1);
}

void test_func(){               //To test the queue operations.

    char str_test[10];
    printf("Enter the coordinate you want to go to in letters and numbers. Also you will see dequeue operation.(First in out first out)\n");

    fflush(stdin);
    gets(str_test);
    printf("Added -> %s\n",str_test);
    printf("Deleted -> %s\n",locations[0]);
    push_func(str_test);
    print_moves(5);

}

int main()
{
    int op;
    printf("Maze:\n\n");
    printMaze();                // Calling the maze.
    printf("\n\n");

    for(int i=0;i<N;i++){       //To draw the solution path, all elements must be '#' at the beginning.
        for(int j=0;j<N;j++){
            solution_path[i][j]='#';
        }
    }

    printf("\n\n");

    printf("!!!REMINDER!!!\nIn the autonomous operating mode of the car, any desired maze can be solved, the input and output coordinates of the maze can be changed in the code and a new solution can be found.\nGo to line 29 to change the maze, line 236 to change input coordinates, line 95 to change output coordinates.");

    printf("\n\nPress 1 to see the coordinates where the car is running autonomously.\n");
    printf("Press 2 to see the coordinates of the maze given in the assignment file.\n");

    scanf("%d",&op);
    printf("\n");
    if(op==1){
        if(mark_path(0,7)==1){          //If there is a solution path,print it in the maze and count moves.
            print_solution_path();
            print_moves_auto();
        }
        else
            printf("\nSolution path not found.");
    }
    else if(op==2){
        logger(1);              //1 number does not mean anything,just for true operation.You will see the reason down the lines.
        printf("Press 3 to run the test function. Press 4 to quit.\n");
        scanf("%d",&op);
        if(op==3){
            logger(5);         //5 number does not mean anything,just for checking the logger function to do not add another coordinates.
        }
        else if(op==4)
            return 0;
    }

    return 0;
}
