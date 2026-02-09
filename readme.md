# 燕雲十六聲 把 KB接上電腦自由演奏的方法

![License](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)
![Node](https://img.shields.io/badge/Node.js-v20+-green?logo=node.js)
![Arduino](https://img.shields.io/badge/Hardware-Arduino_UNO_R4-blue?logo=arduino)

> [!IMPORTANT]
> 本專案僅供學術研究與個人娛樂，**嚴禁商業使用**。

遇到哪邊出錯 請把錯誤碼保留 可以私訊發 Issue 給我或者用任何能讓我看得的方式聯繫我

歡迎有其他建議或想法的朋友們提供我修改建議

有那裡寫不好的地方歡迎私訊或者發 Issue 給我

只支援 windows 系統 PS5 手機平板不支援

本來只是自用，沒想到有那麼多人想要，程式沒設計 UI 介面，請多多包涵，如果真的很多人需要，我再加上
---

## 第一步：準備硬體

在開始之前，請確認你手邊有這些東西：

| 物品 | 說明 |
| :--- | :--- |
| **電子琴** | 必須有 USB 孔，並支援 MIDI 功能。 |
| **Arduino UNO R4 WiFi與外殼** | 作為訊號轉換器，Arduino 有很多型號別，千萬別買錯了!!!，R3跟 R4外殼通用，買 R3外殼也可以。|
| **傳輸線 x2** | 一條接琴，琴的輸出口看是哪種，通常是 USB-Type-A/USB-Type-B to USB(type-A) (接到電腦)，一條接 Arduino 一定是 Type-C to USB(Type-A) (接到電腦)。**USB-Type-A是最常見的 USB 扁扁的那種,Type-B是常常插印表機上面長像較為正方形的** |
| **Windows 電腦** | 目前僅支援 Windows 系統。 |

> [!WARNING]
> **強烈建議購買「Arduino UNO R4 專用透明壓克力外殼」！**
>
> **原因：** Arduino 底部的針腳非常脆弱且帶電。如果不裝殼，放在桌上若不小心碰到金屬物品（如硬幣、鍵盤金屬邊框），會導致**瞬間短路燒毀**。幾十塊的殼可以保護你好幾百塊的板子，非常划算！
**而且我有寫彈什麼音會顯示在 Arduino 上，不用透明外殼很可惜**。

---

## 第二步：安裝必要軟體

請依照順序下載並安裝以下軟體（一直按 Next 下一步即可）：

### 1. 安裝 Node.js
- **下載連結：** [Node.js 官方網站](https://nodejs.org/)
- **下載版本：請下載 **LTS 版本** 建議 v20 以上，大版號千萬注意要是偶數版本(EX: 20.X.X，20這裡一定要是偶數)。
- **安裝時：** 全部預設勾選，一直按下一步直到完成。
- **測試是否安裝成功：** 請在終端機輸入(CMD/PowerShell/Terminal/命令提示字元) `node -v` 檢查是否安裝成功。
- **為什麼要安裝：** 不需要會寫JS，只是為了用他而安裝，跟部分程或遊戲式會叫你安裝 .net/C++ .....等等的道理類似。

### 2. 安裝 Arduino IDE
這是要把程式寫入 Arduino 板子的工具。
- **下載連結：** [Arduino Software](https://www.arduino.cc/en/software)
- **下載版本：** "請下載 Windows 64bit 版本，除非你的電腦非常老舊(但那麼老的電腦，燕雲十六聲應該也玩不起來)"。

---

## 第三步：燒錄 Arduino (只要做一次)

1. 把 **Arduino** 裝進壓克力殼，插上電腦，壓克力殼外面有保護指，記得撕開，他真的有點難撕，別懷疑，他就是要撕掉。
2. 打開 **Arduino IDE**。
3. 點選左上角的 `File` -> `Open`，找到本專案資料夾裡的 `UNOR4/UNOR4.ino` 檔案並打開。
4. 在 IDE 上方選單，選擇你的開發板：
   - 選擇 **Arduino UNO R4 WiFi**。
   - 一定要手動選擇，因為 Port 有時候會不一樣。
5. 點擊左上角的 **➡️ (上傳)** 按鈕，他長像就是往右的箭頭。
6. 等待下方顯示 `Done uploading`，Arduino 上的 LED 矩陣出現愛心，就成功了！如果這邊報錯可以跟我說，記得把**錯誤碼**保留下來給我。

---

## 第四步：綁定你的電子琴 (重要！)

因為每台琴在電腦裡的名字不一定一樣，**第一次使用前**，你需要告訴程式你的琴叫什麼名字。
我當初本來是要自己用的，完全沒想到大家反響熱烈，所以沒特別寫自動偵測,或者選擇的介面。

1. **開啟終端機：**
   - 進入本專案資料夾。
   - 在空白按下**滑鼠右鍵**。
   - 選擇 開啟PowerShell視窗/Open in Terminal/在終端機中開啟/命令提示字元開啟/其他能開啟終端機的按鈕。

2. **安裝工具：**
   輸入以下指令並按 <kbd>Enter</kbd>

   ```bash
   npm i
   ```

跑完後 **確保你的琴是開機的**，再輸入以下指令並按 <kbd>Enter</kbd>

   ```Bash
      node -e "console.log(require('easymidi').getInputs())"
   ```

你會看到類似這樣的結果： [ '2- Digital Keyboard-1' ]

修改程式碼：

複製剛剛看到的琴名[ '**裡面這串所有字都要複製不要管是不是符號或空白**' ]。

用 記事本 或 VS Code 打開資料夾裡的 index.js。

找到大約第 5 行：

```js
const DEVICE_NAME = "2- Digital Keyboard-1"; 
```
把引號裡的字(2- Digital Keyboard-1)，換成你的琴名。

存檔 (Ctrl + S)。

## 第五步：啟動程式 (每次玩遊戲前做這個)
確認琴和 Arduino 都插好了。

在終端機輸入：

```Bash
node index.js
```
看到螢幕顯示 🔌 連線建立成功！ 恭喜你，可以打開遊戲開始玩囉!!!
在進入遊戲之前，可以先找個能打字的地方，按琴鍵的中央C附近位置，測試看看有沒有字跑出來，測試是否有連線成功。

常見問題故障排除
<details> <summary>錯誤：COM Port Busy / 上傳失敗</summary>

原因： 你的 Node.js 程式還在執行，佔用了 Arduino 的線路。 解法：

回到黑色視窗 (終端機)。

按下 <kbd>Ctrl</kbd> + <kbd>C</kbd> 停止程式。

拔掉 Arduino USB 線，重插。

再試一次上傳。

</details>

<details> <summary>錯誤：Cannot find module '...'</summary>

原因： 你忘記安裝依賴套件了。 解法： 請確認你有在專案資料夾內執行過 npm install。

</details>

<details> <summary>按了琴鍵，遊戲沒反應？</summary>

檢查 Arduino 上面有沒有顯示音名 (如 C, D, E)？

沒有顯示： 電子琴沒連上，請重啟 node index.js。

有顯示但遊戲不動： 檢查輸入法是否為英文？遊戲視窗是否置頂？

**有其他問題可以跟我說**

</details>

[!IMPORTANT] 免責聲明： 本專案僅供個人學習與娛樂使用。請勿用於任何商業用途。 若有違反部分遊戲的使用條款，請自行承擔風險。