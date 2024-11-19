
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



# I²C
## Mode selection
該介面可以在以下四種模式之一運行：
- 從機傳輸模式
- 從機接收模式
- 主機傳輸模式
- 主機接收模式

預設情況下，它運行在從機模式。當它生成 START 條件後，介面會自動從從機切換為主機；如果發生仲裁失敗或生成 STOP 條件，則會自動從主機切換為從機，這支持多主機功能。

數據與位址以 8 位元位組的形式傳輸，MSB先傳。起始條件後的第一個位元組包含位址資訊（7 位元模式下為一個位元組，10 位元模式下為兩個位元組）。在主機模式中，位址總是由主機傳輸。

在位組傳輸的 8 個時鐘週期後，會有第 9 個時鐘脈衝，此時接收器必須向發送器發送一個確認位元（Acknowledge）。

確認訊號（Acknowledge）可以通過軟體啟用或禁用。I²C 介面的位址（包含雙重位址 7 位元/10 位元和/或廣播位址）可以通過軟體選擇。

### 通訊流程
在主機模式下，I²C 介面會啟動數據傳輸並生成時鐘信號。串行數據傳輸總是以起始條件（Start）開始，並以停止條件（Stop）結束。起始和停止條件均由軟體在主機模式下生成。

在從機模式中，介面能夠識別自己的位址（7 位元或 10 位元）以及廣播位址（General Call）。廣播位址的檢測功能可以通過軟體啟用或禁用。

## I²C 從機模式
預設情況下，I²C 介面運行在從機模式。要從預設的從機模式切換到主機模式，需要生成起始條件（Start condition）。

必須在 I²C_CR2 暫存器中設置外部時鐘輸入，以便生成正確的時間參數。外部時鐘頻率必須至少為：

- 標準模式 (Sm)：2 MHz
- 高速模式 (Fm)：4 MHz

當檢測到起始條件時，位址會從 SDA 線接收並傳送到移位暫存器中。然後，該位址會與介面的位址 (OAR1) 進行比較，以及與 OAR2（如果 ENDUAL=1）或廣播位址（如果 ENGC=1）進行比較。

注意：在 10 位元位址模式下，比較包括標頭序列 (11110xx0)，其中 xx 代表位址的兩個最重要位元。

標頭或位址不匹配：介面會忽略該位址並等待另一個起始條件。

標頭匹配（僅限於 10 位元模式）：如果 ACK 位元被設置，介面會生成一個確認脈衝，然後等待 8 位元的從機位址。

位址匹配：介面按順序生成：
- 如果 ACK 位元被設置，則生成一個確認脈衝。
- 硬體設置 ADDR 位元，如果 ITEVFEN 位元被設置，則生成中斷。
- 如果 ENDUAL=1，軟體必須讀取 DUALF 位元以檢查哪個從機位址已被確認。

在 10 位元模式下，接收位址序列後，從機總是處於接收模式。當接收到重複的起始條件，隨後是與位址位元匹配且最不重要位元設置為 1 的標頭序列 (11110xx1) 時，它會進入傳輸模式。

TRA 位元指示從機當前處於接收模式還是傳輸模式。

### 從機傳輸模式
在接收到位址後，清除 ADDR，從機將從 DR 暫存器傳送位元組到 SDA 線，這是通過內部移位暫存器完成的。

從機會將 SCL 拉低以延長時間，直到 ADDR 被清除且 DR 被填滿要傳送的數據（參見圖 241 傳輸排序 EV1 EV3）。

當接收到確認脈衝時：
- 如果 ITEVFEN 和 ITBUFEN 位元被設置，則 TxE 位元會由硬體設置，並生成中斷。

如果 TxE 被設置，但在下一次數據傳輸結束之前沒有將數據寫入 I²C_DR 暫存器，則 BTF 位元被設置，介面會等待直到 BTF 被通過讀取 I²C_SR1 來清除，隨後再寫入 I²C_DR 暫存器，並將 SCL 拉低。

### 從機接收模式
在接收到位址後，清除 ADDR，從機將從 SDA 線接收位元組到 DR 暫存器，這是通過內部移位暫存器完成的。在每個位元組之後，介面會依序生成：
- 如果 ACK 位元被設置，則生成確認脈衝。
- 如果 ITEVFEN 和 ITBUFEN 位元被設置，則 RxNE 位元會由硬體設置，並生成中斷。

如果 RxNE 被設置，但在下一次數據接收結束之前沒有讀取 DR 暫存器中的數據，則 BTF 位元被設置，介面會等待直到 BTF 被通過讀取 I²C_DR 暫存器來清除，並將 SCL 拉低（參見圖 242 傳輸排序）。

### 結束從機通訊
在最後一個數據位元組傳輸後，主機會生成一個停止條件。介面檢測到這個條件並設置：

如果 ITEVFEN 位元被設置，則設置 STOPF 位元並生成中斷。
STOPF 位元可以通過讀取 SR1 暫存器後再寫入 CR1 暫存器來清除（參見圖 242 中的 EV4）。

## I²C 主機模式
在主機模式下，I²C 介面發起數據傳輸並生成時鐘信號。串列數據傳輸總是以起始條件開始，並以停止條件結束。當在總線上生成起始條件與 START 位元時，即選擇主機模式。

以下是主機模式所需的步驟：
- 在 I²C_CR2 暫存器中設置外部時鐘輸入，以便生成正確的時間參數。
- 配置時鐘控制暫存器(clock control registers)。
- 配置上升時間暫存器(rise time register)。
- 程式化 I²C_CR1 暫存器以啟用外部設備。
- 在 I²C_CR1 暫存器中設置 START 位元以生成起始條件。

外部時鐘頻率必須至少為：
- 標準模式 (Sm)：2 MHz
- 高速模式 (Fm)：4 MHz

### SCL 主機時鐘生成
CCR 位元用於生成 SCL 時鐘的高位和低位，從生成上升邊緣和下降邊緣開始（分別）。由於從機可能會延長 SCL 線，外部設備會在上升邊緣生成後的 TRISE 位元設置的時間結束時檢查來自總線的 SCL 輸入。

- 如果 SCL 線為低，這表示有從機正在延長總線，高位計數器會停止，直到檢測到 SCL 線為高。這可以保證 SCL 時鐘參數的最小高位時間。
- 如果 SCL 線為高，高位計數器會繼續計數。

實際上，從外部設備生成 SCL 上升邊緣到外部設備檢測 SCL 上升邊緣的反饋回路，即使沒有從機延長時鐘，也需要一些時間。這段回路的持續時間與 SCL 的上升時間（影響 SCL VIH 輸入檢測）有關，加上 SCL 輸入路徑上噪聲濾波器的延遲，以及內部 SCL 輸入與 APB 時鐘的同步延遲。反饋回路使用的最大時間由 TRISE 位元設置，因此無論 SCL 上升時間為何，SCL 頻率都保持穩定。

### 起始條件
設置 START 位元會使介面生成起始條件並在 BUSY 位元被清除時切換到主機模式（設置 MSL 位元）。

注意：在主機模式下，設置 START 位元會導致介面在當前位元組傳輸結束時生成重啟條件。

一旦發送起始條件：
- SB 位元會由硬體設置，如果 ITEVFEN 位元被設置，則會生成中斷。

然後主機會等待讀取 SR1 暫存器，隨後在 DR 暫存器中寫入從機位址（參見圖 243 和圖 244 傳輸排序 EV5）。


### 從機位址傳輸
然後，從機位址通過內部移位暫存器發送到 SDA 線。

- 在 10 位元位址模式下，發送標頭序列會導致以下事件：
    - 如果 ITEVFEN 位元被設置，則由硬體設置 ADD10 位元並生成中斷。
    
    然後，主機會等待讀取 SR1 暫存器，隨後在 DR 暫存器中寫入第二個位址位元組（參見圖 243 和圖 244 傳輸排序）。
    - 如果 ITEVFEN 位元被設置，則由硬體設置 ADDR 位元並生成中斷。
    
    然後，主機會等待讀取 SR1 暫存器，隨後讀取 SR2 暫存器（參見圖 243 和圖 244 傳輸排序）。

- 在 7 位元位址模式下，發送一個位址位元組。
    一旦位址位元組被發送，
    - 如果 ITEVFEN 位元被設置，則由硬體設置 ADDR 位元並生成中斷。
    
    然後，主機會等待讀取 SR1 暫存器，隨後讀取 SR2 暫存器（參見圖 243 和圖 244 傳輸排序）。

主機可以根據發送的從機位址的最低有效位 (LSB) 決定進入發送者或接收者模式。
- 在 7 位元位址模式下：
    - 要進入發送者模式，主機發送 LSB 為 0 的從機位址。
    - 要進入接收者模式，主機發送 LSB 為 1 的從機位址。
- 在 10 位元位址模式下：
    - 要進入發送者模式，主機發送標頭 (11110xx0)，然後發送從機位址（其中 xx 表示位址的兩個最有效位）。
    - 要進入接收者模式，主機發送標頭 (11110xx0)，然後發送從機位址。然後應發送重啟條件，隨後發送標頭 (11110xx1)，（其中 xx 表示位址的兩個最有效位）。
    TRA 位元指示主機當前處於接收者還是發送者模式。

### 主機發送者模式
在位址傳輸後，清除 ADDR 之後，主機通過內部移位暫存器將位元組從 DR 暫存器發送到 SDA 線。

主機會等待第一個數據位元組寫入 I2C_DR（參見圖 243 傳輸排序 EV8_1）。

當接收到確認脈衝時，如果 ITEVFEN 和 ITBUFEN 位元被設置，則 TxE 位元由硬體設置並生成中斷。

如果 TxE 被設置，但在上次數據傳輸結束之前未寫入數據位元組到 DR 暫存器，則 BTF 被設置，接口會等待直到 BTF 被寫入 I2C_DR 清除，並將 SCL 拉低。

### 關閉通訊
在最後一個位元組寫入 DR 暫存器後，軟體設置 STOP 位元以生成停止條件（參見圖 243 傳輸排序 EV8_2）。接口會自動返回到從機模式（MSL 位元被清除）。

注意：停止條件應在 EV8_2 事件中編程，當 TxE 或 BTF 被設置時。

### 主機接收者模式
在位址傳輸後，清除 ADDR 之後，I2C 介面進入主機接收者模式。在此模式下，介面通過內部移位暫存器從 SDA 線接收位元組到 DR 暫存器。每接收到一個位元組後，介面會依序生成：
1. 如果 ACK 位元被設置，則生成一個確認脈衝
2. 如果 ITEVFEN 和 ITBUFEN 位元被設置，則 RxNE 位元被硬體設置並生成中斷（參見圖 244 傳輸排序 EV7）。

如果 RxNE 位元被設置，而在最後一次數據接收結束之前未讀取 DR 暫存器中的數據，則硬體設置 BTF 位元，並且介面會等待直到通過讀取 DR 暫存器清除 BTF，並將 SCL 拉低。

### 關閉通訊
主機對從機傳送的最後一個位元組發送 NACK。在接收到此 NACK 後，從機釋放 SCL 和 SDA 線的控制權。然後，主機可以發送停止/重啟條件。
1. 要在最後接收到的數據位元組之後生成非確認脈衝，ACK 位元必須在讀取倒數第二個數據位元組後立即清除（在倒數第二個 RxNE 事件之後）。
2. 為了生成停止/重啟條件，軟體必須在讀取倒數第二個數據位元組後（在倒數第二個 RxNE 事件之後）設置 STOP/START 位元。
3. 如果只需接收一個位元組，則在 EV6 期間禁用確認（在 ADDR 標誌清除之前），並在 EV6 之後生成停止條件。

在停止條件生成後，介面會自動返回到從機模式（MSL 位元被清除）。

### FREQ
- FREQ 位元 指的是 APB 時脈的頻率，通常用來設定 I2C 的暫存器，讓它知道 MCU 的時脈頻率是多少，從而正確生成資料的設置時間和保持時間。
- TPCLK 是 APB 時脈的週期，也就是每一個時脈週期所需的時間。

換句話說，FREQ 代表的是頻率（例如 MHz），而 TPCLK 代表的是這個頻率對應的時間週期（例如 ns）。
兩者是互為倒數的關係：`TPCLK = 1 / FREQ`​

因此，當你在 FREQ 位元欄設定 APB 的頻率時，TPCLK 會根據這個頻率來計算。例如，如果 FREQ 設為 8 MHz，TPCLK 就是 125 ns。

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
透過設定USART_CR1的SBK位，可以發送一個中斷字符（break character），中斷幀(break frame)的長度取決於M位。
如果SBK=1，則在目前的資料發送後，會在TX線上發送一個中斷字符，當傳送完成後，會由硬體恢復SBK位（在中斷字符的停止位期間）。
USART會由硬體在最後一個中斷字符的結束處插入一個'1'，確保能辨識下一個資料的起始位。

- 如果軟體在中斷傳輸開始之前重置SBK位，則中斷字符不會被傳輸。若要傳輸兩個連續的中斷字符，應在前一個中斷字符的停止位之後再設置SBK位。

### 傳送空閒符號
設置USART_CR1的TE位會使得USART在發送第一筆資料前，發送一個閒置幀(idle frame)，喚醒接收端。

## 接收器
接收器依據USART_CR1 M位的狀態來決定接收8或9位元的資料。

### 起始位偵測
.. image:: /usart_fig300.jpg

Ref: RM0090 Reference Manual P.954<http://www.st.com/web/en/resource/technical/document/reference_manual/DM00031020.pdf>_

在USART中，如果辨認出一個特殊的採樣序列( 1 1 1 0 X 0 X 0 X 0 0 0 0 )，則認定偵測到一個起始位。

如果該序列不完整，則接收端退回起始位偵測並回到空閒狀態，等待下一次的電壓下降。

如果三個採樣點上有僅有兩個是0(第3、5、7採樣點或8、9、10採樣點)，則依然判定為偵測到一個起始位，但NE(噪音標誌)會被設定

採樣的時間間隔

.. image:: /usart_sampling.png Ref: UART receiver clock speed<http://electronics.stackexchange.com/questions/42236/uart-receiver-clock-speed>_

假設baud rate = 9600 bps，則一個bit的傳輸時間為104us，usart會在接收器啟動後的52us，開始採樣

若偵測到開始位元，則開始接收資料，反之則等待104us，再採樣一次

### 資料的接收
在USART接收期間，RX從資料最低有效位元(least significant bit)開始接收，因此在此模式中，USART_DR和received shift register之間包含一個緩衝器(RDR)。

### 接收器的設定
.. image:: /usart_recevier.png

設定USART_CR1暫存器的UE位來啟動USART接收，如圖中的(a)
設定USART_CR1暫存器的M位決定資料長度，如圖中的(b)
設定USART_CR2暫存器中的STOP位來決定停止位元的長度，如圖中的(c)
採用多緩衝器接收資料，則須設定USART_CR3的DMAR啟動DMA，並設置DMA的暫存器，如圖中的(d)
利用USART_BRR暫存器設定baud rate，如圖中的(e)
設定USART_CR1暫存器中的RE位，啟動接收器，並開始偵測起始位，如圖中的(f)
當資料被接收到後: 1. 硬體會設定RXNE位，表示received shift register中的資料已移入RDR中，亦即資料已被接收並可被讀出，如圖中的(g) 2. 若USART_CR1中的RXNEIE被設定時，會產生一個中斷，如圖中的(h) 3. 資料接收期間如檢測到frame錯誤或是噪音、溢出錯誤等問題，相關的標誌將被設定(FE、NF、ORE) 4. 藉由讀取USART_DR可清除RXNE位，RXNE位必須要在下一資料接收前被清除，以免產生溢出錯誤 5. 在DMA接收時，RXNE在每個字元接收後被設置，並因DMA讀取RDR而被清除

### 接收斷開符號
USART在接收斷開符號後，可像處理frame錯誤一樣處理

### 接收空閒符號
當空閒符號被偵測到時，USART處理步驟如同一般資料一樣處理，但如果USART_CR1的IDLEIE被設置時，將會產生一個中斷

### 溢出錯誤
若RXNE沒有被覆位，此時又接收到一個新資料，則會發生溢出錯誤，如圖中的(i)

當溢出錯誤產生時: 1. USART_SR中的ORE位將被設置，如圖中的(j) 2. RDR中的內容將不會被清除，因此讀取USART_DR仍可以得到之前的資料 3. 若USART持續在接收中，則Received shift register中的資料將被覆蓋 4. 如果RXNEIE被設置，或是EIE(Error interrupt enable)和DMAR位被設定，則會產生一個中斷，如圖中的(k) 5. 依序讀取USART_SR和USART_DT暫存器，可清除ORE位

當ORE位被設置時，表示至少有一個資料已遺失，有以下兩種可能性: 1. 如果RXNE=1，表示之前的資料還在RDR中，且可被讀出 2. 如果RXNE=0，表示之前的資料已被讀走，RDR已無資料可被讀取，此種情況發生在讀取RDR中上一筆資料時，又接收到新的資料時發生。

### 噪音錯誤
透過不同的採樣技術，可以區分有效的輸入資料和噪音，並進行資料恢復。

透過設定USART_CR1中的OVER8位可選16或8次的採樣，見Fig. 250和Fig. 251: - OVER8 = 1: 採用8次採樣，採樣的頻率較快(最高頻率為fPCLK/8) - OVER8 = 0: 採用16次採樣，採樣的頻率最高為fPCLK/16

設定USART_CR3中的ONEBIT位可選則不同的採樣技術: - ONEBIT = 0: 採樣資料中心的 3 bits，若此3 bits不相等，則NF位會被設定 - ONEBIT = 1: 只採樣中心的單一bit，此時NF的檢測將會被取消

當在資料接收中檢測到噪音時: - NE會在RXNE位的升緣時被設定 - 無效的資料會從received shift register移入USART_DR暫存器中 - 在單一資料的接收下，不會有中斷產生，但透過NE和RXNE位的設置，由後者來產生中斷；

 在多緩衝器的接收中，如果USART_CR3暫存器中的EIE位被設定，則會產生一個中斷
.. image:: /oversampling16.png

.. image:: /oversampling8.png

.. image:: /noisedetection.png

.. image:: /noisedetectionsampledata.png

Ref: RM0090 Reference Manual P.957~958<http://www.st.com/web/en/resource/technical/document/reference_manual/DM00031020.pdf>_

### Frame錯誤
由於沒有同步上或線路上大量的噪音，使得停止位沒有在預期的時間上接收和識別出來，則發生Frame錯誤

當Frame錯誤被檢測出時: 1. FE位被設定，如圖中的(l) 2. 無效的資料從received shift register移入USART_DR暫存器中 3. 在一般資料的接收下，不會有中斷產生，可藉由RXNEIE位的設置，在中斷中檢測FE位得知發生錯誤；

  在DMA的接收中，如果USART_CR3暫存器中的EIE位被設定，則會產生一個中斷，如圖中的(k)
依序讀取USART_SR和USART_DR暫存器可恢復FE位


## Fractional baud rate generation的設定
### 時鐘頻率與 Baud Rate 的關係
在數據通訊中，時鐘頻率（Clock Frequency） 和 Baud Rate 是兩個關鍵概念，但它們的作用不同：

1. 時鐘頻率（Clock Frequency）：決定每秒的時鐘周期數。比如，若時鐘頻率是 10M Hz，意味著每秒可以產生 10 百萬次時鐘信號。
2. Baud Rate：指的是每秒能傳輸的比特數。例如，9600 bps 意味著每秒可以傳輸 9600 個比特。

#### 範例：
假設你需要傳輸 11000 個比特的數據，通訊時鐘頻率為 10M Hz，而 Baud Rate 設定為 9600 bps。
- 第一秒：你可以在 1 秒內傳輸 9600 個比特，這是由於 Baud Rate 限制了每秒能傳輸的比特數，即使時鐘頻率是 10M Hz，也只會以 9600 bps 傳輸數據。
- 第二秒：剩餘的 1400 比特將在第二秒繼續傳輸完成。

這意味著即使你的時鐘頻率很高，數據的傳輸速率仍然會受到 Baud Rate 的限制，無法超過 9600 bps。

#### 關鍵點：
- 時鐘頻率 決定了傳輸過程中每個比特的傳輸速度，但 Baud Rate 限制了每秒可傳輸的總數據量。即使時鐘頻率很高，如果 Baud Rate 設定較低，傳輸速率也會相對較慢。
---
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

