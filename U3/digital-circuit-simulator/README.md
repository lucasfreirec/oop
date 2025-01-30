# Digital Circuit Simulator

This is a digital circuit simulator developed in C++ as part of an academic project. The aim is to simulate circuits composed of logical gates with 2 or more inputs, including: NOT, AND, NAND, OR, NOR, XOR, and NXOR.

## Features

- Support for modeling logical gates using polymorphic objects.
- User input for the number of circuit inputs and outputs, the number of logical gates, and the characteristics of each gate.
- Reading and writing of circuits in files following a specific format.

## Circuit Files

The circuit files follow a standard format:

```
CIRCUITO <Nin> <Nout> <Nportas>
PORTAS
id_port) type n_in: id_orig_in1 … id_orig_inn_in
...
id_port) type n_in: id_orig_in1 … id_orig_inn_in
SAIDAS
id_out) id_orig_out
...
id_out) id_orig_out
```

### File Example

```
CIRCUITO 3 3 6
PORTAS
1) OR 3: 5 -2 6
2) XO 2: 5 3
3) NO 2: 2 6
4) NT 1: -2
5) AN 2: -1 4
6) NA 2: -2 -3
SAIDAS
1) 2
2) 1
3) 3
```

## Notes

- The simulator uses polymorphic objects to model logical gates, avoiding control flow statements based on gate type.
- File reading allows for some flexibility in format, as long as the necessary information is present and in the correct order.

---
This README was automatically generated from the provided project specifications.
