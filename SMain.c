#include <kipr/botball.h>
void reset();
void rest(int t);
void AGP(int port, int h); //arm ground position
void CP(int ch, int p); //claw position
void CC(int ch);//Close claw to position 0
void moveTo(double lSpeedPercent, double rSpeedPercent, int t); // robot move for time t; lSpeedPercent: left wheel speed percent; rSpeedPercent: right wheel speed percent.
void move(double lSpeedPercent, double rSpeedPercent); // robot move
void moveToD(double lSpeedPercent, double rSpeedPercent, int position); // robot move to left wheel position(left motor position counter)
void moveToDT(double lSpeedPercent, double rSpeedPercent, int position); // robot move to right wheel position(right motor position counter)
void moveBDWhenGrab(double lSpeedPercent, double rSpeedPercent, int position, int port, int h); // robot move back while grabing
void turnLeft(double degree, int onTape); // turn left degree based on left motor position counter to turn 360 degree, different between on tape and off tape
void turnRight(double degree, int onTape); // turn right degree based on left motor position counter to turn 360 degree, different between on tape and off tape
void grabContainer(); // steps to grab enemy container
void singleWheel(int p, int degree); // turn on single wheel
void lineFollowBluW(int t); // linefollow on blue tape
void FBlue(int position, int a2min, int a2max, int a1min, int a1max); //fix position and move forward while pushing blue poms
void pushPoms(); //steps to push blue poms
void cli(); // calibrate position by use distance sensor sensing skyscrapers

/************************************************************************
*
* DO NOT DELETE VARIABLES AND METHODS, ASK LEO OR JASON IF CHANGES REQUIRED
*
*************************************************************************/

//singleWheel(1, 50, 12150);
//singleWheel(1, 80, 12080);

double lMax = 100, rMax = 100; //left, right max speed while moving straight
int armMaxHeight = 900, armGroundHeight = 1820, armTunnalHeight = 1380, armPomHeight = 1485, armCubeHeight = 1620, 
armRaisePomHeight = 1400, armContainerRaiseHeight = 950, armContainerTowHeight = 1245, armGContainerHeight = 1760;
int clawPomPosition = 240, clawGCubePosition = 100, clawRCubePosition = 110, clawMaxPosition = 1800, clawGrabOpenPosition = 1200,
clawClosePosition = 0, clawContainerPosition = 700, onTape360_n50p50 = 4720, offTape360_n50p50 = 4720, onTape360_n10p10 = 5785, 
offTape360_n10p10 = 5730, onTape360_n20p20 = 5735, offTape360_n20p20 = 5680, offTape360_n50p50r = 4720, offTape360_n80p80r = 12080; // GCube = green cube, RCube = red cube
int a1min = 2880, a1max = 2920, a2min = 2880, a2max = 2920;

//moveToD(100, 98.94, 5000);
//moveToD(-100, -98.43, 5000);
int main()
{
    enable_servos();
    //moveToDT(50,-50,-5600);
    //printf("%d\n",get_motor_position_counter(3));
    AGP(3,armMaxHeight);
    //CP(2, clawMaxPosition);
    //CC(2);
    //grabContainer();
    //pushPoms();
    disable_servo(3);
    ao();
    return 0;
}



void grabContainer()
{
    //CP(2, clawContainerPosition);
    CC(2);
    enable_servo(3);
    AGP(3, armMaxHeight);
    moveToD(-100,-100,-3300);
    turnRight(50, 1);
    while(digital(1) == 0){
        move(-100,-100);
    }
    moveToD(100,100,325);
    //moveToD(80,80,1000);
    turnLeft(164, 1);
    //turnLeft(90, 1);
    //singleWheel(1,220);
    moveToD(100,100,2600);

    //AGP(3, armMaxHeight);

    turnLeft(49,1);
    AGP(3, armContainerTowHeight);
    //turnRight(57,1);
    turnRight(45, 1);
    turnLeft(10,1);
    moveToD(-100,-100,-1000);
    moveToD(-30,-30,-200);
    //moveTo(-30,-30,1200);
    AGP(3, armMaxHeight);
    //turnLeft(15, 1);
    CP(2, clawMaxPosition);
    AGP(3, armPomHeight);
    moveToD(32,30, 2500);
    AGP(3,armGContainerHeight);
    CP(2,clawContainerPosition);
    AGP(3, armPomHeight);
    rest(100);
    AGP(3,armMaxHeight);
    moveToD(-80,-80,-2700);

    printf("a1: %d; a2: %d\n", analog(1), analog(2));
}

void pushPoms () {
    moveToD(100,100,400);
    turnRight( 45, 1);
    rest(200);
    moveToD(100, 100, 900);
    turnRight(27, 1);
    //moveToD(100, 100, 5700);
    printf("a1: %d; a2: %d\n", analog(1), analog(2));
    //turnLeft(2,1);
    moveToD(100,99,9000);
    moveToD(70,69.9, 3000);
    moveToD(100, 97, 1000);
    moveToD(100, 99, 5000);
    /*
    cli();
    moveToD(100, 100, 6000);
    cli();
    moveToD(100, 100, 6000);
    cli();
    moveToD(100, 100, 7000);
    */
}

void lineFollowBluW (int t) {
    int whileTimes = 0;
    reset();
    while (whileTimes <= t)
    {
        if (analog(0) < 150)
        {
            moveToD(30, 100, 60);

            printf("\n Following...%d \n", analog(0));
        }
        else
        {
            moveToD(100, 30, 60);
            printf("Untracked, fixing...%d \n", analog(0));
        }
        whileTimes += 1;
    }
    rest(100);
}

void moveTo(double lSpeedPercent, double rSpeedPercent, int t)
{
    reset();
    move(lSpeedPercent,rSpeedPercent);
    msleep(t);
}

void move(double lSpeedPercent, double rSpeedPercent)
{
    motor(1, lMax * lSpeedPercent / 100);
    motor(3, rMax * rSpeedPercent / 100);
}

void moveToDT(double lSpeedPercent, double rSpeedPercent, int position)
{
    reset();
    cmpc(1);
    cmpc(3);
    if(position >= 0)
    {
        while(get_motor_position_counter(3) < position)
        {
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
        }
    }
    else
    {
        while(get_motor_position_counter(3) > position)
        {
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
        }
    }
}

void moveToD(double lSpeedPercent, double rSpeedPercent, int position)
{
    reset();
    cmpc(1);
    cmpc(3);
    if(position >= 0)
    {
        while(get_motor_position_counter(1) < position)
        {
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
        }
    }
    else
    {
        while(get_motor_position_counter(1) > position)
        {
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
        }
    }
}

void reset()
{
    motor(3,0);
    motor(1,0);
    motor(2,0);
}

void rest(int t)
{
    reset();
    msleep(t);
}

void AGP(int port, int h) //arm ground position
{
    reset();
    int p = get_servo_position(port);
    printf("Current arm ground position %d\n", p);
    if(h < p)
    {
        while(p >= h)
        {
            set_servo_position(port,p);
            msleep(10);
            p-=10;
        }
    }
    else
    {
        while(p <= h)
        {
            set_servo_position(port,p);
            msleep(10);
            p+=10;
        }
    }
    rest(300);
}

void CP(int ch, int p) //claw position
{
    reset();
    if(p < get_motor_position_counter(ch))
    {
        while (get_motor_position_counter(ch) > p)
        {
            if(get_motor_position_counter(ch) > p + 40)
            {
                motor(ch,-50);
            }
            else
            {
                motor(ch,-20);
            }

        }
    }
    else
    {
        while (get_motor_position_counter(ch) < p)
        {
            if(get_motor_position_counter(ch) < p - 200)
            {
                motor(ch, 50);
            }
            else
            {
                motor(ch, 30);
            }

        }
    }
    rest(300);
}

void CC(int ch)
{
    reset();
    while ( digital(9) == 0)
    {
        if(get_motor_position_counter(ch) > 240)
        {
            motor(ch, -60);
        }
        else if (get_motor_position_counter(ch) <= 240 && get_motor_position_counter(ch) > 120)
        {
            motor(ch, -20);
        }
        else
        {
            motor(ch, -10);
        }
    }
    printf("Touched\n");
    reset();
    rest(1000);

    clear_motor_position_counter(ch);

}

void moveBDWhenGrab(double lSpeedPercent, double rSpeedPercent, int position, int port, int h)
{
    int x = 0;
    int p = get_servo_position(port);
    printf("Current arm ground position %d\n", p);
    if(h < p)
    {
        x = 1;
    }

    cmpc(1);
    cmpc(3);
    if(position >= 0)
    {
        while(get_motor_position_counter(1) < position)
        {            
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
            if(x == 1)
            {
                if(p >= h)
                {
                    set_servo_position(port,p);
                    p-=4;
                }
            }
            else
            {
                if(p <= h)
                {
                    set_servo_position(port,p);
                    p+=4;
                }
            }
        }
    }
    else
    {
        while(get_motor_position_counter(1) > position)
        {
            motor(1,lMax * lSpeedPercent / 100);
            motor(3,rMax * rSpeedPercent / 100);
            if(x == 1)
            {
                if(p >= h)
                {
                    set_servo_position(port,p);
                    p-=4;
                }
            }
            else
            {
                if(p <= h)
                {
                    set_servo_position(port,p);
                    p+=4;
                }
            }
        }
    }

    rest(300);
}

void turnLeft(double degree, int onTape) // 55 difference taped / untaped
{
    reset();
    if( onTape == 0)
    {
        moveToDT(-100,100, offTape360_n50p50 * fmod(degree, 360) / 360);
        //moveToDT(-50,50,-5655); //black tape
        //moveToDT(-50,50,-5600);
        //moveToDT(-10,10,-5785); //black tape
        //moveToDT(-10,10,-5730);
        //moveToDT(-20,20, -5735); //black tape
        //moveToDT(-20,20, -5680);
    }
    else
    {
        moveToDT(-100,100, onTape360_n50p50 * fmod(degree, 360) / 360);
    }
}


void turnRight(double degree, int onTape) // 55 difference taped / untaped
{
    reset();
    if( onTape == 0)
    {
        moveToDT(100,-100, -offTape360_n50p50 * fmod(degree, 360) / 360);
    }
    else
    {
        moveToDT(100,-100, -onTape360_n50p50 * fmod(degree, 360) / 360);
    }
}


void singleWheel(int p, int degree)
{
    reset();
    cmpc(1);
    cmpc(3);
    int tempPosi = offTape360_n80p80r * fmod(degree, 360) / 360;
    if(degree >= 0)
    {
        while(get_motor_position_counter(p) < tempPosi)
        {
            if(p == 1)
            {
                motor(p,lMax * 80 / 100);
            } 
            else
            {
                motor(p,rMax * 80 / 100);
            }
        }
    }
    else
    {
        while(get_motor_position_counter(p) > tempPosi)
        {
            if(p == 1)
            {
                motor(p,lMax * -80 / 100);
            } 
            else
            {
                motor(p,rMax * -80 / 100);
            }
        }
    }
}

void cli()
{
    reset();
    cmpc(1);
    cmpc(3);
    while(!(analog(2) >= a2min && analog(2) <= a2max && analog(1) >= a1min && analog(1) <= a1max))
    {
        if(analog(2) > a2min && analog(1) >= a1min && analog(1) <= a1max)
        {
            motor(1,lMax * 0 / 100);
            motor(3,rMax * 30 / 100);
        }

        else if(analog(2) < a2max && analog(1) >= a1min && analog(1) <= a1max)
        {
            motor(1,lMax * 0 / 100);
            motor(3,rMax * -30 / 100);
        }

        else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1min)
        {
            motor(1,lMax * 30 / 100);
            motor(3,rMax * -30 / 100);
        }

        else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1max)
        {
            motor(1,lMax * -30 / 100);
            motor(3,rMax * 30 / 100);
        }

        else if(analog(2) < a2min && analog(1) > a1max)
        {
            motor(1,lMax * 30 / 100);
            motor(3,rMax * -30 / 100);
        }

        else if(analog(2) > a2max && analog(1) < a1min)
        {
            motor(1,lMax * -30 / 100);
            motor(3,rMax * 30 / 100);
        }

        else if(analog(2) < a2min && analog(1) < a1min)
        {
            turnLeft(3, 1);
            motor(1,lMax * 50/100);
            motor(3,rMax * 50/100);
            msleep(500);
			turnRight(3, 1);
            motor(1,lMax * -50/100);
            motor(3,rMax * -50/100);
            msleep(700);
        }
        else if(analog(2) > a2min && analog(1) > a1min)
        {
            motor(1,lMax * 10 /100);
            motor(3,rMax * -10 /100);
            msleep(200);
            motor(1,lMax * 30/100);
            motor(3,rMax * 30/100);
            msleep(200);
            motor(1,lMax * -10/100);
            motor(3,rMax * 10/100);
            msleep(200);
        }
        printf("%d and %d\n",analog(1), analog(2));
    }
}

void FBlue(int position, int a2min, int a2max, int a1min, int a1max)
{
    reset();
    cmpc(1);
    cmpc(3);
    //motor(1, 60);
    //motor(3, 100);
    //msleep(400);
    while(get_motor_position_counter(1) < position)
    {
        if(get_motor_position_counter(1) < 1000)
        {
            if(analog(2) >= a2min && analog(2) <= a2max && analog(1) >= a1min && analog(1) <= a1max) //2908
            {
                motor(1,lMax * 50 / 100);
                motor(3,rMax * 50 / 100);
            }
            /*
            else if(analog(2) > a2min && analog(1) >= a1min && analog(1) <= a1max)
            {
                motor(1,lMax * 0 / 100);
                motor(3,rMax * 5 / 100);
            }
            */
            else if(analog(2) < a2max && analog(1) >= a1min && analog(1) <= a1max)
            {
                motor(1,lMax * 0 / 100);
                motor(3,rMax * -5 / 100);
            }
            /*
            else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1min)
            {
                motor(1,lMax * 5 / 100);
                motor(3,rMax * 0 / 100);
            }
            */
            else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1max)
            {
                motor(1,lMax * -5 / 100);
                motor(3,rMax * 5 / 100);
            }
            /*
            else if(analog(2) < a2min && analog(1) > a1max)
            {
                motor(1,lMax * 5 / 100);
                motor(3,rMax * -5 / 100);
            }

            else if(analog(2) > a2max && analog(1) < a1min)
            {
                motor(1,lMax * -5 / 100);
                motor(3,rMax * 5 / 100);
            }
            */
            else if(analog(2) < a2min && analog(1) < a1min)
            {
                motor(1,lMax * 10 /100);
                motor(3,rMax * 0/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 0/100);
                motor(3,rMax * 10/100);
                msleep(200);
            }
            else
            {
                motor(1,lMax * 0 /100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 0/100);
                msleep(200);
            }
        }
        else
        {
            if(analog(2) >= a2min && analog(2) <= a2max && analog(1) >= a1min && analog(1) <= a1max) //2908
            {
                motor(1,lMax * 50 / 100);
                motor(3,rMax * 50 / 100);
            }
            /*
            else if(analog(2) > a2min && analog(1) >= a1min && analog(1) <= a1max)
            {
                motor(1,lMax * 0 / 100);
                motor(3,rMax * 5 / 100);
            }
            */
            else if(analog(2) < a2max && analog(1) >= a1min && analog(1) <= a1max)
            {
                motor(1,lMax * 0 / 100);
                motor(3,rMax * -5 / 100);
            }
            /*
            else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1min)
            {
                motor(1,lMax * 5 / 100);
                motor(3,rMax * 0 / 100);
            }
            */
            else if(analog(2) >= a2min && analog(2) <= a2max && analog(1) < a1max)
            {
                motor(1,lMax * -5 / 100);
                motor(3,rMax * 5 / 100);
            }
            /*
            else if(analog(2) < a2min && analog(1) > a1max)
            {
                motor(1,lMax * 5 / 100);
                motor(3,rMax * -5 / 100);
            }

            else if(analog(2) > a2max && analog(1) < a1min)
            {
                motor(1,lMax * -5 / 100);
                motor(3,rMax * 5 / 100);
            }
            */
            else if(analog(2) < a2min && analog(1) < a1min)
            {
                motor(1,lMax * 10 /100);
                motor(3,rMax * 0/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 0/100);
                motor(3,rMax * 10/100);
                msleep(200);
            }
            else
            {
                motor(1,lMax * 0 /100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 10/100);
                msleep(200);
                motor(1,lMax * 10/100);
                motor(3,rMax * 0/100);
                msleep(200);
            }
        }
        printf("%d and %d\n",analog(1), analog(2));
    }

}
