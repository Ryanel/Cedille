# Scheduling

Scheduling in Cédille is preemptive.
The scheduler uses 3 bins: a queue of active processes, a list of blocked processes, and a sleeping delta-queue.

## Active Processes

Active processes are processes that are currently able to be scheduled to run on a CPU.

Each process is assigned a usage factor. While a process is being run, it's usage factor will go up. The longer a process does not execute, the lower it's usage factor will go, until it is at the minimum. Blocked processes also have their usage factor decay.

The usage factor is based on the amount of real-time has passed and the amount of time that the process has run.

The usage factor will affect the priority of the process. A lower usage factor will mean a higher priority. Priority can also be influenced by a factor called 'niceness'. System processes have a niceness of 100, while user processes have a niceness of 0. This means that System processes will almost-always be scheduled over user processes, but system processes are far more likely to be blocked.

Every tick, the scheduler will look for the highest priority process to schedule. If it cannot schedule any processes, the scheduler will try again next clock cycle.

## Blocked Processes

Blocked processes wait for events, such as an signal, or lock.

Processes will wait in a list until they are unblocked.

## Sleeping Processes

Sleeping processes are put into a delta-queue, sorted by how many ticks are remaining before a process is awoken.

If a process is uninteruptable, it will not respond to an early-wake. Instead, the process that attempted to wake the uninteruptable process will block.

## Multiprocessing

Each CPU will have it's own set of each queue.
