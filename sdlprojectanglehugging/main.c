#include "stdio.h"
#include "stdlib.h"
#include "SDL2/SDL.h"
#include "SDL2_gfx-1.0.1/SDL2_gfxPrimitives.h"
#include "time.h"

#include "formulas.h"
#include "wall.h"
#include "robot.h"

int done = 0;

int check = 0;
int turns[5] = {
    LEFT,
    RIGHT,
    RIGHT,
    RIGHT,
    LEFT};
int checkturn = -1;

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    robot.maze = CRAZY_MAZE;
    struct Wall_collection *head = NULL;
    int front_left_sensor, front_right_sensor, left_sensor, right_sensor, left_back_sensor, right_back_sensor=0;
    clock_t start_time, end_time;
    int msec;

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    // But you can use absolute positions. 10 is used as the width, but you can change this.
    //Code in main.c
    int key_num;
    switch (robot.maze) {
        case BASIC_MAZE:

        {
            insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
            insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
            insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
            insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2, 150, 10);
            insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
            insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
            insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 450, 10);
            insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 250, 10);
            insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
            insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
            insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-100, 10);
            insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);
            insertAndSetFirstWall(&head, 13, OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+220, 100, 10);
            break;
        }

        case COLINS_MAZE:
        {
            insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+175, 100, 10);
            insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2+100, 300, 10);
            insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+150, 10, OVERALL_WINDOW_HEIGHT/2-100);
            insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, 110);
            insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2+50, 150, 10);
            insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2, 250, 10);
            insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-90, 10, 200);
            insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+100, 100, 10);
            insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2, 10, 110);
            insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2, 100, 10);
            insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-50, 350, 10);
            insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2-150, 10, 105);
            insertAndSetFirstWall(&head, 13,  OVERALL_WINDOW_WIDTH/2+150, OVERALL_WINDOW_HEIGHT/2+150, 10, OVERALL_WINDOW_HEIGHT/2-75);
            insertAndSetFirstWall(&head, 14,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2-150, 200, 10);
            insertAndSetFirstWall(&head, 15,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-150, 10, 105);
            insertAndSetFirstWall(&head, 16,  OVERALL_WINDOW_WIDTH/2+300, OVERALL_WINDOW_HEIGHT/2-225, 10, 600);
            insertAndSetFirstWall(&head, 17,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-225, 550, 10);
            insertAndSetFirstWall(&head, 18,  OVERALL_WINDOW_WIDTH/2-75, OVERALL_WINDOW_HEIGHT/2-225, 10, 125);
            insertAndSetFirstWall(&head, 19,  OVERALL_WINDOW_WIDTH/2-125, OVERALL_WINDOW_HEIGHT/2-175, 10, 125);
            insertAndSetFirstWall(&head, 20,  OVERALL_WINDOW_WIDTH/2-175, OVERALL_WINDOW_HEIGHT/2-225, 10, 125);
            insertAndSetFirstWall(&head, 21,  OVERALL_WINDOW_WIDTH/2-225, OVERALL_WINDOW_HEIGHT/2-175, 10, 125);
            insertAndSetFirstWall(&head, 22,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-225, 10, 600);
            insertAndSetFirstWall(&head, 23,  OVERALL_WINDOW_WIDTH/2+50, OVERALL_WINDOW_HEIGHT/2-50, 10, 250);
            insertAndSetFirstWall(&head, 24,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2+235, 600, 10);
            break;
        }
        case ANGULAR_MAZE:
        {
            insertAndSetFirstWall(&head, 1,  0, 0, 12, 480);
            insertAndSetFirstWall(&head, 2,  80, 440, 12, 40);
            insertAndSetFirstWall(&head, 2,  80, 440, 560, 12);
            insertAndSetFirstWall(&head, 0,  0, 0, 640, 12);
            insertAndSetFirstWall(&head, 0,  300, 0, 12, 200);
            insertAndSetFirstWall(&head, 0,  400, 60, 12, 390);
            insertAndSetFirstWall(&head, 0,  400, 60, 60, 12);
            insertAndSetFirstWall(&head, 0,  628, 0, 12, 360);
            insertAndSetFirstWall(&head, 0,  628, 420, 12, 20);
            insertAndSetFirstWall(&head, 0,  560, 180, 70, 12);
            insertAndSetFirstWall(&head, 0,  540, 290, 12, 120);

            //horizontalwall(&head,Length,Width,Height,1 for down or 0 for up)
            horizontalwall(&head,100,0,280,1);
            horizontalwall(&head,210,80,220,1);
            horizontalwall(&head,100,80,220,0);
            horizontalwall(&head,80,0,0,1);
            horizontalwall(&head,50,280,320,0);
            horizontalwall(&head,130,400,200,0);
            horizontalwall(&head,100,460,280,0);
            horizontalwall(&head,80,540,350,1);
            break;
        }

        case CRAZY_MAZE:
        {

            key_num = insertAndSetDiagonalWall(&head, 1,  20, 0, 120, 0);

            // First corridor, getting smaller
            key_num = insertAndSetDiagonalWall(&head, 1,  20, 0, 50, 440);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  120, 0, 100, 380);

            // small path on bottom
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  50, 440, 200, 440);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  100, 380, 160, 380);

            // first zig zag
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  200, 440, 300, 300);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  160, 380, 220, 300);

            // second zig zag
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  300, 300, 200, 160);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  220, 300, 120, 160);

            // third zig zag
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  200, 160, 270, 70);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  120, 160, 220, 20);

            // long corridor on top
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  270, 70, 440, 70);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  220, 20, 500, 20);

            // top diamond, top
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  440, 70, 360, 190);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  500, 20, 620, 190);

            // top diamond, bottom
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  360, 190, 440, 290);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  620, 190, 550, 290);

            // bottom triangle
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  440, 290, 360, 390);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  550, 290, 550, 390);

            // top of bottom corridor
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  360, 390, 300, 390);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  550, 390, 650, 390);

            // side of bottom corridor
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  300, 390, 300, 440);

            // bottom of bottom corridor
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  300, 440, 650, 440);

            // middle diamond, top
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  420, 190, 490, 140);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  560, 190, 490, 140);

            // middle diamond, bottom
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  420, 190, 490, 240);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  560, 190, 490, 240);

            // right angle
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  430, 390, 490, 390);
            key_num = insertAndSetDiagonalWall(&head, key_num+1,  490, 290, 490, 390);
            break;
        }
   
        case Maze5:
        {
            int i;
            insertAndSetFirstWall(&head, 12,  120, 450, 10, 30);
            insertAndSetFirstWall(&head, 12,  220, 450, 10, 30);
            for (i = 0; i < 100; i++){
                insertAndSetFirstWall(&head, i,  20 + i , 350 + i, 10, 10); //1
                insertAndSetFirstWall(&head, i,  20 +100 + i , 350 + i, 10, 10); //1
            }
            insertAndSetFirstWall(&head, 12,  20, 280, 10, 70);
            insertAndSetFirstWall(&head, 12,  120, 280, 10, 70);
            for (i = 0; i < 180; i++){
                insertAndSetFirstWall(&head, i,  20 +190 - i/2 , 100 + i, 10, 10); //1
            }
            for (i = 0; i < 105; i++){
                insertAndSetFirstWall(&head, i,  20 +105/2 - i/2 , 175 + i, 10, 10); //1
            }
            insertAndSetFirstWall(&head, 2,  20, 175, 105/2, 10);
            insertAndSetFirstWall(&head, 2,  20, 20, 10, 155);
            insertAndSetFirstWall(&head, 2,  20, 20, 300, 10);
            insertAndSetFirstWall(&head, 2,  320, 20, 10, 60);
            insertAndSetFirstWall(&head, 2,  80, 100, 130, 10);
            insertAndSetFirstWall(&head, 2,  80, 80, 10, 20);
            insertAndSetFirstWall(&head, 2,  80, 80, 160, 10);

            double j;
            for (i = 0; i < 50; i++){
                j = i;
                insertAndSetFirstWall(&head, i+1,
                                    // the most important bit is below.
                                    // increase the 20 for a tighter bend
                                    // descrease for a more meandering flow
                                    320 + 30*sin(10*j * M_PI/180),
                                    // increase the 5 for a spacier curve
                                    (i * 5)+80,
                                    10, 10);
            }
            for (i = 0; i < 75; i++){
                j = i;
                insertAndSetFirstWall(&head, i+1,
                                    // the most important bit is below.
                                    // increase the 20 for a tighter bend
                                    // descrease for a more meandering flow
                                    240 + 30*sin(10*j * M_PI/180),
                                    // increase the 5 for a spacier curve
                                    (i * 5)+80,
                                    10, 10);
            }
            insertAndSetFirstWall(&head, 2,  345, 330, 105, 10);
            insertAndSetFirstWall(&head, 2,  450, 190, 10, 150);
            insertAndSetFirstWall(&head, 2,  380, 190, 70, 10);
            insertAndSetFirstWall(&head, 2,  380, 20, 10, 170);
            insertAndSetFirstWall(&head, 2,  380, 20, 260, 10);

            insertAndSetFirstWall(&head, 2,  255, 455, 345, 10);
            insertAndSetFirstWall(&head, 2,  600, 100, 10, 365);
            insertAndSetFirstWall(&head, 2,  530, 100, 70, 10);
            insertAndSetFirstWall(&head, 2,  530, 80, 10, 20);
            insertAndSetFirstWall(&head, 2,  530, 80, 110, 10);
            break;
        }

        case Maze6:
        {
            insertAndSetFirstWall(&head, 2,  220, 400, 10, 80);
            insertAndSetFirstWall(&head, 2,  20, 400, 200, 10);
            insertAndSetFirstWall(&head, 2,  20, 50, 10, 350);
            insertAndSetFirstWall(&head, 2,  20, 50, 280, 10);
            insertAndSetFirstWall(&head, 2,  300, 50, 10, 100);
            insertAndSetFirstWall(&head, 2,  300, 150, 110, 10);
            insertAndSetFirstWall(&head, 2,  400, 50, 10, 100);
            insertAndSetFirstWall(&head, 2,  400, 50, 220, 10);
            insertAndSetFirstWall(&head, 2,  620, 50, 10, 290);
            insertAndSetFirstWall(&head, 2,  620, 340, 20, 10);

            insertAndSetFirstWall(&head, 1,  320, 300, 10, 180);
            insertAndSetFirstWall(&head, 2,  120, 300, 200, 10);
            insertAndSetFirstWall(&head, 2,  120, 150, 10, 150);
            insertAndSetFirstWall(&head, 2,  120, 150, 80, 10);
            insertAndSetFirstWall(&head, 2,  200, 150, 10, 100);
            insertAndSetFirstWall(&head, 2,  200, 250, 310, 10);
            insertAndSetFirstWall(&head, 2,  500, 150, 10, 100);
            insertAndSetFirstWall(&head, 2,  500, 150, 10, 100);
            insertAndSetFirstWall(&head, 2,  500, 150, 20, 10);
            insertAndSetFirstWall(&head, 2,  520, 150, 10, 290);
            insertAndSetFirstWall(&head, 2,  520, 440, 120, 10);
        }
    }

    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;
    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1)
            robotAutoMotorMove(&robot, front_left_sensor, front_right_sensor, left_sensor, right_sensor, left_back_sensor, right_back_sensor);
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-225, 10, 100)){
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }
        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_sensor = checkRobotSensorFrontLeftAllWalls(&robot, head);
            if (front_left_sensor>0)
                printf("Getting close on the front left. Score = %d\n", front_left_sensor);

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            if (front_right_sensor>0)
                printf("Getting close on the front right. Score = %d\n", front_right_sensor);

            left_sensor = checkRobotSensorLeftAllWalls(&robot, head);
            if (left_sensor>0)
                printf("Getting close on the left. Score = %d\n", left_sensor);

            right_sensor = checkRobotSensorRightAllWalls(&robot, head);
            if (right_sensor>0)
                printf("Getting close on the right. Score = %d\n", right_sensor);

            left_back_sensor = checkRobotSensorLeftBackAllWalls(&robot, head);
            if (left_back_sensor>0)
                printf("Getting close on the left back. Score = %d\n", left_back_sensor);

            right_back_sensor = checkRobotSensorRightBackAllWalls(&robot, head);
            if (right_back_sensor>0)
                printf("Getting close on the right back. Score = %d\n", right_back_sensor);
        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                done = 1;
            }
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(100);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}
