import java.util.Map;
// HashMap 是 Java 标准库中的哈希表实现，提供了基于键值对（key-value）的存储，可以高效的完成键值对的增删查改操作 。HashMap 允许使用 null 作为键或值，但不保证元素的顺序。
public class HashMap {
    public static void main(String[] args) {
        // 初始化哈希表
        java.util.HashMap<Integer, String> hashmap = new java.util.HashMap<>();
        hashmap.put(1, "one");
        hashmap.put(2, "two");
        hashmap.put(3, "three");

        // 检查哈希表是否为空，输出：false
        System.out.println(hashmap.isEmpty());

        // 获取哈希表的大小，输出：3
        System.out.println(hashmap.size());

        // 查找指定键值是否存在
        // 输出：Key 2 -> two
        if(hashmap.containsKey(2)) {
            System.out.println("Key 2 -> " + hashmap.get(2));
        } else {
            System.out.println("Key 2 not found.");
        }

        // 获取指定键对应的值，若不存在会返回 null
        // 输出：null
        System.out.println(hashmap.get(4));

        // 获取指定键对应的值，若不存在则返回默认值
        // 输出：defaultVal
        System.out.println(hashmap.getOrDefault(4, "defaultVal"));

        // 插入一个新的键值对
        hashmap.put(4, "four");

        // 获取新插入的值，输出：four
        System.out.println(hashmap.get(4));

        // 删除键值对
        hashmap.remove(3);

        // 检查删除后键 3 是否存在
        // 输出：Key 3 not found.
        if(hashmap.containsKey(3)) {
            System.out.println("Key 3 -> " + hashmap.get(3));
        } else {
            System.out.println("Key 3 not found.");
        }

        // 遍历哈希表
        // 输出（顺序可能不同）：
        // 1 -> one
        // 2 -> two
        // 4 -> four
        for(Map.Entry<Integer, String> pair : hashmap.entrySet()) {
            System.out.println(pair.getKey() + " -> " + pair.getValue());
        }
    }
}