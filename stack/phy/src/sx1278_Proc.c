/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 RF chip low level functions driver
 *
 * \remark     Optional support functions.
 *             These functions are defined only to easy the change of the
 *             parameters.
 *             For a final firmware the radio parameters will be known so
 *             there is no need to support all possible parameters.
 *             Removing these functions will greatly reduce the final firmware
 *             size.
 *
 * \version    2.0.0 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include "platform.h"
#include "radio.h"
#include "bsp.h"

#include "sx1276.h"

#include "sx1276-Hal.h"
#include "sx1276-Fsk.h"
#include "sx1276-LoRa.h"

/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       SX12xx.c
 * \brief      Ping-Pong example application on how to use Semtech's Radio
 *             drivers.
 *
 * \version    2.0
 * \date       Nov 21 2012
 * \author     Miguel Luis
 */
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"


#include "radio.h"


#define BUFFER_SIZE                                 0x40 // Define the payload size here


static uint16_t BufferSize = BUFFER_SIZE;			// RF buffer size
static uint8_t Buffer[BUFFER_SIZE];					// RF buffer

static uint8_t EnableMaster = true; 				// Master/Slave selection

tRadioDriver *Radio = NULL;

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
extern uint8_t RFLRState;
/*
 * Manages the master operation
 */
void OnMaster( void )
{
    uint8_t i;
    
    switch( Radio->Process( ) )
    {
    case RF_RX_TIMEOUT:
        // Send the next PING frame
//        Buffer[0] = 'P';
//        Buffer[1] = 'I';
//        Buffer[2] = 'N';
//        Buffer[3] = 'G';
//        for( i = 4; i < BufferSize; i++ )
//        {
//            Buffer[i] = i - 4;
//        }
//        Radio->SetTxPacket( Buffer, BufferSize );
					RFLRState = RFLR_STATE_RX_INIT;		
        break;
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
    
        if( BufferSize > 0 )
        {
          UART1_Transmit(Buffer,BufferSize/2);  
					if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PONG
								BSP_LED_Toggle(1); 
								RFLRState = RFLR_STATE_RX_INIT;							

                // Send the next PING frame            
//                Buffer[0] = 'P';
//                Buffer[1] = 'I';
//                Buffer[2] = 'N';
//                Buffer[3] = 'G';
//                // We fill the buffer with numbers for the payload 
//                for( i = 4; i < BufferSize; i++ )
//                {
//                    Buffer[i] = i - 4;
//                }
//                Radio->SetTxPacket( Buffer, BufferSize );
            }
            else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            { // A master already exists then become a slave
                EnableMaster = false;
                BSP_LED_Off(2); 
            }
        }            
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PING
        BSP_LED_Toggle(2);;
        Radio->StartRx( );
        break;
    default:
        break;
    }
}

/*
 * Manages the slave operation
 */
void OnSlave( void )
{
    uint8_t i;

    switch( Radio->Process( ) )
    {
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
    
        if( BufferSize > 0 )
        {
            if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PING
                BSP_LED_Toggle(1);

               // Send the reply to the PONG string
                Buffer[0] = 'P';
                Buffer[1] = 'O';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                // We fill the buffer with numbers for the payload 
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }

              //  Radio->SetTxPacket( Buffer, BufferSize );
								RFLRState = RFLR_STATE_RX_INIT;
            }
        }
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PONG
        BSP_LED_Toggle(2);
         // Send the next PING frame            
				SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );

        RFLRState = RFLR_STATE_TX_RUNNING;
        break;
    default:
        break;
    }
}


/*
 * Main application entry point.
 */
void SX1278_Thread(void const * argument)
{
    
    Radio = RadioDriverInit( );
    
    Radio->Init( );

    Radio->StartRx( );
    
    while( 1 )
    {
        if( EnableMaster == true )
        {
            OnMaster( );
        }
        else
        {
            OnSlave( );
        }    
        osDelay(50);
    }

}



