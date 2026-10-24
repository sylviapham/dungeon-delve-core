# Week 01 Ownership Audit - Analysis

## 1. Memory Leak

The original build_satchel() function created a Spare rune using a raw pointer but never deleted it. This caused a memory leak because the allocated memory was not released.

I fixed this by using std::unique_ptr, which automatically frees the memory when the rune goes out of scope.

## 2. Double Delete

The original main() function deleted the Gale rune separately and then deleted it again during the cleanup loop. Deleting the same pointer twice can cause undefined behavior or crashes.

I fixed this by removing the manual delete statements and using std::unique_ptr to manage the runes automatically.

## 3. Dangling Reference

The original strongest_rune() function created a local variable called snapshot and returned it as a reference. Once the function ended, snapshot no longer existed, leaving a dangling reference. This could cause unpredictable behavior or crashes.

I fixed this by returning a reference to the actual Rune stored in the satchel instead of a temporary local variable. The reference remains valid as long as the Rune is still alive.


