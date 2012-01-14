#!/usr/bin/python

# Runs the benchmark scripts and reports the times. Each benchmark is invoked
# five times and the result is the median of their times. Time is calculated
# using the Unix 'time' program using the sum of the user and system time for
# the script.

import re
import subprocess
from datetime import date

TIME = re.compile('(\d+\.\d+) user.*(\d+\.\d+) sys')

def timeScript(name):
    process = subprocess.Popen(
        ['time', '../build/Release/finch', name + '.fin'],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = process.communicate()

    if not 'true' in output:
        print "Benchmark {0} failed.".format(name)
        print output
        return None

    m = TIME.search(err)
    if m is None:
        print "Couldn't parse output for {0}.".format(name)
        return None

    userTime = float(m.group(1))
    sysTime = float(m.group(2))

    return userTime + sysTime


def medianTime(name):
    times = []
    for i in range(0, 5):
        time = timeScript(name)
        if time is None:
            # Some error occurred.
            return
        times.append(time)

    times.sort()
    # Get median.
    return times[len(times) / 2]


lexerTime = medianTime('lexer')
fibTime = medianTime('fib')
print 'date          lexer     fib'
print '{0}  {1:6}s {2:6}s'.format(date.today(), lexerTime, fibTime)
