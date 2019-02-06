# DXM_tests
Fault tolerant tests for Dataflow execution models. Uses Codelets Model

## Procedure
This specifically tests on the Fibonacci sequences. Redundant fib objects<\b>
which contain Codelets are introduced to see if all values match. <\b>
Otherwise, this indicates that a fault occurred.

## To Do
3 Tasks can be taken when a fault is detected (check box indicate what is completed):<br/>
- [ ] Re-run redundant tasks until all values match (**Include** HW which had fault)<br/>
- [ ] Pass the value through consensus; don't re-run redundant tasks<br/>
- [ ] Re-run redundant tasks until all values match (**Exclude** HW which had fault)<br/>

## Accomplished
- [X] Enabled re-running tasks<br/>
- [X] Checked if all values match 
