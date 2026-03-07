import java.util.Collections;
// 以上就是 Java ArrayList 的常用方法，主要包括通过索引访问元素以及添加、删除元素的方法 等。ArrayList 是一个动态数组实现，可以根据需要自动调整大小，适合频繁访问元素的场景。
public class ArrayList {
    public static void main(String[] args) {
        int n = 10;
        // 初始化 ArrayList，大小为 10，元素值都为 0
        java.util.ArrayList<Integer> nums = new java.util.ArrayList<>(Collections.nCopies(n, 0));
        // 输出：false
        System.out.println(nums.isEmpty());
        // 输出：10
        System.out.println(nums.size());

        // 在数组尾部插入一个元素 20
        nums.add(20);
        // 输出：11
        System.out.println(nums.size());

        // 得到数组最后一个元素
        // 输出：20
        System.out.println(nums.get(nums.size() - 1));

        // 删除数组的最后一个元素
        nums.remove(nums.size() - 1);
        // 输出：10
        System.out.println(nums.size());

        // 可以通过索引直接取值或修改
        nums.set(0, 11);
        // 输出：11
        System.out.println(nums.get(0));

        // 在索引 3 处插入一个元素 99
        nums.add(3, 99);

        // 删除索引 2 处的元素
        nums.remove(2);

        // 交换 nums[0] 和 nums[1]
        Collections.swap(nums, 0, 1);

        // 遍历数组
        // 输出：0 11 99 0 0 0 0 0 0 0
        for(int num : nums) {
            System.out.print(num + " ");
        }
        System.out.println();
    }
}