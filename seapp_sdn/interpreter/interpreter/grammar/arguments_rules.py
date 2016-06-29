"""
Grammar rules for (ADL) arguments of primitives.

Author:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


# Import the lexer for ADL
from lexer.adllexer import *

# Import utilities
from engine.utilities import print_error
from engine.utilities import check_layer_name

# Import global variables
from config import *


# Unsigned real
def p_unsigned_real(p):
    """
    unsigned_real : REAL
                  | INTEGER
    """
    
    p[0] = str(p[1])


# Signed real
def p_signed_real(p):
    """
    signed_real : REAL
                | SIGNED_REAL
                | INTEGER
                | SIGNED_INTEGER
    """
    
    p[0] = str(p[1])


# Boolean
def p_boolean(p):
    """
    boolean : TRUE
            | FALSE
    """
    
    p[0] = str(p[1]).lower()


# Arg multi type
def p_layer_field_pair(p):
    "layer_field_pair : STRING"
    
    # Remove quotes
    p[0] = p[1].replace("\"", "")


# Arg multi type
def p_multi_type(p):
    """
    multi_type : INTEGER
               | SIGNED_INTEGER
               | REAL
               | SIGNED_REAL
               | STRING
               | ID
    """
    
    p[0] =  str(p[1])


# Node id
def p_node_id(p):
    "node_id : INTEGER"
    
    p[0] = str(p[1])


# Time
def p_time(p):
    """
    time : INTEGER
         | REAL
    """
    
    p[0] = str(p[1])


# Identifier
def p_identifier(p):
    """
    identifier : ID
               | ORIGINAL
    """
    
    # Check if the ID is present in the global symbol dictionary (i.e. it refers to a list or to a function)
    if str(p[1]) in global_symbol_table.keys():
        if global_symbol_table[str(p[1])] == "LIST":
            #p[0] = str(lists[str(p[1])])
            p[0] = str(p[1])
        if global_symbol_table[str(p[1])] == "FUNCTION":
            p[0] = str(p[1])
    
    # Check if the ID is present in the local symbol dictionary
    elif str(p[1]) in symbol_table.keys():
        p[0] = str(p[1])
    
    # It is a string/number constant or an undefined variable identifier
    else:
        p[0] = str(p[1])


# Direction (actually used only by put)
def p_direction(p):
    """
    direction : TX
              | RX
    """
    
    p[0] = str(p[1])


# Threshold (actually used only by drop)
def p_threshold(p):
    """
    threshold : INTEGER
              | REAL
    """
    
    if float(p[1]) <= 1.0:
        p[0] = str(p[1])
    else:
        raise ValueError("Threshold must be in the interval [0,1]")


# Create list (actually used only by create)
def p_create_list(p):
    "create_list : create_list COMMA create_list"
    
    p[0] = str(p[1]) + ":" + str(p[3])


# Create pair (actually used only by create)
def p_create_pair(p):
    "create_list : STRING COMMA multi_type"
    
    if check_layer_name(p[1]) == False:
        print_error("Error: layer name unknown, you can use only: APP or TRA or NET or MAC", str(p.lineno(1)))
    
    p[0] = str(p[1]) + ":" + str(p[3])
