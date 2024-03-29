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

#include "algebraic.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace relativeVelocityModels
{
    defineTypeNameAndDebug(algebraic, 0);
    addToRunTimeSelectionTable(relativeVelocityModel, algebraic, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::relativeVelocityModels::algebraic::algebraic
(
    const dictionary& dict,
    const incompressibleTwoPhaseInteractingMixture& mixture
)
:
    relativeVelocityModel(dict, mixture),
    residualRe_("residualRe", dimless, dict),
    turbulenceCorrect_(dict.getOrDefault("turbulenceCorrect", false))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::relativeVelocityModels::algebraic::~algebraic()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::relativeVelocityModels::algebraic::correct()
{
    volScalarField betac(alphac_*rhoc_);
    volScalarField betad(alphad_*rhod_);

    // Calculate the relative velocity of the continuous phase w.r.t the mean
    volVectorField Ucm(betad*Udm_/betac);

    // particle Reynolds number
    volScalarField Re_p(alphac_ * rhoc_ * mixture_.dd() * mag(Udc_) / (mixture_.nucModel().nu() * rhoc_) + residualRe_);

    volScalarField fd(1. + 0.15 * pow(Re_p, 0.687));

    forAll(Re_p, i)
    {
        if (Re_p[i] >= 1000)
        {
            fd[i] = 0.0183 * Re_p[i];
        }
    }

    //volVectorField a();
    const meshObjects::gravity& g = meshObjects::gravity::New(mixture_.U().db().time());

    // Info << g.value() << endl;
    /*
    meshObjects::gravity g
    (
        mixture_.U().mesh().lookupObject<meshObjects::gravity>("g")
    );
    */
    volVectorField a(g - mixture_.U()*fvc::div(mixture_.U()) - fvc::ddt(mixture_.U()));

    volVectorField Udc((rhod_-rho())*sqr(mixture_.dd())/18./(mixture_.nucModel().nu()*rhoc_)/fd*a);

    /*
    Fluent默认解法，但是不知为何效果很差，弃用
    if(turbulenceCorrect_)
    {
        const volScalarField &nut = mixture_.U().mesh().lookupObject<volScalarField>("nut");
        Udc -= nut*(fvc::grad(alphad_)/alphad_ - fvc::grad(alphac_)/alphac_);
    }
    */

    //volScalarField tau_p(rhod_* mixture_.dd() * mixture_.dd() / 18 / (mixture_.nucModel().nu() * rhoc_));

    //volScalarField K(3 / 4 / mixture_.dd() * rhoc_ * Cd * pow(alphac_, -1.65) * (1 - alphac_) * mag(Udm_ - Ucm));
    
    // 离散相与连续相的相对速度 -> 离散相与混合相的相对速度
    // betad = alphad * rhod
    // betac = alphac * rhoc
    // rhom = betad + betac
    // (betad + betac) * Um = betad * Ud + betac * Uc 
    //
    // Udm = Ud - Um 
    //     = (1- betad / (betad + betac)) * Ud - betac / (betad + betac) * Uc 
    //     = betac / (betad + betac) * Ud - betac / (betad + betac) * Uc 
    //     = betac / rhom * (Ud - Uc) = betac / rhom * Udc
    // dimensionedVector test(dimVelocity, vector(0,0,-0.3));
    Udm_ = betac / rho() * Udc;

    Udc_ = Udc;
}


// ************************************************************************* //
