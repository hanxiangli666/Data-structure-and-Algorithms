public class Queue {
    // Queue 是 Java 标准库中的队列接口，常用的实现类有 LinkedList 和 PriorityQueue。队列遵循先进先出（FIFO）的原则，适合需要按顺序处理元素的场景。
    public static void main(String[] args) {
        // 初始化一个空的整型队列 q
        java.util.Queue<Integer> q = new java.util.LinkedList<>();

        // 在队尾添加元素
        q.offer(10);
        q.offer(20);
        q.offer(30);

        // 检查队列是否为空，输出：false
        System.out.println(q.isEmpty());

        // 获取队列的大小，输出：3
        System.out.println(q.size());

        // 获取队列的队头元素
        // 输出：10
        System.out.println(q.peek());

        // 删除队头元素
        q.poll();

        // 输出新的队头元素：20
        System.out.println(q.peek());
    }
}