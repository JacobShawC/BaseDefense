// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream> 
#include <list> 
/**
 * 
 */
class BASEDEFENSE_API SearchGraph
{
		int V;    // No. of vertices 
		TArray<int> adj;    // Pointer to an array containing adjacency lists 
		void DFSUtil(int v, bool visited[]);  // A function used by DFS 
	public:
		SearchGraph(int V);   // Constructor 
		void addEdge(int v, int w);   // function to add an edge to graph 
		void DFS();    // prints DFS traversal of the complete graph 
};
