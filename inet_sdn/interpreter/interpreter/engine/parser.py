"""
Parse command line arguments and options.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


import sys
import os

try:
    from argparse import ArgumentParser, RawDescriptionHelpFormatter
except ImportError:
    print "Error: library 'argparse' is missing. Please, install it."
    sys.exit(-1)


def parse_options():
    """
    Parse command line arguments and options
    """
    
    epilog = "All arguments and options are mandatory"
    
    # The RawDescriptionHelpFormatter is required to show the epilog
    parser = ArgumentParser(epilog=epilog, formatter_class=RawDescriptionHelpFormatter,version="%prog 1.0")
    
    parser.add_argument("-i", "--input",
                        metavar = "\"INPUT\"",
                        default = "",
                        help = "Name of the ADL file to be processed")
    parser.add_argument("-o", "--output",
                        default = "",
                        metavar = "\"OUTPUT\"",
                        help = "Name of the XML output file")
    
    args = parser.parse_args()
    
    # Convert the list of options to a dictionary
    options = args.__dict__
    
    if options["input"] == "":
        parser.error("Error: Input file is missing.")

    # If not specified, the default output name is the input name
    if options["output"] == "":
        #optionss["output"] = options["input"].replace(".adl", "")
        options["output"] = os.path.splitext(options["input"])[0]
        options["output"] += ".xml"
        print "Using default output filename '" + options['output'] + "'"
    else:
        if os.path.splitext(options["output"])[1] != ".xml":
            options["output"] = os.path.splitext(options["output"])[0]
            options["output"] += ".xml"
            print "Warning: extension changed automatically to '.xml', output filename is '" + options["output"] + "'"

    return options
