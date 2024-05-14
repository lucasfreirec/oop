# Store Stock

This project implements a simplified version of a store inventory manager in C++, demonstrating the concept of inheritance between classes. Additionally, it includes a graphical user interface (GUI) developed with Qt.

## Classes

### Product
- Stores the name and price of any item in the inventory.
- Functionalities include reading, saving, inputting, and printing information.

### Book, CD, DVD
- Inherits from the Product class.
- Stores specific data for each type of item (e.g., author for books).
- Functionalities include reading, saving, inputting, and printing information.
- Implements operators `>>` and `<<` for input and output.

### Store
- Contains containers for books, CDs, and DVDs.
- Functionalities include adding and removing items, reading and saving inventory data from/to files, and printing inventory information.

## File Format

Items are stored in a file, with one item per line. The format allows for easy reading/writing of both base and derived class information.

E.g.:

LISTALIVRO 3
L: "Summer Adventures";$21.99;"Alice"
L: "The Secret of Stars";$14.50;"Carlos"
L: "Lost Pages";$18.75;"Isabela"
LISTACD 2
C: "Heart Melodies";$10.99;9
C: "Night Rhythm";$7.25;12
LISTADVD 4
D: "The Great Journey";$19.99;86
D: "Mysteries of the Universe";$12.00;54
D: "Radical Challenge";$8.50;112
D: "Ocean Secrets";$6.75;78


## Graphical User Interface (GUI)

The GUI basically consists of four Qt classes:
- `MainLoja`: Main window of the program.
- `IncluirLivro`: Window for adding a new book.
- `IncluirCD`: Window for adding a new CD.
- `IncluirDVD`: Window for adding a new DVD.

## Additional Features

While not explicitly mentioned in this summary, the project may include other functionalities as well.

---
This README was automatically generated from the provided project specifications.