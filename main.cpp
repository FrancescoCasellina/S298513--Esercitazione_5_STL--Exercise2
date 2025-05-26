#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
	PolygonalMesh mesh;
/*	if(!ImportMesh(mesh)){
		cerr << "file not found" << endl;
		return 1;
	}
*/

	//the following is only for debug purposes
	if(!ImportCell0Ds(mesh)) {
		cerr << "Something went wrong..." << endl;
        	return 1;
	}
	cout << "All good..." << endl;
	for(size_t i = 0; i < mesh.Cell0DsId.size(); i++) {
		unsigned int id = mesh.Cell0DsId[i];
		double x = mesh.Cell0DsCoordinates(0, id);
		double y = mesh.Cell0DsCoordinates(1, id);
                cout << "Id: " << id << " | Coordinates: (" << x << ", " << y << ") | Marker: ";
		bool foundMarker = false;
		for (const auto& [marker, cellIds] : mesh.MarkerCell0Ds) {
     			if (std::find(cellIds.begin(), cellIds.end(), id) != cellIds.end()) {
            			cout << marker << " ";
            			foundMarker = true;
        		}
    		}
		if (!foundMarker) {
			cout << "none";
		}
		cout << endl;
	}


	//the following is only for debug purposes
	if(!ImportCell1Ds(mesh)) {
		cerr << "Something went wrong..." << endl;
		return 1;
	}
	cout << "All good..." << endl;
	for (unsigned int i = 0; i < mesh.NumCell1Ds; i++) {
		unsigned int id = mesh.Cell1DsId[i];
		int origin = mesh.Cell1DsExtremes(0, id);
		int end = mesh.Cell1DsExtremes(1, id);
		cout << "Cell1D Id: " << id
			<< " | Origin Vertex Id: " << origin
			<< " | End Vertex Id: " << end
			<< " | Marker: ";
                bool foundMarker = false;
                for (const auto& [marker, cellIds] : mesh.MarkerCell1Ds) {
                        if (std::find(cellIds.begin(), cellIds.end(), id) != cellIds.end()) {
                                cout << marker << " ";
                                foundMarker = true;
                        }
                }
                if (!foundMarker) {
                        cout << "none";
                }
                cout << endl;
	}


	//the following is only for debug purposes
	if(!ImportCell2Ds(mesh)) {
		cerr << "Something went wrong..." << endl;
		return 1;
	}
	cout << "All good..." << endl;
	for (unsigned int i = 0; i < mesh.NumCell2Ds; i++) {
		unsigned int id = mesh.Cell2DsId[i];
		cout << "Cell2D Id : " << id << " | Vertices: ";
		for (unsigned int v : mesh.Cell2DsVertices[i]) {
			cout << v << " ";
		}
		cout << "| Edges: ";
		for (unsigned int e : mesh.Cell2DsEdges[i]) {
			cout << e << " ";
		}
		cout << " | Marker: ";
                bool foundMarker = false;
                for (const auto& [marker, cellIds] : mesh.MarkerCell2Ds) {
                        if (std::find(cellIds.begin(), cellIds.end(), id) != cellIds.end()) {
                                cout << marker << " ";
                                foundMarker = true;
                        }
                }
                if (!foundMarker) {
                        cout << "none";
                }
                cout << endl;
	}

	return 0;
}
