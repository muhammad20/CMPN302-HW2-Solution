import sys
import os
import random


def is_int(x):
    try:
        int(x)
        return True
    except ValueError:
        return False


data_count = 10000
unsorted_data_file = "p1_data.in"  # the file to generate data in.
reversed_data_file = "p1_data.out"  # the file where the output of code exists.
running_time_file = "p1.time"  # the file to write the running time.
data = []

print("Generating data...")

with open(unsorted_data_file, "w") as file:
    delimiter = ''
    for i in range(data_count):
        randint = random.randint(1, 50000)
        data.append(randint)
        line = delimiter + str(randint)
        delimiter = '\n'
        file.write(line)

print("Compiling source code...")
os.system("g++ -std=c++11 hw2_p1.cpp -o output")
# If the compilation raised an exception, try this command instead (comment the previous line and uncomment the next one)
# os.system("g++ -o output reverse.cpp -std=c++11")

print("Running binary file...")
os.system("./output " + str(unsorted_data_file) + " " +
          str(reversed_data_file) + " " + str(running_time_file))

print("Validating output...")
data = list(reversed(data))

with open(reversed_data_file, "r") as file:
    try:
        i = 0
        for line in file:
            if line == "\n":
                continue
            if not is_int(line):
                raise ValueError("Wrong Submission A")
            if int(line) != data[i]:
                raise ValueError("Wrong Submission B")
            i = i + 1
        if i != data_count:
            raise ValueError("Wrong Submission C")
        print("Correct Submission")
    except ValueError as err:
        print(err.args[0])
