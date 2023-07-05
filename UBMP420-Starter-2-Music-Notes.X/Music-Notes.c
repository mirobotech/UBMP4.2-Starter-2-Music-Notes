/*==============================================================================
 Project: Music-Notes                   Activity: mirobo.tech/ubmp4-starter-2
 Date:    July 5, 2023
 
 This starter programming project for the mirobo.tech UBMP4 demonstrates two
 simple functions for making music note frequencies. The first one relies on
 pre-calculated period and cycle variables from the Google Sheet linked below:
 https://docs.google.com/spreadsheets/d/1L8dXdkF4tFPJPydvPqrTu1SUgoiHRLngb1Igl7BYPbk/edit?usp=sharing
 
 The second method uses formulas developed in the Google sheet to create a more
 easily usable function that will play notes for a selected time period.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Music note 1/2 period delay in microseconds (us) - can be deleted after
// working through the Programming Analysis Activities
#define C5Period    956
#define C6Period    478

// Music note frequencies in Hertz (Hz)
#define A4      440
#define C5      523
#define Cs5     554
#define E5      659
#define A5      880

// Simple sound function to play the requested number of cycles of 1/2 wave note 
// periods. Period  length is dependent on microcontroller speed. 
// Note wave periods and cycles are from the shared Google Sheet (link at top).
// Usage: play_sound(879, 523); // Play note C5 for 523 1/2-periods (1/2 second)
void play_sound(unsigned int period, unsigned int cycles)
{
    for(cycles; cycles != 0; cycles--)  // Total number of 1/2 period cycles
    {
        BEEPER = !BEEPER;               // Toggle beeper pin
        for(unsigned int i = period; i != 0; i--);  // Period delay
    }
}

// Advanced sound function to play note frequencies (in Hertz) for the specified
// duration (in milliseconds). Calculations are optimized to fit within 16-bit
// integer values (limiting frequencies to between approximately 440Hz and
// 10000 Hz, and durations to between approximately 20 ms and 6000 ms) to speed
// up calculations. Results are approximate, but generally good enough. 
// Usage: play_freq(523, 500);      // Play C5 for 500 ms
void play_freq(unsigned int frequency, unsigned int duration)
{
    unsigned int period = 57600 / (frequency / 8) - 2;
    unsigned int cycles = (duration * 10) / (50000 / frequency) * 10;
    for(cycles; cycles != 0; cycles--)  // Total number of 1/2 period cycles
    {
        BEEPER = !BEEPER;               // Toggle beeper pin
        for(unsigned int i = period; i != 0; i--);  // Period delay
    }    
}

int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure I/O for on-board UBMP4 devices
	
    while(1)
    {
        if(SW2 == 0)
        {
            // Repeat note for the required duration
            for(unsigned int cycles = 523; cycles != 0; cycles --)
            {
                BEEPER = !BEEPER;
                __delay_us(C5Period);   // Delay for note 1/2 period
            }
        }

        if(SW3 == 0)
        {
            // Repeat note for the required duration
            for(unsigned int cycles = 523; cycles != 0; cycles --)
            {
                BEEPER = !BEEPER;
                __delay_us(C6Period);   // Delay for note 1/2 period
            }
        }

        if(SW4 == 0)
        {
            // Create a note using a delay value and number of cycles
            play_sound(879, 523);
        }
        
        if(SW5 == 0)
        {
            // Create a note using frequency and time
            play_freq(C5, 500);
        }

        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}
        

/* Learn More - Program Analysis Activities
 * 
 * 1.   Music notes can be produced by toggling the piezo beeper pin on and off
 *      at 1/2 of the time period of the note frequency.
 * 
 *      For example, the note commonly referred to as 'middle-C' on a piano has
 *      a frequency of 523.25 Hz. The time period is the reciprocal of the 
 *      frequency, and can be calculated by dividing the frequency into 1, e.g.:
 * 
 *      T = 1 / 523.25 , or approximately 1911 microseconds.
 * 
 *      Since the time period represents the entire length of a note wave --
 *      including both the positive and negative wave peaks -- the time period
 *      must be divided by 2. This gives the 1/2 period durations corresponding
 *      to the positive wave peak (when the beeper will be turned on) and the
 *      negative wave peak (when the beeper will be turned off):
 * 
 *      1911 / 2 = 955.5 microseconds
 * 
 *      This 1/2 period value cannot be used as is. The __delay_us() function
 *      only accepts integers as input, so the the delay period value in the
 *      program is rounded to 956.
 * 
 *      Great! The proper note period for both the positive and negative note
 *      peaks has been calculated, and toggling the beeper output pin every
 *      period will create the note. But, how long will the note play when
 *      the button is pressed?
 * 
 * 2.   To automate making music, a program will need to play a note for a
 *      specific duration. A simple way to do this is to repeat the beeper
 *      toggling code in a loop. Replace the code in the if statement in the
 *      program above with the for loop code, below:

        if(SW2 == 0)
        {
            // Repeat note for the required duration
            for(unsigned int cycles = 500; cycles != 0; cycles --)
            {
                BEEPER = !BEEPER;
                __delay_us(C5Period);   // Delay for note 1/2 period
            }
        }

 *      The loop toggles the BEEPER pin, waits for a delay of 956 microseconds
 *      (defined as the C5Period constant to make the code more meaningful and
 *      readable, as C5 is middle-C), and uses a loop to continue producing the
 *      note wave for a total of 500 half-period cycles.
 * 
 *      Next, add a second note one octave higher (having twice the frequency,
 *      or 1/2 of the note period) than the first note by adding the following
 *      program code after the SW2 if statement:

        if(SW3 == 0)
        {
            // Repeat note for the required duration
            for(unsigned int cycles = 500; cycles != 0; cycles --)
            {
                BEEPER = !BEEPER;
                __delay_us(C6Period);   // Delay for note 1/2 period
            }
        }

 *      Now, pressing SW3 will play the second note for the same number of
 *      1/2 period cycles as the first, but this doesn't mean the notes will
 *      play for the same length of time. Try it! What number of cycles can the
 *      second loop be set to so that the note durations will be identical?
 * 
 * 3.   Ready to play a song? Not so fast... there are a lot of variables to
 *      figure out first:
 * 
 *      1.  Note frequencies - activity 1, above, demonstrated how to calculate
 *          the 1/2 period delays required to make any note.
 *      2.  Same note duration - activity 2, above, let you calculate the number
 *          of cycles to play two different notes for the same length of time.
 *      3.  Different note durations - in songs and music, not all notes play
 *          for the same length of time. Written music notation has whole notes,
 *          half-notes, quarter-notes, eighth-notes, etc., and each of these
 *          note lengths can be extended by another half note-length using
 *          dotted-notation.
 *      4.  Song tempo - the speed at which a song will be played will affect
 *          the length of each note.
 * 
 *      The required variables will need to be calculated for each duration of
 *      every note used in a song. Doing all of the calculations is not overly
 *      difficult, just time consuming. Fortunately, a spreadsheet can help.
 *      Open the Google Sheet linked below to see an example note calculator:
 * 
 *  https://docs.google.com/spreadsheets/d/1L8dXdkF4tFPJPydvPqrTu1SUgoiHRLngb1Igl7BYPbk/edit?usp=sharing
 * 
 *      Try the spreadsheet by adding the frequency for C6:
 * 
 *      1.  Put the note name C6 into cell A5 of the spreadsheet.
 *      2.  Enter the frequency for C6 (1046.5 Hz) into cell B5.
 *      3.  Select cells C4-I4, and copy the cells down to C5-I5.
 *      4.  Cell D5 calculates the 1/2 period note delay that can be used
 *          by the __delay_us() function to make the note.
 *      5.  Cells E5-I5 generate the number of cycles required to make five
 *          common note durations at the selected tempo. The value 523 for note
 *          C5 in cell F4 (the half-note duration) is close to the 500 cycles
 *          used in the example code, above. Change the tempo in cell B1 to 63.
 *          This will change the C5 cycle count to 498, even closer to 500.
 * 
 *      How does the cycle count in cell F5 compare to the cycle value you
 *      chose in activity 2, above?
 * 
 *      Set a new tempo in cell B1 (e.g. 100). Use the half-note cycle counts
 *      from the spreadsheet in your program code to update the for loop cycle
 *      values and confirm that both of your two notes, C5 and C6, play for the
 *      same length of time.
 * 
 * 4.   Great, you're now ready to make a song! But, before you do, you may 
 *      have noticed a big problem with this example code. Every note of the
 *      song will need its own program loop using the period and cycle variables
 *      calculated ahead of time. Repeating these loops for each note of a song
 *      will make the program really big.
 * 
 *      It would be much better to make a function containing note loop that
 *      can be called repeatedly instead of repeating the note loops in the
 *      program. Here is an example sound function that would be expected to
 *      work (spoiler, this code will not work):

void play_sound(unsigned int period, unsigned int cycles)
{
    for(cycles; cycles != 0; cycles--)  // Total number of 1/2 period cycles
    {
        BEEPER = !BEEPER;               // Toggle beeper pin
        __delay_us(period);             // 1/2 period delay
    }
}

 *      Why doesn't the above code work? The __delay_us() function requires a
 *      variable coded at compile time -- it won't work with a variable.
 * 
 *      The fix for this problem is simple, and yet a bit frustrating. The delay
 *      function in the example above can be replaced with a delay loop, as
 *      shown in the code below. While the loop is a simple solution, it
 *      frustratingly counts loop cycles, not time, so there is no simple
 *      way of specifying a precise period delay. This loop will also take
 *      more time to run when using MPLAB's free compiler to build the program
 *      than when using the optimized paid compiler instead. Sigh... 
 * 
 *      Add this function to your program. It will be used in the next examples.

// Simple sound function to play the requested number of cycles of 1/2 wave note 
// periods. Period  length is dependent on microcontroller speed. 
// Note wave periods and cycles are from the shared Google Sheet (link at top).
// Usage: play_sound(879, 523); // Play note C5 for 523 1/2-periods (1/2 second)
void play_sound(unsigned int period, unsigned int cycles)
{
    for(cycles; cycles != 0; cycles--)  // Total number of 1/2 period cycles
    {
        BEEPER = !BEEPER;               // Toggle beeper pin
        for(unsigned int i = period; i != 0; i--);  // Period delay
    }
}

 *      The solution to turning this loop into a known period delay will require
 *      another calculation, and it's already built into the spreadsheet used
 *      earlier. The value in cell K4 will generate the required 1/2 period
 *      delay for note C5. Let's try it out. Add this code to the main part of
 *      the program below the other if statements:

        if(SW4 == 0)
        {
            // Create a note using a delay value and number of cycles
            play_sound(879, 523);
        }
        
 *      This function call should now cause note C5 to be played for one half
 *      of a second whenever SW4 is pressed. (Change the cycle count in the SW2
 *      if statement to 523, then pressing SW2 will match the both the pitch
 *      and duration of pressing SW4.)
 * 
 *      What value would you have to replace the cycle count of the C6 note in
 *      the SW3 if condition by to match the duration of the two C5 notes?
 * 
 *  5.  While a song could now be made using a list of play_sound() functions
 *      as demonstrated in step 4, a few more modifications will make for a 
 *      much easier to use note function. The biggest drawback of this function
 *      is the disconnect between the note frequencies and the calculated delay
 *      values. Another problem is having different cycle values for the same
 *      duration of different notes. This last factor makes it difficult to
 *      change the speed at which a song is played, for example.
 * 
 *      Since all of the calculations in the spreadsheet are based on the actual
 *      note frequencies (in Hertz), it should be possible to have the program
 *      calculate all of the required values by itself. We have to be careful
 *      with the algorithm used to do this, however, as the program uses 16-bit
 *      integer values to save memory space and processing time in the limited
 *      and (relatively) slow microcontroller. (Fun fact: the PIC16F1459 used
 *      in UBMP4 and CHRP4 does not have a multiply or a divide instruction!)
 * 
 *      By carefully crafting the algorithms, and by using a limited set of both
 *      frequencies and durations, the microcontroller can calculate both the
 *      note 1/2 period delays and the cycle counts for these limited notes.
 * 
 *      Add this new function to your program:

// Advanced sound function to play note frequencies (in Hertz) for the specified
// duration (in milliseconds). Calculations are optimized to fit within 16-bit
// integer values (limiting frequencies to between approximately 440Hz and
// 10000 Hz, and durations to between approximately 20 ms and 6000 ms) to speed
// up calculations. Results are approximate, but generally good enough.
// Usage: play_freq(523, 500);      // Play C5 for 500 ms
void play_freq(unsigned int frequency, unsigned int duration)
{
    unsigned int period = 57600 / (frequency / 8) - 2;
    unsigned int cycles = (duration * 10) / (50000 / frequency) * 10;
    for(cycles; cycles != 0; cycles--)  // Total number of 1/2 period cycles
    {
        BEEPER = !BEEPER;               // Toggle beeper pin
        for(unsigned int i = period; i != 0; i--);  // Period delay
    }    
}

 *      Next, add the following code to your program's main function:
        
        if(SW5 == 0)
        {
            // Create a note using frequency and time
            play_freq(C5, 500);
        }

 *      Note that this new function uses the frequency 'C5'. The program will
 *      not understand C5 as it has not been defined yet. Add these definitions
 *      to the top of the program code:

// Music note frequencies in Hertz (Hz)
#define A4      440
#define C5      523
#define Cs5     554
#define E5      659
#define A5      880

 *      Of course, the actual frequency can be used by the play_freq() function,
 *      but using a descriptive definition like C5 makes it easier to transcribe
 *      and verify music notes from printed music.
 * 
 *      Give your program another try. Switches SW2, SW4, and SW5 should each
 *      play note C5 for half of a second, using three different methods of
 *      generating the sounds.
 * 
 * 6.   Make a chord progression using the notes above. Create the code to play
 *      A4, Cs5, E5, and A5 in sequence when a button is pressed. Use durations
 *      of 200 ms for the first three notes, and 800 ms for the final note.
 */
