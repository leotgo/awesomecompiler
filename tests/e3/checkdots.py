#!/usr/bin/env python2
import pygraphviz as pgv
from sets import Set
import sys

def label (i):
   try:
      return i.attr['label']
   except AttributeError:
      return None

def same_label (n0, n1):
   return label(n0) == label(n1) and label(n0) != None

def terminate (code):
   sys.exit(code)

def root (graph):
   for i in graph.iternodes():
      if label(i) == "programa":
         return i

if len(sys.argv) < 3:
   print('usage: %s DOTFILE DOTFILE'%(sys.argv[0]))
   terminate (1)
ref, target = sys.argv[1:3]
if '-' == ref:
	ref = '/dev/stdin'
if '-' == target:
	target = '/dev/stdin'
ref = pgv.AGraph (str(ref))
target = pgv.AGraph (str(target))

#1. find the root node of ref and target
ref_root = root(ref)
target_root = root(target)

def compare_tree (r_tree, r_root, t_tree, t_root):
   if not same_label (r_root, t_root):
      return False

   if not len(r_tree.out_neighbors(r_root)) == len(t_tree.out_neighbors(t_root)):
      return False

   t_neigh = t_tree.out_neighbors(t_root)[:]
   for i in r_tree.out_neighbors(r_root):
      for j in range(len(t_neigh)):
         if compare_tree (r_tree, i, t_tree, t_neigh[j]) is True:
            t_neigh.pop(j)
            break
      else:
         return False

   return [] == t_neigh

if compare_tree (ref, ref_root, target, target_root) is False:
   terminate (1)
else:
   terminate (0)
