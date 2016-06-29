"""
Grammar rules for ADL expressions.
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


# Import the lexer for ADL
from lexer.adllexer import *

# Impot class to model expressions
from primitives.expression import Expression

# Import utilities
from engine.utilities import print_error

# Import global variables
from config import *


# Expression that involves an assignment
def p_statement_assign(p):
    """
    statement : ID EQUALS expression 
              | ID PLUSEQ expression
              | ID MINUSEQ expression
              | ID TIMESEQ expression
              | ID DIVIDEQ expression
              | ID MODULEQ expression
    """
    
    # check if the ID is already declared and if it is a variable
    if str(p[1]) not in symbol_table.keys():
        print_error("Error: '" + str(p[1]) + "' is not declared", str(p.lineno(1)) )
    
    else:
        if symbol_table[str(p[1])] != 'VAR':
            print_error("Error: '"+str(p[1])+"' is not a variable", str(p.lineno(1)) )	
    
    expression = str(p[3])
    expression += "\n\t\t\t\t<item>" + str(p[2]) + "</item>"
    expression += "\n\t\t\t\t<item>" + str(p[1]) + "</item>"
    action = Expression(expression)
    actions.append(action)


# Simple arithmetic operation
def p_expression_binop(p):
    """
    expression : expression PLUS expression
               | expression MINUS expression
               | expression TIMES expression
               | expression DIVIDE expression
               | expression MODULE expression
               | expression EXP expression
    """
    
    p[0] = str(p[1]) + str(p[3]) + "\n\t\t\t\t<item>" + str(p[2]) + "</item>"


# Uminus
def p_expression_uminus(p):
    "expression : MINUS expression %prec UMINUS"
    
    p[0] = -p[2]


# Group
def p_expression_group(p):
    "expression : LPAREN expression RPAREN"

    p[0] = p[2]


# String parsing
def p_expression_string(p):
    "expression : STRING"
    
    # If the string is not in the variable table add it
    if str(p[1]) not in variables.keys():
        value = p[1].replace("\"","")
        variables[str(p[1])] = "<value>" + str(value) + "</value><type>STRING</type>"
	
	p[0] =  "\n\t\t<item>" + str(p[1]) + "</item>"


# Number parsing
def p_expression_number(p):
    """
    expression : INTEGER 
               | SIGNED_INTEGER
               | REAL
               | SIGNED_REAL
    """
    
    # If the number is not in the variable table add it
    if str(p[1]) not in variables.keys():
        variables[str(p[1])] = "<value>" + str(p[1]) + "</value><type>NUMBER</type>"
    
    p[0] = "\n\t\t\t\t<item>" + str(p[1]) + "</item>"	


# Identifier parsing
def p_expression_id(p):
    "expression : ID"
    
    # Check if the ID is already declared and if it is a variable
    if str(p[1]) not in symbol_table.keys():
        print_error("Error: '" + str(p[1]) + "' is not declared", str(p.lineno(1)) )
    
    else:
        if symbol_table[str(p[1])] != 'VAR':
            print_error("Error: '" + str(p[1]) + "' is not a variable", str(p.lineno(1)) )
        else:
            p[0] = "\n\t\t\t\t<item>" + str(p[1]) + "</item>"
