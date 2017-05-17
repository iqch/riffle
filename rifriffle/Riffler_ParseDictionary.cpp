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
*	Tool and macros for work with DICT-data
*/

#include "stdafx.h"

// SIMPLE

bool ParseDictionary(PyObject* dict, int n, RtToken tk[], RtPointer vl[])
{
	RifTokenType tokType;
	RifTokenDetail tokDetail;
	RtInt arraylen;

	for(int i=0;i<n;i++)
	{
		RtToken t = tk[i];
		RtPointer p =vl[i];
		RtInt res = RifGetDeclaration(t, &tokType, &tokDetail, &arraylen);

		if(res != 0)
		{
			continue;
		}

		switch(tokType)
		{
		case k_RifInteger: // INT
			if(arraylen == 1)
			{
				// JUST INTEGER
				PyDict_SetItemString(dict, t, Py_BuildValue("i",*((int*)p)));
			}
			else
			{
				// ARRAY
				if(t[3] != '[') continue;
				PyObject* pArgs = PyTuple_New(arraylen);
				int* P = (int*)p;
				for(int j=0;j<arraylen;j++) PyTuple_SetItem(pArgs, j, Py_BuildValue("i",P[j]));
				PyDict_SetItemString(dict, t, pArgs);
			};
			break;
		case k_RifFloat: // FLOAT
			if(arraylen == 1)
			{
				// JUST FLOAT
				PyDict_SetItemString(dict, t, Py_BuildValue("f",*((float*)p)));
			}
			else
			{
				// ARRAY
				PyObject* pArgs = PyTuple_New(arraylen);
				float* P = (float*)p;
				for(int j=0;j<arraylen;j++) PyTuple_SetItem(pArgs, j, Py_BuildValue("f",P[j]));
				PyDict_SetItemString(dict, t, pArgs);
			};
			break;
		case k_RifString: // STRING/SHADER
			if(arraylen == 1)
			{
				// JUST STRING
				const char* strval = *((RtToken*)(p));
				PyDict_SetItemString(dict, t, Py_BuildValue("s",strval));
			}
			else
			{
				// ARRAY
				PyObject* pArgs = PyTuple_New(arraylen);
				char** P = (char**)p;
				for(int j=0;j<arraylen;j++) PyTuple_SetItem(pArgs, j, Py_BuildValue("s",P[j]));
				PyDict_SetItemString(dict, t, pArgs);
			};
			break;
		case k_RifColor: // COLOR
		case k_RifPoint: // POINT
		case k_RifVector: // VECTOR
		case k_RifNormal: // NORMAL
			if(arraylen == 1)
			{
				// JUST POINT
				float* V = (float*)p;
				PyDict_SetItemString(dict, t, Py_BuildValue("(fff)",V[0],V[1],V[2]));
			}
			else
			{
				// ARRAY
				PyObject* pArgs = PyTuple_New(arraylen);
				float* P = (float*)p;
				for(int j=0;j<arraylen;j++)
				{
					PyTuple_SetItem(pArgs, j, Py_BuildValue("(fff)",P[0],P[1],P[2]));
					P+=3;
				}
				PyDict_SetItemString(dict, t, pArgs);
			};
			break;
		case k_RifMatrix: // MATRIX
			if(arraylen == 1)
			{
				// JUST MATRIX
				float* V = (float*)p;
				PyDict_SetItemString(dict, t, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))"
					,V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]
					,V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
			}
			else
			{
				// ARRAY
				PyObject* pArgs = PyTuple_New(arraylen);
				float* P = (float*)p;
				for(int j=0;j<arraylen;j++)
				{
					PyTuple_SetItem(pArgs, j, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))"
						,P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7]
						,P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
					P+=16;
				}
				PyDict_SetItemString(dict, t, pArgs);
			};
			break;
		case k_RifHPoint: // NOT SUPPORTED - RARE USING
		case k_RifMPoint: // NOT SUPPORTED - NO INFO ABOUT FORMAT
		default:
			continue;
		};
	};

	//PyObject_Print(dict,stdout,Py_PRINT_RAW);

	return true;
};

bool CollectDictionary(PyObject* dict, int* N, RtToken** tk, RtPointer** vl)
{
	if(PyDict_Check(dict) == NULL) return false;

	RifTokenType tokType;
	RifTokenDetail tokDetail;
	RtInt arraylen;

	PyObject* keys = PyDict_Keys(dict);

	//PyObject_Print(keys,stderr,Py_PRINT_RAW);
	int n = PyList_Size(keys);
	if(n == 0) return false;

	*N = n;

	*tk = new RtToken[n];		// WE HAVE TO CLEAN THIS OUTSIDE!
	*vl = new RtPointer[n];	// AND THIS TOO!

	for(int i=0;i<n;i++)
	{
		(*tk)[i] = NULL;
		(*vl)[i] = NULL;

		PyObject *item = PyList_GetItem(keys,i);
		PyObject *value = PyDict_GetItem(dict,item);

		// ...INIT value
		RtToken t = PyString_AsString(item);
		if(t == NULL) return false;
		
		RtInt res = RifGetDeclaration(t, &tokType, &tokDetail, &arraylen);

		switch(tokType)
		{
		case k_RifInteger: // INT
			if(arraylen == 1)
			{
				// JUST INTEGER
				int* _VL = new int;
				(*vl)[i] = _VL;
				*_VL = PyInt_AsLong(value);
			}
			else
			{
				// ARRAY
				if(!PyTuple_Check(value)) return false;

				int quants = PyTuple_Size(value);
				int* _VL = new int[quants];
				(*vl)[i] = _VL;

				for(int j=0;j<quants;j++) _VL[j] = PyInt_AsLong(PyTuple_GetItem(value,j));
			};
			break;
		case k_RifFloat: // FLOAT
			if(arraylen == 1)
			{
				// JUST FLOAT
				float* _VL = new float;
				(*vl)[i] = _VL;
				*_VL = PyFloat_AsDouble(value);
			}
			else
			{
				// ARRAY
				if(!PyTuple_Check(value)) return false;

				int quants = PyTuple_Size(value);
				float* _VL = new float[quants];
				(*vl)[i] = _VL;

				for(int j=0;j<quants;j++) _VL[j] = PyFloat_AsDouble(PyTuple_GetItem(value,j));
			};
			break;
		case k_RifString: // STRING/SHADER
			if(arraylen == 1)
			{
				// JUST STRING
				RtToken* V = new RtToken;
				*V = strdup(PyString_AsString(value));
				(*vl)[i] = V;
			}
			else
			{
				// ARRAY
				if(!PyTuple_Check(value)) return false;

				int quants = PyTuple_Size(value);
				RtToken* _VL = new RtToken[quants];
				(*vl)[i] = _VL;

				for(int j=0;j<quants;j++)
				{
					_VL[j] = strdup(PyString_AsString((PyTuple_GetItem(value,j))));
				};
			};
			break;
		case k_RifColor: // COLOR
		case k_RifPoint: // POINT
		case k_RifVector: // VECTOR
		case k_RifNormal: // NORMAL
			if(arraylen == 1)
			{
				// JUST POINT/COLOR
				if(!PyTuple_Check(value)) return false;
				if(PyTuple_Size(value) != 3) return false;
				float* _VL = new float[3];
				(*vl)[i] = _VL;
				for(int j=0;j<3;j++) _VL[j] = PyFloat_AsDouble(PyTuple_GetItem(value,j));
			}
			else
			{
				// ARRAY
				if(!PyTuple_Check(value)) return false;

				int quants = PyTuple_Size(value);
				float* _VL = new float[3*quants];

				for(int j=0;j<quants;j++)
				{
					PyObject* v = PyTuple_GetItem(value,j);
					if(!PyTuple_Check(v)) return false; // ...LEAKS POSSIBLE
					if(PyTuple_Size(v) != 3) return false; // ...LEAKS POSSIBLE

					for(int k=0;j<3;k++)
					{
						_VL[3*j+k] = PyFloat_AsDouble(PyTuple_GetItem(v,k));
					};
				};
				(*vl)[i] = _VL;
			};
			break;
		case k_RifMatrix: // MATRIX
			if(arraylen == 1)
			{
				// JUST MATRIX

				if(!PyTuple_Check(value)) return false;
				if(PyTuple_Size(value) != 4) return false;

				PyObject* R[4];
				for(int j=0;j<4;j++)
				{
					R[j] = PyTuple_GetItem(value,j);
					if(!PyTuple_Check(R[j])) return false;
					if(PyTuple_Size(R[j]) != 4) return false;
				};

				float* _VL = new float[16];
				(*vl)[i] = _VL;

				for(int j=0;j<4;j++)
				{
					for(int k=0;k<4;k++)
					{
						_VL[4*j+k] = PyFloat_AsDouble(PyTuple_GetItem(R[j],k));
					};
				};
			}
			else
			{
				// ARRAY
				if(!PyTuple_Check(value)) return false;

				int quants = PyTuple_Size(value);
				float* _VL = new float[16*quants];

				for(int j=0;j<quants;j++)
				{
					PyObject* v = PyTuple_GetItem(value,j);
					if(!PyTuple_Check(v)) return false; // ...LEAKS POSSIBLE
					if(PyTuple_Size(v) != 4) return false; // ...LEAKS POSSIBLE

					for(int k=0;k<4;k++)
					{
						PyObject* u = PyTuple_GetItem(v,k);
						if(!PyTuple_Check(u)) return false; // ...LEAKS POSSIBLE
						if(PyTuple_Size(u) != 4) return false; // ...LEAKS POSSIBLE

						for(int l=0;l<4;l++)
						{
							_VL[16*j+4*k+l] = PyFloat_AsDouble(PyTuple_GetItem(u,l));
						};
					};
				};
				(*vl)[i] = _VL;
			};
			break;
			//case 'h': // H POINTS ?
			//	break;
		default:
			return false;
		};

		(*tk)[i] = strdup(t);
	};

	return true;
};

bool DisposeTKVL(int n, RtToken* tk, RtPointer* vl)
{
	if(n == 0) return true;

	RifTokenType tokType;
	RifTokenDetail tokDetail;
	RtInt arraylen;

	for(int i=0;i<n;i++)
	{
		if(tk[i] == NULL) return false;
		if(vl[i] == NULL) return false;

		RtInt res = RifGetDeclaration(tk[i], &tokType, &tokDetail, &arraylen);

		switch(tokType)
		{
		case k_RifInteger: // INT
			if(arraylen == 1)
			{
				// JUST INTEGER
				delete ((int*)vl[i]);
			}
			else
			{
				// ARRAY
				delete [] ((int*)vl[i]);
			};
			break;
		case k_RifFloat: // FLOAT
			if(arraylen == 1)
			{
				// JUST FLOAT
				delete ((float*)vl[i]);
			}
			else
			{
				// ARRAY
				delete [] ((float*)vl[i]);
			};
			break;
		case k_RifString: // STRING
			if(arraylen == 1)
			{
				// JUST STRING
				delete [] *((RtToken*)vl[i]);
				delete (RtToken*)vl[i];
			}
			else
			{
				// ARRAY
				char** P = (char**)vl[i];
				for(int j=0;j<arraylen;j++)
				{
					delete [] P[j];
				}
				delete [] P;
			};
			break;
		case k_RifColor: // COLOR
		case k_RifPoint: // POINT
		case k_RifVector: // VECTOR
		case k_RifNormal: // NORMAL
		case k_RifMatrix: // MATRIX
		//case 'h': // H POINTS ?
			delete [] ((float*)vl[i]);
			break;
		default:
			return false;
		};
		delete [] tk[i];
	};

	delete [] tk;
	delete [] vl;

	return true;
};

//  COMPLEX

bool ParseDictionaryUVVF(PyObject* dict, int n, RtToken tk[], RtPointer vl[], int uniform, int varying, int vertex, int facevarying)
{
	RifTokenType tokType;
	RifTokenDetail tokDetail;
	RtInt arraylen;

	for(int i=0;i<n;i++)
	{
		RtToken t = tk[i];
		RtPointer p =vl[i];
		RtInt res = RifGetDeclaration(t, &tokType, &tokDetail, &arraylen);

		if(res != 0) continue;

		switch(tokType)
		{
		case k_RifFloat: // FLOAT
			if(arraylen == 1)
			{
				// JUST FLOAT
				switch(tokDetail)
				{
				case k_RifConstant:
					PyDict_SetItemString(dict, t, Py_BuildValue("f",*((float*)p)));
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++) PyTuple_SetItem(A,j,Py_BuildValue("f",((float*)p)[j]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++) PyTuple_SetItem(A,j,Py_BuildValue("f",((float*)p)[j]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++) PyTuple_SetItem(A,j,Py_BuildValue("f",((float*)p)[j]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++) PyTuple_SetItem(A,j,Py_BuildValue("f",((float*)p)[j]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			}
			else
			{
				// ARRAY
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyObject* pArgs = PyTuple_New(arraylen);
						float* P = (float*)p;
						for(int j=0;j<arraylen;j++) PyTuple_SetItem(pArgs, j, Py_BuildValue("f",P[j]));
						PyDict_SetItemString(dict, t, pArgs);
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						float* P = (float*)p;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("f",*P));
								P+=1;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						float* P = (float*)p;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("f",*P));
								P+=1;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						float* P = (float*)p;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("f",*P));
								P+=1;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						float* P = (float*)p;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("f",*P));
								P+=1;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			};
			break;
		case k_RifString: // STRING/SHADER
			if(arraylen == 1)
			{
				// JUST STRING
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						const char* strval = ((RtToken*)(p))[0];
						PyDict_SetItemString(dict, t, Py_BuildValue("s",strval));
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						char** C = (char**)p;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyTuple_SetItem(A, j, Py_BuildValue("s",*C));
							C+=1;
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
				case k_RifVertex:
				case k_RifFaceVarying:
				default:
					return false;
				};
			}
			else
			{
				// ARRAY
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyObject* pArgs = PyTuple_New(arraylen);
						char** P = (char**)p;
						for(int j=0;j<arraylen;j++) PyTuple_SetItem(pArgs, j, Py_BuildValue("s",P[j]));
						PyDict_SetItemString(dict, t, pArgs);
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						char** C = (char**)p;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("s",*C));
								C+=1;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
				case k_RifVertex:
				case k_RifFaceVarying:
				default:
					return false;
				};
			};
			break;
		case k_RifColor: // COLOR
		case k_RifPoint: // POINT
		case k_RifVector: // VECTOR
		case k_RifNormal: // NORMAL
			if(arraylen == 1)
			{
				float* V = (float*)p;
				// JUST POINT
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyDict_SetItemString(dict, t, Py_BuildValue("(fff)",V[0],V[1],V[2]));
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyTuple_SetItem(A,j,Py_BuildValue("(fff)",V[0],V[1],V[2]));
							V+=3;
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++)
						{
							PyTuple_SetItem(A,j,Py_BuildValue("(fff)",V[0],V[1],V[2]));
							V+=3;
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++)
						{
							PyTuple_SetItem(A,j,Py_BuildValue("(fff)",V[0],V[1],V[2]));
							V+=3;
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++)
						{
							PyTuple_SetItem(A,j,Py_BuildValue("(fff)",V[0],V[1],V[2]));
							V+=3;
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			}
			else
			{
				// ARRAY
				float* P = (float*)p;
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyObject* pArgs = PyTuple_New(arraylen);
						for(int j=0;j<arraylen;j++)
						{
							PyTuple_SetItem(pArgs, j, Py_BuildValue("(fff)",P[0],P[1],P[2]));
							P+=3;
						}
						PyDict_SetItemString(dict, t, pArgs);
					}
				case k_RifUniform:
					{
						if(uniform == -1) break;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("(fff)",P[0],P[1],P[2]));
								P+=3;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("(fff)",P[0],P[1],P[2]));
								P+=3;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("(fff)",P[0],P[1],P[2]));
								P+=3;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("(fff)",P[0],P[1],P[2]));
								P+=3;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			};
			break;
		case k_RifMatrix: // MATRIX
			if(arraylen == 1)
			{
				// JUST MATRIX
				float* V = (float*)p;
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyDict_SetItemString(dict, t, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))"
							,V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]
						,V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
							PyTuple_SetItem(A,j,Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
								V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7],
								V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++)
							PyTuple_SetItem(A,j,Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
								V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7],
								V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++)
							PyTuple_SetItem(A,j,Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
								V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7],
								V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++)
							PyTuple_SetItem(A,j,Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
								V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7],
								V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]));
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			}
			else
			{
				// ARRAY
				float* P = (float*)p;
				switch(tokDetail)
				{
				case k_RifConstant:
					{
						PyObject* pArgs = PyTuple_New(arraylen);
						float* P = (float*)p;
						for(int j=0;j<arraylen;j++)
						{
							PyTuple_SetItem(pArgs, j, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
								P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],
								P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
							P+=16;
						}
						PyDict_SetItemString(dict, t, pArgs);
					}
					break;
				case k_RifUniform:
					{
						if(uniform == -1) break;
						PyObject* A = PyTuple_New(uniform);
						for(int j=0;j<uniform;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
									P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],
									P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
								P+=16;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVarying:
					{
						if(varying == -1) break;
						PyObject* A = PyTuple_New(varying);
						for(int j=0;j<varying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
									P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],
									P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
								P+=16;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifVertex:
					{
						if(vertex == -1) break;
						PyObject* A = PyTuple_New(vertex);
						for(int j=0;j<vertex;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
									P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],
									P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
								P+=16;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				case k_RifFaceVarying:
					{
						if(facevarying == -1) break;
						PyObject* A = PyTuple_New(facevarying);
						for(int j=0;j<facevarying;j++)
						{
							PyObject* a = PyTuple_New(arraylen);
							for(int k=0;k<arraylen;k++) 
							{
								PyTuple_SetItem(a, k, Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",
									P[0],P[1],P[2],P[3],P[4],P[5],P[6],P[7],
									P[8],P[9],P[10],P[11],P[12],P[13],P[14],P[15]));
								P+=16;
							};
							PyTuple_SetItem(A, j, a);
						}
						PyDict_SetItemString(dict, t, A);
					}
					break;
				default:
					return false;
				};
			};
			break;
		case k_RifHPoint: // NOT SUPPORTED - RARE USING
		case k_RifMPoint: // NOT SUPPORTED - NO INFO ABOUT FORMAT
		case k_RifInteger: // INT IS UNSUPPORTED AS GEOMETRIC QUANTITY!
		default:
			continue;
		};
	};

	//PyObject_Print(dict,stdout,Py_PRINT_RAW);

	return true;
};