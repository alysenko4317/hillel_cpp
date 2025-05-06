## 📝 Task Description

This project was developed as the final assignment of the "C++ PRO" course.  
The task: find the number under a given upper bound that produces the longest **Collatz sequence**.  
The assignment included both algorithmic optimization and GUI implementation using Qt.

### 🧠 Task Requirements (translated from original)

Create a multithreaded Qt-based application that:

- Finds the number less than **N** (input by user) that produces the longest Collatz sequence.
- The Collatz rule:
  - n → n / 2 if n is even
  - n → 3n + 1 if n is odd
- Intermediate values can exceed N.
- In case of memory overflow due to large values, an informative error must be shown.
- The application must:
  - Allow selecting the number of threads via `QSlider` (from 1 to system maximum).
  - Allow input of the upper bound via `QSpinBox` (minimum 1,000,000).
  - Include `Start`, `Stop`, and `Exit` buttons with proper UI state toggling.
  - Display:
    - The number with the longest sequence
    - Length of the sequence
    - Time taken (in milliseconds)

### ⚙️ Performance Requirement

- The solution must be **as fast as possible**.
- **Multithreading** is mandatory.

---

This solution achieved exceptional results in both memory and speed optimization, earning high praise from the course instructor.
