#pragma once

#include "DisplayManager.hpp"
#include "GUIManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "GroundData.hpp"
#include "OBJData.hpp"
#include "Filemanager.hpp"

int main()
{
	//Display-Management
	DisplayManager displayManager;
	displayManager.createDisplay();
	displayManager.printVersion();

	//GUI-Management
	GUIManager guiManager;
	guiManager.initializeGUI(displayManager.getWindow());	

	//Renderer
	Renderer renderer;

	//Shaderstuff
	Shader standard_shader("res/shader/vertexShader.glsl", "res/shader/fragmentShader.glsl");

	//Data
	//RawCubeData cube_data;
	GroundData ground_data(128);
	OBJData cube_data("res/obj/cube.obj");	
	//OBJData wall_data("res/obj/wall.obj");

	//Modelcreation
	std::vector<Model*> Models;

	Model cube(&cube_data, &standard_shader, "res/textures/Brick.jpg");
	Models.push_back(&cube);

	/*Model wall(&wall_data, &standard_shader,"res/textures/Brick.jpg");
	Models.push_back(&wall);*/

	Model ground(&ground_data, &standard_shader, "res/textures/Grass_1.jpg");
	ground.translate(glm::vec3(0.0f, -5.0f, 0.0f));
	Models.push_back(&ground);

	//DEBUG
	/*Filemanager filemanager;
	filemanager.writeReadableToFile(wall_data._vertices, "res/data/wallvertices.data");
	filemanager.writeReadableToFile(wall_data._texCoords, "res/data/walltexcoords.data");
	filemanager.writeReadableToFile(wall_data._indices, "res/data/wallindices.data");*/
	/*filemanager.writeReadableToFile(test_data._vertices, "res/data/testvertices.data");
	filemanager.writeReadableToFile(test_data._texCoords, "res/data/testtexcoords.data");
	filemanager.writeReadableToFile(test_data._indices, "res/data/testindices.data");*/

	while (!displayManager.WindowShouldClose())
	{
		//Measure Frametime
		displayManager.measureFrameTime();

		//Clear Buffer and prepare for rendering
		renderer.prepare();

		//Start GUI-Frame
		guiManager.startFrame();

		//Check Keyboard and Mouseinputs
		displayManager.checkForInput();
		
		//Render Stuff		
		renderer.render(Models);

		//GUI Stuff
		{
			//Neues Fenster mit FPS Counter rendern
			guiManager.newWindow("General settings");
			guiManager.printFPS();
			guiManager.exitWindow();
		}

		//Update stuff
		{
			guiManager.renderGUI();
			displayManager.updateDisplay();
		}
	}
	//CleanUP Stuff
	{
		guiManager.cleanUPGUI();
		displayManager.closeDisplay();
	}
	
	return 0;
}