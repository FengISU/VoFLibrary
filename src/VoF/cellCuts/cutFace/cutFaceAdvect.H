/*---------------------------------------------------------------------------*\
    Modified work | Copyright (c) 2017-2019, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the VoFLibrary source code library, which is an 
	unofficial extension to OpenFOAM.

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
    Foam::cutFaceAdvect

Description
    calculates the face fluxes

    Reference:
    \verbatim
        Roenby, J., Bredmose, H. and Jasak, H. (2016).
        A computational method for sharp interface advection
        Royal Society Open Science, 3
        doi 10.1098/rsos.160405

    \endverbatim

Author
    Johan Roenby, DHI, all rights reserved.

SourceFiles
    cutFaceAdvect.C

\*---------------------------------------------------------------------------*/

#ifndef cutFaceAdvect_H
#define cutFaceAdvect_H

#include "OFstream.H"
#include "cutFace.H"
#include "fvMesh.H"
#include "surfaceFields.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class cutFaceAdvects Declaration
\*---------------------------------------------------------------------------*/

class cutFaceAdvect
:
    public cutFace
{
    // Private data

        typedef DynamicList<label> DynamicLabelList;
        typedef DynamicList<scalar> DynamicScalarList;
        typedef DynamicList<vector> DynamicVectorList;
        typedef DynamicList<point> DynamicPointList;

        //- Mesh whose cells and faces to cut at their intersection with an isoface
        const fvMesh& mesh_;

        //- reference to VoF field
        const volScalarField& alpha1_;

        //- Storage for centre of subface
        point subFaceCentre_;

        //- Storage for area vector of subface
        vector subFaceArea_;

        //- Storage for subFacePoints
        DynamicList<point> subFacePoints_;

        //- Storage for subFacePoints
        DynamicList<point> surfacePoints_;

        //- Storage for pointStatus_ cuts the cell at 0
        DynamicList<scalar> pointStatus_;

        //- Storage of the edge weight
        DynamicList<scalar> weight_;

        //- Storage for arrival Times
        DynamicList<scalar> pTimes_;

        //- A face status label taking one of the values:
        //
        //  - -1: face is fully below the isosurface
        //  -  0: face is cut, i.e. has values larger and smaller than isoValue_
        //  - +1: face is fully above the isosurface
        label faceStatus_;

        //- write faces to vtk files
        void isoFacesToFile
        (
            const DynamicList<List<point>>& isoFacePts,
            const word fileName,
            const word fileDir
        ) const;

  public:
    // Constructors

        //- Construct from fvMesh and a scalarField
        //  Length of scalarField should equal number of mesh points
        cutFaceAdvect(const fvMesh& mesh, const volScalarField& alpha1);

        // Member functions

        //- calculates cut centre and cut area (plicReconstruction) and return facestatus
        label  calcSubFace
        (
            const label& faceI,
            const vector& normal,
            const vector& base
        );

        //- calculates cut centre and cut area (iso reconstruction) and return facestatus
        label calcSubFace
        (
            const face& f,
            const pointField& points,
            const scalarField& val,
            const scalar& cutValue
        );

        //- calculates cut centre and cut area (iso reconstruction) and return facestatus
        label calcSubFace
        (
            const label& faceI,
            const label sign,
            const scalar& cutValue
        );

        //- Calculate time integrated flux for a face
        scalar timeIntegratedFlux
        (
            const label faceI,
            const vector& x0,
            const vector& n0,
            const scalar Un0,
            const scalar dt,
            const scalar phi,
            const scalar magSf
        );

        //- Calculate time integrated flux for a face
        scalar timeIntegratedFlux
        (
            const label faceI,
            const scalarField times,
            const scalar Un0,
            const scalar dt,
            const scalar phi,
            const scalar magSf
        );

        //- Calculate time integrated area for a face
        scalar timeIntegratedArea
        (
            const label& faceI,
            const scalar dt,
            const scalar magSf,
            const scalar Un0
        );

        //- Calculate time integrated area for a face
        scalar timeIntegratedArea
        (
            const pointField& fPts,
            const scalarField& pTimes,
            const scalar dt,
            const scalar magSf,
            const scalar Un0
        );

        //- For face with vertices p calculate its area and integrated area
        //  between isocutting lines with isovalues f0 and f1 given vertex
        //  values f
        void quadAreaCoeffs
        (
            const DynamicPointList& pf0,
            const DynamicPointList& pf1,
            scalar& quadArea,
            scalar& intQuadArea
        ) const;

        //- get cutPoints of face
        void cutPoints
        (
             const label& faceI,
             const scalar f0,
             DynamicList<point>& cutPoints
         );

        //- get cutPoints of face
        void cutPoints
        (
            const pointField& pts,
            const scalarField& f,
            const scalar f0,
            DynamicList<point>& cutPoints
        );

        //- returns centre of cutted face
        point subFaceCentre();

        //- returns area vector of cutted face
        vector subFaceArea();

        //- returns the cut edge of the cutted face
        DynamicList<point>& subFacePoints();

        //- returns point of the cutted face in sorted order
        DynamicList<point>& surfacePoints();

        //- resets internal variables
        void clearStorage();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
