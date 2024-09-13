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

# 陳昕佑筆記
- 筆記：<https://hackmd.io/@zoanana990/S1RySgR3A>
- MarkDown語法：<https://hackmd.io/@eMP9zQQ0Qt6I8Uqp2Vqy6w/SyiOheL5N/%2FBVqowKshRH246Q7UDyodFA?type=book>

# 作業系統
- RTOS基礎知識 : https://www.freertos.org/zh-cn-cmn-s/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/01-RTOS-fundamentals
- 作業系統 : https://hackmd.io/@Chang-Chia-Chi/OS-CH3
- C語言編譯流程 : https://hackmd.io/@zoanana990/S1RySgR3A
- Context Switch範例 : https://github.com/jserv/mini-arm-os/tree/master/02-ContextSwitch-1

搞懂
1. 中斷是什麼
2. context switch 是什麼
3. startup.c 在做什麼
4. linker.ld 是什麼