#include <kipr/botball.h>
void reset();
void rest(int t);
void AGP(int port, int h); //arm ground position
void CP(int ch, int p); //claw position
void CC(int ch);//Close claw to position 0
void moveTo(double lSpeedPercent, double rSpeedPercent, int t);
void move(double lSpeedPercent, double rSpeedPercent);
void moveToD(double lSpeedPercent, double rSpeedPercent, int position);
void moveBDWhenGrab(double lSpeedPercent, double rSpeedPercent, int position, int port, int h);

/************************************************************************
*
* DO NOT DELETE VARIABLES AND METHODS, ASK LEO OR JASON IF CHANGES NEEDED
*
*************************************************************************/

double lMax = 100, rMax = 90.2, rbMax = 80; //left, right max speed while moving straight
int armMaxHeight = 1300, armGroundHeight = 1780, armTunnalHeight = 1380, armPomHeight = 1580, armCubeHeight = 1850, armRaisePomHeight = 1400;
int clawPomPosition = 200, clawGCubePosition = 0, clawRCubePosition = 90, clawMaxPosition = 1500, clawGrabOpenPosition = 1200,
clawClosePosition = 0, clawContainerPosition = 870; // GCube = green cube, RCube = red cube

int main()
{
    //test
    enable_servo(2);
    moveToD(-50,-50, -1000);
    CC(2);
    reset();
    CP(2, clawGrabOpenPosition);
    AGP(2, armMaxHeight);
    rest(500);
    AGP(2, armGroundHeight);
    rest(500);
    AGP(2,armPomHeight - 100);
    CP(2, clawGrabOpenPosition);
    moveToD(20,20, 960);
    rest(500);

    CP(2, clawPomPosition);
    AGP(2, armRaisePomHeight);
    rest(800);
    moveToD(-20,-20, -110);
    AGP(2, armCubeHeight);
    CP(2, clawGCubePosition);
    //AGP(2, armPomHeight);
    // CP(2, clawPomPosition);
    /*
    moveToD(-25,-25, -300);
    AGP(2, armPomHeight);
    */
    moveBDWhenGrab(-25,-25, -1100, 2, armPomHeight);
    AGP(2, armCubeHeight);
    CP(2, clawGrabOpenPosition);

    disable_servo(2);
    ao();
    return 0;
}

void moveTo(double lSpeedPercent, double rSpeedPercent, int t)
{
    move(lSpeedPercent,rSpeedPercent);
    msleep(t);
}

void move(double lSpeedPercent, double rSpeedPercent)
{
    motor(0, lMax * lSpeedPercent / 100);
    motor(1, rMax * rSpeedPercent / 100);
}

void moveToD(double lSpeedPercent, double rSpeedPercent, int position)
{
    cmpc(0);
    cmpc(1);
    if(position >= 0)
    {
        while(get_motor_position_counter(1) < position)
        {
            motor(0,lMax * lSpeedPercent / 100);
            motor(1,rMax * rSpeedPercent / 100);
        }
    }
    else
    {
        while(get_motor_position_counter(1) > position)
        {
            motor(0,lMax * lSpeedPercent / 100);
            motor(1,rbMax * rSpeedPercent / 100);
        }
    }
}

void reset()
{
    motor(0,0);
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
                motor(ch,-30);
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
                motor(ch, 20);
            }
            else
            {
                motor(ch, 50);
            }

        }
    }
    rest(300);
}
void CC(int ch)
{
    reset();
    while ( digital(1) == 0)
    {
        if(get_motor_position_counter(ch) > 50)
        {
            motor(2, -40);
        }
        else
        {
            motor(2, -10);
        }
    }
    printf("Touched\n");
    rest(100);

    cmpc(ch);

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

    cmpc(0);
    cmpc(1);
    if(position >= 0)
    {
        while(get_motor_position_counter(1) < position)
        {            
            motor(0,lMax * lSpeedPercent / 100);
            motor(1,rMax * rSpeedPercent / 100);
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
            motor(0,lMax * lSpeedPercent / 100);
            motor(1,rbMax * rSpeedPercent / 100);
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
