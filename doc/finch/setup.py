"""
A Pygments lexer for Finch.
"""
from setuptools import setup

__author__ = 'Robert Nystrom'

setup(
    name='Finch',
    version='1.0',
    description=__doc__,
    author=__author__,
    packages=['finch'],
    entry_points='''
    [pygments.lexers]
    finchlexer = finch:FinchLexer
    '''
)