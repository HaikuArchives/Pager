/*
	ConvertLines.h
	
	Copyright Will Dyson
	will_dyson@pobox.com
	
	Created: 08/10/99 15:09:02
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).
*/

#ifndef CONVERT_LINES_H
#define CONVERT_LINES_H

/*
	A function to convert an arbitrary string of text
	so that its line endings are in be/unix format.
*/
void ConvertLines(char* s, int length);

#endif /* CONVERT_LINES_H */
