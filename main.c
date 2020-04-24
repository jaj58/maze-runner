/*
 * File:   main.c
 * Author: jaj58
 *
 * Created on 20 February 2020, 11:38
 */
#include "allcode_api.h"
#include "star_wars.h"
#include <stdio.h>


typedef enum{
    north,
    east,
    south,
    west
} direction;

typedef enum {
    normal,
    shaded
}cell_lighting;

typedef enum {
    unexplored,
    explored,
    wall
}exit_options;

/*
 * each cell we need to store:
 * bool visited
 * cell_lighting (normal or shaded)
 * exit_options exits[4] -> stored NORTH[0],EAST[1],SOUTH[2],WEST[3]
 * exit_options -> wall, explored, unexplored
 */

typedef struct {
    bool visited;
    cell_lighting lighting;
    exit_options exits[4]; //stored NORTH[0],EAST[1],SOUTH[2],WEST[3]
}cell;

enum ir_sensors{
    left_ir,
    front_left_ir,
    front_ir,
    front_right_ir,
    right_ir,
    back_right_ir,
    back_ir,
    back_left_ir
};

//globals
direction current_direction = north; //store our current facing direction.
direction last_direction = north; //direction we came from last cell.
int current_cell = 0; //current cell number.
int last_cell = 0; //cell we just came from.
int cells_explored = 0; //keeping track of how many cells we've currently explored and when to finish exploring.
int shaded_cells = 0; //keeping track of how many shaded cells there are in an easy to access variable.
cell cells[25]; //using our cell's struct we create a array of a total of 25 cells (5x5 maze).
#define cell_length 150 //mm this never changes so lets just define it, used to move around the maze accurately.
bool going_backwards = false; //used when we are going back the same way so we don't increase the cells explored and other things...


void fill_cell(){
    /*
     *we've already been to this cell, therefore we don't need to fill out it's information
     * nor do we wish to increase our cells_explored variable used below so let's just 
     * return back to work. 
     */
    if(cells[current_cell].visited == true){
        return;
    }
    
    cells_explored++; //we are visiting a new cell let's increase our cells explored counter.
    cells[current_cell].visited = true; //we will now have visited this current cell so let's take note of this

    //unsure if 80 is a good value in the maze but seems to be okay whilst testing.
    if(FA_ReadIR(front_ir) > 80){ //there is a wall in front of us...
        cells[current_cell].exits[north] = wall;
    }
    if(FA_ReadIR(right_ir) > 80){ //there's a wall to our east...
        cells[current_cell].exits[east] = wall;
    }
    if(FA_ReadIR(back_ir) > 80){ //there's a wall to our south, this shouldn't happen but incase....
        cells[current_cell].exits[south] = wall;
    }
    if(FA_ReadIR(left_ir) > 80){ //there's a wall to our west...
        cells[current_cell].exits[west] = wall;
    }
    
    //let's take a total of 3 light readings to try get a more accurate value but let's not
    //waste too much time doing this....
    unsigned short val1,val2,val3;
    val1 = FA_ReadLight();
    FA_DelayMillis(500);
    val2 = FA_ReadLight();
    FA_DelayMillis(500);
    val3 = FA_ReadLight();
    unsigned short avg_val = val1 + val2 + val3 / 3;
    
    //unsure if 1200 is a great value for the actual shaded areas, couldn't remember the values from testing and didn't have access
    //to old code so decided on using this for now but can easily be tested and modified if needed.
    if(avg_val < 1200){ 
        FA_LCDClear(); 
        FA_LCDPrint("found a new shaded area.",24,0,0,FONT_NORMAL,LCD_TRANSPARENT);
        cells[current_cell].lighting = shaded; //make sure we take not this cell is shaded.
        shaded_cells++; //increase our counter for the amount of shaded cells.
        
        //let's play a little victory tone for finding a shaded cell so the user knows.
        FA_PlayNote(2000, 100);
        FA_DelayMillis(200);
        FA_PlayNote(3000, 200);
        FA_DelayMillis(200);
        FA_PlayNote(4000, 400); 
        FA_DelayMillis(5000); //make sure user has time to read display to explain why we did this celebration.
    }
}

//these 3 functions are dirty but do the job of keeping our direction correct
//there are other way's of doing this but this seems to be the most accurate
//without ever causing errors or failing to keep it correct. 
void update_direction_left(){ //we just turned left so let's update our direction based upon that.
    switch(last_direction){
        case north:
            current_direction = west;
            break;
        case east:
            current_direction = north;
            break;
        case south:
            current_direction = east;
            break;
        case west:
            current_direction = south;
            break;
    }
}

void update_direction_right(){//we just turned right so let's update our direction based upon that.
    switch(last_direction){
        case north:
            current_direction = east;
            break;
        case east:
            current_direction = south;
            break;
        case south:
            current_direction = west;
            break;
        case west:
            current_direction = north;
            break;
    }
}

void update_direction_backwards(){//we just did a 180 so let's update our direction based upon that.
   switch(last_direction){
        case north:
            current_direction = south;
            break;
        case east:
            current_direction = west;
            break;
        case south:
            current_direction = north;
            break;
        case west:
            current_direction = east;
            break;
    } 
}

int main()
{
    FA_RobotInit(); //startup the robot.
    FA_LCDBacklight(90);
    
    while(cells_explored != 25){ //keep exploring till we have filled out our maze map...
        fill_cell();//first let's fill out what we can from our current cell and add this info to our map array.
        
        if(!going_backwards){ //lets follow the left wall unless we hit a dead end and 180 which requires a change in behaviour.
            if(cells[current_cell].exits[west] != wall){ //no wall to our left so let's explore that way
                FA_Left(90); 
                FA_Forwards(cell_length ); 
                update_direction_left(); 
                last_cell = current_cell; 
                current_cell++; 
            }else if(cells[current_cell].exits[north] != wall){ //wall to our left but no wall forwards so lets go that way.
                FA_Forwards(cell_length); 
                //still going north no need to update direction...
                last_cell = current_cell; 
                current_cell++; 
            }else if(cells[current_cell].exits[east] != wall){ //wall to our left and forward so let's go right if we can.
                FA_Right(90); 
                FA_Forwards(cell_length); 
                update_direction_right(); 
                last_cell = current_cell; 
                current_cell++; 
            }else{ //we've run out of options and not reached our maximum amount of cells so we've hit a dead end so let's turn around and follow our new behaviour.
                FA_Left(180); 
                FA_Forwards(cell_length); 
                update_direction_backwards(); 
                last_cell = current_cell; 
                current_cell--; //decrease cell count because we've gone back into the cell we came from...
                going_backwards = true; //set going backwards to true because we need to follow different behaviour now we are going back over previously visited cells
            }
            last_direction = current_direction; //set our last direction to our current...
        }else{ //we've had to 180 cause we hit a dead end, which requires slightly different behaviour.
            while(cells[current_cell].exits[west] == wall){ //lets just keep going forward decreasing our cell count till we find a exit to the left
                FA_Forwards(cell_length); 
                last_cell = current_cell;
                current_cell--; //still going back over previously visited cells so decrease our current_cell count.
            }
            //we've found a new exit to our left so lets now turn and follow it
            FA_Left(90);
            FA_Forwards(cell_length); 
            update_direction_left();
            last_cell = current_cell;
            
            /*
             *this is a brand new cell but it could actually be cell 6 but we came from cell 3 
             * so we use cells_explored to keep track here instead of using current_cell + 1. 
             * We also don't need to +1 to this cells_explored variable
             * this is because our struct starts at 0.
             */
            current_cell = cells_explored;
            last_direction = current_direction;
            going_backwards = false; //since we are no longer in this different needed behaviour loop we can go back to our original loop.
        }
    }
    while(!FA_ReadSwitch(0) || !FA_ReadSwitch(1) ){ //wait till one of the push buttons is pressed to exit.. 
        FA_LCDClear();
        char print_info[255]; //char array to store our print info....
        sprintf(print_info, "Finished exploring, we discovered %d shaded cells, press one of our 2 push buttons to exit program.",shaded_cells);
        FA_LCDPrint(print_info, sizeof(print_info), 0,0,FONT_NORMAL, LCD_TRANSPARENT);  
        play_star_wars_theme_tune(); //we've finished let's play a theme tune to show this....
    }

    //exit program since user pressed one of the 2 push buttons.
    return 0;
}


