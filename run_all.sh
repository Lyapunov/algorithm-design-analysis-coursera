#!/bin/bash
set -x

./app/unit_tests

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
./app/assignment_c2w3p1 inputs/input_c2w3p1_target.txt
./app/assignment_c2w3p2 inputs/input_c2w3p1_test_1.txt
./app/assignment_c2w3p2 inputs/input_c2w3p1_target.txt
./app/assignment_c2w3p2 inputs/input_c2w3p2_target.txt
