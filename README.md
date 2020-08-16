# seimey
一款串口终端，基于RT-Thread的FinSH控制台

# V0.0.0

> 初步制定了UI界面，后续将会按照这个界面开发


![image-20200814175426951](https://i.loli.net/2020/08/14/Nelwyr2OU5j89ic.png)

![image-20200814175508818](https://i.loli.net/2020/08/14/Pn4OwRFfBsd7i2K.png)

![image-20200814175542163](https://i.loli.net/2020/08/14/makcdlJCF6Ue5go.png)

![image-20200814175602421](https://i.loli.net/2020/08/14/rId8l6Xm7agOxfo.png)

![image-20200814175638388](https://i.loli.net/2020/08/14/6pRk4zqfuNMIAOx.png)

![image-20200814175704610](https://i.loli.net/2020/08/14/D4lY3hMyOU7Evgq.png)

# V0.0.1

> 增加了JSON的设置, 完善了UI界面，开始写串口部分代码（已完成串口异常，串口列表，串口开关）

![image-20200815190446929](https://i.loli.net/2020/08/15/KYma36pkn5gLJo8.png)

![image-20200815190511665](https://i.loli.net/2020/08/15/8MYHnNyTmQKiRDB.png)

![image-20200815190543571](https://i.loli.net/2020/08/15/3I4XOtaTgU7JpBd.png)

以下是串口的设置`JSON`文件

```json
{
    "BaudRate": "115200",
    "DataBits": "5",
    "FlowControl": "硬流控",
    "Parity": "偶校验",
    "SerialPort": "COM5",
    "StopBits": "2.0"
}
```

# V0.0.2

> 新加了串口解析。实现了线程，设备，定时器的功能，修复了性能，同步，通信的按钮冲突

![image.png](https://i.loli.net/2020/08/16/8bgCSXMJys15rei.png)

![image-20200816173448261](https://i.loli.net/2020/08/16/DKu92eSN16fBtJO.png)

![image.png](https://i.loli.net/2020/08/16/WIGVerb3h4oHN8F.png)