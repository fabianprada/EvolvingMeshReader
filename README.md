# EvolvingMeshReader

This application reads an evolving mesh file and exports the textured mesh (in ply format) for the specified frame.

Compile in Windows from the .sln. 
For Linux and Mac use the Makefile provided within /EvolvingMeshReader.

To run the application, set your current directory to the folder containing the evolving mesh data (i.e., evolvingMesh.bin and the .png files) and exceute:

EvolvingMeshReader --frame 0

This outputs a file Mesh-F00000.ply that you can open with a standard mesh viewer (e.g., Meshlab).


 