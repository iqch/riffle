/*************************************************************
* Copyright (c) 2017 by Egor N. Chashchin. All Rights Reserved.          *
**************************************************************/

/*
*	RiCalls.h - RenderMan DSO Rif-filter for using python scripts
*  for filtering. Embedded module header
*
*	Version: 0.96
*	Authors: Egor N. Chashchin                   
*	Contact: iqcook@gmail.com 
* 
*/
#pragma once

#define DEFINE_RICALL(call) PyObject* _Ri##call(PyObject *self, PyObject *args)

// ONE INT
DEFINE_RICALL(FrameBegin);
DEFINE_RICALL(Sides);

// PLAIN
DEFINE_RICALL(FrameEnd);
DEFINE_RICALL(WorldBegin);
DEFINE_RICALL(WorldEnd);

DEFINE_RICALL(AttributeBegin);
DEFINE_RICALL(AttributeEnd);

DEFINE_RICALL(TransformBegin);
DEFINE_RICALL(TransformEnd);

DEFINE_RICALL(SolidEnd);

//FORWARD_PLAIN(ObjectBegin);
//DEFINE_RICALL(ObjectEnd);

DEFINE_RICALL(MotionEnd);

DEFINE_RICALL(Identity);

DEFINE_RICALL(ReverseOrientation);

DEFINE_RICALL(ArchiveEnd);

DEFINE_RICALL(Else);
DEFINE_RICALL(IfEnd);

DEFINE_RICALL(ResourceBegin);
DEFINE_RICALL(ResourceEnd);
DEFINE_RICALL(EditEnd);
DEFINE_RICALL(EditAttributeEnd);
DEFINE_RICALL(EditWorldEnd);

// ONE FLOAT
DEFINE_RICALL(FrameAspectRatio);
DEFINE_RICALL(PixelVariance);
DEFINE_RICALL(ShadingRate);
DEFINE_RICALL(RelativeDetail);
DEFINE_RICALL(Perspective);

// ONE COLOR
DEFINE_RICALL(Color);
DEFINE_RICALL(Opacity);

// BOOLEAN
DEFINE_RICALL(Matte);

// STRINGS
DEFINE_RICALL(ShadingInterpolation);
DEFINE_RICALL(SolidBegin);
DEFINE_RICALL(EditAttributeBegin);
DEFINE_RICALL(Orientation);
DEFINE_RICALL(CoordSysTransform);
DEFINE_RICALL(CoordinateSystem);
DEFINE_RICALL(ScopedCoordinateSystem);
DEFINE_RICALL(System);

// POLY FLOATS

// 2
DEFINE_RICALL(Clipping);
DEFINE_RICALL(Shutter);
DEFINE_RICALL(Exposure);
DEFINE_RICALL(PixelSamples);

// 3
DEFINE_RICALL(Scale);
DEFINE_RICALL(DepthOfField);
DEFINE_RICALL(Translate);

// 4
DEFINE_RICALL(CropWindow);
DEFINE_RICALL(ScreenWindow);
DEFINE_RICALL(DetailRange);
DEFINE_RICALL(Rotate);

// 6
DEFINE_RICALL(ClippingPlane);

// 7
DEFINE_RICALL(Skew);

// 8
DEFINE_RICALL(TextureCoordinates);

// BOUNDS
DEFINE_RICALL(Bound);
DEFINE_RICALL(Detail);

// MATRICIES
DEFINE_RICALL(ConcatTransform);
DEFINE_RICALL(Transform);

// TOKEN-DICTIONARY
DEFINE_RICALL(EditBeginV);
DEFINE_RICALL(IfBeginV);
DEFINE_RICALL(ElseIfV);
DEFINE_RICALL(ProjectionV);
DEFINE_RICALL(HiderV);
DEFINE_RICALL(OptionV);
DEFINE_RICALL(AttributeV);
DEFINE_RICALL(AtmosphereV);
DEFINE_RICALL(DisplacementV);
DEFINE_RICALL(ExteriorV);
DEFINE_RICALL(InteriorV);
DEFINE_RICALL(SurfaceV);
DEFINE_RICALL(GeometryV);
DEFINE_RICALL(PatchV);
DEFINE_RICALL(DisplayChannelV);
DEFINE_RICALL(CameraV);
DEFINE_RICALL(PixelSampleImagerV);
DEFINE_RICALL(EditWorldBeginV);
DEFINE_RICALL(ImagerV);

// DUO-TOKENS/DICT
DEFINE_RICALL(ResourceV);
DEFINE_RICALL(ShaderV);

// DICT WITH HANDLES
DEFINE_RICALL(ArchiveBeginV);
DEFINE_RICALL(LightSourceV);

// MISC
DEFINE_RICALL(ReadArchiveV);
DEFINE_RICALL(MotionBeginV);
DEFINE_RICALL(Format);
DEFINE_RICALL(GeometricApproximation);
DEFINE_RICALL(Quantize);
DEFINE_RICALL(Illuminate);
DEFINE_RICALL(DisplayV);
DEFINE_RICALL(PixelFilter);
DEFINE_RICALL(Basis);
DEFINE_RICALL(VArchiveRecord);

// MAKERS
DEFINE_RICALL(MakeTextureV);
DEFINE_RICALL(MakeShadowV);
DEFINE_RICALL(MakeCubeFaceEnvironmentV);
DEFINE_RICALL(MakeLatLongEnvironmentV);
DEFINE_RICALL(MakeBrickMapV);

// GPRIMS
DEFINE_RICALL(SphereV);
DEFINE_RICALL(ConeV);
DEFINE_RICALL(CylinderV);
DEFINE_RICALL(DiskV);
DEFINE_RICALL(TorusV);
DEFINE_RICALL(ParaboloidV);
DEFINE_RICALL(HyperboloidV);

DEFINE_RICALL(PointsV);
DEFINE_RICALL(CurvesV);

DEFINE_RICALL(NuPatchV);
DEFINE_RICALL(PatchMeshV);

DEFINE_RICALL(PolygonV);
DEFINE_RICALL(PointsPolygonsV);

DEFINE_RICALL(GeneralPolygonV);
DEFINE_RICALL(PointsGeneralPolygonsV);
DEFINE_RICALL(SubdivisionMeshV);
DEFINE_RICALL(HierarchicalSubdivisionMeshV);

// v 10
DEFINE_RICALL(VolumeV);
DEFINE_RICALL(VPSurfaceV);
DEFINE_RICALL(VPInteriorV);
DEFINE_RICALL(VPAtmosphereV);

// v 11
//DECLARE_CALLBACK(Procedural2V)

// v 12
DEFINE_RICALL(VolumePixelSamples);

// v 13
DEFINE_RICALL(BxdfV);
DEFINE_RICALL(IntegratorV);
DEFINE_RICALL(PatternV);

// v 14
DEFINE_RICALL(LightFilterV);

// v 15
DEFINE_RICALL(DisplaceV);
DEFINE_RICALL(SampleFilterV);
DEFINE_RICALL(DisplayFilterV);
DEFINE_RICALL(LightV);

// TABLE
#define RI_METHOD(method) {#method, _Ri##method, METH_VARARGS,""}
#define RI_METHODV(method) {#method, _Ri##method##V, METH_VARARGS,""}

static PyMethodDef MethodTable[] = {
	// ONE INT
	RI_METHOD(FrameBegin),
	RI_METHOD(Sides),
	
	// PLIAN
	RI_METHOD(FrameEnd),

	RI_METHOD(WorldBegin),
	RI_METHOD(WorldEnd),

	RI_METHOD(AttributeBegin),
	RI_METHOD(AttributeEnd),

	RI_METHOD(TransformBegin),
	RI_METHOD(TransformEnd),

	RI_METHOD(SolidEnd),

	//FORWARD_PLAIN(ObjectBegin);
	//RI_METHOD(ObjectEnd),

	RI_METHOD(MotionEnd),

	RI_METHOD(Identity),

	RI_METHOD(ReverseOrientation),

	RI_METHOD(ArchiveEnd),

	RI_METHOD(Else),
	RI_METHOD(IfEnd),

	RI_METHOD(ResourceBegin),
	RI_METHOD(ResourceEnd),
	RI_METHOD(EditEnd),
	RI_METHOD(EditAttributeEnd),
	RI_METHOD(EditWorldEnd),

	// ONE FLOAT
	RI_METHOD(FrameAspectRatio),
	RI_METHOD(PixelVariance),
	RI_METHOD(ShadingRate),
	RI_METHOD(RelativeDetail),
	RI_METHOD(Perspective),

	// ONE COLOR
	RI_METHOD(Color),
	RI_METHOD(Opacity),

	// BOOLEAN
	RI_METHOD(Matte),

	// STRINGS
	RI_METHOD(ShadingInterpolation),
	RI_METHOD(SolidBegin),
	RI_METHOD(EditAttributeBegin),
	RI_METHOD(Orientation),
	RI_METHOD(CoordSysTransform),
	RI_METHOD(CoordinateSystem),
	RI_METHOD(ScopedCoordinateSystem),
	RI_METHOD(System),

	// POLY FLOATS

	// 2
	RI_METHOD(Clipping),
	RI_METHOD(Shutter),
	RI_METHOD(Exposure),
	RI_METHOD(PixelSamples),

	// 3
	RI_METHOD(Scale),
	RI_METHOD(DepthOfField),
	RI_METHOD(Translate),

	// 4
	RI_METHOD(CropWindow),
	RI_METHOD(ScreenWindow),
	RI_METHOD(DetailRange),
	RI_METHOD(Rotate),

	// 6
	RI_METHOD(ClippingPlane),

	// 7
	RI_METHOD(Skew),

	// 8
	RI_METHOD(TextureCoordinates),

	// BOUNDS
	RI_METHOD(Bound),
	RI_METHOD(Detail),

	// MATRICIES
	RI_METHOD(ConcatTransform),
	RI_METHOD(Transform),

	// TOKEN-DICTIONARY
	RI_METHODV(EditBegin),
	RI_METHODV(IfBegin),
	RI_METHODV(ElseIf),
	RI_METHODV(Projection),
	RI_METHODV(Hider),
	RI_METHODV(Option),
	RI_METHODV(Attribute),
	RI_METHODV(Atmosphere),
	RI_METHODV(Displacement),
	RI_METHODV(Exterior),
	RI_METHODV(Interior),
	RI_METHODV(Surface),
	RI_METHODV(Geometry),
	RI_METHODV(Patch),
	RI_METHODV(DisplayChannel),
	RI_METHODV(Camera),
	RI_METHODV(PixelSampleImager),
	RI_METHODV(EditWorldBegin),
	RI_METHODV(Imager),

	// DUO-TOKENS/DICT
	RI_METHODV(Resource),
	RI_METHODV(Shader),

	// DICT WITH HANDLES
	RI_METHODV(ArchiveBegin),
	RI_METHODV(LightSource),

	// MISC
	RI_METHODV(ReadArchive),
	RI_METHODV(MotionBegin),
	RI_METHOD(Format),
	RI_METHOD(GeometricApproximation),
	RI_METHOD(Quantize),
	RI_METHOD(Illuminate),
	RI_METHODV(Display),
	RI_METHOD(PixelFilter),
	RI_METHOD(Basis),
	RI_METHOD(VArchiveRecord),

	// MAKERS
	RI_METHODV(MakeTexture),
	RI_METHODV(MakeShadow),
	RI_METHODV(MakeCubeFaceEnvironment),
	RI_METHODV(MakeLatLongEnvironment),
	RI_METHODV(MakeBrickMap),

	// GPRIMS
	RI_METHODV(Sphere),
	RI_METHODV(Cone),
	RI_METHODV(Cylinder),
	RI_METHODV(Disk),
	RI_METHODV(Torus),
	RI_METHODV(Paraboloid),
	RI_METHODV(Hyperboloid),

	RI_METHODV(Curves),
	RI_METHODV(Points),

	RI_METHODV(NuPatch),
	RI_METHODV(PatchMesh),

	RI_METHODV(Polygon),
	RI_METHODV(PointsPolygons),

	RI_METHODV(GeneralPolygon),
	RI_METHODV(PointsGeneralPolygons),
	RI_METHODV(SubdivisionMesh),
	RI_METHODV(HierarchicalSubdivisionMesh),

	// v 10
	RI_METHODV(Volume),
	RI_METHODV(VPSurface),
	RI_METHODV(VPInterior),
	RI_METHODV(VPAtmosphere),

	// v 11
	//RI_METHODV(Procedural2)

	// v 12
	RI_METHOD(VolumePixelSamples),

	// v 13
	RI_METHODV(Bxdf),
	RI_METHODV(Integrator),
	RI_METHODV(Pattern),

	// v 14
	RI_METHODV(LightFilter),

	// v 15
	RI_METHODV(Displace),
	RI_METHODV(SampleFilter),
	RI_METHODV(DisplayFilter),
	RI_METHODV(Light),

	// END
	{NULL, NULL, 0, NULL}
};