"""
Grammar rules for (ADL) attacks.

An attack is either:
 + a single node primitive
 + a conditional attack block;
 + an unconditional attack block.
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca	<marco.tiloca84@gmail.com>
"""


# Import the lexer for ADL
from lexer.adllexer import *

# Import utilities
from engine.utilities import print_error
from engine.utilities import clear_data_structure

# Import classes to model attacks
from attacks.conditional import ConditionalAttack
from attacks.unconditional import UnconditionalAttack

# Import global variables
from config import *


# Attack (a single physical action; a conditional attack block; an unconditional attack block)
def p_attack(p):
    """
    attack : physical_statement
           | conditional_attack
           | unconditional_attack
    """
    #print "[p_attack] attack: " + str(p[1])


# Conditional attack
def p_conditional_attack(p):
    "conditional_attack : FROM time NODES IN list_id DO LCBRACKET filter_codeblock RCBRACKET"
    
    # Check if the node list has been declared
    if str(p[5]) not in lists.keys():
        print_error("Error: list '" + str(p[5]) + "' is not declared", str(p.lineno(5)))
    
    # Replace | with :
    node_list = lists[p[5]].replace("|", ":")
    
    # Build the attack and add it to the conditional attack list
    attack = ConditionalAttack(p[2], node_list, variables, actions, p[8])
    conditional_attacks.append(attack)
    
    # Clear data structures for the next attack
    clear_data_structure()


# Unconditional attack
def p_unconditional_attack(p):
    'unconditional_attack : FROM time EVERY time DO LCBRACKET codeblock RCBRACKET'
    
    # Build the attack and add it to the unconditional attack list
    attack = UnconditionalAttack(p[2], p[4], variables, actions)
    unconditional_attacks.append(attack)
    
    # Clear data structures for the next attack
    clear_data_structure()
