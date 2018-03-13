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


#include <Misha/CmdLineParser.h> 
#include "EvolvingMesh.h"

cmdLineParameter<  int > Frame("frame");
cmdLineReadable* params[] =
{
	&Frame,
	NULL
};

void ShowUsage(const char* ex)
{
	printf("Usage %s:\n", ex);
	printf("\t --%s <frame id>\n", Frame.name);
}


int main(int argc, char* argv[])
{
	cmdLineParse(argc - 1, argv + 1, params);
	if (!Frame.set) { ShowUsage(argv[0]); return EXIT_FAILURE; }

	EvolvingMesh eMesh;
	ReadEvolvingMesh("evolvingMesh.bin", eMesh);

	TexturedMesh tMesh;
	GetFrameTexturedMesh(eMesh, Frame.value, tMesh);

	char meshName[256];
	sprintf(meshName, "Mesh-F%05d.ply", Frame.value);
	char atlasName[256];
	sprintf(atlasName, "Atlas-F%05d.png", Frame.value);
	WriteTexturedMesh(tMesh, meshName, atlasName);

	//Read textures
	return 0;
}
