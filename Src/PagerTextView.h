/*
	PagerTextView.h
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/25/98 15:31:25
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
	if you plan on releaseing it).
*/

#ifndef PAGER_TEXT_VIEW_H
#define PAGER_TEXT_VIEW_H

#include <TextView.h>

class BRect;
class BFont;
class BMessage;

class PagerTextView : public BTextView
{
public:
	PagerTextView(BRect frame, const char *name, BRect textRect,
		uint32 resizingMode, uint32 flags);
	PagerTextView(BRect frame, const char *name, BRect textRect, 
		const BFont *font, const rgb_color *color, 
		uint32 resizingMode, uint32 flags);
	PagerTextView(BMessage *archive);
	
	virtual void KeyDown(const char *bytes, int32 numBytes);

private:
	void PageUp(void);
	void PageDown(void);
	void LineUp(void);
	void LineDown(void);
	
	BPoint GetTopOfLine(int32 line_index);
	BPoint GetBottomOfLine(int32 line_index);
};

#endif /*PAGER_TEXT_VIEW_H*/
