import subprocess
import statistics
import math

# Number of samples used
NO_OF_SAMPLES = 100

# Compile serial.c ,mutex.c and readWrite.c source codes
def compileAll():
    subprocess.call(['gcc', '-o', 'serial', 'serial.c'])
    subprocess.call(['gcc',  '-o', 'mutex', 'mutex.c', '-lm', '-lpthread'])
    subprocess.call(['gcc',  '-o', 'readWrite', 'readWrite.c', '-lm', '-lpthread'])

# Execute the process given by command and calculate the average and standard deviation for each cases and each type
def execute(command):
    elapsedTimes = []
    for i in range(NO_OF_SAMPLES):
        time = subprocess.Popen(command, stdout=subprocess.PIPE).communicate()[0]
        elapsedTimes.append(float(time))

    average = statistics.mean(elapsedTimes)
    standardDeviation = statistics.stdev(elapsedTimes)
    samples = math.ceil(math.pow(((100 * 1.96 * standardDeviation) / (5 * average)), 2))
    print('Average: ' + str(average))
    print('Std: ' + str(standardDeviation))
    print('Samples: ' + str(samples))

# Execute command for mutex and readwrite lock for given no of threads
def executeCommands(commands):
    for i in range(len(commands)):
        print("No of Threads: " + str(commands[i][4]))
        execute(commands[i])
        print("")


# Collection of commands to be executed
serial = [['./serial', '0.99', '0.005', '0.005'], ['./serial',  '0.9', '0.05', '0.05'], ['./serial',  '0.5', '0.25', '0.25']]
mutex_1 = [['./mutex',  '0.99', '0.005', '0.005', '1'], ['./mutex',  '0.99', '0.005', '0.005', '2'], ['./mutex',  '0.99', '0.005', '0.005', '4'],['./mutex',  '0.99', '0.005', '0.005', '8']]
mutex_2 = [['./mutex',  '0.9', '0.05', '0.05', '1'], ['./mutex',  '0.9', '0.05', '0.05', '2'], ['./mutex',  '0.9', '0.05', '0.05', '4'],['./mutex',  '0.9', '0.05', '0.05', '8']]
mutex_3 = [['./mutex',  '0.5', '0.25', '0.25', '1'], ['./mutex',  '0.5', '0.25', '0.25', '2'], ['./mutex',  '0.5', '0.25', '0.25', '4'],['./mutex',  '0.5', '0.25', '0.25', '8']]
readWrite_1 = [['./readWrite',  '0.99', '0.005', '0.005', '1'], ['./readWrite',  '0.99', '0.005', '0.005', '2'], ['./readWrite',  '0.99', '0.005', '0.005', '4'],['./readWrite',  '0.99', '0.005', '0.005', '8']]
readWrite_2 = [['./readWrite',  '0.9', '0.05', '0.05', '1'], ['./readWrite',  '0.9', '0.05', '0.05', '2'], ['./readWrite',  '0.9', '0.05', '0.05', '4'],['./readWrite',  '0.9', '0.05', '0.05', '8']]
readWrite_3 = [['./readWrite',  '0.5', '0.25', '0.25', '1'], ['./readWrite',  '0.5', '0.25', '0.25', '2'], ['./readWrite',  '0.5', '0.25', '0.25', '4'],['./readWrite',  '0.5', '0.25', '0.25', '8']]

# Three cases for mutex lock execution
mutex = [mutex_1, mutex_2, mutex_3]

# Three cases for read-Write lock execution 
readWrite = [readWrite_1, readWrite_2, readWrite_3]

# Compile files
compileAll()

# Process and display the output
for i in range(3):
    
    print('CASE ' + str(i + 1)+': No of Member Operation: ' +serial[i][1]+ ' No of Insert Operation: '+ serial[i][2] + ' No Of Delete Operation: '+serial[i][3] +'\n')
    	
    print('Serial:')
    print('-------')
    execute(serial[i])
    print('')
    print('One Mutex for entire list:')
    print('--------------------------')
    executeCommands(mutex[i])
    print('')
    print('Read-Write lock:')
    print('----------------')
    executeCommands(readWrite[i])
    print('____________________________________________________________________________________________')
    print('\n')

    
