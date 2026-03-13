/**
 * 动态数组实现 (Dynamic Array Implementation)
 *
 * 核心思想 / Core Idea:
 *   用一块连续的堆内存模拟可变长数组。
 *   Use a contiguous heap allocation to simulate a resizable array.
 *
 * 扩容策略 / Growth Strategy:
 *   - 容量不足时翻倍 (double when full)
 *   - 元素数量降至容量 1/4 时缩为一半 (shrink to half when size == cap/4)
 *   这两条规则保证所有操作均摊 O(1) 时间。
 *   These two rules guarantee O(1) amortized time for all operations.
 */

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

/**
 * MyArrayList<E> —— 泛型动态数组
 * MyArrayList<E> —— Generic Dynamic Array
 *
 * 成员变量 / Fields:
 *   data : 底层原生数组指针 / pointer to underlying raw array
 *   size : 当前存储的元素个数 / current number of stored elements
 *   cap  : 底层数组的最大容量 / maximum capacity of the underlying array
 */
template<typename E>
class MyArrayList {
private:
    // 真正存储数据的底层数组 / Underlying raw array that stores the elements
    E* data;

    // 记录当前元素个数 / Number of elements currently stored
    int size;

    // 底层数组的最大容量 / Maximum capacity of the underlying array
    int cap;

    // 默认初始容量（故意设为 1 以便快速触发扩容演示）
    // Default initial capacity (set to 1 to easily trigger resize for demo)
    static const int INIT_CAP = 1;

public:
    /**
     * 默认构造函数：以 INIT_CAP 为初始容量创建数组
     * Default constructor: creates array with INIT_CAP initial capacity
     */
    MyArrayList() {
        this->data = new E[INIT_CAP];
        this->size = 0;
        this->cap  = INIT_CAP;
    }

    /**
     * 指定初始容量的构造函数
     * Constructor with a user-specified initial capacity
     *
     * @param initCapacity 初始容量 / initial capacity
     */
    MyArrayList(int initCapacity) {
        this->data = new E[initCapacity];
        this->size = 0;
        this->cap  = initCapacity;
    }

    // -------------------------------------------------------------------------
    // 增 / Add
    // -------------------------------------------------------------------------

    /**
     * 在数组末尾追加元素（均摊 O(1)）
     * Append an element at the end of the array (amortized O(1))
     *
     * @param e 要追加的元素 / element to append
     */
    void addLast(E e) {
        // 容量已满时扩容为原来的 2 倍
        // Double capacity when the array is full
        if (size == cap) {
            resize(2 * cap);
        }
        // 在末尾插入新元素 / Insert the new element at the tail
        data[size] = e;
        size++;
    }

    /**
     * 在指定索引处插入元素（O(n)，需要移动后续元素）
     * Insert an element at the specified index (O(n), shifts subsequent elements)
     *
     * 合法插入位置：[0, size]（size 等价于 addLast）
     * Valid insert positions: [0, size] (size is equivalent to addLast)
     *
     * @param index 插入位置 / insert position
     * @param e     要插入的元素 / element to insert
     */
    void add(int index, E e) {
        // 检查 index 是否在合法的"插入位置"范围内 [0, size]
        // Check that index is a valid "position index" in [0, size]
        checkPositionIndex(index);

        // 容量已满时扩容为原来的 2 倍
        // Double capacity when the array is full
        if (size == cap) {
            resize(2 * cap);
        }

        // 将 data[index..size-1] 整体后移一格，为新元素腾出位置
        // Shift data[index..size-1] one slot to the right to make room
        for (int i = size - 1; i >= index; i--) {
            data[i + 1] = data[i];
        }

        // 将新元素写入 index 位置 / Write the new element at index
        data[index] = e;

        size++;
    }

    /**
     * 在数组头部插入元素（O(n)）
     * Prepend an element at the front of the array (O(n))
     *
     * @param e 要插入的元素 / element to insert
     */
    void addFirst(E e) {
        add(0, e);
    }

    // -------------------------------------------------------------------------
    // 删 / Remove
    // -------------------------------------------------------------------------

    /**
     * 删除并返回末尾元素（均摊 O(1)）
     * Remove and return the last element (amortized O(1))
     *
     * @return 被删除的元素 / the removed element
     * @throws out_of_range 数组为空时 / when the array is empty
     */
    E removeLast() {
        if (size == 0) {
            throw out_of_range("NoSuchElementException");
        }

        // 当元素数量降至容量的 1/4 时，缩容为原来的 1/2，节约内存
        // When size drops to 1/4 of capacity, shrink to half to save memory
        if (size == cap / 4) {
            resize(cap / 2);
        }

        E deletedVal = data[size - 1];

        // 将末尾元素重置为默认值，防止悬挂引用（对基本类型此步可选，但对对象类型有意义）
        // Reset the last slot to default value to avoid dangling references
        // (optional for primitives, but meaningful for object types)
        data[size - 1] = E();
        size--;

        return deletedVal;
    }

    /**
     * 删除并返回指定索引处的元素（O(n)，需要移动后续元素）
     * Remove and return the element at the specified index (O(n), shifts elements)
     *
     * @param index 要删除的位置 / position to remove
     * @return 被删除的元素 / the removed element
     * @throws out_of_range 索引越界时 / when index is out of bounds
     */
    E remove(int index) {
        // 检查 index 是否在合法的"元素位置"范围内 [0, size-1]
        // Check that index is a valid "element index" in [0, size-1]
        checkElementIndex(index);

        // 元素数量降至容量 1/4 时缩容
        // Shrink when size drops to 1/4 of capacity
        if (size == cap / 4) {
            resize(cap / 2);
        }

        E deletedVal = data[index];

        // 将 data[index+1..size-1] 整体前移一格，覆盖被删除的位置
        // Shift data[index+1..size-1] one slot to the left to fill the gap
        for (int i = index + 1; i < size; i++) {
            data[i - 1] = data[i];
        }

        // 重置末尾多余槽位，防止悬挂引用
        // Reset the now-unused last slot to avoid dangling references
        data[size - 1] = E();
        size--;

        return deletedVal;
    }

    /**
     * 删除并返回头部元素（O(n)）
     * Remove and return the first element (O(n))
     *
     * @return 被删除的元素 / the removed element
     */
    E removeFirst() {
        return remove(0);
    }

    // -------------------------------------------------------------------------
    // 查 / Query
    // -------------------------------------------------------------------------

    /**
     * 返回指定索引处的元素（O(1)）
     * Return the element at the specified index (O(1))
     *
     * @param index 要查询的位置 / position to query
     * @return 该位置的元素 / the element at that position
     * @throws out_of_range 索引越界时 / when index is out of bounds
     */
    E get(int index) {
        // 检查索引合法性 / Validate index
        checkElementIndex(index);
        return data[index];
    }

    // -------------------------------------------------------------------------
    // 改 / Update
    // -------------------------------------------------------------------------

    /**
     * 修改指定索引处的元素，返回旧值（O(1)）
     * Update the element at the specified index and return the old value (O(1))
     *
     * @param index   要修改的位置 / position to update
     * @param element 新元素 / new element
     * @return 被替换的旧元素 / the old element that was replaced
     * @throws out_of_range 索引越界时 / when index is out of bounds
     */
    E set(int index, E element) {
        // 检查索引合法性 / Validate index
        checkElementIndex(index);

        E oldVal = data[index];
        data[index] = element;
        return oldVal;
    }

    // -------------------------------------------------------------------------
    // 工具方法 / Utility Methods
    // -------------------------------------------------------------------------

    /**
     * 返回当前元素个数 / Return the current number of elements
     */
    int getSize() {
        return size;
    }

    /**
     * 判断数组是否为空 / Check whether the array is empty
     */
    bool isEmpty() {
        return size == 0;
    }

    /**
     * 将底层数组容量调整为 newCap（O(n)，需复制全部元素）
     * Resize the underlying array to newCap (O(n), copies all elements)
     *
     * @param newCap 目标容量 / target capacity
     */
    void resize(int newCap) {
        // 分配新数组 / Allocate new array
        E* temp = new E[newCap];

        // 将旧数组的所有元素复制到新数组
        // Copy all elements from the old array to the new one
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }

        // 释放旧数组占用的堆内存，防止内存泄漏
        // Free the old array's heap memory to prevent memory leak
        delete[] data;

        data = temp;
        cap  = newCap;
    }

    /**
     * 判断 index 是否为合法的"元素位置"，即 [0, size-1]
     * Check whether index is a valid "element position", i.e. in [0, size-1]
     */
    bool isElementIndex(int index) {
        return index >= 0 && index < size;
    }

    /**
     * 判断 index 是否为合法的"插入位置"，即 [0, size]
     * Check whether index is a valid "position for insertion", i.e. in [0, size]
     * （size 本身合法，表示追加到末尾 / size itself is valid, meaning append）
     */
    bool isPositionIndex(int index) {
        return index >= 0 && index <= size;
    }

    /**
     * 断言 index 在元素范围内，否则抛出异常
     * Assert index is within element range, throw otherwise
     *
     * 适用于 get / set / remove 等需要访问已有元素的操作
     * Used by get / set / remove that must access an existing element
     */
    void checkElementIndex(int index) {
        if (!isElementIndex(index)) {
            throw out_of_range("Index out of bounds");
        }
    }

    /**
     * 断言 index 在插入位置范围内，否则抛出异常
     * Assert index is within position range, throw otherwise
     *
     * 适用于 add 等允许在 size 位置插入的操作
     * Used by add which allows insertion at position size
     */
    void checkPositionIndex(int index) {
        if (!isPositionIndex(index)) {
            throw out_of_range("Index out of bounds");
        }
    }

    /**
     * 打印当前 size、cap 及全部元素（调试用）
     * Print current size, cap, and all elements (for debugging)
     */
    void display() {
        cout << "size = " << size << " cap = " << cap << endl;
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    /**
     * 析构函数：释放底层数组的堆内存
     * Destructor: release the heap memory of the underlying array
     */
    ~MyArrayList() {
        delete[] data;
    }
};


int main() {
    // 初始容量设置为 3 / Create array with initial capacity 3
    MyArrayList<int> arr(3);

    // 依次追加 1~5，触发至少一次扩容
    // Append 1~5, triggering at least one resize
    for (int i = 1; i <= 5; i++) {
        arr.addLast(i);  // 数组变为 [1, 2, 3, 4, 5] / array becomes [1,2,3,4,5]
    }

    arr.remove(3);      // 删除索引 3 的元素 4 → [1, 2, 3, 5]  / remove element at index 3 (value 4)
    arr.add(1, 9);      // 在索引 1 插入 9   → [1, 9, 2, 3, 5] / insert 9 at index 1
    arr.addFirst(100);  // 头部插入 100       → [100, 1, 9, 2, 3, 5] / prepend 100
    int val = arr.removeLast(); // 删除末尾 5 → [100, 1, 9, 2, 3] / remove last (5)

    // 预期输出 / Expected output:
    // 100 1 9 2 3
    for (int i = 0; i < arr.getSize(); i++) {
        cout << arr.get(i) << endl;
    }

    return 0;
}
 