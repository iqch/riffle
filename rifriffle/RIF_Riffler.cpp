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
*	Filter implementaion
*/
#include "stdafx.h"

#include "RiCalls.h"

#define DECLARE_CALLBACK(callback) PyObject* _##callback##Func; static RtVoid _##callback

#define PARSE_CALLBACK(callback) _##callback##Func = PyObject_GetAttrString(m_filterobj, #callback); \
	if(PyCallable_Check(_##callback##Func)) ##callback = &Riffler::_##callback; \
else { Py_XDECREF(_##callback##Func); };

#define PARSE_CALLBACKV(callback) _##callback##VFunc = PyObject_GetAttrString(m_filterobj, #callback); \
	if(PyCallable_Check(_##callback##VFunc)) ##callback##V = &Riffler::_##callback##V; \
else Py_XDECREF(_##callback##VFunc);

#define CLEAN_CALLBACK(callback) if(_##callback##Func != NULL) Py_XDECREF(_##callback##Func);

class Riffler : public RifPlugin, public RifFilter
{
public:
	Riffler(int argc, char **argv);
	virtual ~Riffler();

	virtual RifFilter& GetFilter();

	// REFCOUNT PART
	static unsigned int m_counter;
protected:

	PyObject* m_module;
	PyObject* m_filterobj;

private:

	// CATCHERS

	// ONE INT
	DECLARE_CALLBACK(FrameBegin)(RtInt frame);
	DECLARE_CALLBACK(Sides)(RtInt sides);

	// PLAIN
	DECLARE_CALLBACK(FrameEnd)();

	DECLARE_CALLBACK(WorldBegin)();
	DECLARE_CALLBACK(WorldEnd)();

	DECLARE_CALLBACK(AttributeBegin)();
	DECLARE_CALLBACK(AttributeEnd)();

	DECLARE_CALLBACK(TransformBegin)();
	DECLARE_CALLBACK(TransformEnd)();

	DECLARE_CALLBACK(SolidEnd)();

	DECLARE_CALLBACK(MotionEnd)();

	DECLARE_CALLBACK(Identity)();

	DECLARE_CALLBACK(ReverseOrientation)();

	DECLARE_CALLBACK(ArchiveEnd)();

	DECLARE_CALLBACK(Else)();
	DECLARE_CALLBACK(IfEnd)();

	DECLARE_CALLBACK(ResourceBegin)();
	DECLARE_CALLBACK(ResourceEnd)();
	DECLARE_CALLBACK(EditEnd)();
	DECLARE_CALLBACK(EditAttributeEnd)();
	DECLARE_CALLBACK(EditWorldEnd)();

	// ONE FLOAT
	DECLARE_CALLBACK(FrameAspectRatio)(RtFloat aspect);
	DECLARE_CALLBACK(PixelVariance)(RtFloat variation);
	DECLARE_CALLBACK(ShadingRate)(RtFloat size);
	DECLARE_CALLBACK(RelativeDetail)(RtFloat relativedetail);
	DECLARE_CALLBACK(Perspective)(RtFloat fov);

	// ONE COLOR
	DECLARE_CALLBACK(Color)(RtColor color);
	DECLARE_CALLBACK(Opacity)(RtColor color);

	// BOOLEAN
	DECLARE_CALLBACK(Matte)(RtBoolean onoff);

	// STRINGS
	DECLARE_CALLBACK(ShadingInterpolation)(RtToken type);
	DECLARE_CALLBACK(SolidBegin)(RtToken operation);
	DECLARE_CALLBACK(EditAttributeBegin)(RtToken name);
	DECLARE_CALLBACK(Orientation)(RtToken orientation);
	DECLARE_CALLBACK(CoordSysTransform)(RtToken space);
	DECLARE_CALLBACK(CoordinateSystem)(RtToken space);
	DECLARE_CALLBACK(ScopedCoordinateSystem)(RtToken space);
	DECLARE_CALLBACK(System)(RtToken cmdstring);

	// POLY-FLOATS

	// 2
	DECLARE_CALLBACK(Clipping)(RtFloat hither, RtFloat yon);
	DECLARE_CALLBACK(Shutter)(RtFloat min, RtFloat max);
	DECLARE_CALLBACK(Exposure)(RtFloat gain, RtFloat gamma);
	DECLARE_CALLBACK(PixelSamples)(RtFloat xsamples, RtFloat ysamples);

	// 3
	DECLARE_CALLBACK(Scale)(RtFloat sx, RtFloat sy, RtFloat sz);
	DECLARE_CALLBACK(DepthOfField)(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
	DECLARE_CALLBACK(Translate)(RtFloat dx, RtFloat dy, RtFloat dz);

	// 4
	DECLARE_CALLBACK(CropWindow)(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
	DECLARE_CALLBACK(ScreenWindow)(RtFloat left, RtFloat right, RtFloat top, RtFloat bot);
	DECLARE_CALLBACK(DetailRange)(RtFloat minv, RtFloat lowtran, RtFloat uptran, RtFloat maxv);
	DECLARE_CALLBACK(Rotate)(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);

	// 6
	DECLARE_CALLBACK(ClippingPlane)(RtFloat Nx, RtFloat Ny, RtFloat Nz, RtFloat Px, RtFloat Py, RtFloat Pz);

	// 7
	DECLARE_CALLBACK(Skew)(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

	// 8
	DECLARE_CALLBACK(TextureCoordinates)(RtFloat s1, RtFloat t1, 	RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);

	// BOUNDS
	DECLARE_CALLBACK(Bound)(RtBound bound);
	DECLARE_CALLBACK(Detail)(RtBound bound);

	// MATRICIES
	DECLARE_CALLBACK(ConcatTransform)(RtMatrix transform);
	DECLARE_CALLBACK(Transform)(RtMatrix transform);

	// TOKEN-DICTIONARY
	DECLARE_CALLBACK(EditBeginV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(IfBeginV)(RtToken expr, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ElseIfV)(RtToken expr, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ProjectionV)(RtToken name, 	RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(HiderV)(RtToken type, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(OptionV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(AttributeV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(AtmosphereV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(DisplacementV)(RtToken name,	RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ExteriorV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(InteriorV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(SurfaceV)(RtToken name,	RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(GeometryV)(RtToken type, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PatchV)(RtToken type, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(DisplayChannelV)(RtToken mode, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(CameraV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PixelSampleImagerV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(EditWorldBeginV)(RtToken name, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ImagerV)(RtToken name, RtInt, RtToken[], RtPointer[]);

	// DUO-TOKENS/DICT
	DECLARE_CALLBACK(ResourceV)(RtToken handle, RtToken type, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ShaderV)(RtToken name, RtToken handle, RtInt, RtToken[], RtPointer[]);

	// TOKEN/DICT WITH HANDLES
	PyObject* _ArchiveBeginVFunc;
	static RtArchiveHandle _ArchiveBeginV(RtToken name, RtInt, RtToken[], RtPointer[]);

	PyObject* _LightSourceVFunc;
	static RtLightHandle	_LightSourceV(RtToken name, RtInt, RtToken[], RtPointer[]);

	// MISC
	DECLARE_CALLBACK(ReadArchiveV)(RtToken name, RtArchiveCallback callback, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(MotionBeginV)(RtInt n, RtFloat times[]);
	DECLARE_CALLBACK(Format)(RtInt xres, RtInt yres, RtFloat aspect);
	DECLARE_CALLBACK(GeometricApproximation)(RtToken type, RtFloat value);
	DECLARE_CALLBACK(Quantize)(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat ampl);
	DECLARE_CALLBACK(Illuminate)(RtLightHandle light, RtBoolean onoff);
	DECLARE_CALLBACK(DisplayV)(char *name, RtToken type, RtToken mode, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PixelFilter)(RtFilterFunc filterFunc, RtFloat xwidth, RtFloat ywidth);
	DECLARE_CALLBACK(Basis)(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
	DECLARE_CALLBACK(VArchiveRecord)(RtToken type, char *format, va_list vap);

	// MAKERS
	DECLARE_CALLBACK(MakeTextureV)(char *pic, char *tex, 	RtToken swrap, RtToken twrap,	RtFilterFunc filterFunc, RtFloat swidth, RtFloat twidth, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(MakeShadowV)(char *pic, char *tex, 	RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(MakeCubeFaceEnvironmentV)(char *px, char *nx, char *py, char *ny,	char *pz, char *nz, char *tex, RtFloat fov,	RtFilterFunc filterFunc,	RtFloat swidth, RtFloat ywidth, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(MakeLatLongEnvironmentV)(char *pic, char *tex, RtFilterFunc filterFunc,	RtFloat swidth, RtFloat twidth, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(MakeBrickMapV)(RtInt cnt, RtToken* maps, RtToken brickmap, RtInt n, RtToken tk[], RtPointer vl[]);
	
	// GPRIMS
	DECLARE_CALLBACK(SphereV)(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ConeV)(RtFloat height, RtFloat radius, RtFloat tmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(CylinderV)(RtFloat rad, RtFloat zmin, RtFloat zmax, RtFloat tmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(DiskV)(RtFloat height, RtFloat radius, RtFloat tmax,	RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(TorusV)(RtFloat majrad, RtFloat minrad, RtFloat phimin, RtFloat phimax, 	RtFloat tmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(ParaboloidV)(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat tmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(HyperboloidV)(RtPoint point1, RtPoint point2, RtFloat tmax,	RtInt, RtToken[], RtPointer[]);

	DECLARE_CALLBACK(PointsV)(RtInt nverts, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(CurvesV)(RtToken type, RtInt ncurves, RtInt nvert[], RtToken wrap, RtInt, RtToken[], RtPointer[]);

	DECLARE_CALLBACK(NuPatchV)(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PatchMeshV)(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt, RtToken[], RtPointer[]);
	//DECLARE_CALLBACK(TrimCurve)(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat min[], RtFloat max[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

	DECLARE_CALLBACK(PolygonV)(RtInt nverts, RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PointsPolygonsV)(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(GeneralPolygonV)(RtInt nloops, RtInt nverts[], RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(PointsGeneralPolygonsV)(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(SubdivisionMeshV)(RtToken mask, RtInt nf, RtInt nverts[], RtInt verts[], RtInt nt, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtInt, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(HierarchicalSubdivisionMeshV)(RtToken mask, RtInt nf, 	RtInt nverts[], RtInt verts[], RtInt nt, RtToken tags[],	RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], RtInt, RtToken[], RtPointer[]);
	
	DECLARE_CALLBACK(BlobbyV)(RtInt nleaf, RtInt ninst, RtInt inst[], RtInt nflt, RtFloat flt[], RtInt nstr, RtToken str[], RtInt, RtToken[], RtPointer[]);

	static RtVoid _Procedural(RtPointer data, RtBound bound, RtProcSubdivFunc sdfunc, RtProcFreeFunc freefunc);
	PyObject* _DelayedReadArchiveFunc;
	PyObject* _RunProgramFunc;
	PyObject* _DynamicLoadFunc;

	// TO SKIP
	//RtToken 	_Declare(char *name, char *declaration);
	//RtVoid  	_PixelFidelity(RtFloat variation); // OBSOLETE call: see RiPixelVariance
	//RtVoid		_ErrorHandler(RtErrorHandler);
	//RtVoid  	_ObjectInstance(RtObjectHandle handle);
	//DECLARE_CALLBACK(ObjectBegin)();
	//DECLARE_CALLBACK(ObjectEnd)();
	//RtLightHandle	_AreaLightSourceV(RtToken name, RtInt, RtToken[], RtPointer[]); // USELESS

	/* Version 10 added:
	*      Volume
	*      VPSurface,
	*      VPInterior,
	*      VPAtmosphere*/
	DECLARE_CALLBACK(VolumeV)(RtToken type, RtBound, int*, int, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(VPSurfaceV)(RtToken name, int, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(VPInteriorV)(RtToken name, int, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(VPAtmosphereV)(RtToken name, int, RtToken[], RtPointer[]);

	/* Version 11 added:
	*      ObjectBeginV - SKIP
	*      ObjectInstanceV - SKIP
	*      Procedural2V - COMPLICATED */
	DECLARE_CALLBACK(Procedural2V)(RtProc2SubdivFunc sfunc,RtProc2BoundFunc bfunc,int,RtToken[],RtPointer[]);
	
	/* Version 12 added:
	*      VolumePixelSamples*/
	DECLARE_CALLBACK(VolumePixelSamples)(float xsamples, float ysamples);
	
	/* Version 13 added:
	*      BxdfV
	*      IntegratorV
	*      PatternV*/
	DECLARE_CALLBACK(BxdfV)(RtToken name, RtToken handle, int, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(IntegratorV)(RtToken name,RtToken handle,int,RtToken[],RtPointer[]);
	DECLARE_CALLBACK(PatternV)(RtToken name, RtToken handle, int, RtToken[], RtPointer[]);

	/* Version 14 added:
	*      LightFilterV*/
	DECLARE_CALLBACK(LightFilterV)(RtToken name,RtToken handle,int,RtToken[],RtPointer[]);
	//DECLARE_CALLBACK(EnableLightFilter)(RtLightHandle light,RtToken filter,RtBoolean onoff); - NO PARAMETER PARSING

	/* Version 15 added:
	*      DisplaceV
	*      SampleFilterV
	*      DisplayFilterV
	*      LightV
	*/
	DECLARE_CALLBACK(DisplaceV)(RtToken name, RtToken handle, int, RtToken[], RtPointer[]);
	DECLARE_CALLBACK(SampleFilterV)(RtToken name,RtToken handle,int,RtToken[],RtPointer[]);
	DECLARE_CALLBACK(DisplayFilterV)(RtToken name,RtToken handle,int,RtToken[],RtPointer[]);
	DECLARE_CALLBACK(LightV)(RtToken name, RtToken handle, int, RtToken[], RtPointer[]);


};

unsigned int Riffler::m_counter = 0;

Riffler::Riffler(int argc, char **argv)
	: m_module(NULL)
	, m_filterobj(NULL)
{
	// LOAD MODULE
	if(argc < 1)
	{
		cout << "RIFFLER: Insufficient parameters - script name is not found" << endl;
		return;
	};

	//m_module = PyImport_ImportModule(argv[0]);
	m_module = PyImport_ImportModuleNoBlock(argv[0]);

	if(m_module == NULL)
	{
		cout << "RIFFLER: script " << argv[0] << " is not found" << endl;
		return;
	};

	// FIND FILTER OBJECT
	PyObject *pFunc = PyObject_GetAttrString(m_module, "plugin");

	if(pFunc == NULL)
	{
		cout << "RIFFLER: script " << argv[0] << " does not contain 'plugin'-function" << endl;
		if(PyErr_Occurred()) PyErr_Print();
		Py_DECREF(m_module); m_module = NULL;
		return;
	};
	
	if(!PyCallable_Check(pFunc))
	{
		cout << "RIFFLER: script " << argv[0] << " does not contain 'plugin'-function" << endl;
		if(PyErr_Occurred()) PyErr_Print();
		Py_XDECREF(pFunc); // got 2 references
		Py_DECREF(m_module); m_module = NULL;
		return;
	};

	PyObject *pArgs = PyTuple_New(argc-1);

	for(int i=1; i<argc; i++) 
	{
		PyObject *pVal = PyString_FromString(argv[i]);

		if (pVal == NULL)
		{
			cout << "RIFFLER: cannot convert script's parameters" << endl;
			if(PyErr_Occurred()) PyErr_Print();
			Py_DECREF(pArgs);
			Py_XDECREF(pFunc);
			Py_DECREF(m_module); m_module = NULL;
			return;
		}

		PyTuple_SetItem(pArgs, i-1, pVal);
	};

	PyObject *pArgList = PyTuple_New(1);
	PyTuple_SetItem(pArgList, 0, pArgs);

	m_filterobj = PyObject_CallObject(pFunc, pArgList);
	Py_XDECREF(pArgList);
	Py_XDECREF(pFunc);

	if(m_filterobj == NULL)
	{
		cout << "RIFFLER: calling 'plugin' function failed!" << endl;

		PyErr_Print();
		Py_DECREF(m_module); m_module = NULL;
		return;
	};

	// SETUP AS FILTER

	// ONE INT
	PARSE_CALLBACK(FrameBegin)
	PARSE_CALLBACK(Sides)

	// PLAIN
	PARSE_CALLBACK(FrameEnd)
	PARSE_CALLBACK(WorldBegin)
	PARSE_CALLBACK(WorldEnd)

	PARSE_CALLBACK(AttributeBegin);
	PARSE_CALLBACK(AttributeEnd);

	PARSE_CALLBACK(TransformBegin);
	PARSE_CALLBACK(TransformEnd);

	PARSE_CALLBACK(SolidEnd);

	//PARSE_CALLBACK(ObjectBegin);
	//PARSE_CALLBACK(ObjectEnd);

	PARSE_CALLBACK(MotionEnd);

	PARSE_CALLBACK(Identity);

	PARSE_CALLBACK(ReverseOrientation);

	PARSE_CALLBACK(ArchiveEnd);

	PARSE_CALLBACK(Else);
	PARSE_CALLBACK(IfEnd);

	PARSE_CALLBACK(ResourceBegin);
	PARSE_CALLBACK(ResourceEnd);
	PARSE_CALLBACK(EditEnd);
	PARSE_CALLBACK(EditAttributeEnd);
	PARSE_CALLBACK(EditWorldEnd);

	// ONE FLOAT
	PARSE_CALLBACK(FrameAspectRatio);
	PARSE_CALLBACK(PixelVariance);
	PARSE_CALLBACK(ShadingRate);
	PARSE_CALLBACK(RelativeDetail);
	PARSE_CALLBACK(Perspective);

	// ONE COLOR
	PARSE_CALLBACK(Color);
	PARSE_CALLBACK(Opacity);

	// BOOLEAN
	PARSE_CALLBACK(Matte);

	// STRINGS
	PARSE_CALLBACK(ShadingInterpolation);
	PARSE_CALLBACK(SolidBegin);
	PARSE_CALLBACK(EditAttributeBegin);
	PARSE_CALLBACK(Orientation);
	PARSE_CALLBACK(CoordSysTransform);
	PARSE_CALLBACK(CoordinateSystem);
	PARSE_CALLBACK(ScopedCoordinateSystem);
	PARSE_CALLBACK(System);

	// POLY-FLOATS

	// 2
	PARSE_CALLBACK(Clipping);
	PARSE_CALLBACK(Shutter);
	PARSE_CALLBACK(Exposure);
	PARSE_CALLBACK(PixelSamples);

	// 3
	PARSE_CALLBACK(Scale);
	PARSE_CALLBACK(DepthOfField);
	PARSE_CALLBACK(Translate);

	// 4
	PARSE_CALLBACK(CropWindow);
	PARSE_CALLBACK(ScreenWindow);
	PARSE_CALLBACK(DetailRange);
	PARSE_CALLBACK(Rotate);

	// 6
	PARSE_CALLBACK(ClippingPlane);

	// 7
	PARSE_CALLBACK(Skew);

	// 8
	PARSE_CALLBACK(TextureCoordinates);

	// BOUNDS
	PARSE_CALLBACK(Bound);
	PARSE_CALLBACK(Detail);

	// MATRICES
	PARSE_CALLBACK(ConcatTransform);
	PARSE_CALLBACK(Transform);

	// TOKEN-DICTIONARY
	PARSE_CALLBACKV(EditBegin);
	PARSE_CALLBACKV(IfBegin);
	PARSE_CALLBACKV(ElseIf);
	PARSE_CALLBACKV(Projection);
	PARSE_CALLBACKV(Hider);
	PARSE_CALLBACKV(Option);
	PARSE_CALLBACKV(Attribute);
	PARSE_CALLBACKV(Atmosphere);
	PARSE_CALLBACKV(Displacement);
	PARSE_CALLBACKV(Exterior);
	PARSE_CALLBACKV(Interior);
	PARSE_CALLBACKV(Surface);
	PARSE_CALLBACKV(Geometry);
	PARSE_CALLBACKV(Patch);
	PARSE_CALLBACKV(DisplayChannel);
	PARSE_CALLBACKV(Camera);
	PARSE_CALLBACKV(PixelSampleImager);
	PARSE_CALLBACKV(EditWorldBegin);
	PARSE_CALLBACKV(Imager);

	// DUO-TOKENS/DICT
	PARSE_CALLBACKV(Resource);
	PARSE_CALLBACKV(Shader);

	// DICT WITH HANDLES
	PARSE_CALLBACKV(ArchiveBegin);
	PARSE_CALLBACKV(LightSource);

	// MISC
	PARSE_CALLBACKV(ReadArchive);
	PARSE_CALLBACKV(MotionBegin);
	PARSE_CALLBACK(Format);
	PARSE_CALLBACK(GeometricApproximation);
	PARSE_CALLBACK(Quantize);
	PARSE_CALLBACK(Illuminate);
	PARSE_CALLBACK(DisplayV);
	PARSE_CALLBACK(PixelFilter);
	PARSE_CALLBACK(Basis);

	PARSE_CALLBACK(VArchiveRecord);
	
	//_ArchiveRecordFunc = PyObject_GetAttrString(m_filterobj, "ArchiveRecord");
	//if(PyCallable_Check(_ArchiveRecordFunc)) this->ArchiveEnd ArchiveRecord = &Riffler::_ArchiveRecord;
	//else Py_XDECREF(_ArchiveRecordFunc);

	// MAKERS
	PARSE_CALLBACKV(MakeTexture);
	PARSE_CALLBACKV(MakeShadow);
	PARSE_CALLBACKV(MakeCubeFaceEnvironment);
	PARSE_CALLBACKV(MakeLatLongEnvironment);
	PARSE_CALLBACKV(MakeBrickMap);

	// GPRIMS
	PARSE_CALLBACKV(Sphere);
	PARSE_CALLBACKV(Cone);
	PARSE_CALLBACKV(Cylinder);
	PARSE_CALLBACKV(Disk);
	PARSE_CALLBACKV(Torus);
	PARSE_CALLBACKV(Paraboloid);
	PARSE_CALLBACKV(Hyperboloid);
	
	PARSE_CALLBACKV(Points);
	PARSE_CALLBACKV(Curves);

	PARSE_CALLBACKV(NuPatch);
	PARSE_CALLBACKV(PatchMesh);
	//PARSE_CALLBACK(TrimCurve);

	PARSE_CALLBACKV(Polygon);
	PARSE_CALLBACKV(GeneralPolygon);
	PARSE_CALLBACKV(PointsPolygons);
	PARSE_CALLBACKV(PointsGeneralPolygons);

	PARSE_CALLBACKV(SubdivisionMesh);
	PARSE_CALLBACKV(HierarchicalSubdivisionMesh);

	PARSE_CALLBACKV(Blobby);

	// PROCEDURAL
	_DelayedReadArchiveFunc = PyObject_GetAttrString(m_filterobj, "DelayedReadArchive");
	_RunProgramFunc = PyObject_GetAttrString(m_filterobj, "RunProgram");
	_DynamicLoadFunc = PyObject_GetAttrString(m_filterobj, "DynamicLoad");

	int check = (PyCallable_Check(_DelayedReadArchiveFunc)
		+PyCallable_Check(_RunProgramFunc)
		+PyCallable_Check(_DynamicLoadFunc));
	
	if(check != 0) Procedural = &Riffler::_Procedural;

	if(!PyCallable_Check(_DelayedReadArchiveFunc))
	{
		Py_XDECREF(_DelayedReadArchiveFunc);
		_DelayedReadArchiveFunc = NULL;
	};
	if(!PyCallable_Check(_RunProgramFunc))
	{
		Py_XDECREF(_RunProgramFunc);
		_RunProgramFunc = NULL;
	};
	if(!PyCallable_Check(_DynamicLoadFunc))
	{
		Py_XDECREF(_DynamicLoadFunc);
		_DynamicLoadFunc = NULL;
	};

	// v 10
	PARSE_CALLBACKV(Volume);
	PARSE_CALLBACKV(VPSurface);
	PARSE_CALLBACKV(VPInterior);
	PARSE_CALLBACKV(VPAtmosphere);

	// v 11
	PARSE_CALLBACKV(Procedural2);

	// v 12
	PARSE_CALLBACK(VolumePixelSamples);

	// v 13
	PARSE_CALLBACKV(Bxdf);
	PARSE_CALLBACKV(Integrator);
	PARSE_CALLBACKV(Pattern);

	// v 14
	PARSE_CALLBACKV(LightFilter); 
	//PARSE_CALLBACK(EnableLightFilter);

	// v 15
	PARSE_CALLBACKV(Displace)
	PARSE_CALLBACKV(SampleFilter)
	PARSE_CALLBACKV(DisplayFilter)
	PARSE_CALLBACKV(Light)

};

Riffler::~Riffler()
{
	// CLEANUP
	if(m_filterobj != NULL)
	{
		// PLAIN
		CLEAN_CALLBACK(FrameEnd)
		CLEAN_CALLBACK(WorldBegin)
		CLEAN_CALLBACK(WorldEnd)

		CLEAN_CALLBACK(AttributeBegin);
		CLEAN_CALLBACK(AttributeEnd);

		CLEAN_CALLBACK(TransformBegin);
		CLEAN_CALLBACK(TransformEnd);

		CLEAN_CALLBACK(SolidEnd);

		//CLEAN_CALLBACK(ObjectBegin);
		//CLEAN_CALLBACK(ObjectEnd);

		CLEAN_CALLBACK(MotionEnd);

		CLEAN_CALLBACK(Identity);

		CLEAN_CALLBACK(ReverseOrientation);

		CLEAN_CALLBACK(ArchiveEnd);

		CLEAN_CALLBACK(Else);
		CLEAN_CALLBACK(IfEnd);

		CLEAN_CALLBACK(ResourceBegin);
		CLEAN_CALLBACK(ResourceEnd);
		CLEAN_CALLBACK(EditEnd);
		CLEAN_CALLBACK(EditAttributeEnd);
		CLEAN_CALLBACK(EditWorldEnd);

		// ONE INT
		CLEAN_CALLBACK(FrameBegin)
		CLEAN_CALLBACK(Sides)

		// ONE FLOAT
		CLEAN_CALLBACK(FrameAspectRatio);
		CLEAN_CALLBACK(PixelVariance);
		CLEAN_CALLBACK(ShadingRate);
		CLEAN_CALLBACK(RelativeDetail);
		CLEAN_CALLBACK(Perspective);

		// ONE COLOR
		CLEAN_CALLBACK(Color);
		CLEAN_CALLBACK(Opacity);

		// BOOLEAN
		CLEAN_CALLBACK(Matte);

		// STRINGS
		CLEAN_CALLBACK(ShadingInterpolation);
		CLEAN_CALLBACK(SolidBegin);
		CLEAN_CALLBACK(EditAttributeBegin);
		CLEAN_CALLBACK(Orientation);
		CLEAN_CALLBACK(CoordSysTransform);
		CLEAN_CALLBACK(CoordinateSystem);
		CLEAN_CALLBACK(ScopedCoordinateSystem);
		CLEAN_CALLBACK(System);

		// POLY-FLOATS

		// 2
		CLEAN_CALLBACK(Clipping);
		CLEAN_CALLBACK(Shutter);
		CLEAN_CALLBACK(Exposure);
		CLEAN_CALLBACK(PixelSamples);

		// 3
		CLEAN_CALLBACK(Scale);
		CLEAN_CALLBACK(DepthOfField);
		CLEAN_CALLBACK(Translate);

		// 4
		CLEAN_CALLBACK(CropWindow);
		CLEAN_CALLBACK(ScreenWindow);
		CLEAN_CALLBACK(DetailRange);
		CLEAN_CALLBACK(Rotate);

		// 6
		CLEAN_CALLBACK(ClippingPlane);

		// 7
		CLEAN_CALLBACK(Skew);

		// 8
		CLEAN_CALLBACK(TextureCoordinates);

		// BOUNDS
		CLEAN_CALLBACK(Bound);
		CLEAN_CALLBACK(Detail);

		// MATRICES
		CLEAN_CALLBACK(ConcatTransform);
		CLEAN_CALLBACK(Transform);

		// TOKEN-DICTIONARY
		CLEAN_CALLBACK(EditBeginV);
		CLEAN_CALLBACK(IfBeginV);
		CLEAN_CALLBACK(ElseIfV);
		CLEAN_CALLBACK(ProjectionV);
		CLEAN_CALLBACK(HiderV);
		CLEAN_CALLBACK(OptionV);
		CLEAN_CALLBACK(AttributeV);
		CLEAN_CALLBACK(AtmosphereV);
		CLEAN_CALLBACK(DisplacementV);
		CLEAN_CALLBACK(ExteriorV);
		CLEAN_CALLBACK(InteriorV);
		CLEAN_CALLBACK(SurfaceV);
		CLEAN_CALLBACK(GeometryV);
		CLEAN_CALLBACK(PatchV);
		CLEAN_CALLBACK(DisplayChannelV);
		CLEAN_CALLBACK(CameraV);
		CLEAN_CALLBACK(PixelSampleImagerV);
		CLEAN_CALLBACK(EditWorldBeginV);
		CLEAN_CALLBACK(ImagerV);

		// DUO-TOKENS/DICT
		CLEAN_CALLBACK(ResourceV);
		CLEAN_CALLBACK(ShaderV);

		// DICT WITH HANDLES
		CLEAN_CALLBACK(ArchiveBeginV);
		CLEAN_CALLBACK(LightSourceV);

		// MISC
		CLEAN_CALLBACK(ReadArchiveV);
		CLEAN_CALLBACK(MotionBeginV);
		CLEAN_CALLBACK(Format);
		CLEAN_CALLBACK(GeometricApproximation);
		CLEAN_CALLBACK(Quantize);
		CLEAN_CALLBACK(Illuminate);
		CLEAN_CALLBACK(DisplayV);
		CLEAN_CALLBACK(PixelFilter);
		CLEAN_CALLBACK(Basis);
		CLEAN_CALLBACK(VArchiveRecord);

		// MAKERS
		CLEAN_CALLBACK(MakeTextureV);
		CLEAN_CALLBACK(MakeShadowV);
		CLEAN_CALLBACK(MakeCubeFaceEnvironmentV);
		CLEAN_CALLBACK(MakeLatLongEnvironmentV);
		CLEAN_CALLBACK(MakeBrickMapV);

		// GPRIMS
		CLEAN_CALLBACK(SphereV);
		CLEAN_CALLBACK(ConeV);
		CLEAN_CALLBACK(CylinderV);
		CLEAN_CALLBACK(DiskV);
		CLEAN_CALLBACK(TorusV);
		CLEAN_CALLBACK(ParaboloidV);
		CLEAN_CALLBACK(HyperboloidV);

		CLEAN_CALLBACK(PointsV);
		CLEAN_CALLBACK(CurvesV);

		CLEAN_CALLBACK(NuPatchV);
		CLEAN_CALLBACK(PatchMeshV);
		//CLEAN_CALLBACK(TrimCurve);

		CLEAN_CALLBACK(PolygonV);
		CLEAN_CALLBACK(PointsPolygonsV);
		CLEAN_CALLBACK(GeneralPolygonV);
		CLEAN_CALLBACK(PointsGeneralPolygonsV);

		CLEAN_CALLBACK(SubdivisionMeshV);
		CLEAN_CALLBACK(HierarchicalSubdivisionMeshV);

		CLEAN_CALLBACK(BlobbyV);

		// PROCEDURAL
		CLEAN_CALLBACK(DelayedReadArchive);
		CLEAN_CALLBACK(RunProgram);
		CLEAN_CALLBACK(DynamicLoad);

		// v 10
		CLEAN_CALLBACK(VolumeV);
		CLEAN_CALLBACK(VPSurfaceV);
		CLEAN_CALLBACK(VPInteriorV);
		CLEAN_CALLBACK(VPAtmosphereV);

		// v 11
		CLEAN_CALLBACK(Procedural2V);

		// v 12
		CLEAN_CALLBACK(VolumePixelSamples);

		// v 13
		CLEAN_CALLBACK(BxdfV);
		CLEAN_CALLBACK(IntegratorV);
		CLEAN_CALLBACK(PatternV);

		// v 14
		CLEAN_CALLBACK(LightFilterV);
		//PARSE_CALLBACK(EnableLightFilter);

		// v 15
		CLEAN_CALLBACK(DisplaceV)
		CLEAN_CALLBACK(SampleFilterV)
		CLEAN_CALLBACK(DisplayFilterV)
		CLEAN_CALLBACK(LightV)

	};

	// DESTROY
	if(m_filterobj != NULL) Py_XDECREF(m_filterobj);
	if(m_module != NULL) Py_XDECREF(m_module);

	// HOPING, RIF FILTERS ARE DESTROYED NOT IN PARALLEL
	Riffler::m_counter--;
	if(m_counter == 0)
	{
		Py_Finalize();
	}
};

RifFilter& Riffler::GetFilter()
{
	return *this;
};

extern "C" 
{
	DLLEXPORT RifPlugin* RifPluginManufacture(int argc, char **argv)
	{
		if(Riffler::m_counter == 0)
		{
			// PRIMARY SETUP
			Py_Initialize();

			if(Py_IsInitialized() == 0)
			{
				cout << "RIFFLER: python initialization failed!" << endl;
				return NULL;
			}
			Py_InitModule("riffler_prman", MethodTable);
			Riffler::m_counter++;
		};

		//for(int i=0;i<argc;i++)
		//{
		//	cout << i << ": \t" << argv[i] << endl;
		//};

		return new Riffler(argc,argv);
	};
};

#define CALLBACKFN(callback)RtVoid Riffler::_##callback

#define GETFILTER Riffler* filter = static_cast<Riffler*>(RifGetCurrentPlugin());

#define FORWARD_PLAIN(callback) CALLBACKFN(callback)() { GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, NULL); Py_XDECREF(pResult); };

FORWARD_PLAIN(FrameEnd)

FORWARD_PLAIN(WorldBegin)
FORWARD_PLAIN(WorldEnd)

FORWARD_PLAIN(AttributeBegin);
FORWARD_PLAIN(AttributeEnd);

FORWARD_PLAIN(TransformBegin);
FORWARD_PLAIN(TransformEnd);

FORWARD_PLAIN(SolidEnd);

//FORWARD_PLAIN(ObjectBegin);
//FORWARD_PLAIN(ObjectEnd);

FORWARD_PLAIN(MotionEnd);

FORWARD_PLAIN(Identity);

FORWARD_PLAIN(ReverseOrientation);

FORWARD_PLAIN(ArchiveEnd);

FORWARD_PLAIN(Else);
FORWARD_PLAIN(IfEnd);

FORWARD_PLAIN(ResourceBegin);
FORWARD_PLAIN(ResourceEnd);
FORWARD_PLAIN(EditEnd);
FORWARD_PLAIN(EditAttributeEnd);
FORWARD_PLAIN(EditWorldEnd);

#define FORWARD_INT(callback) CALLBACKFN(callback)(RtInt value) { GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "i", value); Py_XDECREF(pResult); };

FORWARD_INT(FrameBegin)
FORWARD_INT(Sides)

#define FORWARD_FLOAT(callback) CALLBACKFN(callback)(RtFloat value) { GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "f", value); Py_XDECREF(pResult); };

FORWARD_FLOAT(FrameAspectRatio);
FORWARD_FLOAT(PixelVariance);
FORWARD_FLOAT(ShadingRate);
FORWARD_FLOAT(RelativeDetail);
FORWARD_FLOAT(Perspective);

#define FORWARD_COLOR(callback) CALLBACKFN(callback)(RtColor v) \
{ \
	PyObject* pfArgs = Py_BuildValue("(fff)",v[0],v[1],v[2]); \
	PyObject* pArgs = PyTuple_New(1); \
	PyTuple_SetItem(pArgs, 0, pfArgs); \
	GETFILTER; \
	PyObject* pResult = PyObject_CallObject(filter->_##callback##Func, pArgs); \
	Py_XDECREF(pResult); \
	Py_XDECREF(pArgs); \
};

FORWARD_COLOR(Color);
FORWARD_COLOR(Opacity);

// BOOLEAN
CALLBACKFN(Matte)(RtBoolean onoff)
{
	GETFILTER;
	PyObject* pResult = PyObject_CallFunction(filter->_MatteFunc, "i", (onoff == 0 ? 0 : 1));
	Py_XDECREF(pResult);
};

// STRINGS
#define FORWARD_STRING(callback) CALLBACKFN(callback)(RtToken v) { GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "s", v); Py_XDECREF(pResult); };

FORWARD_STRING(ShadingInterpolation);
FORWARD_STRING(SolidBegin);
FORWARD_STRING(EditAttributeBegin);
FORWARD_STRING(Orientation);
FORWARD_STRING(CoordSysTransform);
FORWARD_STRING(CoordinateSystem);
FORWARD_STRING(ScopedCoordinateSystem);
FORWARD_STRING(System);

// POLY-FLOATS

#define POLY_FLOATS2(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "ff", v1, v2);	Py_XDECREF(pResult); };

POLY_FLOATS2(Clipping);
POLY_FLOATS2(Shutter);
POLY_FLOATS2(Exposure);
POLY_FLOATS2(PixelSamples);

POLY_FLOATS2(VolumePixelSamples);

#define POLY_FLOATS3(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2, RtFloat v3) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "fff", v1, v2, v3);	Py_XDECREF(pResult); };

POLY_FLOATS3(Scale);
POLY_FLOATS3(DepthOfField);
POLY_FLOATS3(Translate);

#define POLY_FLOATS4(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2, RtFloat v3, RtFloat v4) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "ffff", v1, v2, v3, v4);	Py_XDECREF(pResult); };

POLY_FLOATS4(CropWindow);
POLY_FLOATS4(ScreenWindow);
POLY_FLOATS4(DetailRange);
POLY_FLOATS4(Rotate);

#define POLY_FLOATS6(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2, RtFloat v3, RtFloat v4, RtFloat v5, RtFloat v6) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "ffffff", v1, v2, v3, v4, v5, v6);	Py_XDECREF(pResult); };

POLY_FLOATS6(ClippingPlane);

#define POLY_FLOATS7(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2, RtFloat v3, RtFloat v4, RtFloat v5, RtFloat v6, RtFloat v7) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "fffffff", v1, v2, v3, v4, v5, v6, v7);	Py_XDECREF(pResult); };

POLY_FLOATS7(Skew);

#define POLY_FLOATS8(callback) CALLBACKFN(callback)(RtFloat v1, RtFloat v2, RtFloat v3, RtFloat v4, RtFloat v5, RtFloat v6, RtFloat v7, RtFloat v8) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "ffffffff", v1, v2, v3, v4, v5, v6, v7, v8);	Py_XDECREF(pResult); };

POLY_FLOATS8(TextureCoordinates);

// BOUNDS
#define BOUNDS(callback) CALLBACKFN(callback)(RtBound b) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "((ff)(ff)(ff))",b[0],b[1],b[2],b[3],b[4],b[5]);	Py_XDECREF(pResult); };

BOUNDS(Bound);
BOUNDS(Detail);

// MATRICES

#define MATRICES(callback) CALLBACKFN(callback)(RtMatrix b) \
{ GETFILTER; PyObject* pResult = PyObject_CallFunction(filter->_##callback##Func, "((ffff)(ffff)(ffff)(ffff))", \
	b[0][0],b[0][1],b[0][2],b[0][3],	\
	b[1][0],b[1][1],b[1][2],b[1][3],	\
	b[2][0],b[2][1],b[2][2],b[2][3],	\
	b[3][0],b[3][1],b[3][2],b[3][3]);	Py_XDECREF(pResult); };

MATRICES(ConcatTransform);
MATRICES(Transform);

// TOKEN-DICTIONARY
bool ParseDictionary(PyObject* dict, int n, RtToken tk[], RtPointer vl[]);

#define DICT RtInt n, RtToken tk[], RtPointer vl[]

#define CALLV(method,part) PyObject* pDict = PyDict_New();	\
PyTuple_SetItem(pArgs, part, pDict); \
if(ParseDictionary(pDict, n, tk, vl)) \
{ \
	GETFILTER; \
	PyObject* pResult = PyObject_CallObject(filter->_##method##Func, pArgs); \
	Py_XDECREF(pResult); \
}; \
Py_XDECREF(pArgs);

#define TOKEN_DICTIONARY(callback) CALLBACKFN(callback)(RtToken name, DICT) \
{ \
	PyObject* pArgs = PyTuple_New(2); \
	PyObject* pName = Py_BuildValue("s",name);	\
	PyTuple_SetItem(pArgs, 0, pName); \
	CALLV(##callback##,1) \
};

//CALLBACKFN(AttributeV)(RtToken name, RtInt n, RtToken tk[], RtPointer vl[])
//{
//	PyObject* pArgs = PyTuple_New(2);
//	PyObject* pName = Py_BuildValue("s",name);
//	PyTuple_SetItem(pArgs, 0, pName);
//	PyObject* pDict = PyDict_New();
//	PyTuple_SetItem(pArgs, 1, pDict);
//	ParseDictionary(pDict, n, tk, vl);
//	GETFILTER;
//	PyObject* pResult = PyObject_CallObject(filter->_AttributeVFunc, pArgs);
//	Py_XDECREF(pResult);
//	Py_XDECREF(pArgs);
//};

TOKEN_DICTIONARY(AttributeV);
TOKEN_DICTIONARY(EditBeginV);
TOKEN_DICTIONARY(IfBeginV);
TOKEN_DICTIONARY(ElseIfV);
TOKEN_DICTIONARY(ProjectionV);
TOKEN_DICTIONARY(HiderV);
TOKEN_DICTIONARY(OptionV);
TOKEN_DICTIONARY(AtmosphereV);
TOKEN_DICTIONARY(DisplacementV);
TOKEN_DICTIONARY(ExteriorV);
TOKEN_DICTIONARY(InteriorV);
TOKEN_DICTIONARY(SurfaceV);
TOKEN_DICTIONARY(GeometryV);
TOKEN_DICTIONARY(PatchV);
TOKEN_DICTIONARY(DisplayChannelV);
TOKEN_DICTIONARY(CameraV);
TOKEN_DICTIONARY(PixelSampleImagerV);
TOKEN_DICTIONARY(EditWorldBeginV);
TOKEN_DICTIONARY(ImagerV);

// v 10
TOKEN_DICTIONARY(VPSurfaceV);
TOKEN_DICTIONARY(VPInteriorV);
TOKEN_DICTIONARY(VPAtmosphereV);

// DUO-TOKENS/DICT
#define DUO_TOKEN_DICTIONARY(callback) CALLBACKFN(callback)(RtToken one, RtToken two, DICT) \
{ \
	PyObject* pArgs = PyTuple_New(3); \
	PyObject* pOne = Py_BuildValue("s",one);	\
	PyTuple_SetItem(pArgs, 0, pOne); \
	PyObject* pTwo = Py_BuildValue("s",two);	\
	PyTuple_SetItem(pArgs, 1, pTwo); \
	CALLV(##callback##,2) \
};

DUO_TOKEN_DICTIONARY(ResourceV);
DUO_TOKEN_DICTIONARY(ShaderV);

// v 13
DUO_TOKEN_DICTIONARY(BxdfV);
DUO_TOKEN_DICTIONARY(IntegratorV);
DUO_TOKEN_DICTIONARY(PatternV);

// v 14
DUO_TOKEN_DICTIONARY(LightFilterV);

// v 15
DUO_TOKEN_DICTIONARY(DisplaceV);
DUO_TOKEN_DICTIONARY(SampleFilterV);
DUO_TOKEN_DICTIONARY(DisplayFilterV);
DUO_TOKEN_DICTIONARY(LightV);

RtArchiveHandle Riffler::_ArchiveBeginV(RtToken name, DICT)
{
	PyObject* pArgs = PyTuple_New(2);
	PyObject* pName = Py_BuildValue("s",name);
	PyTuple_SetItem(pArgs, 0, pName);
	CALLV(ArchiveBeginV,1)

	// STUB
	for(int i=0;i<n;i++)
	{
		if(strcmp(tk[i],"__handleid") == 0)
		{
			RtArchiveHandle h = strdup((RtToken)vl[i]);
			return h;
		};
	};
	
	RtArchiveHandle h = strdup(name);
	return h;
};

RtLightHandle Riffler::_LightSourceV(RtToken name, DICT)
{
	PyObject* pArgs = PyTuple_New(2);
	PyObject* pName = Py_BuildValue("s",name);
	PyTuple_SetItem(pArgs, 0, pName);
	CALLV(LightSourceV,1)

	// STUB
	for(int i=0;i<n;i++)
	{
		if(strcmp(tk[i],"__handleid") == 0)
		{
			RtLightHandle h = strdup((RtToken)vl[i]);
			return h;
		};
	};
	RtLightHandle h = strdup(name);
	return h;
};

// MISC FUNCTIONS
RtVoid Riffler::_ReadArchiveV(RtToken name, RtArchiveCallback callback, DICT)
{
	PyObject* pArgs = PyTuple_New(2);
	PyObject* pName = Py_BuildValue("s",name);
	PyTuple_SetItem(pArgs, 0, pName);
	CALLV(ReadArchiveV,1)
};

RtVoid Riffler::_MotionBeginV(RtInt n, RtFloat times[])
{
	PyObject* pArgs = PyTuple_New(2);
	
	PyObject* pN = Py_BuildValue("i",n);
	PyTuple_SetItem(pArgs, 0, pN);
	
	PyObject* pT = PyTuple_New(n);
	for(int i=0;i<n;i++) PyTuple_SetItem(pT,i,Py_BuildValue("f",times[i]));
	PyTuple_SetItem(pArgs, 1, pT);

	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_MotionBeginVFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_Format(RtInt xres, RtInt yres, RtFloat aspect)
{
	PyObject* pArgs = Py_BuildValue("(iif)",xres,yres,aspect);
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_FormatFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_GeometricApproximation(RtToken type, RtFloat value)
{
	PyObject* pArgs = Py_BuildValue("(sf)",type,value);
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_GeometricApproximationFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_Quantize(RtToken type, RtInt one, RtInt _min, RtInt _max, RtFloat ampl)
{
	PyObject* pArgs = Py_BuildValue("(siiif)",type,one,_min,_max,ampl);
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_QuantizeFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_Illuminate(RtLightHandle light, RtBoolean onoff)
{
	RtInt vl = (onoff == 0 ? 0 : 1);
	PyObject* pArgs = Py_BuildValue("(si)",(char*)light,vl);
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_IlluminateFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_DisplayV(char *name, RtToken type, RtToken mode, DICT)
{
	PyObject* pArgs = PyTuple_New(4);

	PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",name));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",type));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",mode));
	CALLV(DisplayV,3)
};

PyObject* getFilterName(RtFilterFunc filterFunc)
{
	if(filterFunc == RiBesselFilter) return Py_BuildValue("s","bessel");
	else if(filterFunc == RiBlackmanHarrisFilter) return Py_BuildValue("s","blackman-harris");
	else if(filterFunc == RiCatmullRomFilter) return Py_BuildValue("s","catmull-rom");
	else if(filterFunc == RiDiskFilter) return Py_BuildValue("s","disk");
	else if(filterFunc == RiGaussianFilter) return Py_BuildValue("s","gaussian");
	else if(filterFunc == RiMitchellFilter) return Py_BuildValue("s","mitchell");
	else if(filterFunc == RiLanczosFilter) return Py_BuildValue("s","lanczos");
	else if(filterFunc == RiSeparableCatmullRomFilter) return Py_BuildValue("s","separable-catmull-rom");
	else if(filterFunc == RiSincFilter) return Py_BuildValue("s","sinc");
	else if(filterFunc == RiTriangleFilter) return Py_BuildValue("s","triangle");
	else /*if(filterFunc == RiBoxFilter) and default*/ return Py_BuildValue("s","box");
}

RtVoid Riffler::_PixelFilter(RtFilterFunc filterFunc, RtFloat xwidth, RtFloat ywidth)
{
	PyObject* pArgs = PyTuple_New(3);
	PyTuple_SetItem(pArgs, 0, getFilterName(filterFunc));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("f",xwidth));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("f",ywidth));
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_PixelFilterFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_Basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	PyObject* uname = NULL;
	if(ubasis == RiBezierBasis) uname = Py_BuildValue("s","bezier");
	else if(ubasis == RiBSplineBasis) uname = Py_BuildValue("s","spline");
	else if(ubasis == RiCatmullRomBasis) uname = Py_BuildValue("s","catmull-rom");
	else if(ubasis == RiHermiteBasis) uname = Py_BuildValue("s","hermite");
	else if(ubasis == RiPowerBasis) uname = Py_BuildValue("s","power");
	else return;

	PyObject* vname = NULL;
	if(vbasis == RiBezierBasis) vname = Py_BuildValue("s","bezier");
	else if(vbasis == RiBSplineBasis) vname = Py_BuildValue("s","spline");
	else if(vbasis == RiCatmullRomBasis) vname = Py_BuildValue("s","catmull-rom");
	else if(vbasis == RiHermiteBasis) vname = Py_BuildValue("s","hermite");
	else if(vbasis == RiPowerBasis) vname = Py_BuildValue("s","power");
	else return;

	PyObject* pArgs = PyTuple_New(4);
	PyTuple_SetItem(pArgs, 0, uname);
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("f",ustep));
	PyTuple_SetItem(pArgs, 2, vname);
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("f",vstep));
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_BasisFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_VArchiveRecord(RtToken type, char *format, va_list vlist)
{
	//cout << "VLL : " << vlist << endl;

	char buffer[512];
	vsnprintf(buffer, 512, format, vlist);

	PyObject* pArgs = Py_BuildValue("(sss)",type,format,buffer);
	
	GETFILTER;
	PyObject* pResult = PyObject_CallObject(filter->_VArchiveRecordFunc, pArgs);

	Py_XDECREF(pResult);
	Py_XDECREF(pArgs);
};

RtVoid Riffler::_MakeTextureV(char *pic, char *tex, 	RtToken swrap, RtToken twrap,	RtFilterFunc filterFunc, RtFloat swidth, RtFloat twidth, DICT)
{
	PyObject* pArgs = PyTuple_New(6);

	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",pic));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("s",tex));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("(ss)",swrap,twrap));
	PyTuple_SetItem(pArgs,3,getFilterName(filterFunc));
	PyTuple_SetItem(pArgs,4,Py_BuildValue("(ff)",swidth,twidth));
	CALLV(MakeTextureV,5)
};

RtVoid Riffler::_MakeShadowV(char *pic, char *tex, DICT)
{
	PyObject* pArgs = PyTuple_New(3);

	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",pic));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("s",tex));
	CALLV(MakeShadowV,2)
};

RtVoid Riffler::_MakeCubeFaceEnvironmentV(
	char *px, char *nx, char *py, char *ny, char *pz, char *nz, char *tex, 
	RtFloat fov,	 RtFilterFunc filterFunc,	RtFloat swidth, RtFloat ywidth, DICT)
{
	PyObject* pArgs = PyTuple_New(6);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("(ssssss(",px,nx,py,ny,pz,nz));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("s",tex));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",fov));
	PyTuple_SetItem(pArgs,3,getFilterName(filterFunc));
	PyTuple_SetItem(pArgs,4,Py_BuildValue("(ff)",swidth,ywidth));
	CALLV(MakeTextureV,5)
};

RtVoid Riffler::_MakeLatLongEnvironmentV(char *pic, char *tex, RtFilterFunc filterFunc, RtFloat swidth, RtFloat twidth, DICT)
{
	PyObject* pArgs = PyTuple_New(5);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",pic));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("s",tex));
	PyTuple_SetItem(pArgs,2,getFilterName(filterFunc));
	PyTuple_SetItem(pArgs,3,Py_BuildValue("(ff)",swidth,twidth));
	CALLV(MakeLatLongEnvironmentV,4)
};

RtVoid Riffler::_MakeBrickMapV(RtInt cnt, RtToken* maps, RtToken brickmap, DICT)
{
	if(cnt < 1) return;

	PyObject* pArgs = PyTuple_New(3);

	PyObject* pM = PyTuple_New(cnt);
	for(int i=0;i<cnt;i++) PyTuple_SetItem(pM,i,Py_BuildValue("s",maps[i]));

	PyTuple_SetItem(pArgs,0,pM);
	PyTuple_SetItem(pArgs,1,Py_BuildValue("s",brickmap));
	CALLV(MakeBrickMapV,2)
};

// GPRIMS
RtVoid Riffler::_SphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, DICT)
{
	PyObject* pArgs = PyTuple_New(5);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",radius));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",zmin));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",zmax));
	PyTuple_SetItem(pArgs,3,Py_BuildValue("f",tmax));
	CALLV(SphereV,4)
};

RtVoid Riffler::_ConeV(RtFloat height, RtFloat radius, RtFloat tmax, DICT)
{
	PyObject* pArgs = PyTuple_New(4);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",height));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",radius));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",tmax));
	CALLV(ConeV,3)
};

RtVoid Riffler::_CylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, DICT)
{
	PyObject* pArgs = PyTuple_New(5);

	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",radius));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",zmin));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",zmax));
	PyTuple_SetItem(pArgs,3,Py_BuildValue("f",tmax));
	CALLV(CylinderV,4)
};

RtVoid Riffler::_DiskV(RtFloat height, RtFloat radius, RtFloat tmax, DICT)
{
	PyObject* pArgs = PyTuple_New(4);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",height));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",radius));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",tmax));
	CALLV(DiskV,3)
};

RtVoid Riffler::_TorusV(RtFloat majrad, RtFloat minrad, RtFloat phimin, RtFloat phimax, RtFloat tmax, DICT)
{
	PyObject* pArgs = PyTuple_New(6);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",majrad));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",minrad));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",phimin));
	PyTuple_SetItem(pArgs,3,Py_BuildValue("f",phimax));
	PyTuple_SetItem(pArgs,4,Py_BuildValue("f",tmax));
	CALLV(TorusV,5)
};

RtVoid Riffler::_ParaboloidV(RtFloat a1, RtFloat a2, RtFloat a3, RtFloat a4, DICT)
{
	PyObject* pArgs = PyTuple_New(5);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("f",a1));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("f",a2));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",a3));
	PyTuple_SetItem(pArgs,3,Py_BuildValue("f",a4));
	CALLV(ParaboloidV,4)
};

inline PyObject* getAPoint(RtPoint p)
{
	return Py_BuildValue("(fff)",p[0],p[1],p[2]);
};

RtVoid Riffler::_HyperboloidV(RtPoint point1, RtPoint point2,RtFloat a1, DICT)
{
	PyObject* pArgs = PyTuple_New(4);
	PyTuple_SetItem(pArgs,0,getAPoint(point1));
	PyTuple_SetItem(pArgs,1,getAPoint(point2));
	PyTuple_SetItem(pArgs,2,Py_BuildValue("f",a1));
	CALLV(HyperboloidV,3)
};

bool ParseDictionaryUVVF(PyObject* dict, DICT, int uniform, int varying, int vertex, int facevarying);

#define CALLCV(method,part) PyObject* pDict = PyDict_New();	\
	PyTuple_SetItem(pArgs, part, pDict); \
	if(ParseDictionaryUVVF(pDict, n, tk, vl,uniform,varying,vertex,facevarying)) \
{ \
	GETFILTER; \
	PyObject* pResult = PyObject_CallObject(filter->_##method##Func, pArgs); \
	Py_XDECREF(pResult); \
	}; \
	Py_XDECREF(pArgs);

RtVoid Riffler::_PointsV(RtInt nverts, DICT)
{
	PyObject* pArgs = PyTuple_New(2);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("i",nverts));
	int uniform = 1; int varying = nverts; int vertex = nverts; int facevarying = nverts;
	CALLCV(PointsV,1)
};

RtVoid Riffler::_CurvesV(RtToken type, RtInt ncurves, RtInt nvert[], RtToken wrap, DICT)
{
	PyObject* pArgs = PyTuple_New(3);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("(ss)",type,wrap));
	int uniform = ncurves; int varying = -1; int vertex = 0; int facevarying = -1;

	PyObject* pP = PyTuple_New(ncurves);
	for(int i=0;i<ncurves;i++)
	{
		PyTuple_SetItem(pP,i,Py_BuildValue("i",nvert[i]));
		vertex+=nvert[i];
	};
	PyTuple_SetItem(pArgs,1,pP);

	CALLCV(CurvesV,2)
};

RtVoid Riffler::_NuPatchV(
						  RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax,
						  RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, DICT)
{
	PyObject* pArgs = PyTuple_New(3);
	
	// U PART
	{
		int usegs = nu+uorder;
		PyObject* pK = PyTuple_New(usegs);
		for(int i=0;i<usegs;i++) PyTuple_SetItem(pK,i,Py_BuildValue("i",uknot[i]));
		PyTuple_SetItem(pArgs,0,Py_BuildValue("(iO(ff))",uorder,pK,umin,umax));
	}

	// V PART
	{
		int vsegs = nv+vorder;
		PyObject* pK = PyTuple_New(vsegs);
		for(int i=0;i<vsegs;i++) PyTuple_SetItem(pK,i,Py_BuildValue("i",vknot[i]));
		PyTuple_SetItem(pArgs,1,Py_BuildValue("(iO(ff))",vorder,pK,vmin,vmax));
	}

	// DATA
	int uniform = (1+nu-uorder)*(1+nv-vorder);
	int varying = (2+nu-uorder)*(2+nv-vorder); // ...NOT SURE!
	int vertex = nu*nv; int facevarying = -1; //4*uniform; ?

	CALLCV(NuPatchV,3)
};

RtVoid Riffler::_PatchMeshV(RtToken type, 
		RtInt nu, RtToken uwrap, 
		RtInt nv, RtToken vwrap, DICT)
{
	PyObject* pArgs = PyTuple_New(3);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",type));
	PyTuple_SetItem(pArgs,1,Py_BuildValue("((is)(is))",nu,uwrap,nv,vwrap));

	int usteps = 0;
	int vsteps = 0;
	RifGetCurrentBasisSteps(&usteps, &vsteps);

	int up = 0; int vp = 0;

	bool isup = (strcmp(RI_PERIODIC,uwrap) == 0);
	bool isvp = (strcmp(RI_PERIODIC,vwrap) == 0);

	if(strcmp(type,RI_BILINEAR) == 0)
	{
		up = nu; if(isup) up--;
		vp = nv; if(isvp) vp--;
	}
	else
	{
		up = (isup ? nu/usteps : (nu-4/usteps)+1);
		vp = (isvp ? nv/vsteps : (nv-4/vsteps)+1);
	};

	int uniform = 1; 
	int varying = (up+(isup ? 0 : 1))*(vp+(isvp ? 0 : 1)); 
	int vertex = nu*nv; 
	int facevarying = -1; // ...JUST DONT KNOW - may be varying*4

	CALLCV(PatchMeshV,2)
};

//RtVoid Riffler::_TrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat min[], RtFloat max[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
//{
// ...TOO COMPLEX TO BE DONE RIGHT NOW
//};

RtVoid Riffler::_PolygonV(RtInt nverts, DICT)
{
	PyObject* pArgs = PyTuple_New(2);
	PyTuple_SetItem(pArgs,0,Py_BuildValue("i",nverts));
	int uniform = 1; int varying = nverts; int vertex = nverts; int facevarying = nverts;
	CALLCV(PolygonV,1)
};

RtVoid Riffler::_PointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], DICT)
{
	if(npolys < 1) return;

	PyObject* pArgs = PyTuple_New(2);

	int uniform = npolys; int varying = 0; int vertex = 0; int facevarying = 0;

	int* vtx = verts;
	PyObject* pP = PyTuple_New(npolys);
	for(int i=0;i<npolys;i++)
	{
		int nv = nverts[i];
		facevarying += nv;
		PyObject* pL = PyTuple_New(nv);
		for(int j=0;j<nv;j++)
		{
			PyTuple_SetItem(pL,j,Py_BuildValue("i",vtx[j]));
			if(vtx[j]>vertex) vertex=vtx[j];
		}
		PyTuple_SetItem(pP,i,pL);
		vtx+=nv;
	};
	vertex++; varying=vertex;
	PyTuple_SetItem(pArgs,0,pP);
	CALLCV(PointsPolygonsV,2)
};

RtVoid Riffler::_GeneralPolygonV(RtInt nloops, RtInt nverts[], DICT)
{
	if(nloops < 1) return;

	PyObject* pArgs = PyTuple_New(2);

	int uniform = 1; int varying = 0; int vertex = 0; int facevarying = 0;

	PyObject* pP = PyTuple_New(nloops);
	for(int i=0;i<nloops;i++)
	{
		int nv = nverts[i];
		PyTuple_SetItem(pP,i,Py_BuildValue("i",nverts[i]));
		facevarying += nv;
	};
	vertex=varying=facevarying;
	PyTuple_SetItem(pArgs,0,pP);
	CALLCV(PolygonV,2)
};

RtVoid Riffler::_PointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], DICT)
{
	if(npolys < 1) return;

	PyObject* pArgs = PyTuple_New(2);

	int uniform = npolys; int varying = 0; int vertex = 0; int facevarying = 0;

	int* vtx = verts;
	int* nvtx = nverts;
	PyObject* pP = PyTuple_New(npolys);
	for(int i=0;i<npolys;i++)
	{
		int nl = nloops[i];
		int nv = 0;
		for(int j=0;j<nl;j++) nv+=nvtx[j];
		nvtx+=nl;

		facevarying += nv;

		PyObject* pL = PyTuple_New(nv);
		for(int j=0;j<nv;j++)
		{
			PyTuple_SetItem(pL,j,Py_BuildValue("i",vtx[j]));
			if(vtx[j]>vertex) vertex=vtx[j];
		}
		vtx+=nv;

		PyTuple_SetItem(pP,i,pL);
	};
	vertex++; varying=vertex;
	PyTuple_SetItem(pArgs,0,pP);
	CALLCV(PointsGeneralPolygonsV,2)
};

RtVoid Riffler::_SubdivisionMeshV(RtToken mask, 
					RtInt nf, RtInt nverts[], RtInt verts[], 
					RtInt nt, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], DICT)
{
	if(nf < 1) return;

	PyObject* pArgs = PyTuple_New(4);

	// MASK
	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",mask));

	// VERTEX
	int uniform = nf; int varying = 0; int vertex = 0; int facevarying = 0;

	int* vtx = verts;
	PyObject* pP = PyTuple_New(nf);
	for(int i=0;i<nf;i++)
	{
		int nv = nverts[i];
		facevarying += nv;
		PyObject* pL = PyTuple_New(nv);
		for(int j=0;j<nv;j++)
		{
			PyTuple_SetItem(pL,j,Py_BuildValue("i",vtx[j]));
			if(vtx[j]>vertex) vertex=vtx[j];
		}
		PyTuple_SetItem(pP,i,pL);
		vtx+=nv;
	};
	vertex++; varying=vertex;
	PyTuple_SetItem(pArgs,1,pP);

	// TAG PROCESSING
	PyObject* pT = PyTuple_New(nt);

	RtInt* pia = intargs;
	RtFloat* pfa = floatargs;

	for(int i=0;i<nt;i++)
	{
		PyObject* pt = PyTuple_New(3);

		// TAG
		PyTuple_SetItem(pt,0,Py_BuildValue("s",tags[i]));
		
		// INTS
		{
			int ia = nargs[2*i];
			PyObject* pti = PyTuple_New(ia);
			for(int j=0;j<ia;j++) PyTuple_SetItem(pti,j,Py_BuildValue("i",pia[j]));
			pia += ia;
			PyTuple_SetItem(pt,1,pti);
		}

		// FLOATS
		{
			int fa = nargs[2*i+1];
			PyObject* ptf = PyTuple_New(fa);
			for(int j=0;j<fa;j++) PyTuple_SetItem(ptf,j,Py_BuildValue("f",pfa[j]));
			pfa += fa;
			PyTuple_SetItem(pt,2,ptf);
		}
	};

	PyTuple_SetItem(pArgs,2,pT);

	// DICT
	CALLCV(SubdivisionMeshV,3)
};

RtVoid Riffler::_HierarchicalSubdivisionMeshV(
	RtToken mask, 
	RtInt nf, RtInt nverts[], RtInt verts[], 
	RtInt nt, RtToken tags[],	RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken stringargs[], DICT)
{
	if(nf < 1) return;

	PyObject* pArgs = PyTuple_New(4);

	// MASK
	PyTuple_SetItem(pArgs,0,Py_BuildValue("s",mask));

	// VERTEX
	int uniform = nf; int varying = 0; int vertex = 0; int facevarying = 0;

	int* vtx = verts;
	PyObject* pP = PyTuple_New(nf);
	for(int i=0;i<nf;i++)
	{
		int nv = nverts[i];
		facevarying += nv;
		PyObject* pL = PyTuple_New(nv);
		for(int j=0;j<nv;j++)
		{
			PyTuple_SetItem(pL,j,Py_BuildValue("i",vtx[j]));
			if(vtx[j]>vertex) vertex=vtx[j];
		}
		PyTuple_SetItem(pP,i,pL);
		vtx+=nv;
	};
	vertex++; varying=vertex;
	PyTuple_SetItem(pArgs,1,pP);

	// TAG PROCESSING
	PyObject* pT = PyTuple_New(nt);

	RtInt* pia = intargs;
	RtFloat* pfa = floatargs;
	RtToken* psa = stringargs;

	for(int i=0;i<nt;i++)
	{
		PyObject* pt = PyTuple_New(4);

		// TAG
		PyTuple_SetItem(pt,0,Py_BuildValue("s",tags[i]));
		
		// INTS
		{
			int ia = nargs[3*i];
			PyObject* pti = PyTuple_New(ia);
			for(int j=0;j<ia;j++) PyTuple_SetItem(pti,j,Py_BuildValue("i",pia[j]));
			pia += ia;
			PyTuple_SetItem(pt,1,pti);
		}

		// FLOATS
		{
			int fa = nargs[3*i+1];
			PyObject* ptf = PyTuple_New(fa);
			for(int j=0;j<fa;j++) PyTuple_SetItem(ptf,j,Py_BuildValue("f",pfa[j]));
			pfa += fa;
			PyTuple_SetItem(pt,2,ptf);
		}

		// STRINGS
		{
			int sa = nargs[3*i+2];
			PyObject* pts = PyTuple_New(sa);
			for(int j=0;j<sa;j++) PyTuple_SetItem(pts,j,Py_BuildValue("s",psa[j]));
			psa += sa;
			PyTuple_SetItem(pt,3,pts);
		}
	};

	PyTuple_SetItem(pArgs,2,pT);

	// DICT
	CALLCV(HierarchicalSubdivisionMeshV,3)
};

RtVoid Riffler::_BlobbyV(RtInt nleaf, 
			RtInt ninst, RtInt inst[], 
			RtInt nflt, RtFloat flt[], RtInt nstr, RtToken str[], DICT)
{
	PyObject* pArgs = PyTuple_New(5);

	// LEAF
	PyTuple_SetItem(pArgs,0,Py_BuildValue("i",nleaf));

	// INSTRUCTIONS
	PyObject* pI = PyTuple_New(ninst);
	for(int i=0;i<ninst;i++) PyTuple_SetItem(pI,i,Py_BuildValue("i",inst[i]));
	PyTuple_SetItem(pArgs,1,pI);

	// FLOATS
	PyObject* pF = PyTuple_New(nflt);
	for(int i=0;i<nflt;i++) PyTuple_SetItem(pF,i,Py_BuildValue("f",flt[i]));
	PyTuple_SetItem(pArgs,2,pF);

	// STRINGS
	PyObject* pS = PyTuple_New(nstr);
	for(int i=0;i<nstr;i++) PyTuple_SetItem(pS,i,Py_BuildValue("s",str[i]));
	PyTuple_SetItem(pArgs,3,pS);

	int uniform = 1; int varying = nleaf; int vertex = nleaf; int facevarying = -1;

	// DICT
	CALLCV(BlobbyV,4)
};

RtVoid Riffler::_Procedural(RtPointer data, RtBound b, RtProcSubdivFunc sdfunc, RtProcFreeFunc freefunc)
{
	PyObject* bound = Py_BuildValue("((ff)(ff)(ff))",b[0],b[1],b[2],b[3],b[4],b[5]);

	// DRA
	if(sdfunc == RiProcDelayedReadArchive)
	{
		GETFILTER
		if(filter->_DelayedReadArchiveFunc == NULL) return;

		PyObject* pArgs = PyTuple_New(2);
		PyTuple_SetItem(pArgs, 0, bound);
		PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",((char**)data)[0]));

		PyObject* pResult = PyObject_CallObject(filter->_DelayedReadArchiveFunc, pArgs);

		Py_XDECREF(pResult);
		Py_XDECREF(pArgs);

		return;
	};

	// RUN PROGRAM
	if(sdfunc == RiProcRunProgram)
	{
		GETFILTER
		if(filter->_RunProgramFunc == NULL) return;

		PyObject* pArgs = PyTuple_New(3);
		PyTuple_SetItem(pArgs, 0, bound);
		PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",((char**)data)[0]));
		PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",((char**)data)[1]));

		PyObject* pResult = PyObject_CallObject(filter->_RunProgramFunc, pArgs);

		Py_XDECREF(pResult);
		Py_XDECREF(pArgs);

		return;
	};

	// DYNAMIC LOAD
	if(sdfunc == RiProcDynamicLoad)
	{
		GETFILTER
		if(filter->_DynamicLoadFunc == NULL) return;

		PyObject* pArgs = PyTuple_New(3);
		PyTuple_SetItem(pArgs, 0, bound);
		PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",((char**)data)[0]));
		PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",((char**)data)[1]));

		PyObject* pResult = PyObject_CallObject(filter->_DynamicLoadFunc, pArgs);

		Py_XDECREF(pResult);
		Py_XDECREF(pArgs);

		return;
	};
};

RtVoid Riffler::_VolumeV(RtToken type, RtBound, int*, DICT)
{
	// ...TODO
};

RtVoid Riffler::_Procedural2V(RtProc2SubdivFunc sfunc, RtProc2BoundFunc bfunc, DICT)
{
	/// ...TODO
};