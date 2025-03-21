#ifndef __RFID_H__
#define __RFID_H__

#include "stm32f10x.h"

#define RFID_CS_LOW() GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define RFID_CS_HIGH() GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define PCD_RESETPHASE  0x0F
#define PCD_TRANSCEIVE  0x0C
#define PICC_REQIDL     0x26
#define PICC_ANTICOLL   0x93
#define PICC_HALT       0x50

#define MFRC522_REG_COMMAND     0x01
#define MFRC522_REG_FIFO_DATA   0x09
#define MFRC522_REG_FIFO_LEVEL  0x0A
#define MFRC522_REG_CONTROL     0x0C
#define MFRC522_REG_BIT_FRAMING 0x0D
#define MFRC522_REG_COMM_IE_N   0x02
#define MFRC522_REG_COMM_IRQ    0x04
#define MFRC522_REG_ERROR       0x06
#define MFRC522_REG_TX_CONTROL  0x14
#define MFRC522_DUMMY           0x00
#define MFRC522_MAX_LEN         16

void RFID_Init(void);
uint8_t RFID_Request(uint8_t *tagType);
uint8_t RFID_Anticoll(uint8_t *serialNum);
void RFID_Halt(void);
void RFID_ReadCard(void);

#endif