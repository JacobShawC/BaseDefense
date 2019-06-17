// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchGraph.h"

void SearchGraph::DFSUtil(int v, bool visited[])
{

}

SearchGraph::SearchGraph(int V)
{
	this->V = V;
	adj.SetNumZeroed(V);
}

void SearchGraph::addEdge(int v, int w)
{
	adj.Add(w); // Add w to v’s list. 
}

void SearchGraph::DFS()
{

}
