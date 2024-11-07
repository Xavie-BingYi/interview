
# 參考資料
- 筆記：<https://hackmd.io/@zoanana990/S1RySgR3A>
- MarkDown語法：<https://hackmd.io/@eMP9zQQ0Qt6I8Uqp2Vqy6w/SyiOheL5N/%2FBVqowKshRH246Q7UDyodFA?type=book>
- 面試常問：<https://hackmd.io/@6PPVul2mS7OX2GuGXlUCDA/behavior_questions>


# 待辦事項
## 面試計畫 (以下完成後，遞履歷)
### 1. 通訊協定與基礎練習
- **SPI / I2C / UART**：複習並實作這些通訊協定，確保能正確使用 `printf` 進行 debug。

### 2. 中斷機制理解
- **研究與實作中斷**：EXTI（外部中斷）與 NVIC（中斷控制器）的運作機制。
- **參考範例**：
  - https://github.com/zoanana990/qubitas/blob/main/driver/exti.c
  - https://github.com/zoanana990/qubitas/blob/main/driver/nvic.c

### 3. MCU 系統架構
- **STM32 系統啟動流程**：
  - 了解 STM32 的開機配置 (MCU 怎麼開機、怎麼燒進去、從哪裡開機)
  - 查看 STM32 SPEC 的 2.4 Boot configuration，確認使用的引腳和記憶體（有兩根 pin 告訴我從哪一個記憶體開機）。
- **Linker file**：
  - 理解為什麼要使用 `flash.ld` 而非 `ram.ld`。

### 4. 計算機組織 (從之乎找文章)
- **Cache**：理解快取的作用、結構及其效能影響。
- **Pipeline**：學習指令流水線處理，分析其優化效能的方法。

### 5. 作業系統 (從之乎找文章)
- **Lock 和 Semaphore**：理解在多線程環境中如何運用這些同步機制。


## 面試期間計畫 (遞完履歷到面試期間)
### 1. 編譯流程與工具
- **Makefile**：查看 Makefile，包含需要了解 `startup.c` 和 `linker.ld` 的作用。

### 2. 刷題練習
- **群聯三題**：
  - https://m033010041.github.io/2020/02/18/phison-interview-practice/
  - https://hackmd.io/@jQSlhiN3QaGkugpbD4NDAA/rkvNkyQfi
- **LeetCode** (https://leetcode.com/problem-list/linked-list/)
  1. 排序 (Sorting)
  2. Linked List（會順便練到 structure 與 queue）：*206、*707 特別重要
  3. 熟練指標 (pointer)

### 3. 要複習的
1. 台達工作時 GPIO & SPI 怎麼控的
2. **SPI 的 mode (clock/phase) 與協議**、全/半雙工、baud rate，以及遇到的 bug；若除頻後的速度高於 slave 可支援的速度會發生什麼事（掉封包？）
3. **Polling task 跟 interrupt 的差異**；thread 是基於 polling（了解 thread，請 ChatGPT 寫一個 pthread sample code），查看作業系統第三、四章
4. **作業系統 Task** 的概念
5. 面試可能會問：為什麼在某些情況下選擇 polling 而非中斷

### 4. 奇景問過的問題
1. "曾經開發過甚麼專案,請畫出架構圖,之後討論。
2. 如何確保示波器測量正確與準確？
3. "高階語言與組合語言的差別"：高階語言無需分架構，組合語言則需分架構，涉及計算機組織。
4. V_il、V_ih、V_ol、V_oh 分別代表甚麼意思？
5. 你用過什麼軟體開發工具？先問清楚問題，甚麼是軟體開發工具，例如，vscode、gcc 等，還是在問語言。
6. 專業考試的程式題包含排序，例如給一個陣列，從大排到小。
7. 請詳細描述 Interrupt Handler 與 Work Queue 的差別？為何要將工作放在 work queue 執行，而不直接在 interrupt handler 中執行？
8. 專業考試可能包含 I2C 通訊協定、C 語言排序、三用電錶及示波器操作。

## 上班後需要練習
### 1. 作業系統
- **學習課程**： [作業系統課程 (YouTube Playlist)](https://youtube.com/playlist?list=PL9jciz8qz_zyO55qECi2PD3k6lgxluYEV&si=ovEepz80kEPKwIKi)
- **學習重點**：
  - Context Switch 概念
  - IPC & Thread（可同時參考 FreeRTOS 或相關案例）
  - Multi-thread 和 Multi-process 的 IPC 概念（需熟悉第三章 Process）
  - Preemptive、Round Robin（第五章，Scheduler）
  - Mutex、Semaphore、Spinlock（第六章，建議參考 Google）
  - Deadlock 防範（第七章，建議參考 Google）
- **範例學習**：
  - [mini-arm-os - Timer Interrupt](https://github.com/jserv/mini-arm-os/tree/master/05-TimerInterrupt)
  - [mini-arm-os - Threads](https://github.com/jserv/mini-arm-os/tree/master/07-Threads)

### 2. 計算機組織
- **學習課程**：[計算機組織課程 (YouTube Playlist)](https://www.youtube.com/playlist?list=PLj6E8qlqmkFvSHyGAFqY4sX0Ee7eLqmq2)
- **計算機組織筆記**：[HackMD 筆記](https://hackmd.io/@HsuChiChen/computer_organization)

### 3. FreeRTOS
- 學習如何呼叫 FreeRTOS API，並了解其內部運作。

### 4. 專案：實作簡易作業系統 (mini-arm-os)
- **目標**：創建一個簡易作業系統，模擬基本指令操作。
  1. 實現鍵盤輸入，並在 MCU 上顯示輸入的文字（使用 Tera Term）。
  2. 開發 Command 處理邏輯，按下 Enter 後 MCU 可以執行該 Command。
  3. 類似 CMD 指令 `dir` 可顯示資訊，依據此概念，實作在 Tera Term 打字，可以跳出對應的數值。
  4. 查看所有 Process 的狀態，實現背景程式每 5 秒閃爍一次 LED；並新增指令可刪除背景程式，並允許新增新背景程式的指令。

### 5. 考古題
- **指標操作 (Pointer)**：
  - `pointer++`（指標加法）、`mem addr`（記憶體位址）、`function pointer`（函數指標）
- **值呼叫 (Call by Value) 與引用呼叫 (Call by Reference)**：
  - function 的 call by value 相關考題，通常搭配指標一起考。
  - C++ 的 Call by Reference。
- **其他重點**：
  - Static 變數的生命週期
  - Union、Struct、Enum
  - Bitfield
  - Volatile 關鍵字
  - Bitwise 操作
  - Linked List：插入 (insert)、刪除 (remove)、排序 (sort)
- **推薦資源**：
  - [HackMD 考題總結](https://hackmd.io/@Rance/SkSJL_5gX)（包含上述重點）

### 6. 若選擇軟體工程職位，需額外研讀
1. **資料結構**：軟韌當目標不用看，寫軟體要看。
2. **計算機結構**：台灣的講得不好，建議參考 Berkeley 的 CS152 課程。
3. **Linux Kernel**：Memory Management（想進google要看）


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
