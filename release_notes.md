# 更新说明

## V0.0.0

> 初步制定了UI界面，后续将会按照这个界面开发


![image-20200814175426951](https://i.loli.net/2020/08/14/Nelwyr2OU5j89ic.png)

![image-20200814175508818](https://i.loli.net/2020/08/14/Pn4OwRFfBsd7i2K.png)

![image-20200814175542163](https://i.loli.net/2020/08/14/makcdlJCF6Ue5go.png)

![image-20200814175602421](https://i.loli.net/2020/08/14/rId8l6Xm7agOxfo.png)

![image-20200814175638388](https://i.loli.net/2020/08/14/6pRk4zqfuNMIAOx.png)

![image-20200814175704610](https://i.loli.net/2020/08/14/D4lY3hMyOU7Evgq.png)

## V0.0.1

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

## V0.0.2

> 新加了串口解析。实现了线程，设备，定时器的功能，修复了性能，同步，通信的按钮冲突

![image.png](https://i.loli.net/2020/08/16/8bgCSXMJys15rei.png)

![image-20200816173448261](https://i.loli.net/2020/08/16/DKu92eSN16fBtJO.png)

![image.png](https://i.loli.net/2020/08/16/WIGVerb3h4oHN8F.png)

## V0.0.3

> 将性能，同步，通信也全部配置完毕，新增首选项设置

![image-20200817202317928](https://i.loli.net/2020/08/17/IFv6xsnbOkcS2T5.png)

![image-20200817202353876](https://i.loli.net/2020/08/17/EhjwumxXvblePOF.png)

![image-20200817202408920](https://i.loli.net/2020/08/17/dGTXwDgU38fYIRz.png)

![image-20200817202444443](https://i.loli.net/2020/08/17/dGTXwDgU38fYIRz.png)

![image-20200817202512188](https://i.loli.net/2020/08/17/vT2U3BhudOWP6Vs.png)

![image-20200817202521362](https://i.loli.net/2020/08/17/uVYFPTlHKU2Aczt.png)

> 新增串口数据保存，且可选择是否配置

![image-20200817202814111](https://i.loli.net/2020/08/17/LGTN4suSFoYUvIB.png)

![image-20200817202834019](https://i.loli.net/2020/08/17/7K2f6PRE1UoA3zB.png)

## V0.0.4

> 已实现其基本功能

![xxx.gif](https://i.loli.net/2020/08/21/ibwkVJ4ymEXZ37j.gif)

![xxx.gif](https://i.loli.net/2020/08/22/LtBSYHAi3wbo24z.gif)

## V0.0.5

> 修复部分bug

## V0.0.7

> 修复部分bug(msh)

## V0.0.8

> 修复部分BUG，加入文件管理器，加入插件功能

![image.png](https://i.loli.net/2020/08/25/5vAQIOM6KD37Bel.png)

![image.png](https://i.loli.net/2020/08/25/S12DnzRPGtdNYwV.png)

![image.png](https://i.loli.net/2020/08/25/soE8ARFl91ehubc.png)