/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.


Class
    Foam::dynamicMotionSolversRefineBalancedFvMesh

SourceFiles
    dynamicMotionSolversRefineBalancedFvMesh.C
    dynamicMotionSolversRefineBalancedFvMeshTemplates.C

Authors
    Lianzheng Cui <lzhcui@sjtu.edu.cn>
	Zuogang Chen <zgchen@sjtu.edu.cn>
	This mesh balancing part was based on:
	{
     T.G. Voskuilen ( https://github.com/tgvoskuilen/meshBalancing )
     Daniel Deising <deising@mma.tu-darmstadt.de>
     Daniel Rettenmaier <rettenmaier@gsc.tu-darmstadt.de>
	}
    All rights reserved.

Description
    A fvMesh handing mesh motion and refinement with run-time load balancing

    If you use this software for your scientific work or your publications,
    please don't forget to acknowledge explicitly the use of it.

\*---------------------------------------------------------------------------*/

#ifndef dynamicMotionSolversRefineBalancedFvMesh_H
#define dynamicMotionSolversRefineBalancedFvMesh_H


#include "dynamicFvMesh.H"
#include "dynamicMotionSolversRefineFvMesh.H"
#include "hexRef8.H"
#include "PackedBoolList.H"
#include "Switch.H"
#include "decompositionMethod.H"
#include "fvMeshDistributeAddPatch.H"
#include "mapDistributePolyMesh.H"
#include "PtrDictionary.H"
#include "dictionaryEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class dynamicMotionSolversRefineBalancedFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dynamicMotionSolversRefineBalancedFvMesh
:
    public dynamicMotionSolversRefineFvMesh
{

private:


        //- Disallow default bitwise copy construct
        dynamicMotionSolversRefineBalancedFvMesh(const dynamicMotionSolversRefineBalancedFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const dynamicMotionSolversRefineBalancedFvMesh&);

        //-
        label topParentID(label p);

        //-
        bool rebalance_;

public:

    //- Runtime type information
    TypeName("dynamicMotionSolversRefineBalancedFvMesh");


    // Constructors

        //- Construct from IOobject
        explicit dynamicMotionSolversRefineBalancedFvMesh
		(
		const IOobject& io, 
		const bool doInit=true
		);


    //- Destructor
    virtual ~dynamicMotionSolversRefineBalancedFvMesh();


    // Member Functions
	
         //- Initialise all non-demand-driven data
		 virtual bool init(const bool doInit);
		 
        //- Update the mesh for both mesh motion and topology change
        virtual bool update();
		
		// redistribute cells for load-balancing
		virtual bool loadBalancing();

        //- Map all fields in time using given map.
        virtual void mapFields(const mapPolyMesh& mpm);

        //- Template to update all volField boundaries
        template<class Type> void correctBoundaries();

        //-
        bool rebalance()
        {
            return rebalance_;
        }
	// Writing

        //- Write using given format, version and compression
        virtual bool writeObject
        (
            IOstreamOption streamOpt,
            const bool valid
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "dynamicMotionSolversRefineBalancedFvMeshTemplates.C"
#endif

#endif

// ************************************************************************* //
