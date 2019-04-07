#include "mbed.h"
#include "src/utils/matrix.h"
#include "src/drivers/lsm9ds1.h"

#define f_ax 1.0016f
#define f_ay 1.0046f
#define f_az 0.9966f
#define b_ax 0.1871f
#define b_ay -0.0406f
#define b_az -0.3522f
#define f_mx 1.0f
#define f_my 1.0f
#define f_mz 1.0f
#define b_mx 0.0f
#define b_my 0.0f
#define b_mz 0.0f

// Objects
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX,NULL,230400);
LSM9DS1 imu(D4,D5);

// Ticker
Ticker tic_blink;
Ticker tic_imu;

// Flags
bool flag_blink = false;
bool flag_imu = false;


// Callbacks
void callback_blink()
{
    flag_blink = true;
}
void callback_imu()
{
    flag_imu = true;
}

Matrix g_list(100,3), a_list(100,3), m_list(100,3);
Matrix g(3,1), a(3,1), m(3,1);

// Main program
int main()
{
    imu.init();  
    tic_blink.attach(&callback_blink, 0.5);
    tic_imu.attach(&callback_imu, 1.0/50);
    int k = 1;
    while (true) 
    {
        if (flag_blink)
        {
            flag_blink = false;
            led = !led;
        }
        if (flag_imu)
        {
            flag_imu = false;
            imu.read();
            g(1,1) = imu.gx;
            g(2,1) = imu.gy;
            g(3,1) = imu.gz;
            a(1,1) = f_ax*(imu.ax-b_ax);
            a(2,1) = f_ay*(imu.ay-b_ay);
            a(3,1) = f_az*(imu.az-b_az);
            m(1,1) = f_mx*(imu.mx-b_mx);
            m(2,1) = f_my*(imu.my-b_my);
            m(3,1) = f_mz*(imu.mz-b_mz);
            g_list(k,1) = imu.gx;
            g_list(k,2) = imu.gy;
            g_list(k,3) = imu.gz;
            a_list(k,1) = imu.ax;
            a_list(k,2) = imu.ay;
            a_list(k,3) = imu.az;
            m_list(k,1) = imu.mx;
            m_list(k,2) = imu.my;
            m_list(k,3) = imu.mz;
            k++;
            //pc.printf("%.2f\n",norm(m));
        }
        if (k > 100) 
        {
            k = 1;
            for (int i = 1; i <= 100; i++) {
                for (int j = 1; j <= 3; j++) {
                    pc.printf("%.8f",m_list(i,j));
                    if (j < 3) {
                        pc.printf("\t");
                    }
                }
                pc.printf("\n");
            }
            pc.printf("\n");
        }
    }
}