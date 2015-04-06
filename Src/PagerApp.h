/*
	PagerApp.h
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/18/98 05:08:10
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).
*/

#ifndef PAGER_APP_H
#define PAGER_APP_H

#define APP_SIGNATURE "application/x-vnd.WD-Pager"

#include <Application.h>

class BFilePanel;
class BMessage;

/*
	PagerApp class
	Defines PagerApp
	Does important app-wide stuff
*/
class PagerApp : public BApplication
{
	public:
						PagerApp();
						~PagerApp();
		virtual void	MessageReceived(BMessage* message);
		virtual void	RefsReceived(BMessage* message);
		virtual void	ReadyToRun(void);
		virtual void	ArgvReceived(int32 argc, char **argv);
		
	private:
		
		bool		got_file_as_arg;
		BFilePanel*	openpanel;
};

#endif /*PAGER_APP_H*/
