#include "archlab.hpp"
#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "CNN/canela.hpp"

extern model_t * build_model(const dataset_t & ds);
extern void train_model(model_t * model, dataset_t & train, int epochs);
extern double test_model(model_t * model, dataset_t & test);

int main(int argc, char *argv[])
{
	std::vector<std::string> dataset_s;
	int scale_factor;
	
	std::vector<std::string> default_set;
	default_set.push_back("mnist");
	std::vector<int> mhz_s;
	std::vector<int> default_mhz;

	bool disable_prefetcher;
	load_frequencies();
	default_mhz.push_back(cpu_frequencies_array[0]);
	
    	archlab_add_option<int>("scale", scale_factor, 1, "The scale factor.  Bigger (smaller) numbers mean longer (shorter) run times by running more samples.  The default is 10, which should allow optimized code to run to completion without timing out.  If you want to run without opts, turn it down.");
	

	archlab_add_flag("no-prefetch", disable_prefetcher, false ,  "Disable the HW prefetcher.");
	archlab_add_option<std::vector<std::string> >("dataset",
						      dataset_s,
						      default_set,
						      "mnist",
						      "Which dataset to use: 'mnist', 'emnist', 'cifar10', or 'cifar100'. "
						      "Pass it multiple times to run multiple datasets.");


	std::stringstream clocks;
	for(int i =0; cpu_frequencies_array[i] != 0; i++) {
		clocks << cpu_frequencies_array[i] << " ";
	}
	std::stringstream fastest;
	fastest << cpu_frequencies_array[0];
	archlab_add_option<std::vector<int> >("MHz",
					      mhz_s,
					      default_mhz,
					      fastest.str(),
					      "Which clock rate to run.  Possibilities on this machine are: " + clocks.str() +
					      ".  Pass it multiple times to run the experiment and multiple frequencies.");
	archlab_parse_cmd_line(&argc, argv);

	for(auto & ds: dataset_s) {
		std::cout << "Running " << ds << "\n";
		
		dataset_t *train = new dataset_t;
		dataset_t *test = new dataset_t;
	
		if (ds == "mnist") {
			*train = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/mnist/mnist-train.dataset", 200 * scale_factor);
			*test = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/mnist/mnist-test.dataset", 200 * scale_factor);
		} else if (ds == "emnist") {
			*train = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/mnist/emnist-byclass-train.dataset", 150 * scale_factor);
			*test = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/mnist/emnist-byclass-test.dataset", 150 * scale_factor);
		} else if (ds == "cifar10") {
			*train = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/cifar/cifar10_data_batch_1.dataset", 100 * scale_factor);
			*test = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/cifar/cifar10_test_batch.dataset", 100 * scale_factor);
		} else if (ds == "cifar100") {
			*train = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/cifar/cifar100_train.dataset", 5 * scale_factor);
			*test = dataset_t::read(std::string(std::getenv("CANELA_ROOT")) + "/datasets/cifar/cifar100_test.dataset", 5 * scale_factor);
		}

		theDataCollector->register_tag("dataset", ds);
		theDataCollector->register_tag("training_inputs_count", train->test_cases.size());
		//theDataCollector->register_tag("test_inputs_count", test->test_cases.size());

		model_t * model = build_model(*train);

		std::cout << model->geometry() << "\n";
		std::cout << "Training data size: " << (train->get_total_memory_size()+0.0)/(1024*1024)  << " MB" << std::endl;
		std::cout << "Training cases    : " << train->size() << std::endl;
		std::cout << "Testing data size : " << (test->get_total_memory_size()+0.0)/(1024*1024)  << " MB" << std::endl;
		std::cout << "Testing cases     : " << test->size() << std::endl;

		
		//double total_error ;
		for(auto & mhz: mhz_s) {
			ArchLabTimer timer; // create it.
			pristine_machine();
			set_cpu_clock_frequency(mhz);
			if (disable_prefetcher) {
				theDataCollector->disable_prefetcher();
			}
			timer.go();
			train_model(model, *train, 1);
			//total_error = test_model(model, *test);
		}
		//std::cout << total_error << "\n";
		delete test;
		delete train;

	}
	
	archlab_write_stats();
	return 0;
}
