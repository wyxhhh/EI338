import threading

def quick_sort(nums, head, tail):
    if head+1 == tail or head == tail or head > tail:
        return
    i, j = head, tail-1
    while (i < j):
        while (i < j and nums[j] >= nums[i]):
            j -= 1
        nums[i], nums[j] = nums[j], nums[i]
        while (i < j and nums[i] <= nums[j]):
            i += 1
        nums[i], nums[j] = nums[j], nums[i]

    thread1 = threading.Thread(target=quick_sort, args=(nums, head, j))
    thread1.start()
    thread2 = threading.Thread(target=quick_sort, args=(nums, j+1, tail))
    thread2.start()
    thread1.join()
    thread2.join()


def main():
    nums = [6, 8, 3, 98, 90, 6, 43, 92817, 6522, 54, 92]
    quick_sort(nums, 0, len(nums))
    for num in nums:
        print(num, end=" ")


if __name__ == "__main__":
    main()
