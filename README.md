# Stack Machine

Stack Machine emulates physical machine, operands in memory structure are stored in stack data structure.

<pre>
|   +   | <- operator
|-------|
|   4   | <- 2. top of stack (push 4)
|-------|
|   3   | <- 1. top of stack (push 3)
|-------|

|   4   | <- 3. top of stack (pop)
|-------|
|   3   |
|-------|
|       |
|-------|

|   3   | <- 4. top of stack (pop)
|-------|
|       |
|-------|
|       |
|-------|

|       |
|-------|
|       |
|-------|
|   7   | <- 5. top stack (push = 3 + 4)
|-------|

HALT
</pre>

Ones program finishes, machine goes into HALT STATE.

During lifetime of machine, it passes though various states and stages:

#### STATES
- **STATE_RESET:** Initial state of machine
- **STATE_RUN:** Running state of machine [executing program]
- **STATE_HALT:** Stopped state of machine [program finished]

#### STAGES
- **FETCH:** It fetches the instruction from memory
- **DECODE:** Decodes the type of instruction [operator or operand]
- **EXECUTE:** Performs action based on decoded output.

_Inspiration: [@Philip Bohun](https://github.com/pbohun)_
