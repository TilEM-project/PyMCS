/**********************************************************************
* Copyright (c) 2018 SmarAct GmbH
*
* This sample program shows how to read the analog aux inputs
* of the sensor module.
*
* Note that a special sensor module is neccessary in order to use the
* additional inputs.
*
* (Please read the MCS Programmer's Guide chapter about channel
* properties first)
* Properties are key/value pairs in the MCS that affect the behavior
* of the controller. To read or write the value of a property, the
* property must be addressed with its key. Keys consist of a component
* selector, a sub-component selector and the property name.
*
* THIS  SOFTWARE, DOCUMENTS, FILES AND INFORMATION ARE PROVIDED 'AS IS'
* WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
* BUT  NOT  LIMITED  TO,  THE  IMPLIED  WARRANTIES  OF MERCHANTABILITY,
* FITNESS FOR A PURPOSE, OR THE WARRANTY OF NON-INFRINGEMENT.
* THE  ENTIRE  RISK  ARISING OUT OF USE OR PERFORMANCE OF THIS SOFTWARE
* REMAINS WITH YOU.
* IN  NO  EVENT  SHALL  THE  SMARACT  GMBH  BE  LIABLE  FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, CONSEQUENTIAL OR OTHER DAMAGES ARISING
* OUT OF THE USE OR INABILITY TO USE THIS SOFTWARE.
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <MCSControl.h>


/* All MCS commands return a status/error code which helps analyzing 
   problems */
void ExitIfError(SA_STATUS st) {
    if(st != SA_OK) {
        printf("MCS error %u\n",st);
        exit(1);
    }
}


int main(int argc, char* argv[])
{
    SA_STATUS error = SA_OK;
    SA_INDEX mcsHandle;
    int value;
    double volt;

    /* **********************************************************************************/
    /* Synchronous communication */
    /* Open the first MCS with USB interface in syncronous communication mode */
    /* Use the locator "network:192.168.1.200:5000" for MCS with ethernet interface, adjust the IP address if neccessary */
    ExitIfError(SA_OpenSystem(&mcsHandle, "usb:ix:0", "sync"));
    printf("MCS sync mode\n");

    /* The sensors must be in "enabled" mode to use the analog aux inputs. */
    ExitIfError(SA_SetSensorEnabled_S(mcsHandle, SA_SENSOR_ENABLED));

    /* Read the analog aux input of channel 0 to 2.
    The read value ranges from 0 to 4095 which corresponds to 0V and 3.3V respectively. */
    for (int i=0; i<3; i++) {
        ExitIfError(SA_GetChannelProperty_S(mcsHandle,i,
                        SA_EPK(SA_SENSOR,SA_ANALOG_AUX_SIGNAL,SA_VALUE),&value));
        volt = 3.3 / 4096.0 * (double)value;
        printf("Analog aux input of channel %i is: %i (%0.2fV)\n",i, value, volt);
    }

    ExitIfError(SA_CloseSystem(mcsHandle));

    /* **********************************************************************************/
    /* Asynchronous communication */
    /* Open the first MCS with USB interface in asyncronous communication mode */
    ExitIfError(SA_OpenSystem(&mcsHandle, "usb:ix:0", "async"));
    printf("MCS async mode\n");

    SA_PACKET packet;

    /* Read the analog aux input of channel 0 to 2. */
    /* Note: when reading a property in asynchronous mode the value is returned
    in a SA_CHANNEL_PROPERTY_PACKET_TYPE */
    for (int i = 0; i < 3; i++) {
        ExitIfError(SA_GetChannelProperty_A(mcsHandle, i,
            SA_EPK(SA_SENSOR, SA_ANALOG_AUX_SIGNAL, SA_VALUE)));
    }

    /* Receive packets from the MCS. */
    for (int i = 0; i < 3; i++) {
        ExitIfError(SA_ReceiveNextPacket_A(mcsHandle, 1000, &packet));
        switch (packet.packetType) {
            case SA_NO_PACKET_TYPE:         /* SA_ReceiveNextPacket_A timed out */
                break;
            case SA_ERROR_PACKET_TYPE:      /* The MCS has sent an error message, handle error */
                printf("MCS received error %u\n", packet.data1);
                break;
            case SA_CHANNEL_PROPERTY_PACKET_TYPE:
                /* The channelIndex holds the source channel, the data1 field holds the property key and 
                the data2 field holds the property value. */
                volt = 3.3 / 4096.0 * (double)packet.data2;
                printf("Analog aux input of channel %i is: %i (%0.2fV)\n", packet.channelIndex, packet.data2, volt);
                break;
        }
    }

    ExitIfError(SA_CloseSystem(mcsHandle));

    /* **********************************************************************************/

    return 0;
}