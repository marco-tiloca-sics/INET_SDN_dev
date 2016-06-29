#!/usr/bin/env python

"""
This file defines the grammar of the Attack Description Language (ADL).

Authors:
 + Alessandro Pischedda <alessandro.pischedda@gmail.com>
 + Marco Tiloca         <marco.tiloca84@gmail.com>
 + Francesco Racciatti  <racciatti.francesco@gmail.com>
"""


import sys
sys.path.insert(0,"../..")

import ply.lex as lex

import re


# List of tokens
tokens = (
    # Types
    'ID',
    'INTEGER',
    'REAL',
    'STRING',
    'SIGNED_INTEGER',
    'SIGNED_REAL',
    'VAR',
    'LIST',
    'PACKET', 
    # Arithmetic operators
    'PLUS',
    'MINUS',
    'TIMES',
    'DIVIDE',
    'MODULE',
    # Assignment operators
    'EQUALS',
    'PLUSEQ',
    'MINUSEQ',
    'TIMESEQ',
    'DIVIDEQ',
    'MODULEQ',    
    # Comparison operators
    'GRTHAN',
    'LESSTHAN',
    'GREQTHAN',
    'LEQTHAN',
    'EQUAL',
    'DIFFERENT',
    # Logical operators
    'AND',
    'OR',
    # List operators
    'NODES',
    'IN',
    # Brackets
    'LPAREN',
    'RPAREN',
    'LCBRACKET',
    'RCBRACKET',
    # Punctuation
    'COMMA',
    # Cyclic statements
    'FROM',
    'EVERY',
    'DO',
    # Conditional statements
    'FILTER',
    #'IF',
    #'THEN',
    #'ELSE',
    # Primitive keywords
    'DESTROY',
    'DISABLE',
    'MOVE',
    'DROP',
    'CHANGE',
    'RETRIEVE',
    'CLONE',
    'CREATE',
    'SEND',
    'PUT',
    # Well-known values
    'TX',
    'RX',
    'TRUE',
    'FALSE',
    'ORIGINAL',
    'SELF',
    # TODO: check EXP
    'EXP',
    # Function
    'FUNCTION',
)


# Reserved keywords
reserved = {
    #'if'   : 'IF',
    #'then' : 'THEN',
    #'else' : 'ELSE',
    'var'  : 'VAR',
    'packet' : 'PACKET',
    'from' : 'FROM',
    'do' : 'DO',
    'in' : 'IN',
    'every' : 'EVERY',
    'list' : 'LIST',
    'nodes' : 'NODES',
    'filter' : 'FILTER',
    'and' : 'AND',
    'or' : 'OR',
    'TX' : 'TX',
    'RX' : 'RX',
    'TRUE' : 'TRUE',
    'FALSE' : 'FALSE',
    'original' : 'ORIGINAL',
    'SELF' : 'SELF',
    # Primitive reserved keywords
    'destroy' : 'DESTROY',
    'disable' : 'DISABLE',
    'move' : 'MOVE',
    'drop' : 'DROP',
    'change' : 'CHANGE',
    'retrieve' : 'RETRIEVE',
    'clone' : 'CLONE',
    'create' : 'CREATE',
    'send' : 'SEND',
    'put' : 'PUT',
    # Function
    'function' : 'FUNCTION',
}


# Token parsing rules
t_PLUS = r'\+'
t_MINUS	= r'-'
t_EXP = r'\*\*'
t_TIMES = r'\*'
t_DIVIDE = r'/'
t_MODULE = r'%'
t_EQUALS = r'='
t_PLUSEQ = r'\+='
t_MINUSEQ = r'-='
t_TIMESEQ = r'\*='
t_DIVIDEQ = r'/='
t_MODULEQ = r'%='
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LCBRACKET = r'\{'
t_RCBRACKET = r'\}'
t_COMMA = r'\,'
t_STRING = r'\"([^\\"]|(\\.))*\"'
t_EQUAL = r'=='
t_DIFFERENT = r'!='
t_GRTHAN = r'>'
t_GREQTHAN = r'>='
t_LESSTHAN = r'<'
t_LEQTHAN = r'<='


# An identifier has been found (it can be a reserved keyword)
def t_ID(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    
    # Check if the identifier is a keyword
    if t.value in reserved.keys():
        t.type = reserved[t.value]
    else:
        t.type = 'ID'
    
    return t


# A real number has been found
def t_REAL(t):
	r'\d+\.\d+'
	try:
		t.value = float(t.value)
	except ValueError:
		raise SyntaxError
		t.value = 0
	
	return t


# A signed real number has been found
def t_SIGNED_REAL(t):
	r'-\d+\.\d+'
	try:
		t.value = float(t.value)
	except ValueError:
		raise SyntaxError
		t.value = 0
	
	return t


# An integer number has been found
def t_INTEGER(t):
	r'\d+'
	try:
		t.value = int(t.value)
	except ValueError:
		raise SyntaxError
		t.value = 0
	
	return t


# A signed integer number has been found
def t_SIGNED_INTEGER(t):
	r'-\d+'
	try:
		t.value = int(t.value)
	except ValueError:
		raise SyntaxError
		t.value = 0
	
	return t


# When the lexer finds a new line it has to increment the line counter
def t_newline(t):
    r'\n+'
    t.lexer.lineno += t.value.count("\n")
    pass    


# Ignore tab occurences
t_ignore = " \t"


# Ignore comments
def t_comment(t):
    r'\#.*'
    pass


# Uncorrect statement or character
def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)


# Build the lexer
lexer = lex.lex()
