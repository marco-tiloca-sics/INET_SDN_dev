"""
Utilities for primitives.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


import sys


# Wrong number of arguments
def error_arguments (name, ex_number, g_number):
    sys.exit("Error: " + name + "() takes exactly " + str(ex_number) + " arguments ( " + str(g_number) +  " given )")


# Wrong number of arguments (fakeread only)
def wrong_argc(name, g_argc):	
    if name == "fakeread":
        sys.exit("ERROR: basing on the distortion function used, fakeread takes exactly 5 or 7 arguments, " + g_argc + "given")
