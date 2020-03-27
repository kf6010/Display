/*
 * Simple program for FRDM-K64F with CAN loopback 
 *
 * Implemented using mbed OS 5 and threads scheduled according to RMS
 *
 * DK - 02-Feb-2019
 */

/* TODO:
 * comments like this give some additional guidance for the assignment
 * with a view to working alone rather than in a pair.
 * Reflecting the current distancing situation.
 *                                              Alun     27 March 2020
 *   I've commented out sections of code that are
 *   unneeded for the assignment.
 *   Please delete commented-out, unneeded, and redundant sections
 *   of code before submitting the source.
 *   It makes the job of marking and reviewing easier.
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
static volatile canMessage_t rxMsg;
static volatile bool rxDone;

/* unneeded threads
Thread writer(osPriorityRealtime);
Thread reader(osPriorityRealtime1);
*/

/* TODO:
 * Declare threads for the RequestT and DisplayT tasks
 */


static void led1Toggle(void);

/* TODO: you still need the CAN interrupt handler */
static void canHandler(void);

/* unneeded headers 
static void canWriteTask(void);
static void canReadTask(void);
*/

/* TODO:
 * You need two tasks(threads)
 *     RequestT
 *     DisplayT
 */
void RequestT(void);
void DisplayT(void);

int main()
{
    osStatus status;

    red = 0;

	/* TODO:
	 * Normally you would CAN connect two boards,
	 * If you are working alone, or with a single board
	 * then you will have to use the CAN adaptor in
	 * `loopback' mode.
	 */
    canInit(BD125000, false);

    pc.printf("Display -- Loopback test\n");

	/* YOU don't need the read and write tasks from the exercises.
	 * but they ARE good examples to follow
	 */
	/* comment out unneeded code:
        status = reader.start(canReadTask);
        assert(osOK == status);
        status = writer.start(canWriteTask);
        assert(osOK == status);
	*/

	/* TODO:
	 * Create and start RequestT and DisplayT tasks
	 */


    while (true) {
        led1Toggle();
        wait_ms(500);
    }
}


/* TODO:
 * A simple interrupt handler for CAN message reception.
 * Ideally this should release DisplayT using a semaphore
 * when a message is available
 */
void canHandler(void)
{
    canTransferRxFrame(&rxMsg);
    rxDone = true;
}

/* TODO:
 * Periodic task to send ReqT messages to TempT task.
 */
void RequestT(void)
{
	/* TODO:
	 * populate message with ID and fields
	 */
	static canMessage_t ReqT;

	while(true) { /* OS Thread so needs while-true loop */
		/* TODO:
		 * Send the ReqT message.
		 * If you are using the loopback device, the message is echoed back as 
		 * the AckT message so populate it with some data as if from TempT in
		 * EngineMonitor.
		 */

		/* TODO:
		 * Task should have a 500ms period,
		 * use the appropriate delay here.
		 */
	}
}

/* TODO:
 * Task to respond to AckT messages,
 * unpack the data from the message and display information
 * on the pc serial communications.
 */
void DisplayT(void)
{
	/* TODO:
	 * declare needed data and variables
	 */
	static canMessage_t AckT;

	while(true) { /* OS Thread so needs while-true loop */
		/* TODO: wait for semaphore */
		/* TODO: unpack data from AckT message */
		/* TODO: display values on PC */
	}
}

/*-------------------------------*/
static void led1Toggle(void)
{
    red = 1 - red;
}

/* Transmit CAN message with arbitrary id and 8 bytes of
 * data consisting of a repeated count of the number of transmitted messages
 */
/*
void canWriteTask(void)
{

    static canMessage_t txMsg = { 0x23, 8, 0, 0 };
    bool txOk;

    while (true) {
        // Transmit message on CAN 
        txOk = canWrite(&txMsg);
        if (txOk) {
            txCount += 1;
            txMsg.dataA = txCount;
            txMsg.dataB = txCount;
        }
        wait_ms(250);
    }
}
*/

/* Read and display messages arriving on the CAN port */
/*
void canReadTask(void)
{
    canRxInterrupt(canHandler); // configure CAN to interrupt on message reception

    rxDone = false;
    while (true) {
        if (rxDone) {           // rxDone could be better handled by a semaphore
            rxDone = false;
            pc.printf
                ("ID: 0x%lx LEN: 0x%01lx DATA_A: 0x%08lx DATA_B: 0x%08lx\n",
                 rxMsg.id, rxMsg.len, rxMsg.dataA, rxMsg.dataB);
            rxCount += 1;
        }
        wait_ms(100);
    }
}
*/
