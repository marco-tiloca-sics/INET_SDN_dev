"""
Utility functions.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


import sys
import os

from config import *


def packet_check(argument, lineno):
    """
    Check if the provided argument is a packet and it has been previously declared
    """
    
    if argument not in symbol_table.keys() or symbol_table[argument] != 'PACKET':
        print_error("\tError : undefined packet '"+str(argument)+"'", str(lineno))


def clear_data_structure():
    """
    Clear global variables and global data structures
    """
    
    packetFilter = ""
    # Reset to its default value
    packetFilterOperator = "AND"
    
    del actions[:]
    variables.clear()
    symbol_table.clear()


def check_layer_name(field):
    """
    Check if the user has specified an existing layer name
    """
    
    hygienize = field.replace("\"", "")
    layer_name = (hygienize.split("."))[0]
    
    if layer_name in layer_names:
        return True
    return False
    

def check_control_structure_name(field):
    """
    Check if the user has specified an existing control structure name
    """
    
    hygienize = field.replace("\"", "")
    control_structure_name = (hygienize.split("."))[0]
    
    if control_structure_name in control_structures_names:
        return True
    return False


def print_error(error_msg, error_lineno):
    """
    Print error messages
    """
    print error_msg + " - Line " + str(error_lineno)
    exit(-1)
