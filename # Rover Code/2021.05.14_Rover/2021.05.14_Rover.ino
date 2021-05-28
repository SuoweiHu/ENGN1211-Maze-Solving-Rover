#include <Servo.h>
#include <math.h>
#include "_config_pin.h"
#include "_config_motor.h"
#include "_config_timing.h"
#include "_notations.h"

Servo myservo;

void setup() {
    Serial.begin(9600); // Open Serial connection

    pinMode(PIN_A_MOTOR_L, OUTPUT); // Setup Left Motor
    pinMode(PIN_B_MOTOR_L, OUTPUT); // ================
    pinMode(PIN_EN_MOTOR_L, OUTPUT);// ================
    pinMode(PIN_A_MOTOR_R, OUTPUT); // Setup Right Motor
    pinMode(PIN_B_MOTOR_R, OUTPUT); // ================
    pinMode(PIN_EN_MOTOR_R, OUTPUT);// ================
    pinMode(PIN_TRIGGER, OUTPUT);   // Setup Sensor 
    pinMode(PIN_ECHO, INPUT);       // ================
    myservo.attach(PIN_SERVO);      // Setup Servo Motor

    // Sweep servo and signal rover is ready
    // myservo.write(SM_CENTRE); delay(1000);      
    myservo.write(SM_LEFT);   delay(1000);
    myservo.write(SM_RIGHT);  delay(1000);
    myservo.write(SM_CENTRE); delay(1000);      
    println("Rover is ready to go ....");
}

void loop() {
    // while(true){make_leftTurn(); delay(1000);}
    // while(true){make_rightTurn(); delay(1000);}
    // while(true){myservo.write(SM_MID_L-23); double dist = read_dist(); println(String(dist));}

    forward();
    double dist = read_dist();
    double dist_l = 0;
    double dist_r = 0;
    double dist_c = 0;
    // println(String(dist));
    if(dist < 7.5){
        stop();
        delay(500);

        int wallType = check_frontWallType();

        stop();
        delay(500);
        

        switch (wallType){
            case NORMAL_WALL:
                // go back to the original position
                forward();
                delay(BACK_OFF+200);
                stop();

                myservo.write(SM_LEFT);delay(500);
                dist_l = read_dist();
                myservo.write(SM_RIGHT);delay(500);
                dist_r = read_dist();
                myservo.write(SM_CENTRE);delay(500);
                dist_c = read_dist();

                println(String(dist_l));
                println(String(dist_c));
                println(String(dist_r));
                println("");

                if(dist_c >= dist_l && dist_c >= dist_r){stop();}
                if(dist_r>dist_l){make_rightTurn();}
                else if(dist_l>dist_r){make_leftTurn(); }
                else{make_rightTurn();}
                break;

            case FORTYFIVE_WALL_ON_LEFT:
                // go back to the original position
                forward();
                delay(BACK_OFF-300);
                stop();

                // while(true){println("FORTYFIVE_WALL_ON_LEFT");stop();}
                make_fortyFive_rightTurn();
                break;

            case FORTYFIVE_WALL_ON_RIGHT:
                // go back to the original position
                forward();
                delay(BACK_OFF-300);
                stop();

                // while(true){println("FORTYFIVE_WALL_ON_RIGHT");stop();}
                make_fortyFive_leftTurn();
                break;
        }

        delay(2000);
    }
}

void print  (String _str_){Serial.print  (_str_);}
void println(String _str_){Serial.println(_str_);}

void stop(){
    analogWrite(PIN_EN_MOTOR_L, 0); digitalWrite(PIN_A_MOTOR_L, LOW); digitalWrite(PIN_B_MOTOR_L, LOW); // L-Motor
    analogWrite(PIN_EN_MOTOR_R, 0); digitalWrite(PIN_A_MOTOR_R, LOW); digitalWrite(PIN_B_MOTOR_R, LOW); // R-Motor
}
void forward(){
    analogWrite(PIN_EN_MOTOR_L, ML_SPEED); digitalWrite(PIN_A_MOTOR_L, HIGH); digitalWrite(PIN_B_MOTOR_L, LOW); // L-Motor
    analogWrite(PIN_EN_MOTOR_R, MR_SPEED); digitalWrite(PIN_A_MOTOR_R, HIGH); digitalWrite(PIN_B_MOTOR_R, LOW); // R-Motor
}
void backward(){
    analogWrite(PIN_EN_MOTOR_L, ML_SPEED); digitalWrite(PIN_A_MOTOR_L, LOW); digitalWrite(PIN_B_MOTOR_L, HIGH); // L-Motor
    analogWrite(PIN_EN_MOTOR_R, MR_SPEED); digitalWrite(PIN_A_MOTOR_R, LOW); digitalWrite(PIN_B_MOTOR_R, HIGH); // R-Motor
}
void left(){
    // analogWrite(PIN_EN_MOTOR_L, 0);        digitalWrite(PIN_A_MOTOR_L, HIGH); digitalWrite(PIN_B_MOTOR_L, LOW); // L-Motor
    analogWrite(PIN_EN_MOTOR_L, ML_SPEED); digitalWrite(PIN_A_MOTOR_L, LOW); digitalWrite(PIN_B_MOTOR_L, HIGH); // L-Motor
    analogWrite(PIN_EN_MOTOR_R, MR_SPEED); digitalWrite(PIN_A_MOTOR_R, HIGH); digitalWrite(PIN_B_MOTOR_R, LOW); // R-Motor
}
void right(){
    analogWrite(PIN_EN_MOTOR_L, ML_SPEED); digitalWrite(PIN_A_MOTOR_L, HIGH); digitalWrite(PIN_B_MOTOR_L, LOW); // L-Motor
    analogWrite(PIN_EN_MOTOR_R, MR_SPEED); digitalWrite(PIN_A_MOTOR_R, LOW); digitalWrite(PIN_B_MOTOR_R, HIGH); // R-Motor
    // analogWrite(PIN_EN_MOTOR_R, 0);        digitalWrite(PIN_A_MOTOR_R, HIGH); digitalWrite(PIN_B_MOTOR_R, LOW); // R-Motor
}
void make_leftTurn() {left(); delay(L_TURN_DELAY_LEFT); stop();}
void make_rightTurn(){right();delay(L_TURN_DELAY_RIGHT);stop();}
void make_fortyFive_leftTurn() {left(); delay(L_TURN_DELAY_HALF_L); stop();}
void make_fortyFive_rightTurn(){right();delay(L_TURN_DELAY_HALF_R); stop();}
void make_uTurn()    {left(); delay(U_TURN_DELAY);      stop();}
void test_motor(){
    // Put this in "LOOP" to test the motors
    forward();delay(1000);
    // stop();delay(500);
    // backward();delay(1000);
    // stop();delay(500);
    // left();delay(1000);
    // stop();delay(500);
    // right();delay(1000);
    // stop();delay(500);
    // right();delay(1000);
    // stop();delay(500);
    // left();delay(1000);
    // stop();delay(500);
}

void sort_longArray(long input_array[], int size)
{
    // (This is implmented using "Bubble Sort")
    
    //swapping region from 0-(n-1) narrow to 0-0
    for (int i = 0; i < (size - 1); i++) 
    {
        //iterate over the element 
        for (int o = 0; o < (size - (i + 1)); o++) 
        {
            //swap element with their next one until a number that is bigger is found
            if (input_array[o] > input_array[o + 1]) 
            {
                int t = input_array[o];
                input_array[o] = input_array[o + 1];
                input_array[o + 1] = t;
            }
        }
    }
}
void sort_doubleArray(double input_array[], int size)
{
    // (This is implmented using "Bubble Sort")

    //swapping region from 0-(n-1) narrow to 0-0
    for (int i = 0; i < (size - 1); i++)
    {
        //iterate over the element 
        for (int o = 0; o < (size - (i + 1)); o++)
        {
            //swap element with their next one until a number that is bigger is found
            if (input_array[o] > input_array[o + 1])
            {
                int t = input_array[o];
                input_array[o] = input_array[o + 1];
                input_array[o + 1] = t;
            }
        }
    }
}
double read_dist(){
    bool debug_sonar = false;    // Print debug message while reading ?
    long duration      = 0;     // The duration before reflection wave's arrival (sonar sensor)
    double distance    = 0;     // The measured distance via equation "distance = (duration * .0343) / 2" (sonar sensor)
    int interval_of_read = 10;                   // Delay for xxx micro-seconds between reads
    int number_of_read = 7;                      // The outcome is printed by the average or yyy read
    int median_index = number_of_read / 2;       // The index of the median item in data's sorted array
    double distance_s[number_of_read];           // Initailize sum for recording individual reads

    for (int i = 1; i <= number_of_read; i++)
    {
        // Make a read and set global varibale "duration" and "distance"
        digitalWrite(PIN_TRIGGER, LOW);
        delayMicroseconds(2);
        digitalWrite(PIN_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(PIN_TRIGGER, LOW);
        duration = pulseIn(PIN_ECHO, HIGH);
        distance = duration * 0.034 / 2;

        // Record current read of duration & distance
        distance_s[i - 1] = distance;
        if (debug_sonar){Serial.println(String("[SONAR_DEBUG] [Read-") + i + String("] distance: ") + distance + String(" centimeter;"));}                        //DEBUG PRINT
        
        // Delay before next ultrasonic reading
        delayMicroseconds(interval_of_read); 
    }

    sort_doubleArray(distance_s, number_of_read);      // Sort the arrays
    double median_distance = distance_s[median_index]; // Find the medians

    distance = median_distance;
    if(distance<3.00){distance=3.00;}
    if(distance>20.00){distance=20.00;}

    //DEBUG PRINT
    if (debug_sonar){Serial.println(String("[SONAR_DEBUG] [Median]") + String(" distance: ") + distance + String(" centimeter;"));Serial.println();}
    
    return distance;
}

int check_frontWallType(){

    // reverse back a little
    backward();
    delay(BACK_OFF);
    stop();
    
    // gather sensor data
    myservo.write(SM_MID_L);delay(500);
    double test_l = read_dist();
    myservo.write(SM_MID_R);delay(500);
    double test_r = read_dist();
    myservo.write(SM_CENTRE);delay(500);
    double test_c = read_dist();

    // check wall type
    int wallType = NORMAL_WALL;
    if(test_l<test_c){
        wallType = FORTYFIVE_WALL_ON_LEFT;
        // println("encountered 45 wall on the left side");

    }
    else if(test_r<test_c){
        wallType = FORTYFIVE_WALL_ON_RIGHT;
        // println("encountered 45 wall on the right side");
    }

    
    return wallType;
}
