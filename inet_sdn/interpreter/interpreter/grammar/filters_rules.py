"""
Grammar rules for (ADL) packet filter (conditional attack).
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


# Import the lexer for ADL
from lexer.adllexer import *

# Import print error utility
from engine.utilities import print_error

# Import global variables
from config import *


# Filter definition
def p_filter(p):
    "filter_code : FILTER LPAREN filter_content RPAREN"
    
    global packetFilter
    global packetFilterOperator
    # remove quotes
    packetFilter = p[3].replace("\"","")
    #packetFilter = packetFilter + packetFilterOperator
    
    p[0] = p[3]


def p_filter_content_mono(p):
    "filter_content : filter_elem"
    
    p[0] = p[1].replace("\"","")


def p_filter_content_generic(p):
    "filter_content : filter_content logic_operator filter_content"
    
    # Admitted only AND or OR (are appendend at the end)
    filter_content_generic = (p[1] + ":" + p[3] + ":" + p[2]).replace("\"","")
    p[0] = filter_content_generic


# Handle conditions contained between two paren
def p_filter_content_paren(p):
    "filter_content : LPAREN filter_content RPAREN"
    p[0] = p[2]


# Handle composite condition direct form and internal direct form
def p_filter_elem_compound_direct_internal_direct(p):
    "filter_elem : LPAREN full_path arithmetic_operator arithmetic_operand RPAREN comp_operator generic_operand"
    
    p[0] = "[" + ":" + str(p[1]) + ":" + str(p[2]) + ":" + str(p[3]) + ":" + str(p[4]) + ":" + str(p[5]) + ":" + str(p[6]) + ":" + str(p[7]) + ":" + "]"


# Handle composite condition direct form and internal reverse form
def p_filter_elem_compound_direct_internal_reverse(p):
    "filter_elem : LPAREN arithmetic_operand arithmetic_operator full_path RPAREN comp_operator generic_operand"
    
    p[0] = "[" + ":" + str(p[1]) + ":" + str(p[4]) + ":" + str(p[3]) + ":" + str(p[2]) + ":" + str(p[5]) + ":" + str(p[6]) + ":" + str(p[7]) + ":" + "]"


# Handle composite condition reverse form and internal direct form
def p_filter_elem_compound_reverse_internal_direct(p):
    "filter_elem : generic_operand comp_operator LPAREN full_path arithmetic_operator arithmetic_operand RPAREN"
    
    # Re-assemble condition as in 'p_filter_elem_composite'
    p[0] = "[" + ":" + str(p[3]) + ":" + str(p[4]) + ":" + str(p[5]) + ":" + str(p[6]) + ":" + str(p[7]) + ":" + str(p[2]) + ":" + str(p[1]) + ":" + "]"


# Handle composite condition reverse form and internal reverse form
def p_filter_elem_compound_reverse_internal_reverse(p):
    "filter_elem : generic_operand comp_operator LPAREN arithmetic_operand arithmetic_operator full_path RPAREN"
    
    # Re-assemble condition as in 'p_filter_elem_composite'
    p[0] = "[" + ":" + str(p[3]) + ":" + str(p[6]) + ":" + str(p[5]) + ":" + str(p[4]) + ":" + str(p[7]) + ":" + str(p[2]) + ":" + str(p[1]) + ":" + "]"


# Handle simple condition direct form
def p_filter_elem_simple_direct(p):
    "filter_elem : full_path comp_operator generic_operand"
    
    p[0] = "[" + ":" + str(p[1]) + ":" + str(p[2]) + ":" + str(p[3]) + ":" + "]"


# Handle simple condition reverse form
def p_filter_elem_simple_reverse(p):
    "filter_elem : generic_operand comp_operator full_path"
    
    p[0] = "[" + ":" + str(p[3]) + ":" + str(p[2]) + ":" + str(p[1]) + ":" + "]"


# Rull path of fields
def p_full_path(p):
    "full_path : STRING"
    
    full_path = str(p[1])
    # Remove char '"'
    full_path = full_path.replace("\"", "")
    
    # Check the consistency of the full path of fields "layer.field1.field2.field3 ..."
    id_counter = 0
    while len(full_path) != 0:
        # Search identifier in the string head
        try:
            substring_found = re.search('^[a-zA-Z_][a-zA-Z_0-9]*', full_path).group(0)
            id_counter += 1
        except AttributeError:
            print_error("Error: layer.field attribute in the packet-filter has a bad structure, layer or field name is missing", str(p.lineno(1)))
        
        # Check if the layer is valid
        if id_counter == 1:
            if substring_found not in layer_names: 
                print_error("Error: layer name unknown, you can use only APP or TRA or NET or MAC", str(p.lineno(1)))
        
        # Remove substring found
        full_path = full_path.replace(substring_found, "", 1)
        
        if len(full_path) == 0:
            break;
        
        # Search '.' in the string head
        try:
            substring_found = re.search('^\.', full_path).group(0)
        except AttributeError:
            print_error("Error: layer.field attribute in packet-filter has a bad structure, '.' is missing", str(p.lineno(1)))
        
        full_path = full_path.replace(substring_found, "", 1)
    
        if len(full_path) == 0:
            print_error("Error: layer.field attribute in change action has a bad structure, field name is missing", str(p.lineno(1)))    
    
    if id_counter < 2:
        print_error("Error: layer.field attribute in packet-filter has a bad structure, field name is missing", str(p.lineno(1)))  

    # Full path checked
    p[0] = str(p[1])


# Generic operand number
def p_generic_operand_number(p):
    """
    generic_operand : INTEGER
                    | SIGNED_INTEGER
                    | REAL
                    | SIGNED_REAL
    """
    
    p[0] = str(p[1])


# Generic operand identifier
def p_generic_operand_identifier(p):
    """
    generic_operand : ID
                    | SELF
    """
    
    p[0] = str(p[1])


# Generic operand string or local list
def p_generic_operand_string(p):
    """
    generic_operand : STRING
    """
    
    # Remove char '"'
    operand = (str(p[1]).replace("\"", ""))
        
    # Check if operand is a simple string or a local list
    if re.search('\|', operand):
    
        local_list = operand
        
        # Check the consistency of the local list
        id_counter = 0
        while len(local_list) != 0:
            # Search number in the string head
            try:
                substring_found = re.search('^\-?\d+(\.\d+)?', local_list).group(0)
                id_counter += 1
            except AttributeError:
                print_error("Error: packet-filter's local list supports only list of double", str(p.lineno(1)))
            
            # Remove substring found
            local_list = local_list.replace(substring_found, "", 1)
            
            if len(local_list) == 0:
                break;
            
            # Search '|' in the string head
            try:
                substring_found = re.search('^\|', local_list).group(0)
            except AttributeError:
                print_error("Error: local list's elements in the packet-filter must be separated by the char '|'", str(p.lineno(1)))
            
            local_list = local_list.replace(substring_found, "", 1)
        
            if len(local_list) == 0:
                print_error("Error: missing double in the packet-filter's local list", str(p.lineno(1)))    
        
    # operand checked
    p[0] = operand
    

# Arithmetic operand
def p_arithmetic_operand(p):
    """
    arithmetic_operand : INTEGER
                       | SIGNED_INTEGER
                       | REAL
                       | SIGNED_REAL
    """
    
    p[0] = str(p[1])



# Comparison operator
def p_comp_operator(p):
    """
    comp_operator : EQUAL
                  | DIFFERENT
                  | GRTHAN
                  | LESSTHAN
                  | GREQTHAN
                  | LEQTHAN
    """
    
    p[0] = p[1]


# Arithmetic operator
def p_arithmetic_operator(p):
    """
    arithmetic_operator : PLUS
                        | MINUS
                        | TIMES
                        | DIVIDE
                        | MODULE
    """
    
    p[0] = p[1]


# Logic operator
def p_logic_operator(p):
    """
    logic_operator : AND
                   | OR
    """
    
    global packetFilterOperator
    packetFilterOperator = str(p[1]).upper()
    p[0] = str(p[1]).upper()
