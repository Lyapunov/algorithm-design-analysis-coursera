#!/bin/bash
set -x

./app/unit_tests

# assignment c1w1p1
./app/assignment_c1w1p1 ./inputs/input_c1w1p1_test.txt
./app/assignment_c1w1p1 ./inputs/input_c1w1p1_target.txt

# assignment c1w2p1
./app/assignment_c1w2p1 0 ./inputs/input_c1w2p1_test.txt
./app/assignment_c1w2p1 1 ./inputs/input_c1w2p1_test.txt
./app/assignment_c1w2p1 2 ./inputs/input_c1w2p1_test.txt
./app/assignment_c1w2p1 0 ./inputs/input_c1w2p1_target.txt
./app/assignment_c1w2p1 1 ./inputs/input_c1w2p1_target.txt
./app/assignment_c1w2p1 2 ./inputs/input_c1w2p1_target.txt

# assignment c1w3p1
./app/assignment_c1w3p1 inputs/input_c1w3p1_target.txt

# assignment c1w4p1
./app/assignment_c1w4p1 ./inputs/input_c1w4p1_test_1.txt
./app/assignment_c1w4p1 ./inputs/input_c1w4p1_target.txt

# assignment c1w5p1
./app/assignment_c1w5p1 ./inputs/input_c1w5p1_test.txt 1 4
./app/assignment_c1w5p1 ./inputs/input_c1w5p1_target.txt 1 7 37 59 82 99 115 133 165 188 197

# assignment c2w2p1
./app/assignment_c2w1p1 inputs/input_c2w1p1_bad.txt
./app/assignment_c2w1p1 inputs/input_c2w1p1_test_1.txt
./app/assignment_c2w1p1 inputs/input_c2w1p1_target.txt

# assignment c2w2p2
./app/assignment_c2w1p2 inputs/input_c2w1p1_test_1.txt
./app/assignment_c2w1p2 inputs/input_c2w1p1_target.txt

# assignment c2w2p3
./app/assignment_c2w1p3 inputs/input_c2w1p3_bad.txt
./app/assignment_c2w1p3 inputs/input_c2w1p3_test.txt
./app/assignment_c2w1p3 inputs/input_c2w1p3_target.txt


# assignment c2w2p1
./app/assignment_c2w2p1 inputs/input_c2w2p1_test_10_2.txt 2
./app/assignment_c2w2p1 inputs/input_c2w2p1_test_100_4.txt 4
./app/assignment_c2w2p1 inputs/input_c2w2p1_test_200_2.txt 2
./app/assignment_c2w2p1 inputs/input_c2w2p1_target.txt 2
./app/assignment_c2w2p1 inputs/input_c2w2p1_target.txt 10
./app/assignment_c2w2p1 inputs/input_c2w2p1_target.txt

# assignment c2w2p2
./app/assignment_c2w2p2 inputs/input_c2w2p2_test_200.txt
./app/assignment_c2w2p2 inputs/input_c2w2p2_test_2000.txt
./app/assignment_c2w2p2 inputs/input_c2w2p2_test_20000.txt
./app/assignment_c2w2p2 inputs/input_c2w2p2_target.txt

# assignment c2w3p1
./app/assignment_c2w3p1 inputs/input_c2w3p1_test_1.txt
./app/assignment_c2w3p1 inputs/input_c2w3p2_test_1.txt
./app/assignment_c2w3p1 inputs/input_c2w3p1_target.txt
./app/assignment_c2w3p2 inputs/input_c2w3p1_test_1.txt
./app/assignment_c2w3p2 inputs/input_c2w3p2_test_1.txt
./app/assignment_c2w3p2 inputs/input_c2w3p1_target.txt
./app/assignment_c2w3p2 inputs/input_c2w3p2_target.txt

# problem set c2w3p5
./src/problem_set_c2w3p5.py

# assignment c2w4p1
./app/assignment_c2w4p1 ./inputs/input_c2w4p1_test_1.txt
./app/assignment_c2w4p1 ./inputs/input_c2w4p1_test_2.txt
./app/assignment_c2w4p1 ./inputs/input_c2w4p1_target_1.txt
./app/assignment_c2w4p1 ./inputs/input_c2w4p1_target_2.txt
./app/assignment_c2w4p1 ./inputs/input_c2w4p1_target_3.txt

# assignment c2w5p1
./app/assignment_c2w5p1 ./inputs/input_c2w5p1_test1.txt
./app/assignment_c2w5p1 ./inputs/input_c2w5p1_test2.txt
./app/assignment_c2w5p1 ./inputs/input_c2w5p1_target.txt

# assignment c2w6p1
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_test_1.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_test_2.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_test_3.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_1.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_2.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_3.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_4.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_5.txt
./app/assignment_c2w6p1 ./inputs/input_c2w6p1_target_6.txt

