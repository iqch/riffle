import os,sys
import riffler_prman as ri

class Filter :
	def __init__(self,args) :

		print "ARGS : ",args

		self.args = args
		
		return

	def VArchiveRecord(self,cmt,fmt,buff) :

		print "CMNT : ",cmt," : ",fmt," :: ",buff

		#ri.VArchiveRecord(...)

		return

	def Bxdf(self,name,shader,tk) :

		print "BXDF ",name,shader,tk

		ri.Bxdf(name,shader,tk)

		return

	def AttributeBegin(self) :

		print "ATB"
		ri.AttributeBegin()

		return

	def AttributeEnd(self) :

		print "ATE"
		ri.AttributeBegin()

		return


	def FrameBegin(self,frame) :

		print "FBE : ",frame
		ri.FrameBegin(frame)

		return

	# def Sides(self,sides) :

	# 	print "Sides : ",sides
	# 	ri.Sides(sides)

	# 	return

def plugin(args) : return Filter(args)