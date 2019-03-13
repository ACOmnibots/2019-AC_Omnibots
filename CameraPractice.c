#include <kipr/botball.h>

int main()
{

    printf("Hello World\n");

    int t1 = 0, t2 =0;
    int a = 0, b = 0;
    enable_servos();
    set_servo_position(2, 800);
    int suc = 0;
    while(suc == 0){
    	suc = camera_open_black();
        //suc = camera_open();
    }
    while( t1 < 10000 && analog(2) < 2300)
    {
        camera_update();
        msleep(10);
        a = get_object_center_x(0,0);
        if( a <= 76)
        {
            motor(3, 80);
            motor(2, 95);
            msleep(50);
            printf("left\n");
        }
        else if(a > 76 && a < 84)
        {
            motor(3, 100);
            motor(2, 95);
            msleep(50);
            printf("mid\n");
        }
        else if(a >= 84)
        {
            motor(3, 95);
            motor(2, 80);
            msleep(50);
            printf("right\n");
        }
        
        t1 ++;
    }
    
    printf("first loop finished");
    motor(3, 100);
    motor(2, 100);
    msleep(300);
    
    set_servo_position(2, 1700);

    cmpc(3);
    cmpc(2);
    mtp(3, 1000, -1080);
    mtp(2, 1000, -1080);
    block_motor_done(3);
    block_motor_done(2);
    
    set_servo_position(2, 1000);
    
    motor(3, -100);
    motor(2, -100);
    msleep(1000);
    motor(3, -100);
    motor(2, 100);
    msleep(350);

    while(t2 < 10000 && analog(2) < 2300)
    {
        camera_update();
        msleep(10);
        b = get_object_center_x(1, 0);
        if( b <= 76)
        {
            motor(3, 80);
            motor(2, 95);
            msleep(50);
            printf("bleft\n");
        }
        else if(b > 76 && b < 84)
        {
            motor(3, 100);
            motor(2, 95);
            msleep(50);
            printf("bmid\n");
        }
        else if(b >= 84)
        {
            motor(3, 95);
            motor(2, 80);
            msleep(50);
            printf("bright\n");
        }
        
        t2 ++;
        
    }
    motor(3, 0);
    motor(2, 0);
    
    set_servo_position(2, 1700);

    cmpc(3);
    cmpc(2);
    mtp(3, 1000, -1080);
    mtp(2, 1000, -1080);
    block_motor_done(3);
    block_motor_done(2);
    
    set_servo_position(2, 1000);
    disable_servos();
    
    camera_close();
    ao();
    
    return 0;
}
