import sys
import ply.lex as lex
import ply.yacc as pyyacc

P = 1234577

# ------------------------------------------------------------
# LEXER
# ------------------------------------------------------------
tokens = (
    'NUMBER',
    'PLUS', 'MINES', 'TIMES', 'DIV', 'POW',
    'LPAREN', 'RPAREN',
)

t_PLUS    = r'\+'
t_MINES   = r'-'
t_TIMES   = r'\*'
t_DIV     = r'/'
t_POW     = r'\^'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'

t_ignore  = ' \t'

def t_NUMBER(t):
    r'\d+'
    # Zachowujemy surową wartość liczby (nie redukujemy jej od razu modulo P),
    # aby móc poprawnie obsługiwać ujemne wykładniki i duże potęgi.
    raw = int(t.value)
    if raw >= P:
        print(f"Błąd: liczba {raw} zbyt duża (≥ p).")
    t.value = raw
    return t

def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

def t_backslash_continue(t):
    r'\\\n'
    pass  # ignorujemy \ na końcu linii

def t_comment(t):
    r'\#.*'
    pass  # ignorujemy komentarze

def t_error(t):
    print("Błąd leksykalny.")
    t.lexer.skip(1)

lexer = lex.lex()

# ------------------------------------------------------------
# PARSER
# ------------------------------------------------------------
precedence = (
    ('left', 'PLUS', 'MINES'),
    ('left', 'TIMES', 'DIV'),
    ('right', 'UMINUS'),
    ('right', 'POW'),
)

def p_statement_expr(p):
    "statement : expr"
    rpn, val, raw = p[1]
    print(" ".join(map(str, rpn)))
    print(f"Wynik: {val}")

def p_expr_binop(p):
    """expr : expr PLUS expr
            | expr MINES expr
            | expr TIMES expr
            | expr DIV expr"""
    left_rpn, left_val, left_raw = p[1]
    right_rpn, right_val, right_raw = p[3]
    op = p[2]

    # Operacje arytmetyczne wykonujemy na wartościach modulo P.
    if op == '+':
        result = (left_val + right_val) % P
        raw = None
        if left_raw is not None and right_raw is not None:
            raw = left_raw + right_raw
    elif op == '-':
        result = (left_val - right_val) % P
        raw = None
        if left_raw is not None and right_raw is not None:
            raw = left_raw - right_raw
    elif op == '*':
        result = (left_val * right_val) % P
        raw = None
        if left_raw is not None and right_raw is not None:
            raw = left_raw * right_raw
    else:  # '/'
        if right_val == 0:
            print("Błąd: dzielenie przez zero.")
            result = 0
            raw = None
        else:
            result = (left_val * pow(right_val, P-2, P)) % P
            raw = None

    p[0] = (left_rpn + right_rpn + [op], result, raw)

def p_expr_pow(p):
    "expr : expr POW expr"
    base_rpn, base_mod, base_raw = p[1]
    exp_rpn, exp_mod, exp_raw = p[3]

    # Zakaz składania potęg: nie wolno a^(b^c)
    if len(exp_rpn) > 0 and exp_rpn[-1] == '^':
        print("Błąd: nie można składać potęg.")
        return

    # Wybieramy surowy wykładnik, jeśli jest dostępny (np. nie był
    # wynikiem operacji, której surowej wartości nie przechowujemy).
    if exp_raw is not None:
        exp_raw_used = exp_raw
    else:
        exp_raw_used = exp_mod

    # Jeśli wykładnik jest ujemny — traktujemy go zgodnie z arytmetyką modularną
    # jako wykładnik modulo (P-1). Przy zerowej podstawie i ujemnym wykładniku
    # wystąpi błąd (dzielenie przez zero).
    if base_mod == 0 and exp_raw_used < 0:
        print("Błąd: podnoszenie zera do ujemnej potęgi.")
        p[0] = (base_rpn + exp_rpn + ['^'], 0, None)
        return

    exp_mod_for_pow = exp_raw_used % (P - 1)
    result = pow(base_mod, exp_mod_for_pow, P)
    p[0] = (base_rpn + exp_rpn + ['^'], result, None)

def p_expr_group(p):
    "expr : LPAREN expr RPAREN"
    p[0] = p[2]

def p_expr_number(p):
    "expr : NUMBER"
    raw = p[1]
    mod = raw % P
    # W RPN wypisujemy wartość zredukowaną modulo P (zgodnie z dotychczasowym
    # zachowaniem programu), ale jednocześnie przechowujemy surową wartość
    # dla operacji takich jak potęgowanie z ujemnym wykładnikiem.
    p[0] = ([mod], mod, raw)

def p_expr_uminus(p):
    "expr : MINES expr %prec UMINUS"
    rpn, val, raw = p[2]
    neg_raw = None if raw is None else -raw
    negated = (-val) % P
    p[0] = ([negated], negated, neg_raw)

def p_error(p):
    if p:
        print("Błąd składniowy.")
    else:
        pass  # EOF

parser = pyyacc.yacc()

# ------------------------------------------------------------
# Główna pętla
# ------------------------------------------------------------
def process_input():
    buffer = ""
    for line in sys.stdin:
        raw_line = line.rstrip('\n')

        # Komentarz?
        if raw_line.lstrip().startswith('#'):
            continue

        # Kontynuacja linii?
        if raw_line.endswith('\\'):
            buffer += raw_line[:-1]
            continue

        buffer += raw_line
        input_line = buffer.strip()

        if input_line == "":
            buffer = ""
            continue

        try:
            parser.parse(input_line + '\n', lexer=lexer)
        except Exception as e:
            print("Błąd.")
        finally:
            buffer = ""

if __name__ == "__main__":
    process_input()