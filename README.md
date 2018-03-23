# DynamicDFS

This project is a part of the Advanced Problem Solving course that I had taken in my 1st semester of the M.Tech program at IIITH. The project implements the algorithm given by Surender Baswana and Shahbaz Khan in their paper which can be found at https://www.cse.iitk.ac.in/users/sbaswana/Papers-published/Incr-DFS.pdf.

The purpose of the algorithm is to maintain a DFS tree for an undirected graph under insertion of edges. For processing any arbitrary online sequence of edge insertions, this algorithm takes total O(n^2) time. The main goal of this algorithm is to maintain the dfs tree such that the time taken on any edge update is smaller than the time taken by the best static algorithm.
