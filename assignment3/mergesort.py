import threading

def merge_sort(nums, head, tail):
    if head+1 == tail or head == tail or head > tail:
        return
    mid = int((head+tail)/2)

    thread1 = threading.Thread(target=merge_sort, args=(nums, head, mid))
    thread1.start()
    thread2 = threading.Thread(target=merge_sort, args=(nums, mid, tail))
    thread2.start()
    thread1.join()
    thread2.join()

    tmp, i, j = list(), head, mid
    while i < mid and j < tail:
        if nums[i] < nums[j]:
            tmp.append(nums[i])
            i += 1
        else:
            tmp.append(nums[j])
            j += 1
    while i < mid:
        tmp.append(nums[i])
        i += 1
    while j < tail:
        tmp.append(nums[j])
        j += 1
    for k in range(tail - head):
        nums[head+k] = tmp[k]
    return



def main():
    nums = [6, 8, 3, 98, 90, 6, 43, 92817, 6522, 54, 92]
    merge_sort(nums, 0, len(nums))
    for num in nums:
        print(num, end=" ")


if __name__ == "__main__":
    main()
