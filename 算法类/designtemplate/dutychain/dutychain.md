## 责任链模式

责任链模式就是一个请求到来的时候，经过一个流程处理，每一个节点都会判断是否能够处理如果可以处理就进行处理，如果处理不了就交给下一个责任点处理，这就是责任链模式。



**C++中初始化列表：**

初始化列表是这样的，它是构造函数执行的第一步也就是初始化阶段，第二步是计算阶段，即使没有初始化列表构造函数也会先对所有的成员执行初始化操作，也就是调用成员的默认构造函数，如果是类的话要是没有默认构造函数就会报错，那怎么办？很简单，就用初始化列表去调用类的其他构造函数即可。

必须使用初始化列表的情况：

1. 常量成员，因为常量只能初始化不能赋值，所以必须放在初始化列表里面
2. 引用类型，引用必须在定义的时候初始化，并且不能重新赋值，所以也要写在初始化列表里面
3. 没有默认构造函数的类类型，因为使用初始化列表可以不必调用默认构造函数来初始化，而是直接调用拷贝构造函数初始化

**总之能用初始化列表就用初始化列表，毕竟性能比较高！**

### 总结：

这里我只用了两个简单的责任点，然后顶层的上级就没有了，也可以再继续加长链结构，也可以加一下复杂点的东西，我写的比较简单，主要是为了理解责任链这种设计模式。



​	
