
grammar Calc;

prog: (assign ';')+ ;

assign: ID '=' expr ;

expr: expr op=('*'|'/') expr   # MulDiv
    | expr op=('+'|'-') expr   # AddSub
    | INT                      # Int      
    | ID                       # Var      
    | '(' expr ')'             # Parens   
    ;

ID: [a-zA-Z]+ ;       
INT: [0-9]+ ;        
WS: [ \t\r\n]+ -> skip ;