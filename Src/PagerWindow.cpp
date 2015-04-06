/*
	PagerWindow.cpp
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/18/98 04:39:26
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).
	
	
	Note: this file still needs some major code clean-up. Sorry.
*/

#include <StorageKit.h>
#include <InterfaceKit.h>
#include <Application.h>
#include <fs_attr.h>
#include <stdlib.h>


#include "PagerWindow.h"
#include "PagerApp.h"
#include "ConvertLines.h"
#include "Constants.h"


/*
	Constructor
	
	Sets up the menus, reads the file, puts it into
	the PagerTextView. 
*/
PagerWindow::PagerWindow(entry_ref ref, BRect frame)
			: BWindow(frame, "", B_DOCUMENT_WINDOW, 0)
{
	BFile textfile;
	
	BRect textframe = Bounds();
	BRect textrect;
	
	//setup vars
	file_styles = NULL;
	url_styles = NULL;
	disp_file_styles = true;
	disp_url_styles = false;
	
	// add menubar to window
	Init_Menu();
	
	//add text view
	textframe.top = menubar->Bounds().bottom + 1.0;
	textframe.right -= B_V_SCROLL_BAR_WIDTH;
	textrect = textframe;
	textrect.OffsetTo(B_ORIGIN);
	textrect.InsetBy(3.0,3.0);
	textview = new PagerTextView(textframe, "text_view", textrect,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED);
	scrollview = new BScrollView("scroll_view", textview,
		B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER);
	AddChild(scrollview);
	
	
	// setup view
	
	textview->MakeFocus(true); // view is now focus for UI events
	textview->MakeEditable(false); // yo, can't touch this! Haha :)
	textview->SetStylable(true); // turn on multiple text styles
	
	//add text from ref
	
	char name[B_FILE_NAME_LENGTH];
	BEntry entry;
	off_t size;
	attr_info style_info;

	entry.SetTo(&ref, true);
	if (entry.InitCheck() == B_NO_ERROR)
	{
		entry.GetName(name);
		SetTitle(name);
	}

	textfile.SetTo(&ref, B_READ_ONLY);
	
	if (textfile.InitCheck() == B_NO_ERROR)
	{
		textfile.GetSize(&size);
		
		// get text and styles out of the file
		
		char* text_buf = new char[size];
		if (text_buf == NULL)
		{
			// out of memory err
		}
		
		ssize_t amt_read;
		
		// read from file, recording num bytes actually read
		amt_read = textfile.Read((void*)text_buf, size);
		
		// here we convert to be/unix line endings
		ConvertLines(text_buf, amt_read); // see ConvertLines.cpp
		
		textview->SetText(text_buf, amt_read);
	
		delete[] text_buf; // nor text_buf
	}
	
	if (textfile.GetAttrInfo("styles", &style_info) == B_OK)
	{
		void* flat_styles_buf = (void*) malloc(style_info.size);
		textfile.ReadAttr("styles", style_info.type, 0, flat_styles_buf, style_info.size);
		
		// style_bytes gets the number of bytes allocated for style_buf
		file_styles = BTextView::UnflattenRunArray(flat_styles_buf);
		
		// free buffer
		free(flat_styles_buf);
	}
	
	// add selected styles to view
	Set_Styles();
	
	Show(); //show window
}

/*
	Destructor:
	free text_run_arrays
*/
PagerWindow::~PagerWindow()
{
	if (file_styles != NULL)
	{
		free(file_styles);
	}
	if (url_styles != NULL)
	{
		free(url_styles);
	}
}

// This code should really go in PagerTextView
void PagerWindow::Set_Styles()
{
	text_run_array default_style;
	text_run theRun;
	
	theRun.offset = 0;
	theRun.font = be_fixed_font;
	theRun.color.red = 0;
	theRun.color.green = 0;
	theRun.color.blue = 0;
	theRun.color.alpha = 0;
	
	//setup default
	default_style.count = 1;
	default_style.runs[0] = theRun;
	
	// lock window before calling bview function
	Lock();
	
	if ((disp_url_styles == true)&&(url_styles != NULL))
	{
		
	}
	
	if ((disp_file_styles == true)&&(file_styles != NULL))
	{
		textview->SetRunArray(0, textview->TextLength(), file_styles);
	}
	else
	{
		// Set the whole view to the default style
		textview->SetRunArray(0, textview->TextLength(), &default_style);
	}
	Unlock();
	
	return;
}

void PagerWindow::Init_Menu()
{
	BMenu* menu;
	BMenuItem* item;
	
	//add the menubar
	
	menubar = new BMenuBar(Bounds(), "menu_bar");
	
	// file menu
	menu = new BMenu("File");
	menu->AddItem(item=new BMenuItem("Open" B_UTF8_ELLIPSIS,
		new BMessage(MENU_FILE_OPEN), 'O'));
	
	item->SetTarget(be_app); // sends the message to BeApp, not this window
	
	menu->AddItem(new BMenuItem("Close",
		new BMessage(B_QUIT_REQUESTED), 'W'));
	
	menu->AddSeparatorItem();
	
	menu->AddItem(new BMenuItem("Quit",
		new BMessage(MENU_FILE_QUIT), 'Q'));
	
	menubar->AddItem(menu);
	
	//edit menu
	
	menu = new BMenu("Edit");
	menu->AddItem(item = new BMenuItem("Copy", new BMessage(B_COPY), 'C'));
	item->SetTarget(0, this); // make window's focus view the target

	menu->AddItem(item = new BMenuItem("Select All", 
		new BMessage(B_SELECT_ALL), 'A'));
	item->SetTarget(0,this); // make window's focus view the target
	
	menu->AddSeparatorItem();
	
	menu->AddItem(item = new BMenuItem("Preferences", new BMessage(MENU_EDIT_PREFS)));
	item->SetTarget(be_app); // make application the target
	
	menubar->AddItem(menu);
	
	// options menu
	menu = new BMenu("Options");
	menu->AddItem(urls = new BMenuItem("Hilite URLs", new BMessage(MENU_OPTIONS_URLS)));
	
	menu->AddItem(styles = new BMenuItem("Use Styles", new BMessage(MENU_OPTIONS_STYLES)));
	styles->SetMarked(true);
	
	menubar->AddItem(menu);
	
	AddChild(menubar); // add menubar to window
}

/*
	QuitRequested
	Q: The User wants to close window. OK?
	A: Yes. Nothing to save, nothing to do.
*/
bool PagerWindow::QuitRequested() 
{
	// need to check to see if we are the last window open
	// if so, quit the app.
	
	be_app->Lock();
	int32 windows = be_app->CountWindows();
	be_app->Unlock();
	
	// if this is the last window, windows will be 2: This window
	// and the BFilePanel.
	
	if (windows < 3)
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
	
	return true;
}

/*
	FrameResized
	Make the text view change size with the window
*/
void PagerWindow::FrameResized(float width, float height)
{
	BRect textrect = textview->TextRect();
	
	textrect.right = textrect.left + 
		((width - B_V_SCROLL_BAR_WIDTH) - 3.0);
	textview->SetTextRect(textrect);
}

/*
	MessageReceived
	Prosess incoming messages and take action.
*/
void PagerWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case MENU_FILE_QUIT:
		{
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
		}
		break;
		
		case MENU_OPTIONS_URLS:
		{
			BAlert* lert = new BAlert("urls lert", "URL highliteing not implemented yet. Sorry.", "Ok");
			lert->Go();
		}
		break;
		
		// This belongs in PagetTextView too.
		case MENU_OPTIONS_STYLES:
		{
			// Toggle the checkmark and option together
			if (styles->IsMarked())
			{
				disp_file_styles = false;
				Set_Styles();
				styles->SetMarked(false); // was on, turn off
			}
			else
			{
				disp_file_styles = true;
				Set_Styles();
				styles->SetMarked(true); // was off, turn on
			}
		}
		break;
		
		default:
		{
			BWindow::MessageReceived(message);
		}
		break;
	}
}
