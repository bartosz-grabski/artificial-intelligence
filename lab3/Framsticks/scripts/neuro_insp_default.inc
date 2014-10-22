//included from neuro_inspection.script
//(settings for each experiment can be stored in a separate file like this one) 

var genofile="other.gen";
var genonr=4; //number of genotype in file to use
var simsteps=100;

var logtofile="neuro_insp_log.txt";
var neuronstolog=[2,4,0]; //indexes of neurons to log
var statsteps=[0,0,0]; //for each of neuronstolog: number of the simulation step to compute avg and stddev of neural state. If you don't need this, just put 0. Only has sense for repeat>1
var forced=[2,20,60,0,  4,40,70,0.1]; //quads of values: neuron#, fromstep, tostep, value

var repeat=1;

