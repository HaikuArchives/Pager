/*
	PagerTextView.cpp
	
	Copyright Will Dyson <will_dyson@pobox.com>
	
	Created: 07/25/98 15:35:42
	
	Liscense:
	You may use this code for anything you wish, including 
	commercial programs, as long as you give me credit for 
	my work in your program's ReadMe (and in the source code, 
'	if you plan on releaseing it).
*/

#include <Point.h>
#include <Alert.h>

#include "PagerTextView.h"

// Dummy constructors pass arguments to BTextView
PagerTextView::PagerTextView(BRect frame, const char *name, BRect textRect, uint32 resizingMode, uint32 flags)
: BTextView(frame, name, textRect, resizingMode, flags)
{}

PagerTextView::PagerTextView(BRect frame, const char *name, BRect textRect, const BFont *font, const rgb_color *color, uint32 resizingMode, uint32 flags)
: BTextView(frame, name, textRect, font, color, resizingMode, flags)
{}

PagerTextView::PagerTextView(BMessage *archive)
: BTextView(archive)
{}

/*
	Capture key events that we are interested in and call
	functions to deal with them.
*/
void PagerTextView::KeyDown(const char *bytes, int32 numBytes)
{
	switch ( bytes[0] )
	{
		case B_DOWN_ARROW:
			LineDown();
			break;
			
		case B_UP_ARROW:
			LineUp();
			break;
			
/*
	if you use this code for an application where there is a logical
	series of separate files to view (like an email reader), you might
	consider makeing B_SPACE detect if the view is at the end of the current
	file and moving to the next file if so (instead of calling PageDown() ).
*/
		case B_SPACE:
			PageDown();
			break;
			
		case B_PAGE_UP:
			PageUp();
			break;
			
		case B_PAGE_DOWN:
			PageDown();
			break;
			
		default:
			// inherited function
			BTextView::KeyDown(bytes, numBytes);
			break;
	}
}


/*
	Algorithm:
	
	find the lowest visable point in the view and call LineAt()
	on it. This gets the index (line number) of the lowest line. Now get
	the line above the lowest one (subtract one from index). This line is
	the lowest _entirely visible_ line (the last one the user could read). 
	This is the one we will make the new highest _entirely visible_ line, 
	enableing the user to pick up reading right where they left off.
	
	In order to move the line to the top of the screen, we must have the
	y-coordinate of the top of the line. To get this we call OffsetAt() on 
	that line to get the first charecter of the line. Call PointAt()
	on that offset to get the top,left corner of that first char. Find the 
	difference between the current top of the view and that point. 
	ScrollBy(that difference).
	
	Convoluted, but it should work - even in stylable views with multiple font
	sizes.
*/
void PagerTextView::PageDown(void)
{
	// this is the current visible area of the view, defined in veiw coordinates
	BRect bounds_rect = Bounds();
	
	// this is the overall boundries for text display
	BRect data_rect = TextRect();
	
	BPoint bottom_point(0, bounds_rect.bottom);
	
	// this is the lowest line that has any part visible
	int32 bottom_line = LineAt(bottom_point);
	
	// if bottom_line is > 40% obscured by bottom,
	// use next line above that.
	
	BPoint line_top = GetTopOfLine(bottom_line);
	BPoint line_bottom = GetBottomOfLine(bottom_line);
	
	// if less than 60% visible...
	if ((bottom_point.y - line_top.y) <= (0.6 * (line_bottom.y - line_top.y)))
	{
		bottom_line = bottom_line - 1; // move bottom_line up one
		
		line_top = GetTopOfLine(bottom_line); // refresh point at top of line
	}

/*
	Now we scroll the point at the top of bottom_line to the top of the screen,
	making it now the topmost line visible. But first we check to make
	sure that wouldn't scroll us _past_ the bottom. 
*/
	
	// dist from the top of bottom line to top of bounds
	float bot_line_to_top = line_top.y - bounds_rect.top;
	
	// dist from bottom of bounds to bottom of the view
	float bottom_to_bottom = data_rect.bottom - bounds_rect.bottom;
	
	float scroll_height = 0;
	
	// scroll by wichever one of these distances is lesser (when we are
	// at the bottom of the textview, bottom_to_bottom is zero).
	if (bot_line_to_top <= bottom_to_bottom)
		scroll_height = bot_line_to_top;
	else
		scroll_height = bottom_to_bottom;
	
	ScrollBy(0, scroll_height); // 0 pixels x, scroll_height y
	
	// IMHO, this is the proper way to do text paging (as opposed to simply scrolling
	// one screen_height).
}

// these next three functions should work more or less similarly to PageDown()

void PagerTextView::PageUp(void)
{
	BRect bounds_rect = Bounds(); //this is the visible area in view coords
	
	BPoint top_point(0, bounds_rect.top);
	
	int32 top_line = LineAt(top_point); // index of line at top of visible area
	
	
	BPoint line_bottom = GetBottomOfLine(top_line); // find bottom of line
	BPoint line_top = GetTopOfLine(top_line);
	
	// if less than 60% visible...
	if ((line_bottom.y - top_point.y) <= (0.6 * (line_bottom.y - line_top.y)))
	{
		top_line = top_line + 1; // move top_line down one
		
		line_bottom = GetBottomOfLine(top_line); // refresh point at bottom of line
	}

	// scroll line_bottom to the bottom of the screen, making sure
	// we won't (try to) scroll over the top.
	float scroll_height = 0;
	float top_line_to_bot = line_bottom.y - bounds_rect.bottom;
	float top_to_top = 0 - bounds_rect.top;
	
	if (top_line_to_bot >= top_to_top)
		scroll_height = top_line_to_bot;
	else
		scroll_height = top_to_top;
	
	ScrollBy(0, scroll_height);
}

void PagerTextView::LineUp(void)
{
	BRect bounds_rect = Bounds(); //this is the visible area in view coords
	
	BPoint top_point(0, bounds_rect.top - 1); // one pixel above current top
	
	// index of line one pixel above viewable area. This is the topmost
	// line with some part of it above the viewable area. We will move this
	// line to the top of the viewable area. This might be a line that was
	// partially viewable (top_point is somewhere in its middle), or the line 
	// just above the top (top_point is its bottom). Either way, it is the line
	// that the user wants to see next.
	int32 top_line = LineAt(top_point);
	
	BPoint line_top = GetTopOfLine(top_line);
	
	// height to scroll so that the top of our new top line is at the top of the
	// viewable area. 
	float scroll_height = line_top.y - bounds_rect.top;
	
	ScrollBy(0, scroll_height);
}

void PagerTextView::LineDown(void)
{	
	BRect bounds_rect = Bounds(); //this is the visible area of the view
	
	BPoint bottom_point(0, bounds_rect.bottom);
	
	// this is the lowest line that has any part visible
	int32 bottom_line = LineAt(bottom_point); 
	
	BPoint line_bottom = GetBottomOfLine(bottom_line);
	
	float scroll_height = line_bottom.y - bounds_rect.bottom; // calc the height we need to scroll
	
	ScrollBy(0, scroll_height); // 0 pixels x, scroll_height y
}

BPoint PagerTextView::GetTopOfLine(int32 line_index)
{
	int32 line_offset = OffsetAt(line_index);
	BPoint top_point = PointAt(line_offset);
	
	return top_point;
}

BPoint PagerTextView::GetBottomOfLine(int32 line_index)
{
	int32 line_offset = OffsetAt(line_index); // offset of first char of top_line
	
	float line_height = 0; // height of the line at line_point: to be filled by pointat()
	
	// line_top_point gets point at top left corner of top line
	BPoint line_top_point = PointAt(line_offset, &line_height);
	
	BPoint line_bottom_point(0, line_top_point.y + line_height); // find bottom of line
	
	return line_bottom_point;
}
