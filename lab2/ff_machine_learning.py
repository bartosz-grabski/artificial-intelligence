from pybrain.datasets.supervised import SupervisedDataSet
from pybrain.tools.shortcuts     import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.structure.modules import SigmoidLayer
from pybrain.utilities import percentError
import sys


class DataProcessor(object):

	def __init__(self,data):
		self.data = data
		self.minFeature = min(data)
		self.maxFeature = max(data)

	def normalize(self):
		maxFeature = self.minFeature
		minFeature = self.maxFeature
		data = self.data
		normalized = map(lambda x: (x - minFeature)/(maxFeature-minFeature),data)
		self.normalized = normalized
		return normalized

	def convertToNGrams(self,n):
		data = self.normalized
		ngrams = []
		ngrams = [data[i:i+n] for i in xrange(len(data) - n+1)]
		self.ngrams = ngrams
		return ngrams

	def denormalize(self,el):
		return el*(self.maxFeature-self.minFeature) + self.minFeature

def prepareCSVData(filename,column):
		f = open(filename)
		data = []
		next(f) #skipping first line containing column description
		for line in f:
			data.insert(0,float(line.split(",")[4]))
		return data


def createDataSet(ngrams, testDataProportion):
		ngramLength = len(ngrams[0])
		a = SupervisedDataSet(ngramLength,1)
		for g in ngrams:
			a.addSample(g[0:ngramLength],g[ngramLength-1])
		return a.splitWithProportion(testDataProportion)

def buildFFNetwork(trainingData,hiddenLayers,outputDim):
	return buildNetwork(trainingData.indim, hiddenLayers, outputDim, hiddenclass=SigmoidLayer, outclass=SigmoidLayer)

def run(momentum,learningRate, epoch, dataProcessor):

	epochErrors = []
	percentageErrors = []

	ngrams = dataProcessor.ngrams
	
	trainingData,testData = createDataSet(ngrams,0.75)
	network = buildFFNetwork(trainingData,20, 1)


	trainer = BackpropTrainer(network,dataset = trainingData, momentum=momentum, learningrate = learningRate)

	for i in xrange(epoch): #did not use trainer.trainEpochs to manually append each error for visualisation purposes
		epochErrors.append(trainer.train())

	prediction = network.activate(ngrams[-1]);


	#testResults = network.activateOnDataset(trainingData)
	#target = trainingData['target']


	#results = []
	#for i in xrange(len(testResults)):
	#	results.append(int(abs(testResults[i][0] - target[i][0]) < 0.001))

	#resultsPercentage = float(reduce(lambda x,y: x+y,results,0))/len(results)

	return epochErrors, prediction


	#percentage = map(lambda x: abs(x[0] - x[1][]) < 0.01, zip(testData['target'],testResults))
	#print percentage 

	#percentage = len(filter(lambda x: x == True, percentage))/len(percentage)


	#print percentage

	#print epochErrors

def syntax():
	print "./"+sys.argv[0]+" MOMENTUM LEARNING_RATE N"

if __name__ == "__main__":
	if len(sys.argv) < 4:
		syntax()
		exit()

	momentum = float(sys.argv[1])
	learning_rate = float(sys.argv[2])
	n = int(sys.argv[3])

	data = prepareCSVData("google_stock.csv",3)

	dataProcessor = DataProcessor(data)
	dataProcessor.normalize()
	dataProcessor.convertToNGrams(5)

	results = []
	predictions = [];
	for i in xrange(n):
		epochErrors, prediction = run(momentum, learning_rate, 40, dataProcessor)
		results.append(epochErrors)
		predictions.append(prediction[0])

	mean = zip(*results)
	mean = map(sum,mean)
	mean = map(lambda x: x/n, mean)

	prediction = sum(predictions)/n
	prediction = dataProcessor.denormalize(prediction)

	filename = "mom"+str(momentum)+"lr"+str(learning_rate)+"n"+str(n)

	outfile = open(filename,"w")
	outfile.write("%s %s\n" % (mean[0], prediction))
	for item in mean[1:]:
		outfile.write("%s\n" % item)


