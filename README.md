This code searches for a valid schedule given a event dataset using Bratley's algorithm.

Input data is parsed from a json file, using a defined parse function to store the data in a map (ID,value) of hold input data. When a node is created on the tree it gets the input data from the map by passing an ID and assigning the values.

The code prints all the steps taken as it recursively iterates through a tree including the path from root of the parent, path from root with child insertes, deadline time, elapsed time, and whether that node meets timing constraints.

If a generated node does not meet timing the recursive line is broken, and the parent node is cut off and freed to prune the tree. If the node t does meet timing it recursively generates its children nodes, and checks their timing (The task of deciding which children are created is calculated by iterating up the tree and finding which nodes have not been created on the line). 

The creation of the tree is cut off when a valid schedule is found. THis discrepency is shown in my example cases in which input_large_shortest_path, runs much faster than input_large_longest_path as it finds a schedule on the shortest path along the leftmost branch of the tree.

The final line indicates whether a valid schedule was generated or not by probing the return value of the recursive tree generation function.