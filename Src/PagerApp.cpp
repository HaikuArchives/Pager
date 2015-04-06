/*
	PagerApp.cpp
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/18/98 05:08:03
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, '
	if you plan on releaseing it).
*/

#include <Application.h>
#include <List.h>
#include <FilePanel.h>
#include <Alert.h>

#include "PagerApp.h"
#include "PagerWindow.h"

#include "Constants.h"

/*
	Constructor
	Inits data for app.
*/
PagerApp::PagerApp()
			: BApplication(APP_SIGNATURE)
{	
	got_file_as_arg = false;
	openpanel = new BFilePanel();
}

PagerApp::~PagerApp()
{
	delete openpanel;
}

/*
	MessageReceived
	Does all the work of handleing messages.
*/
void PagerApp::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case MENU_FILE_OPEN:
		{
			openpanel->Show(); // show the openpanel
		}
		break;
		
		case MENU_EDIT_PREFS:
		{
			BAlert* lert = new BAlert("prefs lert", "Prefs not implemented yet. Sorry.", "Ok");
			lert->Go();
		}
		break;
		
		default:
		{
			BApplication::MessageReceived(message);
		}
		break;
	}
}

/*
	This Function loops through files that are passed to the
	app by BMessage and opens windows for them which are told
	to display the contents of said file.
*/
void PagerApp::RefsReceived(BMessage* message)
{
	int32 count;
	uint32 type;
	entry_ref ref;
	BEntry entry;
	PagerWindow* window;

	
	message->GetInfo("refs", &type, &count);
	if (type != B_REF_TYPE) return;
	
	for (int16 i = (count - 1); i >= 0; --i)
	{
		if (message->FindRef("refs", i, &ref) == B_OK)
		{
			entry.SetTo(&ref, true);
			if ((entry.InitCheck() == B_NO_ERROR) && entry.Exists())
			{
				if (entry.IsFile())
				{
					got_file_as_arg = true;
					
					window = new PagerWindow(ref);
				}
			}
		}
	}
}

/*
	Hook function that is called when the app has finished dealing
	with everything passed in by RefsReceived and ArgVReceived. If
	no files were opened, then quit.
*/
void PagerApp::ReadyToRun(void)
{
	if (got_file_as_arg == false)
	{
		Quit();
	}
}

/*
	This function loops through arguments passed by the shell
*/
void PagerApp::ArgvReceived(int32 argc, char**argv)
{
	BEntry entry;
	PagerWindow* window;
	entry_ref ref;

	for (int32 i = argc - 1; i > 0; --i)
	{
		entry.SetTo(argv[i], true);
		if ((entry.InitCheck() == B_NO_ERROR) && entry.Exists())
			{
				if (entry.IsFile() && entry.GetRef(&ref) == B_NO_ERROR)
				{
					got_file_as_arg = true;
					
					window = new PagerWindow(ref);
				}
			}
	}
}


/*
	Main
	Included in PagerApp.cpp for lack of anyplace else for
	it to go.
*/
int main(void) 
{
	PagerApp theapp;
	theapp.Run();
	return B_NO_ERROR;
}
