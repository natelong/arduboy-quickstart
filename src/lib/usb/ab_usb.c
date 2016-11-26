// nate
#include "../ab.h"

#include "internal/descriptors.h"
#include "internal/Endpoint.h"
#include <avr/wdt.h>

#define LED_SETUP()    DDRC |= (1<<7); DDRB |= (1<<0); DDRD |= (1<<5);
#define L_LED_OFF()    PORTC &= ~(1<<7)
#define L_LED_ON()     PORTC |= (1<<7)
#define L_LED_TOGGLE() PORTC ^= (1<<7)
#define TX_LED_OFF()   PORTD |= (1<<5)
#define TX_LED_ON()    PORTD &= ~(1<<5)
#define RX_LED_OFF()   PORTB |= (1<<0)
#define RX_LED_ON()    PORTB &= ~(1<<0)

uint32_t pulseTimer = 0;
#define PULSE_FRAMES 15;

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


/** Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
 *  to relay data to and from the attached USB host.
 */
void EVENT_USB_Device_ConfigurationChanged(void) {
    /* Setup CDC Notification, Rx and Tx Endpoints */
    Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN, CDC_NOTIFICATION_EPSIZE, ENDPOINT_BANK_SINGLE);
    Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_IN, CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);
    Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, CDC_TXRX_EPSIZE, ENDPOINT_BANK_SINGLE);
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

void EVENT_USB_Device_Connect(void) {
    ab_setLED(0, 0, 255);
    pulseTimer = PULSE_FRAMES;
}

void EVENT_USB_Device_Disconnect(void) {
    ab_setLED(255, 0, 0);
    pulseTimer = PULSE_FRAMES;
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
    USB_Init();
}

void ab_usb_update(void) {
    USB_USBTask();

    if (pulseTimer && pulseTimer-- == 1) {
        ab_setLED(0, 0, 0);
    }
}
