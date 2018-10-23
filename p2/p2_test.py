import sys
import os
import random
import collections


def is_int(x):
    try:
        int(x)
        return True
    except ValueError:
        return False


data_count = 1000
unsorted_data_file = "p2_data.in"  # the file to generate data in.
frequent_data_file = "p2_data.out"  # the file where the output of code exists.
running_time_file = "p2.time"  # the file to write the running time.
data = []

print("Generating data...")
with open(unsorted_data_file, "w") as file:
    delimiter = ''
    for i in range(data_count):
        randint = abs(int(random.normalvariate(100, 50)))
        data.append(randint)
        line = delimiter + str(randint)
        delimiter = '\n'
        file.write(line)

frequent_data = []
counter = collections.Counter(data)
sorted_tuples = counter.most_common()
for tuple in sorted_tuples:
    for i in range(tuple[1]):
        frequent_data.append(tuple[0])

print("Compiling source code...")
os.system("g++ -std=c++11 hw2_p2.cpp -o output")
# If the compilation raised an exception, try this command instead (comment the previous line and uncomment the next one)
# os.system("g++ -o output frequencies.cpp -std=c++11")
print("Running binary file...")
os.system("./output " + str(unsorted_data_file) + " " +
          str(frequent_data_file) + " " + str(running_time_file))

print("Validating output...")

with open(frequent_data_file, "r") as file:
    try:
        i = 0
        for line in file:
            if line == "\n":
                continue
            if not is_int(line):
                raise ValueError("Wrong Submission A")
            if int(line) != frequent_data[i]:
                raise ValueError("Wrong Submission B")
            i = i + 1
        if i != data_count:
            raise ValueError("Wrong Submission C")
        print("Correct Submission")
    except ValueError as err:
        print(err.args[0])
