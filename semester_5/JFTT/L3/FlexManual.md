

## Overview

This manual expands on the short reference and gives practical patterns, matching examples, common formatting, and notes on advanced features. It draws on the small scanners stored under `flex/examples/manual`.

### Typical workflow

- Create a flex input file (commonly with `.l` or `.lex` extension).
- Run: `flex file.l` → produces `lex.yy.c` (the generated C scanner).
- Compile: `gcc -o scanner lex.yy.c -lfl` (or `-ll` on some systems).
- Execute: `./scanner` (scanner reads from stdin by default unless you write your own `main`).

## File layout and sections

A flex input file has these sections in order:

1) Optional C declarations (between `%{` and `%}`) — copied to the top of the generated C file.
2) Optional Definitions — used for `#define`-like regex aliases and options.
3) The Rules section (starts after the first `%%`) — pattern/action pairs.
4) The User subroutines section (after the second `%%`) — arbitrary C functions, `main()`, helpers.

Example skeleton:

```
%{ /* C declarations: includes, typedefs, globals */ %}

NAME    [A-Za-z][A-Za-z0-9_]*
DIGIT   [0-9]

%%
{NAME}  { printf("IDENT(%s)\n", yytext); }
{DIGIT}+ { printf("NUM(%s)\n", yytext); }
.      { ECHO; }
%%

int main(void) { yylex(); return 0; }
```

## Pattern syntax and practical examples

Basic building blocks:
- Literal characters: `abc` matches that sequence.
- Character classes: `[abc]`, ranges like `[0-9]`, or combined `[A-Za-z0-9_]`.
- Negation: `[^0-9]` matches any character that is not a digit.
- Dot: `.` matches any character except newline (use `(.|\n)` if you want newline).
- Quantifiers — what each one means (apply to the preceding atom; use parentheses to quantify a group, e.g. `(ab)+`):
    - `x?` — zero or one occurrence of `x`. Example: `colou?r` matches `color` or `colour`.
    - `x*` — zero or more occurrences (may match the empty string). Example: `a*` matches ``, `a`, `aaaa`.
    - `x+` — one or more occurrences (requires at least one). Example: `[0-9]+` matches `7`, `42`, `1234`.
    - `{n}` — exactly n occurrences. Example: `a{3}` matches `aaa` only.
    - `{n,}` — n or more occurrences. Example: `\d{2,}` matches `12`, `123`, `1234`, ...
    - `{n,m}` — between n and m occurrences (inclusive). Example: `x{1,3}` matches `x`, `xx`, or `xxx`.
    - Note: flex quantifiers are greedy (they try to match as much as possible). flex does not support Perl-style lazy quantifiers (`*?`, `+?`) — use different rules or explicit patterns if you need minimal matching.
- Anchors: `^` (beginning of line), `$` (end of line).
- Alternation: `(foo|bar|baz)`.
- Definitions: put `NAME regex` in the Definitions section and use `{NAME}` in rules.
- Literal quoting: escape special chars with `\` or quote strings with `""`.

Practical examples:

- Match identifiers: `{LETTER}({LETTER}|{DIGIT})*` where `{LETTER}` is `[A-Za-z_]`.
- Integer constants: `0|[1-9][0-9]*` (avoid leading zeros if desired).
- Floating point (simple): `[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?`
- String literal (double-quoted, with escapes): `"([^"\\]|\\.)*"`

Longest-match behavior and rule ordering:

- flex always selects the longest possible match. If two rules both match the same longest text, the rule earlier in the file wins.
- Example: If you want `if` to be a keyword but also allow identifiers, put the `"if"` rule before the identifier rule so `if` is matched as a keyword.

## Expressions

Character class expressions appear inside a character class between `[` and `]` using the `[:name:]` form. Use them like `[[:digit:]]` or combined with other elements, e.g. `[[:alnum:]_]`.

Each expression:

- `[:alnum:]` — alphanumeric characters: letters and digits.  
    Example: `a`, `Z`, `0`.

- `[:alpha:]` — alphabetic letters only.  
    Example: `a`, `M`, `z`.

- `[:blank:]` — horizontal whitespace: space and tab.  
    Example: `' '` (space), `\t` (tab).

- `[:cntrl:]` — control characters (non-printing characters).  
    Example: NUL (`\0`), BEL (`\a`), DEL.

- `[:digit:]` — decimal digits `0`–`9`.  
    Example: `3`, `7`.

- `[:graph:]` — visible (printing) characters except space; letters, digits, punctuation.  
    Example: `A`, `9`, `!`.

- `[:lower:]` — lowercase letters.  
    Example: `a`, `m`, `z`.

- `[:print:]` — printable characters including space.  
    Example: `' '` (space), `A`, `5`.

- `[:punct:]` — punctuation characters (symbols and punctuation marks).  
    Example: `!`, `.`, `,`, `;`.

- `[:space:]` — all whitespace characters (space, tab, newline, vertical tab, form feed, carriage return).  
    Example: `' '`, `\n`, `\t`.

- `[:upper:]` — uppercase letters.  
    Example: `A`, `Q`, `Z`.

- `[:xdigit:]` — hexadecimal digits: `0`–`9`, `A`–`F`, `a`–`f`.  
    Example: `9`, `B`, `f`.

- `[:word:]` — (common extension) word characters: typically letters, digits, and underscore — equivalent to `[A-Za-z0-9_]`.  
    Example: `x`, `7`, `_`.

Notes:
- Use these only inside a character class: `[[:alpha:]]`, `[[:alnum:]\-]`, etc.
- You can mix them with ranges and literals: `[[:lower:]0-9_]`.
- The exact set for some classes depends on the locale and scanner implementation.
- `[:word:]` is not a standard POSIX class but is commonly provided (e.g., in flex) — if unsure, use `[[:alnum:]_]`.

## Actions and communicating with parser code

- In action blocks you write plain C code. Use `printf()`, `free()`, `strdup()`, etc.
- Use `yylval` (of type `YYSTYPE`) to pass semantic values to a parser like Bison. Example:

```
[0-9]+ { yylval.ival = atoi(yytext); return T_INTEGER; }
[A-Za-z_][A-Za-z0-9_]* { yylval.sval = strdup(yytext); return T_IDENT; }
```

- `yylloc` can be filled for location tracking (line/column) if your parser expects locations.
- `yywrap()` function called when the scanner reaches the end of input(file) .
- `yytext` is a null-terminated **string of the matched text**; `yyleng` is its length.
- `yylex()` is the main scanning function; it returns the token code (int) for the matched pattern.


## Example

This scanner compresses runs of spaces and tabs into a single space and discards whitespace that appears at the end of a line. Note: put the trailing-whitespace rule before the general whitespace rule so it wins when both could match the same text.

```
%%
[ \t]+$      { /* trailing whitespace: ignore */ }
[ \t]+       { putchar(' '); }   /* compress runs of blanks/tabs to one space */
\n           { putchar('\n'); }  /* preserve newlines */
.            { ECHO; }           /* echo any other character */
%%
```

- The first rule ([ \t]+$) matches whitespace immediately before end-of-line and does nothing (drops it).
- The second rule compresses other runs of spaces/tabs to a single space.
- Keep a newline rule to preserve line breaks; otherwise newlines could be swallowed by other rules.
- Place the rules in this order so the more specific trailing-whitespace case is chosen first.


## Advanced features (start conditions, yymore, unput, REJECT, EOF)

Start conditions let you write rules that are active only in specific scanner states. They are declared in the Definitions section:

```
%s COMMENT   /* exclusive start condition */
%x QUOTE     /* exclusive start condition */

%%
<INITIAL>"/*"    { BEGIN(COMMENT); }
<COMMENT>"*/"     { BEGIN(INITIAL); }
<QUOTE>"\""      { /* handle quoted string state */ }
```

Exclusive (`%x`) vs inclusive (`%s`) start conditions control whether rules are active only in that state or are active in addition to `INITIAL`.

yymore(): append the next matched token to the current `yytext` instead of replacing it. Useful when a token is matched in pieces (e.g., multiline strings or combined tokens):

```
"/*"    { yymore(); BEGIN(COMMENT); }
<COMMENT>[^*]+    { /* consume content, appended to yytext */ }
```

unput(c) puts character c back onto the input; useful for hand-crafted lookahead beyond regular lookahead.

REJECT tells flex to disregard the current rule and continue searching for other matches; it enables backtracking but is slow and rarely needed.

EOF rules: Use `<<EOF>>` to provide an action when end-of-file is encountered (useful for unterminated strings or cleanup):

```
<QUOTE><<EOF>> { fprintf(stderr, "unterminated string\n"); }
```

## Examples from `flex/examples/manual` (how to run and what to inspect)

- Basic word counting (`wc.lex`): demonstrates token counting (characters/words/lines). Try it to see how `yytext` and `yyleng` are used.
- Expression scanner (`expr.lex`): tokenizes numbers and operators for use with `expr.y`.
- `yymore.lex` / `yymore2.lex`: show `yymore()` behavior on multi-part tokens.
- `reject.lex`: demonstrates the `REJECT` macro; inspect it to understand backtracking.

Common run commands:

```bash
# build a single example (replace with the filename you want)
cp flex/examples/manual/expr.lex example.l
flex example.l
gcc -o example lex.yy.c -lfl
echo "1+2*3" | ./example
# or
./example < input.txt
```

If a `Makefile.examples` or `Makefile.am` exists in the examples folder, try `make -f flex/examples/manual/Makefile.examples` to build sample scanners (paths/names may vary).

## Formatting and style

- Keep definitions at the top of a scanner to make rules readable (`NAME`, `DIGIT`, etc.).
- Prefer `{NAME}` substitutions rather than repeating long character classes.
- Put keywords rules before identifiers to ensure keywords are recognized as distinct tokens.
- Keep actions small and delegate complex logic to functions in the User subroutines section.

## Performance and debugging tips

- Avoid `REJECT` and complicated backtracking where possible — they slow scanning.
- Use start conditions to limit active rules, which can speed up scanners.
- If debugging, add `printf("matched: %s\n", yytext);` in actions temporarily.

## Try it quickly

1) Choose an example scanner from `flex/examples/manual`, copy it to your working directory.
2) Run `flex file.l` and `gcc -o bin lex.yy.c -lfl`.
3) Provide input via pipe or file and inspect stdout/stderr.

## Where to learn more

- Official manual: http://flex.sourceforge.net/manual/
- `info flex` or `man flex` on Unix-like systems.

---

If you'd like, I can:

- Run a specific example from `flex/examples/manual` and append its sample input/output to this manual.
- Convert the example index into a full table with one-line behavior descriptions and links.
- Add a short section showing how to integrate a flex scanner with a Bison parser (simple `parser.y` + `scanner.l`).


