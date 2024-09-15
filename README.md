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

# 待辦事項
1. 看 Makefile (知道幹嘛就好)
2. 安裝makefile
3. context switch 是什麼
4. 中斷是什麼
5. startup.c 在做什麼
6. linker.ld 是什麼

**參考資料**
- 筆記：<https://hackmd.io/@zoanana990/S1RySgR3A>
- MarkDown語法：<https://hackmd.io/@eMP9zQQ0Qt6I8Uqp2Vqy6w/SyiOheL5N/%2FBVqowKshRH246Q7UDyodFA?type=book>


# 作業系統
- RTOS基礎知識 : https://www.freertos.org/zh-cn-cmn-s/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/01-RTOS-fundamentals
- 作業系統 : https://hackmd.io/@Chang-Chia-Chi/OS-CH3
- C語言編譯流程 : https://hackmd.io/@zoanana990/S1RySgR3A
- Context Switch範例 : https://github.com/jserv/mini-arm-os/tree/master/02-ContextSwitch-1


# Makefile 簡介

## 安裝 Make 工具
### 使用 Git for Windows 安裝 Make
- Git for Windows 自帶了 `make` 工具。，並安裝 `Git Bash` 終端。安裝 Git for Windows 會自動安裝 `make`。
- 安裝步驟：
1. 前往 Git for Windows 官方網站 下載安裝程式。
2. 安裝完成後，打開 `Git Bash`。
3. 在 `Git Bash` 中輸入 `make --version`，檢查是否安裝成功。

### 如果 `make` 工具未安裝
- 在 Windows 上，`make` 不是預設安裝的工具。如果在 `Git Bash` 中輸入 `make --version` 並收到 `bash: make: command not found`的錯誤訊息，則需要手動安裝 `make` 工具。

### 透過 Chocolatey 安裝 Make
1. 安裝 Chocolatey：
- 如果尚未安裝 Chocolatey，請參考 官方指南 進行安裝。
2. 安裝 Make 工具：
- 打開 `Git Bash` 或 PowerShell，輸入以下指令來安裝 `make`：
```bash
choco install make
```
- 安裝完成後，關閉並重新打開 `Git Bash`，再次輸入 `make --version` 來確認安裝是否成功。

## 1. Makefile 是什麼？

`Makefile` 是一個用來管理和自動化程式編譯過程的文件。它通常搭配 `make` 工具來使用，`make` 工具可以根據 `Makefile` 的內容自動執行指定的指令。

在 `Makefile` 中，可以定義：
- 目標 (targets)：要生成的檔案或要執行的操作（如編譯出可執行檔）。
- 相依性 (dependencies)：要生成目標所需的檔案或其他程式碼。
- 規則 (rules)：指定如何從相依檔案生成目標檔案的指令。

## 2. Makefile 的作用

在簡單專案中，你可以用單一指令來編譯，如：
```bash
g++ -S main.cpp -o main.o
```
但當專案變得複雜，涉及多個檔案時，手動輸入每個檔案的編譯指令就變得不切實際。Makefile 能夠解決這個問題，提供以下功能：

- 自動化編譯流程：只需要執行 `make`，Makefile 就會根據定義的規則自動進行編譯。

- 管理多個檔案：當專案中有多個 `.cpp` 或 `.h` 檔案時，Makefile 會追蹤它們的相依性，自動編譯相關檔案。

- 增量編譯：Makefile 會只編譯修改過的檔案，不會重新編譯整個專案，大幅提升編譯效率。

## 3. Makefile 與 C 語言編譯流程的關聯

C 語言的編譯流程分為四個階段：

1. 預處理 (Pre-processing)：處理巨集、引入標頭檔案等。
2. 編譯 (Compilation)：將 C 語言原始碼轉換為中間的組合語言。
3. 組譯 (Assembly)：將組合語言轉換為目標機器碼（.o 檔）。
4. 連結 (Linking)：將多個物件檔連結成可執行檔案。

`Makefile` 幫助自動化這些步驟，根據定義的規則生成所需的目標檔案。你可以在 `Makefile` 中定義編譯和連結的指令，並自動完成整個編譯過程。

## 4. Makefile 的優點

1. 簡化編譯過程：你只需執行 `make` 指令，Makefile 就會完成編譯所需的所有工作。
2. 提高效率：Makefile 會只編譯變更過的檔案，減少不必要的重複編譯。
3. 統一編譯指令：專案成員都可以使用相同的編譯流程，確保一致性。

## 總結
Makefile 是管理和自動化專案編譯過程的有效工具，特別適合管理多個檔案的專案。它可以簡化 C 語言編譯流程中的每個步驟，讓你不必手動輸入繁瑣的指令，大大提升開發效率。