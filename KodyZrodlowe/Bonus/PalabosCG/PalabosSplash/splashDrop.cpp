
/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011 FlowKit Sarl
 * Avenue de Chailly 23
 * 1012 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Main author: Andrea Parmigiani */

#include <vector>
using namespace std;

#include "palabos3D.h"
#include "palabos3D.hh"

using namespace plb;
using namespace std;

#define DESCRIPTOR descriptors::ForcedD3Q19Descriptor
typedef double T;


// Smagorinsky constant for LES model.
const T cSmago = 0.0;

// Physical dimensions of the system (in meters).
const T lx = 1.0;
const T ly = 1.0;
const T lz = 1.0;

const T iniRhoFluid = T(1); 
const T rhoEmpty    = T(1);
Array<T,3> forceOrientation(T(),T(),(T)1);
    
plint writeImagesIter   = 60;
plint getStatisticsIter = 60;

plint maxIter;
plint N;
plint nx, ny, nz;
T delta_t, delta_x;
Array<T,3> externalForce;
T nuPhys, nuLB, tau, omega;

std::string outDir;
plint obstacleCenterXYplane, obstacleLength, obstacleWidth, obstacleHeight, beginWaterReservoir, waterReservoirHeight;
plint waterLevelOne, waterLevelTwo, waterLevelThree, waterLevelFour;

void setupParameters() {
    delta_x = T(1)/N;
    nx = (plint) (lx*N);
    ny = (plint) (ly*N);
    nz = (plint) (lz*N);


    // Gravity in lattice units.
    externalForce = Array<T,3>(0., 0., (-9.8* delta_t * delta_t)/delta_x);
    tau            = (nuPhys*DESCRIPTOR<T>::invCs2*delta_t)/(delta_x*delta_x) + 0.5;
    omega          = 1./tau;    
    nuLB           = (tau-0.5)*DESCRIPTOR<T>::cs2; // Viscosity in lattice units.
}

int initialFluidFlags(plint iX, plint iY, plint iZ) {
   
   int R = nx/15;
   bool insideDrop = ( sqrt( pow(iX-nx/2,2.0) + pow(iY-ny/2,2.0) + pow(iZ-(nz/2+6*R),2.0)) < R );
//   bool insideDrop = ( sqrt( pow(iX-nx/2+nx/8,2.0) + pow(iY-ny/2+ny/8,2.0)) < R ); // column
   bool insideCylinder = ( sqrt( pow(iX-nx/2,2.0) + pow(iY-ny/2,2.0)) < nx/2-10 );
   
   if( insideDrop)
     {
	return freeSurfaceFlag::fluid;
     }   
   else
     if (iZ <= nz/2) {
        return freeSurfaceFlag::fluid;
    }
    else 
     if( insideCylinder)
     {
        return freeSurfaceFlag::empty;
    }
   else return freeSurfaceFlag::wall;
}

void writeResults(MultiBlockLattice3D<T,DESCRIPTOR>& lattice, MultiScalarField3D<T>& volumeFraction, plint iT)
{
    static const plint nx = lattice.getNx();
    static const plint ny = lattice.getNy();
    static const plint nz = lattice.getNz();
    Box3D slice(0, nx-1, ny/2, ny/2, 0, nz-1);
/*    ImageWriter<T> imageWriter("leeloo");
    imageWriter.writeScaledPpm(createFileName("u", iT, 6),
                               *computeVelocityNorm(lattice, slice)); 

    imageWriter.writeScaledPpm(createFileName("rho", iT, 6),
                               *computeDensity(lattice, slice));
                   
    imageWriter.writeScaledPpm(createFileName("volumeFraction", iT, 6), *extractSubDomain(volumeFraction, slice));
*/
    // Use a marching-cube algorithm to reconstruct the free surface and write an STL file.
    std::vector<T> isoLevels;
    isoLevels.push_back((T) 0.5);
    typedef TriangleSet<T>::Triangle Triangle;
    std::vector<Triangle> triangles;
    isoSurfaceMarchingCube(triangles, volumeFraction, isoLevels, volumeFraction.getBoundingBox().enlarge(-5));
//    TriangleSet<T>(triangles).writeAsciiSTL(createFileName(outDir+"/interface", iT, 6)+".stl");
    TriangleSet<T>(triangles).writeBinarySTL(createFileName(outDir+"/binterface", iT, 6)+".stl");
}

int main(int argc, char **argv)
{
    plbInit(&argc, &argv);
    global::directories().setInputDir("./");
        
    if (global::argc() != 6) {
        pcout << "Error missing some input parameter\n";
    }

    try {
        global::argv(1).read(outDir);
        global::directories().setOutputDir(outDir+"/");

        global::argv(2).read(nuPhys);
        global::argv(3).read(N);
        global::argv(4).read(delta_t);
        global::argv(5).read(maxIter);
    }
    catch(PlbIOException& except) 
     {
        pcout << except.what() << std::endl;
        pcout << "The parameters for this program are :\n";
        pcout << "1. Output directory name.\n";
        pcout << "2. kinematic viscosity in physical Units (m^2/s) .\n";
        pcout << "3. number of lattice nodes for lz .\n";
        pcout << "4. delta_t .\n"; 
        pcout << "5. maxIter .\n";
        pcout << "Reasonable parameters on a desktop computer are: " << (std::string)global::argv(0) << " tmp 1.e-5 40 1.e-3 80000\n";
        pcout << "Reasonable parameters on a parallel machine are: " << (std::string)global::argv(0) << " tmp 1.e-6 100 1.e-4 80000\n";
        exit (EXIT_FAILURE);
    }
    
    setupParameters();
    
    pcout << "delta_t= " << delta_t << endl;
    pcout << "delta_x= " << delta_x << endl;
    pcout << "delta_t*delta_t/delta_x= " << delta_t*delta_t/delta_x << endl;
    pcout << "externalForce= " << externalForce[2] << endl;
    pcout << "relaxation time= " << tau << endl;
    pcout << "omega= " << omega << endl;
    pcout << "kinematic viscosity physical units = " << nuPhys << endl;
    pcout << "kinematic viscosity lattice units= " << nuLB << endl;
    
    global::timer("initialization").start();

    SparseBlockStructure3D blockStructure(createRegularDistribution3D(nx, ny, nz));

    Dynamics<T,DESCRIPTOR>* dynamics
        = new SmagorinskyBGKdynamics<T,DESCRIPTOR>(omega, cSmago);

    FreeSurfaceFields3D<T,DESCRIPTOR> fields(blockStructure, dynamics->clone(), iniRhoFluid, rhoEmpty, externalForce);

    // Set all outer-wall cells to "wall" (here, bulk-cells are also set to "wall", but it
    // doesn't matter, because they are overwritten on the next line).
    setToConstant(fields.flag, fields.flag.getBoundingBox(), (int)freeSurfaceFlag::wall);
    // In the bulk (all except outer wall layer), initialize the flags as specified by
    // the function "initialFluidFlags".
    setToFunction(fields.flag, fields.flag.getBoundingBox().enlarge(-1), initialFluidFlags);
    
    fields.defaultInitialize();

    pcout << "Time spent for setting up lattices: "
          << global::timer("initialization").stop() << endl;
    T lastIterationTime = T();

    for (plint iT = 0; iT <= maxIter; ++iT) {
        global::timer("iteration").restart();
        
        if (iT % getStatisticsIter==0) {
            pcout << endl;
            pcout << "ITERATION = " << iT << endl;
            pcout << "Time of last iteration is " << lastIterationTime << " seconds" << endl;
            //writeStatistics(fields);
        }
        
        // This includes the collision-streaming cycle, plus all free-surface operations.
        fields.lattice.executeInternalProcessors();


        if (iT % writeImagesIter == 0) {
            global::timer("images").start();
            writeResults(fields.lattice, fields.volumeFraction, iT);
            pcout << "Total time spent for writing images: "
                << global::timer("images").stop() << endl;
        }                           
        lastIterationTime = global::timer("iteration").stop();
    }
}

