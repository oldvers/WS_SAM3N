/**
 * \page twi_temp_sensor TWI Temperature Sensor Example
 *
 * \section Purpose
 *
 * This basic example program demonstrates how to use the TWI peripheral
 * to access an temperature sensor.
 *
 * \section Requirements
 *
 *  This package can be used with SAM3N evaluation kits. An external USART_DB
 *  board is needed for test.
 *  Please connect the USART_DB board to SAM3N-EK board with following table:
 *  - <b>SAM3N-EK   -- USART_DB</b>
 *   - 3V3        -- 3V3
 *   - TWD0(PA3)  -- TWD0
 *   - TWCK0(PA4) -- TWCK0
 *   - GND        -- GND
 *
 * \section Description
 *
 * The code can be roughly broken down as follows:
 * <ul>
 * <li>Configure TWI pins.
 * <li>Enable TWI peripheral clock.
 * <li>Configure TWI clock.
 * <li>Initialize TWI as twi master.
 * <li>The main function, which implements the program behavior.
 * <ol>
 * <li>Read all the registers within the sensor.
 * <li>Read the configuration such as adc resolution,alert mode etc.
 * <li>Get the current ambient temerature.
 * </ol>
 * </ul>
 *
 * \section Usage
 *
  * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- TWI Temperature Sensor Example  xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# The following traces detail operations on the temperature sensor,
 *    displaying success or error messages depending on the results of
 *    the commands.
 *
 * \section References
 * - twi_temp_sensor/main.c
 * - twi.c
 * - twid.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the twi temperature sensor example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "tasktwi.h"

//#include <stdio.h>
//#include <stdarg.h>
//#include <string.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** TWI clock frequency in Hz. */
#define TWCK            100000

/** TWI0 ID */
#define TWI_TEMP_ID    TWI0

#define INVALID_TEMP (uint16_t)(-110)

/** Slave address of Temperature Sensor. */
#define MCP9800_ADDRESS   0x48

/** Internal register within MCP9800 */
#define TEMP_REG       0x0
#define CONF_REG       0x1
#define HYST_REG       0x2
#define LIMT_REG       0x3

typedef struct
{
  uint8_t bOneShot;
  uint8_t bRes;
  uint8_t bFaultQueue;
  uint8_t bAlrtPol;
  uint8_t bMode;
  uint8_t bShutdown;
}Confg_Reg;

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Pio pins to configure. */
static const Pin pins_twi_temp[] = {PINS_TWI0};

/** TWI driver instance. */
static Twid twid;

/** constant represent float value */
const char* sixteenths[]={ "0","0625","125","1875","25","3125","375","4375","5","5625","625","6875","75","8125","875", "9375"};

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Get the real temperature code regardless of different adc resolutions
 */

static uint16_t _GetRealTemp( uint16_t reg_value, uint8_t res )
{
    if ( 8 < res && 13 > res )
    {
        return (reg_value>>(16-res));
    }

    return INVALID_TEMP ;
}

/**
 * \brief Fill the register value into specific structure MCP9800 configuration register.
 */
static void _FillConfigReg( uint8_t reg_value,Confg_Reg *conf )
{
    uint8_t res_array[]={9,10,11,12};
    uint8_t fault_queue_array[]={1,2,4,6};
    uint8_t temp;

    temp = ((reg_value>>7)&0x1);
    conf->bOneShot = temp;
    temp = ((reg_value>>5)&0x3);
    conf->bRes = res_array[temp];
    temp = ((reg_value>>3)&0x3);
    conf->bFaultQueue = fault_queue_array[temp];
    temp = ((reg_value>>2)&0x1);
    conf->bAlrtPol = temp;
    temp = ((reg_value>>1)&0x1);
    conf->bMode = temp;
    conf->bShutdown = (reg_value&0x1);
}

/**
 * \brief Display the prompting menu to interact with user
 */

static void _DisplayMenu( void )
{
    printf( "\n\r" );
    printf( " --- Temperature Sensor Operation Menu: ---\n\r" ) ;
    printf( " A --- Read all the internal registers of Sensor\n\r" ) ;
    printf( " C --- Get current configuration\n\r" ) ;
    printf( " T --- Get Ambient Temperature\n\r" ) ;
    printf( "\n\r" );
}

/*----------------------------------------------------------------------------
 *        Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for TWI temperature sensor example.
 *
 * \return Unused (ANSI-C compatibility).
 */

//extern int main( void )
void vTWITask(void * pvParameters)
{
    //uint8_t ucKey ;
    uint8_t Tdata[2] ;
    uint16_t temp ;
    Confg_Reg confg ;
  
  QueueHandle_t  LCDMsgQueue = (QueueHandle_t)pvParameters;
  tLCDMsg Msg;

  /* Disable watchdog */
  //WDT_Disable( WDT ) ;

  /* Output example information */
  printf("-- TWI Temperature Sensor Example %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  /* Configure TWI pins */
  PIO_Configure(pins_twi_temp, PIO_LISTSIZE(pins_twi_temp));

  /* Configure TWI */
  PMC_EnablePeripheral(ID_TWI0);
  TWI_ConfigureMaster(TWI0, TWCK, BOARD_MCK);
  TWID_Initialize(&twid, TWI0);

  /*Display Menu */
  _DisplayMenu() ;

  /* Enter menu loop */
  for(;;)
  {
//        ucKey = UART_GetChar();

//        if ( ucKey == 'A' || ucKey == 'a' )
//        {
//            TWID_Read(&twid, MCP9800_ADDRESS, TEMP_REG, 0x01, Tdata, 0x02, 0);
//            temp = ((Tdata[0]<<8)|(Tdata[1]));
//            printf("--I-- ambient temperature value:0x%X\n\r",temp);

//            TWID_Read(&twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0);
//            printf("--I-- configuraiton register value:0x%X\n\r",Tdata[0]);

//            TWID_Read(&twid, MCP9800_ADDRESS, HYST_REG, 0x01, Tdata, 0x02, 0);
//            printf("--I-- hysteresis register value:0x%X\n\r",((Tdata[0]<<8)|(Tdata[1])));

//            TWID_Read(&twid, MCP9800_ADDRESS, LIMT_REG, 0x01, Tdata, 0x02, 0);
//            printf("--I-- limit-set register value:0x%X\n\r",((Tdata[0]<<8)|(Tdata[1])));
//        }

//        if ( ucKey == 'C' || ucKey == 'c' )
//        {
//            TWID_Read( &twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0 ) ;
//            _FillConfigReg( Tdata[0], &confg ) ;

//            printf("--I-- Temperature Configuration!!\n\r");
//            printf("--I-- One-Shot Mode Enabled/Disabled:%d\n\r",confg.bOneShot);
//            printf("--I-- ADC Resolution:%d bits \n\r",confg.bRes);
//            printf("--I-- Fault Queue:%d\n\r",confg.bFaultQueue);
//            printf("--I-- Alert Polarity:%d\n\r",confg.bAlrtPol);
//            printf("--I-- COMP/INT mode:%d\n\r",confg.bMode);
//            printf("--I-- ShutDown Mode Enabled/Disabled:%d\n\r",confg.bShutdown);
//        }

//        if ( ucKey == 'T' || ucKey == 't' )
//        {
            uint8_t id;
            uint16_t integer;
            TWID_Read( &twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0 ) ;
            _FillConfigReg( Tdata[0], &confg ) ;
            TWID_Read( &twid, MCP9800_ADDRESS, TEMP_REG, 0x01, Tdata, 0x02, 0 ) ;

            temp = ((Tdata[0]<<8)|(Tdata[1]));
            temp = _GetRealTemp(temp,confg.bRes);
            id = temp%(2<<(confg.bRes-8-1));
            id =  id <<(4-(confg.bRes-8));
            integer =   temp/(2<<(confg.bRes-8-1));
            printf( "The ambient temperature is:%d.%s C\n\r",integer,sixteenths[id] ) ;
            
//            Msg.Type = LCD_MSG_LM75;
//            Msg.Value.U8[0] = 25;
//            xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
    Msg.Type = LCD_MSG_LM75;
  
    //Msg.Value.U16[0] = (*(U16*)Tdata);
    Msg.Value.U16[0] = ((S16)(Tdata[0] << 8) | Tdata[1]) / 128;
    //Msg.Value.U16[1] = DHT21_GetHumid();
    xQueueSend(LCDMsgQueue, (void *)&Msg, 0);
    
//        }
//        _DisplayMenu() ;

     vTaskDelay(1000);
   }
}

