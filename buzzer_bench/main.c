/**
 * @file main.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @date Oct 4th/2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#include <wiringPi.h>
#include <softTone.h>

#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

#define SCALE_MAX 20

int scale[20] =
    { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 170, 180, 200, 300, 400,
	500
};

typedef struct {
	int waitForRelease;
} button_s;

// Pins used (wiringPi numbers)
const int UP_TMP_PIN = 6;
const int DN_TMP_PIN = 13;
const int BEEPER_PIN = 12;

static int scale_val;

/**
 * usage(const char *progName)
 * @brief program usage
 * @param progName
 * @return -1 for failure
 */
static int usage(const char *progName)
{
	fprintf(stderr, "Set it up\n", progName);
	return EXIT_FAILURE;
}

/**
 * print_banner()
 * @brief Prints app banner
 */
static inline void print_banner()
{
	printf("\nWearHacks2015 Buzzer Bench\n\n");
	printf("Author: Ronnie brash (ron.brash@gmail.com)\n");
	printf("------------------------------------------\n");
}


/**
 * turn_off_beeper()
 * @brief Turn off beeper
 */
static void turn_off_beeper()
{
	softToneWrite(BEEPER_PIN, 0);
}

/**
 * setup_buttons()
 * @brief Setup buttons to be used by the external 
 * user interface - to be manipulated by the user
 */
static inline void setup_buttons()
{
	/*
	 * For each button:
	 * 1.) Set button as INPUT
	 * 2.) Enable pull-up resistor on button
	 */
	pinMode(UP_TMP_PIN, INPUT);
	pullUpDnControl(UP_TMP_PIN, PUD_UP);

	pinMode(DN_TMP_PIN, INPUT);
	pullUpDnControl(DN_TMP_PIN, PUD_UP);

}

/**
 * main(int argc, char *argv[])
 * @brief main function
 * @param argc
 * @param argv
 * @return 0 for success, -1 for error
 */
int main(int argc, char *argv[])
{
	button_s b_state = { 0 };
	b_state.waitForRelease = FALSE;
	scale_val = 0;

	print_banner();

	wiringPiSetup();
	softToneCreate(BEEPER_PIN);

	// Initialize the user interface buttons
	setup_buttons();

	// Set priority
	piHiPri(99);

	turn_off_beeper();
	for (;;) {


		while (digitalRead(UP_TMP_PIN) == HIGH) {

			if (scale_val < SCALE_MAX) {
				scale_val++;
			}
			
			printf("scale %d %d\n", scale_val, scale[scale_val]);
			softToneWrite(BEEPER_PIN, scale[scale_val]);
			delay(500);

		}

		while (digitalRead(DN_TMP_PIN) == HIGH) {

			if (scale_val > 0) {
				scale_val--;
			}
			
			printf("scale %d %d\n", scale_val, scale[scale_val]);
			softToneWrite(BEEPER_PIN, scale[scale_val]);
			delay(500);

		}
	}

	return 0;
}
