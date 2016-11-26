// nate
#include "../ab.h"

#include "internal/descriptors.h"
#include "internal/Endpoint.h"
#include "internal/EndpointStream.h"
#include "internal/Device.h"
#include <avr/wdt.h>

#define LED_SETUP()    DDRC |= (1<<7); DDRB |= (1<<0); DDRD |= (1<<5);
#define L_LED_OFF()    PORTC &= ~(1<<7)
#define L_LED_ON()     PORTC |= (1<<7)
#define L_LED_TOGGLE() PORTC ^= (1<<7)
#define TX_LED_OFF()   PORTD |= (1<<5)
#define TX_LED_ON()    PORTD &= ~(1<<5)
#define RX_LED_OFF()   PORTB |= (1<<0)
#define RX_LED_ON()    PORTB &= ~(1<<0)

/** Contains the current baud rate and other settings of the first virtual serial port. This must be retained as some
 *  operating systems will not open the port unless the settings can be set successfully.
 */
static CDC_LineEncoding_t LineEncoding = {
    .BaudRateBPS = 0,
    .CharFormat  = CDC_LINEENCODING_OneStopBit,
    .ParityType  = CDC_PARITY_None,
    .DataBits    = 8
};

static uint8_t lineState = 0;

void USB_Disable(void) {
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    UDCON  |=  (1 << DETACH);   // USB_Detach();
    USBCON  &= ~(1 << USBE);    // USB_Controller_Disable();
    USB_PLL_Off();
    UHWCON  &= ~(1 << UVREGE);  // USB_REG_Off();
    USBCON  &= ~(1 << OTGPADE); // USB_OTGPAD_Off();
}




/** Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
 *  to relay data to and from the attached USB host.
 */
void EVENT_USB_Device_ConfigurationChanged(void) {
    /* Setup CDC Notification, Rx and Tx Endpoints */
    Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT, EP_DIR_IN, CDC_NOTIFICATION_EPSIZE, ENDPOINT_BANK_SINGLE);
    Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK, EP_DIR_IN, CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);
    Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK, EP_DIR_OUT, CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void) {
    /* Ignore any requests that aren't directed to the CDC interface */
    if ((USB_ControlRequest.type & (CONTROL_REQTYPE_TYPE | CONTROL_REQTYPE_RECIPIENT)) !=
            (REQTYPE_CLASS | REQREC_INTERFACE)) {
        return;
    }

    /* Process CDC specific control requests */
    switch (USB_ControlRequest.request) {
        case CDC_REQ_SetControlLineState:
            lineState = USB_ControlRequest.value;
            break;
        case CDC_REQ_SetLineEncoding:
            if (USB_ControlRequest.type == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
                Endpoint_ClearSETUP();

                /* Read the line coding data in from the host into the global struct */
                Endpoint_Read_Control_Stream(&LineEncoding, sizeof(CDC_LineEncoding_t));
                Endpoint_ClearIN();
            }
            break;
    }

    if (USB_ControlRequest.request == CDC_REQ_SetLineEncoding || USB_ControlRequest.request == CDC_REQ_SetControlLineState) {
        if (LineEncoding.BaudRateBPS == 1200 && (lineState & 0x01) == 0) {
            USB_Disable();
            ab_reset();
        }
    }
}

void ab_usb_init(void) {
    // Disable watchdog if enabled by bootloader/fuses
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    LED_SETUP();
    L_LED_OFF();
    TX_LED_OFF();
    RX_LED_OFF();

    sei();
    USB_Disable();

    UHWCON  |=  (1 << UVREGE); // USB_REG_On();
    PLLFRQ = ((1 << PLLUSB) | (1 << PDIV3) | (1 << PDIV1));
    USB_INT_DisableAllInterrupts();
    USB_INT_ClearAllInterrupts();
    USBCON &= ~(1 << USBE); // USB_Controller_Reset();
    USBCON |=  (1 << USBE); // USB_Controller_Reset();
    USB_CLK_Unfreeze();
    USB_PLL_Off();
    USB_DeviceState = DEVICE_STATE_Unattached;
    USB_Device_ConfigurationNumber = 0;
    UDCON &= ~(1 << LSM); // USB_Device_SetFullSpeed();
    USB_INT_EnableVBUS();
    Endpoint_ConfigureEndpoint(ENDPOINT_CONTROLEP, EP_TYPE_CONTROL, EP_DIR_OUT, USB_Device_ControlEndpointSize, ENDPOINT_BANK_SINGLE);
    USB_INT_ClearSuspend();
    USB_INT_EnableSuspend();
    USB_INT_EnableReset();
    UDCON  &= ~(1 << DETACH); // USB_Attach();
    USBCON  |=  (1 << OTGPADE); // USB_OTGPAD_On();
}

void ab_usb_update(void) {
    if (USB_DeviceState != DEVICE_STATE_Unattached) {
        uint8_t PrevEndpoint = Endpoint_GetCurrentEndpoint();
        Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
        if (Endpoint_IsSETUPReceived()) USB_Device_ProcessControlRequest();
        Endpoint_SelectEndpoint(PrevEndpoint);
    }
}
