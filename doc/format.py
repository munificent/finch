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

    modified = datetime.fromtimestamp(os.path.getmtime(path))
    mod_str = modified.strftime('%B %d, %Y')

    # write the html output
    with open(htmlpath(basename), 'w') as out:
        out.write('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">\n')
        out.write('<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">\n')
        out.write('<head>\n')
        out.write('<meta http-equiv="Content-type" content="text/html;charset=UTF-8" />\n')

        out.write('<title>The Finch Programming Language: ' + title + '</title>\n')
        out.write('<link rel="stylesheet" type="text/css" href="http://fonts.googleapis.com/css?family=Inconsolata|Reenie+Beanie">\n')
        out.write('<link rel="stylesheet" type="text/css" href="styles.css" />\n')
        out.write('<script src="jquery-1.3.2.min.js" type="text/javascript"></script>\n')
        out.write('</head>\n')
        out.write('<body id="top">\n')

        out.write('<div class="content">\n')

        # title
        out.write('<h1><a href="index.html">Finch:</a> {0}</h1>\n'.format(title))

        # content
        html = markdown.markdown(contents, ['def_list', 'codehilite'])
        out.write(html)

        out.write('<p class="footer">Last modified on {0} | email = robert, domain = stuffwithstuff.com</p>\n'.format(mod_str));

        out.write('</div>\n')
        out.write('</body>\n')
        out.write('</html>')

    print "converted", basename


# process each markdown file
for f in glob.iglob('markdown/*.markdown'):
    formatfile(f)