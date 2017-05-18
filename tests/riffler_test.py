import os,sys
import riffler_prman as ri

class Filter :
	def __init__(self,args) :

		print "ARGS : ",args

		self.args = args
		
		return

	def _VArchiveRecord(self,cmt,fmt,buff) :

		print "CMNT : ",cmt," : ",fmt," :: ",buff

		ri.VArchiveRecord(cmt,fmt,buff)

		return

	def Bxdf(self,name,shader,tk) :

		print "BXDF ",name,shader

		ri.Bxdf(name,shader,tk)

		return

	def DynamicLoad2(self,bound,tk) :

		print "DYNLD2 ",bound,tk

		ri.Procedural2("DSO",tk)

		return

	def _AttributeBegin(self) :

		print "ATB"
		ri.AttributeBegin()

		return

	def _AttributeEnd(self) :

		print "ATE"
		ri.AttributeEnd()

		return


	def _FrameBegin(self,frame) :

		print "FBE : ",frame
		ri.FrameBegin(frame)

		return

	# def Sides(self,sides) :

	# 	print "Sides : ",sides
	# 	ri.Sides(sides)

	# 	return

def plugin(args) : return Filter(args)