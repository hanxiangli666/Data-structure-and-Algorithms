import java.util.Arrays;
// LinkedList 是 Java 标准库中的双向链表实现。相比于 ArrayList，LinkedList 在头部和尾部插入和删除元素时性能更好，但在通过索引访问元素时性能较差。
public class LinkedList {
    public static void main(String[] args) {
        // 初始化链表
        java.util.LinkedList<Integer> lst = new java.util.LinkedList<>(Arrays.asList(1, 2, 3, 4, 5));

        // 检查链表是否为空，输出：false
        System.out.println(lst.isEmpty());

        // 获取链表的大小，输出：5
        System.out.println(lst.size());

        // 在链表头部插入元素 0
        lst.addFirst(0);
        // 在链表尾部插入元素 6
        lst.addLast(6);

        // 获取链表头部和尾部元素，输出：0 6
        System.out.println(lst.getFirst() + " " + lst.getLast());

        // 删除链表头部元素
        lst.removeFirst();
        // 删除链表尾部元素
        lst.removeLast();

        // 在链表中插入元素
        // 移动到第三个位置
        lst.add(2, 99);

        // 删除链表中某个元素
        lst.remove(1);

        // 遍历链表
        // 输出：1 99 3 4 5
        for(int val : lst) {
            System.out.print(val + " ");
        }
        System.out.println();
    }
}