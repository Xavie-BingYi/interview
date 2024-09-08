# MarkDown語法
- <https://hackmd.io/@eMP9zQQ0Qt6I8Uqp2Vqy6w/SyiOheL5N/%2FBVqowKshRH246Q7UDyodFA?type=book>


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
- <https://chivincent.gitbooks.io/c-tutorial/content/chapter2/compile.html>
- <https://medium.com/@alastor0325/https-medium-com-alastor0325-compilation-to-linking-c07121e2803>


# GPIO
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


# UART (適用STM32F4xx全系列)
- 協議大全 : <https://wiki.csie.ncku.edu.tw/embedded/USART?revision=0ef36332e497437cb7b1fdabc6f0a33202ab2159>
- 協議簡介 : <https://blog.csdn.net/XiaoXiaoPengBo/article/details/124043034>
- 工具 : <https://tera-term.en.softonic.com/?ex=RAMP-2081.4>

## 簡介
通用同步異步接收發送器(USART)提供靈活的全雙工數據交換方式，適用於需要行業標準NRZ異步串行數據格式的外部設備。USART通過分數波特率生成器，提供非常廣泛的波特率範圍。

他支援同步單向通訊 & 半雙工單線通訊。同時支援LIN(本地互聯網絡)，Smartcard協議和IrDA(紅外通訊技術
) SIR(串行紅外) ENDEC(串行紅外編碼器/解碼器)規範，以及數據通訊(CTS/RTS)。也允許多處理器通訊。

## 特性
- 全雙工的**非同步通訊**
- NRZ標準資料格式(Mark/Space)
	NRZ(Nonreturn to Zero):不歸零編碼
	這是一種傳送資訊的編碼方式，它以正脈波代表1，負脈波代表0，當訊號連續為'1'時，則保持正脈波，直到出現'0'為止
	它的特色是編碼解碼較為簡單，但缺乏同步傳輸的能力，且無法提供較佳的訊號校正能力。
- 分數波特率生成器系統
	通用可編程發送和接收波特率(參考Datasheets以獲取最大APB頻率下的波特率)
- 可程式化的資料長度 (8 or 8+1 bits)
- 可程式化的停止位元 (1 or 2 bits)
- 具LIN Master同步中斷發送能力 & LIN Slave中斷檢測能力
	當USART硬體配置為LIN時，支援13位元中斷生成 & 10/11位元中斷檢測
- 提供同步傳輸的CLK信號
- IrDA SIR編碼解碼器
	正常模式下支援3/16 bit duration
- Smartcard模擬能力
	- 略
- 單線半雙工通訊
- 藉由DMA的資料傳輸，每個USART都能用DMA發送和接收資料
	- 略
- 獨立的發送器和接收器的Enable Bit(TE、RE)
- 傳輸檢測標誌
	- 接收緩衝區滿(Receive buffer full, RXNE)
	- 傳送緩衝區空(Transmit buffer empty, TXE)
	- 傳輸結束(End of transmission flags, TC)
- 奇偶檢測控制
	- 發送奇偶檢測位(Transmits parity bit)
	- 對接收的資料進行檢測(Checks parity of received data byte)
- 4個錯誤檢測標誌
	- 溢出錯誤(Overrun error)
	- 雜訊檢測(Noise detection)
	- 幀差錯誤(Frame error)
	- 奇偶檢測錯誤(Parity error)
- 支援10種中斷
	- CTS改變(CTS changes, CTSIE)
	- LIN中斷檢測(LIN break detection, LBDIE)
	- 傳送緩衝區空(Transmit data register empty, TXEIE)
	- 傳送完成(Transmission complete, TCIE)
	- 接收緩衝區滿(Receive data register full, RXNEIE)
	- 空閒線路檢測(Idle line received, IDLEIE)
	- 溢出錯誤(Overrun error)
		在一般情況下，本身不產生中斷，在DMA情況下，則由EIE產生中斷，經檢驗USART_CR1的FE位可得知溢出錯誤
	- 幀差錯誤(Framing error)
		在一般情況下，本身不產生中斷，而由RXNE產生中斷，經檢驗USART_CR1的FE位可得知Frame錯誤
		在DMA情況下下，則由EIE產生中斷，經檢驗USART_CR1的FE位得知錯誤
	- 雜訊錯誤(Noise error)
		在一般情況下，本身不產生中斷，而由RXNE產生中斷，經檢驗USART_CR1的NF位得知錯誤
		在DMA情況下下，則由EIE產生中斷，經檢驗USART_CR1的NF位得知錯誤
	- 奇偶檢驗錯誤(Parity error, PEIE)
- 多處理器通訊 - 如果地址未匹配，則進入靜音模式
- 喚醒靜音模式 (通過空閒線路檢測 or 地址標記檢測)
- 2種接收器喚醒模式
	- Address bit(MSB,9^th^ bit)：MSB為'1'的資料被認為是地址，否則為一般資料。
	在這資料中，接收端會將最後4 bits與USART_CR2暫存器中的ADD位比較，若相同則清除RWU位，後面的資料將能正常接收。
	- Idle line：在接收端處於靜默(mute mode)時，可透過發送空閒符號(即所有位均為'1'的資料)，喚醒接收端。

## 功能介紹
- USART Block Diagram

  	![alt text](./note%20image/USART%20block%20diagram.png)

- 任何USART雙向通信至少需要兩個腳位：接收資料輸入(RX)和發送資料輸出(TX)
	- RX: 接收資料為串行輸入，並藉由採樣技術來判斷有效的資料及雜訊。
	- TX: 發送資料。當發送器被啟動時，如果沒有傳送數據，則TX保持高電位；被禁用時，輸出引腳則返回I/O配置。
	在Single-wire或Smartcard mode時，此I/O同時被用於資料的傳送和接收(在USART層，數據則通過SW_RX接收)。

- 通過這些引腳，在正常USART模式下，串行數據做為幀(frame)，進行傳送與接收，包含：
	- 一個idle line在傳送與接收前
	- 一個起始位(start bit)
	- 一個data word(8 or 9 bits)，從最低有效位開始(根據USART_CR1暫存器中的M位選擇8或9位元決定資料長度)
	- 0.5, 1, 1.5或2個停止bitS，表示幀的結束
	- 這個介面使用分數波特率生成器 - 12 bits的整數 & 4 bits的小數表示方法
	- 一個狀態暫存器(USART_SR)
	- 資料暫存器(USART_DR)
	- baud rate暫存器(USART_BRR) - 12 bits的整數 & 4 bits的小數
	- Smartcard模式有一個保護時間暫存器(USART_GTPR)

- 另外在同步模式中，還需要其他腳位：
	- CK(SCLK)：發送器的時鐘輸出，用於同步傳輸的時鐘，相當於SPI主模式。(略)

- 在Hardware flow control中必須包含腳位:
	- CTS(nCTS): 清除發送，若在高電位，則當目前資料傳送結束後，中斷下一次的資料傳送
	- RTS(nRTS): 發送請求，若在低電位，則表示USART已經準備好接收資料

### USART character description (data frame)
- 資料長度(word length)根據USART_CR1暫存器中的M位選擇8或9位元

	![alt text](./note%20image/word%20length%20programming.png)

- 在起始位(start bit)期間，TX處於低電位，在停止位期間，TX處於高電位

- 一個idle character被解釋全由'1'組成，並接著下一個資料的起始位

- 一個break character則全由'0'所組成，在整個break frame結束時傳送1或2個停止位('1')以確認起始位

- 傳送和接收皆由共同的波特率生成器驅動，當enable bit分別被設置時，clock將被生成

## 傳送器
傳送器依據USART_CR1的M位狀態來決定發送8或9位元的資料。
當transmit enable bit(TE)被設定時，transmit shift register的資料將由TX腳位送出，對應的時鐘脈衝會輸出至CK(SCLK)引腳。

### 資料的傳送
在USART發送期間，TX首先傳送資料的最低有效位元(least significant bit)，因此在此模式中，USART_DR包含一個緩衝區(TDR)，介於內部總線(bus)和transmit shift register之間。

每個資料(character)在傳送前都會有一個低電位的起始位for one bit period，並由可配置數量的停止位結束。

USART支援以下停止位：0.5, 1, 1.5 和 2 個停止位。

Note：
在數據傳輸期間，不應重置TE位元。重置TE位元會在傳輸過程中損壞TX引腳上的數據，因為波特率計數器會被凍結，當前正在傳輸的數據會丟失。在啟用TE位元後，會發送一個空閒幀(idle frame)。

### 設置停止位
每個資料傳輸的停止位數量可以在控制暫存器2(USART_CR2)的第13和第12位進行設定
	- 1 bit的stop bit：預設的默認停止位位元數 
	- 2 bits的stop bit：normal USART, single-wire 和 modem modes 
	- 0.5 bits的stop bit：Smartcard mode接收數據用 
	- 1.5 bits的stop bit：Smartcard mode發送數據用

一個閒置幀(idle frame)傳輸包含停止位

stop bits其實不算是個bit，他是傳輸結束後的一段時間(period)，用以區隔每個傳輸的資料，其功用是在非同步傳輸的時候可以告訴接收器，資料傳輸已經結束。透過增加stop bits的長度，可讓接收器能有足夠的時間可以處理該資料

當m = 0時，一個break transmission有10個低位元；而m = 1時，則有11個低位元，並且無法傳輸transmit long breaks（長度超過10/11個低位元）。

另外，由於transmit shift register搬移到TDR中最少需要1/2 baud clock，因此在Smartcard mode的接收中，最少必須設定0.5 bit的stop bits

![alt text](./note%20image/Configurable%20stop%20bits.png)

### 傳送器的設定
1. 設定USART_CR1暫存器的UE位來啟動傳輸
2. 設定USART_CR1暫存器的M位決定資料長度
3. 設定USART_CR2暫存器中的STOP位來決定停止位元的長度
4. 採用多緩衝器的話，則須設定USART_CR3的DMAT啟動DMA，並設置DMA的暫存器
5. 利用USART_BRR暫存器設定baud rate
6. 設置USART_CR1的TE位，在第一筆資料傳送前，傳送一個idle frame
7. 將欲發送的資料放入USART_DR中(這會清除TXE位元)，若有多筆資料要傳送，則重複步驟
8. 一個frame的資料發送完畢後，TC位會被設定(TC=1)，這表示最後一個frame的傳輸已完成。這在禁用USART或進入停止模式(Halt mode)時是必須的，以避免破壞最後一次傳輸。

### Single byte communication
![alt text](./note%20image/TC,TXE%20behavior%20when%20transmitting.png)

- 當資料放入USART_DR會由硬體清除TXE位，則表示: 
	1. 資料已從TDR中進入transmit shift register，資料的發送已開始
	2. TDR暫存器已被清空
	3. 下一筆資料可放入USART_DR中

- 若TXEIE位被設置，則會產生一個中斷

- 如果USART正在發送資料，對USART_DR的寫入資料會移到TDR暫存器中，並在目前的資料傳送結束後把該筆資料移進移位暫存器(transmit shift register)中

- 如果USART沒有在發送資料，則對USART_DR的寫入資料會直接放入移位暫存器，並啟動傳送，當傳送開始時，硬體會立即設定TXE位

- 如果一個frame被傳輸(在停止位之後)，且TXE位被設置，TC位將變為高電平。如果USART_CR1暫存器中的TCIE位被設置，則會產生中斷。

- 在將最後一個數據寫入USART_DR寄存器後，必須等待TC=1，才能禁用USART或讓微控制器進入低功耗模式。

- 先讀取USART_SR暫存器，再寫入USART_DR暫存器，則可清除TC位
- TC位也可以通過寫入'0'來清除。這種清除程序僅建議在多緩衝區(Multibuffer communication)通訊時使用。

### 傳送斷開符號
透過設定USART_CR1的SBK位，可以發送一個斷開符號，斷開符號的長度取決於M位。
如果SBK=1，則在目前的資料發送後，會再TX線上發送一個斷開符號，當傳送完成後，會由硬體恢復SBK位。
USART會由硬體在最後一個斷開符號的結束處插入一個’1’，確保能辨識下一個資料的起始位。

### 傳送空閒符號
設置USART_CR1的TE位會使得USART在發送第一筆資料前，發送一個空閒符號，喚醒接收端。



## 接收器
接收器依據USART_CR1 M位的狀態來決定接收8或9位元的資料。
在USART接收期間，RX從資料最低有效位元(least significant bit)開始接收，因此在此模式中，USART_DR和received shift register之間包含一個緩衝器(RDR)。





# Fractional baud rate generation的設定
接收器和傳送器的Baud rate分別由USART_BRR設置USARTDIV的整數部分(Mantissa)及小數部分(Fraction)，計算方式如下所示:

 ![alt text](./note%20image/波特率公式.png)

- 範例 : https://blog.csdn.net/m0_50728139/article/details/113747627
 ![alt text](./note%20image/波特率計算範例.png)

其中USARTDIV為一個無號的定點數(unsigned fixed point number)，fCK為給周邊設備的時鐘。

- 當OVER8 = 0 時，表示取樣率小數部分佔USART_BRR的DIV_Fraction[3:0]，共 4 bits
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



