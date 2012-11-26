Cube
====

API implementation spec:

	#include "Cube.h"

	CUBE cube();

	void setup() {
	  cube.begin();
	  cube.initSerial( baud, port );                 // init the cube serial command interface (if not initialised, serial interface not attached)
	}

	void loop() {
	  // Entire cube:
	  cube.all( colour );                            // "all <colour>"
	  cube.shift( axis, direction );                 // (direction = '+'' or '-'') "shift <axis> <direction>"
	  // Specific LED:
	  cube.set( x, y, z, colour );                   // "set <location> <colour> <colour> <colour>" (only single currently implemented)
	  cube.next( colour );                           // "next <colour> <colour>" (only single currently implemented)
	  // Specific plane:
	  cube.setplane( axis, position, colour );       // "setplane <axis> <position> <colour>"
	  cube.moveplane( axis, from position, to position, colour ); // "move <axis> <from position> <to position> <colour>" ('to position' could be "h", for hidden
	  cube.copyplane( axis, from position, to position );    // "copy <axis> <from position> <to position>"
	}

	// location: 0, 1, 2, 3, h/H  (where H is a special hidden plane)
	// colour (function): #define RED = 0xff0000 in header file, also BLACK / "black", "BlAcK", maybe off if not a reserved name issue
	// colour (serial): #rgb (24 bits per pixel, 8 bits per colour) or colour name ("red", "green", ...)
	//                  in the flash based lookup table, the red line is: { "red",   RED,
	//                                                                      "green", GREEN,
	//                                                                       ...
	//                                                                    }
	//                  if a single character used as colour in a command, a single char lookup table is used:
	//                     r g b c m y w 0(off/black), etc
	// axis: X, Y or Z or AXIS_X, AXIS_Y, AXIS_Z
	// position: 0, 1, 2, 3, h/H
	// direction: +, -

	// Case insensitive for serial commands

	// maybe later:
	// serial command:
	//  get <pin>  // returns 
	//  pin: 0...17(ish) returns digital pin state as '0' or '1'
	//       a0...a5 returns '0' to '1023' as an ascii number for the most recent ADC pin read
	//                                     (the cube reads them all to a live array in its loop)
	// Graphics / shapes:
	// cube.line( x1, y1, z1, x2, y2, z2, colour );   // "line <location1> <location2> <colour>"
	// cube.bar( start location, height, colourstart, colourend, colouroff
	// cube.sphere( centre location, size, colourcentre, colour outside
	// cube.box( startlocation, endlocation, style:edges only/solidwalls/solidfilled, colour, filledcolour