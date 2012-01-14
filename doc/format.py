#!/usr/bin/python

import glob
import markdown
import os
from datetime import datetime

from finch import FinchLexer

def htmlpath(pattern):
    return 'html/' + pattern + '.html'

def formatfile(path):
    basename = os.path.basename(path)
    basename = basename.split('.')[0]

    # see if the html is up to date
    """
    sourcemod = os.path.getmtime(path)
    destmod = 0
    if os.path.exists(htmlpath(basename)):
        destmod = max(destmod, os.path.getmtime(htmlpath(basename)))

    if sourcemod < destmod:
        print basename, "is up to date"
        return
    """

    title = ''

    # read the markdown file and preprocess it
    contents = ''
    with open(path, 'r') as input:
        # read each line, preprocessing the special codes
        for line in input:
            stripped = line.lstrip()
            indentation = line[:len(line) - len(stripped)]

            if stripped.startswith('^'):
                command,_,args = stripped.rstrip('\n').lstrip('^').partition(' ')
                args = args.strip()

                if command == 'title':
                    title = args
                else:
                    print "UNKNOWN COMMAND:", command, args

            else:
                contents = contents + line

    html = markdown.markdown(contents, ['def_list', 'codehilite'])

    modified = datetime.fromtimestamp(os.path.getmtime(path))
    mod_str = modified.strftime('%B %d, %Y')

    fields = {'title': title, 'html': html, 'mod': mod_str}

    # write the html output
    with open(htmlpath(basename), 'w') as out:
        out.write(
            '''
            <!DOCTYPE html>
            <html>
            <head>
            <meta http-equiv="Content-type" content="text/html;charset=UTF-8" />
            <title>The Finch Programming Language: {title}</title>
            <link rel="stylesheet" type="text/css" href="styles.css" />
            </head>
            <body id="top">
            <div class="content">
            <div class="header">
            <h1><a href="index.html">Finch:</a> {title}</h1>
            <p class="nav">
                <a href="index.html">Welcome</a> |
                <a href="expressions.html">Expressions</a> |
                <a href="objects.html">Objects</a>
            </p>
            </div>
            {html}
            <p class="footer">Last modified on {mod} | email = robert, domain = stuffwithstuff.com</p>
            </div>
            </body>
            </html>
            '''.format(**fields))

    print "converted", basename


# process each markdown file
for f in glob.iglob('markdown/*.markdown'):
    formatfile(f)