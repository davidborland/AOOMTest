Process for creating surfaces:

Export OBJ and VRML versions from PyMOL.
Run CleanOBJ on OBJ file.
Remove .mtl file, and references to it in OBJ file.
Run AOGen on cleaned OBJ file to generate ambient occlusion data.
Run OBJPerVertexColor on cleaned OBJ and VRML file to generate per vertex color data.
Create a .info file, with the surface information
Load .info file into AOOMTest