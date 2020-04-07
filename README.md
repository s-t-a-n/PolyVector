# PolyVector
Testdriving polymorphism for vector-like structures

Implemented are:

1. Dynamic buffer
2. Static First-In-First-Out Buffer
3. Static Last-In-First-Out Buffer
4. Static First-In-First-Out Ringbuffer
4. Threadsafe Static First-In-First-Out Ringbuffer

Make vectors with vecnew, vecclone. Destroy them with vecdestroy.
Add items with push, pushback, pushfront, set, insert or safe_add in case of
threadsafe usage.
Get items with peek, peekback, peekfront, get or safe_get in case of threadsafe
usage.
Destroy items with pop, popback, popfront or remove.

See tests for examples.

Code is following (or just blatantly ripping) basic object orientation as specified
at https://www.cs.rit.edu/~ats/books/ooc.pdf
