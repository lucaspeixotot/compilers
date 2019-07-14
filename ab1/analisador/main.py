from lexer import Lexer

tokens = []

def main():
    lexer = Lexer()
    # lexer.open_file("hello-world.lc")
    lexer.open_file("fibonacci.lc")

    token = lexer.next_token()
    while token is not None :
        tokens.append(token)
        token = lexer.next_token()

    for x in tokens :
        print(x.to_string())

if __name__ == "__main__":
    main()      


    
