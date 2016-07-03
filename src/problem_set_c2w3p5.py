#!/usr/bin/python2.7

W = [ 0.05, 0.4, 0.08, 0.04, 0.1, 0.1, 0.23 ]

def optimal_bst_time( Input ):
   if len( Input ) <= 1:
      return sum( Input )

   optslice = optimal_bst_time( Input[1:len(Input)] )
   for i in range(1,len(Input)):
      optslice = min( optslice, optimal_bst_time( Input[0:i] ) + optimal_bst_time( Input[i+1:len(Input)] ) )

   return optslice + sum( Input )

print optimal_bst_time( W )
