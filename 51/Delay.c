//延时方法1
void Delay(unsigned int xms)//12Mhz
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}
//延时方法2
/单片机C语言延时程序用C语言写出来程序非常的简练，它是一种模块化的语言，一种比汇编更高级的语言，但是就是这样一种语言也还是有它不足之处：它的延时很不好控制，我们常常很难知道一段延时程序它的精确延时到底是多少，这和汇编延时程序没法比。但有时后写程序又不得不要用到比较精确的延时，虽然说可以用混合编程的方式解决，但这种方式不是每个人都能掌握，且写起来也麻烦。所以，通过测试我给大家提供一个延时子程序模块，并以此给一个出我们经常用到的延时的数据表格。（注意：表格中的数据只适合我的延时模块，对其他的延时程序不适用，切忌！！！！！！！！别到时候延时不对来骂我）

延时模块：其中问号代表要填的数，要延时多少，到表格中去找数据，然后填上就OK！切忌3条FOR语句不能颠倒顺序

void Delay()
{
	unsigned char a,b,c;
	for(a=0;a<?;a++)
	for(b=0;b<?;b++)
	for(c=0;c<?;c++);
} 

1
2
3
4
5
6
7
8
                                                       数据表如下
/**********************************************************************************************************************/
延时时间                    a的值                 b的值                      c的值             延时误差（us）
10us                       1                       1                       1                         -0.5      
20us                       1                       1                       8                         0 
30us                       1                       1                       15                        +0.5
40us                       2                       1                       9                         0 
50us                       1                       1                       28                        0 
60us                       1                       1                       35                        +0.5
70us                       1                       1                       42                        +1
80us                       1                       1                       48                        0
90us                       1                       1                       55                        +0.5 
100us                      1                       1                       61                        -0.5 
200us                      1                       1                       128                       0 
300us                      3                       1                       63                        +1.5 
400us                      2                       1                       129                       0 
500us                      5                       1                       63                        +0.5 
600us                      6                       1                       63                         0 
700us                      7                       1                       63                        -0.5 
800us                      1                       3                       175                       +0.5 
900us                      9                       1                       63                        -1.5 
1ms                        1                       3                       219                       -1.5
2ms                        2                       3                       220                       +3
3ms                        3                       3                       220                       +3
Xms                        X                       3                       220                       +3   
（X的范围为2到255)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
基本上我们平时用到的延时都在这里了，每种延时的误差都很小，最大也不过3us，有的甚至没有误差，已经很精确了，如果想延时1秒钟，你可以连续调用延时250ms的程序4次，总共延时误差12us，这样的误差已经不算误差了，用汇编语言编写还达不到这个程度。

现在你不再为延时不精确担忧了，参照表中的数据写延时，想延时多少就能延时多少。

再次重申：此表格是根据我的延时模块编写，这也是大多数朋友都习惯用的一个模块，如果你用其他模块或是改动了我的模块，延时都不准的，不信大家可以试试！
————————————————
版权声明：本文为CSDN博主「Mr.Lan1973」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_42166190/article/details/112645704
//延时方法3