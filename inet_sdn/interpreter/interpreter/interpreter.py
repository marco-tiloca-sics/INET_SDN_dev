#!/usr/bin/env python

"""
This python script takes one description file as input (aka ADL file), 
and produces an XML Attack Configuration File as output.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


import sys
import os

# Import ply.yacc
import ply.yacc as yacc

# Import ADL's grammar rules
from grammar.attacks_rules import *
from grammar.general_rules import *
from grammar.errors_rules import *

from grammar.filters_rules import *
from grammar.arguments_rules import *
from grammar.expressions_rules import *
from grammar.primitives_rules import *


# Import the core scripts
from engine.parser import parse_options
from engine.checker import check_input_filename
from engine.checker import check_output_filename
from engine.writer import write_output_file
from engine.cleaner import remove_pyc_files


if __name__ == "__main__":

    # Initialize yacc
    yacc.yacc(debug=0, write_tables=0, start="program")

    # Retrieve command line arguments and options
    options = parse_options()
    
    # Check if the input arguments is consistent
    check_input_filename(options["input"])
    
    # Check if the output argument is consistent (if not correct it)
    options["output"] = check_output_filename(options["output"])

    # Retrieve the content of the ADL file
    input_file = open(options["input"], "r")
    input_file_content = input_file.read()
    input_file.close()
        
    # Parse the content of the ADL file
    yacc.parse(input_file_content)

    # Produce the output XML file
    write_output_file(options["output"])
    
    # Remove bytecode
    dirname = os.path.dirname(sys.argv[0])
    remove_pyc_files(dirname)
    
