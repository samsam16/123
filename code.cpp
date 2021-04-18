#include <iostream>
#include "CNN/canela.hpp"

using namespace std;

model_t * build_model(const dataset_t & ds)  {
	model_t * model = new model_t();
	fc_layer_t *layer2 = new fc_layer_t(ds.data_size, ds.label_size.x);
	model->add_layer(*layer2 );
	return model;
}

void train_model(model_t * model,
		 dataset_t & train,
		 int epochs) {
	
	for (int i = 0; i < epochs; i++) {
		for ( auto & t : train.test_cases ) {
			model->train(t);
		}
	}
}

double test_model(model_t * model,
		  dataset_t & test) {
	int correct = 0;
	int incorrect = 0;
	
	for (auto& t : test.test_cases ) {
		tensor_t<double>& out = model->apply(t.data);
		
		tdsize guess = out.argmax();
	        tdsize answer = t.label.argmax();
		if (guess == answer) {
			correct++;
		} else {
			incorrect++;
		}
	}

	double total_error = (correct+0.0)/(correct+ incorrect +0.0);
	return total_error;
}
