"""
Grammar rules for ADL error handlers.
 
Author:
 + Francesco Racciatti <racciatti.francesco@gmail.com> 
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>

"""

# Import lexer for ADL
from lexer.adllexer import *


# Error in attack declaration
def p_physical_attack_error(p):
    "attack : FROM error DO LCBRACKET codeblock RCBRACKET"
    
    print "Error: wrong attack declaration - Line " + str(p.lineno(2))


# Error in variable declaration
def p_statement_vardef_error(p):
    """
    statement : VAR ID EQUALS error
              | VAR error
    """
    
    print "Error: something missing in the variable declaration - Line " + str(p.lineno(2))


# Error in physical primitives (physical attack)
def p_physical_funcall_error(p):
    """
    physical_statement : DESTROY LPAREN error RPAREN
                       | MOVE LPAREN error RPAREN
                       | DISABLE LPAREN error RPAREN
    """
    
    print "Error: syntax wrong in " + p[1] + "() argument declaration - Line " + str(p.lineno(3))


# Error in logical primitives (conditional or unconditional attack)
def p_logical_funcall_error(p):
    """
    logical_statement : DROP LPAREN error RPAREN
                      | CHANGE LPAREN error RPAREN
                      | RETRIEVE LPAREN error RPAREN
                      | CLONE LPAREN error RPAREN
                      | CREATE LPAREN error RPAREN
                      | SEND LPAREN error RPAREN
                      | PUT LPAREN error RPAREN
    """
    
    print "Error: syntax wrong in " + p[1] + "() argument declaration - Line " + str(p.lineno(1))


# Invocation of an undefined phisical primitive
def p_undefined_physical_funcall_error(p):
    "physical_statement : error LPAREN args RPAREN"
    
    print "Error: undefined primitive - Line " + str(p.lineno(1))
	
	
# Invocation of an undefined logical primitive
def p_undefined_logical_funcall_error(p):
    "logical_statement : error LPAREN args RPAREN"
    
    print "Error: undefined primitive - Line " + str(p.lineno(1))


# Generic error handler
def p_error(p):
    tok = lexer.token()
    print "Error: error - Line " +str(tok.lineno) + " " + str(tok)
    
