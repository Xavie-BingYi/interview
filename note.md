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