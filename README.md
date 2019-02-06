# DXM_tests
Fault tolerant tests for Dataflow execution models. Uses Codelets Model

## Procedure
This specifically tests on the Fibonacci sequences. Redundant *fib* objects
which contain Codelets are introduced to see if all values match.
Otherwise, this indicates that a fault occurred.<br/>

## To Do
__Order the Synchronizing Unit (SU) to re-run the Computational Units (CU) Tasks. Two means to do this:__<br/>
- [ ] Create new CU (how will HW layout be affected, if at all?)<br/>
- [ ] Re-run same CU (requires modifying their dependency count)

__Three alternative solutions can be taken when a fault is detected (check box indicate what is completed):__<br/>
- [ ] Pass the value through consensus; don't re-run redundant tasks<br/>
- [ ] Re-run redundant tasks until all values match (**Include** HW which had fault)<br/>
- [ ] Re-run redundant tasks until all values match (**Exclude** HW which had fault)<br/>

__Metric Testing (compare to Fib w/o error correction and compare to OpenMP?)__
- [ ] Measure Execution Time (ET)
- [ ] Measure HW Temp
- [ ] Create script to collect and graph data

## Accomplished
__Add redundancy to Fib__
- [X] Enabled redundant tasks<br/>
- [X] Checked if all values match 
- [X] Invoke errors to occur with given probability
