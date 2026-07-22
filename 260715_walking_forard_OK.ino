#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- サーボ換算用パラメータ ---
const float DEG_TO_PCA = 2.27; // 1度あたりのPCAカウント変化量
const int   PCA_CENTER = 325;  // 90度（付け根のセンター）のPCAカウント値

// 💡 完璧に直った各ひざサーボの初期角度（今回はここから一切動きません！）
const float FR_KNEE_INIT = 180.0; // 1番ピン（前右）
const float FL_KNEE_INIT = 00.0;  // 3番ピン（前左）
const float BR_KNEE_INIT = 00.0;  // 5番ピン（後右）
const float BL_KNEE_INIT = 180.0; // 7番ピン（後左）

// --- 歩行モーションのパラメータ ---
const float PERIOD = 1500.0;     // 周期
const float SWING_AMP = 20.0;   // 付け根の振幅:20度の方がいい
// 12度はテストモード

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n--- 8-Servo: Swing Only Mode ---");

  Wire.begin(21, 22); 
  pwm.begin();
  pwm.setPWMFreq(50); 

  // 🛠️ 起動直後の姿勢でスタンバイ
  moveServo(0, 0.0, 90.0);       // 0番：前右・付け根
  moveServo(1, 0.0, FR_KNEE_INIT); // 1番：前右・ひざ
  
  moveServo(2, 0.0, 90.0);       // 2番：前左・付け根
  moveServo(3, 0.0, FL_KNEE_INIT); // 3番：前左・ひざ
  
  moveServo(4, 0.0, 90.0);       // 4番：後右・付け根
  moveServo(5, 0.0, BR_KNEE_INIT); // 5番：後右・ひざ
  
  moveServo(6, 0.0, 90.0);       // 6番：後左・付け根
  moveServo(7, 0.0, BL_KNEE_INIT); // 7番：後左・ひざ
  
  delay(1000);
}

void loop() {
  unsigned long current_time = millis();
  
  // 現在の時間からベースとなる「位相（ラジアン）」を計算（0 〜 2*PI）
  float phase = 2.0 * PI * ((float)(current_time % (unsigned long)PERIOD) / PERIOD);

  // ----------------------------------------------------
  // 💡 付け根（肩）の前後スイング波を計算
  // ----------------------------------------------------
  // ペアA（前右・後左）の前後スイング
  float swing_A = SWING_AMP * cos(phase);

  // ペアB（前左・後右）の前後スイング（180度ずらして「あべこべ」にする）
  float phase_B = phase + PI;
  float swing_B = SWING_AMP * cos(phase_B);

  // ----------------------------------------------------
  // 🛠️ PCA9685へ分配
  // ----------------------------------------------------
  
  // ■ 前右（ペアA）
  moveServo(0,  swing_A, 90.0);         // 付け根（スイングする）
  moveServo(1,  0.0,     FR_KNEE_INIT); // ★ひざはコサイン波なし（0.0度固定）

  // ■ 前左（ペアB）
  moveServo(2, -swing_B, 90.0);         // 付け根
  moveServo(3,  0.0,     FL_KNEE_INIT); // ★ひざはコサイン波なし（0.0度固定）

  // ■ 後右（ペアB）
  moveServo(4,  swing_B, 90.0);         // 付け根
  moveServo(5,  0.0,     BR_KNEE_INIT); // ★ひざはコサイン波なし（0.0度固定）

  // ■ 後左（ペアA）
  moveServo(6, -swing_A, 90.0);         // 付け根
  moveServo(7,  0.0,     BL_KNEE_INIT); // ★ひざはコサイン波なし（0.0度固定）

  delay(15);
}

/**
 * サーボを任意の角度に動かす関数
 */
void moveServo(int ch, float offset_deg, float base_deg) {
  float total_deg = base_deg + offset_deg;
  int pca_value = PCA_CENTER + (int)((total_deg - 90.0) * DEG_TO_PCA);
  
  if (pca_value < 150) pca_value = 150;
  if (pca_value > 500) pca_value = 500;

  pwm.setPWM(ch, 0, pca_value);
}
