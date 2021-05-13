Sanguine Project
Author: Vlad Munteanu

Instructions:
	- Setup: Execute the generate-proj.bat to generate the project files. Afterwards, open the solution and build all the projects.
		 Lastly, either set up as the startup project either the SanguineEditor, for the editor application, or the GameClient for the networked client application.

	- Fist time usage: when the application starts, it will have all it's modal windows undocked. Snap the windows into whichever anchors you wish, for optimal usage.

	- To rotate and move the camere you have to hold down the right mouse button

Modules:
	- Sanguine: the Engine dll library
	- SanguineEditor: the editor application for demostrating the Sanguine capabilities
	- GameServer: a TCP dedicated server for a Sanguine client
	- GameClient: the Sanguine client which should connect to the DGS

Implemented capabilities:
	- Integrated premake5 as a project build system
	- Integrated OpenGL as the main rendering API
	- Integrated GLad as the OpenGL loader
	- Integrated GLFW for managing windows, input signals, and OpenGL contexts
	- Integrated GLM for necessary geometry calculations
	- Integrated STB_Image for loading textures
	- Integrated ASSIMP for loading 3D objects
	- Integrated ASIO for the networking middleware (Client/Server interfaces)
	- Integrated ImGui integrated for displaying menus and contextual controls
	- Integrated ImGuizmo for guizmo widgets attached to the scene objects for TRS manipulations
	- Integrated YAML for supporting importing and exporting scenes in a human readable format
	- Integrated EnTT for supporting a highly optimized ECS framework, for storing and retriveing methods for templated components
	- It was built on the Engine DLL model that exposes and loads in a Client / Application project;
	- A hardware abstraction layer and a complete middleware such that it will allow support for multiple rendering APIs/platforms
	- Transform3D class for transformation calculations
	- Abstracts as well as the classes with the actual implementations for the rendering primitives:  Shader, Texture, Material, Mesh
	- Perspective and orthographic camera
	- Mouse and keyboard input polling events
	- Layer/Surface Stack for specific input event handling (Event Dispatcher combined with a Layer Stack system)
	- Implements a forward renderer system
        - Support for mesh texturing
	- Support for Phong lighting / material system
	- Scene object hierarchy for adding/deleting game objects in the current scene
	- Save/load scene files to/from disk
	- Implemented an Entity Component System with the following available components:
		- Tag (for naming objects)
		- Transform component
		- Mesh component
		- Native script component (cpp files for dynamic runtime logic of objects)
		- Scene component (if you want to configure multiple cameras in the scene)
		- Collider component (Sphere, AABB)
	- Logging support
	- Profilling support
	- Physics engine:
		- Collisions and their resolution between AABB, Spheres, Planes, Meshes (Triangles)
		- Gravity and friction simulation

	- ** GameClient exclusive feature: chat room 
