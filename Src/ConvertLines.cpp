/*
	ConvertLines.cpp
	
	Copyright Will Dyson
	will_dyson@pobox.com
	
	Created: 08/02/99 00:57:35
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).
	
	Thanks to "Jonathan Perret <jperret@cybercable.fr>" for the
	idea for this routine.
*/

#include "ConvertLines.h"

#include <vector>


// a very finite set of states
enum state
{
	SEARCH,
	FOUND_CR
};

/*
	A function to convert an arbitrary string of text
	so that its line endings are in be/unix format.
	
	We go through the string, looking for Carrige Returns (\r).
	When we find one, we change it to a NewLine (\n). If the next
	charecter after the \r is a \n, we remove it (actually, put it
	in a list of charecters to be removed later).
	
	We use a fun little finite state machine to do the search this
	way. Reminds me of my CS61 (algorithms & data structures) class...
*/

void ConvertLines(char* s, int length)
{
	if (s == NULL)
	{
		// error, but no error handler. sorry
	}
	
	int index = 0;
	state mach_state = SEARCH;
	vector<int> RemoveChars;
	
	// scan for '\r\n' and '\r'
	for (index=0; index < length; index++)
	{
		// if we've found an \r\n combo
		if (mach_state == FOUND_CR && s[index] == '\n')
		{
			RemoveChars.push_back(index); // mark char for removal
			mach_state = SEARCH;
		}
		else
		{
			mach_state = SEARCH;
			
			// replace \r with \n
			if (s[index] == '\r')
			{
				s[index] = '\n'; // covert in place
				mach_state = FOUND_CR;
			}
		}
	}
	
	// walk through s and remove extra newlines
	if (! RemoveChars.empty() )
	{
		int removeindex = 0; //index into RemoveChars
		
		index = RemoveChars[removeindex]; // start at first \n
		
		// copy charecters from (removeindex) ahead.
		// this compacts out any chars in RemoveChars
		while ((index + removeindex) < length)
		{
			if ((index + removeindex) == RemoveChars[removeindex])
			{
				// incrementing removeindex when we get to an index
				// that is in removechars
				removeindex++;
			}
			
			s[index] = s[index + removeindex];
			index++;
		}
		
		// fill end of string with Nulls
		while (index < length)
		{
			s[index] = '\0';
			index++;
		}
	}
}
