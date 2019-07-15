from token import Token
import re

letter = '[a-zA-Z]'
letter_underscore = '[a-zA-Z_]'
digit = '[0-9]'

regex_rules = [
    ('\d+', 'CONST_INT'),
    ('\d+.\d*', 'CONST_FLOAT'),
    ('[a-zA-Z0-1]')
    ('[A-Za-z_]\w+', 'ID'),
    ('\-', 'MINUS'),
    ('\+', 'PLUS'),
    ('\(', 'OPEN_PAREN'),
    ('\)', 'CLOSE_PAREN'),
    ('\{', 'OPEN_KEYS'),
    ('\}', 'CLOSE_KEYS'),
    ('\;', 'SEMICOLON'),
    ('\"', 'DOUBLE_QUOTES'),
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
        while "" in self.program :
            self.program.remove("")
            
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
        self.column = self.regex_whitespace.search(line, self.column).start()
        m = self.regex.match(line, self.column)
        if m :
            val = m.group()
            cat_val = m.lastgroup
            tk = Token(self.line+1, self.column+1, cat_val, val)
            self.column = m.end()
            return tk

        raise NoMatchExistenceRules()
            
        
        
            
