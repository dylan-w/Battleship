/*Battleship Game*/
/*Dylan Wesenberg*/
/*Two Player*/
/*Carrier will be       C*/
/*Battleship will be    B*/
/*Destroyer will be     D*/
/*Submarine will be     S*/
/*Patrol Boat will be   P*/
/*Board is defined by which player sets the ships*/

/*Header Files*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


/*Function Prototypes*/
void main();                                                                                /*Startup function. Leads into game path*/
void view_instructions();                                                                   /*simply view all instructions*/
void name_amend(int x);                                                                     /*Amend name. Input is the player being amended*/
void game_path();                                                                           /*Path for one game loop*/
void board_set(int x);                                                                      /*Set up the board. Input is the current player*/
void board_view(int x, int y);                                                              /*View the current board*/
                                                                                            /*Input is the player, and setup or game view*/
void counter_reset();                                                                       /*reset all counter variables*/
void board_reset();

void ship_placement(int x, char ship, int c);                                               /*player number, ship character, ship length*/
int placement_check(int x, int c, int row, int column, int orientation);                    /*player number, ship length*/
void turn_input(int current_player);                                                        /*Get the correct player input and mark the game board each turn, returns sunk ship*/
int whose_turn(int t, int order);                                                           /*returns which player's turn it is (1/2)*/
void hit_count_decrement(int row, int column, int current_player);                          /*decrements hit counts for ships after turn input*/
void sunk_ship_check(int current_player);                                                   /*determines if a ship has just been sunk and marks it*/
int win_check(int current_player);                                                          /*checks if all ships are sunk for current player*/
void win_sequence(int winner);                                                              /*Announces winner at end of game*/





/*Global Variables*/

char setup_board_one[10][10];                                                               /*Viewed and amended by player one*/

char game_board_one[10][10];                                                                /*Viewed and amended by player two*/

char setup_board_two[10][10];                                                               /*Viewed and amended by player two*/

char game_board_two[10][10];                                                                /*Viewed and amended by player one*/

char player_one[20];                                                                        /*String for player one's name*/
char player_two[20];                                                                        /*String for player two's name*/

struct counter
{
    int carrier;
    int battleship;
    int destroyer;
    int submarine;
    int patrol_boat;
};

struct counter hit_counter_one =                                                            /*counter of hits on player one's ships*/
{
    5, 4, 3, 3, 2
};

struct counter hit_counter_two =                                                            /*counter of hits on player two's ships*/
{
    5, 4, 3, 3, 2
};

struct counter sunk_marker_one =                                                            /*which of player one's ships are sunk*/
{
    0, 0, 0, 0, 0
};

struct counter sunk_marker_two =                                                            /*which of player two's ships are sunk*/
{
    0, 0, 0, 0, 0
};

void main()                                                                                                     /*Setup function. Only runs on startup*/
{
    char instructions = 'A';

    board_reset();

    printf("\nWelcome to Battleship!\n");
    printf("\nWould you like to see all of the instructions?\n(Y/N):");

    do
    {
        fflush(stdin);
        scanf("%c", &instructions);
    } while(toupper(instructions)!= 'Y' && toupper(instructions)!= 'N');                                        /*Check for correct Y or N answer*/

    if(toupper(instructions) == 'Y')                                                                            /*Go to instructions()*/
        view_instructions();

    else
        printf("\n\nAlright then. You\'re on your own.\n");

    name_amend(1);                                                                                              /*Set Player 1 name*/

    name_amend(2);                                                                                              /*Set Player 2 name*/

    game_path();                                                                                                /*Enter Single game function loop*/

}   /*//////END MAIN()\\\\\\*/

void name_amend(int x)                      /*change the name of designated player*/
{
    /*int x represents which player is currently calling for a name change. 1 or 2*/
    printf("\nPlayer %d, type your name:\n", x);
    fflush(stdin);
    scanf("%s", x == 1? &player_one : &player_two);                                                 /*Scan in name string to the respective player*/

    printf("\nPlayer %d is now known as %s.\n", x, x == 1 ? player_one : player_two);                 /*Confirm name change*/

    return;
}   /*//////END name_amend()\\\\\\*/

void game_path()                                                                                    /*One single game loop*/
{
    /*////Variable Initializations\\\\*/

    int t = 0;                                                                                      /*turn counter*/
    long int seed = 0L;                                                                             /*stores seed for random turn order*/
    int order = 0;                                                                                  /*created by random number to define turn order*/
    int current_player = 0;
    int winner = 0;                                                                                 /*stores a winner at the end of each turn*/
                                                                                                    /*0 = no winner, 1 = player one wins, 2 = player two wins*/
    char another_game = 'Z';                                                                        /*choice for another game*/
    char name_change = 'Z';                                                                         /*choice for name change before anothr game*/

    do                              /*Single game loop*/
    {
        board_reset();

        counter_reset();

        winner = 0;

        seed = time(NULL);                                                                              /*set seed to current time*/

        srand(seed);                                                                                    /*set random seed*/

        printf("\nLet the battle commence!\n");

        printf("\n%s, you will set up your board first.\n", player_one);

        printf("\n%s, press ENTER to continue when %s isn't looking...\n", player_one, player_two);

        fflush(stdin);
        getchar();

        board_set(1);                                                                                   /*set board function for player one*/

        printf("\nNow %s, your turn to set up your fleet\n", player_two);

        printf("\n%s, press ENTER to continue when %s isn't looking...\n", player_two, player_one);

        fflush(stdin);
        getchar();

        board_set(2);                                                                                   /*set board function for player two*/

        printf("\nNow both boards are set.\n");                                                         /*Now both boards are set for the game to begin*/

        printf("\nNow let the battle commence!\n");

        order = (rand() %2) + 1;                                                                        /*order set to either 1 or 2*/

        printf("\n%s will start this battle.\n", order == 1 ? player_one : player_two);

        for(t = 0; winner == 0; t++)                                                                    /*one game turn loop*/
        {
            current_player = whose_turn(t, order);

            board_view(current_player, 2);                                                              /*view your game board to start the turn*/

            printf("\nSo %s, this is what you can see of the enemy.\n"
                   "Now choose your coordinates to fire upon.\n", current_player == 1 ? player_one : player_two);

            turn_input(current_player);

            winner = win_check(current_player);

            printf("\n\n~~~~~~~~~~\n\n");

        }   /*//////END of single turn for() loop\\\\\\*/

                                    /*Now a winner has been determined*/

        win_sequence(winner);


        do                                                                                              /*prompts for another game*/
        {
            printf("\nWould you like to play again? (Y/N)\n");
            fflush(stdin);
            scanf("%c", &another_game);
        }while(!(toupper(another_game) == 'Y' && toupper(another_game) == 'N'));                        /*Is answer Y or N ? */

        if(toupper(another_game == 'Y'))
        {
            do
            {
                printf("\nWill player one still be %s in this game? (Y/N)\n", player_one);
                fflush(stdin);
                scanf("%c", &name_change);
                name_change = toupper(name_change);
            }while(!(name_change == 'Y' || name_change == 'N'));

            if(name_change == 'N')
            {
                name_amend(1);
            }

            do
            {
                printf("\nWill player two still be %s in this game? (Y/N)\n", player_two);
                fflush(stdin);
                scanf("%c", &name_change);
                name_change = toupper(name_change);
            }while(!(name_change == 'Y' || name_change == 'N'));

            if(name_change == 'N')
            {
                name_amend(2);
            }
        }
    }while(toupper(another_game) == 'Y');                                                               /*single game loop*/




}   /*//////END game_path()\\\\\\*/

int whose_turn(int t, int order)
{
    if(order == 1)                                                                              /*define the current player wrt order and turn number*/
    {
        if(t%2 == 0)
            return 1;
        else
            return 2;
    }
    else
    {
        if(t%2 == 0)
            return 2;
        else
            return 1;
    }
}

void board_set(int x)                                                                               /*set up the board for the designated player*/
{
    int i = 0;      /*counter*/

    printf("\nNow it's time for %s to place your ships.\n", x == 1 ? player_one : player_two);

    printf("\nDon't let %s see!\n", x == 1 ? player_two : player_one);

    printf("\nFirst you will place your Carrier which takes up 5 spaces.\n");

    ship_placement(x, 'C', 5);      /*Carrier*/

    printf("\n\nNow place the battleship which is 4 spaces long.\n");

    ship_placement(x, 'B', 4);      /*Battleship*/

    printf("\n\nAnd the 3 spaces for the Destroyer\n");

    ship_placement(x, 'D', 3);      /*Destroyer*/

    printf("\n\nThe submarine next takes up 3 spaces as well.\n");

    ship_placement(x, 'S', 3);      /*Submarine*/

    printf("\n\nAnd last to be placed is the Patrol Boat which takes up 2 spaces.\n");

    ship_placement(x, 'P', 2);      /*Patrol Boat*/

    board_view(x, 1);

    printf("\nCongratulations %s, your fleet is positioned superbly\n", x == 1 ? player_one : player_two);

    printf("Press ENTER to continue...\n");

    fflush(stdin);
    getchar();

    for(i=0; i<50; i++)
        printf("*");

    for(i=0; i<100; i++)
        printf("\n");

    for(i=0; i<50; i++)
        printf("*");

    return;
}/*//////END board_set()\\\\\\*/

void board_view(int x, int y)               /*view the board for the designated player*/
{
    /*First variable, player defined as current player*/
    /*Second variable, 1 = setup view, 2 = game play view*/
    int counter_row = 0;
    int counter_column = 0;

    printf("   ");

    for(counter_column = 0; counter_column<10; counter_column++)
    {
        printf("%-2d", counter_column+1);
    }

    printf("\n\n");

    if(x == 1)                                                                              /*player one will view setup_board_one during setup*/
                                                                                            /*and game_board_two during gameplay*/
    {
        for(counter_row=0; counter_row<10; counter_row++)
        {
            printf("%-2c", counter_row + 65);

            for(counter_column=0; counter_column<10; counter_column++)
            {
                printf("%2c", y == 1 ? setup_board_one[counter_row][counter_column] : game_board_two[counter_row][counter_column]);
            }

            printf("\n");

        }
    }
    else    /*x == 2*/                                                                      /*player two will view setup_board_one during setup*/
                                                                                            /*and game_board_one during game play*/
    {
        for(counter_row=0; counter_row<10; counter_row++)
        {
            printf("%-2c", counter_row + 65);

            for(counter_column=0; counter_column<10; counter_column++)
            {
                printf("%2c", y == 1 ? setup_board_two[counter_row][counter_column] : game_board_one[counter_row][counter_column]);
            }

            printf("\n");

        }
    }


}/*//////END board_view()\\\\\*/

void ship_placement(int x, char ship, int c)
{
    /*input int x is the player who is setting their board*/
    /*input int y is the ship being placed*/

    char row = 'Z';
    int column = 100;
    int orientation = 5;
    int i = 0;
    int marker = 0;

    board_view(x, 1);

    do                                                                                                  /*START loop asking for correct ship placement input*/
    {
        row = 'Z';                      /*reset all input values*/
        column = 0;
        orientation = 3;

        do                                                                                              /*prompt for beginning coordinate to place the ship*/
        {
            printf("\nInput the coordinate for the upper-most, or left-most point of the ship \n"
                "as LETTER SPACE NUMBER\n");

            fflush(stdin);
            scanf("%c %d", &row, &column);
        }while(!((toupper(row)>='A' && toupper(row) <= 'J') && (column>=1 && column<=10)));             /*check for valid entry. A-J and 1-10*/

        do                                                                                              /*prompt for orientation of the ship*/
        {
            printf("Press 1 to place your ship down from that point \n"
                    "or press 2 to place your ship right from that point:\n");

            fflush(stdin);
            scanf("%d", &orientation);
        }while(!(orientation==1 || orientation==2));                                                    /*check for valid entry*/

        row = toupper(row);                                                                             /*convert row to uppercase letter*/

        row = row - 65;                                                                                 /*change ascii value of row to coordinate value*/

        column--;                                                                                       /*subtract 1 from column to make it the array coordinate*/


    }while(placement_check(x, c, row, column, orientation));                                /*End ship placement loop*/
                                                                                            /*Check if ship fits on board and doesn't cross another ship*/

    for(i = 0 ; i < c ; i++)                                                                /*mark the selected squares as occupied by the given ship*/
    {
         if(x == 1)                                                                         /*if current player is player one*/
         {
             if(orientation == 1)
                setup_board_one[row+i][column] = ship;

             else
                setup_board_one[row][column+i] = ship;
         }
         else   /*x == 2*/                                                                  /*if current player is player two*/
         {
             if(orientation == 1)
                setup_board_two[row+i][column] = ship;

            else
                setup_board_two[row][column+i] = ship;
         }
    }

    return;

}/*//////END ship_placement\\\\\\*/

int placement_check(int x, int c, int row, int column, int orientation)
{
    int i;                                                                                  /*for loop counter*/
    int marker = 0;

    for(i = 0 ; i < c ; i++)                                                                /*check that all the squares for the ship are open spaces*/
    {

        if(x == 1)                                                                         /*if current player is player one*/
        {
            if(orientation == 1)
            {
                if(setup_board_one[row+i][column] != 'O')
                    marker = 1;
            }

            else   /*orientation == 2*/
            {
                if(setup_board_one[row][column+i] != 'O')
                    marker = 1;
            }

         }
         else   /*x == 2*/                                                                  /*if current player is player two*/
         {
            if(orientation == 1)
            {
                if(setup_board_two[row+i][column] != 'O')
                    marker = 1;
            }

            else    /*orientation == 2*/
            {
                if(setup_board_two[row][column+i] != 'O')
                    marker = 1;
            }
        }
    }

    if(orientation == 1)                                                                    /*checking within the boundries*/
    {
        if(row + c > 10)
            marker = 1;
    }
    else    /*orientation == 2*/
    {
        if(column + c > 10)
            marker = 1;
    }

    if(marker != 0)
    {
        printf("\nInvalid ship placement \n"
               "Your ship either crosses another ship or leaves the boundry of the board \n"
               "Try another placement.\n");
    }
    else
    {
        printf("\nShip placement approved!\n");
    }


    return marker;
}

void turn_input(int current_player)                                                                                   /*Get player input each turn and mark the game board*/
{
    char row = 'z';
    int column = 100;
    /*Initialize Variables*/

    do                                                                                                          /*outer loop for correct player input*/
    {
        do
        {
            printf("\n%s, enter your firing coordinates:\n", current_player == 1 ? player_one : player_two);
            fflush(stdin);
            scanf("%c %d", &row, &column);

            if(!((toupper(row)>='A' && toupper(row) <= 'J') && (column>=1 && column<=10)))                      /*explain incorrect input*/
            {
                printf("\nThe coordinate you have chosen is not within the bounderies of the game board\n");
                printf("\nTry again\n");
            }
        }while(!((toupper(row)>='A' && toupper(row) <= 'J') && (column>=1 && column<=10)));

        column--;                                                                                               /*convert column to array coordinate*/

        row = toupper(row);

        row = row - 65;                                                                                         /*convert row ascii entry to array coordinate*/

        if((current_player == 1 ? game_board_two[row][column] : game_board_one[row][column]) != 'O')            /*explain incorrect input*/
        {
            printf("\nThis coordinate has been fired upon already\n");
            printf("\nTry again\n");
        }

    }while((current_player == 1 ? game_board_two[row][column] : game_board_one[row][column]) != 'O');           /*Has the coordinate been guessed and marked yet?*/



    if((current_player == 1 ? setup_board_two[row][column] : setup_board_one[row][column]) == 'O')
                                                                                            /*if the coordinate is empty on the setup board, then create "Miss" mark*/
    {
        printf("\nMISS!!\n");
        ((current_player == 1) ? (game_board_two[row][column] = 'M') : (game_board_one[row][column] = 'M'));
    }
    else                                                                                /*If the coordinate on the setup board is occupied by a ship, then create "Hit" mark*/
    {
        printf("\nHIT!!\n");
        ((current_player == 1) ? (game_board_two[row][column] = 'H') : (game_board_one[row][column] = 'H'));
    }

    hit_count_decrement(row, column, current_player);

}   /*//////END Turn_input()\\\\\\*/

void hit_count_decrement(int row, int column, int current_player)
{

    switch((current_player == 1) ? (setup_board_two[row][column]) : (setup_board_one[row][column]))
    {
        case 'C':
            current_player == 1 ? hit_counter_two.carrier-- : hit_counter_one.carrier--;
            break;

        case 'B':
            current_player == 1 ? hit_counter_two.battleship-- : hit_counter_one.battleship--;
            break;

        case 'D':
            current_player == 1 ? hit_counter_two.destroyer-- : hit_counter_one.destroyer--;
            break;

        case 'S':
            current_player == 1 ? hit_counter_two.submarine-- : hit_counter_one.submarine--;
            break;

        case 'P':
            current_player == 1 ? hit_counter_two.patrol_boat-- : hit_counter_one.patrol_boat--;
            break;

        default:
            /*no hit*/
            break;

    /*done decrementing the counters for the current turn*/
    }

    sunk_ship_check(current_player);                                                    /*now check if a ship has been sunk*/

}   /*//////END hit_count_decrement()\\\\\\*/

void sunk_ship_check(int current_player)                                                    /*test if a ship has been sunk and mark it*/
{
    if(current_player == 1)
    {
        if(hit_counter_two.carrier == 0)
        {
            if(sunk_marker_two.carrier == 0)                        /*has ship already been marked as sunk*/
                printf("\nYou sunk my carrier!\n");

            sunk_marker_two.carrier = 1;                           /*mark ship as sunk*/
        }
        if(hit_counter_two.battleship == 0)
        {
            if(sunk_marker_two.battleship == 0)
                printf("\nYou sunk my battleship!\n");

            sunk_marker_two.battleship = 1;
        }
        if(hit_counter_two.destroyer == 0)
        {
            if(sunk_marker_two.destroyer == 0)
                printf("\nYou sunk my destroyer!\n");

            sunk_marker_two.destroyer = 1;
        }
        if(hit_counter_two.submarine == 0)
        {
            if(sunk_marker_two.submarine == 0)
               printf("\nYou sunk my submarine!\n");

            sunk_marker_two.submarine = 1;
        }
        if(hit_counter_two.patrol_boat == 0)
        {
            if(sunk_marker_two.patrol_boat == 0)
               printf("\nYou sunk my patrol boat!\n");

            sunk_marker_two.patrol_boat = 1;
        }
    }
    else    /*current_player == 2*/
    {
        if(hit_counter_one.carrier == 0)
        {
            if(sunk_marker_one.carrier == 0)                        /*has ship already been marked as sunk*/
                printf("\nYou sunk my carrier!\n");

            sunk_marker_one.carrier = 1;                           /*mark ship as sunk*/
        }
        if(hit_counter_one.battleship == 0)
        {
            if(sunk_marker_one.battleship == 0)
                printf("\nYou sunk my battleship!\n");

            sunk_marker_one.battleship = 1;
        }
        if(hit_counter_one.destroyer == 0)
        {
            if(sunk_marker_one.destroyer == 0)
                printf("\nYou sunk my destroyer!\n");

            sunk_marker_one.destroyer = 1;
        }
        if(hit_counter_one.submarine == 0)
        {
            if(sunk_marker_one.submarine == 0)
               printf("\nYou sunk my submarine!\n");

            sunk_marker_one.submarine = 1;
        }
        if(hit_counter_one.patrol_boat == 0)
        {
            if(sunk_marker_one.patrol_boat == 0)
               printf("\nYou sunk my patrol boat!\n");

            sunk_marker_one.patrol_boat = 1;
        }
    }




}   /*//////END sunk_ship_check()\\\\\\*/

int win_check(int current_player)                                               /*check if all ships have been sunk for current player*/
{
    if(current_player == 1)
    {
        if(sunk_marker_two.carrier == 1 &&
           sunk_marker_two.battleship == 1 &&
           sunk_marker_two.destroyer == 1 &&
           sunk_marker_two.submarine == 1 &&
           sunk_marker_two.patrol_boat == 1)

           return 1;

        else
            return 0;
    }
    else    /*current_player() = 2*/
    {
        if(sunk_marker_one.carrier == 1 &&
           sunk_marker_one.battleship == 1 &&
           sunk_marker_one.destroyer == 1 &&
           sunk_marker_one.submarine == 1 &&
           sunk_marker_one.patrol_boat == 1)
           return 2;

        else
            return 0;
    }
}   /*//////END win_check()\\\\\\*/

void board_reset()                                              /*reset all coordinates on all boards to O*/
{
    int i;                                                      /*row counter for coordinate reset*/
    int j;                                                      /*column counter for coordinate reset*/

    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            setup_board_one[i][j]= 'O';
            game_board_one[i][j]= 'O';
            setup_board_two[i][j]= 'O';
            game_board_two[i][j]= 'O';
        }
    }

    return;
}/*//////END board_reset()\\\\\\*/

void counter_reset()
{
    hit_counter_one.carrier = 5;
    hit_counter_one.battleship = 4;
    hit_counter_one.destroyer = 3;
    hit_counter_one.submarine = 3;
    hit_counter_one.patrol_boat = 2;

    hit_counter_two.carrier = 5;
    hit_counter_two.battleship = 4;
    hit_counter_two.destroyer = 3;
    hit_counter_two.submarine = 3;
    hit_counter_two.patrol_boat = 2;

    sunk_marker_one.carrier = 0;
    sunk_marker_one.battleship = 0;
    sunk_marker_one.destroyer = 0;
    sunk_marker_one.submarine = 0;
    sunk_marker_one.patrol_boat = 0;

    sunk_marker_two.carrier = 0;
    sunk_marker_two.battleship = 0;
    sunk_marker_two.destroyer = 0;
    sunk_marker_two.submarine = 0;
    sunk_marker_two.patrol_boat = 0;

}   /*//////END counter_reset()\\\\\\*/

void view_instructions()
{
    printf("\n\nSo you think you need to read the instructions?"
           "\nWhere's your sense of adventure?\n");
    printf("Press ENTER to keep them coming");
    fflush(stdin);
    getchar();

    printf("\n\nSo the object of the game is to \"sink\" your opponent's ships \n"
           "before he does the same to you.\n");
    printf("\nYou will start with each player secretly placing all of their ships \n"
           "on their board.\n");
    printf("\nDo not let your opponent see the screen while you are placing your ships!\n");
    getchar();

    printf("\nYou will each need to place five ships on your board to begin.\n\n");
    printf("You need to place a carrier which takes up 5 spaces,"
           "\nA battleship which takes up 4 spaces,"
           "\nA destroyer and a submarine which take up 3 spaces,"
           "\nAnd a Patrol Boat which takes up 2 spaces.\n");
    getchar();

    printf("\nAfter each player sets their boards, \n"
           "you will alternate guessing coordinates to try and hit ships.\n");
    printf("\nCoordinates are represented as a letter and a number like (B,5)\n");
    getchar();

    printf("\nThe first player to sink all their opponent's ships is the winner.\n");

    printf("\nHappy hunting!");

    getchar();

    return;
}   /*//////END view_instructions()\\\\\\*/

void win_sequence(int winner)
{
    printf("\nCongrats %s. You are the winner.\n", winner == 1 ? player_one : player_two);

    printf("\nYou sunk all of %s's ships!\n", winner == 1 ? player_two : player_one);
}










