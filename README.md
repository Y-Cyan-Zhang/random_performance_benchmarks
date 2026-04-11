# Random Performance Tests That I'm Doing

The purposes of these files is to mess around with random C++ performance tests, not for any pratical reasons, but for solely pedagogical purposes.

#### vector_vs_deque.cpp
```
Testing vector vs deque for 10 million elements
Push Back: vector=37.8756ms, deque=31.7829ms
Random Access: vector=12.3571ms, deque=4.73229ms
```

#### vec_reserve.cpp
 ```
Not reserved vector, 10 million push_backs: 149.884ms
Reserved vector, 10 million push_backs: 89.2705ms
```
