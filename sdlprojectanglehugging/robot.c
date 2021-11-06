#include "robot.h"

int preState1= 0, preState2 =0, preState3 =0, preState4 =0, preState5 =0, preState6 =0, preState7 =0, preState8 =0, 
preState9 =0, preState10 =0, preState11 =0, preState12 =0, preState13 =0,  preState14 =0;

int wall_follower_status = 0; // =1 leftwall, = 2 rightwall
int giu_khoang_cach = 0;
void setup_robot(struct Robot *robot){
    robot->angle = 0;
    switch (robot->maze) {
        case BASIC_MAZE:
        {
            robot->x = OVERALL_WINDOW_WIDTH/2-50;
            robot->y = OVERALL_WINDOW_HEIGHT-50;
            robot->true_x = OVERALL_WINDOW_WIDTH/2-50;
            robot->true_y = OVERALL_WINDOW_HEIGHT-50;
            break;
        }


        case COLINS_MAZE:
        {
            robot->x = OVERALL_WINDOW_WIDTH/2;
            robot->y = OVERALL_WINDOW_HEIGHT-50;
            robot->true_x = OVERALL_WINDOW_WIDTH/2;
            robot->true_y = OVERALL_WINDOW_HEIGHT-50;
            break;
        }


        case ANGULAR_MAZE:
        {
            robot->x = OVERALL_WINDOW_WIDTH/2-280;
            robot->y = OVERALL_WINDOW_HEIGHT-50;
            robot->true_x = OVERALL_WINDOW_WIDTH/2-280;
            robot->true_y = OVERALL_WINDOW_HEIGHT-50;
            break;
        }


        case CRAZY_MAZE:
        {
            robot->x = 60;
            robot->y = 20;
            robot->true_x = 60;
            robot->true_y = 20;
            robot->angle += 180;
            break;
        }
        case Maze5:
        {
            robot->x = 170;
            robot->y = 460;
            robot->true_x = 170;
            robot->true_y = 460;
            robot->width = ROBOT_WIDTH;
            robot->height = ROBOT_HEIGHT;
            robot->direction = 0;
            robot->angle = 0;
            robot->currentSpeed = 0;
            robot->crashed = 0;
            robot->auto_mode = 0;
            break;
        }
        case Maze6:
        {
            robot->x = 270;
            robot->y = 460;
            robot->true_x = 270;
            robot->true_y = 460;
            robot->width = ROBOT_WIDTH;
            robot->height = ROBOT_HEIGHT;
            robot->direction = 0;
            robot->angle = 0;
            robot->currentSpeed = 0;
            robot->crashed = 0;
            robot->auto_mode = 0;
            printf("Press arrow keys to move manually, or enter to move automatically\n\n");
            break;
        }


    }
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;
    robot->state = SEARCHING;
    robot->targetAngle = 0;
    robot->oldState = SEARCHING;
    robot->following = 0;
    robot->decelDelay = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int angle = robot->angle - 90;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int angle = robot->angle + 90;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorLeftBackAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int angle = robot->angle - 90;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorRightBackAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int angle = robot->angle + 90;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    */
    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    int xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    int yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    //Front Right Sensor
    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int i;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    int angle = robot->angle - 90;

    // Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Right Sensor
    angle = robot->angle + 90;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Left Back Sensor
    angle = robot->angle - 90;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Right Back Sensor
    angle = robot->angle + 90;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

}



void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > MAX_ROBOT_SPEED)
                robot->currentSpeed = MAX_ROBOT_SPEED;
            break;
        case DOWN :
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                robot->currentSpeed = -MAX_ROBOT_SPEED;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            break;
        case REVERSE:
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE+ 180 )%360;
            break;
        
    }
    robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}


void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int front_right_sensor, int left_sensor, int right_sensor,
                        int left_back_sensor, int right_back_sensor) {

    int for_front, away_front, for_main, away_main, for_back, away_back=0;

    //printf("---new cycle---\n");
    printf("front_left_sensor: %d, front_right_sensor: %d, left_sensor: %d, right_sensor: %d, left_back_sensor: %d, right_back_sensor: %d\n", 
             front_left_sensor, front_right_sensor, left_sensor, right_sensor, left_back_sensor, right_back_sensor);


    // implement via wall follower algorithm

    int maxvalueSensor = getMaxValueSensor( front_left_sensor, front_right_sensor, left_sensor, right_sensor, left_back_sensor, right_back_sensor);
    printf("maxvalueSensor = %d\n", maxvalueSensor);
    // tang toc ban dau

    if( robot->currentSpeed <=1){
         robot->currentSpeed = 1;
    }
    if (robot->currentSpeed < SOFT_SPEED_LIMIT && giu_khoang_cach == 0) {
            printf("tang toc case 1\n");
            //robot->direction = UP;
            robot->currentSpeed ++;
    }

    // xác định bên tường đâu tiên chạm vào: 

    switch (wall_follower_status)
    {

    case 0: // chua cham tuong lan nao
        {
            // cham tuong trai lan dau
            if ((maxvalueSensor == front_left_sensor && maxvalueSensor > front_right_sensor)
                || (maxvalueSensor == left_sensor && maxvalueSensor > right_sensor)) 
            {
                printf("cham tuong trai\n");
                wall_follower_status = 1;
                robot->direction = LEFT;
            }

            // cham tuong phai lan dau
            else if ((maxvalueSensor == front_right_sensor && maxvalueSensor > front_left_sensor)
                || (maxvalueSensor == right_sensor  && maxvalueSensor > left_sensor)) 
            {
                /* code */
                printf("cham tuong phai\n");
                wall_follower_status = 2;
                robot->direction = RIGHT;
            }
            else
            {
                // if(maxvalueSensor == front_right_sensor && maxvalueSensor == front_left_sensor &&  front_left_sensor >0)
                // {
                //     printf("chon re phai \n");
                //     robot->direction = RIGHT;
                //     robot->currentSpeed = 0;
                // }
                //else
                {
                    printf("di thang \n");
                    robot->direction = RIGHT;

                }

            }
            break; 
        }

    case 1: // cham tuong bên trai dau tien, bam tuong ben trai
        /* code */
        {

            int overValue = 2; 
            if(right_sensor > 0)
            {
                overValue = right_sensor;
            }

            printf("bam tuong trai \n");
            if(front_left_sensor >0 && front_left_sensor<=overValue && left_sensor >0 && left_sensor <= overValue)
            {
                {
                    robot->direction = LEFT;
                    printf("bam tuong trai, di ziczac \n");
                }
                giu_khoang_cach = 0;

            }
            else
            {
                printf("giu khoang cach \n");
                robot->direction = RIGHT;
                giu_khoang_cach = 1;
            }
        

            break;
        }
    case 2: // cham tuong bên phai dau tien, bam tuong ben phai
        /* code */
        {
            printf("bam tuong phai \n");
            int overValue = 2; 
            // if(left_sensor > 0)
            // {
            //     overValue = left_sensor;
            // }

            if(front_right_sensor >=0 && front_right_sensor<=overValue && right_sensor >=0 && right_sensor <= overValue)
            {
                {
                    printf("bam tuong phai \n");
                    robot->direction = RIGHT ;
                }

                giu_khoang_cach = 0;
            }
            else
            {
                printf("giu khoang cach \n");
                robot->direction = LEFT;
                robot->currentSpeed --;
                giu_khoang_cach = 1;
            }

            break;
        }
    
    default:
        break;
    } 
    
    if (robot->direction != UP && robot->direction != DOWN)
    {   
        preState14 = preState13;
        preState13 = preState12;
        preState12= preState11;
        preState11 = preState10;
        preState10 = preState9;
        preState9 = preState8;
        preState8 = preState7;
        preState7 = preState6;
        preState6 = preState5;
        preState5 = preState4;
        preState4 = preState3;
        preState3 = preState2;
        preState2 = preState1;
        preState1 = robot->direction;
    }
    

}

int getMaxValueSensor( int front_left_sensor, int front_right_sensor, int left_sensor, int right_sensor,
                        int left_back_sensor, int right_back_sensor)
    {
        int max = 0;
        if (max < front_left_sensor)
        {
            max = front_left_sensor;
        }
        if (max < front_right_sensor)
        {
            max = front_right_sensor;
        }
        if (max < left_sensor)
        {
            max = left_sensor;
        }
        if (max < right_sensor)
        {
            max = right_sensor;
        }
        if (max < left_back_sensor)
        {
            max = left_back_sensor;
        }
        if (max < right_back_sensor)
        {
            max = right_back_sensor;
        }
        return max;
    }
// void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int front_right_sensor, int left_sensor, int right_sensor,
//                         int left_back_sensor, int right_back_sensor) 
//                         {

//     int for_front, away_front, for_main, away_main, for_back, away_back=0;

//     if (robot->following == LEFT) {
//         for_front = front_left_sensor;
//         away_front = front_right_sensor;
//         for_main = left_sensor;
//         away_main = right_sensor;
//         for_back = left_back_sensor;
//         away_back = right_back_sensor;
//     } else {
//         for_front = front_right_sensor;
//         away_front = front_left_sensor;
//         for_main = right_sensor;
//         away_main = left_sensor;
//         for_back = right_back_sensor;
//         away_back = left_back_sensor;
//     }

//     //printf("---new cycle---\n");
//     printf("front_left_sensor: %d, front_right_sensor: %d, left_sensor: %d, right_sensor: %d, left_back_sensor: %d, right_back_sensor: %d\n", front_left_sensor, front_right_sensor, 
//     right_sensor, right_sensor, left_back_sensor, right_back_sensor);

//     switch (robot->state) {

//     case SEARCHING:
//         printf("searching\n");
//         if ((front_left_sensor > 0) || (front_right_sensor > 0)) {
//             if (robot->currentSpeed > 0) {
//                 printf("searching case 1\n");
//                 robot->direction = DOWN;
//             } else {
//                 printf("searching case 2\n");
//                 robot->following = LEFT;
//                 robot->state = TURN_AWAY;
//                 robot->targetAngle = 90;
//             }
//         } else if ((left_sensor > 0) || (left_back_sensor > 0)) {
//             printf("searching case 3\n");
//             robot->following = LEFT;
//             robot->state = FOLLOWING;
//         } else if ((right_sensor > 0) || (right_back_sensor > 0)) {
//             printf("searching case 4\n");
//             robot->following = RIGHT;
//             robot->state = FOLLOWING;
//         } else if (robot->currentSpeed < SOFT_SPEED_LIMIT) {
//             printf("searching case 5\n");
//             robot->direction = UP;
//         }
//         break;

//     case FOLLOWING:
//         printf("following\n");
//         //printf("speed is %d\n", robot->currentSpeed);
//         if (((for_main == 0) && (for_back == 0)) || (for_front > 0) || (away_front > 0)) {
//             //printf("decelerating\n");
//             printf("following case 1\n");
//             robot->direction = DOWN;
//             robot->state = DECELERATE;
//         } else if (robot->currentSpeed < SOFT_SPEED_LIMIT) {
//             if(left_sensor>0 && right_sensor>0 && right_back_sensor>0 && left_back_sensor == 0)
//             {
//                 printf("following case 2\n");
//                 //robot->targetAngle -= DEFAULT_ANGLE_CHANGE;
//                 robot->direction = LEFT;
//                 //robot->direction = UP;
//             }
//             else
//             {
//                 printf("following case 3\n");
//                 //robot->targetAngle += DEFAULT_ANGLE_CHANGE;
//                 robot->direction = RIGHT;
//                 robot->direction = UP;

//             }
//             //robot->state = SEARCHING;

//         }// else if ((for_back > for_main) && (for_main > 0) && (for_back > 0)) {
//          //   robot->direction = robot->following;
//         //}
//         break;

//     case TURN_FORW:
//         printf("turning forward\n");
//         if (robot->targetAngle <= 0) {
//             if ((for_front > 0) || (away_front > 0)) {
//                 printf("turning forward case 1\n");
//                 robot->state = DECELERATE;
//             } if (robot->currentSpeed < SOFT_SPEED_LIMIT) {
//                 printf("turning forward case 2\n");
//                 robot->direction = UP;
//             } else if (for_main > 0) {
//                 printf("turning forward case 3\n");
//                 robot->state = FOLLOWING;
//             }
//         } else {
//             //printf("turning\n");
//             printf("turning forward case 4\n");
//             robot->direction = robot->following;
//             robot->targetAngle -= DEFAULT_ANGLE_CHANGE;
//         }
//         break;

//     case TURN_AWAY:
//         printf("turning away\n");
//         if (robot->targetAngle <= 0) {
//             printf("turning away case 1\n");
//             robot->state = FOLLOWING;
//         } else {
//              robot->targetAngle -= DEFAULT_ANGLE_CHANGE;
//             if (robot->following == LEFT) {
//                 printf("turning away case 2\n");
//                 robot->direction = RIGHT;
//             } else {
//                 printf("turning away case 3\n");
//                 robot->direction = LEFT;
//             }
           
//         }
//         break;

//     case DECELERATE:
//         printf("DECELERATE\n");
//         if (robot->decelDelay > 0) {
//             robot->decelDelay--;
//             if (robot->decelDelay == 0) {
//                 printf("DECELERATE case 1\n");
//                 robot->direction = DOWN;
//             }
//         } else if (robot->currentSpeed > 1) {
//             printf("DECELERATE case 2\n");
//             robot->direction = DOWN;
//         } else if (robot->currentSpeed > 0) {
//             if ((for_front > 3) || (away_front > 3)) {
//                 printf("DECELERATE case 3\n");
//                 robot->direction = DOWN;
//             } else if ((for_main == 0) && (for_back == 0)) {
//                 printf("DECELERATE case 4\n");
//                 robot->decelDelay = 2;
//             }
//         } else if ((for_main == 0) && (for_back == 0)) {
//             printf("DECELERATE case 5\n");
//             robot->state = TURN_FORW;
//             robot->targetAngle = 90;
//         } else {
//             printf("DECELERATE case 6\n");
//             robot->state = TURN_AWAY;
//             robot->targetAngle = (away_front - for_front) * 15 + 90;
//             printf("Angle is %d\n", robot->targetAngle);
//         }
//         break;

//     }

// }