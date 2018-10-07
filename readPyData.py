import pickle
import numpy as np
import os
from subprocess import call


"""
prepares text document for c++
needs python3.5 for pickle compatability
manually remove all data before running script
"""


n_row = 107
n_col = 125
NUM_ITER = 30


sharedFolder = "/home/fs01/yb263/Simulator-HWA/final_data"
datafolder = "txtData"
PREDGRAPH = "predator_graph20"
INFDIR = "prey_instances"

def getGraph(filename=PREDGRAPH):

	graph = pickle.load(open(sharedFolder + "/"+ filename, "rb"))
		
	with open(sharedFolder + "/" + datafolder + "/" + filename + ".txt", "w") as writefile:

		for key in graph:
			for elem in graph[key]:
				num = '%f' % (float(elem[2])/1.0) # suppress scientific not, see https://stackoverflow.com/questions/658763/how-do-i-suppress-scientific-notation-in-python
				fromID = "{},{}".format(key[0], key[1])
				toID = "{},{}".format(elem[0], elem[1])
				writeString = "{} {} {}\n".format(fromID,toID,num)
				writefile.write(writeString)





def readInfected(filename):

	mat = np.load(open(sharedFolder + "/" + INFDIR + "/" + filename, "rb"))
	with open(sharedFolder + "/" + datafolder + "/" + filename + ".txt", "w") as writefile:
		for i in range(mat.shape[0]):
			for j in range(mat.shape[0]):
				if mat[i,j] > 0 and mat[i,j] < 1950 + NUM_ITER:
					writeString = "{} {} {}\n".format(i,j,int(mat[i,j])-1950) #note data formatted for years
					writefile.write(writeString)



def writeAllInf():
	files = os.listdir(sharedFolder + "/" + INFDIR)
	#files = files[:10]

	for file in files:
		readInfected(file)





def createList():
	"""
	create textfile of lists for all infected files
	"""

	dataDir = sharedFolder + "/" + datafolder
	files = [f for f in os.listdir(dataDir)] 

	with open(sharedFolder + "/" +  datafolder + "/infList.txt", "w") as writefile:
		for file in files:
			writefile.write(dataDir +"/" + file + "\n")


def prepareAll():
	"""
	clean up data dir manually first, calls all functions above in right order
	"""
	writeAllInf()
	createList()
	getGraph()


prepareAll()


