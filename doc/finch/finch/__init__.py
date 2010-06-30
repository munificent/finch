from pygments import highlight
from pygments.lexers import PythonLexer
from pygments.formatters import HtmlFormatter

from pygments.lexer import RegexLexer
from pygments.token import *

print 'loading'

class FinchLexer(RegexLexer):
    print 'constructed lexer'
    
    name = 'Finch'
    aliases = ['finch']
    filenames = ['*.fin']

    tokens = {
        'root': [
            (r'\s+', Text),
            
            # reserved words
            (r'(self|undefined)', Name.Builtin),
            (r'(\<\-|\<\-\-|\.|;)', Operator),
             
            # built-in operators and punctuation
            (r'[,()\\\[\]{}]', Punctuation),
            
            # comments
            (r'\'.*?\n', Comment.Single),
            
            # literals
            
            # numbers
            (r'-?\d+\.\d+', Number.Float),
            (r'-?\d+', Number.Integer),
            
            # strings
            (r'L?"', String, 'string'),
            
            # user-defined names
            (r'[a-zA-Z_][a-zA-Z_0-9`~!@#$%^&*\-=+\/?<>]*:', Keyword),
            (r'[a-z][a-zA-Z_0-9`~!@#$%^&*\-=+\/?<>]*', Name),
            (r'[A-Z][a-zA-Z_0-9`~!@#$%^&*\-=+\/?<>]*', Name.Variable.Global),
            (r'_[a-zA-Z_0-9`~!@#$%^&*\-=+\/?<>]*', Name.Variable.Instance),
            (r'[`~!@#$%^&*\-=+\/?<>]+', Operator.Word),
            
            # argument lists
            (r'\|', Keyword.Declaration, 'arglist'),
            
        ],
        'string': [
            (r'"', String, '#pop'),
            (r'\\([\\abfnrtv"\']|x[a-fA-F0-9]{2,4}|[0-7]{1,3})', String.Escape),
            (r'[^\\"\n]+', String), # all other characters
            (r'\\\n', String), # line continuation
            (r'\\', String), # stray backslash
        ],
        'arglist': [
            (r'\|', Keyword.Declaration, '#pop'),
            (r'[a-z][a-zA-Z_0-9`~!@#$%^&*\-=+\/?<>]*', Name.Variable), # variable name
            (r'\s+', Whitespace), # everything else
        ],
    }
    
    # bob: hack. i know i want it to guess finch
    def analyse_text(text):
        print 'in analyze text'
        return 1.0