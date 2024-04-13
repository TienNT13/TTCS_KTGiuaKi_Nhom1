// em thay doi o day ne..hihi
//hihi hahah


#include <stdint.h>
#include <temhumsensor.h>
#include <Ucglib.h>
#include <timer.h>
#include <stm32f401re_i2c.h>
#include <stm32f401re_spi.h>
#include <string.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>
#include <stdio.h>
#include <stm32f401re_adc.h>
#include <stm32f401re_tim.h>
#include <lightsensor.h>
#include <kalman_filter.h>
#include <Ucglib.h>
#include <ucg.h>
#include <string.h>

#define CYCLE_SEND_DATA_1 1000 // ms

#define CYCLE_SEND_DATA_2 2000 // ms

/*Defined SENSOR ***************************************************************/

#define SI7020_ADDR                          0x40
#define CMDR_MEASURE_VALUE                   0xE0

#define	LOW									0
#define	HIGH								1
#define BTN_PRESS							1

/*Defined SPI **********************************************************************/

#define SPI1_CS_PORT           				 GPIOB
#define SPI1_CS_PIN            				 GPIO_Pin_6

#define SPI1_RST_PORT          				 GPIOC
#define SPI1_RST_PIN           				 GPIO_Pin_7

#define SPI1_MOSI_PORT         				 GPIOA
#define SPI1_MOSI_PIN          				 GPIO_Pin_7

#define SPI1_SCK_PORT          				 GPIOA
#define SPI1_SCK_PIN           				 GPIO_Pin_5

#define SPI1_RS_PORT           				 GPIOA
#define SPI1_RS_PIN            				 GPIO_Pin_9

#define SPI1_ENABLE_PORT       				 GPIOB
#define SPI1_ENABLE_PIN        				 GPIO_Pin_10

#define SPI1_MODE_PORT          			 GPIOA
#define SPI1_MODE_PIN           			 GPIO_Pin_8

/*Defined I2C ******************************************************************/

#define I2Cx_RCC							 RCC_APB1Periph_I2C1
#define I2Cx_SENSOR							 I2C1
#define I2C_GPIO_RCC		    			 RCC_AHB1Periph_GPIOB
#define I2C_GPIO							 GPIOB
#define I2C_PIN_SDA			    		 	 GPIO_Pin_9
#define I2C_PIN_SCL			    			 GPIO_Pin_8

/*Privated Function ************************************************************/
static void AppCommon();
static void SPI1_Init(void);

static void I2C_Init_temphumi(void);
void I2C_start(void);
void I2C_address_direction(uint8_t address, uint8_t direction);
void I2C_transmit(uint8_t byte);
void I2C_stop(void);
uint8_t I2C_receive_nack(void);
uint8_t I2C_receive_ack(void);
static void TemHumSensor_readRegister(
		uint8_t address,
	    uint8_t* pAddressRegister,
	    uint8_t* pDataRegister,
	    uint8_t Length_Data,
	    uint16_t delay);
void processGetValueSensor(void);
uint32_t GetTemp_Sensor(void);
uint32_t GetHumi_Sensor(void);
void delay_ms(uint32_t ms);
uint32_t CalculatorTime(uint32_t dwTimeInit, uint32_t dwTimeCurrent);
void Scan_SensorLCD(void);
void Scan_TimeSensor(uint32_t byRepeats);

static void LightSensor_AdcInit(void);
static void LedControl_TimerOCInit(void);
uint16_t LightSensor_AdcPollingRead(void);
uint16_t Kanman_Light(uint16_t lightLevel);
static void LedControl_TimerOCSetPwm(uint32_t Compare);
static void ABL_Process(void);

/* Private variables ---------------------------------------------------------*/
static ucg_t ucg;
uint32_t time_initial = 0;
uint32_t time_current, time_initial;
uint32_t idTimer = 0;
static char src1[20] = "";
static char src2[20] = "";
static char src3[20] = "";
static char src4[20] = "";
static char src5[20] = "";
static uint32_t time_total;
static uint8_t temperature, humidity;
static uint8_t temperature1, humidity1;
//static uint8_t temperature2, humidity2;

// cam bien anh sang
uint16_t AdcValue=0;
static uint16_t lightLevelAfterFilter = 0;
static uint16_t Kanman_light=0;
//static char src3[20] = "";
//static ucg_t ucg;

/* Private defined*****************************************************************/
#define ADC_PORT				    		GPIOC
#define ADC_PIN             			    GPIO_Pin_5
#define Tim_Period      			  		8399

/*
 * @func    I2C_start
 * @brief   Generate I2C start condition
 * @param   None
 * @retval  None
 */

// Define Logic GPIO_PIN

#define GPIO_PIN_SET						1
#define GPIO_PIN_RESET						0
#define GPIO_PIN_LOW						0
#define GPIO_PIN_HIGH						1

// LED BOARD
#define LEDBOARD_GPIO_PORT						GPIOA
#define LEDBOARD_GPIO_PIN						GPIO_Pin_5
#define LEDBOARD_PIN							5
#define LEDBOARDControl_SetClock				RCC_AHB1Periph_GPIOA

// LED RED 1
#define LEDRED1_GPIO_PORT						GPIOA
#define LEDRED1_GPIO_PIN						GPIO_Pin_1
#define LEDRED1_PIN								1
#define LEDRED1Control_SetClock					RCC_AHB1Periph_GPIOA

// LED RED 2
#define LEDRED2_GPIO_PORT						GPIOB
#define LEDRED2_GPIO_PIN						GPIO_Pin_13
#define LEDRED2_PIN								13
#define LEDRED2Control_SetClock					RCC_AHB1Periph_GPIOB

// LED GREEN 1
#define LEDGREEN1_GPIO_PORT						GPIOA
#define LEDGREEN1_GPIO_PIN						GPIO_Pin_0
#define LEDGREEN1_PIN							0
#define LEDGREEN1Control_SetClock				RCC_AHB1Periph_GPIOA

// LED GREEN 2
#define LEDGREEN2_GPIO_PORT						GPIOA
#define LEDGREEN2_GPIO_PIN						GPIO_Pin_11
#define LEDGREEN2_PIN							11
#define LEDGREEN2Control_SetClock				RCC_AHB1Periph_GPIOA

// LED BLUE 1
#define LEDBLUE1_GPIO_PORT						GPIOA
#define LEDBLUE1_GPIO_PIN						GPIO_Pin_3
#define LEDBLUE1_PIN							3
#define LEDBLUE1Control_SetClock				RCC_AHB1Periph_GPIOA

// LED BLUE 2
#define LEDBLUE2_GPIO_PORT						GPIOA
#define LEDBLUE2_GPIO_PIN						GPIO_Pin_10
#define LEDBLUE2_PIN							10
#define LEDBLUE2Control_SetClock				RCC_AHB1Periph_GPIOA

// BUTTON 1
#define BUTTON1_GPIO_PORT						GPIOB
#define BUTTON1_GPIO_PIN						GPIO_Pin_5
#define BUTTON1_PIN								5
#define BUTTON1Control_SetClock					RCC_AHB1Periph_GPIOB

// BUTTON 2
#define BUTTON2_GPIO_PORT						GPIOB
#define BUTTON2_GPIO_PIN						GPIO_Pin_3
#define BUTTON2_PIN								3
#define BUTTON2Control_SetClock					RCC_AHB1Periph_GPIOB

// BUTTON 3
#define BUTTON3_GPIO_PORT						GPIOA
#define BUTTON3_GPIO_PIN						GPIO_Pin_4
#define BUTTON3_PIN								4
#define BUTTON3Control_SetClock					RCC_AHB1Periph_GPIOA

// BUTTON 4
#define BUTTON4_GPIO_PORT						GPIOB
#define BUTTON4_GPIO_PIN						GPIO_Pin_0
#define BUTTON4_PIN								0
#define BUTTON4Control_SetClock					RCC_AHB1Periph_GPIOB

// BUTTON 5
#define BUTTON5_GPIO_PORT						GPIOB
#define BUTTON5_GPIO_PIN						GPIO_Pin_4
#define BUTTON5_PIN								4
#define BUTTON5Control_SetClock					RCC_AHB1Periph_GPIOB

// buzzer
#define COI_GPIO_PORT							GPIOC
#define COI_GPIO_PIN							GPIO_Pin_9
#define COI_PIN									9
#define COI_Control_SetClock					RCC_AHB1Periph_GPIOC

// khoi tao led board
static void Ledboard_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDBOARDControl_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led red 1

	GPIO_InitStructure.GPIO_Pin = LEDBOARD_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDBOARD_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao led red 1
static void Ledred1_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDRED1Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led red 1

	GPIO_InitStructure.GPIO_Pin = LEDRED1_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDRED1_GPIO_PORT, &GPIO_InitStructure);
}


// khoi tao led red 2
static void Ledred2_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port B
	RCC_AHB1PeriphClockCmd(LEDRED2Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led red 2

	GPIO_InitStructure.GPIO_Pin = LEDRED2_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDRED2_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao led green 1
static void Ledgreen1_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDGREEN1Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led green 1

	GPIO_InitStructure.GPIO_Pin = LEDGREEN1_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDGREEN1_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao led green 2
static void Ledgreen2_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDGREEN2Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led green 2

	GPIO_InitStructure.GPIO_Pin = LEDGREEN2_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDGREEN2_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao led blue 1
static void Ledblue1_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDBLUE1Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led blue 1

	GPIO_InitStructure.GPIO_Pin = LEDBLUE1_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDBLUE1_GPIO_PORT, &GPIO_InitStructure);
}

static void Ledblue2_init(void) {
	// Khai bao bien thuoc kieu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cap xung clock hoat dong cho port A
	RCC_AHB1PeriphClockCmd(LEDBLUE2Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien led blue 2

	GPIO_InitStructure.GPIO_Pin = LEDBLUE2_GPIO_PIN;

	//Chon chan dieu khien led1 che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// PUSH PULL

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	// trang thai ban dau tren chan la Pull down

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(LEDBLUE2_GPIO_PORT, &GPIO_InitStructure);
}


// khoi tao nut nhan 1
static void Button1_init(void) {
	// Khai báo biến thuộc kiểu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock hoạt động cho PORT B

	RCC_AHB1PeriphClockCmd(BUTTON1Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien nut nhan 1

	GPIO_InitStructure.GPIO_Pin = BUTTON1_GPIO_PIN;

	//Chon chan dieu khien nut che do input

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// trang thai ban dau tren chan la Pull up

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(BUTTON1_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao nut nhan 2
static void Button2_init(void) {
	// Khai báo biến thuộc kiểu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock hoạt động cho PORT B

	RCC_AHB1PeriphClockCmd(BUTTON2Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien nut nhan 2

	GPIO_InitStructure.GPIO_Pin = BUTTON2_GPIO_PIN;

	//Chon chan dieu khien nut che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// trang thai ban dau tren chan la Pull up

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(BUTTON2_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao nut nhan 3
static void Button3_init(void) {
	// Khai báo biến thuộc kiểu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock hoạt động cho PORT B

	RCC_AHB1PeriphClockCmd(BUTTON3Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien nut nhan 3

	GPIO_InitStructure.GPIO_Pin = BUTTON3_GPIO_PIN;

	//Chon chan dieu khien nut che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// trang thai ban dau tren chan la Pull up

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(BUTTON3_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao nut nhan 4
static void Button4_init(void) {
	// Khai báo biến thuộc kiểu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock hoạt động cho PORT A

	RCC_AHB1PeriphClockCmd(BUTTON4Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien nut nhan 4

	GPIO_InitStructure.GPIO_Pin = BUTTON4_GPIO_PIN;

	//Chon chan dieu khien nut che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// trang thai ban dau tren chan la Pull up

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(BUTTON4_GPIO_PORT, &GPIO_InitStructure);
}

// khoi tao nut nhan 5
static void Button5_init(void) {
	// Khai báo biến thuộc kiểu struct
	GPIO_InitTypeDef GPIO_InitStructure;

	// Cấp xung clock hoạt động cho PORT B

	RCC_AHB1PeriphClockCmd(BUTTON4Control_SetClock, ENABLE);

	// chon chan su dung chuc nang dieu khien nut nhan 5

	GPIO_InitStructure.GPIO_Pin = BUTTON5_GPIO_PIN;

	//Chon chan dieu khien nut che do output

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Toc do xu ly

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// trang thai ban dau tren chan la Pull up

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// khoi tao cac gia tri

	GPIO_Init(BUTTON5_GPIO_PORT, &GPIO_InitStructure);
}

static void coi_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(COI_Control_SetClock, ENABLE);


	GPIO_InitStructure.GPIO_Pin = COI_GPIO_PIN;


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;


	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;


	GPIO_Init(COI_GPIO_PORT, &GPIO_InitStructure);

}

// set trang thai cua buzzer (COI)
static void coiControlSetStatus( GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN,
		uint8_t Status) {

	// SET bit in BSRR Registers

		if (Status == GPIO_PIN_SET) {
			GPIOx->BSRRL = GPIO_PIN;
		}
		if (Status == GPIO_PIN_RESET) {
			GPIOx->BSRRH = GPIO_PIN;
		}
}

//set trang thai LED
static void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN,
		uint8_t Status) {
	// SET bit in BSRR Registers

	if (Status == GPIO_PIN_SET) {
		GPIOx->BSRRL = GPIO_PIN;
	}
	if (Status == GPIO_PIN_RESET) {
		GPIOx->BSRRH = GPIO_PIN;
	}
}

// doc trang thai cua nut nhan
static uint8_t ButtonRead_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN) {
	uint8_t Read_Pin = 0x00;

	if ((GPIOx->IDR & GPIO_PIN) != (uint32_t) Bit_RESET) {
		Read_Pin = (uint8_t) Bit_SET;
	} else {
		Read_Pin = (uint8_t) Bit_RESET;
	}
	return Read_Pin;
}

void delay() {
	for (uint32_t i = 0; i < 500000; i++);
}


int main (void){

	AppCommon();

	//uint8_t nut_nhan1 ;
	uint8_t nut_nhan2 ;
	uint8_t nut_nhan3 ;
	uint8_t nut_nhan4 ;
	//uint8_t nut_nhan5 ;

	Ledboard_init();
	Ledred1_init();
	Ledred2_init();
	Ledgreen1_init();
	Ledgreen2_init();
	Ledblue1_init();
	Ledblue2_init();
	Button1_init();
	Button2_init();
	Button3_init();
	Button4_init();
	Button5_init();
	coi_Init();

	// moi nap code xog den sang 2 lan
	for(int i = 0; i < 2; i++){
		LedControl_SetStatus(LEDBOARD_GPIO_PORT, LEDBOARD_GPIO_PIN, GPIO_PIN_HIGH);
		delay_ms(500);
		LedControl_SetStatus(LEDBOARD_GPIO_PORT, LEDBOARD_GPIO_PIN, GPIO_PIN_LOW);
		delay_ms(500);
	}

	while(1){

		processGetValueSensor();
		processTimerScheduler();
		ABL_Process(); // thay đổi độ sáng của led RGB theo cường độ ánh sáng với chu kỳ 100 ms.

		// nut nhan 1 dieu khien ledred 1,2 , coi keu nhay 2 lan
		nut_nhan2 = ButtonRead_Status(BUTTON2_GPIO_PORT, BUTTON2_GPIO_PIN);
		LedControl_SetStatus(LEDBOARD_GPIO_PORT, LEDBOARD_GPIO_PIN, GPIO_PIN_LOW);

		if ( nut_nhan2 == 0) {
			for(int i = 0; i < 2; i++){
				delay_ms(500);
				coiControlSetStatus(COI_GPIO_PORT, COI_GPIO_PIN, GPIO_PIN_HIGH);
				LedControl_SetStatus(LEDRED1_GPIO_PORT, LEDRED1_GPIO_PIN, GPIO_PIN_HIGH);
				LedControl_SetStatus(LEDRED2_GPIO_PORT, LEDRED2_GPIO_PIN, GPIO_PIN_HIGH);

				delay_ms(500);

				coiControlSetStatus(COI_GPIO_PORT, COI_GPIO_PIN, GPIO_PIN_LOW);
				LedControl_SetStatus(LEDRED1_GPIO_PORT, LEDRED1_GPIO_PIN, GPIO_PIN_LOW);
				LedControl_SetStatus(LEDRED2_GPIO_PORT, LEDRED2_GPIO_PIN, GPIO_PIN_LOW);
//				delay(1000);
			}
		}

		// nut nhan 2 trong 3s dieu khien led blue sang
		nut_nhan3 = ButtonRead_Status(BUTTON3_GPIO_PORT, BUTTON3_GPIO_PIN);
		uint8_t ok ;
		uint8_t count1 = 0;
		if(nut_nhan3 == 0){
			while(count1 <=3){

				count1++;
				delay_ms(750);
			}
			if(count1 >= 3){

				ok = 1;

			}
		}else{
			count1 = 0;
			//delay(500);
		}



		// nut nhan 3  giu 3s de tat led blue2 tat
		nut_nhan4 = ButtonRead_Status(BUTTON4_GPIO_PORT, BUTTON4_GPIO_PIN);
		uint8_t count2 = 0;
		//uint8_t ok1 ;
			if(nut_nhan4 == 0){
				while(count2 <=3){
					count2++;
					delay_ms(750);
				}
				if(count2 >= 3){
					ok = 0;

				}
			}else{
				count2 = 0;
				//delay(500);
			}

			if(ok == 1){
				LedControl_SetStatus(LEDBLUE1_GPIO_PORT, LEDBLUE1_GPIO_PIN, GPIO_PIN_HIGH);
				LedControl_SetStatus(LEDBLUE2_GPIO_PORT, LEDBLUE2_GPIO_PIN, GPIO_PIN_HIGH);

				delay_ms(500);
			}
			if(ok == 0){
				LedControl_SetStatus(LEDBLUE1_GPIO_PORT, LEDBLUE1_GPIO_PIN, GPIO_PIN_LOW);
				LedControl_SetStatus(LEDBLUE2_GPIO_PORT, LEDBLUE2_GPIO_PIN, GPIO_PIN_LOW);

				delay_ms(500);
			}
	}
	return 0;
}

void I2C_start(void) // de truyen bit start
{
	// Wait until I2Cx is not busy anymore
	while (I2C_GetFlagStatus(I2Cx_SENSOR, I2C_FLAG_BUSY));

	// Generate start condition
	I2C_GenerateSTART(I2Cx_SENSOR, ENABLE); //  ham de truuyen bit start

	// Wait for I2C EV5.
	// It means that the start condition has been correctly released
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_MODE_SELECT));
}

/**
 * @func    I2C_address_direction
 * @brief   e
 * @param   None
 * @retval  None
 */
void I2C_address_direction(uint8_t address, uint8_t direction) // truyen bit dia chi voi 2 tham so la dia chi va truyen/nhan du lieu
{
	// Send slave address
	I2C_Send7bitAddress(I2Cx_SENSOR, address, direction); // de truyen dia chi

	// Wait for I2C EV6
	// It means that a slave acknowledges his address
	if (direction == I2C_Direction_Transmitter)		// truyền





	{
		while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if (direction == I2C_Direction_Receiver)  // nhận
	{
		while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/**
 * @func    I2C_transmit
 * @brief   Transmit one byte to I2C bus
 * @param   None
 * @retval  None
 */
void I2C_transmit(uint8_t byte) // truuyen dia chi lay du lieu cuua thanh ghi trong cam bien
{
	// Send data byte
	I2C_SendData(I2Cx_SENSOR, byte); // ham de truuyen du lieu
	// Wait for I2C EV8_2.
	// It means that the data has been physically shifted out and
	// output on the bus)
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/* @func    I2C_stop
* @brief   Generate I2C stop condition
* @param   None
* @retval  None
*/
void I2C_stop(void) // hoan tat quua trinh truyen du lieu
{
	// Generate I2C stop condition
	I2C_GenerateSTOP(I2Cx_SENSOR, ENABLE);
}

/**
 * @func    I2C_receive_nack
 * @brief   receive data
 * @param   None
 * @retval  None
 */
uint8_t I2C_receive_nack(void) // nhan duu lieu
{
	// Disable ACK of received data
	I2C_AcknowledgeConfig(I2Cx_SENSOR, DISABLE);
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(I2Cx_SENSOR);
}

/**
 * @func    I2C_receive_ack
 * @brief   receive data
 * @param   None
 * @retval  None
 */
uint8_t I2C_receive_ack(void)
{
	// Enable ACK of received data
	I2C_AcknowledgeConfig(I2Cx_SENSOR, ENABLE);
	// Wait for I2C EV7
	// It means that the data has been received in I2C data register
	while (!I2C_CheckEvent(I2Cx_SENSOR, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read and return data byte from I2C data register
	return I2C_ReceiveData(I2Cx_SENSOR);
}

static void TemHumSensor_readRegister( // giao tiep cac thanh ghi trong cam bien de doc gia tri nhiet do, do am
		uint8_t address,	// Địa chỉ cảm biến.
	    uint8_t* pAddressRegister, // Địa chỉ của thanh ghi chứa dữ liệu nhiệt độ, độ ẩm.
	    uint8_t* pDataRegister, // Dữ liệu đọc được từ thanh ghi tương ứng.
	    uint8_t Length_Data, //Độ dài Dữ liệu đọc được từ thanh ghi tương ứng.
	    uint16_t delay)	//Dữ liệu đọc được từ thanh ghi tương ứng.
{
	uint8_t LengthCmd = pAddressRegister[0];

	I2C_start();	// conditon start
	I2C_address_direction(address << 1, I2C_Direction_Transmitter); // send slave address - Transmit

	for (uint8_t i = 1; i < LengthCmd; i++) {
		I2C_transmit(pAddressRegister[i]);		// Send Data from register
	}

    if (delay > 0) {
        delay_ms(delay);
    }

	I2C_stop();// condition stop

	I2C_start(); // condition start
	I2C_address_direction(address << 1, I2C_Direction_Receiver); // send slave address - Received

	for (uint8_t i = 0; i < Length_Data; i++)
	{
		if (i == (Length_Data - 1))
		{
			pDataRegister[i] = I2C_receive_nack();  // 	NA
		}
		else
		{
			pDataRegister[i] = I2C_receive_ack();   // A
		}
	}
	I2C_stop();

}

/**
 * @func    GetTemp_Sensor
 * @brief   Get value temperature
 * @param   None
 * @retval  Temperature
 */
uint32_t GetTemp_Sensor(void) // xuu li du lieuu nhiet do
{
	uint32_t RT;
    uint8_t pRT[3] = { 0 };	// pRT[0]: MSB		pRT[1]: LSB

    uint8_t CMD_MEASURE_TEMP[2] =  { 2, 0xE3 }; // gửi độ dài byte cần truyền và CMD_MEASURE chế độ HOLD MASTER MODE

    TemHumSensor_readRegister(SI7020_ADDR, CMD_MEASURE_TEMP, pRT, 3, 4);

    RT = (pRT[0] << 8) + pRT[1];			// RT_CODE (MSB << 8) + LSB		// Temp = ((17572* RT)/(0xFFu +1) - 4685)/100
    RT = ((RT * 17572) >> 16) - 4685;
    RT = RT/100;
    return RT;
}

/**
 * @func    GetHumi_Sensor
 * @brief   Get value humidity
 * @param   None
 * @retval  Humidity
 */
uint32_t GetHumi_Sensor(void) // xuuu ly du lieu do am
{
	uint32_t RH;
	uint8_t pRH[3] = { 0 };


	uint8_t CMD_MEASURE_HUMI[2] =  { 2, 0xE5 }; // gửi độ dài byte cần truyền và CMD_MEASURE chế độ HOLD MASTER MODE

    TemHumSensor_readRegister(SI7020_ADDR, CMD_MEASURE_HUMI, pRH, 3, 8);

    RH = (pRH[0] << 8) + pRH[1]; // RH_CODE (MSB << 8) + LSB
    RH = ((RH * 12500) >> 16) - 600;
    RH = RH/100;

    return RH;
}

void processGetValueSensor(void) { // xu li du lieu nhiet do va do am sau 1s và in ra man hinh lcd
	// Get timer
	time_current = GetMilSecTick();

	if (time_current >= time_initial) {
		time_total += time_current - time_initial;
	} else {
		time_total += 0xFFFFFFFFU - time_current + time_initial;
	}
	if (time_total == CYCLE_SEND_DATA_1) {
		//Dùng để đọc giá trị nhiệt độ trả về từ cảm biến thông qua giao tiếp I2C.
		temperature = (uint8_t) (GetTemp_Sensor());

		//Dùng để đọc giá trị độ ẩm trả về từ cảm biến thông qua giao tiếp I2C.
		humidity = (uint8_t) (GetHumi_Sensor());

	}
	if(time_total >= CYCLE_SEND_DATA_2)
	{
		temperature1 = (uint8_t) (GetTemp_Sensor());

		humidity1 = (uint8_t) (GetHumi_Sensor());

		// nếu thời gian chênh lệch giữa lần đo 1 và lần đo 2 là 2 thì sẽ cập nhật giá trị sensor
		if( ((temperature1 > temperature ) && (temperature1 - temperature >= 2))
		 || ((temperature1 < temperature ) && (temperature1 - temperature <= 2))
		 || ((humidity1 > humidity ) && (humidity1 - humidity >= 2))
		 || ((humidity1 < humidity ) && (humidity1 - humidity <= 2)))
		{
			ucg_DrawString(&ucg,0 , 32, 0, "Nhom 1 ");
			memset(src1, 0, sizeof(src1));
			sprintf(src1, " Temp = %d oC  ", temperature1);
			ucg_DrawString(&ucg, 0, 52, 0, src1);

			memset(src2, 0, sizeof(src2));
			sprintf(src2, " Humi = %3d %%   ", humidity1);
			ucg_DrawString(&ucg, 0, 72, 0, src2);

			memset(src5, 0, sizeof(src5));
			sprintf(src5, " Light = %d lux  ", Kanman_light);
			ucg_DrawString(&ucg, 0, 92, 0, src5);
		}

		time_total = 0;
		// Reset lại biến thời gian sau quá trình đọc giá trị nhiệt độ - độ ẩm.

	}
	time_initial = time_current;
}


/**
* @func 	Scan_SensorLCD
* @brief	Scan display value sensor
* @param	None
* @retval	None
*/
void Scan_SensorLCD(void)
{
	ucg_DrawString(&ucg, 0, 32, 0, "Nhom 1");
	memset(src3, 0, sizeof(src3));
	sprintf(src3, " Temp = %d oC  ", temperature);
	ucg_DrawString(&ucg, 0, 52, 0, src3);

	memset(src4, 0, sizeof(src4));
	sprintf(src4, " Humi = %3d %%   ", humidity);
	ucg_DrawString(&ucg, 0, 72, 0, src4);

	memset(src5, 0, sizeof(src5));
	sprintf(src5, " Light = %d lux  ", Kanman_light);
	ucg_DrawString(&ucg, 0, 92, 0, src5);
}
/**
* @func 	Scan_1s
* @brief	Scan period 1s
* @param	None
* @retval	None
*/
void Scan_TimeSensor(uint32_t byRepeats)
{
	if (idTimer != NO_TIMER) {
	TimerStop(idTimer);
	}
	idTimer = TimerStart("Scan_sensor", byRepeats, TIMER_REPEAT_FOREVER, (void*) Scan_SensorLCD, NULL);
}

 void delay_ms(uint32_t ms) {

	uint32_t startTime = GetMilSecTick(); //Lưu lại thời điểm hiện tại.
	while (CalculatorTime(startTime, GetMilSecTick()) <= ms);

//	Đợi cho đến khi hết khoảng time cài đặt

}

/**
 * @func   CalculatorTime
 * @brief  Caculator time button
 * @param  None
 * @retval None
 */
 uint32_t CalculatorTime(uint32_t dwTimeInit, uint32_t dwTimeCurrent) {
 	uint32_t dwTimeTotal;
 	if (dwTimeCurrent >= dwTimeInit) {
 		dwTimeTotal = dwTimeCurrent - dwTimeInit;
 	} else {
 		dwTimeTotal = 0xFFFFFFFFU + dwTimeCurrent - dwTimeInit;
 	}
 	return dwTimeTotal;

 }

 static void AppCommon()
 {
 	 SystemCoreClockUpdate();
 	 TimerInit();

     SPI1_Init();
     I2C_Init_temphumi();

     TemHumSensor_Init();

     LightSensor_AdcInit();		//Khởi tạo ngoại vi ADC ở chế độ Polling.
     LedControl_TimerOCInit();   //Khởi tạo ngoại vi TIMER ở chế độ băm xung Output Compare.

     KalmanFilterInit(2, 2, 0.001); // Khởi tạo bộ lọc Kalman

     Ucglib4WireSWSPI_begin(&ucg, UCG_FONT_MODE_SOLID); //là hàm khởi tạo LCD.
     ucg_ClearScreen(&ucg);
     ucg_SetFont(&ucg, ucg_font_ncenR12_hr);
     ucg_SetColor(&ucg, 0, 255, 255, 255);
     ucg_SetColor(&ucg, 1, 0, 0, 0);
     ucg_SetRotate180(&ucg);
     Scan_TimeSensor(5000);
     time_initial = GetMilSecTick();
 }

static void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA, GPIOB and GPIOC Clocks enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN | SPI1_RS_PIN | SPI1_MODE_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_CS_PIN | SPI1_ENABLE_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_RST_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**
 * @func   I2C_Init_temphumi
 * @brief  Init I2C temp and humi
 * @param  None
 * @retval None
 */
static void I2C_Init_temphumi(void)
{
// Initialization struct
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// Step 1: Initialize I2C
	RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx_SENSOR, &I2C_InitStruct);
	I2C_Cmd(I2Cx_SENSOR, ENABLE);

	// Step 2: Initialize GPIO as open drain alternate function
	RCC_AHB1PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);

	/* Connect PXx to I2C_SCL */
	GPIO_PinAFConfig(I2C_GPIO, GPIO_PinSource8, GPIO_AF_I2C1);

	/* Connect PXx to I2C_SDA */
	GPIO_PinAFConfig(I2C_GPIO, GPIO_PinSource9, GPIO_AF_I2C1);
}

/**
 * @func   LightSensor_AdcInit
 * @brief  Light Sensor ADC Polling initialize
 * @param  None
 * @retval None
 */
static void LightSensor_AdcInit(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable peripheral clocks ***********************************************/
	RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);

	/* ADC Deinit ***************************************************/
	ADC_DeInit();

	/* ADC Common Init ********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init **************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//Polling Configure
	ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);
	ADC_ContinuousModeCmd(ADC1, DISABLE);
	ADC_DiscModeChannelCountConfig(ADC1, 1);
	ADC_DiscModeCmd(ADC1, ENABLE);


	/* ADC1 regular channel15 configuration ************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_15Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}


/**
 * @func   LedControl_TimerOCInit
 * @brief  Control Led with Timer Output Capture
 * @param  None
 * @retval None
 */
static void LedControl_TimerOCInit(void)
{
	GPIO_InitTypeDef 			GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef			TIM_OC_InitStruct;


	// GPIO Configure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);

	//TimeBase Configure

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;

	/************************************************************************
	 * 	FPwm = Fclk / (ARR+1) * (PSC + 1)																	*
	 ************************************************************************/
	TIM_TimeBaseInitStruct.TIM_Period = 8399; // ARR = 8399
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);


	//TimeIC Configure
	TIM_OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OC_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_InitStruct.TIM_Pulse = 0;
	TIM_OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC4Init(TIM1, &TIM_OC_InitStruct);

	TIM_Cmd(TIM1, ENABLE);


	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


/**
 * @func   LightSensor_AdcPollingRead
 * @brief  Read value from ADC
 * @param  None
 * @retval None
 */
uint16_t LightSensor_AdcPollingRead(void)
{
	uint16_t result = 0;

	//  Bắt đầu quá trình chuyển đổi dữ liệu ADC.
	ADC_SoftwareStartConv(ADCx_SENSOR);

	// Đợi quá trình chuyển đổi được hoàn tất để đọc dữ liệu.
	while (ADC_GetFlagStatus(ADCx_SENSOR, ADC_FLAG_EOC) == RESET);

	// Read value
	result = ADC_GetConversionValue(ADCx_SENSOR);

	return result;

}

/**
 * @func   Kanman_Light
 * @brief  Use Kanman filter for Light Sensor
 * @param  None
 * @retval None
 */
uint16_t Kanman_Light(uint16_t lightLevel)
{
	lightLevelAfterFilter = KalmanFilter_updateEstimate(lightLevel);

	return lightLevelAfterFilter;
}

/**
 * @func   LedControl_TimerOCSetPwm
 * @brief  Control Led with PWM
 * @param  None
 * @retval None
 */
static void LedControl_TimerOCSetPwm(uint32_t Compare)
{
	TIM_SetCompare4(TIM1,Compare);
}
/**
 * @func   ABL_Process
 * @brief  AutoBrightness use Light Sensor
 * @param  None
 * @retval None
 */
static void ABL_Process(void)
{
	uint32_t dwTimeCurrent;
	static uint32_t dwTimeTotal, dwTimeInit;

	dwTimeCurrent = GetMilSecTick();

	if(dwTimeCurrent >= dwTimeInit)
	{
		dwTimeTotal += dwTimeCurrent - dwTimeInit;
	}
	else
	{
		dwTimeTotal += 0xFFFFFFFFU - dwTimeCurrent + dwTimeInit;
	}

	if(dwTimeTotal >= 100)
	{
		dwTimeTotal = 0;
		AdcValue  = LightSensor_AdcPollingRead(); //Đọc giá trị ADC đo được từ cảm biến ánh sáng
		Kanman_light = Kanman_Light(AdcValue);


		LedControl_TimerOCSetPwm(Kanman_light);

		ucg_DrawString(&ucg,0 , 32,0 , "Nhom 1");
		memset(src3, 0, sizeof(src3));
		sprintf(src3, " Temp = %d oC  ", temperature);
		ucg_DrawString(&ucg, 0, 52, 0, src3);

		memset(src4, 0, sizeof(src4));
		sprintf(src4, " Humi = %3d %%   ", humidity);
		ucg_DrawString(&ucg, 0, 72, 0, src4);

		memset(src5, 0, sizeof(src5));
		sprintf(src5, " Light = %d lux  ", Kanman_light);
		ucg_DrawString(&ucg, 0, 92, 0, src5);
	}
	dwTimeInit = dwTimeCurrent;
}
