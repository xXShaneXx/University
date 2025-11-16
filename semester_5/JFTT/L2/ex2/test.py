
empty_string = ""
empty_string_2 = ''
empty_triple = """"""
empty_triple_2 = ''''''




escaped_double = "To jest \" cytat w stringu. # To NIE jest \" komentarz." 
print(escaped_double) 

escaped_single = 'Tutaj z kolei jest \' apostrof. # To też NIE jest komentarz.' 
print(escaped_single) 






tricky_triple = """
To jest string. Dwa cudzysłowy "" to nie to samo co trzy.
A to jest # hash wewnątrz.
""" 
print(tricky_triple)

tricky_triple_2 = '''
Podobnie tutaj. Dwa apostrofy '' nie kończą stringa.
'''
print(tricky_triple_2)


mixed = 'string' + "#" + """hash""" 
print(mixed)

lonely_hash = "#"