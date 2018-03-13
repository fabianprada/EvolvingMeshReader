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


#pragma once
#include "TexturedMesh.h"

class Chart {
public:
	std::vector<TriangleIndex> triangles;
	std::vector<Point2D<double>> textureCoordinates;
	std::vector<Point3D<double>> referenceEmbedding;
	int frameStart;
	int frameEnd;
};

class ChartEmbedding {
public:
	std::vector<Point3D<double>> vertices;
};

class Frame{
public:
	std::vector<int> chartIds;
	std::vector<ChartEmbedding> chartEmbeddings;
};

class EvolvingMesh {
public:
	std::vector<Chart> charts;
	std::vector<Frame> frames;
};

//Utilities

#include <set>
class IndexedVertex3D {
public:
	IndexedVertex3D(Point3D<double> p_p, int p_index) {
		p = p_p;
		index = p_index;
	}
	Point3D<double> p;
	int index;
};

class IndexedVertex3DComparison {
public:
	bool operator()(const IndexedVertex3D & p1, const IndexedVertex3D & p2) const {
		for (int i = 0; i < 3; i++) {
			if (p1.p[i] < p2.p[i])
				return true;
			else if (p2.p[i] < p1.p[i])
				return false;
		}
		return false;
	}
};

void GetFrameTexturedMesh(const EvolvingMesh & eMesh, const int frameId, TexturedMesh & mesh) {
	const Frame & frame = eMesh.frames[frameId];
	std::set<IndexedVertex3D, IndexedVertex3DComparison> indexedVertices;

	std::vector<Point3D<double>> & vertices = mesh.vertices;
	std::vector<TriangleIndex> & triangles = mesh.triangles;
	std::vector<Point2D<double>> & textureCoordinates = mesh.textureCoordinates;

	int lastAddedVertex = 0;
	for (int i = 0; i < frame.chartIds.size(); i++) {
		int chartId = frame.chartIds[i];
		const Chart & chart = eMesh.charts[chartId];
		const std::vector<Point3D<double>> & chartEmbedding = frame.chartEmbeddings[i].vertices;
		for (int t = 0; t < chart.triangles.size(); t++) {
			int vertexIndices[3] = { -1,-1,-1 };
			for (int k = 0; k < 3; k++) {
				Point3D<double> vertexPos = chartEmbedding[chart.triangles[t][k]];
				IndexedVertex3D indexedVertex(vertexPos, lastAddedVertex);
				auto findVertexIndex = indexedVertices.find(indexedVertex);
				if (findVertexIndex == indexedVertices.end()) {
					indexedVertices.insert(indexedVertex);
					vertices.push_back(vertexPos);
					vertexIndices[k] = lastAddedVertex;
					lastAddedVertex++;
				}
				else {
					vertexIndices[k] = (*findVertexIndex).index;
				}
				textureCoordinates.push_back(chart.textureCoordinates[chart.triangles[t][k]]);
			}
			triangles.push_back(TriangleIndex(vertexIndices[0], vertexIndices[1], vertexIndices[2]));
		}
	}
}

int ReadEvolvingMesh(const char * meshName, EvolvingMesh & eMesh, bool readEmbedding = true) {
	FILE * file;
	file = fopen(meshName, "rb");

	//Read charts
	int numCharts;
	fread(&numCharts, sizeof(int), 1, file);
	eMesh.charts.resize(numCharts);
	for (int i = 0; i < numCharts; i++) {
		Chart & chart = eMesh.charts[i];
		fread(&chart.frameStart, sizeof(int), 1, file);
		fread(&chart.frameEnd, sizeof(int), 1, file);

		int numTriangles;
		fread(&numTriangles, sizeof(int), 1, file);
		chart.triangles.resize(numTriangles);
		fread(&chart.triangles[0], sizeof(TriangleIndex), numTriangles, file);

		int numTexCoords;
		fread(&numTexCoords, sizeof(int), 1, file);
		chart.textureCoordinates.resize(numTexCoords);
		chart.referenceEmbedding.resize(numTexCoords);
		fread(&chart.textureCoordinates[0], sizeof(Point2D<double>), numTexCoords, file);
		fread(&chart.referenceEmbedding[0], sizeof(Point3D<double>), numTexCoords, file);
	}

	//Read frames
	int numFrames;
	fread(&numFrames, sizeof(int), 1, file);
	eMesh.frames.resize(numFrames);
	for (int i = 0; i < numFrames; i++) {
		Frame & frame = eMesh.frames[i];
		int numEmbeddedCharts;
		fread(&numEmbeddedCharts, sizeof(int), 1, file);
		frame.chartIds.resize(numEmbeddedCharts);
		fread(&frame.chartIds[0], sizeof(int), numEmbeddedCharts, file);

		if (readEmbedding) {
			frame.chartEmbeddings.resize(numEmbeddedCharts);
			for (int j = 0; j < numEmbeddedCharts; j++) {
				ChartEmbedding & chartEmbedding = frame.chartEmbeddings[j];
				int numVertices = chartEmbedding.vertices.size();
				fread(&numVertices, sizeof(int), 1, file);
				chartEmbedding.vertices.resize(numVertices);
				fread(&chartEmbedding.vertices[0], sizeof(Point3D<double>), numVertices, file);
			}
		}
	}

	fclose(file);
	return 1;
}
