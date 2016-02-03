#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NB_PLAYER 10
#define PLATEAU_HEIGHT 200
#define PLATEAU_WIDTH 200

#define MAX_LEN_POSSIBLE (int)(65535 / sizeof(int))

#define LIFE_POINTS 21

#define CORVETTE_ID 6  //pique
#define CORVETTE_WIDTH 1
#define CORVETTE_NB 1

#define DESTROYER_ID 5 //trefle
#define DESTROYER_WIDTH 3
#define DESTROYER_NB 2

#define CROISEUR_ID 3   //coeur
#define CROISEUR_WIDTH 4
#define CROISEUR_NB 2

#define PORTE_AVION_ID 4    //carreaux
#define PORTE_AVION_WIDTH 6
#define PORTE_AVION_NB 1

/*

Début des prototypages

*/

//Generate the empty map for all the players
void init_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI);
//Print the actual map
void print_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int whosPlaying);
//Random function
int randomize(int MIN, int MAX);
//run the test for the boat to be placed without any confusions
int test_boat_positon(int x, int y, int direction, int length_boat, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player);
//Set the boat placement
void set_boat(int id, int size, int nbr, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player);
//Call everything ships one by one
void set_all_ships(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI);
//define the color of foreground and background
void color(int t,int f);
//Define the number of boats and the size of the map from user inputs
void set_parameters(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]);
//Set the different colors for the different players
void set_color_ships(int k);
//Ask inputs for the coordinate to shoot
int set_longitude();
int set_lattitude();
//Roll the different turns of the players and all the ongoing of the game
void player_rolling(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI);
//test to know if the shoots hit or not
int test_hit(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude);
//Update the map with all the shoots and hits
void update_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude, int playerHit, int touch);
//Test of victory
int victory_test(int lifeTable[NB_PLAYER], int nb_joueurs);
//Set the screen for victory
void set_victory_screen(int lifeTable[NB_PLAYER], int nb_joueurs);
//Initialize the life points for all the players
int* init_table_life(int lifeTable[NB_PLAYER], int nb_joueurs);
    //select game mode (Disabled because not finished)
    //int set_mode();
//Draw the starting menu
void draw_menu(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]);
//Control user input int
int control_input_int();
    //Control user input char
    //char control_input_char();

/*

End of prototypes and beginning of the main

*/
int main(int argc, char *argv[])
{
    char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER];
    draw_menu(board_game);

    return 0;
}

/*

From here start all the different functions

*/

/*char control_input_char(){
    char userChoise = ' ';
    char userNumber[MAX_LEN_POSSIBLE];

    while(userChoise){
        fflush(stdin);
        scanf ("%s", userNumber);

        userChoise = userNumber[0];
        if(userChoise >= 0 && userChoise < 201){
            return userChoise;
        }else{
            userChoise = 1;
            printf ("Please use just char written on the longitude board ");
        }
    }

    return 0;
}*/

int control_input_int(){
    int userChoise = 1;
    char userNumber[MAX_LEN_POSSIBLE];

    while(userChoise){
        fflush(stdin);
        scanf ("%s", userNumber);

        userChoise = atoi(userNumber);
        if(userChoise >= 0 && userChoise < 201){
            return userChoise;
        }else{
            userChoise = 1;
            printf ("Please use just numbers ");
        }
    }

    return 0;
}

void draw_menu(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]){

    system("cls");

    color(10, 0);

    printf("#############################################################\n");
    printf("#############################################################\n");
    printf("#                                                           #\n");
    printf("#                                                           #\n");
    printf("#                WELCOME TO MY OWN BATTLESHIP               #\n");
    printf("#                                                           #\n");
    printf("#                                                           #\n");
    printf("#############################################################\n");
    printf("#############################################################\n");

    color(15, 0);

    set_parameters(board_game);
}

/*int set_mode(){
    int mode = 0;
    printf("Please select the mode of ship setting : \n");
    printf("0 : Automatic (Random)\n");
    printf("1 : Manual (Give your own coordinates)\n");
    while(mode < 0 && mode > 1){
        mode = control_input_int();
    }

    return mode;
}*/

void set_victory_screen(int lifeTable[NB_PLAYER], int nb_joueurs){
    int i = 0;
    int winner = 0;
    char flag = 0;
    for(i = 0; i < nb_joueurs; i++){
        if(lifeTable[i] > 0){
            winner = i + 1;
        }
    }

    while(flag < 10){
        for(i = 0; i < 15; i++){
            Sleep(10);
            system("cls");
            color(i, 0);
            printf("#############################################################\n");
            printf("#############################################################\n");
            printf("#                                                           #\n");
            printf("#                                                           #\n");
            printf("#                          VICTORY                          #\n");
            printf("#                   Player %d is the best !!!                #\n", winner);
            printf("#                                                           #\n");
            printf("#                                                           #\n");
            printf("#############################################################\n");
            printf("#############################################################\n");
        }
        flag++;
    }
}

int victory_test(int lifeTable[NB_PLAYER], int nb_joueurs){
    int countingPlayers = 0;
    int i = 0;
    for(i = 0; i < nb_joueurs; i++){
        if(lifeTable[i]  == 0){
            countingPlayers++;
        }
    }
    if(countingPlayers == (nb_joueurs - 1)){
        return 0;
    }
    else{
        return 1;
    }
}

void update_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int player, char longitude, int lattitude, int playerHit, int touch){
    if(touch == 1){
        //15 est une étoile/mine en ASCII
        board_game[lattitude - 1][longitude - 65][player] = 15;
        board_game[lattitude - 1][longitude - 65][playerHit] = 15;
    }
    else{
        //88 est un X en ASCII
        board_game[lattitude - 1][longitude - 65][player] = 88;
    }
}

int test_hit(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_total_joueurs, int player, char longitude, int lattitude){
    printf("\nJe test le tir\n");
    int k = 1;
    int playerHit = 0;
    int touch = 0;
    char next = ' ';
    for(k = 1; k <= nb_total_joueurs; k++){
        if(board_game[lattitude - 1][longitude - 65][k] != '~'){
            if(k == player){
                printf("\nYou can't shoot your own ships ...\n");
                break;
            }
            else{
                playerHit = k;
                touch = 1;
                switch(board_game[lattitude - 1][longitude - 65][playerHit]){
                    case 6:
                        printf("\nxxx You hit a corvette of player %d xxx\n", playerHit);
                        break;
                    case 5:
                        printf("\nxxx You hit a destroyer of player %d xxx\n", playerHit);
                        break;
                    case 3:
                        printf("\nxxx You hit a croiseur of player %d xxx\n", playerHit);
                        break;
                    case 4:
                        printf("\nxxx You hit a porte-avion of player %d xxx\n", playerHit);
                        break;
                    default:
                        break;
                }
            }
        }
        else{
            printf("\nYou missed, try again ...\n");
            break;
        }
    }
    update_board(board_game, width, height, nb_total_joueurs, player, longitude, lattitude, playerHit, touch);
    fflush(stdin);
    while(next != '\r' && next != '\n'){
        printf("Press enter for next player ...\n");
        next = getchar();
    }

    return playerHit;
}

int* init_table_life(int lifeTable[NB_PLAYER], int nb_total_joueurs){
    int i = 0;
    for(i = 0; i < nb_total_joueurs; i++){
        lifeTable[i] = LIFE_POINTS;
    }
    return lifeTable;
}

void player_rolling(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI){
    int player = 0;
    int longitude = 0;
    int lattitude = 0;
    int total_player = nb_joueurs + nb_AI;
    int lifeTable[NB_PLAYER];
    init_table_life(lifeTable, total_player);
    int flag = 1;
    while(flag){
        if(nb_joueurs > 0){
            for(player = 1; player <= total_player; player++){
                int hit = 0;
                if(lifeTable[total_player - 1] > 0){
                    printf("Player %d have to play...\n", player);
                    if(player <= nb_joueurs){
                        print_board(board_game, width, height, nb_joueurs, player);
                        printf("Give the coordinates you want to shoot ?\n");
                        longitude = set_longitude();
                        lattitude = set_lattitude();
                    }
                    else{
                        longitude = randomize(1, width);
                        lattitude = randomize(1, height);
                    }
                    hit = test_hit(board_game, width, height, total_player, player, longitude, lattitude);
                    if(hit){
                        (lifeTable[hit - 1])--;
                    }
                }
                else{
                    printf("\nPlayer %d is dead !!!\n", player);
                }
            }
        }
        flag = victory_test(lifeTable, total_player);
    }
    set_victory_screen(lifeTable, total_player);
}

int set_longitude(){
    char longitude = 0;
    fflush(stdin);
    printf("\nPlease set the longitude (characters only) : ");
    longitude = getchar();
    return longitude;
}

int set_lattitude(){
    int lattitude = 0;
    fflush(stdin);
    printf("Please set the lattitude : ");
    lattitude = control_input_int();
    return lattitude;
}

void set_color_ships(int k){
    switch(k){
        case 0:
            //couleur de la mer
            color(15, 0);
            break;
        case 1:
            //Couleur ROUGE du Player 1
            color(12, 0);
            break;
        case 2:
            //Couleur BLEU du player 2
            color(9, 0);
            break;
        case 3:
            //Couleur VERT FLUO du player 3
            color(10, 0);
            break;
        case 4:
            //Couleur JAUNE FLUO du player 4
            color(14, 0);
            break;
        default:
            //couleur de la mer
            color(15, 0);
            break;
    }
}

void set_parameters(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER]){
    int width = 1;
    int height = 1;
    int nb_joueurs = 1;
    int nb_AI = 1;

    printf("Please enter the number of human players (MAX 10 and recommended below 4): ");
    nb_joueurs = control_input_int();
    printf("Please enter the number of A.I. (MAX 10 and recommended below 4): ");
    nb_AI = control_input_int();
    printf("Please set the width of the map you want (MAX 200 and recommended 26) : ");
    width = control_input_int();
    printf("Please set the height of the map (MAX 200 recommended 18): ");
    height = control_input_int();
    init_board(board_game, width, height, nb_joueurs, nb_AI);
    set_all_ships(board_game, width, height, nb_joueurs, nb_AI);
    player_rolling(board_game, width, height, nb_joueurs, nb_AI);
}

void color(int t,int f){
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}
/*les valeurs:
0: noir
1: bleu foncé
2: vert
3: bleu-gris
4: marron
5: pourpre
6: kaki
7: gris clair
8: gris
9: bleu
10: vert fluo
11: turquoise
12: rouge
13: rose fluo
14: jaune fluo
15: blanc */

void set_all_ships(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI){
    int p = 0;
    int nb_tot_player = 0;
    nb_tot_player = nb_joueurs + nb_AI;
    for(p = 0; p < nb_tot_player; p++){
        set_boat(PORTE_AVION_ID, PORTE_AVION_WIDTH, PORTE_AVION_NB, board_game, width, height, p);
        set_boat(CROISEUR_ID, CROISEUR_WIDTH, CROISEUR_NB, board_game, width, height, p);
        set_boat(DESTROYER_ID, DESTROYER_WIDTH, DESTROYER_NB, board_game, width, height, p);
        set_boat(CORVETTE_ID, CORVETTE_WIDTH, CORVETTE_NB, board_game, width, height, p);
   }
}

void set_boat(int id, int size, int nbr, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player){
    int i = 0;
    int j = 0;
    //int mode = 0;
    //mode = set_mode();

    while(nbr > 0){
        int flag = 1;
        int direction = randomize(0,3);

        while(flag){

            //if(mode == 0){
            int x = randomize(1, width);
            int y = randomize(1, height);
            /*}
            else{
                printf("Now you should place your boats with x (between 1 and %d) and y (between 1 and %d) : ", width, height);
            x = control_input_int();
            y = control_input_int();
            }*/
            //
            if(test_boat_positon(x, y, direction, size, board_game, width, height, player) == 0){
                if(direction == 0){
                    for(i = y; i <= (y + (size - 1)); i++){
                        j = x;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id;
                        printf("Player %d : %c\n", player + 1, id);
                        flag = 0;
                    }
                }
                else if(direction == 1){
                    for(j = x; j <= (x + (size - 1)); j++){
                        i = y;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id;
                        printf("Player %d : %c\n", player + 1, id);
                        flag = 0;
                    }
                }
                else if(direction == 2){
                    for(i = y; i >= (y - (size - 1)); i--){
                        j = x;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id;
                        printf("Player %d : %c\n", player + 1, id);
                        flag = 0;
                    }
                }
                else if(direction == 3){
                    for(j = x; j >= (x - (size - 1)); j--){
                        i = y;
                        board_game[i-1][j-1][0] = id;
                        board_game[i-1][j-1][player+1] = id;
                        printf("Player %d : %c\n", player + 1, id);
                        flag = 0;
                    }
                }
            }
        }
        nbr--;
    }
}

int test_boat_positon(int x, int y, int direction, int size, char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int player){
    int test = 1;
    int i = 0;
    int j = 0;
    if(direction == 0){
        if((y + (size - 1)) <= height){
            for(i = y; i <= (y + (size - 1)); i++){
                j = x;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 1){
        if((x + (size - 1)) <= width){
            for(j = x; j <= (x + (size - 1)); j++){
                i = y;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 2){
        if((y - (size - 1)) > 0){
            for(i = y; i >= (y - (size - 1)); i--){
                j = x;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    else if(direction == 3){
        if((x - (size - 1)) > 0){
            for(j = x; j >= (x - (size - 1)); j--){
                i = y;
                if(board_game[i-1][j-1][0] == '~'){
                    test = 0;
                }
                else{
                    test = 1;
                    break;
                }
            }
        }
        else{
            test = 1;
        }
    }
    if(test == 1){
        return -1;
    }
    else{
        return 0;
    }
}

int randomize(int MIN, int MAX){
    srand(time(NULL));
    return (rand() % (MAX - MIN + 1)) + MIN;
}

void init_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int nb_AI){
    int i = 0;
    int j = 0;
    int k = 0;
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            for(k = 0; k < (nb_joueurs + nb_AI + 1); k++){
                board_game[i][j][k] = '~';
            }
        }
    }
}

void print_board(char board_game[PLATEAU_HEIGHT][PLATEAU_WIDTH][NB_PLAYER], int width, int height, int nb_joueurs, int whosPlaying){

    system("cls");

    int i = 0;
    int j = 0;
    for(i = -2; i < height; i++){
        for(j = -3; j < width; j++){
            set_color_ships(0);
            if(board_game[i][j][whosPlaying] == 6 || board_game[i][j][whosPlaying] == 5 || board_game[i][j][whosPlaying] == 3 || board_game[i][j][whosPlaying] == 4){
                set_color_ships(whosPlaying);
            }
            else if(board_game[i][j][whosPlaying] == 88 || board_game[i][j][whosPlaying] == 15){
                color(12, 15);
            }
            if(i == -2 && j >= 0){
                color(0, 15);
                printf("%c", (char)(j + 65));
                if(j > 90){
                    printf(" ");
                }
            }
            else if(i == -2 && j < 0){
                printf(" ");
            }
            else if(i == -1){
                printf(" ");
            }
            else{
                if(j == -3 && i >= 0){
                    color(0, 15);
                    printf("%d", i + 1);
                }
                else if(j == -1 && i >= 0){
                    if(i <= 8){
                        color(0, 0);
                        printf("  ");
                    }
                    else{
                        color(0, 0);
                        printf(" ");
                    }
                }
                else if(j >= 0){
                    printf("%c", board_game[i][j][whosPlaying]);
                }
            }
        }
        printf("\n");
    }
}
