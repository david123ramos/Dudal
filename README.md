# Dudal

*Dudal* is a programming language created solely for fun, falling under the category of recreational programming.

*Dudal* is based on Reverse Polish Notation (RPN), wherein each expression is stacked on a data structure before the instruction executes. This implies that instructions use the values on the top of the main stack.

## Examples:

1. Example of printing 'Hello, World!' on stdio represented by ':' -> 'Hello, World!':
2. Example of a simple sum -> `1 1 +`

In the second example, the value 1 is stacked twice in the main stack, and the '+' operator uses the first and second elements from the top of the stack, storing the result again in the stack.

## Equality Operations:

- `1 1 + 2 ==` -> This returns true (or 1) because 2 is equal to 2.
- `1 1 + 2 !=` -> (not equal) returns false
- `1 1 + 3 >` -> (greater than) returns false
- `1 1 + 3 <` -> (less than) returns true

## Additional Operations:

- `3 2 *` -> (multiplication) returns 6
- `10 2 ^` -> (power) returns 100
- `10 2 ^ 50 - 2 /` -> returns 25

## Planned Features:

- [ ] Abstract Syntax Tree (AST)
- [ ] Conditionals (`if...elsif...else`)
- [ ] Loops (`1 2 while {}`)
- [ ] Variables (`$a = 1`)
- [ ] Functions (e.g., `func(a, b, c) {}`)
