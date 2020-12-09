/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

/**
 * @file
 * @brief This file defines public functions offered by AST utilities API-Level 0.
 */

#include "Vector2.h"
#include "SdlApplication.h"

/////////////////////////////////////////////////
/////// I/O Functions
/////////////////////////////////////////////////

/**
 * @defgroup io_group I/O Functions
 * @brief The functions defined in this group are dedicated to basic input output operations.
 * @{
 */

/**
 * Outputs some simple text to standard output stream.
 * This function is mostly used to test if the compiler
 * tool chain is working.
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   SayHello();
 * 
 *   return 0;
 * }
 * ```
 */ 
void SayHello();

    // std::cout << "An error has occurred: " << GetLastErrorMessage() << std::endl;
    // std::cout << GetErrorDetails() << std::endl;


/**
 * Outputs the last error message including error details.
 * 
 * This is only a convenience function for error output. The same
 * result  could also be achieved with the method for error retrieval
 * and the Standard C++ library. An implementation of this function
 * could look like this:
 * 
 * ```
 * #include <iostream>
 * #include <AstUtils.h>
 * 
 * void SayError()
 * {
 *   std::cout << "An error has occurred: " << GetLastErrorMessage() << std::endl;
 *   std::cout << GetErrorDetails() << std::endl;
 * }
 * ```
 */
void SayError();

/**
 * Outputs the specified text using the standard output stream.
 * If `text` is a `nullptr` an empty line will be printed.
 * 
 * @param text  a C string holding the text to be printed
 * @param eol   whether 'end-of-line' should be added
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   SayText("Hello world");
 * 
 *   return 0;
 * }
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * 
 * int main()
 * {
 *   std::cout << "Hello World" << std::endl;
 * 
 *   return 0;
 * }
 * ```
 * 
 */
void SayText(const char* text = nullptr, bool eol = true);

/**
 * Outputs the specified integer value using the standard output stream.
 * 
 * @param value the integer value to be printed
 * @param eol   whether 'end-of-line' should be added
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   int x = 42;
 *   SayInt(x);
 * 
 *   return 0;
 * }
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * 
 * int main()
 * {
 *   int x = 42;
 *   std::cout << x << std::endl;
 * 
 *   return 0;
 * }
 * ```
 */
void SayInt(int value, bool eol = true);

/**
 * Outputs the specified double value using the standard output stream.
 * 
 * @param value the double value to be printed
 * @param eol   whether 'end-of-line' should be added
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   double x = 17.3;
 *   SayDouble(x);
 * 
 *   return 0;
 * }
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * 
 * int main()
 * {
 *   double x = 17.3;
 *   std::cout << x << std::endl;
 * 
 *   return 0;
 * }
 * ```
 */
void SayDouble(double value, bool eol = true);

/**
 * Outputs version information about this library.
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   SayVersion();
 * 
 *   return 0;
 * }
 * ```
 * 
 * Possible output:
 * ```
 * AST Utilities Version 1.0.0 (64 bit address space) [build: Aug 13 2020, 16:04:50]
 * ```
 */
void SayVersion();

/**
 * Outputs the elapsed time in a nice human readable way.
 * 
 * @param text   preceding text printed before elapsed time
 * 
 * @sa timer_group
 * 
 * **Example**
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   // Measure required time to calculate something.
 *   StartTimer();
 *   CalculateSomething();
 *   StopTimer();
 * 
 *   // Output required time.
 *   SayElapsedTime("Required Time");
 * 
 *   return 0;
 * }
 * ```
 * Possible output:
 * ```
 * Required Time 2 min 55 sec 84 ms
 * ```
 */
void SayElapsedTime(const char* text = nullptr);

/**
 * Reads an integer value from the standard input stream.
 * 
 * @param text   preceding text printed before the use input is collected
 * @return the integer value entered by the user
 * 
 * **Example**
 * ```
 * #include <AstUtils.h>
 * 
 * void main()
 * {
 *   int x;
 * 
 *   x = AskInt("Please enter a number:");
 *   Say("You have entered the following number ", false);
 *   SayInt(x);
 * 
 *   return 0;
 * }
 * ```
 * Possible output:
 * ```
 * Please enter a number: 42
 * You entered the following number 42
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * 
 * int main()
 * {
 *   int x;
 * 
 *   std::cout << "Please have enter a number: ";
 *   std::cin >> x;
 *   std::cout << "You have entered the following number " << x << std::endl;
 * 
 *   return 0;
 * }
 * ```
 */
int AskInt(const char* text = nullptr);

/**
 * Reads an double value from the standard input stream.
 * 
 * @param text   preceding text printed before the use input is collected
 * @return the double value entered by the user
 * 
 * **Example**
 * ```
 * #include <AstUtils.h>
 * 
 * void main()
 * {
 *   double x;
 * 
 *   x = AskInt("Please enter a real number:");
 *   Say("You have entered the following number");
 *   SayDouble(x);
 * 
 *   return 0;
 * }
 * ```
 * Possible output:
 * ```
 * Please enter a number: 17.3
 * You entered the following number
 * 17.3
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * 
 * int main()
 * {
 *   double x;
 * 
 *   std::cout << "Please have enter a real number: ";
 *   std::cin >> x;
 *   std::cout << "You have entered the following number" << std::endl;
 *   std::cout << x << std::endl;
 * 
 *   return 0;
 * }
 * ```
 * 
 * Using C++ streams also offers the option to avoid unnecessary line breaks 
 * and to write code in a more compact form, especially when including the 
 * namespace form the C++ Standard Library to avoid writing `std`.
 * 
 * ```
 * #include <iostream>
 * 
 * using namespace std;
 * 
 * int main()
 * {
 *   double x;
 *   
 *   cout << "Please have enter a real number: ";
 *   cin >> x;
 *   cout << "You have entered the number " << x << endl;
 * 
 *   return 0;
 * }
 * ```
 */
double AskDouble(const char* text = nullptr);

/**
 * Reads an float value from the standard input stream.
 * 
 * @param text   preceding text printed before the use input is collected
 * @return the float value entered by the user
 */
float AskFloat(const char* text = nullptr);

/**
 * Reads a string from the standard input stream.
 * 
 * @param text   preceding text printed before the use input is collected
 * @return the string entered by the user
 * 
 * **Example**
 * ```
 * #include <AstUtils.h>
 * 
 * void main()
 * {
 *   const char* name = AskString("Please enter your name:");
 *   SayText("Your name is ", false);
 *   SayText(name);
 * 
 *   return 0;
 * }
 * ```
 * Possible output:
 * ```
 * Please enter a number: Batman
 * Your name is Batman
 * ```
 * 
 * **Using the C++ Standard Library**
 * 
 * The same result can be achieved without ASTU using the 
 * stream-based input/output functionality of the C++ Standard Library.
 * 
 * ```
 * #include <iostream>
 * #include <string>
 * 
 * int main()
 * {
 *   std::string name;
 * 
 *   std::cout << "Please have enter your name: ";
 *   std::getline(std::cin, name);
 *   std::cout << "Your name is " << name << std::endl;
 * 
 *   return 0;
 * }
 * ```
 */
const char* AskString(const char* text = nullptr);

// TODO implement ask bool.
// bool AskBool(const char * text);


/**@}*/

/////////////////////////////////////////////////
/////// File Functions
/////////////////////////////////////////////////


/**
 * @defgroup File_group File I/O
 * @brief A collection of functions dedicated to reading and writing files.
 * @{
 */

int OpenFile(const char *filename, bool openForReading = true);
const char* ReadString();
double ReadDouble();
int ReadInt();
int CloseFile();
char ReadChar();
int SkipLine();
bool Readable();
bool CompareString(const char* s1, const char* s2);

/**@}*/

/////////////////////////////////////////////////
/////// Math Functions
/////////////////////////////////////////////////

/**
 * @defgroup math_group Mathematics
 * @brief A collection of functions dedicated to mathematical calculations.
 * @{
 */

/**
 * Converts degrees to radians.
 * 
 * @param deg   the angle in degrees
 * @return the angle in radians
 */
double ToRadians(double deg);

/**
 * Converts radians to degrees.
 * 
 * @param rad   the angle in radians
 * @return the angle in degrees
 */
double ToDegrees(double rad);

/**
 * Returns the smaller of the given values. 
 * 
 * **Using plain C++ without the Standard Library or ASTU**
 *  
 * The same result can be achieved without ASTU using plain C++
 * and the `<algorithm>` header.
 * 
 * ```cpp
 * #include <iostream>
 * #include <algorithm>
 * 
 * int main()
 * {
 *   int a = 42;
 *   int b = 17;
 *   int c = std::min(a, b);
 *  
 *   std::cout << "the minimum of " << a << " and " << b " is " << c << std::endl;
 *   return 0;
 * }
 * ```
 * 
 * @param a the first value
 * @param b the second value
 * @return The smaller of a and b
 */
int Minimum(int a, int b);

/**
 * Returns the smaller of the given values. 
 * 
 * The same result can be achieved without ASTU using plain C++
 * and the `<algorithm>` header by chaining
 * `std::min()` like this:
 *  
 * ```cpp
 * std::min(a, std::min(a, b))
 * ```
 *  
 * @param a the first value
 * @param b the second value
 * @param c the third value
 * @return the smaller of a, b and c
 */
int Minimum(int a, int b, int c);

int Maximum(int a, int b);
int Maximum(int a, int b, int c);

/**
 * Convenient type alias for astu::Vector2 template using double as data type.
 */
using Vector2d = astu::Vector2<double>;

/**
 * Returns a random number within the specified range.
 * This function will generate a random number using a 
 * pseudo random number generator.
 * 
 * The generated numbers are of type `double` and lie within the interval of
 * [minValue, maxValue).
 * 
 * *Note:* the notation for the interval *[minValue, maxValue)* means
 * that `minValue` is included but `maxValue` is not.
 * 
 * @param minValue  the minimum value
 * @param maxValue  the minimum value
 * @return the new random number
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   // Generates a random number in the interval of [100, 200).
 *   double x1 = GetRandomDouble(100, 200);
 * 
 *   // Generates a random number in the interval of [0, 1).
 *   double x2 = GetRandomDouble();
 * 
 *   return 0;
 * }
 * ```
 */
double GetRandomDouble(double minValue = 0.0, double maxValue = 1.0);

/**
 * Returns a random number within the specified range.
 * This function will generate a random number using a 
 * pseudo random number generator.
 * 
 * The generated numbers are of type `int` and lie within the interval of
 * [minValue, maxValue).
 * 
 * *Note:* the notation for the interval *[minValue, maxValue)* means
 * that `minValue` is included but `maxValue` is not.
 * 
 * @param minValue  the minimum value
 * @param maxValue  the minimum value
 * @return the new random number
 */
int GetRandomInt(int minValue = 0, int maxValue = 32767);

/**
 * Returns the given double value to the nearest integer.
 * 
 * @param value the double value to be rounded
 * @return the rounded integer value
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   SayText("The floating point value 17.3 is rounded to");
 *   SayInt( RoundToInt(17.3) );
 *   SayText();
 * 
 *   SayText("The floating point value 17.8 is rounded to");
 *   SayInt( RoundToInt(17.8) );
 *   SayText();
 * 
 *   SayText("The floating point value 17.5 is rounded to");
 *   SayInt( RoundToInt(17.5) );
 * 
 *   return 0;
 * }
 * ```
 * 
 * Output:
 * ```
 * The floating point value 17.3 is rounded to
 * 17
 * The floating point value 17.8 is rounded to
 * 18
 * The floating point value 17.5 is rounded to
 * 18
 * ```
 * **Using plain C++ without the Standard Library or ASTU**
 *  
 *The same result can be achieved without ASTU using plain C++.
 * Rounding a floating-point value to an integer can be done 
 * by casting the floating-point value to an int data type while 
 * adding 0.5.
 * 
 * ```cpp
 * double f = 17.7;
 * int i = static_cast<int>(f + 0.5);
 * ```
 * 
 * The cast will always truncate the floating-point value to an 
 * integer, ignoring the fractional part number. In case the 
 * fractional part of the floating-point value is below 0.5, 
 * adding 0.5 will not change the result. In case the number's 
 * fractional part is more than 0.5, adding 0.5 will change the 
 * number's integer part to the next higher value. For example, 
 * 17.7 + 0.5 results in 18.2, and casting away the fractional 
 * part results in 18.
 */
int RoundToInt(double value);

/**
 * Returns the greatest common divisor (gcd) of two numbers.
 * Both number must be greater or equal to zero.
 * 
 * @param a the first number
 * @param b the second number
 * @param the gcd of the two numbers
 */
int GreatestCommonDivisor(int a, int b);

/**
 * Returns the lowest common multiple (lcm) of two numbers.
 * Both number must be greater or equal to zero.
 * 
 * @param a the first number
 * @param b the second number
 * @param the lcm of the two numbers
 */
int LowestCommonMultiple(int a, int b);

/**
 * Randomly rearrange array of integers.
 * 
 * @param values    the array of integers to be shuffled
 * @param numValues the number of integers
 */
void Shuffle(int *values, int numValues);

/**
 * Tests whether a certain bit is set within an integer.
 * 
 * **Using plain C++ without the Standard Library or ASTU**
 * 
 * Testing whether a certain bit is set can be achieved by bit shifting
 * the value 1 (which has the first bit set and all others set to zero)
 * to the left, according to the bit to test. This new value can now
 * be processed with a bitwise AND operator (&) and the result
 * will be something other than zero if the bit is set. The entire
 * function could be implemented like this:
 * 
 * ```
 * bool IsBitSet(int value, int bit)
 * {
 *   return value & (1 << bit);
 * }
 * ```
 * 
 * @param value the integer value to test
 * @param bit   the n-th bit to test
 * @return `true` if the bit is set
 */
bool IsBitSet(int value, int bit);

/**
 * Sets a certain bit within an integer value.
 * 
 * @param value the integer value
 * @param bit   the n-th bit to set
 * @return the modified value with the bit set
 */
int SetBit(int value, int bit);

/**
 * Clears a certain bit within an integer value.
 * 
 * @param value the integer value
 * @param bit   the n-th bit to clear
 * @return the modified integer value with the bit cleared
 */
int ClearBit(int value, int bit);

/**@}*/

/**
 * @defgroup timer_group Timer
 * @brief The function in this module are dedicated to time measurement.
 * 
 * This module uses an internal representation of the currently measured time.
 * According to API-Level 0 this module does not expose any functions or
 * structures from the used C++ Standard Library. However, the current
 * implementation uses the high performance timer offered by the `<chrono>` 
 * header of the C++ Standard library.
 * 
 * This module offers only one timer and hence cannot be used to
 * measure the duration of calculation at once.
 * @{
 */

/**
 * Starts the internal timer.
 * When this function is called, the internal time measurement is reset to
 * zero and the timer is started.
 * This function is used in combination with StopTimer().
 * 
 * @sa GetMilliseconds()
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   StartTimer();
 *   CalculateSomething();
 *   StopTimer();
 * 
 *   // TODO output the elapsed time.
 * 
 *   return 0;
 * }
 * ```
 */
void StartTimer();

/**
 * Stops the internal timer.
 * Calling this function will stop the internal timer.
 * 
 * @sa StartTimer()
 */
void StopTimer();

/**
 * Returns the elapsed milliseconds.
 * This function returns the milliseconds elapsed between the two 
 * consecutive calls to StartTimer() and StopTimer().
 * 
 * @sa SayElapsedTime()
 * 
 * **Example**
 * 
 * ```
 * #include <iostream>
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   StartTimer();
 *   CalculateSomething();
 *   StopTimer();
 * 
 *   std::cout << "Required time " << GetMilliseconds() << " ms" << std::endl;
 * 
 *   return 0;
 * }
 * ``` */
int GetMilliseconds();

/**@}*/

/////////////////////////////////////////////////
/////// Audio Functions
/////////////////////////////////////////////////

/**
 * @defgroup audio_group Audio
 * @brief Functions to import, export and manipulate audio data.
 * 
 * @{
 */

/**
 * Writes the given raw audio data to an file using the Waveform Audio File format.
 * The Waveform Audio File format usually has the extension .wav.
 * 
 * @param filename      the file name including the path
 * @param data          the audio data, 32-bit float values per channel
 * @param size          the number of float values 
 * @param sampleRate    the number of samples per second
 * @param channels      the number of channels
 * @return 0 on success, error code otherwise
 * 
 * **Example**
 * This example generates an audio file containing half a second of silence using
 * a sample rate of 48000 samples per second and one audio channel.
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * #define SAMPLE_RATE 44100
 * 
 * int main()
 * {
 *   int numSamples = SAMPLE_RATE / 2;
 *   float* samples = new float[numSamples];
 *
 *   for (int i = 0; i < numSamples; ++i) {
 *     samples[i] = 0.0;
 *   }
 * 
 *   int err = WriteAudio("silence.wav", samples, numSamples, SAMPLE_RATE);
 * 
 *   if (err != ErrorCode::NO_ERROR) {
 *     SayText( GetErrorMessage(err) );
 *   }
 * } 
 * ``` 
 */
int WriteAudio(const char * filename, float *data, int size, int sampleRate = 44100, int channels = 1);

/**
 * Reads an audio file.
 * 
 * The pointer for the `size` parameter receives the number of samples.
 * 
 * The pointer to `sampelReate` parameter receives the sample rate of the audio file.
 *  
 * Accepted file formats:
 * 
 * - Waveform Audio File Format (.wav)
 * 
 * @param filename      the filename including the path
 * @param size          receives the number of floating point values (samples)
 * @param sampleRate    receives the sample rate
 * @param numChannels   receives the number of channels
 * @return the raw audio data
 */
float *ReadAudio(const char* filename, int* size, int *sampleRate, int *numChannels);

/**
 * Extracts the an audio channel from given sample data.
 * 
 * @param data          the audio samples with interleaved channels
 * @param size          the total number of samples including all channels
 * @param numChannels   the number of channel
 * @param channel       the channel to extract
 * @param resultSize   receives the number of samples of the extracted channel
 * @return the extracted samples of the channel
 */
float *ExtractChannel(float *data, int size, int numChannels, int channel, int *resultSize = nullptr);

/**
 * Interleaves data of two audio channels to one channel. 
 * The two provided audio channels must contain the same number of audio samples.
 *  
 * @param ch1Data       the audio samples of the first audio channel
 * @param ch2Data       the audio samples of the second audio channel
 * @param size          the number of samples of both audio channels
 * @param resultSize    receives the number of audio samples of the interleaved channels
 * @return the interleaved audio data
 */
float *InterleaveChannels(float* ch1Data, float* ch2Data, int size, int *resultSize = nullptr);

/**
 * Converts the sample rate of audio data.
 * 
 * @param data          the audio samples to convert
 * @param size          the number of audio samples
 * @param srcRate       the source sampling rate of the audio samples
 * @param dstRate       the destination sampling rate
 * @param resultSize    receives the number of audio samples of converted data
 * @return the audio data with the new sampling rate
 */
float *ConvertSampleRate(float *data, int size, int srcRate, int dstRate, int *resultSize = nullptr);

/**@}*/

/////////////////////////////////////////////////
/////// Graphics Functions
/////////////////////////////////////////////////

/**@}*/

/**
 * @defgroup graphics_group Graphics
 * @brief This module offers functions to graphical operations.
 * 
 * It offers functions to load, store, create and manipulate images. 
 * Currently only BMP files are supported.
 * @{
 */

/**
 * Creates a new image with the specified width and height.
 * The new image is created internally and can be accessed using 
 * other functions within this module. Any image which might have 
 * been created or loaded before is deleted by calling this function.
 * 
 * @param w the width of the image in pixel
 * @param h the height of the image in pixel
 * @return ErrorCode::NO_ERROR on success, the error code otherwise
 * 
 * **Example**
 * 
 * ```
 * #include <AstUtils.h>
 * 
 * int main()
 * {
 *   // Create a new image using Full HD resolution.
 *   createImage(1920, 1080);
 * 
 *   // TODO draw something, store the image.
 * 
 *   return 0;
 * }
 * ```
 */
int CreateImage(int w, int h);

/**
 * Clears the image using the current clear color.
 */
void ClearImage();

/**
 * Writes the current image data to a bitmap file.
 * 
 * @param filename  the filename of the image including the path
 * @return ErrorCode::NO_ERROR on success, the error code otherwise
 */
int WriteImage(const char* filename);

void SetDrawColor(int r, int g, int b, int a = 255);
void SetClearColor(int r, int g, int b);

void DrawLine(double x0, double y0, double x1, double y1, double w = 1);
void DrawCircle(double x, double y, double r);

/**@}*/

/////////////////////////////////////////////////
/////// Error Handling
/////////////////////////////////////////////////

/**
 * @defgroup error_group Error Handling
 * @brief This module is dedicated to error handling.
 * 
 * @section intro_sect Introduction
 * 
 * This module offers a error handling concept which does not make use of C++
 * exceptions and hence can be considered being old-syle. The basic concept 
 * is that a function returns an error code which is in case of success equal
 * to zero or the enum constant `NO_ERROR`. However, some functions return
 * some kind of a result and cannot return an error code at the same time.
 * The solution is that all functions which can possible fail assign an error
 * code to a global hidden variable called `last_error`.
 * Functions which can return an error code in some way do this additionally
 * to the global error variable. 
 * 
 * The application can query this last-error by the use of the function
 * `GetLastError` and translate the error code to a human 
 * readable text message using the function `GetErrorMessage`.
 * 
 * @subsection usage_sect Example Usage
 * Here is an example for the basic error handling idiom in case the used
 * function returns an error code and the error condition
 * is unrecoverable and the program has to terminate.
 * 
 * ```
 * if ( DoSomethingWhichCanFail() != ErrorCode::NO_ERROR) {
 * 
 *   SayText( GetErrorMessage( GetLastError() ) );
 * 
 *   // TODO clean up e.g., free memory etc. and exit program
 * }
 * ```
 * 
 * Using the fact that error codes differ from zero and using the convenient
 * methods like `GetLastErrorMessage()`, the code can be shortened to following 
 * version.
 * 
 * ```
 * if ( !DoSomethingWhichCanFail() ) {
 * 
 *   SayText( GetLastErrorMessage() );
 * 
 *   // TODO clean up e.g., free memory etc. and exit program
 * }
 * ```
 * 
 * In case the function to be used returns some kind of result, the idiom changes
 * like this.
 * 
 * ```
 * auto result = ReturnSomethingWhichCanFail();
 * if ( GetLastError() != ErrorCode::NO_ERROR ) {
 * 
 *   SayText( GetLastErrorMessage() );
 * 
 *   // TODO clean up e.g., free memory etc. and exit program
 * }
 * ```
 * 
 * Here is a slightly shorter version using the convenient function `HasError()`.
 * ```
 * auto result = ReturnSomethingWhichCanFail();
 * if ( HasError() ) {
 * 
 *   SayText( GetLastErrorMessage() );
 * 
 *   // TODO clean up e.g., free memory etc. and exit program
 * }
 * ```
 * 
 * @{
 */

/**
 * These error codes are set by library functions in case
 * the operation could not carried out successfully.
 */ 
enum ErrorCode {
    /** Indicates that no error has occurred. */
    NO_ERROR = 0x0000,

    /** At least one of the specified parameters is invalid. */
    INVALID_PARAMETER,

    /** A file could not be opened for reading. */
    UNABLE_TO_OPEN_FILE_FOR_READING,

    /** A file could not be opened for writing. */
    UNABLE_TO_OPEN_FILE_FOR_WRITING,

    /** A file could not be read correctly. */
    UNABLE_TO_READ_FILE,

    /** A file could not be imported correctly. */
    UNABLE_TO_IMPORT_FILE,

    /** The operation is not supported. */
    NOT_SUPPORTED,

    /** The current state is invalid for this operation. */
    INVALID_STATE,

    /** An error specifc to Simple Direct Layer. */
    SDL_ERROR,

    /** An error specifc to Simple Direct Layer. */
    JACK_ERROR,

    /** Custom error code set by application. */
    APP_ERROR,

    /** The specified error code is unknown. */
    UNKNOWN_ERROR_CODE,
};

/**
 * Returns the last-error code.
 * 
 * @return the last-error code
 */
int GetLastError();

/**
 * Sets the last-error code.
 * 
 * This function can be used to set an error code by the application.
 * Custom error codes should be negative in order to be distinguishable 
 * from error codes set by the library.
 * 
 * @param errorCode the error code
 */
void SetLastError(int errorCode);

/**
 * Convenient function to test whether an error has occurred.
 * 
 * Calling this method is equivalent to expression
 * 
 * ```
 * GetLastError() != ErrorCode::NO_ERROR
 * ```
 * 
 * @return `true` if an error has occurred
 */
bool HasError();

/**
 * Returns the error message for the specified error code.
 * 
 * @param errorCode the error code of the requested message
 */
const char* GetErrorMessage(int errorCode);

/**
 * Convenient function which returns the error message of the last error.
 * 
 * Calling this method is equivalent to 
 * 
 * ```
 * GetErrorMessage( GetLastError() );
 * ```
 */
const char* GetLastErrorMessage();

/**
 * Returns the a more detailed error message of the last-error.
 * 
 * Note: not all operations offer a detailed error message in case
 * of a problem and the returned string might be empty.
 * 
 * @return detailed error message or an empty string
 */
const char* GetErrorDetails();

/**
 * Sets detailed error description.
 * 
 * This function can be used to set a detailed error description by the application.
 * 
 * @param text  the error description
 */
void SetErrorDetails(const char *text);

/**@}*/


/////////////////////////////////////////////////
/////// SDL Application
/////////////////////////////////////////////////

/**
 * @defgroup sdl_group SDL Application
 * @brief Functions to write (simple) SDL-based applications.
 * 
 * Simple DirectMedia Layer (SDL) is a cross-platform development library 
 * providing low level access to audio, keyboard, mouse, joystick, and
 * graphics hardware.
 * The developer versions of the SDL library must be installed separately,
 * although the procedure differs for the different operating systems. 
 * The developer versions of the SDL library are available here:
 * https://www.libsdl.org/
 * 
 * @section sdl_example_sect Minimal SDL-based Application
 * 
 * ```
 * #include <iostream>
 * #include <AstUtils.h>
 * 
 * void ReportError()
 * {
 *     std::cout << "An error has occurred: " << GetLastErrorMessage() << std::endl;
 *     std::cout << GetErrorDetails() << std::endl;
 * }
 * 
 * int main()
 * {
 *     if (InitApp(640, 480) != NO_ERROR) {
 *         ReportError();
 *         return -1;
 *     }
 * 
 *     while(!IsAppTerminated()) {
 *         ClearCanvas();
 *         UpdateApp();
 *     }
 * 
 *     QuitApp();
 * } 
 * ```
 * 
 * @{
 */

/**@}*/


/*! \mainpage AST Utilities - API Level 0
 * 
 * AST Utilities **API Level 0** is the simplest and API-Level provided by this 
 * utility library. Level 0 does not require a namespace to be used nor does
 * it introduce classes or objects. It solely defines functions and uses
 * only C strings. API-Level 0 also does not expose any items from the 
 * C++ Standard Library.
 * 
 * 
 * @section hist_sect Version History
 * This library is continuously updated and extended. The version history
 * gives an overview of the changes this library has gone through, including
 * the latest updates. The version history can be found on this page 
 * [Version History](@ref CHANGES).
 * 
 * @section io_sect Modules
 *
 * API level 0 contains four modules with functions, each of which is dedicated 
 * to a separate task area.
 *  
 * - @ref io_group
 * - @ref math_group
 * - @ref timer_group
 * - @ref audio_group
 * - @ref graphics_group
 * - @ref sdl_group
 * - @ref error_group
 */