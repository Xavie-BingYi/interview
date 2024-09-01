# 要怎麼更新程式碼到 git 上面

git 檔案分成兩種：有追蹤、沒有追蹤的
- 有追蹤的檔案：可以看到哪些行被你刪掉或增加
- 沒有追蹤的檔案：直接把檔案加到 git 的伺服器裡面

```commandline=
git add <你的檔案名稱>
```

等你把你要修改的檔案加完之後
你就可以把這次的修改寫一個筆記，commit (提交)
```commandline=
git commit
```

等你的筆記做完想要把他上傳到伺服器 (網站版的 github) 的時候
```commandline=
git push
```

# C語言編譯流程
- https://chivincent.gitbooks.io/c-tutorial/content/chapter2/compile.html
- https://medium.com/@alastor0325/https-medium-com-alastor0325-compilation-to-linking-c07121e2803

# Enable GPIO
## init
1. 確認原理圖中欲點亮之GPIO之腳位的Port & Pin
2. 確認方塊圖中的模組在哪個Bus上(AHB1,...) _DATASHEET2 Description
3. 確認每個模組映射到的位址(RCC,Port,...) _SPEC2.3 Memory map
4. 確認需enable哪一個bit _SPEC6.3 RCC regist map
## GPIO regist
確認對應GPIO port的位址 _SPEC2.3 Memory map
再開啟對應模式的register _8.4
- set mode : 設定input或output或alternate function
- set/get bit : 
	- output : (0x14) or bit 0 ~ 15為set bit 16 ~ 31為reset bit(0x18)
	- intput : (0x10)
- set pull-up/-down
- set speed 

# UART
## 協議大全 : https://wiki.csie.ncku.edu.tw/embedded/USART?revision=0ef36332e497437cb7b1fdabc6f0a33202ab2159
## 協議簡介 : https://blog.csdn.net/XiaoXiaoPengBo/article/details/124043034
## 工具 : https://tera-term.en.softonic.com/?ex=RAMP-2081.4

- 可程式化的資料長度 (8 or 8+1 bits)
- 可程式化的停止位元 (1 or 2 bits)
- 任何USART雙向通信至少需要兩個腳位：接收資料輸入(RX)和發送資料輸出(TX)
	- RX: 接收資料輸入，並藉由採樣的技術判斷資料及噪音
	- TX: 發送資料，當發送器被啟動時，如果沒有傳送數據，則TX保持高電位。在Single-wire half-duplex或Smartcard mode時，此I/O同時被用於資料的傳送和接收
- 根據USART_CR1暫存器中的M位選擇8或9位元決定資料長度
- 使用fractional baud rate generator —— 12位整數和4位小數的表示方法，放在baud rate暫存器(USART_BRR)中
- 一個狀態暫存器(USART_SR)
- 資料暫存器(USART_DR)

## Fractional baud rate generation的設定
接收器和傳送器的Baud rate分別由USART_BRR設置USARTDIV的整數部分(Mantissa)及小數部分(Fraction)，計算方式如下所示:
 ![alt text](./note%20image/波特率公式.png)

- 範例 : https://blog.csdn.net/m0_50728139/article/details/113747627
 ![alt text](./note%20image/波特率計算範例.png)

其中USARTDIV為一個無號的定點數(unsigned fixed point number)，fCK為給周邊設備的時鐘。

- 當OVER8 = 0 時，小數部分佔USART_BRR的DIV_Fraction[3:0]，共 4 bits
- 當OVER8 = 1 時，小數部分佔USART_BRR的DIV_Fraction[2:0]，共 3 bits，其中DIV_Fraction[3]應該保持’0’

USART_BRR被更新後，baud rate的計數器中的值也會同時被更新，因此在傳輸途中不應該更新USART_BRR中的值。 另外，如果TE或RE被分別禁止，則baud rate的計數器也會停止計數。

使用stm32f407vgt6官方lib時，會透過檔案中設定的時脈和baud rate去換算出USART_BRR的值，包含整數與小數部分。.
- BRR(USARTDIV) 的值 Mantissa = 0x088B ; Fraction = 0x08 =>計算方式 0x88B->0d2187 + 8/16 = 2187.5

if over8=0 計算baud rate的方式: baud rate = usart時脈/(8(2-over8)DIV).
- usart時脈42Mhz, baud rate = 42000000/(822187.5) = 1200

usart是接在APB BUS上方，stm32f407vgt6有兩組APB各對應不同usart。usart時脈要看APB供應的時脈， APB時脈要透過RCC和PLL設定去看clock tree。.
- 預設stm32f4-discovery這塊板子外部震盪器(HSE_VALUE)是8Mhz(官方lib好像設定成25Mhz)。.
- 8Mhz透過pll_M(8)除頻輸入PLL =>8Mhz/8=1Mhz.
- 1Mhz輸入PLL,透過pll_N(0x5400)倍頻再透過pll_P(2)除頻，作為sysclk => (1Mhz*0x5400>>6)/2 = 168Mhz.
- sysclk轉接HCLK都是168Mhz.
- HCLK>>2轉給PCLK1 => 168Mhz>>2 = 42Mhz.

## 傳送器
傳送器依據USART_CR1的M位狀態來決定發送8或9位元的資料。 當transmit enable bit(TE)被設定時，資料放入transmit shift register後，經由TX腳位送出， 同時，相對應的時鐘脈衝會由SCLK腳位輸出。
在USART發送期間，TX首先傳送資料的最低有效位元(least significant bit)，因此在此模式中，USART_DR和transmit shift register之間包含一個緩衝器(TDR)。
## 接收器
接收器依據USART_CR1 M位的狀態來決定接收8或9位元的資料。
在USART接收期間，RX從資料最低有效位元(least significant bit)開始接收，因此在此模式中，USART_DR和received shift register之間包含一個緩衝器(RDR)。

