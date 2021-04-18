#include <iostream>
#include "CNN/canela.hpp"
#include "pin_tags.h"

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
	//references to input and output tensors of the layer
	layer_t *layer = model->layers[0];
	tensor_t<double> *input_tensor = &layer->in;
	tensor_t<double> *output_tensor = &layer->out;
	
	//downcast a layer_t object to it's derived class fc_layer_t to access the weight tensor	
	fc_layer_t *fc_layer = dynamic_cast<fc_layer_t*>(layer);
	tensor_t<double> *weight_tensor = &fc_layer->weights;

	int input_tensor_size = input_tensor->element_count();
	int output_tensor_size = output_tensor->element_count();
	int weight_tensor_size = weight_tensor->element_count();
	
	int tag_number = 0;
	for (int i = 0; i < epochs; i++) {
		for ( auto & t : train.test_cases ) {
 			string input_tag = "Input" + to_string(tag_number);
 			string output_tag = "Output" + to_string(tag_number);
 			string weight_tag = "Weights" + to_string(tag_number);
			DUMP_START(input_tag.c_str(), (void *) input_tensor->data, (void *) &input_tensor->data[input_tensor_size - 1], false);
			DUMP_START(output_tag.c_str(), (void *) output_tensor->data, (void *) &output_tensor->data[output_tensor_size -1], false);
			DUMP_START(weight_tag.c_str(), (void *) weight_tensor->data, (void *) &weight_tensor->data[weight_tensor_size -1], false);
			model->train(t);
			DUMP_STOP(input_tag.c_str());
			DUMP_STOP(output_tag.c_str());
			DUMP_STOP(weight_tag.c_str());
			tag_number++;
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
