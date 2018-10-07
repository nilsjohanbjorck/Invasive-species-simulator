from subprocess32 import Popen, PIPE, STDOUT, call
import os
import time

class cppInst:
	"""
	Class for interacting with cpp
	"""

	def __init__(self, callString="./glucose-syrup/simp/glucose glucose-syrup/simp/marg3x3add8.shuffled-as.sat03-1449.cnf", verbose=True):
	
		self.p = Popen(callString.split(" "), stdin = PIPE, stdout = PIPE, stderr = PIPE, bufsize = 1)
		self.broken = False
		self.verbose = verbose

	def forward(self):
		"""
		fast forwards until asks for input or finishes
		"""

		latest = ""

		while True:

			time.sleep(0.001)
			string = self.p.stdout.readline().rstrip()

			if string != "" and self.verbose: 
				print (string)
			
			if "OUTPUT" in string:
				latest = string

			if "INPUT" in string: 
				break


		if latest != "": return latest

	def write(self, string='skrapfile'): 
		"""
		writes to glucose
		"""

		if self.verbose:
			print ("communicating - " + string)
		try:
			self.p.stdin.write(string + '\n')
		except IOError:
			print ("PIPE BROKEN - process done")
			self.broken = True



class Master:
	"""
	class for running a cppInst
	"""

	def __init__(self, callString="./cLionCode/main", graphFile = "/home/shared/dataInvasives-BillyAndJohan/txtData/predator_graph.txt", infList= "/home/shared/dataInvasives-BillyAndJohan/txtData/infList.txt", ii=-1, jj=-1, cat=-1, verbose=True):
		self.ii = ii
		self.jj = jj
		self.cat = cat
		print (graphFile + " " + infList)
		self.inst = cppInst(callString=callString, verbose=verbose)
		self.inst.forward()
		self.inst.write(infList)
		self.inst.forward()
		self.inst.write(graphFile)
		self.inst.forward()


	def evalStrat(self, stratList):
		# stratlist is list of tuples (row, col, year)
		saveTo = os.getcwd() + "/" + str(self.cat) + "_strat_" + str(self.ii) + "_" + str(self.jj) + ".txt"
		write_st = time.time()
		with open(saveTo, "wb") as writefile:
			for tuples in stratList:
				writefile.write("{} {} {}\n".format(tuples[0][0], tuples[0][1], tuples[1])) 
		self.inst.write(string=saveTo)
		outputString = self.inst.forward()
		return float(outputString.split(":")[-1])
		



