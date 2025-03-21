#include "rfid.h"
#include "spi.h"
#include "uart.h"

static void RFID_WriteReg(uint8_t reg, uint8_t value) {
    RFID_CS_LOW();
    SPI_SendByte((reg << 1) & 0x7E);
    SPI_SendByte(value);
    RFID_CS_HIGH();
}

static uint8_t RFID_ReadReg(uint8_t reg) {
    uint8_t result;
    RFID_CS_LOW();
    SPI_SendByte(((reg << 1) & 0x7E) | 0x80);
    result = SPI_SendByte(MFRC522_DUMMY);
    RFID_CS_HIGH();
    return result;
}

static void RFID_AntennaOn(void) {
    uint8_t value = RFID_ReadReg(MFRC522_REG_TX_CONTROL);
    if ((value & 0x03) != 0x03) {
        RFID_WriteReg(MFRC522_REG_TX_CONTROL, value | 0x03);
    }
}

static uint8_t RFID_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen) {
    uint8_t status = 0, irqEn = 0x77, waitIRq = 0x30, n, lastBits;
    uint16_t i;

    RFID_WriteReg(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
    RFID_WriteReg(MFRC522_REG_COMM_IRQ, 0x00);
    RFID_WriteReg(MFRC522_REG_FIFO_LEVEL, 0x80);
    RFID_WriteReg(MFRC522_REG_COMMAND, 0x00);

    for (i = 0; i < sendLen; i++) {
        RFID_WriteReg(MFRC522_REG_FIFO_DATA, sendData[i]);
    }

    RFID_WriteReg(MFRC522_REG_COMMAND, command);
    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x80);

    i = 2000;
    do {
        n = RFID_ReadReg(MFRC522_REG_COMM_IRQ);
        i--;
    } while ((i != 0) && !(n & waitIRq));

    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x00);

    if (i != 0 && !(RFID_ReadReg(MFRC522_REG_ERROR) & 0x1B)) {
        status = 1;
        n = RFID_ReadReg(MFRC522_REG_FIFO_LEVEL);
        lastBits = RFID_ReadReg(MFRC522_REG_CONTROL) & 0x07;
        *backLen = (lastBits) ? ((n - 1) * 8 + lastBits) : (n * 8);
        n = (n > MFRC522_MAX_LEN) ? MFRC522_MAX_LEN : n;
        for (i = 0; i < n; i++) {
            backData[i] = RFID_ReadReg(MFRC522_REG_FIFO_DATA);
        }
    }
    return status;
}

void RFID_Init(void) {
    GPIO_InitTypeDef gpioInit;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &gpioInit);
    RFID_CS_HIGH();

    SPI_Config();
    RFID_WriteReg(MFRC522_REG_COMMAND, PCD_RESETPHASE);
    RFID_AntennaOn();
}

uint8_t RFID_Request(uint8_t *tagType) {
    uint8_t status, backBits, buffer[MFRC522_MAX_LEN];
    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x07);
    buffer[0] = PICC_REQIDL;
    status = RFID_ToCard(PCD_TRANSCEIVE, buffer, 1, buffer, &backBits);
    if (status && backBits == 0x10) {
        *tagType = buffer[0];
        *(tagType + 1) = buffer[1];
    }
    return status;
}

uint8_t RFID_Anticoll(uint8_t *serialNum) {
    uint8_t status, i, backBits, buffer[MFRC522_MAX_LEN];
    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x00);
    buffer[0] = PICC_ANTICOLL;
    buffer[1] = 0x20;
    status = RFID_ToCard(PCD_TRANSCEIVE, buffer, 2, buffer, &backBits);
    if (status) {
        for (i = 0; i < 4; i++) serialNum[i] = buffer[i];
    }
		return status;
}

void RFID_Halt(void) {
    uint8_t buffer[4], backLen;
    buffer[0] = PICC_HALT;
    buffer[1] = 0;
    RFID_ToCard(PCD_TRANSCEIVE, buffer, 2, buffer, &backLen);
}

void RFID_ReadCard(void) {
    uint8_t tagType[2], serialNum[5], status, i;
    status = RFID_Request(tagType);
    if (status) {
        USART_SendString("Card detected! Type: ");
        USART_SendHex(tagType[0]);
        USART_SendHex(tagType[1]);
        USART_SendString("\r\n");

        status = RFID_Anticoll(serialNum);
        if (status) {
            USART_SendString("Card UID: ");
            for (i = 0; i < 4; i++) {
                USART_SendHex(serialNum[i]);
                USART_SendChar(' ');
            }
            USART_SendString("\r\n");
        } else {
            USART_SendString("Anticoll error\r\n");
        }
        RFID_Halt();
    }
}