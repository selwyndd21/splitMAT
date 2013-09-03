#!/usr/bin/python

import numpy as np # include NumPy
import sys         # include system variables
import getopt      # use system options
# from StringIO import StringIO

########################################
# set input output file
########################################
inputfile = ''
outputfile = ''
try:
   opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["ifile=","ofile="])
except getopt.GetoptError:
   print 'test.py -i <inputfile> -o <outputfile>'
   sys.exit(2)
for opt, arg in opts:
   if opt == '-h':
      print 'test.py -i <inputfile> -o <outputfile>'
      sys.exit()
   elif opt in ("-i", "--ifile"):
      inputfile = arg
   elif opt in ("-o", "--ofile"):
      outputfile = arg
print '    Input file:', inputfile, ';    Output file:', outputfile

########################################
# main program
########################################

# read data from: inputfile
E,x,r=np.genfromtxt(inputfile,
      dtype=float,#delimiter=None
      skip_header=0,skip_footer=0,autostrip=True,
#     missing_values="total",filling_values=-1,
      unpack=True,
      )
# for debug
print "    read data ZA-lib and atom-density"

#calculate total flux
g=0
fluxT = 0
while E[g] > 0 :
  fluxT = fluxT + x[g]
  g = g+1

print "      Total flux=", fluxT
if abs(fluxT - x[g])/x[g] > 1E-4 :
  print "      Total flux should be ", x[g]
  print "    Exit with Error !!"
  quit()
else :
  print "      Total flux verified, difference < ", abs(fluxT - x[g])/x[g] 

# calculate flux/lethargy
#print E
fluxT = fluxT - x[g-1] # normalized total flux
y = [] #* x.itemie
g=0
while E[g] > 1.1E-10 :
  u = np.log(  E[g] / E[g+1] )
  y.append( x[g] / u /fluxT )
  #print "%1.8e %1.5e %1.5e -> %1.5e" % (E[g], x[g] ,u, y[g])
  g = g+1

# delete total flux and the lowest energy
#y = np.delete(y, g)
E = np.delete(E, [g,g+1])
x = np.delete(x, [g,g+1])
#print E

# save data
np.savetxt(outputfile, np.column_stack( (E,x,y) ), fmt=('%11.8E %14.5E %14.5E') )
print "    end pseudo-material interpolation"


