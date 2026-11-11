/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include <stdio.h>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <rlgl.h>
#include <math.h>
#include <stdlib.h>

int window_height = 640;
int window_width = 640;

int lois_x = 0;
int lois_y = 0;

typedef struct edge {
		//vertices
		int x_a, y_a;
		int x_b, y_b;
		int* weight;
		int id;
} edge_t;
	//list
	typedef struct node{
		edge_t *edge;
		struct node *next;
	} node_t;

node_t* return_all_neighbours(int x, int y){
	node_t* head = 0;
	node_t *current = 0;
	

	if (x != 0)
	{
		// (x-1,y)
		edge_t *edge = malloc(sizeof(edge_t));
		*edge = (edge_t){.x_a = x, .y_a = y, .x_b = x -1, .y_b = y,.weight = NULL, .id = 0};
		if(current) {
    		current->next = calloc(sizeof(node_t),1);
    		current = current->next;
		} else {
    		current = calloc(sizeof(node_t),1);
    		head = current;
		}
		current->edge = edge;
	}
	if(y!=0){
		// (x,y-1)
		edge_t *edge = malloc(sizeof(edge_t));
		*edge = (edge_t){.x_a = x, .y_a = y, .x_b = x, .y_b = y-1,.weight = NULL, .id = 0};
		if(current) {
    		current->next = calloc(sizeof(node_t),1);
    		current = current->next;
		} else {
    		current = calloc(sizeof(node_t),1);
    		head = current;
		}
		current->edge = edge;
	}
	if(y<=(window_height-32)/32){
		// (x,y+1)
		edge_t *edge = malloc(sizeof(edge_t));
		*edge = (edge_t){.x_a = x, .y_a = y, .x_b = x, .y_b = y+1,.weight = NULL, .id = 0};
		if(current) {
    		current->next = calloc(sizeof(node_t),1);
    		current = current->next;
		} else {
    		current = calloc(sizeof(node_t),1);
    		head = current;
		}
		current->edge = edge;
	}
	if(x<=(window_width-32)/32){
		// (x+1,y)
		edge_t *edge = malloc(sizeof(edge_t));
		*edge = (edge_t){.x_a = x, .y_a = y, .x_b = x + 1, .y_b = y,.weight = NULL, .id = 0};
		if(current) {
    		current->next = calloc(sizeof(node_t),1);
    		current = current->next;
		} else {
    		current = calloc(sizeof(node_t),1);
    		head = current;
		}
		current->edge = edge;
	}
	
	return head;
}

int main ()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	
	InitWindow(window_width,window_height, "grünenschmalz");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture lois = LoadTexture("lois.png");


	node_t* adjacency[window_width / 32][window_height / 32];
	for (int x = 0; x < (window_width / 32); x++)
	{
		for (int y = 0; y < (window_height / 32);y++){
			adjacency[x][y] = return_all_neighbours(x, y);
		}
	}

	//set weights, identify the edges that are double
	int i = 1;
	for (int x = 0; x < (window_width / 32); x++)
	{
		
		for (int y = 0; y < (window_height / 32);y++){
			node_t *current = adjacency[x][y];
			int *r = calloc(sizeof(int), 1);
			
			*r = rand() % 10;
			if (current->edge->weight == NULL)
			{
				int *x_a = &current->edge->x_a;
				int *x_b = &current->edge->x_b;
				int *y_a = &current->edge->y_a;
				int *y_b = &current->edge->y_b;
				node_t *traverse = adjacency[*x_b][*y_b];
				while(traverse){
					if(traverse->edge->weight == NULL){
						if(traverse->edge->x_a == *x_b && traverse->edge->y_a == *y_b && traverse->edge->x_a == *x_b && traverse->edge->y_b == *y_a){
							(traverse->edge->weight) = r;
							(current->edge->weight) = r;
							(current->edge->id) = i;
							(traverse->edge->id) = i;
							i = i + 1;
							break;
						}
					}
					traverse = traverse->next;
				}
			
			}
		}
	}	
	


		// game loop
		while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close button on the window
		{
			// update
			if (IsKeyPressed(KEY_RIGHT))
			{
				lois_x = trunc(fmin(lois_x + 32, window_width - 32));
			}
			else if (IsKeyPressed(KEY_LEFT))
			{
				lois_x = trunc(fmax(lois_x - 32, 0));
			}
			else if (IsKeyPressed(KEY_UP))
			{
				lois_y = trunc(fmax(lois_y - 32, 0));
			}
			else if (IsKeyPressed(KEY_DOWN))
			{
				lois_y = trunc(fmin(lois_y + 32, window_height - 32));
			}

			// drawing
			BeginDrawing();

			// Setup the back buffer for drawing (clear color and depth buffers)
			ClearBackground(WHITE);

			// draw some text using the default font
			// DrawText("Hello Louis", 200,200,20,WHITE);

			// draw our texture to the screen

			DrawTexture(lois, lois_x, lois_y, WHITE);

			for (int x = 0; x < 21; x++)
			{
				DrawLine(x * 32, 0, x * 32, 640, BLACK);
			}
			for (int y = 0; y < 21; y++)
			{
				DrawLine(0, y * 32, 640, y * 32, BLACK);
			}

			// end the frame and get ready for the next one  (display frame, poll input, etc...)
			EndDrawing();
		}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(lois);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

