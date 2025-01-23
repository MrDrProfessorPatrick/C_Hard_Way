/**
 * Note: The returned array must be malloced, assume caller calls free().
 
 	Input: nums = [2,7,11,15], target = 9
	Output: [0,1]
	Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
 */
 
int main() 
{
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int res[];
    
    for(i = 0; i < numsSize; i++) {
    	for(inner = i+1; inner < numsSize; inner++) {
    		if(inner > nums) break;
    		if(nums[i] + nums[inner] == target) {
    			res[0] = nums[i];
    			res[1] = nums[inner];
    		}
    	}
    }
    
    return res;
}

twoSum([2,7,11,15], 4, 9, 2)

} 

