
#include <utility>
#include <iostream>

class InternalArray {
private:
    int* m_pArray = nullptr;
    unsigned int m_uiSize = 0;

public:

    InternalArray() = default;

    InternalArray(unsigned int newSize) {
        // According to the C++ standard, allocating an array with size 0 is VALID,
        // and the returned pointer is unique and non-null. However, implementations
        // may differ in behavior, and dereferencing such a pointer results in undefined
        // behavior. To ensure safety and compatibility, handle the case where newSize == 0
        // explicitly here and in other parts of code.
        m_pArray = newSize > 0 ? new int[newSize] : nullptr;
        m_uiSize = newSize;
    }

    ~InternalArray() {
        delete[] m_pArray;
    }

    InternalArray(const InternalArray& other)
    {
        if (this != &other) {
            m_uiSize = other.m_uiSize;
            m_pArray = m_uiSize > 0 ? new int[m_uiSize] : nullptr;
            for (unsigned int i = 0; i < m_uiSize; ++i) {
                m_pArray[i] = other.m_pArray[i];
            }
        }
    }

    InternalArray(InternalArray&& other) noexcept
        : m_pArray(other.m_pArray)
        , m_uiSize(other.m_uiSize)
    {
        other.m_pArray = nullptr;
        other.m_uiSize = 0;
    }

    InternalArray& operator=(const InternalArray& other)
    {
        if (this != &other) {
            delete[] m_pArray;

            m_uiSize = other.m_uiSize;
            m_pArray = m_uiSize > 0 ? new int[m_uiSize] : nullptr;
            for (unsigned int i = 0; i < m_uiSize; ++i) {
                m_pArray[i] = other.m_pArray[i];
            }
        }

        return *this;
    }

    InternalArray& operator=(InternalArray&& other) noexcept
    {
        if (this != &other) {
            delete[] m_pArray;

            m_pArray = other.m_pArray;
            m_uiSize = other.m_uiSize;

            other.m_pArray = nullptr;
            other.m_uiSize = 0;
        }

        return *this;
    }

    void fill(int value) {
        for (unsigned int i = 0; i < m_uiSize; ++i) {
            m_pArray[i] = value;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const InternalArray& array) {
        for (unsigned int i = 0; i < array.m_uiSize; ++i) {
            os << array.m_pArray[i] << " ";
        }
        return os;
    }
};

int main()
{
    InternalArray array1(5);
    array1.fill(10);
    std::cout << "Array1 (filled): " << array1 << "\n\n";

    InternalArray array2 = array1;
    std::cout << "Array2 (copied from Array1): " << array2 << "\n\n";

    InternalArray array3;
    array3 = array1;
    std::cout << "Array3 (assigned from Array1): " << array3 << "\n\n";

    InternalArray array4 = std::move(array1);
    std::cout << "Array4 (moved from Array1): " << array4 << std::endl;
    std::cout << "Array1 (after move): " << array1 << "\n\n";

    InternalArray array5;
    array5 = std::move(array2);
    std::cout << "Array5 (moved from Array2): " << array5 << std::endl;
    std::cout << "Array2 (after move): " << array2 << "\n\n";

    return 0;
}
