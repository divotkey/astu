/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

/**
 * @file
 * @brief This file defines public functions offered by AST utilities API-Level 0.
 */

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

/**
 * Outputs the specified text using the standard output stream.
 * If `text` is a `nullptr` an empty line will be printed.
 * 
 * @param text  a C string holding the text to be printed
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
void SayText(const char* text = nullptr);

/**
 * Outputs the specified integer value using the standard output stream.
 * 
 * @param value  the integer value to be printed
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
void SayInt(int value);

/**
 * Outputs the specified double value using the standard output stream.
 * 
 * @param value  the double value to be printed
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
void SayDouble(double value);

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
 *   Say("You have entered the following number");
 *   Say(x);
 * 
 *   return 0;
 * }
 * ```
 * Possible output:
 * ```
 * Please enter a number: 42
 * You entered the following number
 * 42
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
 *   int x;
 *   
 *   cout << "Please have enter a number: ";
 *   cin >> x;
 *   cout << "You have entered the number " << x << endl;
 * 
 *   return 0;
 * }
 * ```
 * 
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
 *   Say(x);
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
 * 
 */
double AskDouble(const char* text = nullptr);


// TODO implement ask bool.
// bool AskBool(const char * text);


/**@}*/

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
 * Returns a random number within the specified range.
 * This function will generate a random number using a 
 * pseudo random number generator.
 * 
 * The generated numbers are of type `double` an lie within the interval of
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
 * @param w the width of the image
 * @param h the height of the image
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
void CreateImage(int x, int y);

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
 * - @ref graphics_group
 */