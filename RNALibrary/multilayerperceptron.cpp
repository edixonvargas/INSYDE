#include "multilayerperceptron.h"
//#include <complex>

MultilayerPerceptronPattern::MultilayerPerceptronPattern(int ninputs, int ntargets) :
	SimpleInputPattern(ninputs)
{
	initMLPP(ntargets);
}

MultilayerPerceptronPattern::MultilayerPerceptronPattern(vector<double> inputs, vector<double> targets) :
	SimpleInputPattern(inputs)
{
	initMLPP(targets);
}

MultilayerPerceptronPattern::MultilayerPerceptronPattern(double *inputs, size_t sinputs, double *targets, size_t stargets) :
	SimpleInputPattern(inputs, sinputs)
{
	initMLPP(targets, stargets);
}

void MultilayerPerceptronPattern::setTargets(double *v, size_t s)
{

	targets.assign(v, v + s);
}

void MultilayerPerceptronPattern::setTargets(vector<double> targets)
{
	this->targets = targets;
}

vector<double> MultilayerPerceptronPattern::getTargets()
{
	return targets;
}

void MultilayerPerceptronPattern::initMLPP(int ntargets)
{
	targets.resize(ntargets);
}

void MultilayerPerceptronPattern::initMLPP(vector<double> targets)
{
	this->targets = targets;
}

void MultilayerPerceptronPattern::initMLPP(double *targets, size_t s)
{
	setTargets(targets, s);
}

MultilayerPerceptron::MultilayerPerceptron(int ninputs, int noutputs, const vector<int> &layersizes, const TransferFunctionType &tf)
{
	initMLP(ninputs, noutputs, layersizes, tf);
}

void MultilayerPerceptron::initMLP(int ninputs, int noutputs, const vector<int> &hiddenLayerSizes, const TransferFunctionType &tf)
{
	nInputs = ninputs;
	setAlfa(1);
	setLayerSizes(hiddenLayerSizes);
	setInputSize(ninputs);
	setOutputSize(noutputs);
	setTransferFunctionType(tf);
	randomizeWeights();
}

vector<vector<double> > MultilayerPerceptron::getLayerOutputs(const vector<double> &inputs)
{
	size_t  nLayers = layerWeights.size(),
			nNeurons, nWeights;
	double sum;

//	vector<double> __outputs(10);
//	vector<double> __inputs(inputs);
	vector<vector<double> > layerOutputs(layerWeights.size() + 1);
	__inputs = vector<double>(inputs);

	for(size_t layeri = 0; layeri <= nLayers; layeri++){
		nNeurons = (layeri == nLayers ? outputWeights.size() : layerWeights[layeri].size());
//		vector<double> __outputs = vector<double>(nNeurons, 0);
		__outputs = vector<double>(nNeurons, 0);

		for(size_t neuron = 0; neuron < nNeurons; neuron++){
			nWeights = (layeri == nLayers ? outputWeights[neuron].size() : layerWeights[layeri][neuron].size());
			sum = 0;
			for(size_t weight = 0; weight < nWeights; weight++){
				if(weight == nWeights - 1){
					sum += (layeri == nLayers ? -outputWeights[neuron][weight] : -layerWeights[layeri][neuron][weight]);
				}else{
					sum += (layeri == nLayers ? outputWeights[neuron][weight]*__inputs[weight] : layerWeights[layeri][neuron][weight]*__inputs[weight]);
				}
			}
			switch(tf){
				case Sigmoid:
					__outputs[neuron] = 1/(1+exp(-alfa*sum));
					break;
				case Tanh:
					__outputs[neuron] = tanh(alfa*sum);
					break;
			}
		}
		__inputs = __outputs;
		layerOutputs[layeri] = __outputs;
	}

	return layerOutputs;
}

void MultilayerPerceptron::setLayerSize(unsigned int layer, int size)
{
	size_t nLayers = layerWeights.size();

	layerWeights[layer].resize(size);

	//Si la capa a redimensionar es anterior a la penultima capa
	if(layer <= nLayers - 2){
		size_t nNeurons = layerWeights[layer+1].size();
		for(size_t i = 0; i < nNeurons; i++){
			layerWeights[layer+1][i] = getRandomValues(size+1);
		}
	}else if(layer == nLayers - 1){
		size_t nNeurons = outputWeights.size();
		for(size_t i = 0; i < nNeurons; i++){
			outputWeights[i] = getRandomValues(size+1);
		}
	}
}

int MultilayerPerceptron::getLayerSize(unsigned int layer)
{
	return layerWeights[layer].size();
}

void MultilayerPerceptron::setLayerSizes(const vector<int> &sizes)
{
	//    layerSizes = sizes;
//	this->hiddenLayerSizes = sizes;
	size_t nLayers = sizes.size();
	layerWeights = vector<vector<vector<double> > >(nLayers);
	for(size_t i = 0; i < nLayers; i++){
		layerWeights[i] = vector<vector<double > >(sizes[i]);
		for(int j = 0; j < sizes[i]; j++){
			if(i == 0){
				layerWeights[i][j] = getRandomValues(nInputs + 1);
			}else{
				layerWeights[i][j] = getRandomValues(sizes[i - 1] + 1);
			}
		}
		//		setLayerSize(i, sizes[i]);
	}
}

vector<int> MultilayerPerceptron::getLayerSizes()
{
	size_t nLayers = layerWeights.size();
	vector<int> lsizes(nLayers);
	for(size_t i = 0; i < nLayers; i++){
		lsizes[i] = (layerWeights[i].size());
	}
	return lsizes;
}

void MultilayerPerceptron::setOutputSize(size_t size)
{
	outputWeights.resize(size);
	size_t nOutputWeights = layerWeights[layerWeights.size()-1].size();
	for(size_t i = 0; i < size; i++){
		outputWeights[i] = getRandomValues(nOutputWeights + 1);
	}
}

int MultilayerPerceptron::getOutputSize()
{
	return outputWeights.size();
}

void MultilayerPerceptron::setInputSize(int size)
{
	nInputs = size;
	int sNeurons = layerWeights[0].size();

	for(int i = 0; i < sNeurons; i++){
		layerWeights[0][i] = getRandomValues(nInputs + 1);
	}
}

int MultilayerPerceptron::getInputSize()
{
	return layerWeights[0][0].size() - 1;
}

vector<double> MultilayerPerceptron::getOutput(const vector<double> &inputs)
{
	size_t  nLayers = layerWeights.size(),
			nNeurons, nWeights;
	double sum;

	__inputs = vector<double>(inputs);

	for(size_t layeri = 0; layeri <= nLayers; layeri++){
		nNeurons = (layeri == nLayers ? outputWeights.size() : layerWeights[layeri].size());
		__outputs = vector<double>(nNeurons, 0);

		for(size_t neuron = 0; neuron < nNeurons; neuron++){
			nWeights = (layeri == nLayers ? outputWeights[neuron].size() : layerWeights[layeri][neuron].size());
			sum = 0;
			for(size_t weight = 0; weight < nWeights; weight++){
				if(weight == nWeights - 1){
					sum += (layeri == nLayers ? -outputWeights[neuron][weight] : -layerWeights[layeri][neuron][weight]);
				}else{
					sum += (layeri == nLayers ? outputWeights[neuron][weight]*__inputs[weight] : layerWeights[layeri][neuron][weight]*__inputs[weight]);
				}
			}
			switch(tf){
				case Sigmoid:
					__outputs[neuron] = 1/(1+exp(-alfa*sum));
					break;
				case Tanh:
					__outputs[neuron] = tanh(alfa*sum);
					break;
			}
		}
		__inputs = __outputs;
	}

	return __outputs;
}

vector<double> MultilayerPerceptron::getOutput(const vector<int> &inputs)
{
	int sInputs = inputs.size();
	vector<double> dblInputs(sInputs);
	for(int i = 0; i < sInputs; i++){
		dblInputs[i] = double(inputs[i]);
	}
	return getOutput(dblInputs);
}

void MultilayerPerceptron::setTransferFunctionType(MultilayerPerceptron::TransferFunctionType tf)
{
	this->tf = tf;
}

MultilayerPerceptron::TransferFunctionType MultilayerPerceptron::getTransferFunctionType()
{
	return tf;
}

void MultilayerPerceptron::randomizeWeights()
{
	size_t
			nLayers = layerWeights.size(),
			layer, neuron,
			nNeurons,
			nOutputs;

	srand(clock());
	for(layer = 0; layer < nLayers; layer++){
		nNeurons = layerWeights[layer].size();
		for(neuron = 0; neuron < nNeurons; neuron++){
			layerWeights[layer][neuron] = (layer == 0 ? getRandomValues(getInputSize()+1) : getRandomValues(layerWeights[layer-1].size()+1));
		}
	}
	nOutputs = outputWeights.size();
	for(size_t i = 0; i < nOutputs; i++){
		outputWeights[i] = getRandomValues(layerWeights[nLayers-1].size() + 1);
	}
}

MultilayerPerceptron::TrainingResult MultilayerPerceptron::startTraining(const vector<MultilayerPerceptronPattern*> &ts, unsigned int epochs, double errormin, double learningRate, TrainingAlgorithm ta)
{
	size_t sTS = ts.size();
	vector<vector<double> > inputs(sTS);
	vector<vector<double> > targets(sTS);
	for(size_t i = 0; i < sTS; i++){
		inputs[i] = ts[i]->getInputs();
		targets[i] = ts[i]->getTargets();
	}

	TrainingResult tr = startTraining(inputs, targets, epochs, errormin, learningRate, ta);

	return tr;
}

MultilayerPerceptron::TrainingResult MultilayerPerceptron::startTraining(const vector<vector<double> > &inputs, const vector<vector<double> > &targets, unsigned int epochs, double errormin, double learningRate, TrainingAlgorithm ta)
{

	size_t
			nPatterns,
			nNeurons,
			nBOutputs,
			nOutputs;

	vector<double>
			yObtained,
			deltaOut(outputWeights.size(), 0);

	deltaHidden.resize(layerWeights.size());
	for(size_t i = 0; i < deltaHidden.size(); i++){
		deltaHidden[i] = vector<double>(layerWeights[i].size(), 0);
	}


	nPatterns = inputs.size();
	int nLayers  = int(layerWeights.size());

	double pMSE;
//	unsigned long epc;

	double sumDeltas;
	nOutputs = getOutputSize();
//	MultilayerPerceptron::TrainingResult tr;
	tr.time = 0;
	tr.epochs = 0;
	tr.layerWeightsHistory.clear();
	tr.outputWeightsHistory.clear();
	tr.MSEHistory.clear();
	tr.layerWeightsHistory.push_back(layerWeights);
	tr.outputWeightsHistory.push_back(outputWeights);
	tr.MSEHistory.push_back(0);
	vector<vector<double> > layerOutputs;
	training = true;
	clock_t t_ini = clock();
	do{
//		tr.MSE = 0;
		pMSE = 0;
		for(size_t p = 0; p < nPatterns; p++){

			//Se obtienen las salidas para cada una de las capas
			layerOutputs = getLayerOutputs(inputs[p]);
			yObtained = layerOutputs[layerOutputs.size() - 1];
			for(int layer = nLayers; layer >= 0; layer--){
				nNeurons = (layer == nLayers ? outputWeights.size() : layerWeights[layer].size());
//				deltaOut = vector<double>(nNeurons, 0);
				for(size_t neuron = 0; neuron <= nNeurons; neuron++){

					//Se inicia el calculo de todos los deltas
					if(layer == nLayers){ //Si es la capa de salida
						if(neuron < nNeurons){
							switch(tf){
								case Sigmoid:
									deltaOut[neuron] = alfa * yObtained[neuron] * (1 - yObtained[neuron]) * (targets[p][neuron] - yObtained[neuron]);
									break;
								case Tanh:
									deltaOut[neuron] = alfa * (1 - (yObtained[neuron]*yObtained[neuron])) * (targets[p][neuron] - yObtained[neuron]);
									break;
							}
						}else{
							continue;
						}
					}else{
						size_t nDeltaElements = (layer == nLayers - 1 ? outputWeights.size() : layerWeights[layer + 1].size());
						sumDeltas = 0;
						for(size_t element = 0; element < nDeltaElements; element++){
							if(layer == nLayers - 1){
								sumDeltas += deltaOut[element] * outputWeights[element][neuron];
							}else{
								sumDeltas += deltaHidden[layer+1][element] * layerWeights[layer+1][element][neuron];
							}
						}

						switch(tf){
							case Sigmoid:
								deltaHidden[layer][neuron] = alfa * layerOutputs[layer][neuron] * (1 - layerOutputs[layer][neuron]) * sumDeltas;
								break;
							case Tanh:
								deltaHidden[layer][neuron] = alfa * (1 - (layerOutputs[layer][neuron]*layerOutputs[layer][neuron])) * sumDeltas;
								break;
						}
					}
				}
			}

			//Comienza la actualizacion de los pesos
			for(int layer = nLayers; layer >= 0; layer--){
				nNeurons = (layer == nLayers ? nOutputs : layerWeights[layer].size());
				for(size_t i = 0; i < nNeurons; i++){
					nBOutputs = (layer == 0 ? inputs[p].size() : layerWeights[layer - 1].size());
					for(size_t j = 0; j <= nBOutputs; j++){
						if(layer == nLayers){
							outputWeights[i][j] += (j == nBOutputs ? -learningRate*deltaOut[i] : learningRate*deltaOut[i]*layerOutputs[layer-1][j]);
						}else if(layer == 0){
							layerWeights[layer][i][j] += (j == nBOutputs ?
															  -learningRate*deltaHidden[layer][i] :
															  learningRate*deltaHidden[layer][i]*inputs[p][j]);
						}else{
							layerWeights[layer][i][j] += (j == nBOutputs ? -learningRate*deltaHidden[layer][i] : learningRate*deltaHidden[layer][i]*layerOutputs[layer-1][j]);
						}
					}
				}
			}
			yObtained = getOutput(inputs[p]);
			for(size_t neuron = 0; neuron < nOutputs; neuron++){
				pMSE += (targets[p][neuron] - yObtained[neuron])*(targets[p][neuron] - yObtained[neuron]);
			}
		}
		pMSE += pMSE/2;
		tr.MSE = pMSE;
		tr.MSEHistory.push_back(tr.MSE);
//		tr.layerWeightsHistory.push_back(layerWeights);
//		tr.outputWeightsHistory.push_back(outputWeights);
//		epc++;
		tr.epochs++;
	}while(tr.MSE >= errormin && tr.epochs < epochs && training);

	tr.time = double(clock() - t_ini)/CLOCKS_PER_SEC;

	return tr;
}

MultilayerPerceptron::TrainingResult MultilayerPerceptron::getTrainingSnapshot()
{
	return tr;
}

void MultilayerPerceptron::stopTraining()
{
	training = false;
}

void MultilayerPerceptron::setAlfa(double a)
{
	if(a >= 0 && a <= 1){
		alfa = a;
	}else if(a < 0){
		alfa = 0;
	}else{
		alfa = 1;
	}
}

double MultilayerPerceptron::getAlfa()
{
	return alfa;
}

