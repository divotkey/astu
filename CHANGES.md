
# Version History {#CHANGES}

# Version 0.9.3
*Date: 2021-07-08*

- Improved compatibility with GNU C++ Compiler
- Added input mapping service.
- Added task service.
- Added `GetRandomBool()` function.
- Renamed classed and structures for 2D environments to new naming convention.

# Version 0.9.2
*Date: 2021-06-06*

- Added `EntityFactoryService` and extended `Entity` and `EntityComponent` class.

# Version 0.9.1
*Date: 2021-05-26*

- Improved service macros.
- Improved Camera2d.
- Added `IRenderService` interface to set background color.
- Added additional color constants.
- Added signal for mouse movement.

# Version 0.9.0
*Date: 2021-05-24*

- Renamed `MouseButtonListener` alias to `IMouseButtonListener`.
- Renamed `KeystrokeListener` alias to `IKeystrokeListener`.
- Renamed `ITimeService` to `ITimeManager`.
- Improved API of `ServiceManager`.
- Improved configuration of service features by inheritance (experimental).
- Improved `Vector2<T>` template class.
- Added scene graph for two-dimensional renderings.
- Improved/added `MathUtils` class.
- Improved signal facility.

# Version 0.8.0
*Date: 2021-04-07*

- Added interface `ILineRenderer` and to graphics module.
- Added SDL-based implementations of `ILineRenderer` interface.
- Added parameter to `SayVersion` and `SayCopyright` to add an empty line after the output.

# Version 0.7.0
*Date: 2021-03-01*

- Added `GetMicroseconds()` in addition to `GetMilliseconds()`.
- Added functions to query copyright information.

# Version 0.6.4
*Date: 2021-02-02*

- Fixed missing `#pragma once` in ImageRenderer.h

# Version 0.6.3
*Date: 2021-01-20*

- Changed Matrix3 class to be a template and moved it to public include folder.
- Improved `Palette` class.
- Extended and improved `ImageRenderer` class.
- The documentation of the Full API has been updated and slightly extended.

# Version 0.6.1
*Date: 2021-01-14*

- Added option to specify the maximum recursion depth the the scene quadtree in `ImageRenderer` class.

# Version 0.6.0
*Date: 2021-01-13*

- Removed some warnings emitted by MS compilers.
- Fixed missing `#pragma once` in AstUtils0.h
- Added `RotateDeg` method to `Vector2` class.
- Added class ' `ColorHsv` to cover HSV color space as well.
- Better encapsulation of internal functions for SDL applications (API-Level 0).
- Added `RenderRegularPolygon` function for SDL application (API-Level 0).

# Version 0.5.3
*Date: 2020-12-16*
- Extended `Color` class.

# Version 0.5.2
*Date: 2020-12-10*

- Extended `Vector2` template with multiplication operator between two vectors.
- Fixed issue regarding outdated version information.

# Version 0.5.1
*Date: 2020-12-09*

- Added convenient function `SayError()` for easy error output.
- Refactored `Vector2` class to be a template class.
- Added `Vector2d` alias for `astu::Vector2<double>`.
- Improved support for SDL-based applications using API-level 0.

# Version 0.4.0
*Date: 2020-11-25*

- Added service facility for simulations and games.
- Added Entity Component System (ECS) to be used for simulations and games.
- Added template for mathematical vectors in three dimensional space.
- Started to add basic support for SDL-based application for API-level 0.

# Version 0.3.0
*Date: 2020-10-21*

- Initial public release.
- Fixed bug in ask functions causing `AskString` not to work after calling `AskInt` etc.
- Improved error module.
- Added additional math functions.
- Fixed some issues, e.g, compiler warnings and errors on macOS and Windows. 
- Started to work on full access level functions and classes.
- Improved Image class.

# Version 0.2.0
*Date: 2020-08-19*

- Output methods can optionally omit end-of-line from output.
- Added module for error handling.
- Added module for audio processing (API-Level 0).

# Version 0.1.0
*Date: 2020-08-14*

- Initial version.