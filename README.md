# Quadrupe Robot
四足歩行ロボット開発プロジェクト
<img width="832" height="685" alt="image" src="https://github.com/user-attachments/assets/b668775c-62b6-4a0a-a9b0-e35090414820" />
<img width="1125" height="2436" alt="image" src="https://github.com/user-attachments/assets/ca339d41-310a-48ac-83cf-2b29f46cc03a" />
## 使用部品
- ESP32
- SG90サーボ
## 目標
自律歩行ロボットを作る
## 参考サイト
ESP32を題材にした学習サイト(Eng)
https://esp32.implrust.com

# プログラムの使用方法
## ESP32MacADDRcheck
ESP32のMACアドレスを取得するためのプログラム。
### 注意
- シリアルモニターを開いて出力を確認します。出力の中にアドレス情報が出力される。
- シリアルモニターを開いた後にesp 32ボードのリセットボタンを押す。
- そうしないと表示されない。
## PS4Cont_Ctrl
PS4コントローラでロボットを動かすためにコントローラとESP32をBluetooth接続する。
### 注意
- ボードライブラリ2.0.14で接続の実績があります。IDEでライブラリバージョンを2.0.14にして書き込んでください。
- ver3ではPS4コントローラで接続ができませんでした。
- PS4ControllerライブラリがESP32 Core v2.xという古いOSの仕組み（Bluetoothの命令の出し方）に合わせて作られていることが理由と思われます。
### 使用方法
- ESP32に電源を供給したのちにPS4コントローラのPSボタンを押す。
- 接続されるのでスティックの上下左右で動かす。
