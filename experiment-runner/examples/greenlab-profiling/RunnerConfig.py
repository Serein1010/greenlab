from EventManager.Models.RunnerEvents import RunnerEvents
from EventManager.EventSubscriptionController import EventSubscriptionController
from ConfigValidator.Config.Models.RunTableModel import RunTableModel
from ConfigValidator.Config.Models.FactorModel import FactorModel
from ConfigValidator.Config.Models.RunnerContext import RunnerContext
from ConfigValidator.Config.Models.OperationType import OperationType
from ProgressManager.Output.OutputProcedure import OutputProcedure as output

import yaml
from fabric import Connection

from typing import Dict, List, Any, Optional
from pathlib import Path
from os.path import dirname, realpath

import psutil
import time
import pandas as pd
import threading


class RunnerConfig:
    ROOT_DIR = Path(dirname(realpath(__file__)))

    # ================================ USER SPECIFIC CONFIG ================================
    """The name of the experiment."""
    name: str = "new_runner_experiment"

    """The path in which Experiment Runner will create a folder with the name `self.name`, in order to store the
    results from this experiment. (Path does not need to exist - it will be created if necessary.)
    Output path defaults to the config file's path, inside the folder 'experiments'"""
    results_output_path: Path = ROOT_DIR / "experiments"

    """Experiment operation type. Unless you manually want to initiate each run, use `OperationType.AUTO`."""
    operation_type: OperationType = OperationType.AUTO

    """The time Experiment Runner will wait after a run completes.
    This can be essential to accommodate for cooldown periods on some systems."""
    time_between_runs_in_ms: int = 10000

    # Dynamic configurations can be one-time satisfied here before the program takes the config as-is
    # e.g. Setting some variable based on some criteria
    def __init__(self):
        """Executes immediately after program start, on config load"""

        EventSubscriptionController.subscribe_to_multiple_events(
            [
                (RunnerEvents.BEFORE_EXPERIMENT, self.before_experiment),
                (RunnerEvents.BEFORE_RUN, self.before_run),
                (RunnerEvents.START_RUN, self.start_run),
                (RunnerEvents.START_MEASUREMENT, self.start_measurement),
                (RunnerEvents.INTERACT, self.interact),
                (RunnerEvents.STOP_MEASUREMENT, self.stop_measurement),
                (RunnerEvents.STOP_RUN, self.stop_run),
                (RunnerEvents.POPULATE_RUN_DATA, self.populate_run_data),
                (RunnerEvents.AFTER_EXPERIMENT, self.after_experiment),
            ]
        )
        self.run_table_model = None  # Initialized later
        self.fabconfig = None
        self.c = None
        self.t = None
        self.pid = None
        self.cpu_usage = None
        self.stop_measurement_thread = False
        self.stop_run_thread = False
        output.console_log("Custom config loaded")

    def create_run_table_model(self) -> RunTableModel:
        """Create and return the run_table model here. A run_table is a List (rows) of tuples (columns),
        representing each run performed"""

        factor_algo = FactorModel(
            "Algorithm", ["fasta", "knucleotide", "pidigits", "regexredux", "revcomp"]
        )
        # TODO: add other languages
        factor_language = FactorModel("Language", ["py", "js", "cpp"])
        factor_gpt = FactorModel("GPT", [False, True])
        # TODO: enable repetitions in formal experiments
        factor_repetitions = FactorModel("Repetitions", list(range(1, 6)))

        self.run_table_model = RunTableModel(
            factors=[factor_algo, factor_language, factor_gpt, factor_repetitions],
            data_columns=[
                "avg_cpu",
                "avg_mem",
                "avg_disk_io",
                "start_time",
                "end_time",
                "run_time",
            ],
        )

        return self.run_table_model

    def before_experiment(self) -> None:
        """Perform any activity required before starting the experiment here
        Invoked only once during the lifetime of the program."""

        output.console_log("Config.before_experiment() called!")

        # Load the connection configuration
        with open("fabconfig.yml") as f:
            self.fabconfig = yaml.safe_load(f)

        host = self.fabconfig["hosts"]["raspberrypi"]

        # Replace the following parameters with your own Raspberry Pi's IP address, username and password
        # For start_run() and stop_run()
        self.c = Connection(
            host["hostname"],
            user=host["user"],
            connect_kwargs={"password": host["password"]},
        )
        # For start_measurement() and stop_measurement()
        self.t = Connection(
            host["hostname"],
            user=host["user"],
            connect_kwargs={"password": host["password"]},
        )

    def before_run(self) -> None:
        """Perform any activity required before starting a run.
        No context is available here as the run is not yet active (BEFORE RUN)"""

        output.console_log("Config.before_run() called!")

    def start_run(self, context: RunnerContext) -> None:
        """Perform any activity required for starting the run here.
        For example, starting the target system to measure.
        Activities after starting the run should also be performed here."""

        output.console_log("Config.start_run() called!")

        algo = context.run_variation["Algorithm"]
        lang = context.run_variation["Language"]
        gpt = context.run_variation["GPT"]

        gpt_path = "handwritten"

        if gpt:
            print(f"Running {algo} in {lang} with GPT generated code")
            gpt_path = "gpt"
        else:
            print(f"Running {algo} in {lang} with handwritten code")
            gpt_path = "handwritten"

        if lang == "py":
            if algo == "helloworld":
                # for test purpose
                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'nohup python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "fasta":
                # TODO: change input size
                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py 1000000',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "knucleotide":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "pidigits":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py 100',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "regexredux":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "revcomp":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'python -OO {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.py < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

        if lang == "js":
            if algo == "fasta":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'node {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.js 1000000',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "knucleotide":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'node {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.js < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "pidigits":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'node {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.js 100',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "regexredux":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'node {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.js < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "revcomp":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'node {self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo}.js < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

        if lang == "cpp":
            if algo == "fasta":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'{self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo} 1000000',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "knucleotide":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'{self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo} < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "pidigits":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'{self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo} 100',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "regexredux":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'{self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo} < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

            if algo == "revcomp":

                def run_thread():
                    if not self.stop_run_thread:
                        self.c.run(
                            f'{self.fabconfig["hosts"]["codepath"]}{gpt_path}/{algo} < {self.fabconfig["hosts"]["codepath"]}handwritten/input1000.txt',
                            hide=True,
                        )

                self.c_thread = threading.Thread(target=run_thread)
                self.c_thread.start()

    def start_measurement(self, context: RunnerContext) -> None:
        """Perform any activity required for starting measurements."""
        output.console_log("Config.start_measurement() called!")

        cpu_profiler_cmd = "top -b -n1 | grep 'Cpu(s)' | awk '{print $2 + $4}'"
        mem_profiler_cmd = "free | grep Mem | awk '{print $3/$2 * 100.0}'"
        disk_io_profiler_cmd = "iostat -dx | awk 'NR==4 {print $14}'"

        self.system_usage_data = []  # Create a list to store system usage data
        self.start_time = time.time()  # Record the start time

        def profiler_thread():
            while not self.stop_measurement_thread:
                cpu_result = self.t.run(cpu_profiler_cmd, hide=True)
                mem_result = self.t.run(mem_profiler_cmd, hide=True)
                disk_io_result = self.t.run(disk_io_profiler_cmd, hide=True)

                cpu_usage = float(cpu_result.stdout.strip())
                mem_usage = float(mem_result.stdout.strip())
                disk_io = float(disk_io_result.stdout.strip())

                self.system_usage_data.append(
                    [cpu_usage, mem_usage, disk_io]
                )  # Store system usage data

                print(
                    f"CPU Usage: {cpu_usage}%, Memory Usage: {mem_usage}%, Disk I/O: {disk_io}"
                )
                time.sleep(1)

            self.end_time = time.time()  # Record the end time

        self.t_thread = threading.Thread(target=profiler_thread)
        self.t_thread.start()

    def interact(self, context: RunnerContext) -> None:
        """Perform any interaction with the running target system here, or block here until the target finishes."""

        output.console_log("Config.interact() called!")

    def stop_measurement(self, context: RunnerContext) -> None:
        """Perform any activity here required for stopping measurements."""

        output.console_log("Config.stop_measurement called!")

    def stop_run(self, context: RunnerContext) -> None:
        """Perform any activity here required for stopping the run.
        Activities after stopping the run should also be performed here."""

        output.console_log("Config.stop_run() called!")

        # Stop the thread that gets the CPU usage
        self.stop_run_thread = True
        self.c_thread.join()
        # The reason we need to stop the measurement thread here is because the experiment runner
        # will call stop_measurement(), which can be stopped as soon as possible, before stop_run(), which lasts for a while
        self.stop_measurement_thread = True
        self.t_thread.join()
        self.cpu_usage = None

    def populate_run_data(self, context: RunnerContext) -> Optional[Dict[str, Any]]:
        """Parse and process any measurement data here.
        You can also store the raw measurement data under `context.run_dir`
        Returns a dictionary with keys `self.run_table_model.data_columns` and their values populated
        """

        df = pd.DataFrame(
            self.system_usage_data, columns=["cpu_usage", "mem_usage", "disk_io"]
        )  # Use the system_usage_data list

        df.to_csv(context.run_dir / "raw_data.csv", index=False)

        run_time = self.end_time - self.start_time  # Calculate the run time

        run_data = {
            "avg_cpu": round(df["cpu_usage"].mean(), 3),
            "avg_mem": round(df["mem_usage"].mean(), 3),
            "avg_disk_io": round(df["disk_io"].mean(), 3),
            "start_time": self.start_time,
            "end_time": self.end_time,
            "run_time": run_time,
        }
        return run_data

    def after_experiment(self) -> None:
        """Perform any activity required after stopping the experiment here
        Invoked only once during the lifetime of the program."""

        output.console_log("Config.after_experiment() called!")

        self.c.close()
        self.t.close()

    # ================================ DO NOT ALTER BELOW THIS LINE ================================
    experiment_path: Path = None
