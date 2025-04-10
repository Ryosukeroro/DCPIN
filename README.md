# DCPIN
DCPINは、岩根亮介の技術をすべて詰め込んだ究極の岩根亮介専用機体です。
移動機構として四輪オムニホイール機構(自作)を採用しています。

# 機体図
<img src="https://github.com/user-attachments/assets/d64a460b-2bc1-42fc-97da-57b8c8728361" width="500">


# 機体詳細
オムニホイールは45度配置となっています。
DCモーターを採用しています。今はなきこちら→https://x.com/suzakulabshop/status/1243741995413254146

# 岩根亮介専用機体_DCPIN_制御概要
rosserialを使ってROSとArduinoを通信しています。
Arduino_Unoで各ホイールの速度制御を行っています。速度制御は目標速度に対して、FF+FB制御(PID制御)を行っております。
速度情報取得のため、エンコーダーを使用しています。エンコーダーはこちら→https://www.digikey.jp/ja/products/detail/same-sky-formerly-cui-devices/AMT102-V/827015
エンコーダーの分解能は256にしていました。

# rosserialの使い方
rosserialの使い方はこちら→https://qiita.com/ryosukeroro/items/99f0151eece430890b44
