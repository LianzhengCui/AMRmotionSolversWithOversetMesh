/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 OpenFOAM Foundation
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
    Foam::RBD::rigidBodySolidMotion

Description
    Six degree of freedom motion for a rigid body.

    Angular momentum stored in body fixed reference frame.  Reference
    orientation of the body (where Q = I) must align with the cartesian axes
    such that the Inertia tensor is in principle component form.  Can add
    restraints (e.g. a spring) and constraints (e.g. motion may only be on a
    plane).

    The time-integrator for the motion is run-time selectable with options for
    symplectic (explicit), Crank-Nicolson and Newmark schemes.

SourceFiles
    rigidBodySolidMotionI.H
    rigidBodySolidMotion.C
    rigidBodySolidMotionIO.C

\*---------------------------------------------------------------------------*/

#ifndef rigidBodySolidMotion_H
#define rigidBodySolidMotion_H

#include "rigidBodyModel.H"
#include "rigidBodyModelState.H"
#include "pointField.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declarations
class Time;

namespace RBD
{

// Forward declarations
class rigidBodySolver;

/*---------------------------------------------------------------------------*\
                      Class rigidBodySolidMotion Declaration
\*---------------------------------------------------------------------------*/

class rigidBodySolidMotion
:
    public rigidBodyModel
{
    friend class rigidBodySolver;

    // Private data

        //- Motion state data object
        rigidBodyModelState motionState_;

        //- Motion state data object for previous time-step
        rigidBodyModelState motionState0_;

        //- Initial transform for external forces to the bodies reference frame
        List<spatialTransform> X0P_;

        //- Acceleration relaxation coefficient
        scalar aRelax_;

        //- Acceleration damping coefficient (for steady-state simulations)
        scalar aDamp_;

        //- Switch to turn reporting of motion data on and off
        Switch report_;

        //- Motion solver
        autoPtr<rigidBodySolver> solver_;


    // Private Member Functions

        //- Initialize the body-state
        void initialize();

        //- No copy construct
        rigidBodySolidMotion(const rigidBodySolidMotion&) = delete;

        //- No copy assignment
        void operator=(const rigidBodySolidMotion&) = delete;


public:

    // Constructors

        //- Construct null
        rigidBodySolidMotion(const Time& time);

        //- Construct from dictionary
        rigidBodySolidMotion
        (
            const Time& time,
            const dictionary& dict
        );

        //- Construct from constant and state dictionaries
        rigidBodySolidMotion
        (
            const Time& time,
            const dictionary& dict,
            const dictionary& stateDict
        );


    //- Destructor
    ~rigidBodySolidMotion();


    // Member Functions

        // Access

            //- Return the report Switch
            inline bool report() const;

            //- Return the motion state
            inline const rigidBodyModelState& state() const;

            //- Return the motion state for modification
            inline rigidBodyModelState& state();

            //- Return the initial transform to the global frame for the
            //  given body
            spatialTransform X0P(const label bodyId) const;


        // Edit

            //- Store the motion state at the beginning of the time-step
            inline void newTime();


        // Update state

            //- Calculate and optionally relax the joint acceleration qDdot from
            //  the joint state q, velocity qDot, internal force tau (in the
            //  joint frame) and external force fx (in the global frame)
            void forwardDynamics
            (
                rigidBodyModelState& state,
                const scalarField& tau,
                const Field<spatialVector>& fx
            ) const;

            //- Integrate velocities, orientation and position
            //  for the given time and time-step
            void solve
            (
                const scalar t,
                const scalar deltaT,
                const scalarField& tau,
                const Field<spatialVector>& fx
            );
			
			//update the X0P_
			void equalstate();

            //- Report the status of the motion of the given body
            void status(const label bodyID) const;

            //- Report linear velocity of the given body
            const vector vCofR(const label bodyID) const;

            //- Report CofR of the given body
            const vector cCofR(const label bodyID) const;


        // Transformations

            //- Transform the given initial pointField of the specified body
            //  to correspond to the current motion state
            tmp<pointField> transformPoints
            (
                const label bodyID,
                const pointField& previousPoints
            ) const;

            //- Transform the given initial pointField of the specified body
            //  to correspond to the current motion state scaled using
            //  'slerp' interpolation
            tmp<pointField> transformPoints
            (
                const label bodyID,
                const scalarField& weight,
                const pointField& previousPoints
            ) const;

            //- Transform the given initial pointField of the specified body
            //  to correspond to the current motion state scaled using
            //  'slerp' interpolation
            tmp<pointField> transformPoints
            (
                const labelList& bodyIDs,
                const List<const scalarField*>& weights,
                const pointField& previousPoints
            ) const;


        //- Write
        void write(Ostream&) const;

        //- Read coefficients dictionary and update system parameters,
        //  constraints and restraints but not the current state
        bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rigidBodySolidMotionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
