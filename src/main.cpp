/*
 * Simple program for FRDM-K64F with CAN loopback 
 *
 * Implemented using mbed OS 5 and threads scheduled according to RMS
 *
 * DK - 02-Feb-2019
 */


#include <stdbool.h>
#include <stdint.h>
#include <mbed.h>
#include <stdio.h>
#include <assert.h>
#include "can.h"

static DigitalOut red(LED_RED);
static Serial pc(USBTX, USBRX, 115200);
static uint32_t txCount;
static uint32_t rxCount;
//static volatile canMessage_t rxMsg;
static volatile bool rxDone;

Thread writer(osPriorityRealtime);
Thread reader(osPriorityRealtime1);

static void led1Toggle(void);
static void canWriteTask(void);
static void canReadTask(void);
static void canHandler(void);

static canMessage_t rxMsg;

int main()
{
    osStatus status;

    red = 0;
    canInit(BD125000, false); /* Do not want loopback mode */
    pc.printf("Display -- \n");

    status = reader.start(canReadTask);
    assert(osOK == status);
    status = writer.start(canWriteTask);
    assert(osOK == status);

    while (true) {
        led1Toggle();
        ThisThread::sleep_for(500);
    }
}

static void led1Toggle(void)
{
    red = 1 - red;
}

/* Transmit CAN message with arbitrary id and 8 bytes of
 * data consisting of a repeated count of the number of transmitted messages
 */
void canWriteTask(void)
{

    static canMessage_t txMsg = { 0x25, 8, 0, 0 }; /* Need unique Ids for messages */
    bool txOk;

    while (true) {
        // Transmit message on CAN 
        txOk = canWrite(&txMsg);
        if (txOk) {
            txCount += 1;
            txMsg.dataA = txCount;
            txMsg.dataB = rxCount;
        }
        ThisThread::sleep_for(250);
    }
}

/* Read and display messages arriving on the CAN port */
void canReadTask(void)
{

    canRxInterrupt(canHandler); // configure CAN to interrupt on message reception

    rxDone = false;
    while (true) {
        if (rxDone) {           // rxDone could be better handled by a semaphore
            rxDone = false;
			rxCount+=1;
			canRead(&rxMsg);
            pc.printf
                ("ID: %#lx LEN: %lx DATA_A: %#06lx DATA_B: %#06lx\n",
                 rxMsg.id, rxMsg.len, rxMsg.dataA, rxMsg.dataB);
            rxCount += 1;
        }
        ThisThread::sleep_for(100);
    }
}

/* A simple interrupt handler for CAN message reception */
void canHandler(void)
{
    canTransferRxFrame(&rxMsg);
    rxDone = true;
}

