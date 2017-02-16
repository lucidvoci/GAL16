import os
import sys
import time

num_of_graphs = 5

def main():
    if len(sys.argv) != 2:
        print "First parameter - select version 1 to 6"
        print "Second parameter - select version -s to -p"
        return 1
    if int(sys.argv[1]) not in [1, 2, 3, 4, 5, 6]:
        print "First parameter - select version 1 to 6"
        return 1
    for i in range(0, num_of_graphs):
        print "Generating " + str(i) + " ..."
        os.system("./main -g " + sys.argv[1])
        print "Copying " + str(i) + " ..."
        os.system("cp input.txt input_" + sys.argv[1] + "_" + str(i) + ".txt");
        time.sleep(0.2)
    for i in range(0, num_of_graphs):
        #print "Running " + str(i) + " ..."
        os.system("./main -s input_" + sys.argv[1] + "_" + str(i) + ".txt")
        os.system("./main -p input_" + sys.argv[1] + "_" + str(i) + ".txt")

if __name__ == "__main__": main()
