#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define REVERSE 5

#define OVERALL_WINDOW_WIDTH 640
#define OVERALL_WINDOW_HEIGHT 480

#define ROBOT_WIDTH 25
#define ROBOT_HEIGHT 20

#define WALL_WIDTH 10

#define DEFAULT_ANGLE_CHANGE 3
#define DEFAULT_SPEED_CHANGE 1
#define MAX_ROBOT_SPEED 25
#define PI 3.14159265
#define SENSOR_VISION 30

#define SEARCHING 1
#define FOLLOWING 2
#define TURN_FORW 3
#define TURN_AWAY 4
#define DECELERATE 5

#define BASIC_MAZE 1
#define COLINS_MAZE 2
#define ANGULAR_MAZE 3
#define CRAZY_MAZE 4
#define Maze5 5
#define Maze6 6

#define SOFT_SPEED_LIMIT 3

struct Wall {
    int x,y;
    int width, height;
};

struct Wall_collection {
    int key;
    struct Wall wall;
    struct Wall_collection *next;
};

struct Robot {
    int x, y;
    double true_x, true_y;
    int direction;
    int angle;
    int currentSpeed;
    int width, height;
    int crashed;
    int auto_mode;
    int state;
    int following;
    int targetAngle;
    int oldState;
    int decelDelay;
    int maze;
};

#endif // STRUCTURES_H_INCLUDED
