#ifndef MULTILAYERPERCEPTRONPATTERN_H
#define MULTILAYERPERCEPTRONPATTERN_H

#include "ANNglobal.h"
#include "simplepattern.h"

class ANNFRAMEWORK_EXPORT MultilayerPerceptronPattern : public SimpleInputPattern
{
	public:
		explicit MultilayerPerceptronPattern(int ninputs, int ntargets);
		explicit MultilayerPerceptronPattern(vector<double> inputs, vector<double> targets);
		explicit MultilayerPerceptronPattern(double *inputs, size_t sinputs, double *targets, size_t stargets);

		void setTargets(double v[]);
		void setTargets(double *v, size_t s);
		void setTargets(vector<double> targets);
		vector<double> getTargets();

	private:

		vector<double> targets;

		void initMLPP(int ntargets);
		void initMLPP(vector<double> targets);
		void initMLPP(double *targets, size_t s);

};

#endif // MULTILAYERPERCEPTRONPATTERN_H