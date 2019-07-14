val_categories = {
    'CONST_INT': 0,
    'CONST_FLOAT': 1,
    'CONST_CHAR': 2,
    'CONST_STRING': 3,
    'CONST_BOOL': 4,
    'OP_MINUS': 5,
    'OP_PLUS': 6,
    'OP_DIVIDE': 7,
    'OP_MULT': 8,
    'OP_EQUAL': 9,
    'OP_DIFF': 10,
    'OP_LESS': 11,
    'OP_LEQ': 12,
    'OP_GREATER': 13,
    'OP_GEQ': 14,
    'OP_OR': 15,
    'OP_AND': 16,
    'OP_NOT': 17,
    'OP_CONCAT': 18,
    'OPEN_PAREN': 19,
    'CLOSE_PAREN': 20,
    'OPEN_BRACKETS': 21,
    'CLOSE_BRACKETS': 22,
    'OPEN_KEYS': 23,
    'CLOSE_KEYS': 24,
    'SEMICOLON': 25,
    'COMMA': 26,
    'DOUBLE_QUOTES': 27,
    'INT_TYPE': 28,
    'CHAR_TYPE': 29,
    'STRING_TYPE': 30,
    'BOOL_TYPE': 31,
    'FLOAT_TYPE': 32,
    'FORMAT_FIELD': 33,
    'FORMAT_DECIMAL': 34,
    'ID': 35,
    'DEFUN': 36,
    'DEFINT': 37,
    'DEFCHAR': 38,
    'DEFFLOAT': 39,
    'DEFBOOL': 40,
    'DEFSTRING': 41,
    'IF': 42,
    'ELSE': 43,
    'FOR': 44,
    'WHILE': 45,
    'ECHO': 46,
    'RETURN': 47,
    'BREAK': 48,
    'CONTINUE': 49,
    'NIL': 50,
    'SET': 51,
    'READ': 52,
    'EOF': 53
}

reserved_words_table = {
    'defun': "DEFUN",
    'echo': "ECHO",
    'return': "RETURN",
    'int': "INT_TYPE",
    'char': "CHAR_TYPE",
    'string': "STRING_TYPE",
    'bool': "BOOL_TYPE",
    'float': "FLOAT_TYPE",
    'defint': 'DEFINT',
    'defchar': "DEFCHAR",
    'defstring': "DEFSTRING",
    'defbool': "DEFBOOL",
    'deffloat': "DEFFLOAT",
    'if': "IF",
    'else': "ELSE",
    'for': "FOR",
    'while': "WHILE",
    'echo': "ECHO",
    'return': "RETURN",
    'break': "BREAK",
    'continue': "CONTINUE",
    'nil': "NIL",
    'set': "SET",
    'read': "READ"
    
}

class CatValInvalid(Exception) :
    pass

class Category :
    def __init__(self, val, cat_val) :
        if cat_val in val_categories :
            if val in reserved_words_table:
                self.val = reserved_words_table[val]
            else :
                self.val = cat_val
            self.id = val_categories[self.val]
        else :
            raise CatValInvalid()

class Token :
    def __init__(self, line, column, cat_val, val) :
        self.line = line
        self.column = column
        self.cat = Category(val, cat_val)
        self.val = val

    def to_string(self) :
        return "Token: \"%s\" (%s) at line %d in column %d" % (self.val, self.cat.val, self.line, self.column)
