# Stuffs made during the courses "Algorithms: Design and Analysis, Part 1 and 2"

##Introduction

I really liked taking the Coursera courses "Algorithms: Design and Analysis, Part 1" and "Part 2". These old-fashioned algorithm courses are the simplified and online versions of real undergraduate courses, so nothing is really surprising here. Just data structures, like heap and binary search tree, and algorithms like the quick sort and Dijkstra's shortest path. These are just the basics, not the advanced stuffs the whole world is buzzing around in these days ( like GPU and FPGA algorithms ).

But I enjoy keeping the basics fresh and taking these courses again and again. And in the current season, I decided implementig the weekly assignments in C++. My goal was finding out how hard is coding a fast solution for these problems, using only a couple of seconds to solve the given problems on the given inputs. Hence instead of focusing on the clear and elegant formulation of algorithms, I focused on using easy and intermediate optimization techniques in C++. And I should tell it was not very hard to come up with a fast solution for each of these problems. The conclusion I drew was that with any new PC, reading 100 Mbytes from a text file and then extracting the strongly connected components of that graph is not a very hard task to the "iron". Even the C++ compiler was very helpful to me, e.g. it was powerful enough to generate such a neat code from the simple recursive DFS implementation that I could not outperform with a tricky and non-recursive implementation. (And if you read discussion forums and you see people who are struggling with python, because the recursive DFS implementation is running out of stack in python, then suddenly you start to feel that there is still some place for the C++ community.)

##Details

The repo has one console app for each assignment of the courses. It also has the target inputs of the assignment in a zipped form. (These are almost the input files can be downloaded from the coursera page, but they take 160 Mybte in total. To optimize disk space usage and zipping, I runned 'sort' on some files in order to decrease entropy. I added the number of vertices and edges to some other inputs, just to reuse existing file reader functions from previous assignments.) The inputs can be unzipped with the command *make inputs*. Once they are available, the apps can be runned on the inputs with *run_all.sh*, so all problems are solved just in real time. About the running time: using SSD and an i3 processor, the total running time of *run_all.sh* should be 15-20 seconds.

##How to run

Clone the repo. Then:
* make inputs
* make app
* make solve

##Extras
I created some extras related to the course stuffs, will document them later.
