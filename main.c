/*
**
**                           Main.c
**  Tilt Axis Sensor Using STM32F4- Discovery Board
**
**********************************************************************/
/*
 $Revision: 00 $
 $Author: Jiri Krivak $
 $Date:  2.5.2019 $
 $Id: 186122 $

**********************************************************************/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_accelerometer.h"

#include <usbd_core.h>
#include <usbd_desc.h>
#include <usbd_cdc_if_template.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const double Phi_const = -0.075208957458567; //Roll Reference
const double Theta_const = -0.056383426681259; //Pitch Reference
const double Psi_const = 1.476735092166910; //Yaw Reference


USBD_HandleTypeDef USBD_Device;
// redifine _Write for mapping prinf function to work with USB virtual port
int _write(int fd, char * str, int len)
{
    for (int i = 0; i < len; i++)
    {

        VCP_write(&str[i],1);

    }
    return len;
}

void PITCH_ROLL(double InputData[3]);
void derotate_vector(double *Vect ,double phi,double theta,double psi , int16_t XYZ[3]);

int Timer1(uint32_t delay)
{
    static uint32_t tickStart = 0;

    // Just for the first init
    if (tickStart == 0)
        tickStart = HAL_GetTick();

    // Test if if delay is done
    if ((HAL_GetTick() - tickStart) < delay)
        return 0;

    // delay done, update the new time
    tickStart = HAL_GetTick();
    return 1;
}
int Timer2(uint32_t delay)
{
    static uint32_t tickStart = 0;

    // Just for the first init
    if (tickStart == 0)
        tickStart = HAL_GetTick();

    // Test if if delay is done
    if ((HAL_GetTick() - tickStart) < delay)
        return 0;

    // delay done, update the new time
    tickStart = HAL_GetTick();
    return 1;
}



int main(void)
{
    int16_t XYZ_vect[3] = {};
    double XYZ_vect2[3] = {};


    HAL_Init();
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    // USB CDC initialization
    USBD_Init(&USBD_Device, &VCP_Desc, 0);

    USBD_RegisterClass(&USBD_Device, &USBD_CDC);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_Template_fops);
    USBD_Start(&USBD_Device);

    // wait the end of the USB initialization
    while (!g_VCPInitialized) {}
    HAL_Delay(2000);
    BSP_ACCELERO_Init();
    BSP_ACCELERO_Click_ITConfig();
    HAL_Delay(500);
    while(1)
    {
        if (Timer1(1000))
        {
            printf("X axis: %d Y axis: %d Z axis: %d \r",XYZ_vect[0],XYZ_vect[1],XYZ_vect[2]);
            BSP_ACCELERO_GetXYZ(XYZ_vect);
            derotate_vector(XYZ_vect2,Phi_const,Theta_const,Psi_const ,XYZ_vect);
            PITCH_ROLL(XYZ_vect2);

        }

        //if (Timer2(20)) BSP_ACCELERO_GetXYZ(XYZ_vect);
    }
}


// Echo USB UART input char to USB UART output
void VCP_ECHO(char *byte)
{
    if (VCP_read(&byte, 1) == 1)
    {
        VCP_write(&byte, 1);
    }
}


void PITCH_ROLL(double InputData[3])
{

  double x_Buff = InputData[0] ;
  double  y_Buff = InputData[1] ;
  double  z_Buff = InputData[2] ;
  printf("ROT: X axis: %lf Y axis: %lf Z axis: %lf \r",x_Buff,y_Buff,z_Buff);
  //double roll = atan(y_Buff / z_Buff) * 57.3 ;
  double pitch = atan(y_Buff / sqrt( z_Buff * z_Buff + x_Buff * x_Buff)) * 57.3;
  double roll  = atan((- x_Buff) / sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3 ;

  printf("Roll: %lf Pitch: %lf\n\r",roll,pitch);

}



// Callibration with Reference Plane
void derotate_vector(double *Vect ,double phi,double theta,double psi , int16_t XYZ[3])
{
// Roll(phi) Pitch(theta) Yaw(psi), angles in radians
printf("AVG: X axis: %d Y axis: %d Z axis: %d \r",XYZ[0],XYZ[1],XYZ[2]);
double m11 = cos(theta)*cos(psi);
double m21 = cos(theta)*sin(psi);
double m31 = -sin(theta);
double m12 = sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi);
double m22 = sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi);
double m32 = sin(phi)*cos(theta);
double m13 = cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi);
double m23 = cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi);
double m33 = cos(phi)*cos(theta);
 // derotate vector (product matrix by vector)
Vect[0] = m11*XYZ[0] + m12*XYZ[1] + m13*XYZ[2];
Vect[1] = m21*XYZ[0] + m22*XYZ[1] + m23*XYZ[2];
Vect[2] = m31*XYZ[0] + m32*XYZ[1] + m33*XYZ[3];

}
