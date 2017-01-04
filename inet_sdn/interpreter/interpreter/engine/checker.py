"""
Check the consistency of the name of the input and output files.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


import sys
import os


def check_input_filename(filename):
    """
    Check if the input file:
     - exists
     - is not a directory
     - has the right extension
    """
    
    if not os.path.exists(filename):
        print "Error: the input file '" + str(filename) + "' does not exist."
        sys.exit(-1)

    if not os.path.isfile(filename):
        print "Error: '" + str(filename) + "' is a directory."
        sys.exit(-1)
        
    if os.path.splitext(filename)[1] != ".adl":
        print "The input file extension is not '.adl', it will be processed anyway"
            

def check_output_filename(filename):
    """
    Check if the name of the output file:
     - has a valid name
     - does not exist yet
     - is not directory
    """
   
    while(True):
    
        # Check if the filename already exists
        if os.path.exists(filename) == True:
            
            # Check if the filename is actually the name of a directory
            if not os.path.isfile(filename):
                filename = str(raw_input("Warning: '" + str(filename) + "' is probably a directory, type a new output filename: "))
                continue
        
            # Choose if you want to overwrite
            while(True):
                overwrite = str(raw_input("Warning: '" + str(filename) + "' already exists, do you want to overwrite it? [y/n]: "))
                
                if overwrite == "y":
                    return filename
                
                elif overwrite == "n":
                    while(True):
                        filename = str(raw_input("Type a new output filename: "))
                        
                        # The new output filename cannot be an empty name
                        if filename == "":
                            print "Error: you must specify an output filename"
                            continue
                        
                        # Set the .xml extension if it is missing or wrong
                        elif os.path.splitext(filename)[1] != ".xml":
                            filename = os.path.splitext(filename)[0]
                            filename += ".xml"
                            print "Warning: extension changed automatically to '.xml', the new output filename is '" + filename + "'"
                            break
                        
                        # New filename is ok
                        else:
                            break
                        
                    break
                    
                else:
                    print "Error: Option '" + overwrite + "' is not valid."
        
        else:
            break
      
    return filename
