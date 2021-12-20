/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
    Foam::dynamicMotionSolversRefineFvMesh
	
Authors
    Lianzheng Cui <lzhcui@sjtu.edu.cn>
	Zuogang Chen <zgchen@sjtu.edu.cn>
	The mesh balancing part was based on:
	{
     T.G. Voskuilen ( https://github.com/tgvoskuilen/meshBalancing )
     Daniel Deising <deising@mma.tu-darmstadt.de>
     Daniel Rettenmaier <rettenmaier@gsc.tu-darmstadt.de>
	}
    All rights reserved.

Description
    A fvMesh with built-in mesh refinement and mesh motion.

    Determines which cells to refine/unrefine and does all in update().

SourceFiles
    dynamicMotionSolversRefineFvMesh.C

\*---------------------------------------------------------------------------*/

#ifndef dynamicMotionSolversRefineFvMesh_H
#define dynamicMotionSolversRefineFvMesh_H

#include "dynamicMotionSolverListFvMesh.H"
#include "hexRef.H"
#include "bitSet.H"
#include "cellSet.H"
#include "adaptCriteria.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class dynamicMotionSolversRefineFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dynamicMotionSolversRefineFvMesh
:
    public dynamicMotionSolverListFvMesh
{
protected:

        //- Mesh cutting engine
        autoPtr<hexRef> meshCutter_;

        //- Fluxes to map
        HashTable<word> correctFluxes_;

        //- Protected cells (usually since not hexes)
        bitSet protectedCell_;

        //- Number of refinement/unrefinement steps done so far.
        label nRefinementIterations_;

        //- Dump cellLevel for post-processing
        bool dumpLevel_;

        //- adaptCriteria
        autoPtr<adaptCriteria> adaptCriteriaPtr_;
		
		//cellSet for identify the moving region
		cellSet* movingSetPtr;
		
		//Map for overset mesh if exit
		labelIOList* indexMapPtr;
		
		// autoPtr<mapPolyMesh> refineMap_;
		
		// autoPtr<mapPolyMesh> unrefineMap_;
		
		
		


    // Protected Member Functions

        //- Calculate cells that cannot be refined since would trigger
        //  refinement of protectedCell_ (since 2:1 refinement cascade)
        void calculateProtectedCells(bitSet& unrefineableCell) const;

        //- Read the projection parameters from dictionary
        void readDict();


        //- Refine cells. Update mesh and fields.
        virtual autoPtr<mapPolyMesh> refine(const labelList&);

        //- Unrefine cells. Gets passed in centre points of cells to combine.
        virtual autoPtr<mapPolyMesh> unrefine(const labelList&);


        // Selection of cells to un/refine

            //- Calculates approximate value for refinement level so
            //  we don't go above maxCell
            scalar getRefineLevel
            (
                const label maxCells,
                const label maxRefinement,
                const scalar refineLevel,
                const scalarField&
            ) const;

            //- Get per cell max of connected point
            scalarField maxPointField(const scalarField&) const;

            //- Get point max of connected cell
            scalarField maxCellField(const volScalarField&) const;

            scalarField cellToPoint(const scalarField& vFld) const;

            scalarField error
            (
                const scalarField& fld,
                const scalar minLevel,
                const scalar maxLevel
            ) const;

            //- Select candidate cells for refinement
            virtual void selectRefineCandidates
            (
                const scalar lowerRefineLevel,
                const scalar upperRefineLevel,
                const scalarField& vFld,
                bitSet& candidateCell
            ) const;

            //- Subset candidate cells for refinement
            virtual labelList selectRefineCells
            (
                const label maxCells,
                const label maxRefinement,
                const bitSet& candidateCell
            ) const;

            // //- Select points that can be unrefined.
            // virtual labelList selectUnrefinePoints
            // (
            //     const scalar unrefineLevel,
            //     const bitSet& markedCell,
            //     const scalarField& pFld
            // ) const;

            //- Extend markedCell with cell-face-cell.
            void extendMarkedCells(bitSet& markedCell) const;

            //- Check all cells have 8 anchor points
            void checkEightAnchorPoints(bitSet& protectedCell) const;

            //- Map single non-flux surface<Type>Field
            //  for new internal faces (e.g. AMR refine). This currently
            //  interpolates values from surrounding faces (faces on
            //  neighbouring cells) that do have a value.
            template <class T>
            void mapNewInternalFaces
            (
                const labelList& faceMap,
                GeometricField<T, fvsPatchField, surfaceMesh>&
            );

            //- Correct surface fields for new faces
            template <class T>
            void mapNewInternalFaces(const labelList& faceMap);

            //- Correct surface fields for new faces. Converts any oriented
            //  fields into non-oriented (i.e. phi -> Uf) before mapping
            template <class T>
            void mapNewInternalFaces
            (
                const surfaceVectorField& Sf,
                const surfaceScalarField& magSf,
                const labelList& faceMap
            );


private:

        //- No copy construct
        dynamicMotionSolversRefineFvMesh(const dynamicMotionSolversRefineFvMesh&) = delete;

        //- No copy assignment
        void operator=(const dynamicMotionSolversRefineFvMesh&) = delete;

public:

    //- Runtime type information
    TypeName("dynamicMotionSolversRefineFvMesh");


    // Constructors

        //- Construct from IOobject
        explicit dynamicMotionSolversRefineFvMesh
		(
		const IOobject& io,
		const bool doInit=true
		);


    //- Destructor
    virtual ~dynamicMotionSolversRefineFvMesh() = default;


    // Member Functions
	
	     //- Initialise all non-demand-driven data
        virtual bool init(const bool doInit);

        //- Direct access to the refinement engine
        const autoPtr<hexRef>& meshCutter() const
        {
            return meshCutter_;
        }

        //- Cells which should not be refined/unrefined
        const bitSet& protectedCell() const
        {
            return protectedCell_;
        }

        //- Cells which should not be refined/unrefined
        bitSet& protectedCell()
        {
            return protectedCell_;
        }
		
        //- Update the mesh for both mesh motion and topology change
        virtual bool update();

        //- Map all fields in time using given map.
        virtual void mapFields(const mapPolyMesh& mpm);
		

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
    #include "dynamicMotionSolversRefineFvMeshTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
