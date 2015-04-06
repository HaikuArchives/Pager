/*
	PagerWindow.h
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/18/98 04:02:05
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).'
*/

#ifndef PAGER_WINDOW_H
#define PAGER_WINDOW_H

#include <Window.h>
#include <MessageFilter.h>

#include "PagerTextView.h"


/*
	Class PagerWindow
	Adds FrameResized, menus and support for
	multiple windows to standard BWindow.
*/

class PagerWindow : public BWindow 
{
public:
						PagerWindow(entry_ref ref, BRect frame = BRect(50,50,550,450));
						~PagerWindow();
	virtual bool	QuitRequested();
	virtual void	MessageReceived(BMessage* message);
	virtual void	FrameResized(float width, float height);
	
	void 				Set_Styles();

private:
		
		void				Init_Menu();
		
		BMenuBar* 		menubar;
		BMenuItem*		styles;
		BMenuItem*		urls;
		
		PagerTextView*	textview;
		BScrollView* 	scrollview;
		
		text_run_array* file_styles;
		text_run_array* url_styles;
		
		bool disp_file_styles;
		bool disp_url_styles;
};

#endif /*PAGER_WINDOW_H*/
