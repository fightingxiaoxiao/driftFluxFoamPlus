/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2016 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "CompressibleTurbulenceModel.H"
#include "incompressibleTwoPhaseInteractingMixture.H"
#include "addToRunTimeSelectionTable.H"
#include "makeTurbulenceModel.H"

#include "laminarModel.H"
#include "RASModel.H"
#include "LESModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTurbulenceModelTypes
(
    geometricOneField,
    volScalarField,
    compressibleTurbulenceModel,
    CompressibleTurbulenceModel,
    incompressibleTwoPhaseInteractingMixture
);

makeBaseTurbulenceModel
(
    geometricOneField,
    volScalarField,
    compressibleTurbulenceModel,
    CompressibleTurbulenceModel,
    incompressibleTwoPhaseInteractingMixture
);

#define makeLaminarModel(Type)                                                 \
    makeTemplatedTurbulenceModel                                               \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixtureCompressibleTurbulenceModel,   \
        laminar,                                                               \
        Type                                                                   \
    )

#define makeRASModel(Type)                                                     \
    makeTemplatedTurbulenceModel                                               \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixtureCompressibleTurbulenceModel,   \
        RAS,                                                                   \
        Type                                                                   \
    )

#define makeLESModel(Type)                                                     \
    makeTemplatedTurbulenceModel                                               \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixtureCompressibleTurbulenceModel,   \
        LES,                                                                   \
        Type                                                                   \
    )

// -------------------------------------------------------------------------- //
// Laminar models
// -------------------------------------------------------------------------- //

#include "Stokes.H"
makeLaminarModel(Stokes);


// -------------------------------------------------------------------------- //
// RAS models
// -------------------------------------------------------------------------- //

#include "kEpsilon.H"
makeRASModel(kEpsilon);

#include "buoyantKEpsilon.H"
makeRASModel(buoyantKEpsilon);

#include "realizableKE.H"
makeRASModel(realizableKE);

// -------------------------------------------------------------------------- //
// LES models
// -------------------------------------------------------------------------- //

#include "Smagorinsky.H"
makeLESModel(Smagorinsky);

#include "kEqn.H"
makeLESModel(kEqn);


// ************************************************************************* //
