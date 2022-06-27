THE OPDESK WIDGET'S test-app DIRECTORY
--------------------------------------

    This directory is a demonstration of an actual 'real world'
    working application that uses the OpDesk API to make a node
    based editor that generates code, and lets the user save/load
    their node based 'gui programs'.

    In this case an application that lets the user layout boxes
    that generate simple working perl scripts.

    You should be able to copy this entire directory and modify
    to taste, or use it as a guide for making your own application.

    In particular, this application shows the intended design
    pattern for deriving your own classes from the OpDesk widgets
    in order to define your own data types, language constructs,
    macro expanders, 'save' and 'load' techniques for saving your
    layouts, and user interface techniques (like adding popup 
    dialogs for the buttons, handling copy/paste operations, etc).

    For instance, here's a list of the derived classes this
    application defines:
    
        OpDesk API	Your App's
    	Base Class	Derived Class	Description
	-----------	-------------	------------------------------------------
    	OpButton	MyButton	Handles loading/saving buttons, data types,
					constants, and popup dialogs.

    	OpBox 		FSM_Node		Handles load/save of box and buttons,
					source code generation, box naming.

    	OpDesk		MyDesk		Handles load/save of desk, copy paste,
					and connection errors.

    The other files:

	File		Description
	---------	------------------------------------------------------------
        Subs.[CH]	Subroutines global to the app

	test-app.C	The app itself: handles creating the window, default boxes
			and their data types, and the app's menu bar options, like
			load/save dialogs, file management, code generation, copy
			and paste, etc.

FILES
    test-layout.lay -- an example layout

AUTHOR

    Greg Ercolano, Aug 27 2009

ACKNOWLEDGEMENTS

    Al Ibrahim for paying the bounty for me to finish writing this widget.
    Without his financial stimulous, I probably would never have put the
    effort into completing this widget.

    Cinema Graphics' "ShadeTree" software, which was co-designed by
    Reid Baker, Greg Ercolano, Rodney Iwashina and Liza Keith in 1995.
    Thanks to Reid Baker for his permission.

