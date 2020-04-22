/*
 * File:   main.c
 * Author: jac
 *
 * Created on 20 February 2020, 11:38
 */
#include "allcode_api.h"
#include "mario_theme_tune.h"
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
direction current_direction = north; //store our current facing direction
direction last_direction = north; //direction we came from)
int current_cell = 0; //current cell number
int last_cell = 0; //cell we just came from

int cells_explored = 0; //don't increase if we are revisiting same cell....
cell cells[25];
#define cell_length 150 //mm 


void fill_cell(){
    
    
    static bool first_run = true;
    /*
     *we've already been to this cell, therefore we don't need to fill out it's information
     * nor do we wish to increase our cells_explored variable used below so let's just 
     * return back to work. 
     */
    if(cells[current_cell].visited == true && first_run == true){
        first_run = false;
        return;
    }else if(cells[current_cell].visited == true){
        return;
    }
    
    cells_explored++; //we are visiting a new cell let's increase our cells explored counter.
    cells[current_cell].visited = true;

    //todo: figure out a actual reasonable value for if there is a wall there or not, 100 is probably not that value....?
    if(FA_ReadIR(front_ir) > 100){
        cells[current_cell].exits[north] = wall;
    }
    if(FA_ReadIR(right_ir) > 100){
        cells[current_cell].exits[east] = wall;
    }
    if(FA_ReadIR(back_ir) > 100){
        cells[current_cell].exits[south] = wall;
    }
    if(FA_ReadIR(left_ir) > 100){
        cells[current_cell].exits[west] = wall;
    }
    
    //to be worked on..... need to test sample values of light and decide what sort
    //of values are shaded and not...
    if(FA_ReadLight() < 50){
        cells[current_cell].lighting = shaded;
    }
}

//these 3 functions are dirty but do the job of keeping our direction correct, there is definitely a
//better way of doing this but atm it works and isn't over complicated will see what i can do to fix later?

void update_direction_left(){
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

void update_direction_right(){
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

void update_direction_backwards(){
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
    FA_RobotInit();
    FA_LCDBacklight(80);

    //keep exploring till we have filled out our maze map...
    while(cells_explored != 25){
        //first let's fill out what we can from our current cell and add this info to our map array.
        fill_cell();
        
        //lets follow the left wall.
        if(cells[current_cell].exits[west] != wall){
            FA_Left(90);
            FA_Forwards(cell_length / 2); 
            update_direction_left();
            last_cell = current_cell;
            current_cell++;
        }else if(cells[current_cell].exits[north] != wall){
            FA_Forwards(cell_length /2);
            //still going north no need to update direction...
            last_cell = current_cell;
            current_cell++;
        }else if(cells[current_cell].exits[east] != wall){
            FA_Right(90); 
            FA_Forwards(cell_length / 2);
            update_direction_right();
            last_cell = current_cell;
            current_cell++;
        }else{
            FA_Left(180);
            FA_Forwards(cell_length / 2);
            update_direction_backwards();
            last_cell = current_cell;
            current_cell--;
        }
        
        last_direction = current_direction;
   }

    return 0; // Actually, we should never get here...
}

