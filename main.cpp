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
	if(!ImportMesh(mesh)){
		cerr << "file not found" << endl;
		return 1;
	}

	

	return 0;
}
