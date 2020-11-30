#  Project2 Report
**SID:** 11711201  
**Name:**  王皓天  
## Error Message Logic
when meet error cause by another error, 
my message will based on code's definition, even if the definition is illegal.
i.e. 
```c
int test(){return 1.0;}
int main(){return test();}
```
The function *test* will be push into symboltable, that there is no error reported in *main*
## Bonus Feature
- scope check
  In my implementation, i combain stack and hashtable to make each scope a independent symboltable, table will be pop when scope ends.
- Function must has return type explicity.
  ```
  if no return, error
  if exist error return type, error
  ```

## Special cases
### Different from given test
test/test_2_r12.spl
```c
int test_2_r12()
{
  float arr1[10];
  float arr2[10];
  float a = 1.1;
  int i = 0;
  while (i < 10)
  {
    arr1[i] = a;
    a = a * a;
  }
  i = 0;
  while (i < 10)
  {
    arr2[arr1[i]] = a;
  }
  return 0;
}
```

```
test2/test_2_r14.spl
1,4c1,2
< Error type 14 at Line 10: no such member: age
< Error type 5 at Line 10: unmatching type on both sides of assignment
< Error type 14 at Line 12: no such member: weight
< Error type 5 at Line 12: unmatching type on both sides of assignment
---
> Error type 14 at Line 10: accessing an undefined structure member
> Error type 14 at Line 12: accessing an undefined structure member
``
when the type to be check is undefined, there won't be a assign error.