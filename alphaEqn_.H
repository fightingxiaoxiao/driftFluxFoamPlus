    surfaceScalarField alphaPhi
    (
        IOobject
        (
            "alphaPhi",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar(phi.dimensions(), Zero)
    );        
    
    surfaceScalarField phir(fvc::flux(UdmModel.Udm()));

    fvScalarMatrix alpha1Eqn
    (
        fvm::ddt(alpha1)
      + fvm::div(phi,alpha1)
      + fvm::div(phir,alpha1)
      - fvm::laplacian(turbulence->nut(), alpha1)
    );
    alpha1Eqn.solve();

    //tmp<surfaceScalarField> talphaPhiUD();
    alphaPhi += alpha1Eqn.flux();

    alpha2 = 1.0 - alpha1;

    rhoPhi = alphaPhi*(rho1 - rho2) + phi*rho2;
    rho = mixture.rho();
