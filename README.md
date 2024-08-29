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
	- output : (0x14) or bit 0~15為set bit 16~31為reset bit(0x18)
	- intput : (0x10)
- set pull-up/-down
- set speed 

# C語言編譯流程