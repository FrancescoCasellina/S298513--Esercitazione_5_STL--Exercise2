#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary {


bool ImportMesh(PolygonalMesh &mesh) {
	if(!ImportCell0Ds(mesh))
		return false;
	if(!ImportCell1Ds(mesh))
		return false;
/*	if(!ImportCell2Ds(mesh))
		return false;
*/
	return true;
}
// ***************************************************************************


bool ImportCell0Ds(PolygonalMesh &mesh) {
	ifstream file("./Cell0Ds.csv");

	//error for a missing Cell0Ds.csv
	if(file.fail()) {
		cerr << "Error importing Cell0Ds.csv" << endl;
		return false;
	}

	list<string> listLines; //create list to save all lines

	//store lines
	string line;
	while (getline(file, line))
	        listLines.push_back(line);

	file.close();

	listLines.pop_front(); //remove header

	mesh.NumCell0Ds = listLines.size(); //memorize number of lines

	//error for an empty or broken Cell0Ds.csv
	if (mesh.NumCell0Ds == 0) {
		cerr << "Cell0D empty or broken" << endl;
		return false;
	}

	//create matrix for Coordinates and vector for Id
	mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(2, mesh.NumCell0Ds);
	mesh.Cell0DsId.reserve(mesh.NumCell0Ds);

	for (const string &line : listLines) {
		istringstream converter(line); //reads rows from Cell0Ds.csv and saves to variable converter

		string token; //create temp variable

		//create variables for each column and save data per each line
		getline(converter, token, ';');
		unsigned int id = std::stoul(token);
		getline(converter, token, ';');
		unsigned int marker = std::stoul(token);
		getline(converter, token, ';');
		double x = std::stod(token);
		getline(converter, token, ';');
		double y = std::stod(token);

		//store x y coordinates inside respective matrix
		mesh.Cell0DsCoordinates(0, id) = x;
		mesh.Cell0DsCoordinates(1, id) = y;

		//store id inside respective vector
		mesh.Cell0DsId.push_back(id);

		//store markers per id
	        if(marker != 0) {
			const auto it = mesh.MarkerCell0Ds.find(marker);
			if(it == mesh.MarkerCell0Ds.end()) {
				mesh.MarkerCell0Ds.insert({marker, {id}});
			}
			else {
				it->second.push_back(id);
			}
		}
	}

	return true;
}
// ***************************************************************************


bool ImportCell1Ds(PolygonalMesh &mesh) {
        ifstream file("./Cell1Ds.csv");

        //error for a missing Cell1Ds.csv
        if(file.fail()) {
                cerr << "Error importing Cell1Ds.csv" << endl;
                return false;
        }

        list<string> listLines; //create list to save all lines

        //store lines
        string line;
        while (getline(file, line))
                listLines.push_back(line);

        file.close();

        listLines.pop_front(); //remove header

        mesh.NumCell1Ds = listLines.size(); //memorize number of lines

        //error for an empty or broken Cell1Ds.csv
        if (mesh.NumCell1Ds == 0) {
                cerr << "Cell1D empty or broken" << endl;
                return false;
        }

        //create matrix for Extremes and vector for Id
        mesh.Cell1DsExtremes = Eigen::MatrixXi::Zero(2, mesh.NumCell1Ds);
        mesh.Cell1DsId.reserve(mesh.NumCell1Ds);

        for (const string &line : listLines) {
                istringstream converter(line); //reads rows from Cell1Ds.csv and saves to variable converter

                string token; //create temp variable

                //create variables for each column and save data per each line
                getline(converter, token, ';');
                unsigned int id = std::stoul(token);
                getline(converter, token, ';');
                unsigned int marker = std::stoul(token);
                getline(converter, token, ';');
                unsigned int origin = std::stoul(token);
                getline(converter, token, ';');
                unsigned int end = std::stoul(token);

                //store origin end extremes inside respective matrix
                mesh.Cell1DsExtremes(0, id) = origin;
                mesh.Cell1DsExtremes(1, id) = end;

                //store id inside respective vector
                mesh.Cell1DsId.push_back(id);

                //store markers per id
                if(marker != 0) {
                        const auto it = mesh.MarkerCell1Ds.find(marker);
                        if(it == mesh.MarkerCell1Ds.end()) {
                                mesh.MarkerCell1Ds.insert({marker, {id}});
                        }
                        else {
                                it->second.push_back(id);
                        }
                }
        }

        return true;
}
// ***************************************************************************

/*
bool ImportCell2Ds(TriangularMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        array<unsigned int, 3> vertices;
        array<unsigned int, 3> edges;

        converter >>  id;
        for(unsigned int i = 0; i < 3; i++)
            converter >> vertices[i];
        for(unsigned int i = 0; i < 3; i++)
            converter >> edges[i];

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
    }

    return true;
}
*/
}
