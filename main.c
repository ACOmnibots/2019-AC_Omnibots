#include <kipr/botball.h>
#define Pi 3.14159
/* note
* a meter and motor maxspeeds example (test 0 result)
*
*	clear_motor_position_counter(1);
*   clear_motor_position_counter(2);
* while(get_motor_position_counter(1) < 11555 || get_motor_position_counter(2) < 9693)
* {
*		motor(1,100); //left
*       motor(2,84); //right
* }
*/

void lineFollow(int t);
void moveTo(double l, double r, int t);
void movements_from_percent_speed(double leftp, double rightp, int time);
void init (double xx, double yy, double facing_anglee, double outerRR, double fbwheeld);
void setSpeed(double max); // in mm
void setlrPercent(double lp, double rp);
void setMTCPM(int l, int r);
double absv(double a);

double lMax, rMax, maxSpeed,x, y, turning_angle, facing_angle, angularspeed, outerR, innerR, fbwheeldis; //outerR = front wheels distance
double maxangle = 2 * Pi, minangle= -2 * Pi, maxangled = 180, minangled = -180;

int  M1TCPM = 0, M2TCPM = 0;

int main()
{
    //init
    init(0.0,0.0,0.0,130,220);
    setlrPercent(100,84);
    setSpeed(45);
    setMTCPM(11555, 9693);
	//
    
    moveTo(5.0, 10.0, 1000);

    printf("%lf \n",x);
    printf("%lf \n",y);
    printf("%lf \n",turning_angle);
    printf("%lf \n",facing_angle);
    printf("%lf \n",angularspeed);
    printf("%lf \n",outerR);
    printf("%lf \n",innerR);
    printf("%lf \n",maxSpeed);

    return 0;
}

void init (double xx, double yy, double facing_anglee, double outerRR, double fbwheeld){
    x = xx;
    y = yy;
    facing_angle = facing_anglee;
    outerR = outerRR;
    fbwheeldis = fbwheeld;
}

double absv(double a){
    if(a >= 0){
        return a;
    }
    else{
        return -a;
    }
}

void setMTCPM(int l, int r){
    M1TCPM = l; //motor 1 tics count per meter at max speed
    M2TCPM = r;	//motor 2 tics count per meter at max speed
}

void setSpeed(double max){
    maxSpeed = max;
}

void setlrPercent(double lp, double rp){
    lMax = lp;
    rMax = rp;
}

void moveTo(double l, double r, int t){
    motor(1, lMax * l / 100);
    motor(3, rMax * r / 100);
    msleep(t);
    movements_from_percent_speed(maxSpeed * l / 100, maxSpeed * r / 100, t);
}

void lineFollow(int t){
    int count = 0;
    while(count < t){
        if(analog(5) > 1400){
            moveTo(60,80,10);
            printf("moving straight, a bit to right\n");
        }
        else{
            moveTo(30,-30,10);
            printf("turning left\n");
        }
        count++;
    }
}

void movements_from_percent_speed(double leftp, double rightp, int time){
    double temp;
    printf("%lf and %lf \n",leftp, rightp);
    if(leftp != rightp){
        angularspeed = (leftp - rightp) / outerR;
        if(leftp > rightp){
            innerR = rightp / absv(angularspeed);
        }
        else{
            innerR = leftp / absv(angularspeed);
        }
    }
    else{
        angularspeed = 0;
        innerR = 0;
    }

    turning_angle = angularspeed * (time / 1000.0);    //NOTE:USING RADIANS

    x = x - (innerR + 0.5 * outerR)*(1 - cos(turning_angle)); //1 //lack going straight, change turning_angle to facing_angle?
    y = y + (innerR + 0.5 * outerR) * sin(turning_angle);//2 //lack going straight, change turning_angle to facing_angle?

    turning_angle = x / (2 * Pi) * 360; //NOTE: USING DEGREES //3

    temp = facing_angle + turning_angle; //4

    if(fmod(temp, maxangle) == 0 && fmod( (temp / maxangle), 2) == 1){ 	////////////
        facing_angle = 180;												//////////
    }																	/////////
    else{																////////
        facing_angle = fmod(temp,maxangle);								/////////
    }																	//////////
}	
///////////
