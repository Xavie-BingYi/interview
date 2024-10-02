
# 待辦事項
1. 中斷是什麼
面試會問到為甚麼不用中斷而是用polling
2. context switch 是什麼
3. 看 Makefile，包含需要了解startup.c linker.ld在做什麼
4. 製作PPT與練習面試 
https://hackmd.io/@6PPVul2mS7OX2GuGXlUCDA/behavior_questions
5. 考古題
https://hackmd.io/@jQSlhiN3QaGkugpbD4NDAA/rkvNkyQfi
6. 作業系統(process semaphore memory virtual memory cache filesystem)
https://youtube.com/playlist?list=PL9jciz8qz_zyO55qECi2PD3k6lgxluYEV&si=ovEepz80kEPKwIKi 

## 排程
- 9/30 ~ 10/4(5天) 
    1. context switch(caller, callee, push, pop)：
    2. 搞懂Makefile
    3. interrupt(hello.c) : https://github.com/jserv/mini-arm-os/tree/master/05-TimerInterrupt
    4. https://github.com/jserv/mini-arm-os/tree/master/07-Threads
- 10/5 ~ 10/12(8天) 複習 SPI, GPIO, UART, printf, 台達工作(陳昕佑出國)
- 10/13 ~ 10/15(3天) 中斷實作
- 10/16(1天) 修正面試PPT + PPT加入中斷內容 + 複習PPT內容 + 遞履歷
    - 面試會問的問題 (要人家問甚麼都答得出來的程度)
        1. 台達工作時 GPIO & SPI 怎麼控的
        2. SPI的mode(clock/phase) & 協議 & 全/半雙工 & baud rate & 遇到的bug ；如果除頻完後的速度高於slave可以支援的速度會發生甚麼事(掉封包?)
        3. polling task跟interrupt的差異；thread polling是base on (了解thread;叫chatgpi 寫一個pthread sample code),看作業系統第三,四章
        4. 作業系統要了解task是甚麼

- 10/17 ~ 刷考古題後看作業系統


# 參考資料
- 筆記：<https://hackmd.io/@zoanana990/S1RySgR3A>
- MarkDown語法：<https://hackmd.io/@eMP9zQQ0Qt6I8Uqp2Vqy6w/SyiOheL5N/%2FBVqowKshRH246Q7UDyodFA?type=book>
- C語言編譯流程 : https://hackmd.io/@zoanana990/S1RySgR3A

# 更新程式碼到 Git 上（使用 Sourcetree）
在使用 Git 進行版本控制時，檔案通常分為兩種類型：有追蹤的檔案和沒有追蹤的檔案。

**有追蹤的檔案**
- 有追蹤的檔案：這些檔案已經被 Git 追蹤，您可以看到哪些行被新增或刪除。

**沒有追蹤的檔案**
- 沒有追蹤的檔案：這些檔案尚未被 Git 追蹤，需要先將它們添加到 Git 的追蹤中。

## 步驟 1: 將檔案加入追蹤
首先，需要將修改過的檔案添加到 Git 的暫存區。這樣做會告訴 Git 哪些檔案的變更，應該被記錄。
```commandline=
git add <你的檔案名稱>
```
如果要添加所有修改過的檔案，可以使用：
```commandline=
git add .
```
## 步驟 2: 提交變更
在將檔案添加到暫存區後，需要撰寫提交信息（commit message）來描述這些變更。
```commandline=
git commit -m "你的提交訊息"
```
提交訊息應該簡潔明瞭，清楚描述這次變更的內容。

## 步驟 3: 上傳到伺服器
最後，可以將提交的變更上傳到遠端伺服器（如 GitHub）。
```commandline=
git push
```

# 計算機基本概念
## 計算機組織（Computer Organization）
計算機組織主要研究硬體層面的組成部分如何協同工作，包括：
- CPU 的內部架構：包含暫存器、ALU（算術邏輯單元）、控制單元等組件。
- 記憶體層次架構：包括 Cache（快取）和 RAM（隨機存取記憶體）之間的數據交換與互動。

### CPU（中央處理器）
CPU 是計算機中進行數據處理的核心元件，包含以下重要的子模塊：
1. 暫存器：CPU 內部的一組小型存儲器，用於暫時存放數據和指令，為 ALU 提供運算數據。
2. ALU（算術邏輯單元）：負責執行算術（加減乘除）和邏輯（與、或、非）運算。
3. 控制單元：負責從記憶體中抓取指令，並協調 CPU 內部的各個組件來執行這些指令。

以上這些組件都集成在同一顆 CPU IC（集成電路）中，它們共同協助完成數據處理和指令執行。

### RAM（隨機存取記憶體）
RAM 是 CPU 外部的一顆獨立 IC，用於存儲程式和數據。CPU 通過總線（Bus）與 RAM 進行連接，讀取需要處理的數據和指令，並將處理結果回寫到 RAM。

### Cache（快取記憶體）
Cache 是位於 CPU 內的一部分小型快速存儲器，負責暫存經常使用的數據。它的目的是減少 CPU 與 RAM 之間的訪問延遲，加快數據存取速度。Cache 通常包含在 CPU IC 中，幫助提升 CPU 整體運行效率。

## 計算機結構（Computer Architecture）
計算機結構偏向於設計和優化計算機系統，使其具備更高的效能。主要涉及以下內容：

1. 指令集架構（ISA）：定義 CPU 如何理解和執行指令。指令集是處理器與軟體之間的界面。
2. 處理器設計原理：研究如何提高處理效能，採用技術如超純量架構（同時執行多條指令）和管線化（分段處理指令）。
3. 記憶體管理：包括多層次的記憶體系統設計，如 Cache 層次結構和虛擬記憶體，目的是更有效地管理數據存取。

## 嵌入式系統（Embedded Systems）
嵌入式系統專注於在特定應用中使用處理器和記憶體的設計。通常會涉及部分 CPU 的內部架構和記憶體層次知識，並且強調資源有限環境下的效能優化。嵌入式系統通常會搭配特殊的處理器和記憶體組件，實現特定功能，例如物聯網裝置、工業控制系統等。
