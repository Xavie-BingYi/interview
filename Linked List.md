# 目錄
1. [什麼是 Linked List？](#一什麼是-linked-list)
2. [Linked List 與 RTOS 的關係](#二linked-list-與-rtos-的關係)
3. [Linked List 在嵌入式系統中的角色](#三linked-list-在-嵌入式系統中的角色)
4. [單向鏈表（Singly Linked List）](#四單向鏈表singly-linked-list)
5. [.ld 檔案與 Linked List 的區別](#五ld-檔案與-linked-list-的區別)
6. [如何在嵌入式專案中實作 Linked List？](#六如何在嵌入式專案中實作-linked-list)
7. [小結](#七小結)

# 一、什麼是 Linked List？

Linked List（連結串列）是一種資料結構，**由一串節點（nodes）組成，每個節點包含兩部分：資料和指向下一個節點的指標（pointer）。**  

Linked List 的核心精神在於**動態性**與**靈活性**。它不需要像陣列（array）那樣佔用連續的記憶體空間，而是透過指標串接分散的記憶體區塊，使其在某些應用場景中比陣列更有效率。

## Linked List 的應用與特點

Linked List 常見於：
- 資料庫管理系統
- 記憶體管理
- 任務排程
- RTOS（即時作業系統）的內部結構，例如任務控制塊（Task Control Block, TCB）的管理

學會 Linked List 有助於掌握：
- 動態記憶體分配
- 指標操作
- 嵌入式系統開發與韌體撰寫的基礎技能  

透過這些技能，可以在資源有限的環境中優化運算與儲存結構。

## Linked List 的三大特性

1. **動態性**
   - 支援資料量的動態增減，節點可以隨需求新增或刪除，不需要預先分配固定空間。
   - 適合用於資料數量無法預測的場景，例如即時資料處理。

2. **靈活性**
   - 節點的排列順序由指標決定，可隨意調整或重新排序。
   - 插入或刪除操作不需要大量搬移資料，比陣列更高效（特別是在中間進行插入或刪除時）。

3. **結構簡單，功能強大**
   - 基本結構僅需一個節點和一個指標，卻可以擴展為雙向鏈結串列、循環串列等進階形式，應用範圍廣泛。


# 二、Linked List 與 RTOS 的關係

## 為什麼 RTOS 會使用 Linked List？

在 RTOS（Real-Time Operating System，實時作業系統）中，Linked List 扮演重要角色，因為它提供了靈活且高效的方式來管理動態資源與任務排程：

1. **動態排程與任務管理**
   - RTOS 中的任務通常是動態的，任務數量和順序會隨時間變化。
   - Linked List 可以高效地管理任務列表，方便進行新增、刪除和查找操作。

2. **資源管理**
   - RTOS 需要管理有限的系統資源（如記憶體、I/O 裝置）。
   - Linked List 能夠實現資源的動態分配與釋放，避免浪費。

## 是否所有 RTOS 系統都需要 Linked List？

並非每個 RTOS 都必須使用 Linked List，這取決於系統的需求和設計複雜度：
- **需要 Linked List 的情況**：如果系統有複雜的任務管理或需要動態資源分配，Linked List 是極具優勢的選擇。
- **不需要 Linked List 的情況**：對於簡單的系統，若任務數量固定且資源管理較為靜態，可以使用其他資料結構或簡單排程機制替代。

# 三、Linked List 在嵌入式系統中的角色

## 當使用嵌入式系統編寫韌體時，Linked List 扮演著什麼角色？

在嵌入式系統中，Linked List 主要用來解決以下問題：

- **動態儲存資料**：在需要處理不定長度資料的情況下，Linked List 可以提供比靜態資料結構（如陣列）更靈活的儲存方式。
  
- **實現其他資料結構**：Linked List 是許多其他資料結構的基礎，例如 Queue（佇列）、Stack（堆疊）等。這些資料結構在處理資料流、管理工作隊列、或簡單的 LIFO/FIFO 操作中都非常有用。

- **提高資源使用效率**：在嵌入式系統中，記憶體可能是有限的。Linked List 可以幫助更高效地利用記憶體，並且提供動態的記憶體管理（例如，根據需要分配或釋放記憶體）。

## 我需要寫 Linked List 檔案嗎？沒有這個檔案會發生什麼事？

如果你的嵌入式專案中需要使用 Linked List 來處理動態資料或管理資源，那麼寫一個專門的 Linked List 檔案是必要的。這樣可以將 Linked List 的操作封裝在一個單獨的模組中，增強程式的可讀性、可維護性與重用性。

如果你沒有實作 Linked List，或者在專案中沒有使用合適的資料結構，可能會出現以下問題：

1. **資源管理不夠靈活**：如果需要動態分配記憶體或管理任務，沒有 Linked List，可能會依賴其他資料結構，這些結構可能無法提供足夠的靈活性來應對變化多端的需求。
   
2. **效能問題**：對於需要頻繁插入、刪除資料的情況，沒有 Linked List 可能需要手動處理資料重排或記憶體重新分配，這樣會導致效能降低，尤其是在記憶體有限的情況下。

## 這個檔案具體在做什麼事情？

Linked List 的 `.c` 和 `.h` 檔案中，主要包含了資料結構和操作函數的實作。具體來說，`LinkedList.c` 會實現建立節點、添加節點、刪除節點、列印列表等基本功能，而 `LinkedList.h` 則是提供這些操作函數的宣告。這樣的封裝使得程式在使用 Linked List 時更加簡單，也方便未來的擴展和維護。

# 四、單向鏈表（Singly Linked List）
## 1. 單向鏈表簡介
單向鏈表（Singly Linked List）是一種資料結構，它由一系列的節點（node）組成。每個節點包含兩個主要部分：**資料部分**和**指標部分**。資料部分存儲節點的值，而指標部分則指向鏈表中的下一個節點。如果是鏈表的最後一個節點，其指標部分會指向 `NULL`，表示鏈表的結束。

## 2. 單向鏈表節點的結構
在單向鏈表中，每個節點通常由兩個部分構成：

- **資料部分**：這部分儲存節點的值，可以是任意類型的資料，例如整數、字串或對象等。
- **指標部分**：這部分是指向鏈表中下一個節點的指標，保持鏈表結構的連貫性。對於最後一個節點，指標部分會指向 `NULL`，表示鏈表的結束。

這種設計結構是單向鏈表的基本形式，節點通常僅儲存一個值。如果需要，也可以儲存多個值或一個對象，但這會使鏈表結構變得更複雜，並且偏離單向鏈表的簡單設計。

## 3. 節點是否可以儲存多個值？
雖然每個節點通常只儲存一個值，但理論上也可以讓每個節點儲存多個值。例如，可以讓節點儲存一個數組或列表，或者儲存一個包含多個屬性的對象。然而，這樣的設計會使鏈表變得更加複雜，並且偏離單向鏈表的簡單和直觀的設計理念。因此，單向鏈表的節點一般儲存一個單一的值。

## 4. 指標部分的作用
在單向鏈表中，**指標部分**是鏈接節點的關鍵。它指向鏈表中的下一個節點，保持了鏈表的連貫性。例如，假設我們有一個包含三個節點的鏈表，節點的值分別為 `1`、`2` 和 `3`，其結構可表示為：
```r
[1 | next] -> [2 | next] -> [3 | next] -> NULL
```
在這個鏈表中：
- 節點1的 `next` 指標指向節點2。
- 節點2的 `next` 指標指向節點3。
- 節點3的 `next` 指標指向 `NULL`，表示鏈表的結束。

這樣的指標鏈接使得我們能夠按順序遍歷整個鏈表。

## 5. 程式碼範例：節點結構體定義

在程式實現中，單向鏈表的節點通常使用結構體來表示。以下是常見的結構體定義範例，這個結構體定義了每個節點的資料和指標部分：

```c
struct ListNode {
    int val;            // 資料部分，存儲節點的值
    struct ListNode *next;  // 指標部分，指向下一個節點
};
```
- **val**：這部分存儲節點的資料，通常是某種類型的數據，例如整數、字串等。
- **next**：這是一個指標，指向鏈表中的下一個節點。如果當前節點是鏈表的最後一個節點，那麼 `next` 指標會指向 `NULL`，表示鏈表結束。

這樣的設計使得每個節點都能夠與下一個節點相連接，並且能夠依序遍歷整個鏈表。通過 `next` 指標，鏈表中的每個節點都可以按順序訪問，直到遇到指向 `NULL` 的節點，即表示鏈表結束。

## 6. 單向鏈表的遍歷與結構

單向鏈表的遍歷通常從頭節點開始，依次訪問每個節點，直到遇到 `NULL`。這是因為每個節點的指標部分指向下一個節點，這樣就能夠實現順序遍歷。

例如，假設有一個單向鏈表，其結構為 `1 -> 2 -> 3 -> NULL`，我們可以依次訪問每個節點，並從 `next` 指標跳到下一個節點，直到我們到達 `NULL`。

總結來說，單向鏈表的設計簡單而高效，適合用於需要動態數據存儲和操作的情境。每個節點包含資料和指標兩個部分，這使得鏈表能夠靈活地擴展和連接，並支持順序訪問。

# 五、`.ld` 檔案與 Linked List 的區別

很多初學者容易把 `.ld` 檔案和 Linked List 混淆，因為它們在嵌入式系統中都很常見。但 `.ld` 檔案的實際作用與 Linked List 無關。

- **Linked List**：一種鏈狀資料結構，用來在程式中儲存和操作資料。
- **.ld 檔案**：也稱為 Linker Script，主要作用是告訴編譯器如何配置韌體在記憶體中的佈局。它定義了程式各部分（如代碼段、資料段）應該加載到 MCU 的哪個位置。

簡單來說，Linked List 是程式中操作的資料結構，而 `.ld` 是告訴系統如何載入程式的配置檔案。

# 六、如何在嵌入式專案中實作 Linked List？
## 步驟 1：建立 `LinkedList.h` 檔案

這個 `.h` 檔案會定義 Linked List 的節點結構（`struct ListNode`）和一些基本的操作函數宣告。

```c
// LinkedList.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct ListNode {
    int val;
    struct ListNode *next;
};

// 宣告 Linked List 基本操作函數
struct ListNode* createNode(int val);
void addNode(struct ListNode **head, int val);
void deleteNode(struct ListNode **head, int val);
void printList(struct ListNode *head);

#endif
```
## 步驟 2：建立 `LinkedList.c` 檔案

在這個 `.c` 檔案中，我們實作 `LinkedList.h` 中宣告的函數。這些函數負責 Linked List 的建立、插入、刪除和列印操作。

```c
// LinkedList.c
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

// 建立新節點
struct ListNode* createNode(int val) {
    struct ListNode *newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// 在 Linked List 開頭添加節點
void addNode(struct ListNode **head, int val) {
    struct ListNode *newNode = createNode(val);
    newNode->next = *head;
    *head = newNode;
}

// 刪除指定值的節點
void deleteNode(struct ListNode **head, int val) {
    struct ListNode *temp = *head, *prev = NULL;
    while (temp != NULL && temp->val != val) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) *head = temp->next;
    else prev->next = temp->next;
    free(temp);
}

// 列印整個 Linked List
void printList(struct ListNode *head) {
    struct ListNode *temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->val);
        temp = temp->next;
    }
    printf("NULL\n");
}
```
## 步驟 3：在 `main.c` 中使用 Linked List

現在，我們可以在 `main.c` 中包含 `LinkedList.h`，然後調用剛剛定義的函數，對 Linked List 進行操作。

```c
// main.c
#include "LinkedList.h"

int main() {
    struct ListNode *head = NULL;

    // 添加節點
    addNode(&head, 3);
    addNode(&head, 5);
    addNode(&head, 7);

    // 列印 Linked List
    printList(head);

    // 刪除節點
    deleteNode(&head, 5);
    printList(head);

    return 0;
}
```

## 使用 GCC 編譯專案
假設你已經建立了 main.c、LinkedList.c 和 LinkedList.h 檔案，使用 GCC 來編譯專案：
```bash
gcc main.c LinkedList.c -o main
```
執行編譯後的檔案，程式會根據 Linked List 的操作函數來處理資料。

# 七、小結
Linked List 在嵌入式韌體中有廣泛應用，特別在 RTOS 和動態資源管理中。建立 Linked List 的 `.c` 和 `.h` 檔案來定義和實作所需的功能，可以讓你的程式擁有靈活的資料管理能力。至於 `.ld` 檔案，它僅僅是用於記憶體配置，與 Linked List 的功能無關。因此，在專案中確保 `.ld` 和 Linked List 的區分，以便更清楚地管理韌體開發中的不同部分。

