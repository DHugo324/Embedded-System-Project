# Embedded_System_Final

### Introduction

兩個玩家，每一個玩家會有五個骰子和三條命。骰子的值會由程式隨機生成，確認玩數字後，兩位玩家會輪流叫喊「X 個 Y」，即代表兩個玩家的十個骰字總共有 X 個 Y，X 必須要大於等於前一個人喊的 X，若 X 等於前一個人所喊，則 Y 必須比前一個 Y 大；若 X 大於前一個人所喊，則 Y 不需要比前一個大。只要有一方不信任另一位玩家叫喊的值，他就可以喊停，這時就會計算兩人的骰子值，並看是否有滿足「X 個 Y」，如果有的話，叫喊「X 個 Y」的人贏；反之，喊停的人贏。勝方不會扣生命值，但敗方會扣一。

### Description

- ESP32
    - UART：接收 8051 開發板傳來的資料（猜的數字、喊停、準備狀態等）、傳送資料給 8051 開發板（遊戲狀態、對手猜的數字、剩餘生命值等）。
    - MQTT：傳送遊戲狀態至 adafruit 上面的 feed，以及接收期更新的狀態。
    - Firebase：儲存玩家一和二的戰績結果。
- 8051
    - 喇叭：遊戲提示聲、勝利音樂、戰敗音樂。
    - 七段顯示器：顯示玩家的骰子值、對手的猜測、自己的輸入、每一輪結束後的生命值。
    - 矩陣鍵盤：輸入欲猜測的值、遊戲準備、狀態確認。
    - UART：傳送資料給 ESP32（猜的數字、喊停、準備狀態等）、接收 ESP32 傳來的資料（遊戲狀態、對手猜的數字、剩餘生命值等）。

### Operation Flow

- 遊戲進行需要兩個玩家都進行準備，按下準備按鈕後，8051 開發板會先透過 UART 傳送資料給ESP32，ESP32 再將狀態透過 MQTT 更新到 adafruit 上。
- 若兩人都準備完畢，ESP32 會透過 UART 通知 8051 遊戲開始並傳送隨機產生的五個數字，8051 開發板會顯示這串數字在數碼管上。接下來一樣需要等待玩家確認（流程如上）。
- 兩人都確認完畢後即由玩家一先使用矩陣鍵盤開始猜數字，猜完後會透過 UART 傳給 ESP32、ESP32 透過 MQTT 更新到 adafruit、玩家二那邊的 ESP32 接收後透過 UART 傳給 8051 開發板，此時，玩家二會看到對方猜的數字，並且可以開始猜或是喊停。（重複此段落步驟直到有人喊停才會進入下一段落）
- 喊停的那方會透過 UART 傳送喊停的訊息給 ESP32、ESP32 再透過 MQTT 傳送給另一玩家讓另一玩家知道。傳送完後即開始計算此輪的勝負，計算完畢後再將結果透過 UART 傳回 8051 開發板並顯示遊戲結果。（若雙方生命皆未歸零，回到第二段落；反之，回到第一段落，即可以一直繼續遊戲。）

### Usage

- ESP32
  - 須將 xtools.py, utime.py, urequests.py, sound.py 上傳到 ESP32 裝置上，上傳完畢後即可開始使用
  - player1.py 或 player2.py。請切記一個玩家使用一個程式，請勿兩個都使用同一份。
- 8051
  - main.c 會引入 delay.h, keyscan.h, uart.h，函式定義請見 delay.c, keyscan.c, uart.c。
  - 矩陣鍵盤輸入，按下1-10，代表1-10，13鍵位在GUESS階段時可以切換顯示的內容，16鍵位為確認/叫停的按鈕