/*
Copyright (c) 2018, Michael Kazhdan and Fabian Prada
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution.

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/


#ifndef SIMPLE_MESH_INCLUDED
#define SIMPLE_MESH_INCLUDED

#include <Misha/Ply.h>


class TexturedMesh{
public:
	std::vector<Point3D<double>> vertices;
	std::vector<Point3D<double>> normals;
	std::vector<TriangleIndex> triangles;
	std::vector<Point2D<double>> textureCoordinates;
};

void WriteTexturedMesh(TexturedMesh & mesh, const char * fileName, const char * atlasName = NULL)
{
	std::vector< PlyTexturedFace< float > > texturedTriangles;
	texturedTriangles.resize(mesh.triangles.size());
	for (int i = 0; i < mesh.triangles.size(); i++){
		texturedTriangles[i].resize(3);
		for (int j = 0; j < 3; j++){
			texturedTriangles[i][j] = mesh.triangles[i][j];
			texturedTriangles[i].texture(j) = Point2D<float>(mesh.textureCoordinates[3 * i + j][0], mesh.textureCoordinates[3 * i + j][1]);
		}
	}

	std::vector< PlyVertex< float > > vertices(mesh.vertices.size());
	for (int i = 0; i < mesh.vertices.size(); i++)vertices[i].point = Point3D<float>(mesh.vertices[i][0], mesh.vertices[i][1], mesh.vertices[i][2]);

	if (atlasName != NULL){
		char ** comments = new char *[1];
		char atlas_comment[256];
		sprintf(atlas_comment, "TextureFile %s", atlasName);
		comments[0] = atlas_comment;
		PlyWritePolygons(fileName, vertices, texturedTriangles, PlyVertex< float >::WriteProperties, PlyVertex< float >::WriteComponents, PlyTexturedFace< float >::WriteProperties, PlyTexturedFace< float >::WriteComponents, PLY_ASCII, comments, 1);
	}
	else{
		PlyWritePolygons(fileName, vertices, texturedTriangles, PlyVertex< float >::WriteProperties, PlyVertex< float >::WriteComponents, PlyTexturedFace< float >::WriteProperties, PlyTexturedFace< float >::WriteComponents, PLY_ASCII);
	}
}

#endif//SIMPLE_MESH_INCLUDED
