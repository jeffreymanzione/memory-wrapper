// intern.h
//
// Created on: Feb 11, 2018
//     Author: Jeff Manzione
//
// Provides unique pointers to strings.
// See: https://en.wikipedia.org/wiki/String_interning.
//
// It can be desirable to guarantee that all exact distinct string has only 1
// copy. This library allows for an inputted string to be exchanged for its
// globally unique version.
//
// intern_init();
// char *string1 = intern("unique_string");  // Creates a unique ptr.
// char *string2 = intern("unique_string");  // Returns the existing ptr.
// assert(string1 == string2); // will succeed.
// intern_finalize();

#ifndef ALLOC_ARENA_INTERN_H_
#define ALLOC_ARENA_INTERN_H_

// Initializes the string intern.
void intern_init();

// Finalizes the string intern and frees any relevant memory.
void intern_finalize();

// Interns the given [str] and returns a unique pointer to that string.
char *intern(const char str[]);

// Interns a given string range (from start to end), returning a pointer to the
// interned string.
char *intern_range(const char str[], int start, int end);

#endif /* ALLOC_ARENA_INTERN_H_ */
