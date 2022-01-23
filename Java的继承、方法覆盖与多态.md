# Java的继承、方法覆盖与多态

上回书说到，Java与C++的区别与转变。这次，继续。

## 继承

### 概念

继承是面向对象三大特征之一，封装居首位，封装之后形成了独立体，独立体 A和独立体B 之间可能存在继承关系。其实程序中的继承灵感来自于现实生活，在现实生活中继承处处可见。

继承时子类继承父类的特征和行为，使得子类对象（实例）具有父类的属性，或子类从父类继承方法，使得子类具有与父类相同的行为。

比如：兔子和羊属于食草动物类，狮子和豹属于食肉动物类。食草动物和食肉动物又是属于动物类。所以继承需要符合的关系是：is-a（Bird is-a Animal），父类更通用，子类更具体。虽然食草动物和食肉动物都是属于动物，但是两者的属性和行为上有差别，所以子类会具有父类的一般特性也会具有自身的特性。

**为什么要使用继承呢？**

在不同的类中也可能会有共同的特征和动作，可以把这些共同的特征和动作放在一个类中，让其它类共享。因此可以定义一个通用类，然后将其扩展为其它多个特定类，这些特定类继承通用类中的特征和动作。继承是 Java 中实现软件重用的重要手段，避免重复，易于维护。当然，C++里面也是如此。

### 语法

与C++的继承句法略有不同，Java采用了通俗易懂的`extends`，来作为继承的关键词，相比于`::`就好理解多了。不过有一说一，`::`在C++里面用处真多，作用域、继承……

```java
class 类名 extends 父类名{
    类体;
}
```

现在，定义一个银行账户类

```java
public class Account {  //银行账户
    private String actno;   //账号
    private double balance;    //余额

    public String getActno() {
        return actno;
    }

    public void setActno(String actno) {
        this.actno = actno;
    }
    
    public double getBalance(){
        return balance;
    }
    
    public void setBalance(double balance){
        this.balance = balance;
    }
}
```

但是，实际上，银行账户有许多的类型，但都具有上面共同的属性，因此，上面的类便作为父类（基础），其他的账户类型就只需要继承之后设定特有属性就行了。

比如，信用账户：

```java
public class CreditAccount extends Account{
    private double credit;

    public double getCredit(){
        return credit;
    }

    public void setCredit(){
        this.credit = credit;
    }
}
```

那么下面便是测试，证明信用账户是可以创建父类里面的属性的。

```java
public class TestAccount {
    public static void main(String[] args) {
        CreditAccount act = new CreditAccount();
        act.setActno("1919810");
        act.setBalance(100299.00);
        act.setCredit(100.0);
        System.out.println(act.getActno()+"信用账户,余额"+act.getBalance());
    }
}
```

输出结果：

```
1919810信用账户,余额100299.0

```

### 特性

1. B类继承 A类，则称 A类为超类(superclass)、父类、基类，B类则称为子类(subclass)、派生类、扩展类。

2. java 中的继承只支持单继承，不支持多继承，C++中支持多继承，这也是 java 体现简单性的一点，换句话说，java 中不允许这样写代码：class B extends A,C{ }。 

3. 虽然 java 中不支持多继承，但有的时候会产生间接继承的效果，例如：class C extends B，class B extends A，也就是说，C 直接继承 B，其实 C 还间接继承 A。 

4. java 中规定，子类继承父类，除构造方法和被 private 修饰的数据不能继承外，剩下都可以继承。

5. java 中的类没有显示的继承任何类，则默认继承 Object 类，Object 类是 java 语言提供的根类~~（祖宗类）~~，也就是说，一个对象与生俱来就有 Object 类型中所有的特征。

6. 继承也存在一些缺点，例如：CreditAccount 类继承 Account 类会导致它们之间的耦合度非常高，Account 类发生改变之后会马上影响到 CreditAccount 类。

总的来说，是C++的简化。

## 方法覆盖

当从父类继承过来的方法无法满足子类的需求时，就要开始考虑方法的覆盖了。

有点类似于重载。语法十分简单，就是在子类里面重写一下然后把方法体给改了就可以了，这就不需要太多展示了。

不过，需要注意：

1. 方法覆盖发生在具有继承关系的父子类之间，这是首要条件；
2. 覆盖之后的方法与原方法具有相同的返回值类型、相同的方法名、相同的形式参数列表；

另外，在使用方法覆盖的时候：

1. 由于覆盖之后的方法与原方法一模一样，建议在开发的时候采用复制粘贴的方式，不建议手写，因为手写的时候非常容易出错，比如在 Object 类当中有 toString()方法，该方法中的 S 是大写的，在手写的时候很容易写成小写 tostring()，这个时候你会认为 toString()方法已经被覆盖了，但由于方法名不一致，导致最终没有覆盖，这样就尴尬了；

2. 私有的方法不能被继承，所以不能被覆盖；

3. 构造方法不能被继承，所以也不能被覆盖；

4. 覆盖之后的方法不能比原方法拥有更低的访问权限，可以更高（学习了访问控制权限修饰符之后你就明白了）；

5. 覆盖之后的方法不能比原方法抛出更多的异常，可以相同或更少（学习了异常之后就明白了）；

6. 方法覆盖只是和方法有关，和属性无关；

7. 静态方法不存在覆盖。

## 多态

**多态（Polymorphism）**属于面向对象三大特征之一，它的前提是封装形成独立体，独立体之间存在继承关系，从而产生多态机制。多态是同一个行为具有多个不同表现形式或形态的能力。

在 java 中允许这样的两种语法出现，一种是**向上转型（Upcasting）**，一种是**向下转型（Downcasting）**，向上转型是指子类型转换为父类型，又被称为自动类型转换，向下转型是指父类型转换为子类型，又被称为强制类型转换。

在 java 语言中有这样的一个规定，无论是向上转型还是向下转型，两种类型之间必须要有继承关系，没有继承关系情况下进行向上转型或向下转型的时候编译器都会报错。

创建一个动物类：

```java
public class Animal {
	public void move(){
		System.out.println("Animal move!");
	} 
}
```

猫类：

```java
public class Cat extends Animal {
    //方法覆盖
    public void move() {
        System.out.println("走猫步！");
    }

    //子类特有
    public void catchMouse() {
        System.out.println("抓老鼠！");
    }
}
```

鸟类：

```java
public class Bird extends Animal {
    //方法覆盖
    public void move() {
        System.out.println("鸟儿在飞翔！");
    }

    //子类特有
    public void sing() {
        System.out.println("鸟儿在歌唱！");
    }
}
```

```java
public class TestPolly {
    public static void main(String[] args) {
		//创建 Animal 对象
        Animal a = new Animal();
        a.move();
		//创建 Cat 对象
        Cat c = new Cat();
        c.move();
		//创建鸟儿对象
        Bird b = new Bird();
        b.move();
    }
}
```

很明显，这是可以正常运行的。那么多态呢？

```java
public class TestPolly2 {
    public static void main(String[] args) {
        Animal a1 = new Cat();
        a1.move();
        Animal a2 = new Bird();
        a2.move();
    }
}
```

多态就是“同一个行为（move）”作用在“不同的对象上”会有不同的表现结果。java 中之所以有多态机制，是因为 java 允许一个父类型的引用指向一个子类型的对象。也就是说允许这种写法：Animal a2 = new Bird()，因为 Bird is a Animal 是能够说通的。其中 Animal a1 = new Cat()或者 Animal a2 = new Bird()都是父类型引用指向了子类型对象，都属于向上转型（Upcasting），或者叫做自动类型转换。

那么，向下转型呢？

```java
public class TestPolly3 {
    public static void main(String[] args) {
        Animal a = new Cat();
        a.catchMouse();
    }
}
```

这一段代码是报错的。是因为“Animal a = new Cat();”在编译的时候，编译器只知道 a 变量的数据类型是 Animal，也就是说它只会去Animal.class 字节码中查找 catchMouse()方法，结果没找到，自然“静态绑定”就失败了，编译没有通过。就像以上描述的错误信息一样：在类型为 Animal 的变量 a 中找不到方法catchMouse()。

这样的情况可以通过**强制类型转换**来解决，但是，有时候编译通过，也会在运行过程中报错，那应该怎么解决向下转型的问题呢？

###  instanceof

instanceof 运算符的运算结果是布尔类型，可能是 true，也可能是 false，假设(c instanceof Cat)结果是 true 则表示在运行阶段 c 引用指向的对象是 Cat 类型，如果结果是 false 则表示在运行阶段 c 引用指向的对象不是 Cat 类型。有了 instanceof 运算符，向下转型就可以这样写：

```java
public class TestPolly4 {
    public static void main(String[] args) {
        Animal a = new Bird();
        if (a instanceof Cat) {
            Cat c = (Cat) a;
            c.catchMouse();
        }
    }
}
```

### 多态的必要条件

1. 继承
2. 方法覆盖
3. 父类引用指向子类

----

