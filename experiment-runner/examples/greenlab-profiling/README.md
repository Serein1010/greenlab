# GreenLab Profiling

## Configurations

Please add a file named `fabconfig.yml` to `experiment-runner` folder with the following content:

```yaml
hosts:
  raspberrypi:
    hostname: xxx.xxx.xxx.xxx
    user: xxx
    password: "xxx"
  codepath: "~/xxx/xxx/codes/"
```

in which the codepath has the following structure:

```shell
codes
├── handwritten
│   ├── fasta.py
│   ├── fasta.cpp
│   ├── ...
├── gpt
│   ├── fasta.py
│   ├── fasta.cpp
│   ├── ...
```

## Dependencies

The necessary dependencies and their corresponding installation derivatives on Raspberry Pi are listed below:

- `sysstat` for Disk I/O monitoring

  ```shell
  sudo apt-get install sysstat
  ```

- `gmpy2` which is used in `pidigits.py`

  ```shell
  sudo apt-get install libmpfr-dev
  sudo apt-get install libgmp-dev libmpc-dev
  pip install gmpy2
  ```

  If it's not working, try the following:

  ```shell
  sudo pip3 install gmpy
  sudo pip3 install gmpy2
  ```

- `Node.js` for JavaScript execution

  ```shell
  sudo apt-get install nodejs
  ```

- `npm` for Node.js package management

  ```shell
  sudo apt-get install npm
  ```

- `big-integer` for JavaScript big integer support

  ```shell
  npm install big-integer
  ```

- `Java` for Java compilation and execution

  ```shell
  sudo apt update
  sudo apt install openjdk-11-jdk
  java -version
  ```

- `gcc` for C/C++ compilation and related libraries

  ```shell
  sudo apt-get update
  sudo apt-get install g++
  sudo apt-get install libpthread-stubs0-dev
  sudo apt-get install build-essential
  ```

- Enter the `~/xxx/xxx/codes/handwritten` folder and run the following command to compile the C/C++ codes:

  ```shell
  g++ -std=c++20 -O3 -o fasta fasta.cpp -lpthread
  g++ -c -pipe -O3 -fomit-frame-pointer -march=native -std=c++14 knucleotide.cpp -o knucleotide.o
  g++ knucleotide.o -o knucleotide -Wl,--no-as-needed -lpthread
  g++ -c -pipe -O3 -fomit-frame-pointer -march=native -std=c++14 -g pidigits.cpp -o pidigits.o
  g++ pidigits.o -o pidigits -lgmp -lgmpxx
  sudo apt-get install g++ libpcre++-dev
  sudo apt-get install libpcre3-dev
  g++ -c -pipe -O3 -fomit-frame-pointer -march=native -std=c++17 -fopenmp -flto regexredux.cpp -o regexredux.o
  g++ regexredux.o -o regexredux -fopenmp -lpcre
  g++ revcomp.cpp -o revcomp -lpthread -O3
  ```

- Enter the `~/xxx/xxx/codes/gpt` folder and run the following command to compile the C/C++ codes:

  ```shell
  g++ fasta.cpp -o fasta
  g++ knucleotide.cpp -o knucleotide -O3
  g++ pidigits.cpp -o pidigits -O3 -lmpfr -lgmp
  g++ regexredux.cpp -o regexredux -O3 -std=c++11
  g++ revcomp.cpp -o revcomp -O3
  ```

- Enter the `~/xxx/xxx/codes/handwritten` folder and run the following command to compile the Java codes:

  ```shell
  javac binarytrees.java
  javac fasta.java
  javac regexredux.java
  javac spectralnorm.java
  javac revcomp.java
  ```

- Enter the `~/xxx/xxx/codes/gpt` folder and run the following command to compile the Java codes:

  ```shell
  javac binarytrees.java
  javac fasta.java
  javac regexredux.java
  javac spectralnorm.java
  javac revcomp.java
  ```

- Enter the `~/xxx/xxx/codes/handwritten` folder and run the following command to compile the C codes:

  ```shell
  gcc fasta.c -o fasta.compiled
  gcc knucleotide.c -o knucleotide.compiled
  gcc pidigits.c -o pidigits.compiled -lgmp -lm
  gcc -pipe -Wall -O3 -fomit-frame-pointer nbody.c -o nbody.compiled -lm
  gcc -pipe -Wall -O3 -fomit-frame-pointer -fopenmp spectralnorm.c -o spectralnorm.compiled -lm
  ```

- Enter the `~/xxx/xxx/codes/gpt` folder and run the following command to compile the C codes:

  ```shell
  gcc fasta.c -o fasta.compiled
  gcc knucleotide.c -o knucleotide.compiled
  gcc pidigits.c -o pidigits.compiled -lgmp -lm
  gcc -pipe -Wall -O3 -fomit-frame-pointer nbody.c -o nbody.compiled -lm
  gcc -pipe -Wall -O3 -fomit-frame-pointer -fopenmp spectralnorm.c -o spectralnorm.compiled -lm
  ```

## Run

#### Start Experiment
Run the following command **in the folder** `greenlab/experiment-runner`:

```shell
python experiment-runner/ examples/greenlab-profiling/RunnerConfig.py
```

or if you are using `python3` as the Python 3 alias:

```shell
python3 experiment-runner/ examples/greenlab-profiling/RunnerConfig.py
```

#### Data Merging
Before merging, confirm that the name of the files to be merged have the corresponding index number.

For example, we can merge the monsoon power consumption data(stored in `HV Output1.csv`) with the experiment-runner data(stored in `experiments/new_runner_experiment/run_table.csv`).

But we cannot do so if the monsoon power consumption data is stored in `HV Output6.csv`, because their index number are not corresponding to each other.



Run the folloiwng command in `greenlab/experiment-runner/examples/greenlab-profiling`
```shell
python CalPower.py
```

