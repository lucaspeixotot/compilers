from token import Token
import re

letter = "[A-Za-z]"
digit = "[0-9]"
symbol = "\\s|!|#|\\$|%|&|\\”|\\(|\\)|\\*|\\+|\\,|\\-|\\.|\\/|:|;|<|=|>|\\?|@|[|]|\\^|_|\\‘|\\{|\\||}|\\∼"


regex_rules = [
    (digit + '+', 'CONST_INT'),
    (digit + '+.' + digit, 'CONST_FLOAT'),
    ("\"(" + letter + "|" + digit + "|" + symbol + ")*\"", 'CONST_STRING'),
    ("\"(" + letter + "|" + digit + "|" + symbol + ")\"", 'CONST_CHAR'),
    ("(true|false)", 'CONST_BOOL'),
    ("(" + letter + "|_)\w*" , 'ID'),
    ('\-', 'OP_MINUS'),
    ('\+', 'OP_PLUS'),
    ('\/', 'OP_DIVIDE'),
    ('\*', 'OP_MULT'),
    ('\=\=', 'OP_EQUAL'),
    ('\!=', 'OP_DIFF'),
    ('\<\=', 'OP_LEQ'),
    ('\<', 'OP_LESS'),
    ('\>\=', 'OP_GEQ'),
    ('\>', 'OP_GREATER'),
    ('\|\|', 'OP_OR'),
    ('\&\&', 'OP_AND'),
    ('\!', 'OP_NOT'),
    ('\<\<', 'OP_CONCAT'),
    ('\(', 'OPEN_PAREN'),
    ('\)', 'CLOSE_PAREN'),
    ('\[', 'OPEN_BRACKETS'),
    ('\]', 'CLOSE_BRACKETS'),
    ('\{', 'OPEN_KEYS'),
    ('\}', 'CLOSE_KEYS'),
    ('\;', 'SEMICOLON'),
    (',', 'COMMA'),
    ('\"', 'DOUBLE_QUOTES'),
    ('int', 'INT_TYPE'),
    ('char', 'CHAR_TYPE'),
    ('string', 'STRING_TYPE'),
    ('bool', 'BOOL_TYPE'),
    ('float', 'FLOAT_TYPE'),
    ('%', 'FORMAT_FIELD'),
    ('%%', 'FORMAT_DECIMAL'),
    ('defun', 'DEFUN'),
    ('defint', 'DEFINT'),
    ('defchar', 'DEFCHAR'),
    ('deffloat', 'DEFFLOAT'),
    ('defbool', 'DEFBOOL'),
    ('defstring', 'DEFSTRING'),
    ('if', 'IF'),
    ('else', 'ELSE'),
    ('for', 'FOR'),
    ('while', 'WHILE'),
    ('echo', 'ECHO'),
    ('return', 'RETURN'),
    ('break', 'BREAK'),
    ('continue', 'CONTINUE'),
    ('nil', 'NIL'),
    ('set', 'SET'),
    ('read', 'READ')
]

class NoMatchExistenceRules(Exception) :
    pass

class Lexer :
    def __init__(self) :
        self.line = 0
        self.column = 0
        self.program = None
        self.end_program = False
        regexes = []
        for regex, type in regex_rules :
            regexes.append('(?P<%s>%s)' % (type, regex))
        self.regex = re.compile('|'.join(regexes))
        self.regex_whitespace = re.compile('\S')

    def open_file(self, name) :
        try :
            f = open(name, "r")
        except:
            print("Error opening the file %s!" % name)
            return 1

        i = f.read()
        self.program = i.split("\n")
            
        f.close()

        return 0

    def next_token(self) :
        if self.end_program :
            return None
        
        if self.column >= len(self.program[self.line]) :
            self.line += 1
            self.column = 0
            if self.line >= len(self.program) :
                self.end_program = True
                return Token(self.line+1, self.column+1, "EOF", "")

        line = self.program[self.line]

        while line == '' :
            self.line += 1
            self.column = 0
            if self.line >= len(self.program) :
                self.end_program = True
                return Token(self.line+1, self.column+1, "EOF", "")
            line = self.program[self.line]
            
        self.column = self.regex_whitespace.search(line, self.column).start()
        m = self.regex.match(line, self.column)
        if m :
            val = m.group()
            cat_val = m.lastgroup
            tk = Token(self.line+1, self.column+1, cat_val, val)
            self.column = m.end()
            return tk

        print(self.line)
        print(self.column)
        print(line)
        raise NoMatchExistenceRules()
            
        
        
            
