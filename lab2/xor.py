from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure import TanhLayer

ds = SupervisedDataSet(2, 1)

ds.addSample((0, 0), (0,))
ds.addSample((0, 1), (1,))
ds.addSample((1, 0), (1,))
ds.addSample((1, 1), (0,))

net = buildNetwork(2, 3, 1, bias=True, hiddenclass=TanhLayer)
trainer = BackpropTrainer(net, ds, learningrate=0.02, momentum=0.001)

epoch_error = [];

for i in xrange(10000): 
	epoch_error.append((i,trainer.train()))

#trainer.trainUntilConvergence()

print net.activate((0.2,0.9))
print net.activate((0,0))

f = open("epoch_error.plot","w")
for e in epoch_error:
	f.write(str(e[0])+" "+str(e[1])+"\n")

f.close()