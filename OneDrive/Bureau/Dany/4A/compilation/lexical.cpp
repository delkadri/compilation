#include <iostream>
#include <string>
#include <cctype>

// Enum pour les types de tokens
enum TokenType {
    IDENTIFIER,NUMBER, ASSIGN, PLUS, MINUS, ASTERISK,   
    SLASH,PERCENT,LPAREN,RPAREN,LBRACE,RBRACE,LBRACKET,      
    RBRACKET,SEMICOLON,COMMA,AMPERSAND,LESS,GREATER,       
    LESSEQUAL,GREATEREQUAL,EQUAL,NOTEQUAL,IF,ELSE,      
    WHILE,DO,FOR,BREAK,CONTINUE,RETURN,INT, NOT,           
    AND,OR,END_OF_FILE,UNKNOWN        
};

// Structure pour un token
struct Token {
    int type;
    int ligne;
    int valeur;  
    std::string texte;  
};

// Variables globales pour stocker les tokens actuels et précédents
Token T, L;  
const char* src;  // Pointeur vers le code source
int current_line = 1;  // Compteur de ligne


void next() {
    L = T; 

    while (*src != '\0') {
        char c = *src;

        // Ignorer les espaces blancs et compter les lignes
        if (isspace(c)) {
            if (c == '\n') {
                current_line++;
            }
            src++;
            continue;
        }

        // Identifier les identificateurs et mots-clés
        if (isalpha(c)) {
            T.texte.clear();
            while (isalnum(c) || c == '_') {
                T.texte += c;
                c = *(++src);
            }
            if (T.texte == "if") T.type = IF;
            else if (T.texte == "else") T.type = ELSE;
            else if (T.texte == "while") T.type = WHILE;
            else if (T.texte == "do") T.type = DO;
            else if (T.texte == "for") T.type = FOR;
            else if (T.texte == "break") T.type = BREAK;
            else if (T.texte == "continue") T.type = CONTINUE;
            else if (T.texte == "return") T.type = RETURN;
            else if (T.texte == "int") T.type = INT;
            else T.type = IDENTIFIER;
            T.ligne = current_line;
            return;
        }

        // Identifier les nombres
        if (isdigit(c)) {
            T.valeur = 0;
            while (isdigit(c)) {
                T.valeur = T.valeur * 10 + (c - '0');
                c = *(++src);
            }
            T.type = NUMBER;
            T.ligne = current_line;
            return;
        }

        // Identifier les opérateurs et autres caractères
        switch (c) {
            case '+': T.type = PLUS; T.texte= "+"; src++; break;
            case '-': T.type = MINUS; T.texte= "-"; src++; break;
            case '*': T.type = ASTERISK; T.texte= "*"; src++; break;
            case '/': T.type = SLASH; T.texte= "/"; src++; break;
            case '%': T.type = PERCENT; T.texte= "%"; src++; break;
            case '=': 
                if (*(src + 1) == '=') {
                    T.type = EQUAL;
                    T.texte = "==";
                    src += 2;
                } else {
                    T.type = ASSIGN;
                    T.texte = "=";
                    src++;
                }
                break;
            case '<': 
                if (*(src + 1) == '=') {
                    T.type = LESSEQUAL;
                    T.texte = "<=";
                    src += 2;
                } else {
                    T.type = LESS;
                    T.texte = "<";
                    src++;
                }
                break;
            case '>': 
                if (*(src + 1) == '=') {
                    T.type = GREATEREQUAL;
                    T.texte = ">=";
                    src += 2;
                } else {
                    T.type = GREATER;
                    T.texte = ">";
                    src++;
                }
                break;
            case '!': 
                if (*(src + 1) == '=') {
                    T.type = NOTEQUAL;
                    T.texte = "!=";
                    src += 2;
                } else {
                    T.type = NOT;
                    T.texte = "!";
                    src++;
                }
                break;
            case '&':
                if (*(src + 1) == '&') {
                    T.type = AND;
                    T.texte = "&&";
                    src += 2;
                } else {
                    T.type = AMPERSAND;
                    T.texte = "&";
                    src++;
                }
                break;
            case '|':
                if (*(src + 1) == '|') {
                    T.type = OR;
                    T.texte = "||";
                    src += 2;
                }
                break;
            case '(': T.type = LPAREN; T.texte = "("; src++; break;
            case ')': T.type = RPAREN; T.texte = ")"; src++; break;
            case '{': T.type = LBRACE; T.texte = "{"; src++; break;
            case '}': T.type = RBRACE; T.texte = "}"; src++; break;
            case '[': T.type = LBRACKET; T.texte = "["; src++; break;
            case ']': T.type = RBRACKET; T.texte = "]"; src++; break;
            case ';': T.type = SEMICOLON; T.texte = ";"; src++; break;
            case ',': T.type = COMMA; T.texte = ","; src++; break;
            default: T.type = UNKNOWN; T.texte = c; src++; break;
        }
        T.ligne = current_line;
        return;
    }

    T.type = END_OF_FILE;
    T.ligne = current_line;
}

int check(int type) {
    if (T.type != type) return false;
    next();
    return true;
}

void erreurfatal(const std::string& message) {
    std::cerr << "Erreur fatale à la ligne " << T.ligne << ": " << message << std::endl;
    exit(EXIT_FAILURE);
}

void accept(int type) {
    if (T.type != type) erreurfatal("Token inattendu");
    next();
}

int main() {
    src = "int x = 10; if (x % 2 == 0 && y > 0 || !z) { y = y + 1; } else { y = 20; } while (x && y) { z = z || x; } return 0;";

    next();  // Initialiser T avec le premier token

    // Exemples d'utilisation de check() et accept() avec messages de débogage
    if (check(INT)) {
        std::cout << "INT détecté sur la ligne " << T.ligne << std::endl;
        accept(IDENTIFIER);
        std::cout << "IDENTIFIER accepté\n";
        accept(ASSIGN);
        std::cout << "ASSIGN accepté\n";
        accept(NUMBER);
        std::cout << "NUMBER accepté\n";
        accept(SEMICOLON);
        std::cout << "SEMICOLON accepté\n";
    }

    if (check(IF)) {
        std::cout << "IF détecté sur la ligne " << T.ligne << std::endl;
        accept(LPAREN);
        std::cout << "LPAREN accepté\n";
        accept(IDENTIFIER);  // Attend 'x'
        std::cout << "IDENTIFIER accepté pour x\n";
        accept(PERCENT);     // Attend '%'
        std::cout << "PERCENT accepté\n";
        accept(NUMBER);      // Attend '2'
        std::cout << "NUMBER accepté pour 2\n";
        accept(EQUAL);       // Attend '=='
        std::cout << "EQUAL accepté\n";
        accept(NUMBER);      // Attend '0'
        std::cout << "NUMBER accepté pour 0\n";
        accept(AND);         // Attend '&&'
        std::cout << "AND accepté\n";
        accept(IDENTIFIER);  // Attend 'y'
        std::cout << "IDENTIFIER accepté pour y\n";
        accept(GREATER);     // Attend '>'
        std::cout << "GREATER accepté\n";
        accept(NUMBER);      // Attend '0'
        std::cout << "NUMBER accepté pour 0\n";
        accept(OR);          // Attend '||'
        std::cout << "OR accepté\n";
        accept(NOT);         // Attend '!'
        std::cout << "NOT accepté\n";
        accept(IDENTIFIER);  // Attend 'z'
        std::cout << "IDENTIFIER accepté pour z\n";
        accept(RPAREN);
        std::cout << "RPAREN accepté\n";
        accept(LBRACE);
        std::cout << "LBRACE accepté\n";
        accept(IDENTIFIER);
        std::cout << "IDENTIFIER accepté\n";
        accept(ASSIGN);
        std::cout << "ASSIGN accepté\n";
        accept(IDENTIFIER);
        std::cout << "IDENTIFIER accepté\n";
        accept(PLUS);
        std::cout << "PLUS accepté\n";
        accept(NUMBER);
        std::cout << "NUMBER accepté\n";
        accept(SEMICOLON);
        std::cout << "SEMICOLON accepté\n";
        accept(RBRACE);
        std::cout << "RBRACE accepté\n";
    

    } else {
        std::cerr << "Erreur : Token IF non trouvé\n";
    }

    // Test pour "else { y = 20; }"
    if (check(ELSE)) {
        std::cout << "ELSE détecté sur la ligne " << T.ligne << std::endl;
        accept(LBRACE);
        std::cout << "LBRACE accepté\n";
        accept(IDENTIFIER);  // y
        accept(ASSIGN);      // =
        accept(NUMBER);      // 20
        accept(SEMICOLON);   // ;
        accept(RBRACE);      // }
        std::cout << "Bloc ELSE accepté\n";
    }

    // Test pour "while (x && y)"
    if (check(WHILE)) {
        std::cout << "WHILE détecté sur la ligne " << T.ligne << std::endl;
        accept(LPAREN);
        std::cout << "LPAREN accepté\n";
        accept(IDENTIFIER);  // x
        std::cout << "IDENTIFIER accepté pour x\n";
        accept(AND);         // &&
        std::cout << "AND accepté\n";
        accept(IDENTIFIER);  // y
        std::cout << "IDENTIFIER accepté pour y\n";
        accept(RPAREN);
        std::cout << "RPAREN accepté\n";
        accept(LBRACE);
        std::cout << "LBRACE accepté\n";
        accept(IDENTIFIER);  // z
        accept(ASSIGN);      // =
        accept(IDENTIFIER);  // z
        accept(OR);          // ||
        accept(IDENTIFIER);  // x
        accept(SEMICOLON);   // ;
        accept(RBRACE);      // }
        std::cout << "Bloc WHILE accepté\n";
    }

    // Test pour "return 0;"
    if (check(RETURN)) {
        std::cout << "RETURN détecté sur la ligne " << T.ligne << std::endl;
        accept(NUMBER);      // 0
        std::cout << "NUMBER accepté\n";
        accept(SEMICOLON);   // ;
        std::cout << "SEMICOLON accepté\n";
    }

    // Exemple d'utilisation de L
    std::cout << "Dernier token lu : " << L.texte << " à la ligne " << L.ligne << std::endl;

    return 0;
}

