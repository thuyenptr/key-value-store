# Key Value Store

## Môi trường

```
protobuf version: 3.6.1
g++ version: 7.3.0
cmake version:  3.13.0-rc1

```

## Chạy chương trình

### Server:
- Run command sau:
	- ./render-proto-file.sh
	- cmake .
	- make clean
	- make
	- ./Server

### Client:
- Run command sau:
	- ./render-proto-file.sh
	- cmake .
	- make clean
	- make
	- ./Client -<mode> -<add>
- Nếu mode là -s => chạy single client
- Nếu mode là -m => chạy multiple client => thêm argument sau:
	- s : set command
	- g : get command
	- r : remove command
	- e : exist command


## Các kiến thức cơ bản về hệ điều hành

### Memory Allocation
- Định nghĩa đơn giản
    - Dành bộ nhớ để làm một mục đích cụ thể.
    - Các chương trình và services đã được dành một bộ nhớ cụ thể theo yêu cầu của chúng khi thực thi chương trình.
    - Một khi chương trình kết thúc, bộ nhớ được giải phóng và được cấp phát cho chương trình khác hoặc gộp vào bộ nhớ chính.

- Một số loại Memory Allocation:
    - Static Memory Allocation: Chương trình được cấp phát bộ nhớ tại thời điểm biên dịch.
    - Dynamic Memory Allocation: Chương trình được cấp phát bộ nhớ tại thời điểm thực thi.

- Local Memory Allocation on the Stack

```C
int a = 10;
```
- Ví dụ trên khai báo một biến kiểu nguyên, cấp phát môt vùng nhớ để lưu trữ số nguyên 4 bytes. Ta tham chiếu đến data trong bộ nhớ thông qua biến a.

- ```Local Memory Allocation``` đề cập đến quá trình mà chương trình tạo không gian lưu trữ của dữ liệu. Quá trình cấp phát này là cấp phát cục bộ, xảy ra trong phạm vi scope và khi hàm trả về thì bộ nhớ được thu hồi.

```C
int * plus(int a, int b) {
    int c = a + b;
    return &c;
}

// Khi return vùng nhớ local của c bị deallocated
// Do do con trỏ ptr có thể hoạt động không đúng, khi trỏ
// vào vùng nhớ không hợp lệ.

int main(int argc, char * argv[]) {
    int *ptr= plus(1, 2);
    printf("%d\n", * p);
}
```

- ```Stack Allocation``` Mỗi function đều chứa trên stack một cấu trúc gọi là stack frame. Stack frame chứa tất cả bộ nhớ được phân bổ cũng như điểm thực thi khi gọi hàm, ```return pointer```.

```C
int gettwo(){
   return 2;
} 

int getone(){
  return 1;
}

int addonetwo(){
  int one = getone();
  int two = gettwo();
  return one+two;
}

int main(){
  int a = addonetwo();

}
```

- Global Memoy Allocation on the Heap
- Như ví dụ về hàm ```plus``` phía trên, ta không thể lấy con trỏ trỏ tới vùng nhớ biến c vì sau khi return memory bị deallocated. Do đó ta cần một phương thức cho global memory để ngăn tình trạng bị deallocated khi return function.

```C
Node* node = new Node();
```
- Một biến cục bộ node trên stack có đủ bộ nhớ để chứa một địa chỉ. Giá trị trong ô nhớ tại địa chỉ được trả về bởi ```new Node()```. 
- Global memory cho một chương trình gọi là heap. Là một cấu trúc dữ liệu phân mảnh.

- Memory Leaks

- Program Layout: Stack vs. Heap

```
     2^64-1--->  .----------------------.
High Addresses   |      Enviroment      |
                 |----------------------|
                 |                      |   Functions and variable are declared
                 |         STACK        |   on the stack.
base pointer ->  | - - - - - - - - - - -|
                 |           |          |
                 |           v          |
                 :                      :
                 .                      .   The stack grows down into unused space
                 .         Empty        .   while the heap grows up. 
                 .                      .
                 .                      .   (other memory maps do occur here, such 
                 .                      .    as dynamic libraries, and different memory
                 :                      :    alloocat)
                 |           ^          |
                 |           |          |
 break point ->  | - - - - - - - - - - -|   Dynamic memory is declared on the heap
                 |          HEAP        |
                 |                      |
                 |----------------------|
                 |          BSS         |   The compiled binary code is down here as
                 |----------------------|   well as static and initialzed data
                 |          Data        |
                 |----------------------|
Low Addresses    |          Text        |
      0 ----->   '----------------------'  
```

- Stack ```base pointer``` trỏ đến top của stack, khi function call hay return nó sẽ làm nhiệm vụ của nó.
- ```Break point``` trỏ đến top của vùng data segment của chương trình chứa vùng heap. khi heap đầy và yêu cầu nhiều không gian nhớ hơn, beak point được set tới địa chỉ cao hơn.

### Page Table

### User and Kernel Space

### Memory Mapping

- Nằm giữa break point và base pointer là unallocated memory, hay có thể là vùng không sử dụng.
- Vùng này có thể là vùng ánh xạ bộ nhớ. Với một process load dữ liệu từ file vào memory, khi read hoặc write file.
- Một các khác để sử dụng vùng địa chỉ ở giữa đó là load dynamic shared libraries. Khi gọi một hàm printf() hay malloc(), code của function này được giữ ở shared libraries, hay standard library. Chương trình phải chạy code này, tuy nhiên OS lại không muốn load quá nhiều code vào memory. Thay vào đó nó ánh xạ mã cần thiết vào vùng nhớ ở giữa.


### Key Value Store

1. Quick overview of key-value stores
- Some method:
    - get(key): get some data previously saved under the identifier "key", or fail if no data was stored for "key"
    - set(key, value): store the "value" in memory under the identifier "key", so we can access it later referencing the same "key". If some data was already present under the "key", this data will be replaced.
    - remove(key): Delete the data that was stored under the "key"
    - exist(key): Check the "key" already stored or not.
- Underlying implementations using B-Trees. Sometimes, the data is too big to fit in memory, or the data must be persisted in case the system crashes for any reason.

2. Implement key-value stores
    - Programming language: C/C++
    - Algorithmics and data structures: BTree
    - Memory management
    - Concurrency control with multi-processors or multi-threading
    - Networking with a server/client model
    - I/O problems with disk access and use of the file system
    - Producer - Consumer problems

#### Issue implement BTree

![Alt Text](https://gitlab.zalopay.vn/thuyenpt/key-value-store/raw/master/img/381_a.gif)

[Reference](http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap19.htm)

- Structure:
    - BTree has some important fields like:
        - next_pos: mean next position in file, new node created will be here.
        - root_pos: mean that position belong to root node.
        - degree: the goal of a b-tree is to minimize the number of disk accesses. degree depends upon disk block size
    - BTreeNode:
        - child: list of BTreeNode contains child of current node
        - num: number of key store in node, number of child equal num plus one
        - child_pos: very important field, this array contains the position of child node in file
        - pos: position of current node
        - leaf: identify node was leaf or not

#### Socket communication Client to Server

[Reference](http://beej.us/guide/bgnet/html/single/bgnet.html)

- On POSIX systems, in blocking mode ```recv``` will only block until some data is present to be read.
-  Call ```recv``` in non-blocking mode will return any data that the system has in it's read buffer for that socket. But, it don't wait for the data.
- Non-blocking sockets can also be used in conjunction with the select() API. In fact, if you reach a point where you actually WANT to wait for data on a socket that was previously marked as "non-blocking", you could simulate a blocking recv() just by calling select() first, followed by recv().

#### Thread Pool

- Thread pool contain a queue, this queue contains some task, task was added by method add_task(), when server handle client request, it process request, get data from request, and type of task, then create Task object and pass it through add_task().
- Thread pool fields:
    - m_pool_size: how many thread available inside pool
    - m_task_mutex: use to lock when add some task into task queue
    - m_task_cond_var: use to notify thread in thread pool, have some task available in task queue.
    - m_threads: a vector contains m_pool_size thread, use to release thread pointer when shutdown server.
    - m_pool_state: thread pool status, use to notify any thread running that server will shutdown and don't process any task.

#### Mutex lock resource

- Using pthread_mutex
- Define class call Mutex, it contain:
    - m_lock: the mutex instance
    - is_locked: use to identify lock or not

#### Producer - Consumer problems

- Problems: In the same time system allow multi-process can read database. But when write to database only one process can write and no one can read or write in this time.
- Using a variable call rc, and init rc = 0, two Mutex: m_db_mutex, m_rc_mutex

```C
void Server::get_func(void *arg) {
    m_rc_mutex.lock(); // doc quyen truy xuat bien rc
    rc++;
    if (rc == 1) { // tien trinh doc dau tien => khong cho ghi
        m_db_mutex.lock();
    }
    m_rc_mutex.unlock();
    // read database
    VALUE result = store->get(data->key);
    // end read database

    m_rc_mutex.lock(); // doc quyen truy xuat
    rc--; // giam tien trinh doc
    if (rc == 0) { // tien trinh doc cuoi cung
        m_db_mutex.unlock();
    }
    m_rc_mutex.unlock();


}

void Server::set_func(void *arg) {
    m_db_mutex.lock();

    store->set(data->key, data->value);

    m_db_mutex.unlock();
}
```

#### Work flow


![Alt Text](https://gitlab.zalopay.vn/thuyenpt/key-value-store/raw/master/img/key-value-stores.png)

* Client
    - Multiple Client open connection to connect to Server
    - After connect successfuly, client enter command like: get(key), set(key,value), remove(key), exist(key)
    - Data from client contain:
        - key, value
        - type: get, set, remove or exist
        - sign: to identify valid package send to server
    - Client serialize structured data with protobuf method. After that socket client will send this package to Server.

* Server
    - Server create some thread running for task. To manage thread, I'm created class call ThreadPool, it keep task queue, list threads, some lock, ...
    - Server create socket call listener to listen any connection from client.
    - when Socket open to accept connection, the file description was added to fd_set.
    - Server using method call select() to detect data arrive from client
    - When data arrived, using method recv() to get data, after that protobuf will Deserialize this package and get data.
    - From data we will detect type in order to create suitable task. Task will be added to task queue. Any free thread in thread pool will access to task queue and do task.

