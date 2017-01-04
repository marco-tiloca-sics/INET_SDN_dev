"""
Printer functions, used to print messages and to print files.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


# Import classes to model (physical) primitives
from primitives.destroy import Destroy
from primitives.disable import Disable
from primitives.move import Move

# Import classes to model attacks
from attacks.physical import PhysicalAttack
from attacks.conditional import ConditionalAttack
from attacks.unconditional import UnconditionalAttack

# Import utilities
from config import *

    
def write_output_file(out_name):
    """
    Write the xml output file by printing in it the attack statements
    """
    
    # Open the output file
    out_file = open(out_name,"w")
    
    # Write the XML preamble
    out_file.write("<?xml version=\"1.0\"?>\n")
    out_file.write("\n<configuration>\n")
    
    # Write physical attacks
    if (len(destroy_actions) or len(disable_actions) or len(move_actions)):
        
        # Begin physical attack section
        out_file.write("\n\t<Physical>\n")
        
        # Build compact 'destroy' blocks
        if len(destroy_actions):
            action = Destroy()
            actions.append(action) # Actually it is only one action, with no parameters
        
        for key in destroy_actions:
            attack = PhysicalAttack(key, destroy_actions[key], actions)
            physical_attacks.append(attack)
            
        destroy_actions.clear()
        del actions[:]
        
        # Build compact 'disable' blocks
        if len(disable_actions):
            action = Disable()
            actions.append(action) # Actually it is only one action, with no parameters
        
        for key in disable_actions:
            attack = PhysicalAttack(key, disable_actions[key], actions)
            physical_attacks.append(attack)
            
        disable_actions.clear()
        del actions[:]
        
        # Build compact 'move' blocks
        if len(move_actions):
            # key1 is the occurrence time
            for key1 in move_actions:
                # key2 includes the new coordinates as 'x:y:z'
                for key2 in move_actions[key1]: 
                    action = Move(key2)
                    actions.append(action) # Actually it is only one action with new coordinates as argument
                    
                    attack = PhysicalAttack(key1, move_actions[key1][key2], actions)
                    physical_attacks.append(attack)
                    del actions[:]
            
            move_actions.clear()
        
        # Write an XML block for each physical attack
        for attack in physical_attacks:
            out_file.write(str(attack))
        
        # End physical attack section
        out_file.write("\n\t</Physical>\n")
    
    # Write conditional attacks
    if len(conditional_attacks):
        
        # Begin conditional attack section
        out_file.write("\n\t<Conditional>\n")
        
        # Write an XML block for each conditional attack
        for attack in conditional_attacks:
            out_file.write(str(attack))
        
        # End conditional attack section
        out_file.write("\n\t</Conditional>\n")
    
    # Write unconditional attacks
    if len(unconditional_attacks):
        
        # Open unconditional attack section
        out_file.write("\n\t<Unconditional>\n")
        
        # Write an XML block for each unconditional attack
        for attack in unconditional_attacks:
            out_file.write(str(attack))
        
        # End unconditional attack section
        out_file.write("\n\t</Unconditional>\n")	
        
    # Write the XML epilogue
    out_file.write("\n</configuration>")
    
    # Clear the global symbol table
    global_symbol_table.clear()
    
    # Close the output file
    out_file.close()

