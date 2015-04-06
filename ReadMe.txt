Pager 0.6a Read Me
By Will Dyson <will@cs.earlham.edu>

Pager is a small app for reading text files.

There are plenty of other apps to open and display your plain-text documents (such as Be's StyledEdit). However, they are mostly text editors, while Pager is a text viewer. As such, it can make some optimisations for easy reading of documents such as:

	• Files are opened in read_only mode. There is no insertion point in the text (you can, of course, still select and copy text to the clipboard).

	• Space bar is mapped to the page_down operation.

	• Page_down scrolls the document so that the bottommost readable line (> 60% visible) becomes the topmost line. This makes it easy to pick up reading where you left off after a page_down. Page_up operates in a similar (but opposite) manner.

	• The up and down arrow keys scroll up or down by one line.


Liscense:

You may use this code for anything you wish, including commercial programs, as long as you give me credit for my work in your program's ReadMe (and in the source code, if you plan on releaseing it).

Other misc items:

	• StyledEdit style-tags are used by default if present in a file. Scrolling works correctly even if multiple text sizes are present in a file.

	• Source code to Pager is distributed under a BSDish license that allows you to use it in your own projects (even commercial or shareware apps) as long as you give me credit for my work (a simple "Thank you" in the readme should be sufficient). Of particular interest is the PagerTextView object. This (well-commented) BTextView derivitive contains all that you need to implement my paging algorithm in your own app that displays text for reading (such as email or news clients).


Changes Since version 0.5:
	• You can now turn off the loading of styled-edit text styles from the attributes. Under the options menu.

To Do before version 1.0:

	• Implement URL highliteing and activation (opening it when you double-click on it).

	• Saveing of application preferences.