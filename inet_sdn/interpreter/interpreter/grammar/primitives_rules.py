"""
Grammar rules for the primitives provided by ADL.
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca	<marco.tiloca84@gmail.com>
"""


# Import the lexer for ADL
from lexer.adllexer import *

# Import utilities
from engine.utilities import print_error
from engine.utilities import packet_check
from engine.utilities import check_layer_name
from engine.utilities import check_control_structure_name

# Import classes to model primitives
from primitives.destroy import Destroy
from primitives.disable import Disable
from primitives.move import Move
from primitives.drop import Drop
from primitives.send import Send
from primitives.clone import Clone
from primitives.create import Create
from primitives.change import Change
from primitives.retrieve import Retrieve
from primitives.put import Put

# Import global variables
from config import *


# destroy(int nodeID, double occurrence_time)
def p_statement_destroy(p):
    "physical_statement : DESTROY LPAREN node_id COMMA unsigned_real RPAREN"
    
    # 'destroy' invocations are grouped according to their occurrence time
    if not p[5] in destroy_actions.keys():
        destroy_actions[p[5]] = "" + str(p[3])
    
    else:
        destroy_actions[p[5]] = destroy_actions[p[5]] + ":" + str(p[3])


# disable(int node_id, double occurrence_time)
def p_statement_disable(p):
    "physical_statement : DISABLE LPAREN node_id COMMA time RPAREN"
    
    # 'disable' invocations are grouped according to their occurrence time
    if not p[5] in disable_actions.keys():
        disable_actions[p[5]] = "" + str(p[3])
    
    else:
        disable_actions[p[5]] = disable_actions[p[5]] + ":" + str(p[3])


# move(int node_id, double occurrence_time, double coord_x, double coord_y, double coord_z)
def p_statement_move(p):
    "physical_statement : MOVE LPAREN node_id COMMA time COMMA unsigned_real COMMA unsigned_real COMMA unsigned_real RPAREN"
    
    # 'move' invocations are grouped according to their occurrence time. They are also further sub-grouped according to specified coordinates
    move_args = "" + str(p[7]) + ":" + str(p[9]) + ":" + str(p[11])

    if not p[5] in move_actions.keys():
        move_actions[p[5]] = {}
    
    if not move_args in move_actions[p[5]].keys():
        move_actions[p[5]][move_args] = "" + str(p[3])
    
    else:
        move_actions[p[5]][move_args] = move_actions[p[5]][move_args] + ":" + str(p[3])


# drop (packet p, signed_double threshold)
def p_statement_drop(p):
    "logical_statement : DROP LPAREN identifier COMMA threshold RPAREN"
    
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    
    args = str(p[3]) + ":" + str(p[5])
    action = Drop(args)
    actions.append(action)


# send (packet p, double delay)
def p_statement_send(p):
    "logical_statement : SEND LPAREN identifier COMMA unsigned_real RPAREN"
    
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    
    args = str(p[3]) + ":" + str(p[5])
    action = Send(args)
    actions.append(action)


# clone (srcPacket p1, dstPacket p2)
def p_statement_clone(p):
    "logical_statement : CLONE LPAREN identifier COMMA identifier RPAREN"
    
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    packet_check(p[5], p.lineno(1))
    
    args = str(p[3]) + ":" + str(p[5])
    action = Clone(args)
    actions.append(action)


# create (packet p, string layer1.type, type_t content1, string layer2.type, type_t content2, ...)
def p_statement_create(p):
    "logical_statement : CREATE LPAREN identifier COMMA create_list RPAREN"
    
    packet_check(p[3], p.lineno(1))
    
    args = str(p[3]) + ":" + str(p[5])
    
    action = Create(args)
    actions.append(action)


# change (packet p, string layer.field, type_t content)
def p_statement_change(p):
    "logical_statement : CHANGE LPAREN identifier COMMA layer_field_pair COMMA multi_type RPAREN"
    
    # Check the third argument
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    
    # p[7] is multi_type (NUMBER, STRING, ID) 
    if p[7] not in reserved_name and p[7] not in symbol_table.keys():
        
        re_pattern = r"^-?\d+(\.\d+)?"
        pattern = re.compile(re_pattern)
        
        # Add an entry in the variable table if STRING is not present in it
        if p[7][0] == "\"" and p[7][-1] == "\"":
            symbol_table[p[7]] = "VAR"
            variables[p[7]] = "<value>" + p[7][1:-1] + "</value><type>STRING</type>"
        
        # Add an entry in the variable table if NUMBER is not present in it
        elif re.match(pattern, p[7]):
            symbol_table[p[7]] = "VAR"
            variables[p[7]] = "<value>" + p[7] + "</value><type>NUMBER</type>"
        # Return error if the ID is not declared
        else:
            print_error("Error: '" + p[7] + "' undefined variable identifier2", str(p.lineno(1)) )

    # Check if the variable is initialized
    if p[7] not in reserved_name:
        #check if the argument is a declared packet
        if symbol_table[p[7]] == 'PACKET':
            value = p[7]
        else:
            value = variables[p[7]][7]
     
        # Variable not initialized if its first char is '<'
        if value == "<":
            print_error("Error: variable '" + p[7] + "' must be initialized", str(p.lineno(1)) )
    
    # Check layer name and control structure name
    if check_layer_name(p[5]) == False:
        if check_control_structure_name(p[5]) == False:
            print_error("Error: layer name or control structure name unknown, you can use only: APP or TRA or NET or MAC or controlInfo or attackInfo or sending", str(p.lineno(1)))
    
    # Check coerency of layer_field structure "layer.field1.field2.field3 ..."
    layer_field = str(p[5])
    id_counter = 0
    while len(layer_field) != 0:
        # search identifier in the string head
        try:
            substring_found = re.search('^[a-zA-Z_][a-zA-Z_0-9]*', layer_field).group(0)
            id_counter += 1
        except AttributeError:
            print_error("Error: layer.field attribute in change action has a bad structure, id missing", str(p.lineno(1)))
        
        # remove substring found
        layer_field = layer_field.replace(substring_found, "", 1)
        
        if len(layer_field) == 0:
            break;
        
        # search '.' in the string head
        try:
            substring_found = re.search('^\.', layer_field).group(0)
        except AttributeError:
            print_error("Error: layer.field attribute in change action has a bad structure, '.' missing", str(p.lineno(1)))
        
        layer_field = layer_field.replace(substring_found, "", 1)
    
        if len(layer_field) == 0:
            print_error("Error: layer.field attribute in change action has a bad structure, id missing", str(p.lineno(1)))    
    
    if id_counter < 2:
        print_error("Error: layer.field attribute in change action has a bad structure, field missing", str(p.lineno(1)))    
    
    # Coerency test passed, build the object
    args = str(p[3]) + ":" + str(p[5]) + ":" + str(p[7])
    action = Change(args)
    actions.append(action)


# retrieve (packet p, string layer.field, ID content)
def p_statement_retrieve(p):
    "logical_statement : RETRIEVE LPAREN identifier COMMA layer_field_pair COMMA identifier RPAREN"
    
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    
    # Check if the variable is reserved  
 	if p[7] in reserved_name:
		print_error("Error: '" + p[7] + "' is a reserved name", str(p.lineno(1)) )
      
    # Check if the variable has been previously declared
    if p[7] not in symbol_table.keys():
        print_error("Error: '" + p[7] + "' undefined variable identifier", str(p.lineno(1)) )
      
	# Check if the name has been previously used to declare a packet
    if p[7] in symbol_table.keys() and symbol_table[p[7]] != "VAR":
        print_error("Error: ID overloading is not allowed", str(p.lineno(1)) )
    
    # Check if the layer name is valid
    if check_layer_name(p[5]) == False and check_control_structure_name(p[5]) == False:
        print_error("Error: layer name unknown, you can use only: APP or TRA or NET or MAC or controlInfo", str(p.lineno(1)))
    
    # Check coerency of layer_field structure "layer.field1.field2.field3 ..."
    layer_field = str(p[5])
    id_counter = 0
    while len(layer_field) != 0:
        # search identifier in the string head
        try:
            substring_found = re.search('^[a-zA-Z_][a-zA-Z_0-9]*', layer_field).group(0)
            id_counter += 1
        except AttributeError:
            print_error("Error: layer.field attribute in retrieve action has a bad structure, id missing", str(p.lineno(1)))
        
        # remove substring found
        layer_field = layer_field.replace(substring_found, "", 1)
        
        if len(layer_field) == 0:
            break;
        
        # search '.' in the string head
        try:
            substring_found = re.search('^\.', layer_field).group(0)
        except AttributeError:
            print_error("Error: layer.field attribute in retrieve action has a bad structure, '.' missing", str(p.lineno(1)))
        
        layer_field = layer_field.replace(substring_found, "", 1)
    
        if len(layer_field) == 0:
            print_error("Error: layer.field attribute in retrieve action has a bad structure, id missing", str(p.lineno(1)))    
    
    if id_counter < 2:
        print_error("Error: layer.field attribute in retrieve action has a bad structure, field missing", str(p.lineno(1)))    
    
    # Coerency test passed, build the object
    args = str(p[3]) + ":" + str(p[5]) + ":" + str(p[7])
    action = Retrieve(args)
    actions.append(action)


# put (packet p, list dstNodes, direction dir, bool updateStat, double delay)
def p_statement_put(p):
    "logical_statement : PUT LPAREN identifier COMMA identifier COMMA direction COMMA boolean COMMA unsigned_real RPAREN"
    
    if p[3] != "original":
        packet_check(p[3], p.lineno(1))
    
    # Check if the list has been already declared
    if p[5] not in lists.keys():
        print_error("Error: list '" + p[5] + "' is not declared", str(p.lineno(1)) )
    
    # Replace the second argument with the list content
    p[5] = str(lists[str(p[5])])
    args = str(p[3]) + ":" + str(p[5]) + ":" + str(p[7]) + ":" + str(p[9]) + ":" + str(p[11])
    
    action = Put(args)
    actions.append(action)
