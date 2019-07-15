val_categories = {
    'CONST_INT': ,
    'CONST_FLOAT': ,
    'CONST_CHAR': ,
    'CONST_STRING': ,
    'CONST_BOOL': ,
    'OP_MINUS': ,
    'OP_PLUS': ,
    'OP_DIVIDE': ,
    'OP_MULT': ,
    'OP_EQUAL': ,
    'OP_DIFF': ,
    'OP_LESS': ,
    'OP_LEQ': ,
    'OP_GREATER': ,
    'OP_GEQ': ,
    'OP_OR': ,
    'OP_AND': ,
    'OP_NOT': ,
    'OP_CONCAT': ,
    'OPEN_PAREN': ,
    'CLOSE_PAREN': ,
    'OPEN_BRACKETS': ,
    'CLOSE_BRACKETS': ,
    'OPEN_KEYS': ,
    'CLOSE_KEYS': ,
    'SEMICOLON': ,
    'COMMA': ,
    'DOUBLE_QUOTES': ,
    'INT_TYPE': 
    'CHAR_TYPE': ,
    'STRING_TYPE': ,
    'BOOL_TYPE': ,
    'FLOAT_TYPE': ,
    'FORMAT_FIELD': ,
    'FORMAT_DECIMAL': ,
    'ID': ,
    'DEFUN': ,
    'DEFINT': ,
    'DEFCHAR': ,
    'DEFFLOAT': ,
    'DEFBOOL': ,
    'DEFSTRING': ,
    'IF': ,
    'ELSE': ,
    'FOR': ,
    'WHILE': ,
    'ECHO': ,
    'RETURN': ,
    'BREAK': ,
    'CONTINUE': ,
    'NIL': ,
    'SET': ,
    'EOF': ,
}

reserved_words_table = {
    'defun': "DEFUN",
    'echo': "ECHO",
    'return': "RETURN",
    'int': "INT_TYPE",
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
