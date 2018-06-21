#if 1
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/glut.h>
#include <time.h>

#define VER_INIT 18
#define HOR_INIT 5


float x, y, z ; 
int Board [ 26 ] [ 16 ], spin_block, block_model , ver = VER_INIT, hor = HOR_INIT ;  
int Block [ 7 ] [ 4 ] [ 4 ] [ 4 ] = {
{                                                                               // n1 
{ {0, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 2, 0}, {0, 0, 0, 0} },
{ {0, 0, 2, 0}, {0, 0, 2, 0}, {0, 2, 2, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 2, 0, 0}, {0, 2, 2, 2}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 2, 2, 0}, {0, 2, 0 ,0}, {0, 2, 0 ,0} } 
},

{                                                                               // 1
{ {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 3, 3, 3}, {0, 0, 0, 0} },
{ {0, 3, 0, 0}, {0, 3, 0, 0}, {0, 3, 0, 0}, {0, 3, 0, 0} },
{ {0, 0, 0, 0}, {3, 3, 3, 3}, {0, 0, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0}, {0, 0, 3, 0} } 
},

{                                                                               // n2 
{ {0, 0, 0, 0}, {0, 0, 4, 0}, {4, 4, 4, 0}, {0, 0, 0, 0} },
{ {0, 4, 0, 0}, {0, 4, 0, 0}, {0, 4, 4, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 4, 4, 4}, {0, 4, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 4, 4, 0}, {0, 0, 4, 0}, {0, 0, 4, 0} }
},

{                                                                               // zigzag1 
{ {0, 0, 0, 0}, {0, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0} },
{ {0, 0, 0, 0}, {0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0} },
{ {0, 5, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 0, 5, 5}, {0, 5, 5, 0}, {0, 0, 0, 0} }
},

{                                                                               // zigzag2 
{ {0, 0, 0, 0}, {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0 ,0} },
{ {0, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0} },
{ {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 6, 6, 0}, {0, 0, 6, 6}, {0, 0, 0, 0} }
},

{                                                                               // T 
{ {0, 0, 0, 0}, {0, 7, 0, 0}, {7, 7, 7, 0}, {0, 0, 0, 0} },
{ {0, 7, 0, 0}, {0, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {7, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0} },
{ {0, 0, 0, 0}, {0, 0, 7, 0}, {0, 7, 7, 0}, {0, 0, 7, 0} }      
},

{                                                                               // rect 
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } },
{ {0, 0, 0, 0}, {0, 8, 8, 0}, {0, 8, 8, 0}, {0, 0, 0, 0 } } 
}      

};


// Functions for randomizing blocks
// The return value has a random value and is of type int
// There is no parameter
int Random ( void ) {				
	// Initialize a random function by time
	srand ( ( int ) time ( NULL ) ) ;
	return rand ( ) % 6 ;
}


// Functions to initialize the tetris board
// There is no return value
// There is no parameter 
void Board_Init ( void ) {
	for ( int a = 0; a < 22; a++ ) {
		for ( int b = 0; b < 12; b++ ) {
			// create outer wall
			if ( a == 0 || a == 21 || b == 0 || b == 11 ) 
				Board [ a ] [ b ] = 1 ;
			else 
				Board [ a ] [ b ] = 0 ;
		} // end b-loop
	} // end a-loop
}


// Functions to represent blocks stacked on a Tetris board
// There is no return value
// There is no parameter 
void Board_Display ( void ) { 
	for ( int a = 0; a < 22; a ++ ) {
		for ( int b = 0; b < 12; b ++ ) {
			if ( Board [ a ] [ b ] != 0 ) {
				// The glLoadIdentity is used to initialize the matrix.
				glLoadIdentity ( ) ;
				// Functions needed when rotating on the x, y, and z axes
				glRotatef ( x, 1, 0, 0 ) ;						
				glRotatef ( y, 0, 1, 0 ) ;
				glRotatef ( z, 0, 0, 1 ) ;
				
				// To represent block colors
				switch ( Board [ a ] [ b ] ) {					
					case 2 :
						glColor3f ( 1.0, 0.0, 0.0 ) ;
						break ;
						
					case 3 :
						glColor3f ( 0.0, 1.0, 0.0 ) ;
						break ;
						
					case 4 :
						glColor3f ( 0.0, 0.0, 1.0 ) ;
						break ;
						
					case 5 :
						glColor3f ( 0.0, 1.0, 1.0 ) ;
						break ;
						
					case 6 :
						glColor3f ( 1.0, 1.0, 0.0 ) ;
						break ;
						
					case 7 :
						glColor3f ( 1.0, 0.0, 1.0 ) ;
						break ;
						
					case 8 :
						glColor3f ( 0.6, 1.0, 0.3 ) ;
						break ;
						
					default :
						glColor3f ( 1.0, 1.0, 1.0 ) ;
						break ;
				} // end switch
				
				// The block is translated in the coordinate system.
				glTranslatef ( b, a, 0.0 ) ;			
				
				// Outer block
				glutWireCube ( 1.0 ) ;
				// Inner block			
				if ( Board [ a ] [ b ] > 1 ) 
					glutSolidCube ( 0.6 ) ; 
			} // end if
		} // end b-loop
	} // end a-loop
}


// Function that represents the current block
// There is no return value
// There is no parameter
void Block_Display ( void ) {
	for ( int a = 0; a < 4; a++ ) {
		for ( int b = 0; b < 4; b++ ) {
			if ( Block [ block_model ] [ spin_block ] [ a ] [ b ] ) {
				glLoadIdentity ( ) ;
				glRotatef ( x, 1, 0, 0 ) ;
				glRotatef ( y, 0, 1, 0 ) ;
				glRotatef ( z, 0, 0, 1 ) ;
				
				switch ( ( int ) Block [ block_model ] [ spin_block ] [ a ] [ b ] ) {
					case 2 :
						glColor3f ( 1.0, 0.0, 0.0 ) ;
						break ;
						
					case 3 :
						glColor3f ( 0.0, 1.0, 0.0 ) ;
						break ;
						
					case 4 :
						glColor3f ( 0.0, 0.0, 1.0 ) ;
						break ;
						
					case 5 :
						glColor3f ( 0.0, 1.0, 1.0 ) ;
						break ;
						
					case 6 :
						glColor3f ( 1.0, 1.0, 0.0 ) ;
						break ;
						
					case 7 :
						glColor3f ( 1.0, 0.0, 1.0 ) ;
						break ;
						
					case 8 :
						glColor3f ( 0.6, 1.0, 0.3 ) ;
						break ;
						
					default :
						glColor3f ( 1.0, 1.0, 1.0 ) ;
						break ;
				} // end switch
				
				glTranslatef ( hor + b, ver + a, 0.0 ) ;
				
				glutWireCube ( 1.0 ) ;	
				glutSolidCube ( 0.6 ) ;
			} // endif
		} // end b-loop
	} // end a-loop
}


// Functions for initializing opengl settings
// There is no return value
// There is no parameter
void Init ( void ) {
	glClearColor ( 0.0, 0.0, 0.0, 0.0 ) ;
	glColor3f ( 0.0, 0.0, 0.0 ) ;
	Board_Init ( ) ;
	srand ( time ( NULL ) ) ;
	block_model = Random ( ) ;
	// Use depth buffer
	glEnable ( GL_DEPTH_TEST ) ;
}


// Function that automatically resizes an object according to its window size
// There is no return value
// Takes the width and height of the window as parameter
void Reshape ( int width, int height ) {
	// Set object output location
	glViewport ( 0, 0, width, height ) ;
	// Change the matrix mode to the projection matrix
	glMatrixMode ( GL_PROJECTION ) ;
	glLoadIdentity ( ) ;
	// Functions for perspective projection
	gluPerspective ( 60.0, width/height, 1.0, 1000.0 ) ;
	glTranslatef ( 0.0, 0.0, 0.0 ) ;
}


// Function to determine when a block has crashed
// If block crashed, the function returns 1, otherwise it returns 0
// There is no parameter
int Crash_block ( void ) {
	for ( int a = 0; a < 4; a++ ) {
		for ( int b = 0; b < 4; b++ ) {
			if ( ( Board [ ver + a ] [ hor + b ] != 0 ) && ( Block [ block_model ] [ spin_block ] [ a ] [ b ] != 0 ) ) 
				return 1 ;
		}  // end b-loop
	} // end a-loop
	return 0 ;
}


// Function to draw tetris screen in real time
// There is no return value
// There is no parameter
void Display ( void ) {
	// The DEPTH_BUFFER is used to express the object's perspective
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	
	// Set location to view objects
	gluLookAt ( 5, 7, 30, 5, 7, 0, 0, 1, 0 ) ;
	
	// The MODELVIEW is used to represent the position of an object
	glMatrixMode ( GL_MODELVIEW ) ;
	glLoadIdentity ( ) ;
	
	if ( ver > 16 && Crash_block ( ) == 1 ) ver -- ; 
	
	// Temporarily store the value of the transformation matrix on the stack
	glPushMatrix ( ) ;
	{
		Board_Display ( ) ;
		Block_Display ( ) ;
	}
	// Retrieves the value of the transformation matrix temporarily stored in the stack
	glPopMatrix ( ) ;
	
	// The glutSwapBuffers is used to double buffering
	glutSwapBuffers ( ) ;
}


// Functions for moving the x, y, and z axes
// There is no return value
// Use the parameters as the value received from glutKeyboardFunc
// The prameters a and b is empty value ( To fit the format of the function )
void Rotate_Control ( unsigned char key, int a, int b ) {
	switch ( key ) {
		case 'q':
			x += 1;
			break ;
			
		case 'w':
			x -= 1;
			break ;
			
		case 'a':
			y += 1;
			break ;
			
		case 's':
			y -= 1;
			break ;
			
		case 'z':
			z += 1;
			break ;
			
		case 'x':
			z -= 1;
			break ;
		
	} // end switch
	
	// Call the function registered in glutDisplayFunc
	glutPostRedisplay ( ) ;
}


// Functions for controlling descending blocks
// There is no return value
// Use the parameters as the value received from glutKeyboardFunc
// The prameters a and b is empty value ( To fit the format of the function )
void Block_Control ( int key, int a, int b ) {
	switch ( key ) {
		case GLUT_KEY_LEFT :
			hor-- ;
			while ( Crash_block ( ) == 1 ) 
				hor++ ;
			break ;
		
		case GLUT_KEY_RIGHT :
			hor++ ;
			while ( Crash_block ( ) == 1 ) 
				hor-- ;
			break ;
		
		case GLUT_KEY_UP :
			spin_block++ ;
			if ( spin_block > 3 ) spin_block = 0 ;
			while ( ( Crash_block ( ) == 1 ) && ( hor > 2 ) && ( hor < 8 ) ) 
				spin_block -- ;
			while ( ( Crash_block ( ) == 1 ) && ( hor < 2 ) ) 
				hor++ ;
			while ( ( Crash_block ( ) == 1 ) && ( hor > 7 ) )
				hor-- ;
			break ;
		
		case GLUT_KEY_DOWN :
			ver-- ;
			while ( Crash_block ( ) == 1 ) 
				ver++ ;
			break ;
	} // end switch 
}


// Functions for storing stacked blocks on a Tetris board
// There is no return value
// There is no parameter 
void Save_Block ( void ) {
	for ( int a = 0; a < 4; a++ ) {
		for ( int b = 0; b < 4; b++ ) {
			if ( Block [ block_model ] [ spin_block ] [ a ] [ b ] != 0 ) 
				Board [ a + ver + 1 ] [ b + hor ] =  Block [ block_model ] [ spin_block ] [ a ] [ b ] ;
		} //end b-loop
	} // end a-loop
}


// Erase function when a block is full on one line
// There is no return value
// There is no parameter 
void Clear_Line ( void ) {
	int clean;
	
	for ( int a = 19; a > 0; a-- ) {
		for ( clean = 1; clean < 11; clean++ ) {
			if ( Board [ a ] [ clean ] == 0 ) break ;
		} // end clean-loop
		
		if ( clean == 11 ) {
			for ( int b = a; b < 20; b++ ) {
				for ( int c = 1; c < 11; c++ ) {
					Board [ b ] [ c ] = Board [ b + 1 ] [ c ] ;
				} // end c-loop
			} // end b-loop
		} // end if
	} // end a-loop
}


// Functions for running animations every 300 milliseconds
// There is no return value
// The prameters "value" is empty value ( To fit the format of the function )
void Timer ( int value ) {
	ver -- ;
	if ( Crash_block ( ) == 1 ) {
		Save_Block ( ) ;
		Clear_Line ( ) ;
		ver = 18 ;
		hor = 5 ;
		spin_block = 0 ;
		block_model = rand ( ) % 6 ;
		if ( block_model > 6 ) block_model = 0 ; 
	}
	glutTimerFunc ( 300, Timer, 1 ) ;
}



int main ( void ) {
	// Function to set window size
	glutInitWindowSize ( 800, 600 ) ;
	// Set the display mode for double buffering, color and depth buffer.
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH ) ;				
	glutCreateWindow ( "Tetris" ) ;

	Init ( ) ;
	glutDisplayFunc ( Display ) ;
	glutReshapeFunc ( Reshape ) ;
	glutKeyboardFunc ( Rotate_Control ) ;
	// Functions to keep Tetris in idle time
	glutIdleFunc ( Display ) ;
	glutTimerFunc ( 300, Timer, 1 ) ;
	glutSpecialFunc ( Block_Control ) ;
	
	// Functions for looping events continuously
	glutMainLoop ( ) ;
}


#endif
