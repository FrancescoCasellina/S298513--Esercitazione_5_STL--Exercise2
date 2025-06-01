#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <cctype>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
	PolygonalMesh mesh;
	if(!ImportMesh(mesh)){
		cerr << "file not found" << endl;
		return 1;
	}

        //------------debug------------
	cout << "DEBUG MODE -- View stored values? (Y/N)" << endl;
	char sel;
	cin >> sel;
	sel = toupper(sel);
	if (sel != 'Y' && sel != 'N') {
		cerr << "Invalid selection - Select either Y or N." << endl;
		return 2;
	}
	if (sel == 'Y') {
		//---Cell0Ds---
		cout << "-------------Cell0Ds-------------" << endl;
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
		//---Cell1Ds---
                cout << "-------------Cell1Ds-------------" << endl;
		for (unsigned int i = 0; i < mesh.NumCell1Ds; i++) {
			unsigned int id = mesh.Cell1DsId[i];
			int origin = mesh.Cell1DsExtremes(0, id);
			int end = mesh.Cell1DsExtremes(1, id);
			if (origin >= mesh.Cell0DsCoordinates.cols() || end >= mesh.Cell0DsCoordinates.cols()) {
				cerr << "ERROR: Vertex ID out of bounds in Cell1D edge ID " << id << endl;
				return 3;
			}
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
		//---Cell2Ds---
                cout << "-------------Cell2Ds-------------" << endl;
		for (unsigned int i = 0; i < mesh.NumCell2Ds; i++) {
			unsigned int id = mesh.Cell2DsId[i];
			cout << "Cell2D Id : " << id << " | Vertices: ";
			for (unsigned int v : mesh.Cell2DsVertices[i]) {
				cout << v << " ";
			}
			for (unsigned int v : mesh.Cell2DsVertices[i]) {
				if (v >= mesh.Cell0DsCoordinates.cols()) {
					cerr << "ERROR: Vertex ID out of bounds in Cell2D ID " << id << endl;
					return 4;
				}
			}
			cout << "| Edges: ";
			for (unsigned int e : mesh.Cell2DsEdges[i]) {
				cout << e << " ";
			}
			cout << "| Marker: ";
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
	}

	bool EdgCheck = true;
	for (unsigned int i = 0; i < mesh.NumCell1Ds; i++) {
		unsigned int id = mesh.Cell1DsId[i];
		int origin = mesh.Cell1DsExtremes(0, id);
		int end = mesh.Cell1DsExtremes(1, id);

		double X0 = mesh.Cell0DsCoordinates(0, origin);
		double Y0 = mesh.Cell0DsCoordinates(1, origin);
		double X1 = mesh.Cell0DsCoordinates(0, end);
		double Y1 = mesh.Cell0DsCoordinates(1, end);

		double length = std::hypot(X1 - X0, Y1 - Y0);
		if (length <= 0.0) {
			cerr << "ERROR! Found zero length edge -- Id: " << id << endl;
		        EdgCheck = false;
		}
	}
	if (EdgCheck)
		cout << "No zero length edges found..." << endl;

	bool PolCheck = true;
	for (unsigned int i = 0; i < mesh.NumCell2Ds; i++) {
		const auto& vertices = mesh.Cell2DsVertices[i];
		if (vertices.size() < 3) {
	        	cerr << "ERROR! Found broken polygon -- Id: " << mesh.Cell2DsId[i] << endl;
	        	PolCheck = false;
	        	continue;
		}

		double area = 0.0;
		for (size_t j = 0; j < vertices.size(); j++) {
	        	size_t k = (j + 1) % vertices.size();
	        	double xj = mesh.Cell0DsCoordinates(0, vertices[j]);
	        	double yj = mesh.Cell0DsCoordinates(1, vertices[j]);
	        	double xk = mesh.Cell0DsCoordinates(0, vertices[k]);
	        	double yk = mesh.Cell0DsCoordinates(1, vertices[k]);
	        	area += (xj * yk - xk * yj);
		}
		area = std::abs(area) * 0.5;

		if (area <= 0.0) {
			cerr << "ERROR! Found zero area polygon -- Id: " << mesh.Cell2DsId[i] << endl;
			PolCheck = false;
	    	}
	}
	if (PolCheck)
		cout << "No zero area polygons found..." << endl;

	Gedim::UCDUtilities utils;

	Eigen::MatrixXd coords3d(3, mesh.Cell0DsCoordinates.cols());
	coords3d.row(0) = mesh.Cell0DsCoordinates.row(0);  // X
	coords3d.row(1) = mesh.Cell0DsCoordinates.row(1);  // Y
	coords3d.row(2).setZero();                         // Z = 0

	utils.ExportPoints("./Cell0Ds.inp", coords3d);
	utils.ExportSegments("./Cell1Ds.inp", coords3d, mesh.Cell1DsExtremes);

	return 0;
}
