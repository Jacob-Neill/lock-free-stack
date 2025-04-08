# lock-free-stack
A simple Treiber stack implementation

This Treiber stack currently uses an internal toDelete raw atomic pointer, this is an internal lock-free bag. This bag stores all nodes popped from the stack and saves them for deletion later, solving the ABA problem. It is a memory-inefficient solution.
