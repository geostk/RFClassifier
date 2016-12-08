#pragma once
#include "common.hpp"
#include "Frame.hpp"
#include "RandomGenerator.hpp"

typedef std::vector<Feature>::iterator FeatureIterator;

struct LearnerParameters {
	bool is_unary;
	float offset_1[2];
	float offset_2[2];
};

struct DataSplit {

	DataSplit(std::vector<Feature> & d,
		  FeatureIterator s,
		  FeatureIterator e) : data(d), start(s), end(e) {}

	int getSize() const {
		return std::distance(start, end);
	}
	
	std::vector<Feature> & data;
	FeatureIterator start;
	FeatureIterator end;
};


class LabelHistogram {
public:
	LabelHistogram(DataSplit & ds);
	~LabelHistogram() {}
	void print() const {
		std::cout << std::endl << "# ";
		for (auto b : _hist)
			std::cout << b << " ";
		std::cout << "#" << std::endl;
	}

	void normalize() {
		float sum = 0;
		for (auto b : _hist)
			sum += b;
		
		if (sum  > 0)
			for (int i = 0; i < _hist.size(); i++)
				_hist[i]/=sum;
	}
	
	float computeEntropy() const {

		float sum = 0;
		for (int i = 0; i < _hist.size(); i++)
			if (_hist[i] > 0)
				sum += _hist[i]*log(_hist[i]);

		std::cout << "entropy:" << -sum << std::endl;
		getchar();
		return sum;
	}
	
private:
	std::vector<float> _hist;
};


struct NodeConstructor {

	NodeConstructor(int id,
			FeatureIterator s,
			FeatureIterator e) : node_id(id), start(s), end(e) {}
	int node_id;
	FeatureIterator start, end;
};

class Node
{
public:

	Node(int depth) :
		_threshold(0.0),
		_depth(depth),
		_is_leaf(false) {};

	void train(DataSplit);

	float evaluateCostFunction(const DataSplit, float) const;

	FeatureIterator getSplitIterator(DataSplit, float threshold) const;
	FeatureIterator getSplitIterator(DataSplit) const;

	bool isLeaf() { return _is_leaf; }

	int left_child;
	int right_child;

private:

	LearnerParameters _node_params;
	float _threshold;
	int _depth;
	bool _is_leaf;
};