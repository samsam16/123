# Students should not edit this file, since changes here will _only_
# affect how your code runs locally.  It will not change how your code
# executes in the cloud.
from ArchLab.CSE141Lab import CSE141Lab, test_configs
import functools
import logging as log
import unittest
from gradescope_utils.autograder_utils.decorators import weight, partial_credit, visibility
import parameterized
import os
import re

 
class ThisLab(CSE141Lab):
    def __init__(self, **kwargs):
        super(ThisLab, self).__init__(
            lab_name = "Characterizing a Perceptron",
            short_name = "characterizing",
            output_files = ['*.csv', '*.gprof', '*.s'],
            input_files = ['code.cpp', 'config.env', 'outputs/*', "README.md"],
            repo = kwargs.get("repo") or "https://github.com/NVSL/CSE141pp-Lab-Characterizing-A-Perceptron.git",
            reference_tag = kwargs.get("reference_tag") or "master",
            timeout = 240,
        )
        
    class MetaRegressions(CSE141Lab.MetaRegressions):
        def setUp(self):
            CSE141Lab.MetaRegressions.setUp(self)
            self.lab_spec = ThisLab.load(".")

        @parameterized.parameterized.expand(test_configs("solution", "."))
        def test_solution(self, solution, flags):
            if flags.devel and "solution" == solution:
                self.skipTest("Skipping since this solution doesn't work in devel mode")
            
            result, tag = self.run_solution(solution, flags)

            js = result.results

            self.assertFileExists("inst_mix.csv", tag)
            self.assertFileExists("pe.csv", tag)
            self.assertFileExists("code.s", tag)

            b = self.read_text_file("inst_mix.csv", tag)
            runtime = self.lab_spec.csv_extract_by_line(b, 'runtime')
            
            if solution == ".":
                self.assertLess(runtime, 30)
                if flags.grades_valid():
                    self.assertEqual(float(js['gradescope_test_output']['score']), 1, f"Failed on {tag}: score check")
            elif solution == "solution":
                self.assertLess(runtime, 30)
                if flags.grades_valid():
                    self.assertEqual(float(js['gradescope_test_output']['score']), 8, f"Failed on {tag}: score check")

            if flags.gprof:
                self.assertFileExists("inst_mix.gprof", tag)
                self.assertFileExists("pe.gprof", tag)

