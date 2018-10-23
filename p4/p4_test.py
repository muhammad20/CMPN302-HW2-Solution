import sys
import os
import random


def brute_force(arr, k):
    data = []
    length = len(arr)
    for x in range(0, length - k + 1):
        mini = arr[x]
        for y in range(x, x + k):
            if y >= length:
                break
            if arr[y] < mini:
                mini = arr[y]
        data.append(mini)
    return data


def is_int(x):
    try:
        int(x)
        return True
    except ValueError:
        return False


data_count = 1000  # large array to make time difference between reursive and non recursive
k = random.randint(1, data_count-1)
random_data_file = "p4_data.in"  # the file to generate data in.
output_data_file = "p4_data.out"  # the file where the output of code exists.
running_time_file = "p4.time"  # the file to write the running time.
data = []

print("Generating data...")
with open(random_data_file, "w") as file:
    for i in range(data_count):
        randint = random.randint(1, 50000)
        data.append(randint)
        file.write(str(randint)+"\n")

print("Compiling source code...")
os.system("g++ -std=c++11 hw2_p4.cpp -o output")
# If the compilation raised an exception, try this command instead (comment the previous line and uncomment the next one)
# os.system("g++ -o output P4.cpp -std=c++11")
print("Running binary file...")
os.system("./output " + str(k) + " " + str(random_data_file) +
          " " + str(output_data_file) + " " + str(running_time_file))

print("Validating output...")
data = brute_force(data, k)

with open(output_data_file, "r") as file:
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
        if i != len(data):
            raise ValueError("Wrong Submission C")
        print("Correct Submission")
    except ValueError as err:
        print(err.args[0])
