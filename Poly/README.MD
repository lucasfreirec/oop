# Polynomial Mini-Calculator

This project consists of a C++ class named `Poly` capable of representing polynomials of any degree, offering various functionalities for manipulating these polynomials. Below are the main details about the project specification and the functionalities implemented in the `Poly` class.

## Polynomial Specifications

A polynomial \( P(x) \) of degree \( n \) with \( n+1 \) real coefficients \( a_i \) is represented by the expression:

\[ P(x) = a_nx^n + a_{n-1}x^{n-1} + \ldots + a_1x + a_0 \]

The polynomial of degree \( n \) is internally stored in the `Poly` class by two data:
- An integer `degree`.
- A pointer pointing to an array with \( n+1 \) real numbers (`double`).

Empty polynomials are represented by a degree equal to -1 and a null pointer.

## Implemented Functionalities

1. **Constructors**: Default, copy, move, and specific, in addition to the destructor.
2. **Assignment Operators**: Overload of `operator=` by copy and by move.
3. **Query Method**: `getGrau` returns the degree of the polynomial.
4. **Query Method**: `getCoef` returns the value of the \( i \)-th coefficient of the polynomial.
5. **Modification Method**: `setCoef` changes the value of the \( i \)-th coefficient of the polynomial.
6. **Overload of `operator[]`**: Accesses the coefficients of the polynomial.
7. **Method `recriar`**: Redefines the degree and coefficients of the polynomial.
8. **Overload of `operator==`**: Checks if two polynomials are equal.
9. **Overload of `operator!=`**: Checks if two polynomials are different.
10. **Method `empty`**: Checks if the polynomial is empty.
11. **Method `isZero`**: Checks if the polynomial is of degree zero and has a null coefficient.
12. **Query Method `getValor`**: Returns the value of the polynomial for a given real value of \( x \).
13. **Overload of `operator()`**: Calculates the value of the polynomial for a given value of \( x \).
14. **Overload of `operator<<`**: Writes a polynomial in its usual representation.
15. **Overload of `operator>>`**: Allows the user to input coefficients of a `Poly`.
16. **Method `salvar`**: Writes a polynomial to a file.
17. **Method `ler`**: Reads a polynomial from a file.
18. **Overload of unary `-`**: Returns the negative of a polynomial.
19. **Overload of binary `+` and `-`**: Performs addition and subtraction of two polynomials.
20. **Overload of `operator*`**: Returns the product of two polynomials.

## Polynomial Mini-Calculator

The main program is a mini-calculator for polynomials that offers the following options at each iteration:
- Enter a new polynomial.
- Add polynomials.
- Subtract polynomials.
- Multiply polynomials.
- Calculate the last polynomial for a value of \( x \).
- Invert the sign of the last polynomial.
- Swap polynomials.
- Terminate.

At the beginning, the program reads the files `poly_P1.txt`, `poly_P2.txt`, and `poly_result.txt`, if they exist, to set initial values for the polynomials. With each operation, the polynomials are saved in these files so that the program continues from where it left off upon restarting.

---
This README was automatically generated from the provided project specifications.