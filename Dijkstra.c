// Dijkstra API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

// This is a helper function that creates a new PredNode.
PredNode *createPredNode(Vertex v) {
	PredNode *newPredNode = malloc(sizeof(*newPredNode));
	if (newPredNode == NULL) {
        fprintf(stderr, "Not enough memory to make a new node\n");
		exit(1);
	}
	newPredNode->v = v;
	newPredNode->next = NULL;
	return newPredNode;
}

// This is a helper function that takes in a single node each time and frees
// the entire list.
void freePredList(PredNode *pred) {
	PredNode *current;
	while (pred != NULL) {
		current = pred;
		pred = pred->next;
		free(current);
	} 
}

////////////////////////////////////////////////////////////////////////////////

// Finds the shortest paths from a source node to all nodes in a graph and
// returns an array with the distance and predecessor list in ascending order.
NodeData *dijkstra(Graph g, Vertex src) {
	// GraphNumVertices(g) is the number of vertices in this graph.
	// Create a new NodeData struct called closest to find the closest path.
	NodeData *closest = malloc(GraphNumVertices(g) *sizeof(struct NodeData));
	if (closest == NULL) {
        fprintf(stderr, "Not enough memory to make a new path\n");
		exit(1);
	}
	// Set the initial distance and pred list.
	closest->dist = 0;
	closest->pred = NULL;

	// All initial distances from the source node to other nodes begin as INFINITY as
	// these will be compared and changed as a new closer distance is found. If it does
	// not have a parent node then then the pred nodes of nodes is NULL. 
	int i = 0;
	PQ node = PQNew();
	while (i < GraphNumVertices(g)) {
		if (i != src) {
			closest[i].dist = INFINITY;
			closest[i].pred = NULL;
		} else {
			closest[i].dist = 0;
			closest[i].pred = NULL;
		}
		PQInsert(node, i, closest[i].dist); // Add nodes to PQ with priority based on distance from source node.
		i++;
	}

	while (!PQIsEmpty(node)) {
		Vertex findNewV = PQDequeue(node);
		AdjList link = GraphOutIncident(g, findNewV);		
		// Loop through edges and compare the current distance with the next distance.
		while (link != NULL) {
			if (closest[findNewV].dist != INFINITY) {
				 if (closest[link->v].dist == (link->weight + closest[findNewV].dist)) {
					// Connect a new path to the predecessor adjacency list head.					
					PredNode *head = createPredNode(findNewV);
					PredNode *current = closest[link->v].pred;    
					if (current->v < (head->v)) {						
						while (current->next != NULL && (current->next->v) < (head->v)) {
							current = current->next;
						}
						head->next = current->next;
						current->next = head;
			        } else {			            
			            head->next = closest[link->v].pred;
						closest[link->v].pred = head;
				    } // Compare the values and add it to the list if it is closest.
				} else if (closest[link->v].dist > (link->weight + closest[findNewV].dist)) {
				    if (closest[link->v].pred != NULL) {
				        freePredList(closest[link->v].pred);
				    }
					PredNode *head = createPredNode(findNewV);
					closest[link->v].dist = link->weight + closest[findNewV].dist;
					closest[link->v].pred = head;
					PQUpdate(node, link->v, closest[link->v].dist); 
			    }
				
			}	
			link = link->next;			
		}
	}
	PQFree(node);
	return closest;
}

// Frees all memory associated with the given NodeData array.
void freeNodeData(NodeData *data, int nV) {   
    int i = 0;
    while (i < nV) {
        freePredList(data[i].pred);
        i++;
    }
    free(data);
}





